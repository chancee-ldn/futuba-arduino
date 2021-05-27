# futuba-arduino

Some base files to get a Futuba GP1212A02A Vacuum Florescent Display firing some photons. This took an embarrassing amount of time to get working.

### Arduino
A basic Sketch with some commands to play around with. The data sheet for the Futuba GP1212A02A was not easy to understand; so if I've missed anything or there is a better way to execute the commands I'd like to hear it.

### Rust
This is the first thing I've built with Rust so optimisations will be everywhere. The Arduino is a slave and the Rust CLI sends commands which trigger functions to display some pixels on the VFD.

### Resources
[Rust AVR Guidebook](https://book.avr-rust.com)

[MiniDisplay](https://github.com/Slion/MiniDisplay/blob/master/README.md) I wouldn't have got very far without this. It's in C, but gave me enough to see how to structure data to send. They use USB via HID - I needed i2c.

