
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

void moveSnake(int direction) {
  snakeNewPosition = calculateNewPosition(direction);
  for (int i = 0; i < snakeLength; i++) {
    if (snake[i] == snakeNewPosition) {
      gameOver();
    }
  }
  matrix.clear();
  for (int i = snakeLength; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
  snake[0] = snakeNewPosition;
}