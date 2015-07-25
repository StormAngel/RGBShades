//Functions and variables for "Conway's Game of Life" effect

uint8_t OldCells[80];  // Cells currently being drawn; updated from NewCells at the end of each effect loop
uint8_t NewCells[80];  // Cells for the next frame; based on neighbors of OldCells
uint8_t CellAge[80];  // How long since the cells were last switched on;  changes the cell color
byte NewCount, OldCount = 0, Empty = 0, Stable = 0;
boolean Expand = false;

void Clear() {
  for (byte cell=0;cell<80;cell++) {
    OldCells[cell] = 0;
    NewCells[cell] = 0;
  }
  FastLED.clear();
}

void Randomize(byte Prob) {  // A higher value means lower probability of cells added
  for (byte cell=0;cell<80;cell++) {
    if (random(0, 10) > Prob) OldCells[cell] = 1;
  }
}

void Glider(byte Xoff, byte Yoff) {  // Puts a glider somewhere on the matrix
  OldCells[2+Xoff+Yoff*16] = 1;
  OldCells[16+Xoff+Yoff*16] = 1;
  OldCells[18+Xoff+Yoff*16] = 1;
  OldCells[33+Xoff+Yoff*16] = 1;
  OldCells[34+Xoff+Yoff*16] = 1;
}
    
uint8_t Neighbors(byte X, byte Y) {
  if (X == 0) {
    if (Y == 0) {  // Top-Left Corner
      return OldCells[1] + OldCells[15] + OldCells[16] + OldCells[17] + OldCells[31] + OldCells[64] + OldCells[65] + OldCells[79];
    }
    else {  // Left Column
      return OldCells[(Y-1)*16+15] + OldCells[(Y-1)*16] + OldCells[(Y-1)*16+1] + OldCells[Y*16+15] + OldCells[Y*16+1] + OldCells[((Y+1)%5)*16+15] + OldCells[((Y+1)%5)*16] + OldCells[((Y+1)%5)*16+1];
    }
  }
  if (Y == 0) {  // Top Row
    return OldCells[63+X] + OldCells[64+X] + OldCells[64+((X+1)%16)] + OldCells[X-1] + OldCells[(X+1)%16] + OldCells[15+X] + OldCells[16+X] + OldCells[16+((X+1)%16)];
  }
  else {  // Other Cells
    return OldCells[(Y-1)*16+X-1] + OldCells[(Y-1)*16+X] + OldCells[(Y-1)*16+((X+1)%16)] + OldCells[Y*16+X-1] + OldCells[Y*16+((X+1)%16)] + OldCells[((Y+1)%5)*16+X-1] + OldCells[((Y+1)%5)*16+X] + OldCells[((Y+1)%5)*16+((X+1)%16)];
  }
}
