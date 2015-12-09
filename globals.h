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

char   buffer[20];

struct Card
{
  int number;
  char suit;

};

struct AI
{
  Card c1;
  Card c2;

  Card table_cards[5];
  int table_size;

  // add to game, when start of hand set this variable if is_AI == 1
  int initial_stack;
  float threshold_value;
};

AI a;


