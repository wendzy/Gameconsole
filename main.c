#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ili9341.h>
//test pull
uint16_t playeronex = 5;
uint16_t playertwox = 315;
uint16_t playeroney = 0;
uint16_t playertwoy = 0;

void drawPaddle(int x, int y, int s)
{ //draw paddle starting at x,y, extending s down
  for (int i = 0; i < s; i++)
  {
    ili9341_drawpixel(x, y + i, ILI9341_COLOR_WHITE);
    ili9341_drawpixel(x+1, y + i, ILI9341_COLOR_WHITE);
    ili9341_drawpixel(x+2, y + i, ILI9341_COLOR_WHITE);
  }
}

void drawBall(int x, int y)
{ //draw 2x2 ball at x,y

    ili9341_fillrect(x,y,8,8,ILI9341_COLOR_WHITE);
}

  

int main(void)
{
  DDRC |= (1 << PINC5);
  DDRC &= ~(1 << PINC4); //set pin as input
  PORTC |= (1<< PINC4);     // set pin 3 of Port B as output
  PORTD |= 1;  // pull-up on IRQ
  ili9341_init();  //initial driver setup to drive ili9341

  ili9341_backlight(1);
  ili9341_clear(ILI9341_COLOR_BLACK);//fill screen with black colour
  ili9341_setRotation(1);//rotate screen
  _delay_ms(2);

  ili9341_settextcolor(ILI9341_COLOR_CYAN, ILI9341_COLOR_BLACK);
  ili9341_settextsize(4);
  ili9341_setcursor(50,50);
  drawPaddle(playeronex,100,50);
  drawPaddle(playertwox,100,50);
  drawBall(50,50);


  //paddle
  
  
  //ball

///////////////////////////////ball////////////////////////////////////

 if ((ballX == 0) && (ballChangeX == 0))
  {
    ballY = p1 + batsize / 2 - 1;
    if (!digitalRead(player_one_button))
    {
      ballChangeX = 1;
      ballChangeY = 0;
      speakerTone = serve_tone;
    } //serve
  }
  if ((ballX == 30) && (ballChangeX == 0))
  {
    ballY = p2 + batsize / 2 - 1;
    if (!digitalRead(player_two_button))
    {
      ballChangeX = -1;
      ballChangeY = 0;
      speakerTone = serve_tone;
    } //serve
  }


//player scoring
  ballX = ballX + ballChangeX;
  if (ballX > 30)
  {
    ballX = 0;
    ballChangeX = 0;
    ballChangeY = 0;
    playerOneScore = playerOneScore + 1;
    if (playerOneScore == 7)
    {
      playerOneVictory();
    }
  } //P2 has missed, P1 wins
  if (ballX < 0)
  {
    ballX = 30;
    ballChangeX = 0;
    ballChangeY = 0;
    playerTwoScore = playerTwoScore + 1;
    if (playerTwoScore == 7)
    {
      playerTwoVictory();
    }
  } //P1 has missed, P2 wins


//ball position court
  if (ballX == 29)
  { //ball is in player 2's court
    if (abs(ballY - p2 - 1) < 3)
    {                               //ball is within p2's bat
      ballChangeX = -1;             //goes back left
      ballChangeY = ballY - p2 - 1; //change ball angle
      if (ballChangeY == 0)
      {
        ballChangeY = random(-1, 2);
      }                               //mix it up a bit
      speakerTone = ball_paddle_tone; //hit bat
    }
  }
  if (ballX == 1)
  { //ball is in player 1's court
    if (abs(ballY - p1 - 1) < 3)
    {                               //ball is within p1's bat
      ballChangeX = 1;              //goes back right
      ballChangeY = ballY - p1 - 1; //change ball angle
      if (ballChangeY == 0)
      {
        ballChangeY = random(-1, 2);
      }                               //mix it up a bit
      speakerTone = ball_paddle_tone; //hit bat
    }
  }


  int ballChangeYtemp; //to work out half steps
  if (ballX & 1)
  { //on odd steps, only step if 2
    ballChangeYtemp = ballChangeY / 2;
  }
  else
  { //on even steps, step if 1 or 2
    ballChangeYtemp = 0;
    if (ballChangeY > 0)
    {
      ballChangeYtemp = 1;
    }
    if (ballChangeY < 0)
    {
      ballChangeYtemp = -1;
    }
  }
  ballY = ballY + ballChangeYtemp;

  //ball hitting walls
  if (ballY > 300)
  {
    ballY = 300;
    ballChangeY = -1;
    speakerTone = ball_wall_tone;
  } //hit top wall
  if (ballY < 20)
  {
    ballY = 20;
    ballChangeY = 1;
    speakerTone = ball_wall_tone;
  } //hit bottom wall
  ///////////////////////////////ball////////////////////////////////////


  //score

  
  //audio

  
  while (1) 
  {
    _delay_ms(100);
}
 
  }
