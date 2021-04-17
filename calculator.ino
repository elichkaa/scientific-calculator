#include <LiquidCrystal.h>
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);
int number = 0;
boolean newData = false;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(2, 1);
  Serial.begin(9600);
}

void loop() {
 int num = readNumber();
 lcd.print(num);
}

int readNumber(){
  if (Serial.available() > 0) {
  number = Serial.parseInt();
 }
 return number;
}
