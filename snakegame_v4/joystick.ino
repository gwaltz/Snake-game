
int SnakeDirection() {

  int x = analogRead(JOYSTICK_XPIN);
  int y = analogRead(JOYSTICK_YPIN);
  if (x < (xpinval - 80) && lastDirection != 1) {
    snakeDirection = 2;
  } else if (x > (xpinval + 80) && lastDirection != 2) {
    snakeDirection = 1;
  } else if (y < (ypinval - 80) && lastDirection != 3) {
    snakeDirection = 4;
  } else if (y > (ypinval + 80) && lastDirection != 4) {
    snakeDirection = 3;
  }
  return snakeDirection;
}

int joystickHome(int h) {
  int tot = 0;
  for (int i = 0; i < 6; i++) {
    tot += analogRead(h);
  }
  int pinval = tot / 6;
  return pinval;
}

int checkJoystickButton() {
  if (digitalRead(JOYSTICKBUTTON) == 0) {
    int values[] = {0, 100, 150, 255};
    int R = values[random(0,3)];
    int G = values[random(0,3)];
    int B = values[random(0,3)];
    GREEN = matrix.Color(R, G, B);
    generateApple();
  }
}