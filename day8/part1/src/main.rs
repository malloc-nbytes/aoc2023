use std::collections::HashMap;
use std::fs;

const FILEPATH: &str = "./input.txt";

fn parse_input(filepath: &str) -> (String, HashMap<String, String>) {
    let contents = fs::read_to_string(filepath)
        .expect("Something went wrong reading the file");
    let mut lines = contents.lines().filter(|line| !line.is_empty());
    let rules = lines.next().unwrap().to_string();
    let mut path = HashMap::new();
    for line in lines {
        let mut parts = line.split(" = ");
        let key = parts.next().unwrap().to_string();
        let value = parts.next().unwrap().to_string();
        path.insert(key, value);
    }
    (rules, path)
}

fn walk(path: &HashMap<String, String>, rules: &String) -> u32 {
    let mut steps = 0u32;
    let mut current: &str = path.get("AAA").unwrap();
    let mut i = 0usize;

    loop {
        let rule = rules.chars().nth(i%rules.len()).unwrap();
        if rule == 'L' {
            current = path.get(current).unwrap();
        } else {
            current = path.get(current).unwrap();
        }
    }

    steps
}

fn main() {
    let (rules, path) = parse_input(FILEPATH);
    println!("{path:?}");
    // let steps = walk(&path, &rules);
    // println!("{steps}");
}
