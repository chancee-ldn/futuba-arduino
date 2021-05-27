#include <Wire.h>

int busStatus;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars]; // temporary array for use when parsing

// variables to hold the parsed data
char command[numChars] = {0};
char message[numChars] = {0};

boolean newData = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();

  draw_player_interface();
  //text_box("an array of text");
}


void loop() {
  recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        // this temporary copy is necessary to protect the original data
        //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
}


void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void text_box(unsigned char aAddress, unsigned char offset, unsigned char boxId, String aString) {
    // texr box setting
  set_text_box(aAddress, offset, boxId);
    // text box select
  text_box_select(boxId);
    // text box modify
  text_modify();
    // character display
  String str = aString;
  int str_len = str.length() + 1;
  char char_array[str_len];
  str.toCharArray(char_array, str_len);

  for (int i=0;i<str_len;i++) {
    write_byte((str[i]));
  }
   
  Serial.println("running set up of box");
  
}

void draw_player_interface() {

  // address, offset, boxid, string
  text_box(0x0000, 0x30, 0x31, "Song title");
  text_box(0x0784, 0x30, 0x32, "Album title");
  
  draw_mode_button(0x0007, 0x0008);
  draw_skip_button(0x0377, 0x0008);
  draw_play_button(0x0417, 0x0008);
  draw_skip_button(0x04C7, 0x0008);
  draw_back_button(0x07C7, 0x0008);
  
}


void parseData() {      // split the data into its parts
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(command, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    strcpy(message, strtokIndx);
}

void showParsedData() {
        String s = command; // switching char array to string so comparisons work. lazy, but easy to understand
        String c = message;
        if (s == "on") {
          digitalWrite(LED_BUILTIN, HIGH);
        } else if (s == "off") {
          digitalWrite(LED_BUILTIN, LOW);
        } else if (s == "play") {
          draw_skip_button(0x0377, 0x0008);
        } else if (s == "song") {
          //text_box(0x000A, 0x31, c);
          draw_player_interface();
        } else if (s == "reset") {
          clear_display();
        } else {
          Serial.print("string not matching ");
        }
}
