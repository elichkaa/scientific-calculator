# Circuit
![numpad](/images/numpad.jpg "Numpad")

The numpad connected to the Arduino Pins.

![lcd](/images/lcd.jpg "LCD")
Integration between Arduino and LCD display.

![circuit](/images/circuit.jpg "Circuit")
Circuit as a whole.

# Code

# Components and their use
## 1. Keypad
> The Keypad requires the Keypad.h library, which we import at the beginning of our code file. 

In this project the input is taken through a keypad. In order to use the keypad it must be configured first. The Keypad function requires 5 arguments - the makeKeymap function with keys twodimensional array, pins corresponding to rows, pins corresponding to columns, number of rows, number of columns). 
``` 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
```
In our case we have 4 rows and 4 columns as constants. The pin arrays look like this:
```
rowPins = {11, 10, 9, 8};
colPins[cols] = {7, 6, 5, 4};`
```
They are correspondingly connected to Arduino pins 4-11. In the circuit the rowpins are connected to wires with a black end.

![[Pasted image 20210423181251.png]]

When creating the keypad we also have the makeKeymap function, which takes the twodimensional array keys as a parameter. In keys we define the values 
to which the key buttons will be mapped. This is the setup I will be using for this project. 
```
char keys[rows][cols] = {
  {'1','2','3','('},
  {'4','5','6',')'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};`
```
## 2. LCD Display
> The LCD display requires the LiquidCrystal.h library, which we import at the beginning of our code file.  

In this project we display the output on a LCD. This component has many connections, which are used for power and communication. The LCD must be connected to a potentiometer because its opacity needs to be controlled. 
The LCD function requires 5 arguments - rs, enable, and the four corresponding pins to d4, d5, d6, d7.

The RS controls where the characters will appear on screen. The Enable(E or EN) tells the LCD it will be receiving a command. Both are connected to digital Arduino pins. The D4-D7 are the output functions and they are used to send characters to the  screen. They are connected to Analog pins 2-5.

```
LiquidCrystal lcd(13, 12, A2, A3, A4, A5);
```
In order to start the arduino we need to connect it to power and to ground. Both pins on the right side of the data pins are fist to configure. The LED(-) pin must be connected to GND and the LED(+) pin to 5V. A resistor must be added to control the electric current. On the other side of the LCD pin board the GND and VCC pins must be connected to GND and 5V accordingly.

As a second step the Enable pin of the LCD must be connected to the 12th Arduino pin and the RS pin (LCD) to the 13th (Arduino).  These are the values that we add to the function in the code block above.

For the LCD display to perform to its full potential we need to connect it to a potentiometer. It controls the contrast of the displayed text. We bind Terminal 1 to GND and Terminal 2 to 5V. The wiper must be connected to the V0 LCD pin.

## 3. Arduino

