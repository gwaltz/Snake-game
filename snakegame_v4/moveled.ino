
int calculateNewPosition(int snakedirection) {

  Coordinate coords = findRowCol();
  int row = coords.row;
  int col = coords.col;

  int Direction = snakedirection;

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
  return NewPosition;
}