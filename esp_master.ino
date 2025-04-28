#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Wire.h>

#define ROW_NUM       4  // 4 rows
#define COLUMN_NUM    3  // 3 columns
#define SLAVE_ADDR    8
#define SLAVE_ADDR_2  9

char key_layout[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {D1, D2, D3, D4};  // Pin Baris
byte pin_column[COLUMN_NUM] = {D5, D6, D7}; // Pin Kolom

Keypad keypad = Keypad(makeKeymap(key_layout), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

const String correctCode = "1234";
String enteredCode = "";  // Variabel untuk menyimpan kode yang dimasukkan

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
 
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Enter Passcode:");
  Serial.begin(115200);

  /*Serial.println("Scanning...");
  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at address 0x");
      Serial.println(i, HEX);
    }
  }*/

  Wire.begin();
}

void loop() {

  char key = keypad.getKey(); // Membaca input keypad

  if (key) {
    if (key == '*') { // Reset jika '*' ditekan
      enteredCode = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Passcode:");
    } else if (key == '#') { // Periksa passcode jika '#' ditekan
      if (enteredCode == correctCode) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Password Benar!");
        Wire.beginTransmission(SLAVE_ADDR); // Alamat slave
        Wire.write(10);
        Wire.endTransmission();
        Wire.beginTransmission(SLAVE_ADDR_2);
        Wire.write(6);
        Wire.endTransmission();
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Password Salah!");
        Wire.beginTransmission(SLAVE_ADDR_2);
        Wire.write(5);
        Wire.endTransmission();
      }
      delay(2000);  // Tampilkan pesan selama 2 detik
      enteredCode = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Passcode:");
    } else {
      if (enteredCode.length() < lcdColumns) {
        lcd.setCursor(enteredCode.length(), 1);
        lcd.print(key);
        enteredCode += key; // Tambahkan karakter ke passcode
      }
    }
  }
  delay(500);  // Tambahkan jeda untuk mencegah bouncing

  Wire.requestFrom(SLAVE_ADDR, 1); // Meminta 1 byte data dari slave dengan alamat 0x08

  while (Wire.available() >= 1) {
    int trashCanData = Wire.read(); // Baca byte yang diterima
    if (trashCanData == 1) {
      Wire.beginTransmission(SLAVE_ADDR_2);
      Wire.write(1);
      Wire.endTransmission();
    } else if (trashCanData == 2) {
      Wire.beginTransmission(SLAVE_ADDR_2);
      Wire.write(2);
      Wire.endTransmission();
    } else if (trashCanData == 3) {
      Wire.beginTransmission(SLAVE_ADDR_2);
      Wire.write(3);
      Wire.endTransmission();
    } else if (trashCanData == 4) {
      Wire.beginTransmission(SLAVE_ADDR_2);
      Wire.write(4);
      Wire.endTransmission();
    } else if (trashCanData == 5) {
      Wire.beginTransmission(SLAVE_ADDR_2);
      Wire.write(7);
      Wire.endTransmission();
    } else if (trashCanData == 6) {
      Wire.beginTransmission(SLAVE_ADDR_2);
      Wire.write(8);
      Wire.endTransmission();
    } else if (trashCanData == 7) {
      Wire.beginTransmission(SLAVE_ADDR_2);
      Wire.write(9);
      Wire.endTransmission();
    }
    delay(500);  // Tambahkan jeda untuk mencegah bouncing
  }
}