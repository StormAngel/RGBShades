//   Graphical effects to run on the RGB Shades LED array
//   Each function should have the following components:
//    * Must be declared void with no parameters or will break function pointer array
//    * Check effectInit, if false then init any required settings and set effectInit true
//    * Set effectDelay (the time in milliseconds until the next run of this effect)
//    * All animation should be controlled with counters and effectDelay, no delay() or loops
//    * Pixel data should be written using leds[XY(x,y)] to map coordinates to the RGB Shades layout


// Triple Sine Waves
uint8_t sineOffset = 0; // counter for current position of sine waves
void threeSine() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 20;
  }

  // Draw one frame of the animation into the LED array
  for (uint8_t x = 0; x < kMatrixWidth; x++) {
    for (uint8_t y = 0; y < kMatrixHeight; y++) {

      // Calculate "sine" waves with varying periods
      // sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
      uint8_t sinDistanceR = qmul8(abs(y*(255/kMatrixHeight) - sin8(sineOffset*9+x*16)),2);
      uint8_t sinDistanceG = qmul8(abs(y*(255/kMatrixHeight) - sin8(sineOffset*10+x*16)),2);
      uint8_t sinDistanceB = qmul8(abs(y*(255/kMatrixHeight) - sin8(sineOffset*11+x*16)),2);

      leds[XY(x,y)] = CRGB(255-sinDistanceR, 255-sinDistanceG, 255-sinDistanceB); 
    }
  }
  
  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle

}


// RGB Plasma
uint8_t offset  = 0; // counter for radial color wave motion
uint16_t plasVector = 0; // counter for orbiting plasma center
void plasma() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 10;
  }

  // Calculate current center of plasma pattern (can be offscreen)
  uint8_t xOffset = cos8(plasVector/256);
  uint8_t yOffset = sin8(plasVector/256);

  // Draw one frame of the animation into the LED array
  for (uint8_t x = 0; x < kMatrixWidth; x++) {
    for (uint8_t y = 0; y < kMatrixHeight; y++) {
      uint8_t color = sin8(sqrt(sq(((float)x-7.5)*10+xOffset-127)+sq(((float)y-2)*10+yOffset-127))+offset);
      leds[XY(x,y)] = CHSV(color, 255, 255);
    }    
  }

  offset++; // wraps at 255 for sin8
  plasVector += 16; // using an int for slower orbit (wraps at 65536)

}


// Scanning pattern left/right, uses global hue cycle
uint8_t riderPos = 0;
void rider() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 5;
    riderPos = 0;
  }

  // Draw one frame of the animation into the LED array
  for (uint8_t x = 0; x < kMatrixWidth; x++) {
    uint16_t brightness = abs(x*(256/kMatrixWidth) - triwave8(riderPos)*2 + 127)*3;
    if (brightness > 255) brightness = 255;
    brightness = 255 - brightness;
    CRGB riderColor = CHSV(cycleHue, 255, brightness);
    for (uint8_t y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = riderColor;
    }
  }

  riderPos++; // byte wraps to 0 at 255, triwave8 is also 0-255 periodic

}


// Shimmering noise, uses global hue cycle
void glitter() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 15;
  }

  // Draw one frame of the animation into the LED array
  for (uint8_t x = 0; x < kMatrixWidth; x++) {
    for (uint8_t y = 0; y <kMatrixHeight; y++) {
      leds[XY(x,y)] = CHSV(cycleHue,255,random8(5)*63);
    }
  }
}


// Fills saturated colors into the array from alternating directions
uint8_t currentColor = 0;
uint8_t currentRow = 0;
uint8_t currentDirection = 0;
void colorFill() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 45;
    currentColor = 0;
    currentRow = 0;
    currentDirection = 0;
    currentPalette = RainbowColors_p;
  }

  // test a bitmask to fill up or down when currentDirection is 0 or 2 (0b00 or 0b10)
  if (!(currentDirection & 1)) {
    effectDelay = 45; // slower since vertical has fewer pixels
    for (uint8_t x = 0; x < kMatrixWidth; x++) {
      uint8_t y = currentRow;
      if (currentDirection == 2) y = kMatrixHeight - 1 - currentRow;
      leds[XY(x,y)] = currentPalette[currentColor];
    }
  } 

  // test a bitmask to fill left or right when currentDirection is 1 or 3 (0b01 or 0b11)
  if (currentDirection & 1) {
    effectDelay = 20; // faster since horizontal has more pixels
    for (uint8_t y = 0; y < kMatrixHeight; y++) {
      uint8_t x = currentRow;
      if (currentDirection == 3) x = kMatrixWidth - 1 - currentRow;
      leds[XY(x,y)] = currentPalette[currentColor];
    }
  }

  currentRow++;
  
  // detect when a fill is complete, change color and direction
  if ((!(currentDirection & 1) && currentRow >= kMatrixHeight) || ((currentDirection & 1) && currentRow >= kMatrixWidth)) {
    currentRow = 0;
    currentColor += random8(3,6);
    if (currentColor > 15) currentColor -= 16;
    currentDirection++;
    if (currentDirection > 3) currentDirection = 0;
    effectDelay = 300; // wait a little bit longer after completing a fill
  }


}


// Emulate 3D anaglyph glasses
void threeDee() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 50;
  }
  
   for (uint8_t x = 0; x < kMatrixWidth; x++) {
     for (uint8_t y = 0; y < kMatrixHeight; y++) {
       if (x < 7) {
         leds[XY(x,y)] = CRGB::Blue;
       } else if (x > 8) {
         leds[XY(x,y)] = CRGB::Red;
       } else {
         leds[XY(x,y)] = CRGB::Black;
       }
     }
   }
   
   leds[XY(6,0)] = CRGB::Black;
   leds[XY(9,0)] = CRGB::Black;

}


// Random pixels scroll sideways, uses current hue
#define rainDir 0
void sideRain() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 30;
  }
  
  scrollArray(rainDir);
  uint8_t randPixel = random8(kMatrixHeight);
  for (uint8_t y = 0; y < kMatrixHeight; y++) {
    leds[XY((kMatrixWidth-1) * rainDir,y)] = CRGB::Black;
  }
  leds[XY((kMatrixWidth-1)*rainDir, randPixel)] = CHSV(cycleHue, 255, 255); 

}


// Pixels with random locations and random colors selected from a palette
// Use with the fadeAll function to allow old pixels to decay
void confetti() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 10;
    selectRandomPalette();
  }

  // scatter random colored pixels at several random coordinates
  for (uint8_t i = 0; i < 4; i++) {
    leds[XY(random8(kMatrixWidth),random8(kMatrixHeight))] = ColorFromPalette(currentPalette, random8(), 255);//CHSV(random16(255), 255, 255);
  }
  random16_add_entropy(analogRead(A3));  // unconnected analog port
}


// Draw slanting bars scrolling across the array, uses current hue
uint8_t slantPos = 0;
void slantBars() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 5;
  }

  for (uint8_t x = 0; x < kMatrixWidth; x++) {
    for (uint8_t y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CHSV(cycleHue, 255, quadwave8(x*32+y*32+slantPos));
    }
  }

  slantPos-=4;

}


// sine ripple effect
uint8_t  sineRipple_dist[NUM_LEDS];
void sineRipple() {  // startup tasks
  
  if(effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 12;
    
    currentPalette = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Black);
    uint8_t mlt256 = 256 / (kMatrixHeight + kMatrixWidth); // maximum possible distance to origin point
    for(uint8_t y=0; y<kMatrixHeight; y++) {
      for(uint8_t x=0; x<kMatrixWidth; x++) {
        // calculate distance to origin point sqrt((x+_xoffset)*(x+_xoffset) + (y+_yoffset)*(y+_yoffset)) * mlt256
        sineRipple_dist[XY(x,y)] = sqrt((x-7)*(x-7) + (y+1)*(y+1)) * mlt256;
      }
    }
  }
  
  for(uint8_t x=0;x<kMatrixWidth;x++) {
    for(uint8_t y=0;y<kMatrixHeight;y++) {
      leds[XY(x,y)] = ColorFromPalette(currentPalette, quadwave8((((sineRipple_dist[XY(x,y)] * 3) - offset)%256)));
    }
  }
  offset++;
}


// Draws parallel lines from alternating directions
uint8_t lineDir;
uint8_t linePos;
void gridLines() {
  if (effectInit == false) {  //startup tasks
    effectInit = true;
    scrollEffect = false;
    currentColor = 0;
    lineDir = 0;
    linePos = 0;
    FastLED.clear();
  }

  if (lineDir == 0) {
    for (uint8_t y=0;y<5;y+=2) {
      leds[XY(linePos,y)] = CRGB::Green;
    }
    effectDelay = 60;
    if (linePos >= 15) {
      linePos = 0;
      lineDir++;
      effectDelay = 300;
    }
    else linePos++;
  }
  
  else if (lineDir == 1) {
    for (uint8_t x=0;x<16;x+=2) {
      leds[XY(x,linePos)] = CRGB::Red;
    }
    effectDelay = 120;
    if (linePos >= 4) {
      linePos = 0;
      lineDir++;
      effectDelay = 300;
    }
    else linePos++;
  }
  
  else if (lineDir == 2) {
    for (uint8_t y=1;y<5;y+=2) {
      leds[XY((15-linePos),y)] = CRGB::Yellow;
    }
    effectDelay = 60;
    if (linePos >= 15) {
      linePos = 0;
      lineDir++;
      effectDelay = 300;
    }
    else linePos++;
  }
  
  else if (lineDir == 3) {
    for (uint8_t x=1;x<16;x+=2) {
      leds[XY(x,(4-linePos))] = CRGB::Blue;
    }
    effectDelay = 120;
    if (linePos >= 4) {
      linePos = 0;
      lineDir = 0;
      effectDelay = 300;
    }
    else linePos++;
  }
}


// StormAngel's text scroller effect
uint8_t hPos;
uint8_t vPos;
boolean vScroll;
void stormScroll() {  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    scrollEffect = true;
    effectDelay = 160;
    scrollDir = 0;
    horiPos = 7;
    vertiPos = 5;
    hPos = 35;
    vPos = 5;
    sWidth = 60;
    vScroll = false;
  }
  
  FastLED.clear();
  drawChar(28,CHSV(cycleHue, 255, 255), (horiPos)%sWidth, vertiPos);
   // S
  drawChar(29,CHSV(cycleHue, 255, 255),(horiPos+5)%sWidth,vertiPos);
   // T
  drawChar(24,CHSV(cycleHue, 255, 255),(horiPos+11)%sWidth,vertiPos);
   // O
  drawChar(27,CHSV(cycleHue, 255, 255),(horiPos+16)%sWidth,vertiPos);
   // R
  drawChar(22,CHSV(cycleHue, 255, 255),(horiPos+21)%sWidth,vertiPos);
   // M
  
  if (horiPos == 0) {
    horiPos = sWidth-1;
  }
  if (hPos == 0) {
    hPos = sWidth-1;
  }
  
  drawChar(10,CHSV(cycleHue+80, 255, 255),(hPos)%sWidth,vPos);
   // A
  drawChar(23,CHSV(cycleHue+80, 255, 255),(hPos+5)%sWidth,vPos);
   // N
  drawChar(16,CHSV(cycleHue+80, 255, 255),(hPos+11)%sWidth,vPos);
   // G
  drawChar(14,CHSV(cycleHue+80, 255, 255),(hPos+16)%sWidth,vPos);
   // E
  drawChar(21,CHSV(cycleHue+80, 255, 255),(hPos+20)%sWidth,vPos);
   // L
  
  if (vScroll == true) {
    vPos++;
    if (++vertiPos == 5) {
      vScroll = false;
      hPos = 36;
      vPos = 5;
     }
  }
  else if (hPos == 57) {
      vScroll = true;
      horiPos = 14;
      vertiPos = 255;
  }
  horiPos--;
  hPos--;
}


//// Molpy Grapevine Scroller ////
// As seen at the February 2015 "XKCD OTTer Meetup" in Bochum, Germany
// Inspired by the Owen Evans song "Molpies!"
// www.youtube.com/watch?v=GU9RL6D23jk
// For context, read the discussion thread here:
// http://forums.xkcd.com/viewtopic.php?f=7&t=101043&p=3371229#p3371215

#define Enough_Molpies 12
#define Extra_Molpies 88  // Redundancy is Molpish!

// Make new wineskins
uint8_t molpCount, vineCount, molPos, graPos, vWidth;

void molpyVine() {
  if(effectInit == false) {
    effectInit = true;
    scrollEffect = true;
    effectDelay = 60;
    scrollDir = 0;
    molPos = 5;  // Where the molpies are
    sWidth = 28;  // Where the molpies can be
    graPos = 27;  // Where the grapevines are
    vWidth = 67;  // Where the vines can be
    molpCount = 0;  // How many molpies have gone "bye"
    vineCount = 0;  // How many vines have passed
  }

// Molpy Looper
  if (molPos == 0) {  // Here comes a molpy
    sWidth = 120;  // Give the molpy space to move
    molpCount++;  // Count the molpies!
    if (scrollDir == 1) { // Press button, recieve grapevines
        scrollDir = 0;
        molpCount = Enough_Molpies + Extra_Molpies;  // ALL THE MOLPIES!!
      }
    if (scrollDir == 2) { // Press button twice to reset molpy counter
        scrollDir = 0;
        molpCount = 0;  // Nothing to see here
      }
    if (molpCount < Enough_Molpies) {  // Not enough molpies?
      sWidth = 28;  // Send another molpy immediately
    }
    molPos = sWidth;  // Loop the molpies
  }

//Grapevine Looper
  if (graPos == 0) {  // A shrubbery!
    if (vineCount == 0) { // If this is the first vine,
      vWidth = 46;  // bring another immediately
    }
    else {
      vWidth = 67; // Put that thing back where it came from...
    }
    vineCount++;  // Count the vines
    graPos = vWidth;  // Make a loop of grapes
  }
      
  FastLED.clear();
// Molpy Molpy...
  drawChar(22,CHSV(cycleHue+00, 255, 255),(molPos)%sWidth,5);  // M
  drawChar(24,CHSV(cycleHue+20, 255, 255),(molPos+6)%sWidth,5);  // O
  drawChar(21,CHSV(cycleHue+40, 255, 255),(molPos+11)%sWidth,5);  // L
  drawChar(25,CHSV(cycleHue+60, 255, 255),(molPos+15)%sWidth,5);  // P
  drawChar(34,CHSV(cycleHue+80, 255, 255),(molPos+20)%sWidth,5);  // Y
  molPos--;  // I like to move it
  
// Grapevine Grapevine!
  if (molpCount >= Enough_Molpies) {  // Send in the vines
    drawChar(16,CHSV(cycleHue+00, 255, 255),(graPos)%vWidth,5);  // G
    drawChar(27,CHSV(cycleHue+20, 255, 255),(graPos+5)%vWidth,5);  // R
    drawChar(10,CHSV(cycleHue+40, 255, 255),(graPos+10)%vWidth,5);  // A
    drawChar(25,CHSV(cycleHue+60, 255, 255),(graPos+15)%vWidth,5);  // P
    drawChar(14,CHSV(cycleHue+80, 255, 255),(graPos+20)%vWidth,5);  // E
    drawChar(31,CHSV(cycleHue+120, 255, 255),(graPos+24)%vWidth,5);  // V
    drawChar(18,CHSV(cycleHue+140, 255, 255),(graPos+28)%vWidth,5);  // I
    drawChar(23,CHSV(cycleHue+160, 255, 255),(graPos+32)%vWidth,5);  // N
    drawChar(14,CHSV(cycleHue+180, 255, 255),(graPos+38)%vWidth,5);  // E
    graPos--;  // Move it!

    if (vineCount == 2 && graPos == 27) {  // ...or so help me!
      vineCount = 0;
      molpCount = 0;  // Bring more molpies
    }
  }
}


// 1337 Scroller
void eliteScrolls() {
  if(effectInit == false) {  //startup tasks
    effectInit = true;
    scrollEffect = true;
    effectDelay = 80;
    horiPos = 6;
    sWidth = 20;
  }
  
  FastLED.clear();
  drawChar(1,CHSV(cycleHue+00, 255, 255),(horiPos)%sWidth,5);
  drawChar(3,CHSV(cycleHue+20, 255, 255),(horiPos+3)%sWidth,5);
  drawChar(3,CHSV(cycleHue+40, 255, 255),(horiPos+7)%sWidth,5);
  drawChar(7,CHSV(cycleHue+60, 255, 255),(horiPos+11)%sWidth,5);

  if (scrollDir == 1) {  // Scroll left
    horiPos--;
  }
  else if(scrollDir == 2) {  // Scroll right
    horiPos++;
  }  
  else if (horiPos != 6) {  // Scroll to start position
    horiPos++;
  }
  
  if(horiPos >= (sWidth+1)) {
    horiPos = 1;
  }
  if (horiPos == 0) {
    horiPos = sWidth;
  }
}


//Run Conway's Game of Life on the glasses
//based on code by kredal
//github.com/kredal/RGBShades/commit/2cf723b85363d0232035f220d2119ad5847e0e6c
void gameoflife() {
  if (effectInit == false) {  // startup tasks
    effectInit = true;
    scrollEffect = true;
    effectDelay = 150;
    Clear();
    Glider(0,0);
    Glider(8,1);
  }
  if (scrollDir != 0) {  // Press button to recieve life
    scrollDir = 0;
    Randomize(8);  // A higher number means lower chance of cells added
//  Expand = true;
  }
  
  //draw live cells and count them, determine neighbors and store live cells for next frame.
  NewCount = 0;
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 5; y++) {
      byte Cell = y*16+x;  // This is that Cell
      if (OldCells[Cell] == 0) {  // Dead cells
        leds[XY(x,y)] = CRGB::Black;  // Don't draw the cell if its dead
        if (Neighbors(x,y) == 3) {
          NewCells[Cell] = 1;  // Its alive!
          NewCount++;
        }
        if (Expand == true) {  // If the pattern is stable, increase cell growth
          if (Neighbors(x,y) > 1) NewCells[Cell] = 1;
        }
      }
      else {  // Live cells
        leds[XY(x,y)] = CHSV((CellAge[Cell])*20+28, 255, 255);  // Draw live cells, color dependant on age
        if (Neighbors(x,y) != 2 && Neighbors(x,y) != 3) {
          NewCells[Cell] = 0;  // Kill the cell
          CellAge[Cell] = 0;
        }
        else {
          NewCells[Cell] = 1;  // Stayin' alive
          if (CellAge[Cell] < 16) CellAge[Cell]++;  // Increment the cell's age until its old
        }
      }
    }
  }
  Expand = false;
  for (byte Cell = 0; Cell < 80; Cell++) {
    OldCells[Cell] = NewCells[Cell];  // Update the old cells
    NewCount += OldCells[Cell];  // Count the live cells
  }
  if (NewCount == 0) {  // There are no live cells
    if (++Empty > 7) {  // This number determines the delay before randomizing
      Empty = 0;
      Randomize(5);  // 0-9 (A higher value means less cells added)
    }
  }
  else if (NewCount == OldCount) {  // The live cell count hasn't changed
    if (Stable++ > 42) {  // Delay to confirm stable pattern
      Stable = 0;
      Expand = true;
    }
  }
  else Stable = 0;
  OldCount = NewCount;
}



