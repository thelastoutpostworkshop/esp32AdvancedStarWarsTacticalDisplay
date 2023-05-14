//
// ESP32 Advanced Star Wars Tactical Display on a round mini Oled 
//
// Tutorial : 
//

#include <TFT_eSPI.h>
#include "Aurebesh8pt7b.h"

// Initialize the display
TFT_eSPI tft = TFT_eSPI(); 

// Colors
#define COLOR_TACTICAL_BLUE 0x004aff
#define GC9A01A_BLACK 0x0000       ///<   0,   0,   0
#define GC9A01A_NAVY 0x000F        ///<   0,   0, 123
#define GC9A01A_DARKGREEN 0x03E0   ///<   0, 125,   0
#define GC9A01A_DARKCYAN 0x03EF    ///<   0, 125, 123
#define GC9A01A_MAROON 0x7800      ///< 123,   0,   0
#define GC9A01A_PURPLE 0x780F      ///< 123,   0, 123
#define GC9A01A_OLIVE 0x7BE0       ///< 123, 125,   0
#define GC9A01A_LIGHTGREY 0xC618   ///< 198, 195, 198
#define GC9A01A_DARKGREY 0x7BEF    ///< 123, 125, 123
#define GC9A01A_BLUE 0x001F        ///<   0,   0, 255
#define GC9A01A_GREEN 0x07E0       ///<   0, 255,   0
#define GC9A01A_CYAN 0x07FF        ///<   0, 255, 255
#define GC9A01A_RED 0xF800         ///< 255,   0,   0
#define GC9A01A_MAGENTA 0xF81F     ///< 255,   0, 255
#define GC9A01A_YELLOW 0xFFE0      ///< 255, 255,   0
#define GC9A01A_WHITE 0xFFFF       ///< 255, 255, 255
#define GC9A01A_ORANGE 0xFD20      ///< 255, 165,   0
#define GC9A01A_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define GC9A01A_PINK 0xFC18        ///< 255, 130, 198
#define GC9A01A_NORON 0x13

// Display
const int CENTER = 120;
const int TEXT_LINES[4][2] = {{30, 140}, {35, 160}, {44, 180}, {70, 200}};


// Duration
typedef struct
{
  unsigned long time_start;
  unsigned long durationMs;
} Timer;

// Tactical Objects
const int arcs[5][4] = {{300, 380, 30, 0}, {300, 380, 30, 0}, {55, 240, 30, 0}, {55, 90, 45, 0}, {180, 380, 45, 0}};
const int nbrOfArcs = 5;

const int angleLines[6][4] = {
    {CENTER + 16, CENTER + 18, 67, 55},
    {CENTER + 21, CENTER + 9, 67, 15},
    {CENTER + 21, CENTER - 9, 67, 345},
    {CENTER + 12, CENTER - 18, 67, 295},
    {CENTER - 12, CENTER - 18, 67, 240},
    {CENTER - 19, CENTER - 9, 67, 200}};
const int nbrAngleLines = 6;

const int detectedObjects[24][5] = {
    {CENTER - 50, CENTER - 50, 20, 8, 55},
    {CENTER - 40, CENTER - 50, 8, 4, 55},
    {CENTER - 25, CENTER - 50, 20, 9, 60},
    {CENTER - 22, CENTER - 60, 7, 5, 60},
    {CENTER - 5, CENTER - 70, 20, 8, 90},
    {CENTER - 12, CENTER - 70, 8, 5, 90},
    {CENTER + 5, CENTER - 80, 20, 5, 90},
    {CENTER + 10, CENTER - 89, 3, 12, 90},
    {CENTER + 17, CENTER - 75, 8, 8, 105},
    {CENTER + 28, CENTER - 70, 35, 6, 112},
    {CENTER + 40, CENTER - 65, 10, 6, 112},
    {CENTER + 50, CENTER - 68, 15, 6, 117},
    {CENTER + 61, CENTER - 68, 5, 10, 117},
    {CENTER + 68, CENTER - 50, 25, 10, 140},
    {CENTER + 75, CENTER - 40, 6, 7, 150},
    {CENTER + 67, CENTER - 29, 35, 4, 163},
    {CENTER + 75, CENTER - 17, 30, 5, 166},
    {CENTER + 65, CENTER - 6, 10, 8, 180},
    {CENTER + 75, CENTER + 6, 10, 8, 180},
    {CENTER + 85, CENTER + 20, 13, 6, 196},
    {CENTER + 60, CENTER + 25, 60, 4, 196},
    {CENTER + 55, CENTER + 40, 15, 10, 215},
    {CENTER + 60, CENTER + 50, 4, 20, 215},
    {CENTER + 55, CENTER + 60, 14, 4, 238},
};
const int nbrDetectedObjects = 24;

const int undetectedObjects[2][5] = {{CENTER - 70, CENTER - 15, 25, 10, 17}, {CENTER + 25, CENTER + 65, 25, 20, 250}};
const int nbrUndetectedObjects = 2;

const int objectsColor[] = {GC9A01A_GREEN, GC9A01A_NORON, GC9A01A_RED, GC9A01A_ORANGE, GC9A01A_YELLOW};
const int objectColorCount = 5;

// Words
const char *WORDS[] = {
    "com",
    "radar",
    "track",
    "on",
};
int wordsCount = sizeof(WORDS) / sizeof(WORDS[0]);

// Geometry
const int num_degrees = 380;

struct Coordinate
{
  byte x;
  byte y;
};

void setup()
{
  Serial.begin(115200);

  tft.setFreeFont(&Aurebesh8pt7b);
  tft.setTextSize(1);
  tft.setRotation(2);
  tft.begin();
  clearScreen();
  tft.setRotation(0);

  bootTacticalDisplay();
}

void loop(void)
{
  blinkObject(random(1, 4), GC9A01A_WHITE, objectsColor[random(objectColorCount)], 5000, 700);
  blinkArc(arcs[random(nbrOfArcs)], COLOR_TACTICAL_BLUE, objectsColor[random(objectColorCount)], 5000, 700);
  blinkTacticalText("Alert!", 1, 10000, 0);
  writeRadarOn(70, 500, GC9A01A_ORANGE);
  blinkRadarOn(200, 5000);
}

// Sequences
//
void bootTacticalDisplay(void)
{
  drawCircleCentered(CENTER - 17, false, 3, COLOR_TACTICAL_BLUE);
  drawCircleCentered(15, false, 1, COLOR_TACTICAL_BLUE);
  delay(200);

  for (int i = 0; i < nbrOfArcs; i++)
  {
    drawArcWidthCoord(arcs[i], COLOR_TACTICAL_BLUE);
  }

  for (int i = 0; i < nbrAngleLines; i++)
  {
    drawLineAtAngleCoord(angleLines[i], COLOR_TACTICAL_BLUE);
  }

  tft.drawFastVLine(CENTER, TFT_WIDTH, -TFT_WIDTH, COLOR_TACTICAL_BLUE);
  tft.drawFastVLine(CENTER + 1, TFT_WIDTH, -TFT_WIDTH, COLOR_TACTICAL_BLUE);
  tft.drawFastHLine(TFT_WIDTH, CENTER, -TFT_WIDTH, COLOR_TACTICAL_BLUE);
  tft.drawFastHLine(TFT_WIDTH, CENTER + 1, -TFT_WIDTH, COLOR_TACTICAL_BLUE);

  delay(500);

  for (int i = 0; i < nbrDetectedObjects; i++)
  {
    drawRectAtAngleCoord(detectedObjects[i], GC9A01A_WHITE);
    delay(10);
  }

  for (int i = 0; i < nbrUndetectedObjects; i++)
  {
    drawRectAtAngleCoord(undetectedObjects[i], GC9A01A_NORON);
  }
  delay(500);
  writeRadarOn(70, 200, GC9A01A_ORANGE);
}

void blinkRadarOn(int speed, int durationMs)
{
  Timer time;
  time = initTimer(durationMs);
  while (timeNotExpired(time))
  {
    writeRadarOn(0, 0, GC9A01A_ORANGE);
    delay(speed);
    writeRadarOn(0, 0, GC9A01A_BLACK);
    delay(speed);
  }
  writeRadarOn(0, 0, GC9A01A_ORANGE);
}

void writeRadarOn(int letterSpeed, int wordSpeed, uint16_t color)
{
  for (int i = 0; i < wordsCount; i++)
  {
    displayAurebeshString((char *)WORDS[i], i, letterSpeed, color);
    delay(wordSpeed);
  }
}

void blinkObject(int nbr, uint16_t originalColor, uint16_t newColor, int durationMs, int speed)
{
  Timer time;
  int object[nbr];

  for (int i = 0; i < nbr; i++)
  {
    object[i] = random(nbrDetectedObjects);
  }

  time = initTimer(durationMs);
  while (timeNotExpired(time))
  {

    for (int i = 0; i < nbr; i++)
    {
      drawRectAtAngleCoord(detectedObjects[object[i]], newColor);
    }
    delay(speed);
    for (int i = 0; i < nbr; i++)
    {
      drawRectAtAngleCoord(detectedObjects[object[i]], originalColor);
    }
    delay(speed);
  }
}
void blinkArc(const int arc[], uint16_t originalColor, uint16_t newColor, int durationMs, int speed)
{
  Timer time;
  time = initTimer(durationMs);
  while (timeNotExpired(time))
  {
    drawArcWidthCoord(arc, newColor);
    delay(speed);
    drawArcWidthCoord(arc, originalColor);
    delay(speed);
  }
}

void blinkTacticalText(char *s, int line, int durationMs, int speed)
{
  Timer time;
  time = initTimer(durationMs);
  int color = 0;
  int direction = 8;
  writeRadarOn(0, 0, GC9A01A_BLACK);
  while (timeNotExpired(time))
  {
    tft.setCursor(TEXT_LINES[line][0], TEXT_LINES[line][1]);
    tft.setTextColor(create_rgb(color, 0, 0));
    tft.print(s);
    color += direction;
    if (color >= 255)
    {
      color = 255;
      direction = -8;
    }
    if (color < 0)
    {
      color = 0;
      direction = 8;
    }
  }
  tft.setCursor(TEXT_LINES[line][0], TEXT_LINES[line][1]);
  tft.setTextColor(GC9A01A_BLACK);
  tft.print(s);
}

// Geometry functions
//
void drawLineAtAngle(int x, int y, int length, double angle, uint16_t color)
{
  double radians = angle * M_PI / 180.0;
  int x2 = x + length * cos(radians);
  int y2 = y + length * sin(radians);
  tft.drawLine(x, y, x2, y2, color);
}

void drawLineAtAngleCoord(const int coord[], uint16_t color)
{
  drawLineAtAngle(coord[0], coord[1], coord[2], coord[3], color);
}

void drawRectAtAngle(int cx, int cy, int w, int h, int degrees, uint16_t color)
{
  double radians = degrees * M_PI / 180.0;
  int x1, y1, x2, y2, x1cx, y1cy;
  double sinRad = sin(radians);
  double cosRad = cos(radians);
  double w2 = w / 2;
  double h2 = h / 2;

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {

      x1 = cx + j - w2;
      y1 = cy + i - h2;

      x1cx = x1 - cx;
      y1cy = y1 - cy;

      x2 = cx + x1cx * cosRad - y1cy * sinRad;
      y2 = cy + x1cx * sinRad + y1cy * cosRad;

      tft.drawPixel(x2, y2, color);
    }
  }
}

void drawRectAtAngleCoord(const int coord[], uint16_t color)
{
  drawRectAtAngle(coord[0], coord[1], coord[2], coord[3], coord[4], color);
}


void drawArc(int start_degree, int end_degree, int r, uint16_t color)
{
  Coordinate pixel_coordinates[num_degrees];
  for (int i = 0; i < num_degrees; i++)
  {
    pixel_coordinates[i].x = CENTER + r * cos(i * M_PI / 180);
    pixel_coordinates[i].y = CENTER + r * sin(i * M_PI / 180);
  }

  for (int i = start_degree; i <= end_degree; i++)
  {
    tft.drawPixel(pixel_coordinates[i].x, pixel_coordinates[i].y, color);
  }
}

void drawArcWidth(int start_degree, int end_degree, int radius, int increaseWidth, uint16_t color)
{

  for (int i = -increaseWidth; i <= increaseWidth; i++)
  {
    drawArc(start_degree, end_degree, radius + i, color);
  }
}

void drawArcWidthCoord(const int coord[], uint16_t color)
{
  drawArcWidth(coord[0], coord[1], coord[2], coord[3], color);
}

void drawCircleCentered(int r, boolean filled, int increaseWidth, uint16_t color)
{
  if (filled)
  {
    tft.fillCircle(CENTER, CENTER, r, color);
  }
  else
  {
    for (int i = -increaseWidth; i <= increaseWidth; i++)
    {
      tft.drawCircle(CENTER, CENTER, r + i, color);
    }
  }
}

// Display Functions
//
void clearScreen(void)
{
  tft.fillScreen(GC9A01A_BLACK);
}

void displayAurebeshString(char *s, int line, int speed, uint16_t color)
{
  tft.setCursor(TEXT_LINES[line][0], TEXT_LINES[line][1]);
  tft.setTextColor(color);
  for (int i = 0; i < strlen(s); i++)
  {
    tft.print(s[i]);
    delay(speed);
  }
}

// Color Function
//
uint16_t create_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
  return ((red & 0b11111000) << 8) | ((green & 0b11111100) << 3) | (blue >> 3);
}

// Duration Functions
//
Timer initTimer(unsigned long durationMS)
{
  return {millis(), durationMS};
}

boolean timeNotExpired(Timer time)
{
  return millis() - time.time_start < time.durationMs;
}