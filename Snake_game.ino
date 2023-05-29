
#include <FastLED.h>

//matrix settings
#define NUM_LEDS 256
#define DATA_PIN 2
#define BRIGHTNESS 8

//joystick settings
#define JOYSTICKX A2       //define the x direction of the joystick
#define JOYSTICKY A1       //define the y dierection of the joystick
#define JOYSTICK_BUTTON 12  //define the JOYSTICK_BUTTON

int snake[256];     // array of snake elements (16x16)
int snakeSize = 2;  // real snake size
int snakeSpeed = 500;

int row;  // row number
int col;  // column number

int lastDirection = 135;  // start direction
int i, newDirection, OlddX = 1, OlddY, f;

int red, green, blue, fred, fgreen, fblue;  //colors

CRGB leds[NUM_LEDS];


void setup() {
  red = random(0, 255);    //sets a ramdom number from 0-255 for the red value
  green = random(0, 255);  //sets a ramdom number from 0-255 for the green value
  blue = random(0, 255);
  fred = random(127, 255);
  fgreen = random(127, 255);
  fblue = random(127, 255);

  Serial.begin(9600);
  pinMode(JOYSTICKX, INPUT);               //initializes pinMode of Joystick X direction
  pinMode(JOYSTICKY, INPUT);               //initializes pinMode of Joystick Y direction
  pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);  //initializes joystick button and uses built in arduino resistor
  digitalWrite(JOYSTICK_BUTTON, HIGH);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  //sets up data connection between micro controller and LED strip so leds array can control color of LEDs
  FastLED.setBrightness(BRIGHTNESS);                    //sets the brightness of the LEDs
  for (i = 0; i <= 255; i++) {                          //sets all snake array values to 0
    snake[i] = 0;
  }

  for (i = 0; i <= snakeSize; i++) {  //creates snake on snake matrix
    snake[i] = lastDirection + i; //creates snake 3 long using last direction which is 135 in the matrix
  }

  f = random(0, 255);
  FastLED.show();
}

int Snakedirection(int last, int dX, int dY) {

  

  dX = map(dX, 0, 1023, -100, 100);  //scales down the analog joystick x input values of 0 to 1000 down to -1 to 1
  dY = map(dY, 0, 1023, -100, 100);  //scales down the analog joystick y input values of 0 to 1000 down to -1 to 1
  if (dX>10) {
     dX = 1;
     dY = 0;
  }
  else if (dX<-10) {
    dX = -1;
    dY = 0;
  }
  else if(dY>10) {
    dY = 1;
    dX = 0;
  }
  else if(dY<-10) {
    dY = -1;
    dX = 0;
  }
 else {
   dY = 0;
   dX = 0;

 }
  if (dX == 0 && dY == 0 && OlddX != dX) {  //checks if joystick is not being moved and sets dX (direction in X direction) to the previous direction so the snake keeps moving
    dX = OlddX;
  }
  if (dY == 0 && dX == 0 && OlddY != dY) {  //checks if joystick is not being moved and sets dY (direction in Y direction) to the previous direction so the snake keeps moving
    dY = OlddY;
  }
  int newDirection = last;
  if (dX != 0) {                           // moving in X direction
    if (row & 1) {                         
      if (col == 0 && dX == 1) {           //checks to see if snake is on column 0 going in 1 direction
        newDirection = last - 15;          //subtracts 15 from last to set new dirction of snake on other side of LED matrix
      } else if (col == 15 && dX == -1) {  //checks to see if snake is on column 15 going in -1 direction
        newDirection = last + 15;          //adds 15 from last to set new dirction of snake on other side of LED matrix
      } else newDirection = last + dX;     //if the smake is not on the edge of the board then just add last location with current dX
    } else {
      if (col == 0 && dX == 1) {           
        newDirection = last + 15;
      } else if (col == 15 && dX == -1) {
        newDirection = last - 15;
      } else newDirection = last - dX;
    }
  }
  if (dY < 0) {  // moving in Y DOWN direction
    if (row == 15 && dY == -1) {
      newDirection = col;
    } else if (row & 1) {
      newDirection = last + (col * 2) + 1;
    } else {
      newDirection = last + (16 - col - 1) + (16 - col);
    }
  }
  if (dY > 0) {  // moving in Y UP direction
    if (row == 0 && dY == 1) {
      newDirection = 255 - col;
    } else if (row & 1) {
      newDirection = last - (last - 16 * row) - (16 - col);
    } else {
      newDirection = last - (col * 2) - 1;  // не четная
    }
  }
  OlddX = dX;
  OlddY = dY;
  return newDirection;
}

int snakeMove(int snakeDirection) {

  for (i = 0; i <= 255; i++) {
    if (snake[i] == snakeDirection) {
      death();
    }
  }

  FastLED.clear();

  for (i = snakeSize; i >= 1; i--) {
    snake[i] = snake[i - 1];
  }

  snake[0] = snakeDirection;

  for (i = 0; i <= 255; i++) {
    if (snake[i]) {
      leds[snake[i]].setRGB(red, green, blue);
    }
  }

  FastLED.show();
  
  row = (int)(snakeDirection / 16);  // row number
  if (row & 1) {
    col = (row + 1) * 16 - snakeDirection - 1;
  } else {
    col = snakeDirection - row * 16;
  }
  return snakeDirection;
}

void food(int eaten) {
  if (eaten == f) {
    snakeSize++;
    f = random(0, 255);
    red = fred;
    green = fgreen;
    blue = fblue;
    fred = random(0, 255);
    fgreen = random(0, 255);
    fblue = random(0, 255);
    snakeSpeed = snakeSpeed / 1.1;
  } else {
    leds[f].setRGB(fred, fgreen, fblue);
    FastLED.show();
  }
}

void death() {
  snakeSize = 2;
  snakeSpeed = 500;
  red = 255;
  green = 0;
  blue = 0;
}

void color(boolean sw) {
  if (!sw) {

    red = random(0, 255);
    green = random(0, 255);
    blue = random(0, 255);
  }
}

void loop() {
  color(digitalRead(JOYSTICK_BUTTON));
  newDirection = Snakedirection(lastDirection, analogRead(JOYSTICKX), analogRead(JOYSTICKY));
  lastDirection = snakeMove(newDirection);
  food(newDirection);
  delay(snakeSpeed);
  Serial.println(analogRead(2));
  Serial.println(analogRead(1));  
  for (int i; i>255; i++)  {
    Serial.print(snake[i]);
    Serial.print(" ");
  }
  Serial.println(); // print a new line at the end of the array
  delay(1000); // wait for 1 second before printing again

}

