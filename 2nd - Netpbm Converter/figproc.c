#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// FOR BONUS IMPLEMENTATION INSTEAD OF REDIRECTING WITH << ./figproc < "INPUT IMAGE" > "OUTPUT IMAGE" >> USE << ./figproc a < "INPUT IMAGE" > "OUTPUT IMAGE" >>

int maxVal = 0;

void inputError() {
  // Display error message and exit
  fprintf(stderr, "Input Error!\n");
  exit(EXIT_FAILURE);
}

void skipWhitespace(int passthrough) {
  while (1) {
    char ch = getchar();
    if (!isspace(ch)) {
      ungetc(ch, stdin);
      break;
    }
    if (passthrough) putchar(ch);
  }
}

int handleComment(int passthrough) {
  // Read comments and write them if passthrough==1
  skipWhitespace(passthrough);

  char ch = getchar();
  ungetc(ch, stdin);

  if (ch != '#') return 0;

  while (ch = getchar(), !iscntrl(ch)) {
    if (passthrough) putchar(ch);
  }
  if (passthrough) putchar(ch);

  return 1;
}

int getMagicNumber() {
  // Read magic number
  getchar();

  char ch = getchar();
  if (!isdigit(ch)) inputError();
  int magicNumber = ch - '0';

  getchar();

  return magicNumber;
}

int getNumber(int passthrough) {
  // Read ascii number and if passthrough==1 write it
  char ch;
  int number = 0;

  // Skip the whitespace before ascii characters
  skipWhitespace(passthrough);

  // Read all ascii characters until whitespace
  while (ch = getchar(), !isspace(ch)) {
    if (passthrough) putchar(ch);
    number *= 10;
    if (!isdigit(ch)) inputError();
    number += ch - '0';
  }
  if (passthrough) putchar(ch);

  return number;
}

void printNumber(int num) {
  // Write ascii number starting from most significant digit
  int digits = 5;
  if (num < 10000) digits = 4;
  if (num < 1000) digits = 3;
  if (num < 100) digits = 2;
  if (num < 10) digits = 1;

  for (int i=digits-1; i>=0; i--) {
    int div = 1;
    for (int j=0; j<i; j++) {
      div *= 10;
    }

    int digit = num / div;
    num %= div;

    putchar(digit + '0');
  }
}

int convertColor(int r, int b, int g) {
  // Convert color pixel to grey pixel
  return (299*r+587*g+114*b)/1000;
}

int convertGrey(int val) {
  // Convert grey pixel to black & white pixel
  return val < ((maxVal+1)/2);
}

void readBWAscii(int* val) {
  // Read black & white ascii pixel
  *val = getNumber(0);

  if (*val > 1 || *val < 0) inputError();
}

void readGreyAscii(int* val) {
  // Read grey ascii pixel
  *val = getNumber(0);

  if (*val > maxVal) inputError();
}

void readRGBAscii(int* r, int* g, int* b) {
  // Read color ascii pixel
  *r = getNumber(0);
  *g = getNumber(0);
  *b = getNumber(0);

  if (*r > maxVal || *g > maxVal || *b > maxVal) inputError();
}

void readBWBin(int* val, int last) {
  // Read black & white binary pixel
  static int count = 0;
  static char ch = '\0';

  if (count == 0) ch = getchar();

  *val = ch & (1<<7) ? 1 : 0;
  ch <<= 1;
  if (++count > 7 || last) count = 0;
}

void readGreyBin(int* val) {
  // Read grey binary pixel
  *val = getchar();

  if (maxVal >= 256) {
    *val <<= 8;
    *val += getchar();
  }

  if (*val > maxVal) inputError();
}

void readRGBBin(int* r, int* g, int* b) {
  // Read color binary pixel
  if (maxVal >= 256) {
    *r = getchar();
    *r <<= 8;
    *r += getchar();

    *g = getchar();
    *g <<= 8;
    *g += getchar();

    *b = getchar();
    *b <<= 8;
    *b += getchar();

  } else {
    *r = getchar();
    *g = getchar();
    *b = getchar();
  }

  if (*r > maxVal || *g > maxVal || *b > maxVal) inputError();
}

void writeBytes(int bytes) {
  // Write color & grey binary data
  int topByte = bytes >> 8;
  int bottomByte = bytes & ((1<<8) - 1);

  if (maxVal >= 256) {
    putchar(topByte);
  }

  putchar(bottomByte);
}

void writeBits(int bit, int last) {
  // Write black & white binary data
  static char byte = 0;
  static int count = 0;

  byte <<= 1;
  byte += bit & 1;

  count++;
  if (last || count > 7) {
    putchar(byte);
    count = 0;
  }
}

void p2ToP1(int eol) {
  // Convert P2 to P1
  int grey;
  readGreyAscii(&grey);

  int val = convertGrey(grey);

  printNumber(val);
  if (!eol) putchar(' ');
}

void p3ToP2(int eol) {
  // Convert P3 to P2
  int r,g,b;
  readRGBAscii(&r, &g, &b);

  int val = convertColor(r, g, b);

  printNumber(val);
  if (!eol) putchar(' ');
}

void p5ToP4(int eol) {
  // Convert P5 to P4
  int grey;
  readGreyBin(&grey);

  int val = convertGrey(grey);

  writeBits(val, eol);
}

void p6ToP5() {
  // Convert P6 to P5
  int r,g,b;
  readRGBBin(&r, &g, &b);

  int val = convertColor(r, g, b);

  writeBytes(val);
}

void p6ToP3(int eol) {
  // Convert P6 to P3
  int r,g,b;
  readRGBBin(&r, &g, &b);

  printNumber(r);
  putchar(' ');
  printNumber(g);
  putchar(' ');
  printNumber(b);
  if (!eol) putchar(' ');
}

void p3ToP6() {
  // Convert P3 to P6
  int r,g,b;
  readRGBAscii(&r, &g, &b);

  writeBytes(r);
  writeBytes(g);
  writeBytes(b);
}

void p5ToP2(int eol) {
  // Convert P5 to P2
  int grey;
  readGreyBin(&grey);

  printNumber(grey);
  if (!eol) putchar(' ');
}

void p2ToP5() {
  // Convert P2 to P5
  int grey;
  readGreyAscii(&grey);

  writeBytes(grey);
}

void p4ToP1(int eol) {
  // Convert P4 to P1
  int value;
  readBWBin(&value, eol);

  printNumber(value);
  if (!eol) putchar(' ');
}

void p1ToP4(int eol) {
  // Convert P1 to P4
  int value;
  readBWAscii(&value);

  writeBits(value, eol);
}

int main(int argc, char* argv[]) {
  // Parse commandline options
  int convertEncoding = 0;
  if (argc > 1) {
    convertEncoding = 1;
  }

  // Get magic number
  int magicNumber = getMagicNumber();

  // Calculate new magic number
  int newMagicNumber;
  if (convertEncoding) {
    newMagicNumber = magicNumber > 3 ? magicNumber - 3 : magicNumber + 3;
  } else {
    newMagicNumber = magicNumber - 1;
  }

  // Write new magic number
  putchar('P');
  putchar(newMagicNumber+'0');
  putchar('\n');

  int bitmapTarget = (newMagicNumber == 1 || newMagicNumber == 4);

  // Read and write image width
  while(handleComment(1));
  int width = getNumber(1);

  // Read and write image height
  while(handleComment(1));
  int height = getNumber(1);

  while(handleComment(!bitmapTarget));

  // Read maximum pixel value
  if (!(magicNumber == 1 || magicNumber == 4)) maxVal = getNumber(0);

  // Write maximum pixel value
  if (!bitmapTarget) {
    printNumber(maxVal);
    putchar('\n');
  }

  // Convert each pixel in the image
  for (int y=0; y < height; y++) {
    for (int x=0; x < width; x++) {
      // check if pixel is at end of line
      int eol = (x == width-1);
      // if convertEncoding == 0 cases are in range 2-8
      // else cases are in range 16-48
      switch (magicNumber << convertEncoding*3) {
        case 2:
          p2ToP1(eol);
          break;
        case 3:
          p3ToP2(eol);
          break;
        case 5:
          p5ToP4(eol);
          break;
        case 6:
          p6ToP5();
          break;
        case 8:
          p1ToP4(eol);
          break;
        case 16:
          p2ToP5();
          break;
        case 24:
          p3ToP6();
          break;
        case 32:
          p4ToP1(eol);
          break;
        case 40:
          p5ToP2(eol);
          break;
        case 48:
          p6ToP3(eol);
          break;
      }
    }
    // If target is ascii encoded put \n at end of each line
    if (newMagicNumber < 4) putchar('\n');
  }
}
