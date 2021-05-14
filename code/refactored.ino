#include <Keypad.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, A2, A3, A4, A5);

//global vars
bool signIsContained = true;

//constants
const byte rows = 4;
const byte cols = 4;
const byte oneInput = 1;
const byte twoInputs = 2;

//containers
byte rowPins[rows] = {11, 10, 9, 8};
byte colPins[cols]= {7, 6, 5, 4};
char keys[rows][cols] = {
  {'1','2','3','('},
  {'4','5','6',')'},
  {'7','8','9','C'},
  {'.','0','/','D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  printInstructions();
  lcd.clear();
}

void loop()
{
  lcd.print("Choose an option.");
  lcd.setCursor(0,2);
  lcd.print("Press C to enter.");
  int option = getInput();
  lcd.clear();
  printChosen(option);
  redirectToCalculation(option);
  delay(1000);
  lcd.clear();
}

//Input functions
double getInput()
{
  String input = "";
  while(true)
  {
    char key = keypad.getKey();
    if (key)
    {
      if(key == 'C')
        break;
      else if(key == 'D')
        input = input.substring(0,input.length() - 1);
      else
        input += String(key);
      Serial.println("Current number: " + input);
    }
  }
  input.trim();
  return input.toDouble();
}

String getInputForBodmas()
{
  String input = "";
  int counter = 0;
  while(true)
  {
    char key = keypad.getKey();
    if (key)
    {
      if(key == 'C'){
        break;
      }
      else if(key == 'D'){
        input = input.substring(0,input.length() - 1);
      }
      else if(key == '/'){
        if(counter == 1){
        	input = input.substring(0,input.length() - 1);
        	key = '*';
      	}
      	else if(counter == 2){
           input = input.substring(0,input.length() - 1);
           key = '+';
        }
      	else if(counter == 3){
           input = input.substring(0,input.length() - 1);
           key = '-';
        }
        input += String(key);
        counter++;
      }
      else{
        input += String(key);
        counter = 0;
      }
      Serial.println("Current number: " + input);
    }
  }
  input.trim();
  return input;
}

double* getTwoNumberInput()
{
  static double nums[2];
  lcd.clear();
  lcd.print("Enter first num:");
  nums[0] = getInput();
  lcd.setCursor(0,2);
  lcd.print(nums[0]);
  delay(300);
  lcd.clear();
  lcd.print("Enter second num:");
  nums[1] = getInput();
  lcd.setCursor(0,2);
  lcd.print(nums[1]);
  delay(300);
  return nums;
}

double getOneNumberInput()
{
  double number;
  lcd.clear();
  lcd.print("Enter number:");
  number = getInput();
  lcd.setCursor(0,2);
  lcd.print(number);
  delay(500);
  return number;
}

//Calculation functions
void doTwoInputCalculation(char sign, double (*function)(double*))
{
  double result;
  double* multipleInputs;
  multipleInputs = getTwoNumberInput();
  lcd.clear();
  printTwoInputMathExpression(sign, multipleInputs);
  result = function(multipleInputs);
  printResult(result);
}

void doOneInputCalculation(String sign, double (*function)(double))
{
  double input, result;
  double printArray[1];
  input = getOneNumberInput();
  printArray[0] = input;
  lcd.clear();
  printOneInputMathExpression(sign, input);
  result = function(input);
  printResult(result);
}

String doBodmasFromString(String expr, char sign, double (*function)(double*))
{
  while(checkIfSignIsContained(sign, expr) == 1){
    int counter = 0;
    int innerCounter;
    String firstNumStr, secondNumStr, firstPart, secondPart;
    double result, firstNum, secondNum;
    while(expr.length() > counter)
    {
    if(expr[counter] == sign){
      	innerCounter = counter;
      	while((isDigit(expr[innerCounter - 1]) || expr[innerCounter - 1] == '.') && innerCounter - 1 >= 0)
        {
          firstNumStr += expr[innerCounter - 1];
          innerCounter--;
        }
      	innerCounter = counter;
      	while((isDigit(expr[innerCounter + 1]) || expr[innerCounter + 1] == '.') && innerCounter + 1 < expr.length())
        {
          secondNumStr += expr[innerCounter + 1];
          innerCounter++;
        }
      firstNumStr = reverseStr(firstNumStr);
      double arr[2];
      arr[0]=firstNumStr.toDouble();
      arr[1]=secondNumStr.toDouble();
      result = function(arr);
      int startOfFirstNumber = counter - firstNumStr.length();
      int endOfSecondNumber = counter+secondNumStr.length();
      firstPart = expr.substring(0, startOfFirstNumber);
      secondPart = expr.substring(endOfSecondNumber + 1, expr.length());
      expr = firstPart + String(result,3) + secondPart;
      break;
    }
  	counter++;
  }
  if(counter == expr.length())
  {
    signIsContained = false;
  }
  }
  return expr;
}

void doBodmasCalculation()
{
  String input;
  printInstructionsForBodmas();
  input = getInputForBodmas();
  printBodmasExpression(input);
  input = getInputWithoutBracketExpression(input);
  String result = getFinalExpression(input);
  String output = result.substring(0,result.length()-1);
  printResult(output.toDouble());
}

String getInputWithoutBracketExpression(String input){
  String bracketExpression = getBracketExpression(input);
  String resultFromBracketExpression = getFinalExpression(bracketExpression);
  bracketExpression = "(" + bracketExpression + ")";
  input.replace(bracketExpression, resultFromBracketExpression);
  return input;
}

String getFinalExpression(String expr)
{
  String result;
  result = doBodmasFromString(expr, '/', &division);
  result = doBodmasFromString(result, '*', &multiplication);
  result = doBodmasFromString(result, '+', &addition);
  result = doBodmasFromString(result, '-', &subtraction);
  return result;
}

bool checkIfSignIsContained(char sign, String expr)
{
  int n = 0;
  while(expr.length() > n){
    if(expr[n] == sign){
      if(n != 0 || (isDigit(expr[n-1]) && isDigit(expr[n+1])))
    	return true;
    }
    n++;
  }
  return false;
}

String getBracketExpression(String expr)
{
  int counter = 0;
  String bracketsExpr;
  bool inBrackets = false;
  while(expr.length() > counter)
  {
    if(inBrackets){
    	bracketsExpr += expr[counter];
    }
    if(expr[counter] == '('){
      	inBrackets = true;
    }
    else if(expr[counter] == ')'){
    	inBrackets = false;
    }
  	counter++;
  }
  return bracketsExpr.substring(0,bracketsExpr.length()-1);
}

String reverseStr(String str)
{
  int n = str.length();
  char temp;
  for (int i = 0; i < n / 2; i++){
	temp = str[i];
    str[i] = str[n - i - 1];
    str[n-i-1] = temp;
  }
  return str;
}

//Redirect functions
void redirectToCalculation(int option)
{
  double singleInput, result;
  double* multipleInputs;
  lcd.clear();
  switch (option) {
  case 1:
    doTwoInputCalculation('+', &addition);
    break;
  case 2:
    doTwoInputCalculation('-', &subtraction);
    break;
  case 3:
    doTwoInputCalculation('*', &multiplication);
    break;
  case 4:
    doTwoInputCalculation('/', &division);
    break;
  case 5:
    printWarning();
    doOneInputCalculation("sin ", &sine);
    break;
  case 6:
    printWarning();
    doOneInputCalculation("cos ", &cosine);
    break;
  case 7:
    printWarning();
    doOneInputCalculation("tan ", &tangent);
    break;
  case 8:
    doOneInputCalculation("sqrt of", &squareRoot);
    break;
  case 9:
    doOneInputCalculation("log n ", &logarithm);
    break;
  case 10:
    doTwoInputCalculation('^', &exponent);
    break;
  case 11:
    doOneInputCalculation("square of", &square);
    break;
  case 12:
    doBodmasCalculation();
    break;
  default:
    printError();
    break;
}
}

//Math functions
double addition(double* nums)
{
  return nums[0] + nums[1];
}

double subtraction(double* nums)
{
  return nums[0] - nums[1];
}

double multiplication(double* nums)
{
  return nums[0] * nums[1];
}

double division(double* nums)
{
  return nums[0] / nums[1];
}

double sine(double num)
{
  return sin(num);
}

double cosine(double num)
{
  return cos(num);
}

double tangent(double num)
{
  return tan(num);
}

double square(double num)
{
  return num*num;
}

double squareRoot(double num)
{
  return sqrt(num);
}

double logarithm(double num)
{
  return log(num);
}

double exponent(double* nums)
{
  return pow(nums[0], nums[1]);
}

//Print functions
void printTwoInputMathExpression(char sign, double* input)
{
  lcd.clear();
  lcd.print(input[0]);
  lcd.print(sign);
  lcd.print(input[1]);
}

void printOneInputMathExpression(String sign, double input)
{
  lcd.clear();
  lcd.print(sign);
  lcd.print(input);
}

void printWarning()
{
  lcd.clear();
  lcd.print("Works with");
  lcd.setCursor(0,2);
  lcd.print("radians");
  delay(600);
  lcd.clear();
}

void printResult(double result)
{
  lcd.setCursor(0,2);
  lcd.print(result);
}

void printChosen(int optionNumber)
{
  lcd.setCursor(0,0);
  lcd.print("Chosen option: ");
  lcd.setCursor(0,2);
  lcd.print(optionNumber);
  delay(500);
  lcd.clear();
}

void printError()
{
  lcd.setCursor(0,0);
  lcd.print("Invalid option.");
  lcd.setCursor(0,2);
  lcd.print("Try again.");
  delay(800);
  lcd.clear();
  return;
}

void printInstructions()
{
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Casuno!");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Instructions in");
  lcd.setCursor(0, 1);
  lcd.print("Serial Monitor.");
  delay(1000);
  lcd.clear();
  Serial.println("Choose an option:");
  Serial.println("	 1. Addition");
  Serial.println("	 2. Subtraction");
  Serial.println("	 3. Multiplication");
  Serial.println("	 4. Division");
  Serial.println("	 5. Sine");
  Serial.println("	 6. Cosine");
  Serial.println("	 7. Tangent");
  Serial.println("	 8. Square root of a number");
  Serial.println("	 9. Logarithm");
  Serial.println("	 10. Exponent");
  Serial.println("	11. Square of number");
  Serial.println("	12. BODMAS");
  Serial.println("  ");
  Serial.println("Guide for the numpad:");
  Serial.println("A substitutes (");
  Serial.println("B substitutes )");
  Serial.println("* substitutes . for floating point numbers");
  Serial.println("# is used for BODMAS signs.");
  Serial.println("C means Continue");
  Serial.println("D means Delete");
  Serial.println("To enter press C.");
  Serial.println("To delete press D.");
}

void printInstructionsForBodmas(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Instructions in");
  lcd.setCursor(0, 1);
  lcd.print("Serial Monitor.");
  Serial.println();
  Serial.println("BODMAS Instructions:");
  Serial.println("A substitutes (");
  Serial.println("B substitutes )");
  Serial.println("The # symbol is used to enter a sign.");
  Serial.println("Pressing # one time is used for division (/)");
  Serial.println("Pressing # two times is used for multiplication (*)");
  Serial.println("Pressing # three times is used for addition (+)");
  Serial.println("Pressing # four times is used for subtraction (-)");
  delay(600);
  lcd.clear();
  lcd.print("Enter expression");
}

void printBodmasExpression(String expr){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(expr);
}