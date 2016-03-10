// Basic bluetooth test sketch. HC-0x_FC-114_01_9600
//  Uses hardware serial to talk to the host computer and software serial for communication with the bluetooth module
//
//  Pins
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  Arduino D3 to BT RX through a voltage divider
//  Arduino D2 BT TX (no need voltage divider)
//
//  When a command is entered in the serial monitor on the computer 
//  the Arduino will relay it to the bluetooth module and display the result.
//
// The HC-0x FC-114 modules require CR and NL

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3); // RX | TX
//SoftwareSerial RFID(5, 6); // RX and TX
SoftwareSerial RFID(6, 5); // RX and TX

int data1 = 0;
int ok = -1;
int yes = 10;
int no = 12;
char c = ' ';
boolean NL = true;

int speed = 9600;

// use first sketch in http://wp.me/p3LK05-3Gk to get your tag numbers
//int tag1[14] = {2,52,48,48,48,56,54,66,49,52,70,51,56,3};
//int tag2[14] = {2,52,48,48,48,56,54,67,54,54,66,54,66,3};
int tag1[14] = {2, 48, 67, 48, 48, 49, 50, 54, 70, 49, 48, 54, 49, 3}; 
int tag2[14] = {2, 48, 66, 48, 48, 54, 69, 65, 54, 51, 52, 70, 55, 3}; 
int newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // used for read comparisons

boolean comparetag(int aa[14], int bb[14]) {
  boolean ff = false;
  int fg = 0;
  for (int cc = 0 ; cc < 14 ; cc++) {
    if (aa[cc] == bb[cc]) {
      fg++;
    }
  }
  if (fg == 14) {
    ff = true;
  }
  return ff;
}

void setup() {
  Serial.begin(speed);
  Serial.print("Sketch HC-0x_FC-114_01_");
  Serial.println(speed);
  Serial.println("Arduino with HC-0x FC-114 is ready");
  Serial.println("Make sure Both NL & CR are set");
  Serial.println("");
      
  // FC-114 default baud rate is <speed>
  BTSerial.begin(speed);  
  Serial.print("BTserial started at ");
  Serial.println(speed);
  Serial.println("");

  BTSerial.print("Sketch HC-0x_FC-114_01_");
  BTSerial.println(speed);
  BTSerial.println("Arduino with HC-0x FC-114 is ready");
  BTSerial.println("Make sure Both NL & CR are set");
  BTSerial.println("");

  BTSerial.print("BTserial started at ");
  BTSerial.println(speed);
  BTSerial.println("");

  RFID.begin(speed);    // start serial to RFID reader
  Serial.begin(speed);  // start serial to PC 
  pinMode(yes, OUTPUT); // for status LEDs
  pinMode(no, OUTPUT);

}

void checkmytags() { // compares each tag against the tag just read
  ok = 0; // this variable helps decision-making,
  // if it is 1 we have a match, zero is a read but no match,
  // -1 is no read attempt made
  if (comparetag(newtag, tag1) == true) {
    ok++;
  }
  if (comparetag(newtag, tag2) == true) {
    ok++;
  }
}

void readTags() {
  ok = -1;

  if (RFID.available() > 0) {
    // read tag numbers
    delay(100); // needed to allow time for the data to come in from the serial buffer.

    for (int z = 0 ; z < 14 ; z++) { // read the rest of the tag
      data1 = RFID.read();
      newtag[z] = data1;
      Serial.print(data1);
      BTSerial.print(data1);
    }
    RFID.flush(); // stops multiple reads
    Serial.println();
    BTSerial.println();
    checkmytags();
    //delay(2000);
  }


 // now do something based on tag type
  if (ok > 0) { // if we had a match
    Serial.println("Accepted");
    BTSerial.println("Accepted");
    digitalWrite(yes, HIGH);
    delay(1000);
    digitalWrite(yes, LOW);

    ok = -1;
  }
  else if (ok == 0) { // if we didn't have a match
    Serial.println("Rejected");
    BTSerial.println("Rejected");
    digitalWrite(no, HIGH);
    delay(1000);
    digitalWrite(no, LOW);

    ok = -1;
  }

  
}

void loop() {
  readTags();
}

