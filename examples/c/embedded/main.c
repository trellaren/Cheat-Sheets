// ============================================================
// Simple Embedded Application in C
// ============================================================
// Compile:  gcc -std=c11 -Wall -Wextra -pedantic -o embedded main.c
// Run:      ./embedded
//
// _POSIX_C_SOURCE enables POSIX extensions (sigaction, struct sigaction).
// This must appear before any system header is included.
#define _POSIX_C_SOURCE 200809L

// This file demonstrates patterns found in real embedded / bare-metal C:
//
//   1. MEMORY-MAPPED REGISTERS  – volatile structs that mirror hardware
//   2. BIT MANIPULATION         – setting / clearing / toggling bits
//   3. GPIO DRIVER              – LED and button abstraction
//   4. UART DRIVER              – serial transmit / receive ring buffer
//   5. CIRCULAR (RING) BUFFER   – lock-free single-producer / consumer
//   6. FINITE STATE MACHINE     – event-driven traffic-light controller
//   7. SYSTEM TICK & TIMERS     – software timers built on a tick counter
//   8. INTERRUPT SIMULATION     – signal handler as a stand-in for an ISR
//
// On a real microcontroller (e.g. STM32, AVR):
//   • Peripheral base addresses replace the local structs below.
//   • The "simulated tick" is driven by a hardware SysTick or timer ISR.
//   • The UART ring buffer is filled inside the USART receive ISR.
//
// All I/O here is to stdout so the demo runs on any host machine.
// ============================================================

#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// SECTION 1: MEMORY-MAPPED REGISTER SIMULATION
// ============================================================
// On a real MCU the GPIO peripheral lives at a fixed address, e.g.:
//   #define GPIOA_BASE  0x40020000UL
//   #define GPIOA       ((GpioRegs *)GPIOA_BASE)
//
// Here we use a local variable instead so the example compiles on a PC.
//
// volatile tells the compiler: "DO NOT cache or reorder these accesses."
// This is critical for hardware registers because the hardware changes
// the register value without the CPU writing it.

typedef volatile struct {
    uint32_t MODER;    // mode register    (input / output / alternate / analog)
    uint32_t OTYPER;   // output type      (push-pull / open-drain)
    uint32_t OSPEEDR;  // output speed
    uint32_t PUPDR;    // pull-up/pull-down register
    uint32_t IDR;      // input data register   (read-only in HW)
    uint32_t ODR;      // output data register  (read/write)
    uint32_t BSRR;     // bit set/reset register (write-only in HW)
    uint32_t LCKR;     // lock register
} GpioRegs;

// Simulated GPIO peripheral for port A.
static GpioRegs gpio_a_sim = {0};
#define GPIOA (&gpio_a_sim)

// ============================================================
// SECTION 2: BIT MANIPULATION MACROS
// ============================================================
// These macros are ubiquitous in embedded C.  They operate on any
// integer type and avoid magic numbers by naming each bit position.

#define BIT(n)              (1UL << (n))
#define SET_BIT(reg, mask)  ((reg) |=  (mask))
#define CLR_BIT(reg, mask)  ((reg) &= ~(mask))
#define TOG_BIT(reg, mask)  ((reg) ^=  (mask))
#define READ_BIT(reg, mask) ((reg) &   (mask))

// GPIO MODER field: 2 bits per pin.
#define GPIO_MODER_INPUT(pin)  (0x0UL << ((pin) * 2))
#define GPIO_MODER_OUTPUT(pin) (0x1UL << ((pin) * 2))

// Pin numbers for our simulated board.
#define LED_PIN    5   // PA5 → on-board LED
#define BUTTON_PIN 13  // PC13 → user button (read from IDR here)

static void bit_manipulation_demo(void) {
    printf("\n== Section 2: Bit Manipulation ==\n");

    uint32_t reg = 0x00;
    SET_BIT(reg, BIT(3));
    SET_BIT(reg, BIT(7));
    printf("after SET bits 3,7:  0x%08X\n", reg);

    CLR_BIT(reg, BIT(3));
    printf("after CLR bit 3:     0x%08X\n", reg);

    TOG_BIT(reg, BIT(7));
    TOG_BIT(reg, BIT(0));
    printf("after TOG bits 7,0:  0x%08X\n", reg);

    printf("READ bit 0: %lu\n", READ_BIT(reg, BIT(0)));
}

// ============================================================
// SECTION 3: GPIO DRIVER
// ============================================================

// Configure a pin as output.
static void gpio_set_output(GpioRegs *port, uint8_t pin) {
    // Clear the 2-bit MODER field for this pin, then set to OUTPUT (0b01).
    CLR_BIT(port->MODER, 0x3UL << (pin * 2));
    SET_BIT(port->MODER, GPIO_MODER_OUTPUT(pin));
}

// Configure a pin as input.
static void gpio_set_input(GpioRegs *port, uint8_t pin) {
    CLR_BIT(port->MODER, 0x3UL << (pin * 2));
    SET_BIT(port->MODER, GPIO_MODER_INPUT(pin));
}

// Set output pin HIGH.
static void gpio_write_high(GpioRegs *port, uint8_t pin) {
    // BSRR lower 16 bits set the pin; upper 16 bits clear it.
    port->BSRR = BIT(pin);
    SET_BIT(port->ODR, BIT(pin));  // keep ODR in sync for readback
}

// Set output pin LOW.
static void gpio_write_low(GpioRegs *port, uint8_t pin) {
    port->BSRR = BIT(pin + 16);
    CLR_BIT(port->ODR, BIT(pin));
}

// Read digital input value (0 or 1).
static uint8_t gpio_read(const GpioRegs *port, uint8_t pin) {
    return (uint8_t)((port->IDR >> pin) & 0x1U);
}

// Toggle an output pin.
static void gpio_toggle(GpioRegs *port, uint8_t pin) {
    TOG_BIT(port->ODR, BIT(pin));
}

static void gpio_demo(void) {
    printf("\n== Section 3: GPIO Driver ==\n");

    gpio_set_output(GPIOA, LED_PIN);
    gpio_set_input(GPIOA,  BUTTON_PIN);

    printf("LED off – ODR=0x%08X\n", GPIOA->ODR);

    gpio_write_high(GPIOA, LED_PIN);
    printf("LED on  – ODR=0x%08X\n", GPIOA->ODR);

    gpio_write_low(GPIOA, LED_PIN);
    printf("LED low – ODR=0x%08X\n", GPIOA->ODR);

    gpio_toggle(GPIOA, LED_PIN);
    printf("LED tog – ODR=0x%08X\n", GPIOA->ODR);

    // Simulate the button being pressed (set IDR bit externally).
    SET_BIT(GPIOA->IDR, BIT(BUTTON_PIN));
    printf("button pressed: %d\n", gpio_read(GPIOA, BUTTON_PIN));
    CLR_BIT(GPIOA->IDR, BIT(BUTTON_PIN));
    printf("button released: %d\n", gpio_read(GPIOA, BUTTON_PIN));
}

// ============================================================
// SECTION 4: CIRCULAR (RING) BUFFER
// ============================================================
// A ring buffer is the backbone of UART drivers: the ISR writes into
// the buffer; the main loop reads from it without blocking.
//
// Head (write pointer) and tail (read pointer) never exceed the buffer
// size because they are masked with (CAPACITY - 1) on every access.
// CAPACITY must be a power of two for this masking trick to work.

#define RING_BUF_SIZE 16  // must be a power of 2

typedef struct {
    uint8_t  data[RING_BUF_SIZE];
    uint32_t head;  // next write position (producer)
    uint32_t tail;  // next read  position (consumer)
} RingBuffer;

static void ring_init(RingBuffer *rb) {
    memset(rb->data, 0, sizeof(rb->data));
    rb->head = rb->tail = 0;
}

static bool ring_full(const RingBuffer *rb) {
    return (rb->head - rb->tail) >= RING_BUF_SIZE;
}

static bool ring_empty(const RingBuffer *rb) {
    return rb->head == rb->tail;
}

static size_t ring_count(const RingBuffer *rb) {
    return rb->head - rb->tail;
}

// Write one byte.  Returns false if the buffer is full.
static bool ring_push(RingBuffer *rb, uint8_t byte) {
    if (ring_full(rb)) return false;
    rb->data[rb->head & (RING_BUF_SIZE - 1)] = byte;
    rb->head++;
    return true;
}

// Read one byte.  Returns false if the buffer is empty.
static bool ring_pop(RingBuffer *rb, uint8_t *out) {
    if (ring_empty(rb)) return false;
    *out = rb->data[rb->tail & (RING_BUF_SIZE - 1)];
    rb->tail++;
    return true;
}

// ============================================================
// SECTION 5: UART DRIVER (simulated)
// ============================================================
// A real UART driver uses the ring buffer above.
// Here we simulate the ISR by pushing bytes directly into the RX buffer.

static RingBuffer uart_rx_buf;
static RingBuffer uart_tx_buf;

// Called from the USART receive ISR on a real MCU.
// Here we call it manually to simulate incoming serial data.
static void uart_rx_isr_sim(uint8_t byte) {
    if (!ring_push(&uart_rx_buf, byte)) {
        // Buffer overflow – in real firmware you might set an error flag.
        fprintf(stderr, "[UART] rx overflow, byte 0x%02X dropped\n", byte);
    }
}

// Transmit a string: queue each character into the TX ring buffer.
static void uart_send_string(const char *s) {
    while (*s) {
        // In real firmware we would enable the TXEIE interrupt here.
        if (!ring_push(&uart_tx_buf, (uint8_t)*s)) {
            fprintf(stderr, "[UART] tx overflow\n");
            break;
        }
        s++;
    }
}

// Flush the TX buffer to stdout (simulates the USART TX ISR draining it).
static void uart_flush_tx(void) {
    uint8_t b;
    printf("[UART TX] \"");
    while (ring_pop(&uart_tx_buf, &b)) {
        putchar(b);
    }
    printf("\"\n");
}

static void uart_demo(void) {
    printf("\n== Section 5: UART Driver (simulated) ==\n");

    ring_init(&uart_rx_buf);
    ring_init(&uart_tx_buf);

    // Simulate receiving bytes as if they came via hardware interrupt.
    const char *incoming = "AT+VERSION\r\n";
    for (const char *p = incoming; *p; p++) {
        uart_rx_isr_sim((uint8_t)*p);
    }

    printf("[UART RX] received %zu bytes: \"", ring_count(&uart_rx_buf));
    uint8_t b;
    while (ring_pop(&uart_rx_buf, &b)) {
        if (b == '\r') printf("\\r");
        else if (b == '\n') printf("\\n");
        else putchar(b);
    }
    printf("\"\n");

    uart_send_string("OK\r\n");
    uart_send_string("v1.0.0\r\n");
    uart_flush_tx();
}

// ============================================================
// SECTION 6: FINITE STATE MACHINE
// ============================================================
// A traffic-light controller implemented as an FSM.
// FSMs are one of the most common patterns in embedded firmware.

typedef enum {
    STATE_RED,       // stop:  RED on,   AMBER off, GREEN off
    STATE_RED_AMBER, // ready: RED on,   AMBER on,  GREEN off
    STATE_GREEN,     // go:    RED off,  AMBER off, GREEN on
    STATE_AMBER      // caution: RED off, AMBER on,  GREEN off
} TrafficState;

typedef enum {
    EVENT_TIMER,    // periodic tick event
    EVENT_EMERGENCY // emergency vehicle preempt
} TrafficEvent;

typedef struct {
    TrafficState state;
    uint32_t     ticks_in_state;
} TrafficLight;

static const char *state_name(TrafficState s) {
    switch (s) {
        case STATE_RED:       return "RED";
        case STATE_RED_AMBER: return "RED+AMBER";
        case STATE_GREEN:     return "GREEN";
        case STATE_AMBER:     return "AMBER";
        default:              return "UNKNOWN";
    }
}

// Transition table: (current state, event) → next state.
static TrafficState fsm_next(TrafficState current, TrafficEvent ev) {
    if (ev == EVENT_EMERGENCY) return STATE_RED;

    switch (current) {
        case STATE_RED:       return STATE_RED_AMBER;
        case STATE_RED_AMBER: return STATE_GREEN;
        case STATE_GREEN:     return STATE_AMBER;
        case STATE_AMBER:     return STATE_RED;
        default:              return STATE_RED;
    }
}

// Entry action for each state (called when entering the state).
static void fsm_on_enter(TrafficState s) {
    printf("  → entering %s\n", state_name(s));
}

static void fsm_step(TrafficLight *tl, TrafficEvent ev) {
    TrafficState next = fsm_next(tl->state, ev);
    if (next != tl->state) {
        tl->state          = next;
        tl->ticks_in_state = 0;
        fsm_on_enter(next);
    } else {
        tl->ticks_in_state++;
    }
}

static void fsm_demo(void) {
    printf("\n== Section 6: Finite State Machine (traffic light) ==\n");

    TrafficLight tl = {.state = STATE_RED, .ticks_in_state = 0};
    fsm_on_enter(tl.state);

    // Simulate 5 normal timer ticks.
    for (int i = 0; i < 5; i++) {
        printf("tick %d: ", i + 1);
        fsm_step(&tl, EVENT_TIMER);
    }

    // Emergency vehicle preempts the cycle.
    printf("EMERGENCY! ");
    fsm_step(&tl, EVENT_EMERGENCY);

    // Resume normal cycle.
    for (int i = 0; i < 3; i++) {
        printf("tick %d: ", i + 6);
        fsm_step(&tl, EVENT_TIMER);
    }
}

// ============================================================
// SECTION 7: SYSTEM TICK & SOFTWARE TIMERS
// ============================================================
// In bare-metal firmware a hardware timer ISR increments a tick counter
// every 1 ms.  Software timers compare their deadline against the counter.

// On a real MCU:  volatile uint32_t g_tick_ms = 0;
// The ISR does:   g_tick_ms++;
// Here we advance it manually.

static volatile uint32_t g_tick_ms = 0;

// Advance the tick counter (replaces the hardware ISR in this demo).
static void sys_tick_advance(uint32_t ms) {
    g_tick_ms += ms;
}

static uint32_t sys_tick_get(void) {
    return g_tick_ms;
}

// Software timer: fires when (now - start) >= period_ms.
typedef struct {
    uint32_t start_ms;
    uint32_t period_ms;
    bool     running;
} SoftTimer;

static void timer_start(SoftTimer *t, uint32_t period_ms) {
    t->start_ms  = sys_tick_get();
    t->period_ms = period_ms;
    t->running   = true;
}

static bool timer_expired(SoftTimer *t) {
    if (!t->running) return false;
    return (sys_tick_get() - t->start_ms) >= t->period_ms;
}

static void timer_reset(SoftTimer *t) {
    t->start_ms = sys_tick_get();
}

static void timer_demo(void) {
    printf("\n== Section 7: System Tick & Software Timers ==\n");

    SoftTimer blink_timer  = {0};
    SoftTimer status_timer = {0};

    timer_start(&blink_timer,  500);   // blink LED every 500 ms
    timer_start(&status_timer, 1500);  // print status every 1500 ms

    printf("simulating 3000 ms of firmware loop...\n");
    for (uint32_t tick = 0; tick <= 3000; tick += 100) {
        sys_tick_advance(100);  // simulate 100 ms passing per iteration

        if (timer_expired(&blink_timer)) {
            gpio_toggle(GPIOA, LED_PIN);
            printf("  [t=%4u ms] LED blink, ODR=0x%08X\n",
                   sys_tick_get(), GPIOA->ODR);
            timer_reset(&blink_timer);
        }

        if (timer_expired(&status_timer)) {
            printf("  [t=%4u ms] status heartbeat\n", sys_tick_get());
            timer_reset(&status_timer);
        }
    }
}

// ============================================================
// SECTION 8: INTERRUPT SIMULATION (POSIX signals as ISR proxy)
// ============================================================
// On a real MCU an ISR (Interrupt Service Routine) is a function
// called automatically by the CPU when a hardware event occurs.
//
// Rules for ISRs:
//   • Keep them SHORT – do the minimum work, set a flag, leave the rest
//     to the main loop.
//   • Variables shared between an ISR and the main loop MUST be volatile.
//   • Use atomic types or critical sections to prevent data races.
//
// Here we use SIGALRM as a stand-in.  The signal handler mimics an ISR.

static volatile sig_atomic_t g_isr_flag = 0;  // set by ISR, cleared by main

static void simulated_isr(int signum) {
    (void)signum;
    // In a real ISR: read a hardware register, push to ring buffer, set flag.
    g_isr_flag = 1;
}

static void interrupt_demo(void) {
    printf("\n== Section 8: Interrupt Simulation ==\n");

    // Install the signal handler (replaces a real IRQ handler registration).
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = simulated_isr;
    sigaction(SIGINT, &sa, NULL);  // use SIGINT so we can raise it manually

    printf("raising SIGINT to simulate hardware interrupt...\n");
    raise(SIGINT);  // trigger the signal synchronously

    if (g_isr_flag) {
        printf("  ISR fired: g_isr_flag=%d – main loop processing event\n",
               (int)g_isr_flag);
        g_isr_flag = 0;  // acknowledge
    }

    // Restore default SIGINT handler so Ctrl-C works normally again.
    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);

    printf("  ISR processed and flag cleared: g_isr_flag=%d\n",
           (int)g_isr_flag);
}

// ============================================================
// MAIN – simulated firmware entry point
// ============================================================
// On a real MCU main() is called after the startup code initialises
// .bss (zeroed data) and .data (pre-initialised globals) sections and
// the stack pointer.  It normally never returns.

int main(void) {
    printf("=== Embedded Application Demo ===\n");
    printf("Simulating a bare-metal firmware stack on host hardware.\n");

    bit_manipulation_demo();
    gpio_demo();
    uart_demo();
    fsm_demo();
    timer_demo();
    interrupt_demo();

    printf("\n=== Embedded demo complete ===\n");
    return 0;
}
