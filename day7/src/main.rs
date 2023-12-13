use std::fs;

const FILEPATH: &str = "./input.txt";

const HIGHCARD: u32 = 1;
const ONEPAIR: u32 = 2;
const TWOPAIR: u32 = 3;
const THREEKIND: u32 = 4;
const FULLHOUSE: u32 = 5;
const FOURKIND: u32 = 6;
const FIVEKIND: u32 = 7;

#[allow(dead_code)]
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

        // Number of each individual card.
        let mut arr: [u32; MAX_CARDS] = [0; MAX_CARDS];

        // Number of singles, pairs, triples, etc...
        let mut counts: [u32; 5] = [0; 5];

        // Fill the array with the cards.
        hand.cards
            .iter()
            .for_each(|v| arr[translate_card(*v) as usize - 2] += 1);

        // Figure out how many singles, pairs, triples, etc. there are.
        for x in arr {
            if x > 0 {
                counts[x as usize - 1] += 1;
            }
        }

        // Determine the ranking for the hand.
        if counts[4] == 1 {
            hand.rank = FIVEKIND;
        } else if counts[3] == 1 {
            hand.rank = FOURKIND;
        } else if counts[2] == 1 && counts[1] == 1 {
            hand.rank = FULLHOUSE;
        } else if counts[2] == 1 {
            hand.rank = THREEKIND;
        } else if counts[1] == 2 {
            hand.rank = TWOPAIR;
        } else if counts[1] == 1 {
            hand.rank = ONEPAIR;
        } else {
            hand.rank = HIGHCARD;
        }
    }

    for hand in &mut *hands {
        determine_rank(hand);
    }

    hands.sort_by(|a, b| {
        match a.rank.cmp(&b.rank) {
            // Compare cards when ranks are equal.
            std::cmp::Ordering::Equal => {
                for (card_a, card_b) in a.cards.iter().zip(b.cards.iter()) {
                    match card_b.cmp(card_a) {
                        std::cmp::Ordering::Equal => continue,
                        other => return other,
                    }
                }
                panic!("both cards are equal");
            }
            // Clear winner.
            other => other,
        }
    });
}

fn main() {
    let mut data = fs::read_to_string(FILEPATH).unwrap();
    data.pop();

    let mut hands = create_hands(data);
    order_hands(&mut hands);

    let mut s = 0u32;
    for i in 0..hands.len() {
        s += (i as u32 + 1) * hands[i].bet;
        println!("hand {i}: cards: {:?}, rank: {}", hands[i].cards, hands[i].rank);
    }

    println!("s: {s}")
}
