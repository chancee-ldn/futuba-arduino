long randNumber;

void write_byte(short data)
  {
    Wire.beginTransmission(0x70);
    Wire.write(data);
    busStatus = Wire.endTransmission();
    Serial.println(busStatus);
    delay (1);
  }

void fast_byte(short data)
  {
    Wire.beginTransmission(0x70);
    Wire.write(data);
    busStatus = Wire.endTransmission();
  }
/**
Power up vfd
1BH,4AH,42H,Ps
Ps = 30H VFD Power OFF
Ps = 31H VFD Power ON (Default)
*/
void vfd_power_on()
  {
    Serial.println("Power on"); 
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x42);
    
    write_byte(0x31);
    // Need to wait for cathodes to energise
    // 2 seconds seems to be long enough for capacitors from a cold start
    delay (2000);
  }


/**
reset vfd 
1BH,4AH,52H,53H
*/
void reset() {
    Serial.println("Reset");

    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x52);
    write_byte(0x53);
    //delay (2000);
}


// clear display
// 1BH,4AH,43H,44H
//  All the displayed character is erased. The write-in position and every box modify is not changed 
// If scrolling or blinking, scroll and blink are stopped and clear display.
// Data memory, FROM and User definable font data are no effect.
void clear_display() {
    Serial.println("clear");

    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x43);
    write_byte(0x44);
}

void text_clear() {
    Serial.println("text clear");
    write_byte(0x1B);
    write_byte(0x5B);
    write_byte(0x32);
    write_byte(0x4A); 
}



/**
Dimming
1BH,4AH,44H,Ps 
31h = 32%, 
35h = 100%
*/
void dimming() {
    Serial.println("dimming");
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x44);
    write_byte(0x31);
}


//  blink setting
//  1BH,6AH,42H,Ps
//  Ps = 30H approx 0.5s
//  Ps = 31H approx 1s
//  Ps = 32H approx 1.5s
void blink_setting() {
    Serial.println("blink setting");
    write_byte(0x1B);
    write_byte(0x6A);
    write_byte(0x42);
    write_byte(0x30);
}


/**   
Character Address Setting
1BH,4AH,57H,Pm,aL,aH
Pm: DW or Data memory
aL: Lower byte of Data memory
aH: Upper byte of Data memory
*/
void character_address() {
    Serial.println("Character Address");
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x57);
      
    write_byte(0x30);   //Pm: Ps = 30H DW, 31H Data memory
    write_byte(0x0002); //aL = DW lower byte
    write_byte(0x0000); //aH = DW upper byte

    //write_byte(0x07FF); //aL = DW lower byte
    //write_byte(0x0005); //aH = DW upper byte
}


/**
Font size select
1BH,4AH,46H,Pf
Pf = 30H 6x8 dot
Pf = 31H 8x16dot and 16x16 dot 
Pf = 32H 12x24 dot and 24x24 dot 
Pf = 33H 16x32 dot and 32x32 dot
*/
void font_size_select() {
    Serial.println("Font size");
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x46);
    
    write_byte(0x30); //Pf: Font size
}


/**
Charcter modify
1BH,4AH,6DH,Ps,Pf
Ps = Reverse
Pb = Magnified
Ps = 30H  Normal (not reverse) 
Ps = 31H  Reverse
Pb = 30H  Normal size
Pb = 31H  Double size letter in horizontal.
Pd = 32H  Double size letter in horizontal and vertical.
*/
void character_modify() {
    Serial.println("Modify"); 
    write_byte(0x1B);
    write_byte(0x5B);
    write_byte(0x4D);
    write_byte(0x30); 
    write_byte(0x30);  
}
/**
 * 
Ps = 30H  Normal (Remove reverse and blink) Ps = 31H  Reverse
Ps = 32H  Blink
Pb = 30H  Normal size
Pb = 31H  Double size letter in horizontal
*/
void text_modify() {
    write_byte(0x1B);
    write_byte(0x5B);
    write_byte(0x4D);
    write_byte(0x30);

}


/**
Clock setting 
[Code]1BH,6BH,53H,Pd,Ph,Pm 
[Function]Setting the clock data. The setting data is cleared, if the Reset command is input or power 
is turned off. 
Pd = Day of the week 
Ph = hour 
Pm = minute 
[Definable area]
Pd = 00H : Sunday 
Pd = 01H : Monday 
...
Pd = 06H : Saturday 
* Clock setting is canceled, when Pd is input value that is larger than 07H, or Ph is input value that is 
larger than 18H,or Pm is input value that is larger than 3CH. 
*/
void clock_setting() {
  
    write_byte(0x1B);
    write_byte(0x6B);
    write_byte(0x53);

    write_byte(0x00);
    write_byte(0x01);
    write_byte(0x02);
    
    
}

   
/**
Clock display
[Code] 1BH,6BH,55H,Ps,aL,aH,Pf
[Function] Clock is displayed. The display position and the font size can be freely decided. 
Ps = Display type select 
aL = Lower byte of address 
aH = Upper byte of address
Pf = Font size select 
[Definable area]
Ps = 00H : 24hour Ex.[12:34] 
Ps = 01H : 24hour + day of the week  Ex.[Wed._12:34] 
Ps = 10H : 12hour Ex.[PM_00:34] 
Ps = 11H : 12hour + day of the week  Ex.[Wed._PM_00:34] 
Pf = 30H : 6x8 dot 
Pf = 31H : 8x16dot
Pf = 32H : 12x24 dot 
Pf = 33H : 16x32 dot 
* When the clock data is not input, clock is not displayed. 
* The clock display is maintained until Clock display cancel "Clear display" RESET command is input 
or power is turned off. 
The clock display area
Graphic can be displayed excluding the clock display area.
The self adjustment for the position 
that cannot be displayed. 
* Excluding the clock display area can be input other display commands.
*/
void display_clock() {
    write_byte(0x1B);
    write_byte(0x6B);
    write_byte(0x55);
    
    write_byte(0x00);   // Ps ClockFormat
    write_byte(0x0000);   // aL 
    write_byte(0x0007);   // aH
    write_byte(0x30);   // Pf FontSize
}





/**
BMP box group: Setting the BMP box
 Code 1BH,5CH,42H,Pn,aL,aH,Pw,Ph
BMP box can be defined the 3 area to DW. The position of BMP box is set based on the address of DW.
To write data in BMP box, ??? BMP box select ??? is necessary.
Specifiable horizontal size is 256dot (100H) MAX. If horizontal size specify 256dot, Pw = 00H
Pn = Number of a BMP box 
aL = Lower byte of address 
aH = Upper byte of address 
Pw = BMP box width
Ph = BMP box height
 Definable area 
Pn = 31H  BMP box  
Pn = 32H  BMP box 
Pn = 33H  BMP box 
0000H   aL+aH*100   07FFH 
01H   Pw   00H (=100H)
01H   Ph   08H
*/
void bmp_box_setting(unsigned short boxId, unsigned short aAddress, unsigned short width, unsigned short height)
{
  Serial.println("bmp box group");

  write_byte(0x1B);
  write_byte(0x5C);
  write_byte(0x42);
  
  write_byte(boxId);        // BMP Box
  write_byte(aAddress);     // memory address
  write_byte(aAddress>>8);  //  memory address
  write_byte(width==256?0x00:width);        // box width (FD = 253)
  write_byte(height);       // box height
}


/**
BMP box select  
Code 1BH,5CH,48H,Pn  
Function Select of BMP box
Execution ???BMP box select ??? is necessary before ???Setting the Text box ???.
In case of writing by the specified dot writing, it is necessary to cancel this command.
Pn = 30H  Remove the BMP box 
Pn = 31H  BMP box 
Pn = 32H  BMP box 
Pn = 33H  BMP box 
*/
void bmp_box_select(unsigned short boxId) {
  Serial.println("bmp box select");

  write_byte(0x1B);
  write_byte(0x5C);
  write_byte(0x48);
  write_byte(boxId);
  
}


/**
BMP Box Modify
Code: 1BH,5CH,4DH,Ps
Reverse and blink Setting the selected BMP box.
This command doesn???t effects the other BMP box. If the selected BMP box remove, modify setting is removed
Ps = 30H  Normal 
Ps = 31H  Reverse
Ps = 32H  Blink
*/
void bmp_box_modify() {
  Serial.println("bmp box modify");
  write_byte(0x1B);
  write_byte(0x5C);
  write_byte(0x4D);
  write_byte(0x31);
}


/**
Input BMP data in the BMP box
Code: 1BH,5CH,5DH,nL,nH,Pd ...Pd
BMP data is written the BMP box
Number of definable data is due to BMP box size. 
If the data is over range, the over range data is rewritten the final address.

nL = Lower byte of number of definition byte 
nH = Upper byte of number of definition byte 
Pd = BMP data
Pn  = BMP box size (Pw Ph)
*/
void input_bmp_data_box() {
  Serial.println("bmp box data input"); 
  write_byte(0x1B);
  write_byte(0x5C);
  write_byte(0x5D);
  
  write_byte(0x0000);     // nL
  write_byte(0x0000>>8);  // nH


  unsigned short x = 0x00;
  for (int i = 0; i <= 0x0008; i++) {
      fast_byte(0x0F);   // actual pixels
      x++;
      delay (1); 
    }

}
 
/**
Setting the Text box
 Code : 1BH,5BH,54H,Pn,aL,aH,Pq,Pf,Pc,Pl
 Function Setting the Text box. Text box can be defined up to 3 boxes. Text box position is defined
the address. (aL,aH)
 To write data in Textbox, execution ???Textboxselect??? command isn ecessary.
 Definable digit (Pc) and line (Pl) are due to the font size. Number of digit is counted to number
of bytes. (If the 4 characters are displayed with 16x16 dots, Pc=08H) 
---
Pn = Number of text box
Pn = 31H  Text box 
Pn = 32H  Text box 
Pn = 33H  Text box 
aL = Lower address of DW
aH = Upper address of DW
---
Pq = Shift of  4 dot at a setting address
Pq = 30H Setting address (no shift)
Pq = 31H Shift of +4 dot at a setting address 
Pq = 32H Shift of -4 dot at a setting address 
---
Pf = Font size
---
Pc= Number of the digit
---
Pl = Number of the line
*/
void set_text_box(unsigned short aAddress, unsigned short offset, unsigned short boxId, unsigned short boxLength) {
  Serial.println("set text box");
  write_byte(0x1B);
  write_byte(0x5B);
  write_byte(0x54);

  write_byte(boxId);   // Text box No.
  write_byte(aAddress); // address
  write_byte(aAddress>>8); // upper address
  
  write_byte(offset);   //  Pq, no shift
  
  write_byte(0x30);   //  Pf, font size
  write_byte(boxLength);   //  Pc, number of digits the text box should contain (0x10 = 16, 0x2A = 42)
  write_byte(0x01);   //  Pl, line to target 
}


/**
Setting the Text box
 Code : 1BH,5BH,48H,Pn
 Execution ??? Text box select ??? is necessary before ???Setting the Text box ???.
 In case of writing by the specified dot writing, it is necessary to cancel this command.
 Definable area 
Pn = 30H  Remove the Text box 
Pn = 31H  Text box  1
Pn = 32H  Text box  2
Pn = 33H  Text box  3
The commands from 4-2-3 to 4-2-9 are valid in selected Text box. Without selecting Text box, these commands are invalid.
*/
void text_box_select(unsigned char boxId) {
  write_byte(0x1B);
  write_byte(0x5B);
  write_byte(0x48);
  write_byte(boxId);
}



/**
Input BMP data in the BMP box  
Code : 1BH,5CH,5DH,nL,nH,Pd Pd  
Function:  BMP data is written the BMP box
 
Number of definable data is due to BMP box size. If the data is over range, the over range data is rewritten the final address.
nL = Lower byte of number of definition byte 
nH = Upper byte of number of definition byte 
Pd = BMP data 
 Definable area : Pn : BMP box size (Pwx Ph)
*/
void input_bmp_data() {
  Serial.println("input bmp data");
  write_byte(0x1B);
  write_byte(0x5C);
  write_byte(0x5D);
  
  write_byte(0x0000);
  write_byte(0x0005);
  
  write_byte(0xFF);   // actual pixels
  write_byte(0xFF);
  write_byte(0xFF);
  write_byte(0xFF);
  write_byte(0xFF);
   
}


/**
Character address setting
Code : 1BH,4AH,57H,Pm,aL,aH  
Function : Setting a character data write-in address.
Pm = DW or Data memory
aL = Lower byte of Data memory aH = Upper byte of Data memory
 Definable area 0000H   aL + aH * 100   07FFH (DW)
0000H   aL + aH * 100   4FFFH Ps = 30H DW
Ps = 31H Data memory
*/
void character_address_setting(unsigned short aAddress) {
  write_byte(0x1B);
  write_byte(0x4A);
  write_byte(0x57);

  write_byte(0x30);
  write_byte(aAddress ); //aL = DW lower byte
  write_byte(aAddress>>8); //aH = DW upper byte
}
/*
void draw_string_box(unsigned short aAddress, String aString) {
    // texr box setting
  set_text_box();
    // text box select
  text_box_select();
    // text box modify
  character_modify();
    // character display
  String str = aString;
  int str_len = str.length() + 1;
  char char_array[str_len];
  str.toCharArray(char_array, str_len);

  write_byte(' ');
  for (int i=0;i<str_len;i++) {
    //Serial.println(str[i]);
    write_byte((str[i]));
  }
    // cursor setting
}
*/
void draw_string(unsigned short aAddress, String aString) {
  write_byte(0x1B);
  write_byte(0x4A);
  write_byte(0x57);

  write_byte(0x30);
  write_byte(aAddress ); //aL = DW lower byte
  write_byte(aAddress>>8); //aH = DW upper byte

  font_size();

  String str = aString;
  int str_len = str.length() + 1;
  char char_array[str_len];
  str.toCharArray(char_array, str_len);

  write_byte(' ');
  for (int i=0;i<str_len;i++) {
    Serial.println(str[i]);
    write_byte((str[i]));
  }
  
}
/**
Font size
1BH,4AH,46H,Pf
Pm: DW or Data memory
Pf = 31H 8x16dot and 16x16 dot 
Pf = 32H 12x24 dot and 24x24 dot 
Pf = 33H 16x32 dot and 32x32 dot
*/
void font_size() {
  write_byte(0x1B);
  write_byte(0x4A);
  write_byte(0x46);
  write_byte(0x30); //Pf: Font size 

  write_byte(0x1B);
  write_byte(0x5B);
  write_byte(0x4D);
  write_byte(0x30);
}




   
/**                 
BMP data input 
//    [Code] 1BH,4AH,Pm,aL,aH,Ps,nL,nH,Pd...Pd
//    [Function] The BMP data is written in the DW(Display Window) or the Data memory. 
//    Pm= DW or Data memory 
//    aL = DW lower byte 
//    aH = DW upper byte 
//    Ps = Direction of writing 
//    nL = number of BMP data length lower byte 
//    nH = number of BMP data length upper byte 
//    Pd = BMP data 
//    * If X direction is selected as Ps and data is written in the last address, the data in the last address is 
//    overwritten with the remaining data.  
//    [Definable area] Pm = 30H : DW
//    Pm = 31H: Data memory 
//    0000H <= aL + aH * 100 <= 07FFH (DW)
//    0000H <= aL + aH * 100 <= 4FFFH (Data memory) 
//    Ps = 30H: Y direction 
//    Ps = 31H: X direction 
//    0001H <= nL + nH * 100 <= 0100H(DW: X direction) 
//    0001H <= nL + nH * 100 <= 0800H(DW: Y direction) 
//    0001H <= nL + nH * 100 <= 0A00H(Data memory: X direction) 
//    0001H <= nL + nH * 100 <= 5000H(Data memory: Y direction) 
*/
void bmp_data_input(unsigned short vec, unsigned short aAddress, unsigned short aSize) {
    Serial.println("bmp display");
    write_byte(0x1B);
    write_byte(0x4A);
    
    write_byte(0x30); // DW (display window) or Data memory
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(vec); //Direction of writing: Y or X, 31x 30y
    
    write_byte(aSize);//Size of pixel data in bytes (LSB)
    write_byte(aSize>>8);//Size of pixel data in bytes (MSB)

    unsigned short x = 0x00;
    for (int i = 0; i <= aSize; i++) {
      if (x > 255 && x < 320 || x > 576 && x < 720) {
        fast_byte(0x00);   // actual pixels
        //Serial.println(x);
      }
      else {
        fast_byte(0xCC);   // actual pixels
        //Serial.println(x);
      }
      x++;
      
    } 
}    


void draw_play_button(unsigned short aAddress, unsigned short aSize) {
    Serial.println("draw the play button");
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x30); // Draw in display window
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(0x31); //Direction of writing: Y or X

    write_byte(aSize);//Size of pixel data in bytes (LSB)
    write_byte(aSize>>8);//Size of pixel data in bytes (MSB)

    // draw the pixels
    write_byte(0x00);
    write_byte(0x00);
    write_byte(0x7C);
    write_byte(0x38);
    write_byte(0x10);
    write_byte(0x00);
    write_byte(0x00);
    write_byte(0x00);
}
void draw_pause_button(unsigned short aAddress, unsigned short aSize) {
    Serial.println("draw the play button");
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x30); // Draw in display window
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(0x31); //Direction of writing: Y or X

    write_byte(aSize);//Size of pixel data in bytes (LSB)
    write_byte(aSize>>8);//Size of pixel data in bytes (MSB)

    // draw the pixels
    write_byte(0x00);
    write_byte(0x00);
    write_byte(0x7C);
    write_byte(0x00);
    write_byte(0x7c);
    write_byte(0x00);
    write_byte(0x00);
    write_byte(0x00);
}
void draw_skip_button(unsigned short aAddress, unsigned short aSize) {
    Serial.println("draw the play button");
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x30); // Draw in display window
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(0x31); //Direction of writing: Y or X

    write_byte(aSize);//Size of pixel data in bytes (LSB)
    write_byte(aSize>>8);//Size of pixel data in bytes (MSB)

    // draw the pixels
    write_byte(0x00);
    write_byte(0x7C);
    write_byte(0x38);
    write_byte(0x10);
    write_byte(0x7C);
    write_byte(0x38);
    write_byte(0x10);
    write_byte(0x00);
}
void draw_back_button(unsigned short aAddress, unsigned short aSize) {
    Serial.println("draw the play button");
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x30); // Draw in display window
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(0x31); //Direction of writing: Y or X

    write_byte(aSize);//Size of pixel data in bytes (LSB)
    write_byte(aSize>>8);//Size of pixel data in bytes (MSB)

    // draw the pixels
    write_byte(0x10);
    write_byte(0x39);
    write_byte(0x55);
    write_byte(0x11);
    write_byte(0x11);
    write_byte(0x1F);
    write_byte(0x00);
    write_byte(0x00);
}


void draw_equal(unsigned short aAddress) {
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x30); // Draw in display window
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(0x31); //Direction of writing: Y or X

    write_byte(0x0100);//Size of pixel data in bytes (LSB)
    write_byte(0x0100>>8);//Size of pixel data in bytes (MSB)

    //write_byte(0xFF);
    //write_byte(0x06);
    //write_byte(0xFF);

    unsigned short f = 0x00;
    for (int i = 0; i <= 15; i++) { // minus one to keep 0
        unsigned short x = 0x00;
        randNumber = random(1, 8);
        for (int i = 0; i <= 15; i++) { // minus one to keep 0
          
          switch (randNumber) {
            case 1:
              fast_byte(0x01);
              break;
            case 2:
              fast_byte(0x03);
              break;
            case 3:
              fast_byte(0x07);
              break;
            case 4:
              fast_byte(0x0F);
              break;
            case 5:
              fast_byte(0x1F);
              break;
            case 6:
              fast_byte(0x3F);
              break;
            case 7:
              fast_byte(0x7F);
              break;
            case 8:
              fast_byte(0xFF);
              break;
            default:
            // statements
            break;
}
          x++;
        }
      f++;
    }

    /*
    unsigned short x = 0x00;
    for (int i = 0; i <= 255; i++) { // minus one to keep 0
      fast_byte(0xFF);   // actual pixels
      //fast_byte(0x26);   // actual pixels
      //Serial.println(x);
      x++;
    }
    */
}


void draw_mode_button(unsigned short aAddress, unsigned short aSize) {
    Serial.println("draw the play button");
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x30); // Draw in display window
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(0x31); //Direction of writing: Y or X

    write_byte(aSize);//Size of pixel data in bytes (LSB)
    write_byte(aSize>>8);//Size of pixel data in bytes (MSB)

    // draw the pixels
    write_byte(0x00);
    write_byte(0x54);
    write_byte(0x54);
    write_byte(0x54);
    write_byte(0x54);
    write_byte(0x54);
    write_byte(0x54);
    write_byte(0x00);
}

void draw_analyser(unsigned short aAddress) {
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x30); // Draw in display window
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(0x30); //Direction of writing: Y or X

    write_byte(0x0024);//Size of pixel data in bytes (LSB)
    write_byte(0x0024>>8);//Size of pixel data in bytes (MSB)

    unsigned short x = 0;
    for (int i = 0; i <= 34; i++) {
      if (x > 0 && x < 7 || x > 9 && x < 16 || x > 18 && x < 25) {
        fast_byte(0xFFFF);   // actual pixels
      }
      else if (x > 28) {
        fast_byte(0xFF);   // actual pixels
      }
      else {
        fast_byte(0x00);   // actual pixels
      }   
      x++;    
    } 
    
}




short channelOneMax = 0;
short channelTwoMax = 0;

void draw_equaliser(unsigned short aAddress) {
    write_byte(0x1B);
    write_byte(0x4A);
    write_byte(0x30); // Draw in display window
    write_byte(aAddress ); //aL = DW lower byte
    write_byte(aAddress>>8); //aH = DW upper byte
    write_byte(0x31); //Direction of writing: Y or X

    write_byte(0x0100);//Size of pixel data in bytes (LSB)
    write_byte(0x0100>>8);//Size of pixel data in bytes (MSB)

    //write_byte(0xFF);
    //write_byte(0x06);
    //write_byte(0xFF);

    unsigned short f = 0x00;
    for (int i = 0; i <= 15; i++) { // minus one to keep 0
        unsigned short x = 0x00;
        randNumber = random(1, 8);
        for (int i = 0; i <= 15; i++) { // minus one to keep 0
          
          switch (randNumber) {
            case 1:
              if (channelOneMax >= randNumber) {
                
              }
              fast_byte(0x01);
              break;
            case 2:
              fast_byte(0x03);
              break;
            case 3:
              fast_byte(0x07);
              break;
            case 4:
              fast_byte(0x0F);
              break;
            case 5:
              fast_byte(0x1F);
              break;
            case 6:
              fast_byte(0x3F);
              break;
            case 7:
              fast_byte(0x7F);
              break;
            case 8:
              fast_byte(0xFF);
              break;
            default:
            // statements
            break;
}
          x++;
        }
      f++;
    }
}
