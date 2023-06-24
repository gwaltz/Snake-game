#include <Adafruit_NeoPixel.h>


#define MATRIX_PIN 12

//define analog values from joystick as integers
const int JOYSTICK_XPIN = A3;
const int JOYSTICK_YPIN = A4;

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(256, MATRIX_PIN, NEO_GRB + NEO_KHZ800);
uint32_t RED = matrix.Color(255, 0, 0);
uint32_t GREEN = matrix.Color(0, 255, 0);
uint32_t BLUE = matrix.Color(0, 0, 255);

int snake[256];

int snakeNewPosition = 135, snakeLength, apple, col, row, snakeDirection;
int NewPosition, xpinval, ypinval;

unsigned long lastClockTick;
int gameRate;
int numApplesEaten = 0;



struct Coordinate {
  int row;
  int col;
};

void setup() {
  pinMode(JOYSTICK_XPIN, INPUT);  //initializes pinMode of Joystick X direction
  pinMode(JOYSTICK_YPIN, INPUT);
  Serial.begin(9600);
  matrix.begin();
  matrix.clear();
  matrix.setBrightness(15);
  xpinval = joystickHome(JOYSTICK_XPIN);
  ypinval = joystickHome(JOYSTICK_YPIN);
  resetGameVariables();
  generateSnake();
  generateApple();
  drawBoard();
  Serial.println(xpinval);
  Serial.println(ypinval);
}

void loop() {
  detectAppleEaten();
  moveSnake();
  drawBoard();
  delay(gameRate);
}

void generateSnake() {
  for (int i = 0; i < snakeLength; i++) {
    snake[i] = snakeNewPosition + i;
  }
}

void drawBoard() {
  for (int i = 0; i < snakeLength; i++) {
    matrix.setPixelColor(snake[i], GREEN);
  }
  matrix.setPixelColor(apple, RED);
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

void moveSnake() {
  snakeNewPosition = calculateNewPosition();
  for (int i = 0; i < snakeLength; i++) {
    if (snake[i] == snakeNewPosition) {
      Serial.println(snake[i]);
      Serial.println(snakeNewPosition);
      gameOver();
    }
  }

  matrix.clear();

  for (int i = snakeLength; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  snake[0] = snakeNewPosition;
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
  delay(3000);
  matrix.clear();
  setup();
}

int SnakeDirection() {
  int x = analogRead(JOYSTICK_XPIN);
  int y = analogRead(JOYSTICK_YPIN);

  if (x < (xpinval - 80) && snakeDirection != 1) {
    snakeDirection = 2;
  } else if (x > (xpinval + 80) && snakeDirection != 2) {
    snakeDirection = 1;
  } else if (y < (ypinval - 80) && snakeDirection != 4) {
    snakeDirection = 4;
  } else if (y > (ypinval + 80) && snakeDirection != 3) {
    snakeDirection = 3;
  }
  return snakeDirection;
}

Coordinate findRowCol() {
  Coordinate coord;
  coord.row = snakeNewPosition / 16;
  if (coord.row % 2 == 0) {
    coord.col = snakeNewPosition % 16;
  } else {
    coord.col = 15 - (snakeNewPosition % 16);
  }
  return coord;
}

int calculateNewPosition() {

  Coordinate coords = findRowCol();
  int row = coords.row;
  int col = coords.col;

  int Direction = SnakeDirection();

  if (Direction == 1 && row == 0) {  //top edge
    NewPosition = 255 - col;
  } else if (Direction == 2 && row == 15) {  //bottom edge
    NewPosition = 255 - snakeNewPosition;
  } else if (Direction == 3 && col == 0) {  //left edge
    if (row % 2 == 0) {
      NewPosition = snakeNewPosition + 15;
    } else {
      NewPosition = snakeNewPosition - 15;
    }
  } else if (Direction == 4 && col == 15) {  //right edge
    if (row % 2 == 0) {
      NewPosition = snakeNewPosition - 15;
    } else {
      NewPosition = snakeNewPosition + 15;
    }
    Serial.println(col);
  } else if (Direction == 1) {  //up
    if (row % 2 == 0) {
      NewPosition = snakeNewPosition - (col * 2) - 1;
    } else {
      NewPosition = snakeNewPosition - (snakeNewPosition - 16 * row) - (16 - col);
    }
  } else if (Direction == 2) {  //down
    if (row % 2 == 0) {
      NewPosition = snakeNewPosition + (16 - col - 1) + (16 - col);
    } else {
      NewPosition = snakeNewPosition + (col * 2) + 1;
    }
  } else if (Direction == 3) {  //left
    if (row % 2 == 0) {
      NewPosition = snakeNewPosition - 1;
    } else {
      NewPosition = snakeNewPosition + 1;
    }
  } else if (Direction == 4) {  //right
    if (row % 2 == 0) {
      NewPosition = snakeNewPosition + 1;
    } else {
      NewPosition = snakeNewPosition - 1;
    }
  }
  Serial.println(col);
  return NewPosition;
}

int joystickHome(int h) {
  int tot = 0;

  for (int i = 0; i < 6; i++) {
    tot += analogRead(h);
  }

  int pinval = tot / 6;

  return pinval;
}
