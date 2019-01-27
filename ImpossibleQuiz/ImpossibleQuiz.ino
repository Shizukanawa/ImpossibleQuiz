#include <Adafruit_GFX.h>
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
#define ORANGE  0xFC00
#define PURPLE  0x69DD
#define LIGHTBLUE 0xAD7F
#define DARKGREEN 0x1380

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
int lives;
int fiveTwoCheck = 0;

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
  tft.fillScreen(WHITE);
  questionNumber = 0;
  lives = 3;
  answer1 = 0;
  answer2 = 0;
  answer3 = 0;
  answer4 = 0;
  gameStart();
}

/*extern const uint8_t abundance[];
  tft.setAddrWindow(tft.width() - 40 - 40, 20 + 0, tft.width() - 1 - 40, 20 + 39);
  tft.pushColors(abundance, 1600, 1); */

/*extern const uint8_t abundance[];
  tft.setAddrWindow(tft.width() - 140, 20 + 100, tft.width() - 1  - 100, 20 + 139);
  tft.pushColors(abundance, 1600, 1);*/
/* -1 in the 3rd parameter is important */  

/* Code for displaying the image of abundance in images.c */
/* http://www.rinkydinkelectronics.com/_t_doimageconverter565.php */

void loop() {
  checkAnswers();
  
  checkPresses();
}

void gameOver(void)
{
  tft.fillScreen(BLACK);
  answer1 = 0;
  answer2 = 0;
  answer3 = 0;
  answer4 = 0;
  lives = 3;
  questionNumber = -1;
  tft.setCursor(30, 20);
  tft.setTextSize(8);
  tft.setTextColor(RED);
  tft.println("GAME");
  tft.setCursor(30, 85);
  tft.println("OVER");

  tft.setCursor(30, 200);
  tft.setTextSize(3);
  tft.setTextColor(GREEN);
  tft.println("TRY AGAIN?");
}

void gameStart(void)
{
  tft.fillScreen(WHITE);
  
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.setCursor(25, 50);
  tft.println("THE");

  tft.setTextColor(PURPLE);
  tft.setTextSize(3);
  tft.setCursor(30, 59);
  tft.println("IMPOSSIBLE");

  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.setCursor(161, 82);
  tft.println("QUIZ");
  
  tft.fillRect(25, 200, 190, 50, BLUE);
  tft.fillRect(30, 205, 180, 40, WHITE);
  tft.setCursor(63, 210);
  tft.setTextColor(BLACK);
  tft.setTextSize(4);
  tft.println("START");
}

void checkPresses(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MAXY, TS_MINY, tft.height(), 0);
    Serial.print(p.x);
    Serial.print(" ");
    Serial.println(p.y);
    switch(questionNumber)
    {
      case -1:
        if (p.x >= 30 && p.x < 210)
        {
          if(p.y >= 210 && p.y < 230)
          {
            questionOne();
            delay(250);
            questionNumber = 1;
          }
        }
        break;
      case 0:
        if (p.x >= 26 && p.x < 212)
        {
          if (p.y >= 200 && p.y < 260)
          {
            questionOne();
            delay(250);
            questionNumber = 1;
          }
        }
        break;
      case 4:
        if (p.x >= 80 && p.x < 200 && p.y >= 35 && p.y < 45)
        {
          questionFive();
          delay(250);
          ++questionNumber;
        }
        else
          defaultAnswers(p);
        break;
      case 5:
        if (p.x >= 200 && p.x < 240 && p.y >= 200 && p.y < 260)
        {
          if (!fiveTwoCheck)
            questionFive2();
        }
        else if (p.x >= 0 && p.x < 240 && p.y >= 310 && p.y < 350)
        {
          questionNumber = questionNumber;
          /* CODE FOR SKIP & SAFEZONE */
        }
        else if (p.x >= 0 && p.x < 50 && p.y >= 0 && p.y < 50)
          questionNumber = questionNumber;
          /* SAFEZONE FOR QUESTION NUMBER */
        else if (p.x >= 0 && p.x < 45 && p.y >= 150 && p.y < 220)
        {
          ++questionNumber;
          tft.fillScreen(BLACK);
          extern const uint8_t abundance[];
          tft.setAddrWindow(tft.width() - 140, 20 + 100, tft.width() - 1  - 100, 20 + 139);
          tft.pushColors(abundance, 1600, 1);
        }
        else
        {
          if(fiveTwoCheck)
          {
            --lives;
            questionFive2();
            delay(250);
            if(lives == 0)
              gameOver();
          }
        }
        break;
      default:
        defaultAnswers(p);
        break;
    }
  }
}

void checkAnswers(void)
{
  /* ANSWER 1 */
  if (answer1)
  {
    switch (questionNumber)
    {
      case 1:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionOne();
          delay(250);
          answer1 = 0; 
        }
        break;
      case 2:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionTwo();
          delay(250);
          answer1 = 0;    
        }
        break;
      case 3:
        questionFour();
        delay(250);
        answer1 = 0;
        ++questionNumber;
        break;
      case 4:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionFour();
          delay(250);
          answer1 = 0;
        }
        break;
    }
  }
  
  /* ANSWER TWO */
  else if (answer2)
  {
    switch (questionNumber)
    {
      case 1:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionOne();
          delay(250);
          answer2 = 0;
        }
        break;
      case 2:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionTwo();
          delay(250);
          answer2 = 0;
        }
        break;
      case 3:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionThree();
          delay(250);
          answer2 = 0;
        }
        break;
      case 4:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionFour();
          delay(250);
          answer2 = 0;
        }
        break;
    }
  }

  /* ANSWER 3 */
  else if (answer3)
  {
    switch (questionNumber)
    {
      case 1:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionOne();
          delay(250);
          answer3 = 0;
        }
        break;
      case 2:
        questionThree();
        delay(250);
        answer3 = 0;
        ++questionNumber;
        break;
      case 3:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionThree();
          delay(250);
          answer3 = 0;
        }
        break;
      case 4:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionFour();
          delay(250);
          answer3 = 0;
        }
        break;
    }
  }

  /* ANSWER 4 */
  else if (answer4)
  {
    switch (questionNumber)
    {
      case 1:
        questionTwo();
        delay(250);
        answer4 = 0;
        ++questionNumber;
        break;
      case 2:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionTwo();
          delay(250);
          answer4 = 0;
        }
        break;
      case 3:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionThree();
          delay(250);
          answer4 = 0;
        }
        break;
      case 4:
        --lives;
        if (lives == 0)
          gameOver();
        else
        {
          questionFour();
          delay(250);
          answer4 = 0;
        }
        break;
    }
  }
}

void defaultAnswers(TSPoint p)
{
  if (p.x >= 10 && p.x < 110) //First block
  {
    if (p.y >= 180 && p.y < 240)
      answer1 = 1;
    else if (p.y >= 250 && p.y < 300) //Third Block
      answer3 = 1;
  }
  else if (p.x >= 135 && p.x < 230) //Second block
  {
    if (p.y >= 180 && p.y < 240)
      answer2 = 1;
    else if (p.y >= 250 && p.y < 300) //Fourth Block
      answer4 = 1;
  }
}

void printLives(void)
{
  //Lives
  tft.setCursor(10, 300);
  tft.println("LIVES:"); tft.setCursor(90, 300);
  switch (lives)
  {
    case 1:
      tft.setTextColor(RED);
      break;
    case 2:
      tft.setTextColor(YELLOW);
      break;
    case 3:
      tft.setTextColor(GREEN);
      break;
  }
  tft.println(lives);
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

  printLives();

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
}

void questionTwo(void)
{
  //Reset screen
  tft.fillScreen(WHITE);
  
  //Question number
  tft.fillRect(0, 0, 50, 50, RED);
  tft.fillRect(5, 5, 40, 40, WHITE);
  tft.setCursor(17, 17);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("2.");

  printLives();

  //Question
  tft.setCursor(60, 17);
  tft.setTextColor(BLUE);
  tft.println("CAN A");
  tft.setCursor(80, 35);
  tft.println("MATCH BOX?");

  //Answer blocks
  tft.setTextColor(BLACK);
  tft.setTextSize(2.5);
  //1
  tft.fillRect(10, 180, 100, 50, BLUE);
  tft.fillRect(20, 190, 80, 30, WHITE);
  tft.setCursor(43, 198);
  tft.println("YES");
  
  //2
  tft.fillRect(130, 180, 100, 50, BLUE);
  tft.fillRect(140, 190, 80, 30, WHITE);
  tft.setCursor(170, 198);
  tft.println("NO");

  //3
  tft.setTextSize(1);
  tft.fillRect(10, 240, 100, 50, BLUE);
  tft.fillRect(20, 250, 80, 30, WHITE);
  tft.setCursor(33, 258);
  tft.println("NO, BUT A");
  tft.setCursor(38, 268);
  tft.println("TIN CAN");

  //4
  tft.fillRect(130, 240, 100, 50, BLUE);
  tft.fillRect(140, 250, 80, 30, WHITE);
  tft.setCursor(142, 258);
  tft.println("YES, ONE BEAT");
  tft.setCursor(150, 268);
  tft.println("MIKE TYSON");
}

void questionThree(void)
{
  //Reset screen
  tft.fillScreen(WHITE);
  
  //Question number
  tft.fillRect(0, 0, 50, 50, RED);
  tft.fillRect(5, 5, 40, 40, WHITE);
  tft.setCursor(17, 17);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("3.");

  printLives();

  //Question
  tft.setCursor(80, 17);
  tft.setTextColor(BLUE);
  tft.println(".SDRAWKCAB");
  tft.setCursor(60, 35);
  tft.println("NOITSEUQ SIHT");
  tft.setCursor(100, 53);
  tft.println("REWSNA");

  //Answer blocks
  tft.setTextColor(BLACK);
  tft.setTextSize(2.5);
  //1
  tft.fillRect(10, 180, 100, 50, BLUE);
  tft.fillRect(20, 190, 80, 30, WHITE);
  tft.setCursor(43, 198);
  tft.println("K.O");
  
  //2
  tft.fillRect(130, 180, 100, 50, BLUE);
  tft.fillRect(140, 190, 80, 30, WHITE);
  tft.setCursor(152, 198);
  tft.println("WHAT?");

  //3
  tft.setTextSize(1);
  tft.fillRect(10, 240, 100, 50, BLUE);
  tft.fillRect(20, 250, 80, 30, WHITE);
  tft.setCursor(38, 258);
  tft.println("I DON'T");
  tft.setCursor(30, 268);
  tft.println("UNDERSTAND");

  //4
  tft.fillRect(130, 240, 100, 50, BLUE);
  tft.fillRect(140, 250, 80, 30, WHITE);
  tft.setCursor(162, 258);
  tft.println("TENNIS");
  tft.setCursor(165, 268);
  tft.println("ELBOW");
}

void questionFour(void)
{
  //Reset screen
  tft.fillScreen(WHITE);
  
  //Question number
  tft.fillRect(0, 0, 50, 50, RED);
  tft.fillRect(5, 5, 40, 40, WHITE);
  tft.setCursor(17, 17);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("4.");

  printLives();

  //Question
  tft.setCursor(110, 17);
  tft.setTextColor(BLUE);
  tft.println("CLICK");
  tft.setCursor(80, 35);
  tft.println("THE ANSWER");

  //Answer blocks
  tft.setTextColor(RED);
  tft.setTextSize(1.5);
  //1
  tft.fillRect(10, 180, 100, 50, BLUE);
  tft.fillRect(20, 190, 80, 30, WHITE);
  tft.setCursor(25, 202);
  tft.println("OUT OF ORDER");
  
  //2
  tft.fillRect(130, 180, 100, 50, BLUE);
  tft.fillRect(140, 190, 80, 30, WHITE);
  tft.setCursor(145, 202);
  tft.println("OUT OF ORDER");

  //3
  tft.fillRect(10, 240, 100, 50, BLUE);
  tft.fillRect(20, 250, 80, 30, WHITE);
  tft.setCursor(25, 262);
  tft.println("OUT OF ORDER");

  //4
  tft.fillRect(130, 240, 100, 50, BLUE);
  tft.fillRect(140, 250, 80, 30, WHITE);
  tft.setCursor(145, 262);
  tft.println("OUT OF ORDER");
}

void questionFive(void)
{
  //Reset screen
  tft.fillScreen(WHITE);
  
  //Question number
  tft.fillRect(0, 0, 50, 50, RED);
  tft.fillRect(5, 5, 40, 40, WHITE);
  tft.setCursor(17, 17);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("5.");

  printLives();
  
  tft.setTextSize(3);
  tft.setCursor(60, 17);
  tft.setTextColor(BLUE);
  tft.println("PUT THE");
  tft.setCursor(60, 45);
  tft.println("THUMB...");

  tft.setCursor(20, 150);
  tft.println("...ON HERE");

  tft.fillCircle(225, 220, 30, BLUE);
  tft.fillCircle(225, 220, 25, WHITE);
  tft.fillCircle(225, 220, 10, RED);

  for (int i = 0; i < 5; ++i)
    tft.drawLine(152 + i, 175, 185 + i, 205, BLUE); 
  for (int i = 0; i < 4; ++i)
    tft.drawFastVLine(186 + i, 185, 20, BLUE);
  for (int i = 0; i < 4; ++i)
    tft.drawFastHLine(168, 202 + i, 22, BLUE);
}

void questionFive2(void)
{
  fiveTwoCheck = 1;
  //Reset screen
  tft.fillScreen(BLUE);
  
  //Question number
  tft.fillRect(0, 0, 50, 50, RED);
  tft.fillRect(5, 5, 40, 40, LIGHTBLUE);
  tft.setCursor(17, 17);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("5.");

  tft.fillRect(0, 295, 249, 35, LIGHTBLUE);
  printLives();

  tft.fillCircle(225, 220, 30, GREEN);
  tft.fillCircle(225, 220, 10, RED);

  tft.fillCircle(10, 180, 30, GREEN);
  tft.fillCircle(10, 180, 10, RED);

  tft.setTextColor(LIGHTBLUE);
  tft.setCursor(100, 30);
  tft.println("NOW,");
  tft.setCursor(80, 60);
  tft.println("DON'T");
  tft.setCursor(100, 90);
  tft.println("TOUCH");
  tft.setCursor(120, 120);
  tft.println("BLUE");
}
