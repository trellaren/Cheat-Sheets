use std::collections::HashMap;
use std::num::ParseIntError;
use std::sync::mpsc;
use std::thread;

#[derive(Debug, Clone)]
struct User {
    name: String,
    age: u32,
}

impl User {
    fn greet(&self) -> String {
        format!("Hi, I'm {}", self.name)
    }

    fn years_until(&self, target_age: u32) -> u32 {
        target_age.saturating_sub(self.age)
    }
}

trait Summary {
    fn summary(&self) -> String;
}

#[derive(Debug)]
struct ScoreReport {
    user: User,
    scores: Vec<u32>,
}

impl ScoreReport {
    fn average(&self) -> Option<f64> {
        if self.scores.is_empty() {
            return None;
        }

        let total: u32 = self.scores.iter().sum();
        Some(total as f64 / self.scores.len() as f64)
    }

    fn passing_scores(&self) -> Vec<u32> {
        self.scores
            .iter()
            .copied()
            .filter(|score| *score >= 70)
            .collect()
    }
}

impl Summary for ScoreReport {
    fn summary(&self) -> String {
        let average = self.average().unwrap_or_default();
        format!(
            "{} has an average score of {:.1} and passing scores {:?}",
            self.user.name,
            average,
            self.passing_scores()
        )
    }
}

fn divide(a: f64, b: f64) -> Result<f64, &'static str> {
    if b == 0.0 {
        return Err("division by zero");
    }

    Ok(a / b)
}

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

fn square_in_parallel(numbers: Vec<i32>) -> Vec<i32> {
    let (tx, rx) = mpsc::channel();

    let worker = thread::spawn(move || {
        for number in numbers {
            tx.send(number * number).unwrap();
        }
    });

    let mut results: Vec<_> = rx.into_iter().collect();
    worker.join().unwrap();
    results.sort_unstable();
    results
}

fn print_summary(item: &impl Summary) {
    println!("{}", item.summary());
}

fn basic_examples() {
    println!("== Basic Rust ==");

    let mut tools = vec!["cargo", "rustfmt"];
    tools.push("clippy");

    let user = User {
        name: String::from("Ada"),
        age: 27,
    };

    println!("{}", user.greet());
    println!("Tools: {:?}", tools);
    println!("Years until 30: {}", user.years_until(30));

    match divide(10.0, 2.0) {
        Ok(value) => println!("10 / 2 = {value}"),
        Err(error) => println!("error: {error}"),
    }
}

fn intermediate_examples() -> Result<(), ParseIntError> {
    println!("\n== Intermediate Rust ==");

    let parsed = parse_numbers(&["10", "20", "30"])?;
    let report = ScoreReport {
        user: User {
            name: String::from("Lin"),
            age: 31,
        },
        scores: vec![88, 91, 65, 99],
    };
    let frequencies =
        word_frequencies("Rust makes systems programming feel safe and Rust stays fast.");

    println!("Parsed numbers: {:?}", parsed);
    print_summary(&report);
    println!("Word frequencies: {:?}", frequencies);

    Ok(())
}

fn advanced_examples() {
    println!("\n== Advanced Rust ==");

    let longest_label = longest("ownership", "borrowing");
    let squares = square_in_parallel(vec![1, 2, 3, 4, 5]);

    println!("Longest label: {longest_label}");
    println!("Parallel squares: {:?}", squares);
}

fn main() {
    basic_examples();

    if let Err(error) = intermediate_examples() {
        eprintln!("Intermediate examples failed: {error}");
    }

    advanced_examples();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn divide_rejects_zero() {
        assert_eq!(divide(10.0, 0.0), Err("division by zero"));
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
    fn longest_returns_longer_value() {
        assert_eq!(longest("safe", "fearless"), "fearless");
    }

    #[test]
    fn square_in_parallel_returns_sorted_squares() {
        assert_eq!(square_in_parallel(vec![5, 1, 3]), vec![1, 9, 25]);
    }

    #[test]
    fn score_report_filters_passing_scores() {
        let report = ScoreReport {
            user: User {
                name: String::from("Pat"),
                age: 22,
            },
            scores: vec![50, 70, 90],
        };

        assert_eq!(report.passing_scores(), vec![70, 90]);
        assert_eq!(report.average(), Some(70.0));
    }
}
