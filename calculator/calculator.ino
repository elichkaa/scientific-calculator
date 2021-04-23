#include <LiquidCrystal.h>
typedef unsigned long ulong; // max_val = 4294967295

LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);
int number = 0;

boolean newData = false;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(2, 1);
  Serial.begin(9600);
}

void loop() {
 long a = readNumber();
 long b = readNumber();
 printOutput('+', a, b, a+b);
}

long readNumber(){
 char* arr;
 int i = 0;
 if (Serial.available() > 0) {
  arr[i] = (char)Serial.read();
  i++;
 }
 Serial.print(String(arr));
 long num = atol(String(arr).c_str());
 Serial.flush();
 return num;
}

void printOutput(char operation, long firstNumber, long secondNumber, ulong result){
 lcd.setCursor(0, 0);
 lcd.print(firstNumber);
 lcd.print(operation);
 lcd.print(secondNumber);
 lcd.setCursor(0, 1);
 lcd.print(result);
}
