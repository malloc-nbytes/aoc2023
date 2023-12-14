use std::fs;

const FILEPATH: &str = "./input.txt";

#[derive(Debug, PartialEq, PartialOrd, Eq, Ord)]
enum Kind {
    Joker = 0,
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
        n-1
    } else {
        match val {
            'T' => 9,
            'J' => 0,
            'Q' => 11,
            'K' => 12,
            'A' => 13,
            _ => panic!("invalid card"),
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
            Hand { cards, bet, kind: Kind::Joker, rank: 0 }
        })
        .collect::<Vec<_>>()
}

fn order_hands(hands: &mut Vec<Hand>) {

    fn determine_kind(hand: &mut Hand) {
        const MAX_CARDS: usize = 14;

        let mut cards = [0u32; MAX_CARDS];
        for card in &hand.cards {
            let n = translate_card(*card);
            cards[n as usize] += 1;
        }
        println!("hand: {:?}", hand.cards);
        println!("cards: {:?}", cards);

        let mut counts = [0u32; 6];
        let jokers = cards[0];
        counts[0] = jokers;

        for i in 1..MAX_CARDS {
            let n = cards[i];
            if n != 0 {
                counts[n as usize] += 1;
            }
        }
        println!("counts: {:?}", counts);

        if jokers != 0 {
            unimplemented!();
        }

        if counts[5] == 1 {
            hand.kind = Kind::FiveKind;
        } else if counts[4] == 1 {
            hand.kind = Kind::FourKind;
        } else if counts[3] == 1 && counts[2] == 1 {
            hand.kind = Kind::FullHouse;
        } else if counts[3] == 1 {
            hand.kind = Kind::ThreeKind;
        } else if counts[2] == 2 {
            hand.kind = Kind::TwoPair;
        } else if counts[2] == 1 {
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
                    let (ca, cb) = (translate_card(*card_a), translate_card(*card_b));
                    match ca.cmp(&cb) {
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
    for hand in hands {
        s += hand.rank*hand.bet;
        println!("cards: {:?}, kind {:?}, rank: {}", hand.cards, hand.kind, hand.rank);
    }

    println!("s: {s}")
}
