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

		
		let command = parts.next().unwrap();
		if let args = &input {
			println!("Event: command: {:?} message: {:?} ", command, args);
			let mut owned_string: String = "<song,".to_owned();
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



