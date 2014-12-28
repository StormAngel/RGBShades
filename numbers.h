

// numbers are represented as unsigned 16bit numbers:
// 3x5 dots bitwise encoded by columns, then rows.
// i.E. the number "1" is 
// 0 1 0     1    2    4
// 1 1 0     8   16   32
// 0 1 0    64  128  256
// 0 1 0   512 1024 2048
// 0 1 0  4096 819216384
// this results in the uint16_t number "

                            //     0     1      2      3      4      5      6     7      8      9
const uint16_t ArrNumbers[] = {11114 ,9370, 29994, 14819, 18861, 14543, 10958, 9511, 10922, 14762};

void drawNumber(uint8_t _pos, uint8_t _num, CRGB _color, uint8_t _blend=0) {
  if(_pos >= kMatrixWidth) return;  // start position outside visible area, do nothing
  if(_num < 10) {
    uint16_t theNum = ArrNumbers[_num];
    for(uint8_t i=0;i<16;i++) {
      if((1<<i)&theNum) {
        if(_blend) {
          leds[XY(_pos+(i%3),i/3)] += _color;
        } else {
          leds[XY(_pos+(i%3),i/3)] = _color;
        }
      }
    }
  }
}
