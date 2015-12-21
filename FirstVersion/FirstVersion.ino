
#include <SoftwareSerial.h>
#define SER 2 //define the serial date input pin
#define SRCLK 4 //define the serial input clock pin(the shift register clock input)
#define RCLK 3 //define the latch pin(storage register clock input)

SoftwareSerial rfid(7, 8);

byte j = 0;

//Prototypes
void check_for_notag(void);
void halt(void);
void parse(void);
void print_serial(void);
void read_serial(void);
void seek(void);
void set_flag(void);
void InitLeds(void);
int mapMacToIndex(int);
void turnOnAtIndex(int);


//Global var
int flag = 0;
int Str1[11];



int greenIndexes[4] = {1 , 3 , 4 , 6};
int redIndexes[4] = {0 , 2 , 5 , 7};

bool ledDictionary[4] = {false, false, false, false};

void setup() {
  // initialize the digital pin as an output.
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  digitalWrite(SRCLK, LOW);
  digitalWrite(RCLK, LOW);
  Serial.begin(9600);
  Serial.println("Start");
  rfid.begin(19200);
  //delay(10);
  InitLeds();
  halt();
}

void InitLeds() {
  for (int i = 0; i < 8; i++) {
    bitWrite(j, i, 1); //turn on all leds
    write595(j);
    delay(450);
  }
  for (int i = 0; i < 4; i++) {
    bitWrite(j, greenIndexes[i], 0); //turn off green leds
  }
  write595(j);
}
int mapMacToIndex(int value) {
  if (value == 34) {
    return 0;
  } else if (value == 156) { //Liron: theres an overflow in the id of one of the stamps... which turns out to this number. leaving as this for a while
    return 1;
  } else if (value == 157) { //Liron: theres an overflow in the id of one of the stamps... which turns out to this number. leaving as this for a while
    return 2;
  } else if (value == -1) {
    return -1;
  }
  return -1;
}


void loop() {  
  read_serial();
}


void turnOnAtIndex(int i) {
  if (ledDictionary[i] == false) {
    //led was off, turning on
    ledDictionary[i] = true;
    bitWrite(j, greenIndexes[i], 1);
    bitWrite(j, redIndexes[i], 0);
  }
  Serial.print("turning on at index: ");
  Serial.println(i);
  write595(j);
  //delay(10);
}
void write595(byte data)
{
  int i;
  for (i = 0 ; i < 8; i++)
  {
    if (data & 0x80)
    {
      Serial.println("SER=1");
      digitalWrite(SER, HIGH); //SER=1
    }
    else     {

      Serial.println("SER=1");
      digitalWrite(SER, LOW); //SER=0
    }


    data <<= 1;
    digitalWrite(SRCLK, HIGH); //SRCLK=1
    digitalWrite(SRCLK, LOW); //SRCLK=1
  }
  digitalWrite(RCLK, HIGH); //SRCLK=1
  digitalWrite(RCLK, LOW); //SRCLK=1
}

void check_for_notag()
{
  seek();
  delay(10);
  parse();
  set_flag();

  if (flag = 1) {
    seek();
    delay(10);
    parse();
  }
}

void halt()
{
  //Halt tag
  rfid.write((uint8_t)255);
  rfid.write((uint8_t)0);
  rfid.write((uint8_t)1);
  rfid.write((uint8_t)147);
  rfid.write((uint8_t)148);
}

void parse()
{
  while (rfid.available()) {
    if (rfid.read() == 255) {
      for (int i = 1; i < 11; i++) {
        Str1[i] = rfid.read();
      }

      //str = String(Str1[8], HEX);
    }
  }
}

void print_serial()
{
  if (flag == 1) {
    //print to serial port
    /*Serial.print(Str1[8], HEX);
    Serial.print(Str1[7], HEX);
    Serial.print(Str1[6], HEX);
    Serial.print(Str1[5], HEX);
    Serial.println();
    */

    int rtnIndex = mapMacToIndex(Str1[6]);

    Serial.print("Turned on index #: ");
    Serial.println(rtnIndex);

    if (rtnIndex >= 0) {
      turnOnAtIndex(rtnIndex);
    }
    delay(20);
    //check_for_notag();
  }
}

void read_serial()
{
  seek();
  delay(10);
  parse();
  set_flag();
  print_serial();
  delay(10);
}

void seek()
{
  //search for RFID tag
  rfid.write((uint8_t)255);
  rfid.write((uint8_t)0);
  rfid.write((uint8_t)1);
  rfid.write((uint8_t)130);
  rfid.write((uint8_t)131);
  delay(10);
}

void set_flag()
{
  if (Str1[2] == 6) {
    flag++;
  }
  if (Str1[2] == 2) {
    flag = 0;
  }
}

