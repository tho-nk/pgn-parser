use std::time::Instant;

trait Process {
    fn process(&self) -> i32;
}

struct Algo1Ptr {
    nb: i32,
}
struct Algo2Ptr {
    nb: i32,
}
struct Algo3Ptr {
    nb: i32,
}

impl Process for Algo1Ptr {
    fn process(&self) -> i32 {
        let mut sum = 0;
        for i in 0..self.nb {
            sum += i;
        }
        sum
    }
}

impl Process for Algo2Ptr {
    fn process(&self) -> i32 {
        let mut sum = 0;
        for i in 0..self.nb {
            sum += i * 2;
        }
        sum / 2
    }
}

impl Process for Algo3Ptr {
    fn process(&self) -> i32 {
        let mut sum = 0;
        for i in 0..self.nb {
            sum += i * 3;
        }
        sum / 3
    }
}

// Static variants (like std::variant / CRTP usage)
struct Algo1CRTP {
    nb: i32,
}
struct Algo2CRTP {
    nb: i32,
}
struct Algo3CRTP {
    nb: i32,
}

impl Algo1CRTP {
    fn process(&self) -> i32 {
        let mut sum = 0;
        for i in 0..self.nb {
            sum += i;
        }
        sum
    }
}
impl Algo2CRTP {
    fn process(&self) -> i32 {
        let mut sum = 0;
        for i in 0..self.nb {
            sum += i * 2;
        }
        sum / 2
    }
}
impl Algo3CRTP {
    fn process(&self) -> i32 {
        let mut sum = 0;
        for i in 0..self.nb {
            sum += i * 3;
        }
        sum / 3
    }
}

enum Piece {
    P1(Algo1CRTP),
    P2(Algo2CRTP),
    P3(Algo3CRTP),
}

impl Piece {
    fn process(&self) -> i32 {
        match self {
            Piece::P1(p) => p.process(),
            Piece::P2(p) => p.process(),
            Piece::P3(p) => p.process(),
        }
    }
}

fn main() {
    let nb = 100_000; // reduced default to keep runtime reasonable

    // dynamic dispatch (Box<dyn Process>) - like virtual
    let mut v_pieces: Vec<Box<dyn Process>> = Vec::new();
    v_pieces.push(Box::new(Algo1Ptr { nb: 1000 }));
    v_pieces.push(Box::new(Algo2Ptr { nb: 2000 }));
    v_pieces.push(Box::new(Algo3Ptr { nb: 3000 }));

    v_pieces.push(Box::new(Algo1Ptr { nb: 100 }));
    v_pieces.push(Box::new(Algo2Ptr { nb: 200 }));
    v_pieces.push(Box::new(Algo3Ptr { nb: 300 }));

    v_pieces.push(Box::new(Algo1Ptr { nb: 10 }));
    v_pieces.push(Box::new(Algo2Ptr { nb: 20 }));
    v_pieces.push(Box::new(Algo3Ptr { nb: 30 }));

    let start = Instant::now();
    let mut sink: i64 = 0;
    for _ in 0..nb {
        for piece in &v_pieces {
            let r = piece.process();
            sink ^= r as i64; // keep optimizer honest
        }
    }
    let dur_v = start.elapsed();

    // variant (enum) - like std::variant / CRTP static
    let mut c_pieces: Vec<Piece> = Vec::new();
    c_pieces.push(Piece::P1(Algo1CRTP { nb: 1000 }));
    c_pieces.push(Piece::P2(Algo2CRTP { nb: 2000 }));
    c_pieces.push(Piece::P3(Algo3CRTP { nb: 3000 }));

    c_pieces.push(Piece::P1(Algo1CRTP { nb: 100 }));
    c_pieces.push(Piece::P2(Algo2CRTP { nb: 200 }));
    c_pieces.push(Piece::P3(Algo3CRTP { nb: 300 }));

    c_pieces.push(Piece::P1(Algo1CRTP { nb: 10 }));
    c_pieces.push(Piece::P2(Algo2CRTP { nb: 20 }));
    c_pieces.push(Piece::P3(Algo3CRTP { nb: 30 }));

    let start2 = Instant::now();
    for _ in 0..nb {
        for piece in &c_pieces {
            let r = piece.process();
            sink ^= r as i64;
        }
    }
    let dur_c = start2.elapsed();

    println!("Sink: {} (prevent optimization)", sink);
    println!("Time for virtual: {} ms", dur_v.as_millis());
    println!("Time for CRTP-like enum: {} ms", dur_c.as_millis());
}
