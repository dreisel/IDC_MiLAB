
#include <SoftwareSerial.h>
#define SER 2 //define the serial date input pin
#define SRCLK 4 //define the serial input clock pin(the shift register clock input)
#define RCLK 3 //define the latch pin(storage register clock input)

SoftwareSerial rfid(7, 8);
SoftwareSerial xbee(10, 9);



//Prototypes
void check_for_notag(void);
void halt(void);
void parse(void);
void print_serial(void);
void read_serial(void);
void seek(void);
void set_flag(void);

//Global var
int flag = 0;
int Str1[11];

bool ledDictionary[4];

void setup() {
  // initialize the digital pin as an output.
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  digitalWrite(SRCLK, LOW);
  digitalWrite(RCLK, LOW);
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) { //initialize
    ledDictionary[i] = false;
  }


  Serial.println("Start");

  // set the data rate for the SoftwareSerial ports
  xbee.begin(9600);
  rfid.begin(19200);
  delay(10);
  halt();
}


int mapMacToIndex(unsigned long value) {
  if (value == 2231033468L) { 
     return 1;
  } else if (value == 835979288L) { //Liron: theres an overflow in the id of one of the stamps... which turns out to this number. leaving as this for a while
    return 3;
  }
  return 0;
}

String ReadFromRF() {

  read_serial();

}

void loop() {
  String val = ReadFromRF();
}





void turnOnAtIndex(int i) {
  Serial.print("turning on at index: ");
  Serial.println(i);

  byte j = 0x80;
  j >>= i;
  write595(j);
  delay(10);
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
int numOfDigits(int n) {
  int count = 0;
  while (n != 0)
  {
    n /= 10;           /* n=n/10 */
    ++count;
  }
  return count;
}
String str = String("");
int tIndex = 1;
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

    unsigned long val = 0;
    for (int i = 8; i > 4; i--) {
      
      int digits = numOfDigits(Str1[i]);
      for(int j = 0; j < digits; j++) {
        val = val* 10;
      }

      
      val += Str1[i];
      Serial.println(val);
    }
    Serial.println();
    Serial.print(Str1[8]);
    Serial.print(Str1[7]);
    Serial.print(Str1[6]);
    Serial.print(Str1[5]);
    Serial.println();
    Serial.println(val);

    //str = String(Str1[5]);
    //Serial.println(str);
    /*Serial.println(str);*/
    int rtnIndex = mapMacToIndex(val);

    Serial.print("Turned on index #: ");
    Serial.println(val);

    turnOnAtIndex(rtnIndex);
    //print to XBee module
    xbee.print(Str1[8], HEX);
    xbee.print(Str1[7], HEX);
    xbee.print(Str1[6], HEX);
    xbee.print(Str1[5], HEX);
    xbee.println();
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

