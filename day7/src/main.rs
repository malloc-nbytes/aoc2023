use std::fs;

const FILEPATH: &str = "./input.txt";

#[derive(Debug, PartialEq, PartialOrd, Eq, Ord)]
enum Kind {
    None = 0,
    HighCard = 1,
    OnePair = 2,
    TwoPair = 3,
    ThreeKind = 4,
    FullHouse = 5,
    FourKind = 6,
    FiveKind = 7,
}

struct Hand {
    cards: Vec<char>,
    bet: u32,
    kind: Kind,
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
            Hand { cards, bet, kind: Kind::None, rank: 0 }
        })
        .collect::<Vec<_>>()
}

fn order_hands(hands: &mut Vec<Hand>) {

    fn determine_kind(hand: &mut Hand) {
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

        // Determine the kind for the hand.
        if counts[4] == 1 {
            hand.kind = Kind::FiveKind;
        } else if counts[3] == 1 {
            hand.kind = Kind::FourKind;
        } else if counts[2] == 1 && counts[1] == 1 {
            hand.kind = Kind::FullHouse;
        } else if counts[2] == 1 {
            hand.kind = Kind::ThreeKind;
        } else if counts[1] == 2 {
            hand.kind = Kind::TwoPair;
        } else if counts[1] == 1 {
            hand.kind = Kind::OnePair;
        } else {
            hand.kind = Kind::HighCard;
        }
    }

    for hand in &mut *hands {
        determine_kind(hand);
    }

    hands.sort_by(|a, b| {
        match a.kind.cmp(&b.kind) {
            // Compare cards when kinds are equal.
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

    // Determine the rank of each hand.
    let mut rank = 1u32;
    for i in 0..hands.len() {
        // if i > 0 && hands[i].kind != hands[i - 1].kind {
        //     rank += 1;
        // }
        hands[i].rank = rank;
        rank += 1;
    }
}

fn main() {
    let mut data = fs::read_to_string(FILEPATH).unwrap();
    data.pop();

    let mut hands = create_hands(data);
    order_hands(&mut hands);

    let mut s = 0u32;
    for i in 0..hands.len() {
        s += hands[i].rank * hands[i].bet;
        println!("hand {i}: cards: {:?}, kind: {:?}, rank: {}", hands[i].cards, hands[i].kind, hands[i].rank);
    }

    println!("s: {s}")
}
