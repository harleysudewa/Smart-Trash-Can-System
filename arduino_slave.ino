#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <Wire.h>

#define SLAVE_ADDR 9

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Port Soft Serial
    #include <SoftwareSerial.h>
    SoftwareSerial softSerial(/*RX =*/4, /*TX =*/5);
    #define FPSerial softSerial
#else
    #define FPSerial Serial1
#endif

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
#if (defined ESP32)
    FPSerial.begin(9600, SERIAL_8N1, /*RX =*/D3, /*TX  =*/D2);
#else
    FPSerial.begin(9600);
#endif

  Serial.begin(115200);

  Serial.println();
  Serial.println("Initializing DFPlayer... (May take 3-5 seconds)");
  
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  // Komunikasi serial dengan DFPlayer Mini
    Serial.println("Unable to begin:");
    Serial.println("1. Please recheck the connection!");
    Serial.println("2. Please insert the SD card!");
    while(true){
      delay(0);
    }
  }
  Serial.println(F("DFPlayer Mini online!"));
  
  myDFPlayer.volume(30);

  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);

  /*Serial.println("Scanning...");
  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at address 0x");
      Serial.println(i, HEX);
    }
  }*/
}

void loop() {
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
}

// Fungsi untuk menerima data dari Master
void receiveEvent(int howMany) {
  while (0 < Wire.available()) {
    int activateSpeaker = Wire.read();
    if (activateSpeaker == 1) {
      myDFPlayer.play(3);
    } else if (activateSpeaker == 2) {
      myDFPlayer.play(4);
    } else if (activateSpeaker == 3) {
      myDFPlayer.play(5);
    } else if (activateSpeaker == 4) {
      myDFPlayer.play(6);
    } else if (activateSpeaker == 5) {
      myDFPlayer.play(2);
    } else if (activateSpeaker == 6) {
      myDFPlayer.play(1);
    } 
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}