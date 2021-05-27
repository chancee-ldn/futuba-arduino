use std::env;

extern crate serial;

use std::time::Duration;
use std::thread;
use serial::prelude::*;

use std::process::Command;
use std::io;
use std::io::stdin;
use std::io::stdout;
use std::io::Write;

//extern crate terminal_cli;
//use terminal_cli::*;



// Default settings of Arduino
// see: https://www.arduino.cc/en/Serial/Begin
const SETTINGS: serial::PortSettings = serial::PortSettings {
    baud_rate:    serial::Baud9600,
    char_size:    serial::Bits8,
    parity:       serial::ParityNone,
    stop_bits:    serial::Stop1,
    flow_control: serial::FlowNone,
};


struct Event {
	command: String,
	message: Option<String>,
}



fn main() {
    let serial_port = "/dev/tty.usbmodem14101";
    //let serial_port = "/dev/tty.Bluetooth-Incoming-Port";
	println!("Opening port: {:?}", serial_port);
    let mut port = serial::open(&serial_port).unwrap();
    port.configure(&SETTINGS).unwrap();
    port.set_timeout(Duration::from_secs(30)).unwrap();

	loop {
        // need to explicitly flush this to ensure it prints before read_line
        print!("> ");
        stdout().flush();
	
        let mut input = String::new();
        stdin().read_line(&mut input).unwrap();
		let mut parts = input.split_whitespace();

		//#![feature(str_split_whitespace_as_str)]
		//let mut split = "Mary had a little lamb".split_whitespace();
		//let o = split.as_str();
		//println!("{:?}", o);

		//let pattern = std::env::args().nth(1).expect("no pattern given");
		//let path = std::env::args().nth(2).expect("no path given");
		

		
		let command = parts.next().unwrap();
		//let m = parts.next();
		//println!("message: {:?}", m);

		if let args = &input {
			println!("Event: command: {:?} message: {:?} ", command, args);
			let mut owned_string: String = "<song, ".to_owned();
			let another_s: &str = &args;
			let s = &another_s.replace('\n', ""); // new lines remove the cool stuff on the VFD


			let tag: &str = ">";
			let r = owned_string + s + tag;	
				match command {
					"on"  => {	let s = String::from("<on>"); port.write(s.as_bytes());		},
					"off"=> {	let s = String::from("<off>"); port.write(s.as_bytes());	},
					"play" => {	let s = String::from("<play>"); port.write(s.as_bytes());	},
					"song" => { port.write(r.as_bytes());	},	
					"reset"=> {	let s = String::from("<reset>"); port.write(s.as_bytes());	},
					_ =>	println!("no input"),
				}
		}	
		
	
		thread::sleep(Duration::from_secs(1));
	}
}


/*
fn inspect(event: Message) {
	match event {
		Message::On => println!("on"),
	}
}
*/

