use std::fs;

const FILEPATH: &str = "./input.txt";

const HIGHCARD: u32 = 1;
const ONEPAIR: u32 = 2;
const TWOPAIR: u32 = 3;
const THREEKIND: u32 = 4;
const FULLHOUSE: u32 = 5;
const FOURKIND: u32 = 6;
const FIVEKIND: u32 = 7;

struct Hand {
    cards: Vec<char>,
    bet: u32,
    rank: u32,
}

fn translate_card(val: char) -> u32 {
    let n = (val as u32) - ('0' as u32);
    if (2..=9).contains(&n) {
        n
    } else {
        match val {
            'T' => 10,
            'J' => 11,
            'Q' => 12,
            'K' => 13,
            'A' => 14,
            _ => panic!("invalid number")
        }
    }
}

fn create_hands(data: String) -> Vec<Hand> {
    data
        .lines()
        .map(|line| {
            let parts: Vec<&str> = line.split_whitespace().collect();
            let cards = parts[0].chars().collect::<Vec<char>>();
            let bet = parts[1].parse().unwrap();
            Hand { cards, bet, rank: 0 }
        })
        .collect::<Vec<_>>()
}

fn order_hands(hands: &mut Vec<Hand>) {
    fn determine_rank(hand: &mut Hand) {
        const MAX_CARDS: usize = 13;

        let mut arr: [u32; MAX_CARDS] = [0; MAX_CARDS];
        let mut counts: [u32; 7] = [0; 7];

        hand.cards
            .iter()
            .for_each(|v| arr[translate_card(*v) as usize - 2] += 1);

        for &x in arr.iter() {
            counts[(x) as usize - 1] += 1;
        }

        if counts[6] == 1 {
            hand.rank = FIVEKIND;
        } else if counts[5] == 1 {
            hand.rank = FOURKIND;
        }
    }

    for hand in &mut *hands {
        if hand.rank == 0 {
            determine_rank(hand);
        }
    }

    hands.sort_by(|a, b| b.rank.cmp(&a.rank));
}

fn main() {
    let mut data = fs::read_to_string(FILEPATH).unwrap();
    data.pop();

    let mut hands = create_hands(data);

    order_hands(&mut hands);
}
