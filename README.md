# futuba-arduino

Some base files to get a Futuba GP1212A02A Vacuum Florescent Display firing some photons. This took an embarrassing amount of time to get working. If you are here for the Futuba `/futuba-serial` has the Arduino code, you should find enough code examples in there to see how to operate the VFD. The Rust script is purely an interface to execute those commands.

### Futuba
The GP1212A02A resides on address `0x70` when connected via i2c. 
When in BMP mode, you aren't addressing individual pixels but a vertical/horizontal row of eight dots in binary 0b00111111 = ○ ○ ● ● ● ● ● ●:


| ○ | ○ | ○ | ○ | 
|---|---|---|---|
| ○ | ○ | ● | ○ |
| ○ | ○ | ● | ○ |
| ○ | ○ | ● | ● |
| ○ | ○ | ● | ● |
| ○ | ● | ● | ● |
| ○ | ● | ● | ● |
| ● | ● | ● | ● |
| 0x01 | 0x03 | 0xFF | 0x3F |

Or one byte. Which is mapped top to bottom `0x0000 - 0x0007`
So 00000001 would activate the very bottom dot (or the farthest right dot in X mode) in a 8x1 block. This also took a horrible amount of time to grasp.

### Arduino
A sketch with some commands to play around with. The `serial` file sets up communication between the Arduino and the VFD, the `functions` file contains the hex sequences you send to the VFD to trigger events. 

N.B. The data sheet for the Futuba GP1212A02A was not easy to understand; so if I've missed anything or there is a better way to execute the commands I'd like to hear it. The Futuba doesn't seem to like anything over 9600 baud: according to the data sheet there is a jumper setting at `J7` for this - but I can't work out how to adjust it.

### Rust
This is the first thing I've built with Rust so optimisations will be everywhere. The Arduino is a slave and the Rust CLI sends commands which trigger functions to display some pixels on the VFD.

`song a string of text to send to the VFD` will display the second argument in Text Box 1.

`on randomString` will turn the LED on the Arduino to check comms are working, the random string is because I couldn't work out how to check for a null in Rust.

`off randomString` will turn that same LED off


### Notes
Rust needs to know where to look for the Arduino: find the `serial_port` variable and adjust to suit your system. It should listed: `ls /dev`, you can unplug and plugin again, or there is a 'dmesg' trick after a restart.

#### Mac OS
`let serial_port = "/dev/.usbmodem4101"`
#### Khadas + Ubuntu
`let serial_port = "/dev/serial/by-path/xxx"`


### Resources
[Rust AVR Guidebook](https://book.avr-rust.com)

[Silons Arduino VFD](https://slions.net/threads/arduino-micro-noritake-vfd.40/) I wouldn't have got very far without this documentation.

[MiniDisplay](https://github.com/Slion/MiniDisplay/blob/master/README.md)  Engine for the above - it's in C, but gave me enough to see how to structure data to send. They use USB via HID - I needed i2c.

