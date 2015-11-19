
/*****************************
     RFID-powered lockbox

This sketch will move a servo when
a trusted tag is read with the 
ID-12/ID-20 RFID module

Pinout for SparkFun RFID USB Reader
Arduino ----- RFID module
5V            VCC
GND           GND
D2            TX

Pinout for SparkFun RFID Breakout Board
Arduino ----- RFID module
5V            VCC
GND           GND
D2            D0

Connect the servo's power, ground, and
signal pins to VCC, GND,
and Arduino D9

If using the breakout, you can also 
put an LED & 330 ohm resistor between 
the RFID module's READ pin and GND for 
a "card successfully read" indication

by acavis, 3/31/2015


 LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 
Inspired by & partially adapted from
http://bildr.org/2011/02/rfid-arduino/

******************************/

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#include <math.h>



// Choose two pins for SoftwareSerial
SoftwareSerial rSerial(10,11); // RX, TX    input ten 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12,13, 5, 4, 3, 2);

// For SparkFun's tags, we will receive 16 bytes on every
// tag read, but throw four away. The 13th space will always
// be 0, since proper strings in Arduino end with 0

// These constants hold the total tag length (tagLen) and
// the length of the part we want to keep (idLen),
// plus the total number of tags we want to check against (kTags)
const int tagLen = 16;
const int idLen = 13;
const int kTags = 52;
//hearts, diamons, spades, clubs
// Put your known tags here!
char knownTags[kTags][idLen] = {
             "09001738DCFA", //1s //A spade
             "090016FCA645", //2s
             "090017268FB7", //3s
             "09001677761E", //4s
             "0900331EE2C6", //5s
             "090033FDF334", //6s
             "090016AE8E3F", //7s
             "09003449A3D7",//8s
             "0900330A5161",//9s
             "090032ED4096",//10s
             "090033FF2BEE",//11s J
             "09001731E4CB",//12s Q
             "0900345B1573",//13s K
             "090034AA23B4", // 1c Ace
             "090033861BA7", //2c //2 clubs
             "09001715AAA1", // 3c 
             "0900347A7B3C", // 4c 
             "0900169873F4", // 5c
             "09003449D0A4", // 6c
             "0900343EA1A2", // 7c
             "090018418FDF", // 8c
             "090016AC9C2F", //9c  // 9 clubs
             "0900342DABBB", // 10c
             "090033DFF316", // 11c J
             "0900338648F4", // 12c Q
             "09003386922E", // 13c K
             "0900344F3C4E", //Ah //A hearts
             "090017268BB3", //2h  //2 hearts   
             "09003309281B", //3h
             "090034366962", //4h
             "0900334FA3D6", //5h
             "09003449C0B4", //6h
             "090034587015", //7h
             "090034365C57", //8h
             "090033204953", //9h
             "090017083C2A", //10h
             "0900349F9537", //11h
             "090034790B4F", //12h
             "0900175E1757", //13h
             "090016C1CB15", //1d    
             "090016D34B87", //2d   
             "090017E315E8", //3d   
             "09003480EF52", //4d 
             "0900172F2312", //5d 
             "090033EF09DC", //6d
             "09003406556E", //7d 
             "0900336698C4", //8d 
             "0900334A4434", //9d 
             "09003446D1AA", //10d 
             "090033E64E92", //11d 
             "0900343BD8DE", //12d 
             "090033DE7C98" //13d    
};

char keyTags[kTags][4] = {
  "s1", 
  "s2",
  "s3",
  "s4",
  "s5",
  "s6",
  "s7",
  "s8",
  "s9",
  "s10",
  "s11",
  "s12",
  "s13",
  "c1",
  "c2",
  "c3",
  "c4",
  "c5",
  "c6",
  "c7",
  "c8",
  "c9", 
  "c10",
  "c11",
  "c12",
  "c13",
  "h1",
  "h2", 
  "h3",
  "h4",
  "h5",
  "h6",
  "h7",
  "h8",
  "h9",
  "h10",
  "h11",
  "h12",
  "h13",
  "d1",
  "d2",  
  "d3",  
  "d4",
  "d5", 
  "d6",
  "d7",
  "d8",
  "d9",
  "d10", 
  "d11", 
  "d12", 
  "d13"   
};

// Empty array to hold a freshly scanned tag
char newTag[idLen];

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 39, 41,43, 45 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 33, 35, 37 };

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
  // Starts the hardware and software serial ports
   Serial.begin(9600);
   rSerial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  Serial.println("Booting up!");


}

void loop() {
  // Counter for the newTag array
  int i = 0;
  // Variable to hold each byte read from the serial buffer
  int readByte;
  // Flag so we know when a tag is over
  boolean tag = false;
/*
  long inputInt = readFromKeypad();
  if(inputInt != 0)
  {
    Serial.print("this is the main loop bro ");
    Serial.println(inputInt);  
  }  

*/
  writeAndRead("How much do you call");

  // This makes sure the whole tag is in the serial buffer before
  // reading, the Arduino can read faster than the ID module can deliver!
 // Serial.println(rSerial.available());
  if (rSerial.available() == tagLen) {
    tag = true;
    Serial.println("avilable tag");
  }

  if (tag == true) {
    while (rSerial.available()) {
      // Take each byte out of the serial buffer, one at a time
      readByte = rSerial.read();
      Serial.println("reading tag");
      /* This will skip the first byte (2, STX, start of text) and the last three,
      ASCII 13, CR/carriage return, ASCII 10, LF/linefeed, and ASCII 3, ETX/end of 
      text, leaving only the unique part of the tag string. It puts the byte into
      the first space in the array, then steps ahead one spot */
      if (readByte != 2 && readByte!= 13 && readByte != 10 && readByte != 3) {
        newTag[i] = readByte;
        i++;
      }

      // If we see ASCII 3, ETX, the tag is over
      if (readByte == 3) {
        tag = false;
      }

    }
  }


  // don't do anything if the newTag array is full of zeroes
  if (strlen(newTag)== 0) {
    return;
  }

  else {
    int total = 0;
    Serial.println("looking for tag");
    for (int ct=0; ct < kTags; ct++){
        total += checkTag(newTag, knownTags[ct]);
    }

    // If newTag matched any of the tags
    // we checked against, total will be 1
    if (total > 0) {

      // Put the action of your choice here!
      
      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 1);
      lcd.print("Success!");
      lcd.setCursor(0, 2);
      lcd.print(newTag);
       Serial.println("Success!");
    }

    else {
        // This prints out unknown cards so you can add them to your knownTags as needed
        Serial.print("Unknown tag! ");
        Serial.print(newTag);
        Serial.println();
    }
  }

  // Once newTag has been checked, fill it with zeroes
  // to get ready for the next tag read
  for (int c=0; c < idLen; c++) {
    newTag[c] = 0;
  }
}

// This function steps through both newTag and one of the known
// tags. If there is a mismatch anywhere in the tag, it will return 0,
// but if every character in the tag is the same, it returns 1
int checkTag(char nTag[], char oTag[]) {
    for (int i = 0; i < idLen; i++) {
      if (nTag[i] != oTag[i]) {
        return 0;
      }
    }
  return 1;
}

//allows you to pass text
//waits for input and returns it
long writeAndRead(String text)
{
    //when text is greater than 20 wrap
    int lines =round(text.length()/20);
    String textArray[3];
    int cursorValue =splitString(text, textArray ,lines);
    
    long inputInt = readFromKeypad(lines, cursorValue );
    if(inputInt != -1)
    {
      Serial.print("this is the main loop bro ");
      Serial.println(inputInt);  
    }  
    return inputInt;   
}

//text = string to be printed
//ArrayText = array of empty strings with returned strings
int splitString(String text, String *arrayText, int lines)
{
  int end=0;
  for(int i=0; i < lines && lines < 5; i++)
  {
    end = ((i+1) * 20)+i;
    if( text.length() < end)
        end= text.length();
    arrayText[i]= text.substring((i*20)+i,end);   
    lcd.setCursor(0, i+1);
    lcd.print(arrayText[i]);
  }
  return end;
}
//function used to read from keyPad
//# = used for enter
//* =  used for backspace
long readFromKeypad(int lines, int cursorValue)
{
  char input[10];
  long i=0;
  char key = keypad.getKey();
  if (key)
  {
    while( key != '#')
    {
      if(key)
      {
        if(key == '*')
        { 
          if(i > 0)    //check for going under index
          {
            i--;
          }
          input[i]= '\0';
        }
        else
        {
          if( i < 9)  //check for going over
          {
            input[i]= key;
            i++;
            
          }
        }
        lcd.setCursor(0, lines+1);
        lcd.print("                     ");
        lcd.print(atol(input));
        Serial.println(atol(input));
        //lcd.print(atol(input));
        
       }
       key = keypad.getKey();
     }
  }
  else
    return -1;

  
  return atol(input);
}

