use std::collections::HashMap;
use std::thread;

#[derive(Debug)]
struct User {
    name: String,
    age: u32,
}

trait Greeter {
    fn greet(&self) -> String;
}

impl Greeter for User {
    fn greet(&self) -> String {
        format!("Hi, I'm {}", self.name)
    }
}

fn divide(a: f64, b: f64) -> Result<f64, &'static str> {
    if b == 0.0 {
        return Err("division by zero");
    }
    Ok(a / b)
}

fn main() {
    let numbers = vec![1, 2, 3, 4, 5];
    let squares: HashMap<i32, i32> = numbers.iter().map(|n| (*n, n * n)).collect();

    let user = User {
        name: String::from("Ada"),
        age: 27,
    };
    println!("{} ({:?})", user.greet(), squares);

    match divide(10.0, 2.0) {
        Ok(value) => println!("10 / 2 = {value}"),
        Err(error) => println!("error: {error}"),
    }

    let handle = thread::spawn(|| "worker done");
    println!("{}", handle.join().unwrap());
    println!("age: {}", user.age);
}
