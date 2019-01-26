#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define TS_MINX 920
#define TS_MAXX 100
#define TS_MINY 85
#define TS_MAXY 870

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
MCUFRIEND_kbv tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3

#define MINPRESSURE 10
#define MAXPRESSURE 1000

int questionNumber;
int previousQuestion = 0;
int lives;

int answer1;
int answer2;
int answer3;
int answer4;

void setup() {
  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW);
  Serial.begin(9600);
  tft.reset();
  tft.begin(tft.readID());
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.fillScreen(WHITE);
  questionNumber = 1;
  lives = 3;
  answer1 = 0;
  answer2 = 0;
  answer3 = 0;
  answer4 = 0;
}

void loop() {
  if (lives == 0)
  {
    
  }
  
  if (answer1 == 1)
  {
    --lives;
    if (lives == 0)
      gameOver();
    else
    {
      questionOne();
      delay(250);
      answer1 = 0;
    }
  }

  if(questionNumber == 1 && previousQuestion == 0)
    questionOne();

  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MAXY, TS_MINY, tft.height(), 0);
    Serial.print(p.x);
    Serial.print(" ");
    Serial.println(p.y);
    if (questionNumber == 1)
    {
      if (p.x >= 10 && p.x < 110) //First block
      {
        if(p.y >= 180 && p.y < 240)
        {
          answer1 = 1;
        }
      }
    }
  }
}

void gameOver(void)
{
  tft.fillScreen(WHITE);
  answer1 = 0;
  answer2 = 0;
  answer3 = 0;
  answer4 = 0;
  lives = 3;
  previousQuestion = 0;
  questionOne();
}

void questionOne(void)
{
  //Reset screen
  tft.fillScreen(WHITE);
  
  //Question number
  tft.fillRect(0, 0, 50, 50, RED);
  tft.fillRect(5, 5, 40, 40, WHITE);
  tft.setCursor(17, 17);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("1.");

  //Lives
  tft.setCursor(10, 300);
  tft.println("LIVES:"); tft.setCursor(90, 300);
  if (lives == 3)
    tft.setTextColor(GREEN);
  else if (lives == 2)
    tft.setTextColor(YELLOW);
  else if (lives == 1)
    tft.setTextColor(RED);
  tft.println(lives);

  //Question
  tft.setCursor(60, 17);
  tft.setTextColor(BLUE);
  tft.println("HOW MANY HOLES");
  tft.setCursor(60, 35);
  tft.println("IN A POLO?");

  //Answer blocks
  tft.setTextColor(BLACK);
  tft.setTextSize(2.5);
  //1
  tft.fillRect(10, 180, 100, 50, BLUE);
  tft.fillRect(20, 190, 80, 30, WHITE);
  tft.setCursor(43, 198);
  tft.println("ONE");
  
  //2
  tft.fillRect(130, 180, 100, 50, BLUE);
  tft.fillRect(140, 190, 80, 30, WHITE);
  tft.setCursor(163, 198);
  tft.println("TWO");

  //3
  tft.fillRect(10, 240, 100, 50, BLUE);
  tft.fillRect(20, 250, 80, 30, WHITE);
  tft.setCursor(30, 258);
  tft.println("THREE");

  //4
  tft.fillRect(130, 240, 100, 50, BLUE);
  tft.fillRect(140, 250, 80, 30, WHITE);
  tft.setCursor(157, 258);
  tft.println("FOUR");
  
  previousQuestion = 1;
}