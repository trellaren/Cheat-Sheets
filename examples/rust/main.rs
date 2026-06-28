use std::collections::HashMap;
use std::fs;
use std::num::ParseIntError;
use std::path::PathBuf;
use std::sync::mpsc;
use std::thread;
use std::time::{Duration, Instant};

// ============================================================
// BASIC DATA TYPES EXAMPLES
// ============================================================
// This section intentionally shows many Rust data types in one place.
// Think of this as the "quick type tour" for a beginner.
fn data_type_examples() -> (i32, f64, bool, char, usize) {
    // Integer type (signed, 32-bit)
    let integer_value: i32 = 42;

    // Float type (64-bit floating point)
    let float_value: f64 = 3.1415;

    // Boolean
    let feature_enabled: bool = true;

    // Character (Unicode scalar value)
    let language_marker: char = '🦀';

    // Tuple combines multiple different types
    let status_tuple: (&str, u8) = ("ready", 1);

    // Array has fixed size known at compile time
    let retry_codes: [u16; 3] = [200, 201, 500];

    // Vector is growable and heap allocated
    let mut tools: Vec<&str> = vec!["cargo", "rustfmt"];
    tools.push("clippy");

    // HashMap is the standard key-value map in Rust stdlib
    let mut counts: HashMap<&str, usize> = HashMap::new();
    counts.insert("tools", tools.len());

    println!("== Data Types ==");
    println!("integer: {integer_value}");
    println!("float: {float_value}");
    println!("bool: {feature_enabled}");
    println!("char: {language_marker}");
    println!("tuple: {:?}", status_tuple);
    println!("array: {:?}", retry_codes);
    println!("vector: {:?}", tools);
    println!("hash map: {:?}", counts);

    (
        integer_value,
        float_value,
        feature_enabled,
        language_marker,
        tools.len(),
    )
}

// ============================================================
// OBJECT ORIENTED STYLE EXAMPLES (class-like, inheritance-like)
// ============================================================
// Rust does not have "class" and "inheritance" in the same way as Java/C#.
// Instead, Rust uses structs + impl blocks + traits + composition.
// This section demonstrates how to model OOP ideas idiomatically.

#[derive(Debug, Clone)]
struct BankAccount {
    owner: String,
    // Private field: only methods on BankAccount can directly modify this.
    balance: i64,
}

impl BankAccount {
    /// Constructor-style associated function (class constructor equivalent).
    /// Negative opening balances are clamped to 0 because this beginner demo
    /// focuses on state transitions without introducing constructor error flows.
    fn new(owner: &str, opening_balance: i64) -> Self {
        Self {
            owner: owner.to_string(),
            balance: opening_balance.max(0),
        }
    }

    // "Getter" style API.
    fn balance(&self) -> i64 {
        self.balance
    }

    // Encapsulated mutation method.
    fn deposit(&mut self, amount: i64) {
        if amount > 0 {
            self.balance += amount;
        }
    }

    // Safe withdrawal that uses Result for error handling.
    fn withdraw(&mut self, amount: i64) -> Result<i64, &'static str> {
        if amount <= 0 {
            return Err("amount must be positive");
        }
        if self.balance < amount {
            return Err("insufficient funds");
        }
        self.balance -= amount;
        Ok(self.balance)
    }
}

// Trait behaves similarly to an interface/base contract.
trait Notifier {
    fn send(&self, message: &str) -> String;
}

#[derive(Debug)]
struct EmailNotifier {
    address: String,
}

impl Notifier for EmailNotifier {
    fn send(&self, message: &str) -> String {
        format!("Email to {}: {}", self.address, message)
    }
}

// Composition: "inherits behavior" by holding another struct and delegating.
#[derive(Debug)]
struct AuditedNotifier {
    inner: EmailNotifier,
    audit_tag: String,
}

impl Notifier for AuditedNotifier {
    fn send(&self, message: &str) -> String {
        let original = self.inner.send(message);
        format!("[{}] {}", self.audit_tag, original)
    }
}

fn oop_style_examples() {
    println!("\n== Object-Oriented Style Rust ==");

    let mut account = BankAccount::new("Ada", 1_000);
    account.deposit(250);
    match account.withdraw(100) {
        Ok(remaining) => println!("withdraw successful, remaining: {remaining}"),
        Err(error) => println!("withdraw failed: {error}"),
    }

    let plain_email = EmailNotifier {
        address: "ada@example.com".to_string(),
    };
    let audited_email = AuditedNotifier {
        inner: EmailNotifier {
            address: "ops@example.com".to_string(),
        },
        audit_tag: "SECURITY-AUDIT".to_string(),
    };

    println!("account owner: {}", account.owner);
    println!("account balance: {}", account.balance());
    println!("{}", plain_email.send("Balance updated"));
    println!("{}", audited_email.send("Suspicious login check"));
}

// ============================================================
// LIBRARIES + APPLICATION SUPPORT HELPERS
// ============================================================

fn parse_numbers(values: &[&str]) -> Result<Vec<i32>, ParseIntError> {
    values.iter().map(|value| value.parse::<i32>()).collect()
}

fn word_frequencies(text: &str) -> HashMap<String, usize> {
    let mut counts = HashMap::new();
    for word in text.split_whitespace() {
        let cleaned = word
            .trim_matches(|character: char| !character.is_alphanumeric())
            .to_ascii_lowercase();
        if !cleaned.is_empty() {
            *counts.entry(cleaned).or_insert(0) += 1;
        }
    }
    counts
}

fn longest<'a>(left: &'a str, right: &'a str) -> &'a str {
    if left.len() >= right.len() {
        left
    } else {
        right
    }
}

// Shows std library usage:
// - std::fs for file I/O
// - std::time for duration measurement
// - std::path for path handling
fn library_examples() -> Result<String, std::io::Error> {
    println!("\n== Library Usage (std) ==");

    let started = Instant::now();
    let file_name = format!("rust-cheat-sheets-library-demo-{}.txt", std::process::id());
    let demo_path: PathBuf = std::env::temp_dir().join(file_name);

    fs::write(&demo_path, "rust libraries demo line")?;
    let content = fs::read_to_string(&demo_path)?;
    let elapsed: Duration = started.elapsed();
    let elapsed_us = elapsed.as_micros();

    println!("file path: {:?}", demo_path);
    println!("file content: {content}");
    println!("elapsed: {} µs", elapsed_us);

    // Best-effort cleanup for a temporary file.
    let _ = fs::remove_file(demo_path);
    Ok(content)
}

// ============================================================
// DATABASE CONNECTION EXAMPLES (mock + realistic comments)
// ============================================================
// We keep this file standalone (no Cargo.toml), so this uses a mock DB client.
// In a full app, you would usually use:
// - sqlx for async pooling + compile-time query checks
// - diesel for ORM style
// - rusqlite for SQLite

#[derive(Debug, Clone, PartialEq, Eq)]
struct DbUser {
    id: u32,
    name: String,
}

trait DatabaseClient {
    fn connect(&mut self) -> Result<(), String>;
    fn insert_user(&mut self, name: &str) -> Result<DbUser, String>;
    fn list_users(&self) -> Result<Vec<DbUser>, String>;
    fn disconnect(&mut self);
}

#[derive(Debug)]
struct MockDatabase {
    connected: bool,
    next_id: u32,
    users: Vec<DbUser>,
}

impl MockDatabase {
    fn new() -> Self {
        Self {
            connected: false,
            next_id: 1,
            users: vec![],
        }
    }
}

impl DatabaseClient for MockDatabase {
    fn connect(&mut self) -> Result<(), String> {
        self.connected = true;
        Ok(())
    }

    fn insert_user(&mut self, name: &str) -> Result<DbUser, String> {
        if !self.connected {
            return Err("database is not connected".to_string());
        }
        let record = DbUser {
            id: self.next_id,
            name: name.to_string(),
        };
        self.next_id += 1;
        self.users.push(record.clone());
        Ok(record)
    }

    fn list_users(&self) -> Result<Vec<DbUser>, String> {
        if !self.connected {
            return Err("database is not connected".to_string());
        }
        Ok(self.users.clone())
    }

    fn disconnect(&mut self) {
        self.connected = false;
    }
}

fn database_examples() -> Result<Vec<DbUser>, String> {
    println!("\n== Database Connection Example ==");

    let mut db = MockDatabase::new();
    db.connect()?;
    let _ = db.insert_user("Ada")?;
    let _ = db.insert_user("Lin")?;

    let users = db.list_users()?;
    println!("users in mock database: {:?}", users);

    db.disconnect();
    Ok(users)
}

// ============================================================
// APPLICATION EXAMPLE (small CLI-style todo app)
// ============================================================

#[derive(Debug, Clone, PartialEq, Eq)]
struct Task {
    id: u32,
    title: String,
    completed: bool,
}

#[derive(Debug, Default)]
struct TaskApp {
    next_id: u32,
    tasks: Vec<Task>,
}

impl TaskApp {
    fn new() -> Self {
        Self {
            next_id: 1,
            tasks: vec![],
        }
    }

    fn add_task(&mut self, title: &str) -> Task {
        let task = Task {
            id: self.next_id,
            title: title.to_string(),
            completed: false,
        };
        self.next_id += 1;
        self.tasks.push(task.clone());
        task
    }

    fn complete_task(&mut self, id: u32) -> bool {
        if let Some(task) = self.tasks.iter_mut().find(|task| task.id == id) {
            task.completed = true;
            return true;
        }
        false
    }

    fn list_open_tasks(&self) -> Vec<Task> {
        self.tasks
            .iter()
            .filter(|item| !item.completed)
            .cloned()
            .collect()
    }
}

fn application_examples() {
    println!("\n== Application Example ==");

    let mut app = TaskApp::new();
    let first = app.add_task("Write Rust cheat sheet example");
    let second = app.add_task("Add mock database flow");
    let _ = app.complete_task(first.id);

    println!("created tasks: {:?}, {:?}", first, second);
    println!("open tasks: {:?}", app.list_open_tasks());
}

// ============================================================
// ADVANCED EXAMPLE (threading + channels + lifetimes)
// ============================================================

fn square_in_parallel(numbers: Vec<i32>) -> Vec<i32> {
    let (tx, rx) = mpsc::channel();
    let worker = thread::spawn(move || {
        for number in numbers {
            // Send each processed value from the worker thread.
            tx.send(number * number).unwrap();
        }
    });

    // Collect all values after channel is closed when worker exits.
    let mut results: Vec<_> = rx.into_iter().collect();
    worker.join().unwrap();
    results.sort_unstable();
    results
}

fn advanced_examples() {
    println!("\n== Advanced Rust ==");

    let longest_label = longest("ownership", "borrowing");
    let squares = square_in_parallel(vec![1, 2, 3, 4, 5]);

    println!("longest label: {longest_label}");
    println!("parallel squares: {:?}", squares);
}

fn main() {
    let _ = data_type_examples();
    oop_style_examples();

    println!("\n== Intermediate Helpers ==");
    match parse_numbers(&["10", "20", "30"]) {
        Ok(parsed) => println!("parsed numbers: {:?}", parsed),
        Err(error) => eprintln!("parse failed: {error}"),
    }
    println!(
        "word frequencies: {:?}",
        word_frequencies("Rust makes systems programming safer and Rust stays fast.")
    );

    match library_examples() {
        Ok(content) => println!("library demo read back: {content}"),
        Err(error) => eprintln!("library demo failed: {error}"),
    }

    match database_examples() {
        Ok(users) => println!("database demo users: {}", users.len()),
        Err(error) => eprintln!("database demo failed: {error}"),
    }

    application_examples();
    advanced_examples();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn data_type_examples_returns_expected_shapes() {
        let result = data_type_examples();
        assert_eq!(result.0, 42);
        assert!(result.1 > 3.0);
        assert!(result.2);
        assert_eq!(result.3, '🦀');
        assert_eq!(result.4, 3);
    }

    #[test]
    fn bank_account_withdraw_rejects_insufficient_funds() {
        let mut account = BankAccount::new("Pat", 100);
        assert_eq!(account.withdraw(200), Err("insufficient funds"));
    }

    #[test]
    fn parse_numbers_collects_values() {
        assert_eq!(parse_numbers(&["1", "2", "3"]).unwrap(), vec![1, 2, 3]);
    }

    #[test]
    fn word_frequencies_normalize_case_and_punctuation() {
        let counts = word_frequencies("Rust, rust! Borrow checker.");
        assert_eq!(counts.get("rust"), Some(&2));
        assert_eq!(counts.get("borrow"), Some(&1));
        assert_eq!(counts.get("checker"), Some(&1));
    }

    #[test]
    fn mock_database_requires_connection() {
        let mut db = MockDatabase::new();
        assert!(db.insert_user("Ada").is_err());
        db.connect().unwrap();
        assert!(db.insert_user("Ada").is_ok());
    }

    #[test]
    fn task_app_tracks_open_tasks() {
        let mut app = TaskApp::new();
        let task = app.add_task("Task A");
        app.add_task("Task B");
        assert!(app.complete_task(task.id));

        let open = app.list_open_tasks();
        assert_eq!(open.len(), 1);
        assert_eq!(open[0].title, "Task B");
    }

    #[test]
    fn square_in_parallel_returns_sorted_squares() {
        assert_eq!(square_in_parallel(vec![5, 1, 3]), vec![1, 9, 25]);
    }
}
