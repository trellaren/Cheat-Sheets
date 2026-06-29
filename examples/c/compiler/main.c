// ============================================================
// Simple Expression Compiler in C
// ============================================================
// Compile:  gcc -std=c11 -Wall -Wextra -pedantic -o compiler main.c
// Run:      ./compiler
//
// This file implements a minimal compiler pipeline for arithmetic
// expressions.  The pipeline has three classic stages:
//
//   1. LEXER  (lexical analysis / tokenisation)
//      Converts a raw input string into a sequence of tokens.
//      e.g. "3 + 4 * (2 - 1)"  →  NUM(3) PLUS NUM(4) STAR LPAREN …
//
//   2. PARSER  (syntactic analysis)
//      Reads the token stream and builds an Abstract Syntax Tree (AST).
//      Respects operator precedence (PEMDAS/BODMAS):
//        *  /   (higher precedence)
//        +  -   (lower  precedence)
//
//   3. EVALUATOR  (tree walk / code generation)
//      Walks the AST and evaluates the expression to produce a result.
//
// Supported grammar (recursive-descent):
//
//   expr   → term   ( ( '+' | '-' ) term   )*
//   term   → factor ( ( '*' | '/' ) factor )*
//   factor → NUMBER | '(' expr ')'
//
// This deliberately stays simple so every line can be understood.
// Real compilers add symbol tables, type checking, IR generation, etc.
// ============================================================

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// TOKEN TYPES
// ============================================================
// An enum gives each token kind a readable name.

typedef enum {
    TOK_NUM,      // a numeric literal,  e.g. 42 or 3.14
    TOK_PLUS,     // +
    TOK_MINUS,    // -
    TOK_STAR,     // *
    TOK_SLASH,    // /
    TOK_LPAREN,   // (
    TOK_RPAREN,   // )
    TOK_EOF       // end of input
} TokenKind;

// A Token carries its kind and, for numeric tokens, its value.
typedef struct {
    TokenKind kind;
    double    value;  // only meaningful for TOK_NUM
} Token;

// Helper: return the printable name of a token kind (for debugging).
static const char *token_kind_name(TokenKind kind) {
    switch (kind) {
        case TOK_NUM:    return "NUM";
        case TOK_PLUS:   return "PLUS";
        case TOK_MINUS:  return "MINUS";
        case TOK_STAR:   return "STAR";
        case TOK_SLASH:  return "SLASH";
        case TOK_LPAREN: return "LPAREN";
        case TOK_RPAREN: return "RPAREN";
        case TOK_EOF:    return "EOF";
        default:         return "UNKNOWN";
    }
}

// ============================================================
// LEXER
// ============================================================
// The lexer (tokeniser) converts a character string into a flat
// array of Token values.

#define MAX_TOKENS 256

typedef struct {
    Token  tokens[MAX_TOKENS];
    size_t count;
} TokenStream;

// Error helper: print a message and exit.
static void lex_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "[lex error] ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(EXIT_FAILURE);
}

// Tokenise `input` and fill `out`.
static void lex(const char *input, TokenStream *out) {
    out->count = 0;
    const char *p = input;

    while (*p != '\0') {
        // Skip whitespace.
        if (isspace((unsigned char)*p)) { p++; continue; }

        if (out->count >= MAX_TOKENS) {
            lex_error("token limit (%d) exceeded", MAX_TOKENS);
        }

        Token tok = {0};

        switch (*p) {
            case '+': tok.kind = TOK_PLUS;   p++; break;
            case '-': tok.kind = TOK_MINUS;  p++; break;
            case '*': tok.kind = TOK_STAR;   p++; break;
            case '/': tok.kind = TOK_SLASH;  p++; break;
            case '(': tok.kind = TOK_LPAREN; p++; break;
            case ')': tok.kind = TOK_RPAREN; p++; break;
            default:
                if (isdigit((unsigned char)*p) || *p == '.') {
                    // Parse the number with strtod.
                    char *end;
                    tok.kind  = TOK_NUM;
                    tok.value = strtod(p, &end);
                    if (end == p) lex_error("invalid number at: %.10s", p);
                    p = end;
                } else {
                    lex_error("unexpected character '%c' (0x%02X)", *p, (unsigned char)*p);
                }
        }

        out->tokens[out->count++] = tok;
    }

    // Append EOF sentinel.
    out->tokens[out->count++] = (Token){.kind = TOK_EOF, .value = 0};
}

// Pretty-print the token stream (useful for debugging).
static void print_tokens(const TokenStream *ts) {
    printf("Tokens:");
    for (size_t i = 0; i < ts->count; i++) {
        const Token *t = &ts->tokens[i];
        if (t->kind == TOK_NUM) {
            printf(" NUM(%.6g)", t->value);
        } else {
            printf(" %s", token_kind_name(t->kind));
        }
    }
    printf("\n");
}

// ============================================================
// ABSTRACT SYNTAX TREE (AST)
// ============================================================
// The parser builds a tree of Node values.
// Each node is either a numeric literal or a binary operation.

typedef enum {
    AST_NUMBER,  // leaf:   holds a numeric value
    AST_BINOP    // branch: holds an operator and two child nodes
} AstKind;

// Forward declaration so AstNode can reference itself.
typedef struct AstNode AstNode;

struct AstNode {
    AstKind kind;
    union {
        double value;           // AST_NUMBER
        struct {
            char     op;        // '+', '-', '*', '/'
            AstNode *left;
            AstNode *right;
        } binop;                // AST_BINOP
    };
};

// Allocate a number literal node.
static AstNode *ast_number(double value) {
    AstNode *node = malloc(sizeof(AstNode));
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->kind  = AST_NUMBER;
    node->value = value;
    return node;
}

// Allocate a binary-operation node.
static AstNode *ast_binop(char op, AstNode *left, AstNode *right) {
    AstNode *node = malloc(sizeof(AstNode));
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->kind        = AST_BINOP;
    node->binop.op    = op;
    node->binop.left  = left;
    node->binop.right = right;
    return node;
}

// Recursively free an AST.
static void ast_free(AstNode *node) {
    if (!node) return;
    if (node->kind == AST_BINOP) {
        ast_free(node->binop.left);
        ast_free(node->binop.right);
    }
    free(node);
}

// Pretty-print the AST with indentation (useful for debugging).
static void ast_print(const AstNode *node, int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
    if (!node) { printf("(null)\n"); return; }
    if (node->kind == AST_NUMBER) {
        printf("NUM(%.6g)\n", node->value);
    } else {
        printf("BINOP('%c')\n", node->binop.op);
        ast_print(node->binop.left,  depth + 1);
        ast_print(node->binop.right, depth + 1);
    }
}

// ============================================================
// PARSER  (recursive-descent)
// ============================================================
// The parser maintains a cursor (index into the token stream) and
// builds the AST following the grammar rules.

typedef struct {
    const TokenStream *ts;    // token stream produced by the lexer
    size_t             pos;   // current position in the stream
} Parser;

static void parse_error(Parser *p, const char *msg) {
    const Token *tok = &p->ts->tokens[p->pos];
    fprintf(stderr, "[parse error] %s (got %s)\n",
            msg, token_kind_name(tok->kind));
    exit(EXIT_FAILURE);
}

// Peek at the current token without consuming it.
static const Token *parser_peek(Parser *p) {
    return &p->ts->tokens[p->pos];
}

// Consume and return the current token.
static Token parser_advance(Parser *p) {
    return p->ts->tokens[p->pos++];
}

// Consume the current token and verify its kind; abort on mismatch.
static Token parser_expect(Parser *p, TokenKind kind) {
    Token t = parser_advance(p);
    if (t.kind != kind) {
        fprintf(stderr, "[parse error] expected %s, got %s\n",
                token_kind_name(kind), token_kind_name(t.kind));
        exit(EXIT_FAILURE);
    }
    return t;
}

// Forward declarations for the mutually-recursive parse functions.
static AstNode *parse_expr(Parser *p);
static AstNode *parse_term(Parser *p);
static AstNode *parse_factor(Parser *p);

// expr → term ( ( '+' | '-' ) term )*
static AstNode *parse_expr(Parser *p) {
    AstNode *left = parse_term(p);

    while (parser_peek(p)->kind == TOK_PLUS ||
           parser_peek(p)->kind == TOK_MINUS) {
        Token op   = parser_advance(p);
        AstNode *right = parse_term(p);
        char op_ch = (op.kind == TOK_PLUS) ? '+' : '-';
        left = ast_binop(op_ch, left, right);
    }
    return left;
}

// term → factor ( ( '*' | '/' ) factor )*
static AstNode *parse_term(Parser *p) {
    AstNode *left = parse_factor(p);

    while (parser_peek(p)->kind == TOK_STAR ||
           parser_peek(p)->kind == TOK_SLASH) {
        Token op   = parser_advance(p);
        AstNode *right = parse_factor(p);
        char op_ch = (op.kind == TOK_STAR) ? '*' : '/';
        left = ast_binop(op_ch, left, right);
    }
    return left;
}

// factor → NUMBER | '(' expr ')'
static AstNode *parse_factor(Parser *p) {
    const Token *tok = parser_peek(p);

    if (tok->kind == TOK_NUM) {
        Token t = parser_advance(p);
        return ast_number(t.value);
    }

    if (tok->kind == TOK_LPAREN) {
        parser_expect(p, TOK_LPAREN);
        AstNode *inner = parse_expr(p);
        parser_expect(p, TOK_RPAREN);
        return inner;
    }

    parse_error(p, "expected a number or '('");
    return NULL; // unreachable; silences -Wreturn-type
}

// Entry point: parse a full expression and ensure we consumed all tokens.
static AstNode *parse(const TokenStream *ts) {
    Parser p = {.ts = ts, .pos = 0};
    AstNode *root = parse_expr(&p);

    if (parser_peek(&p)->kind != TOK_EOF) {
        parse_error(&p, "unexpected tokens after expression");
    }
    return root;
}

// ============================================================
// EVALUATOR  (tree-walk interpreter)
// ============================================================
// Walk the AST recursively and compute the numeric result.

static double evaluate(const AstNode *node) {
    if (node->kind == AST_NUMBER) {
        return node->value;
    }

    double left  = evaluate(node->binop.left);
    double right = evaluate(node->binop.right);

    switch (node->binop.op) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/':
            if (right == 0.0) {
                fprintf(stderr, "[eval error] division by zero\n");
                exit(EXIT_FAILURE);
            }
            return left / right;
        default:
            fprintf(stderr, "[eval error] unknown operator '%c'\n",
                    node->binop.op);
            exit(EXIT_FAILURE);
    }
}

// ============================================================
// COMPILE + EVALUATE HELPER
// ============================================================
// Runs the full pipeline for one expression string.

static void compile_and_eval(const char *expr) {
    printf("\nExpression: \"%s\"\n", expr);

    // Stage 1: Lex
    TokenStream ts;
    lex(expr, &ts);
    print_tokens(&ts);

    // Stage 2: Parse → AST
    AstNode *root = parse(&ts);
    printf("AST:\n");
    ast_print(root, 1);

    // Stage 3: Evaluate
    double result = evaluate(root);
    printf("Result:  %.10g\n", result);

    ast_free(root);
}

// ============================================================
// MAIN
// ============================================================

int main(void) {
    printf("=== Simple Expression Compiler ===\n");
    printf("Pipeline: source string → tokens → AST → evaluated result\n");

    // Basic arithmetic
    compile_and_eval("3 + 4");
    compile_and_eval("10 - 3 * 2");       // precedence: 10 - 6  = 4
    compile_and_eval("(10 - 3) * 2");     // parentheses: 7  * 2  = 14
    compile_and_eval("100 / 4 / 5");      // left-assoc:  5

    // Floating-point
    compile_and_eval("3.14159 * 2");
    compile_and_eval("(1 + 2) * (3 + 4)");

    // Deeper nesting
    compile_and_eval("2 * (3 + (4 - 1))");

    printf("\n=== Compiler demo complete ===\n");
    return 0;
}
