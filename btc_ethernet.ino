

#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <Servo.h>

String currentLine;
String btcBalance;
String old_btcBalance;

Servo myServo;

#define txPin 9

SoftwareSerial LCD = SoftwareSerial(0 , txPin);
const int LCDdelay=10;  // conservative, 2 actually works


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };
//You need to put your server's domain name here.
char server[] = "example.com";
IPAddress ip(192,168,0,177);
EthernetClient client;

void lcdPosition(int row, int col) {
  LCD.write(0xFE);   //command flag
  LCD.write((col + row*64 + 128));    //position 
  delay(LCDdelay);
}
void clearLCD(){
  LCD.write(0xFE);   //command flag
  LCD.write(0x01);   //clear command.
  delay(LCDdelay);
}
void backlightOn() {  //turns on the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(157);    //light level.
  delay(LCDdelay);
}
void backlightOff(){  //turns off the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(128);     //light level for off.
   delay(LCDdelay);
}
void serCommand(){   //a general function to call the command flag for issuing all other commands   
  LCD.write(0xFE);
}

void setup() {
  myServo.attach(3);
  myServo.write(170);
  Serial.begin(9600);
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
  clearLCD();
  lcdPosition(0,0);
  LCD.print("Starting up...");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);

  if (client.connect(server, 80)) {
    // Make a HTTP request:
    client.println("GET /lcurrent.btc HTTP/1.1");
    client.println("Host: example.com"); //put your server here again
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.println("connection failed");
  }
}

void update() {
  if (client.connect(server, 80)) {
    // Make a HTTP request:
    client.println("GET /current.btc HTTP/1.1");
    client.println("Host: example.com");
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.println("connection failed");
  }
}

void dance() {
  myServo.write(0);
  delay(2000);
  myServo.write(170);
  delay(2000);
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    if (c != '\0') {
      currentLine += c;
    }
    if (c == '\n') {
      currentLine = "";
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    btcBalance = currentLine;
    if (btcBalance != old_btcBalance) {
      clearLCD();
      lcdPosition(0,0);
      LCD.print("BTC Balance:");
      lcdPosition(1,0);
      LCD.print(btcBalance);
      dance();
      clearLCD();
      lcdPosition(0,0);
      LCD.print("Thanks!");
      delay(3000);
      clearLCD();
      lcdPosition(0,0);
      LCD.print("BTC Balance:");
      lcdPosition(1,0);
      LCD.print(btcBalance);
      old_btcBalance = btcBalance;
    
    }
    Serial.println(btcBalance);
    client.stop();

    // do nothing forevermore:
    delay(1000);
    update();
  }
}
