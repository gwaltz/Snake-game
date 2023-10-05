#include <Adafruit_NeoPixel.h>


#define MATRIX_PIN 12
#define JOYSTICKBUTTON 11

//define analog values from joystick as integers
const int JOYSTICK_XPIN = A3;
const int JOYSTICK_YPIN = A4;

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(256, MATRIX_PIN, NEO_GRB + NEO_KHZ800);
uint32_t RED = matrix.Color(255, 0, 0);
uint32_t GREEN = matrix.Color(0, 255, 0);
uint32_t BLUE = matrix.Color(0, 0, 255);
uint32_t BLUEDEATH = matrix.Color(0, 50, 250);

int snake[256];

int snakeNewPosition = 135, snakeLength, apple, col, row, snakeDirection, lastDirection;
int NewPosition, xpinval, ypinval;

unsigned long previousMillis;
unsigned long gameRate;
int numApplesEaten = 0;



struct Coordinate {
  int row;
  int col;
};

void setup() {
  pinMode(JOYSTICK_XPIN, INPUT);  //initializes pinMode of Joystick X direction
  pinMode(JOYSTICK_YPIN, INPUT);
  pinMode(JOYSTICKBUTTON, INPUT_PULLUP);
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(15);
  xpinval = joystickHome(JOYSTICK_XPIN);
  ypinval = joystickHome(JOYSTICK_YPIN);
  resetGameVariables();
  generateSnake();
  generateApple();
  drawBoard();
}

void loop() {
  unsigned long currentMillis = millis();
  checkJoystickButton();
  int Direction = SnakeDirection();  
  if (currentMillis - previousMillis >= gameRate) {
  detectAppleEaten();
  moveSnake(Direction);
  drawBoard(); 
  previousMillis = currentMillis;
  lastDirection = Direction;
  }
}

void generateSnake() {
  for (int i = 0; i < snakeLength; i++) {
    snake[i] = snakeNewPosition + i;
  }
}

void drawBoard() {
  matrix.setPixelColor(apple, RED);
  for (int i = 0; i < snakeLength; i++) {
    matrix.setPixelColor(snake[i], GREEN);
  }
  matrix.show();
}

int generateApple() {
  apple = random(0, 256);
  for (int i = 0; i < snakeLength; i++) {
    if (snake[i] == apple) {
      generateApple();
    }
  }
  return apple;
}

void resetGameVariables() {
  for (int i = 0; i < 256; i++) {
    snake[i] = 0;
  }
  snakeLength = 2;
  gameRate = 600;
  generateApple();
  snakeDirection = 1;
  snakeNewPosition = 135;
}

void detectAppleEaten() {
  if (snake[0] == apple) {
    snakeLength += 1;
    generateApple();
    gameRate = .95 * gameRate;
  }
}

void gameOver() {
  for (int i = 0; i < snakeLength; i++) {
    matrix.setPixelColor(snake[i], RED);
  }
  Serial.println("game over");
  matrix.show();
  delay(1000);
  matrix.clear();
  setup();
}

