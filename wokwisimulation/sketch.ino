#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <Servo.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad Configuration
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 11, 12};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// Password Variables
String inputPassword = "";
String correctPassword;
bool verifyOldPassword = false;
String oldPasswordInput = "";
bool changeMode = false;
String newPassword = "";
const int greenLED = 8;
const int redLED = 9;
const int buzzer = 10;
bool vehicleUnlocked = false;
int attempts = 0;
bool locked = false;
void savePassword(String pass) {

  for (int i = 0; i < 4; i++) {
    EEPROM.write(i, pass[i]);
  }
}
String readPassword() {

  String pass = "";

  for (int i = 0; i < 4; i++) {
    pass += char(EEPROM.read(i));
  }

  return pass;
}
Servo vehicleLock;

void setup() {
  vehicleLock.attach(13);
  vehicleLock.write(0);
  pinMode(greenLED, OUTPUT);
pinMode(redLED, OUTPUT);
pinMode(buzzer, OUTPUT);

digitalWrite(greenLED, LOW);
digitalWrite(redLED, LOW);
digitalWrite(buzzer, LOW);
if (EEPROM.read(0) == 255) {
  savePassword("1234");
}
correctPassword = readPassword();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Vehicle Access");

  lcd.setCursor(0, 1);
  lcd.print("Enter Password");

  delay(2000);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
}

void loop() {
  if (locked) {
  return;
}

  char key = keypad.getKey();

  if (key) {

    // Accept only digits
    if (key >= '0' && key <= '9') {
      if (verifyOldPassword) {

  oldPasswordInput += key;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Current Pass:");

  lcd.setCursor(0, 1);
  lcd.print(oldPasswordInput);

  return;
}

  if (changeMode) {

    newPassword += key;

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("New Password:");

    lcd.setCursor(0, 1);
    lcd.print(newPassword);

  } else {
inputPassword += key;

lcd.clear();

lcd.setCursor(0, 0);
lcd.print("Password:");

lcd.setCursor(0, 1);

for(int i = 0; i < inputPassword.length(); i++) {
  lcd.print("*");
}
  }
}

    // # acts as ENTER
    if (key == '#') {
      if (verifyOldPassword) {

  if (oldPasswordInput == correctPassword) {

    verifyOldPassword = false;

    changeMode = true;
    newPassword = "";

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("New Password:");

  } else {

    verifyOldPassword = false;

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Wrong Password");

    delay(2000);

    lcd.clear();
    lcd.print("Enter Password");
  }

  return;
}
      if (changeMode) {

  savePassword(newPassword);

  correctPassword = readPassword();
 

  changeMode = false;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Password Saved");

  delay(2000);

  lcd.clear();
  lcd.print("Enter Password");

  return;
}

      lcd.clear();

      if (inputPassword == correctPassword) {
        attempts = 0;
        vehicleLock.write(90);
        vehicleUnlocked = true;
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);

        lcd.setCursor(0, 0);
        lcd.print("Vehicle Open");
lcd.setCursor(0,1);
lcd.print("Engine Ready");

      } else {
            digitalWrite(greenLED, LOW);

            digitalWrite(redLED, HIGH);
            attempts++;
            if (attempts >= 3) {

              locked = true;

              lcd.clear();

              lcd.setCursor(0, 0);
              lcd.print("SYSTEM LOCKED");

              lcd.setCursor(0, 1);
              lcd.print("WAIT 30 SEC");

              tone(buzzer, 1000);

              delay(3000);

              noTone(buzzer);

              delay(27000);

              attempts = 0;
              locked = false;

              lcd.clear();
              lcd.print("Enter Password");

              return;
            }

            tone(buzzer, 1000);
            delay(1000);
            noTone(buzzer);

            lcd.setCursor(0, 0);
            lcd.print("Access Denied");
      }

      delay(3000);
      
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, LOW);

      inputPassword = "";

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
    }

    // * acts as CLEAR
    if (key == '*') {

  // Current Password Mode
  if (verifyOldPassword) {

    if (oldPasswordInput.length() > 0) {
      oldPasswordInput.remove(oldPasswordInput.length() - 1);
    }

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Current Pass:");

    lcd.setCursor(0, 1);

    for (int i = 0; i < oldPasswordInput.length(); i++) {
      lcd.print("*");
    }

    return;
  }

  // New Password Mode
  if (changeMode) {

    if (newPassword.length() > 0) {
      newPassword.remove(newPassword.length() - 1);
    }

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("New Password:");

    lcd.setCursor(0, 1);

    for (int i = 0; i < newPassword.length(); i++) {
      lcd.print("*");
    }

    return;
  }

  // Login Mode
  if (inputPassword.length() > 0) {
    inputPassword.remove(inputPassword.length() - 1);
  }

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Password:");

  lcd.setCursor(0, 1);

  for (int i = 0; i < inputPassword.length(); i++) {
    lcd.print("*");
  }

  return;
}
    if (key == 'A') {

  verifyOldPassword = true;
  oldPasswordInput = "";

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Current Pass:");

  return;
}
if (key == 'D' && vehicleUnlocked) {

  vehicleLock.write(0);

  vehicleUnlocked = false;

  digitalWrite(greenLED, LOW);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Vehicle Locked");

  delay(2000);

  lcd.clear();

  lcd.print("Enter Password");

  return;
}
  }
}