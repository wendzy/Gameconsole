#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ili9341.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#define SPEAKER_PORT PORTD
#define SPEAKER_DDR DDRD
#define SPEAKER_PIN 2
#define SERVE_PORT PORTD
#define SERVE_DDR DDRD
#define SERVE_PIN 0
#define SERVE2_PORT PORTD
#define SERVE2_DDR DDRD
#define SERVE2_PIN 1
//audio put in the function of ball paddle and ball later
    //wall sound (play when ball touch the wall)
#define wall_tone playSound(16,226);
    //paddle sound (play when ball touch the paddle)
#define paddle_tone playSound(96,459);
    //point sound
#define point_tone playSound(257,490);

//function prototype

///////////////////////////////score////////////////////////////////////
//score function
void displayScore( int score_p1, int score_p2);
void updateScore(int ballX,int *score_p1, int *score_p2, int ballChangeX,int ballChangeY);
int winCon(int score_p1, int score_p2, int max_score);
///////////////////////////////score////////////////////////////////////
///////////////////////////////Paddle//////////////////////////////////// 
void p1_pos(int p, int ps);
void p2_pos(int p, int ps);
int check_p(int x, int bz, int bh);
void drawPaddle(int x, int y, int s);
void drawBall(int x, int y);
///////////////////////////////Paddle////////////////////////////////////
///////////////////////////////audio////////////////////////////////////
// audio function
void playSound(float duration, float frequency);
///////////////////////////////audio////////////////////////////////////
void updateBallPosition (int *batsize,int *ballX, int *ballY, int *ballChangeX, int *ballChangeY, int p1y, int p2y, int *Speed);
int main(void)
{
///////////////////////////////Paddle////////////////////////////////////
  //paddle init 
  //variable for p1 and p2 position
  int p1x = 2;
  int p2x = 315;
  uint16_t p1y = 0;
  int p2y = 0;
 uint16_t batsize = 20;
  int Speed = 1;
  uint16_t boarder_h = 220;
  int paddlespeed = 5;
  // GPIO pins init
  //player 1
  DDRC &= ~(1 << PINC4) | ~(1 << PINC5); // set pin as input
  PORTC |= (1 << PINC4)| (1 << PINC5); //set pin as high 
  //player 2
  DDRD &= ~(1 << PIND3) | ~(1 << PIND4); // set pin as input
  PORTD |= (1 << PIND3)| (1 << PIND4); //set pin as high 

///////////////////////////////Paddle////////////////////////////////////

  PORTD |= 0b0000011;
  DDRC |= (1 << PINC5);
  DDRC &= ~(1 << PINC4); //set pin as input
  PORTC |= (1<< PINC4);     // set pin 3 of Port B as output
 
  ili9341_init();  //initial driver setup to drive ili9341

  ili9341_backlight(1);
  ili9341_clear(ILI9341_COLOR_BLACK);//fill screen with black colour
  ili9341_setRotation(1);//rotate screen
  _delay_ms(2);

  ili9341_settextcolor(ILI9341_COLOR_CYAN, ILI9341_COLOR_BLACK);
  ili9341_settextsize(4);
  ili9341_setcursor(50,50);

 ///////////////////////////////ball////////////////////////////////////
  int ballX = 0;
  int ballY = 0;
  int ballChangeX = 0;
  int ballChangeY = 0;
  ///////////////////////////////ball////////////////////////////////////
 
 
 
  ///////////////////////////////score////////////////////////////////////
    int score_p1 = 0;
    int score_p2 = 0;
    int max_score = 10;
    // maximun bound for player1
    int p1_bounds = 320;
    // maximun bound for player2
    int p2_bounds = 0;
///////////////////////////////score////////////////////////////////////
  
///////////////////////////////audio////////////////////////////////////
///////////////////////////////audio////////////////////////////////////


while (1) 
{
///////////////////////////////ball////////////////////////////////////
//game start serve
if ((ballX == 0) && (ballChangeX == 0))
  {
    if (bit_is_set(PIND,0)) //analog input
    {
      ballChangeX = 1;
      ballChangeY = 0;
      paddle_tone;
    } //serve

     if (bit_is_set(PIND,1)) //analog input
    {
      ballChangeX = -1;
      ballChangeY = 0;
      paddle_tone;
    } //serve
  drawBall(8,8);
 //updateBallPosition (batsize,ballX, ballY, ballChangeX, ballChangeY, p1y, p2y, Speed);

  ili9341_drawline(0,0,320,0,ILI9341_COLOR_WHITE);
  ili9341_drawline(0,0,0,320,ILI9341_COLOR_WHITE);
   ili9341_drawline(320,0,0,0,ILI9341_COLOR_WHITE);
  ili9341_drawline(0,320,0,0,ILI9341_COLOR_WHITE);
    ///////////////////////////////ball////////////////////////////////////

  ///////////////////////////////Paddle////////////////////////////////////
//check if paddle pos over boarder
p1_pos(p1y,paddlespeed);
p2_pos(p2y,paddlespeed);
p1y = check_p(p1y,batsize,boarder_h);
p2y = check_p(p2y,batsize,boarder_h);
//draw paddle
drawPaddle(p1x,p1y,batsize);
drawPaddle(p2x,p2y,batsize);

 ///////////////////////////////Paddle////////////////////////////////////
///////////////////////////////score////////////////////////////////////

//display score
        displayScore(score_p1, score_p2);
        //update score
        ballX = ballX + ballChangeX;
        if (ballX <  p1_bounds || ballX > p2_bounds)
        {
//point sound
           point_tone;
           updateScore(ballX,&score_p1,&score_p2,ballChangeX,ballChangeY);
        }
        //display the winner
        int winner = winCon(score_p1,score_p2,max_score);
        if (winner != 0)
        {
            printf("Game over!\n");
            if (winner == 1)
            {
                printf("Player 1 wins! \n");
                //set position
            }
            else 
            {
                printf("Player 2 wins! \n");
               //set position
            }
        break;
        }
///////////////////////////////score////////////////////////////////////
    _delay_ms(100);
    }
  }
} 
///////////////////////////////ball////////////////////////////////////

void drawBall(int x, int y)
{
   //draw 2x2 ball at x,y

    ili9341_fillrect(x,y,8,8,ILI9341_COLOR_WHITE);
}

void updateBallPosition (int *batsize,int *ballX, int *ballY, int *ballChangeX, int *ballChangeY, int p1y, int p2y, int *Speed) 
{

    int ballSize = 8;
    int margin = 5;
    //Update ball position with increasing of speed
    *ballX += *ballChangeX * (*Speed);
    *ballY += *ballChangeY * (*Speed);

    //check if ball hit wall
    if (*ballY <= 0 || *ballY + ballSize >=  220)
    {
       playSound(16,226);
        *ballChangeY = -(*ballChangeY);
    }
    

    if ((*ballX <= margin + ballSize && ballChangeX < 0) || (*ballX + ballSize >= 330 - margin && *ballChangeX >0) )
    {
        if(*ballX <= margin + ballSize && ballChangeX < 0)
        {
            if (*ballX >= p1y - ballSize && ballY <= p1y +batsize)
            {
                paddle_tone;
                *ballChangeX = -(*ballChangeX);
                *Speed += 1;
            }
        }
        else if(*ballY >= p2y - ballSize && *ballY <= p2y + batsize)
        {
            paddle_tone;
            *ballChangeX = -(*ballChangeX);
             *Speed += 1; 
        }
    }
    
    //Check if ball is out of bound
    if (*ballX <= 0)
    {
        point_tone;
        *ballX = 330/2 - ballSize/2;
        *ballY = 220/2 - ballSize/2;
        *ballChangeX = -(*ballChangeX);
        *Speed = 1;
    } 
    else if(*ballX + ballSize >= 330)
    {
        point_tone;
        *ballX = 330/2 - ballSize/2;
        *ballY = 220/2 - ballSize/2;
        *ballChangeX = -(*ballChangeX);
        *Speed = 1;
    }

}
///////////////////////////////ball////////////////////////////////////
///////////////////////////////Paddle////////////////////////////////////
  //paddle update position
void p1_pos(int p,int ps) 
{
  if (bit_is_set(PINC, 4)) //if button is pressed, check low
  { // shift up
    p += ps;
  }
  else if (bit_is_set(PINC, 5))
  { // shift down
    p -= ps;
  }
}

void p2_pos(int p,int ps) 
{
  if (bit_is_set(PINB, 3)) //if button is pressed, check low
  { // shift up
    p += ps;
  }
  else if (bit_is_set(PINB, 4))
  { // shift down
    p -= ps;
  }
}

  //check pos limit
  int check_p(int x, int bz, int bh){
  if (x > bh) {
  x = bh-bz;
  }
  else if (x < bh){
  x = 0;
  }
  else{
}
    return x;
  }


void drawPaddle(int x, int y, int s)
{ //draw paddle starting at x,y, extending s down
  for (int i = 0; i < s; i++)
  {
    ili9341_drawpixel(x, y + i, ILI9341_COLOR_WHITE);
    ili9341_drawpixel(x+1, y + i, ILI9341_COLOR_WHITE);
    ili9341_drawpixel(x+2, y + i, ILI9341_COLOR_WHITE);
  }
}


///////////////////////////////Paddle////////////////////////////////////
///////////////////////////////score////////////////////////////////////
//function for score
//display score
void displayScore( int score_p1, int score_p2) 
{
  //give position to score board

  //printf("P1 %d | P2 %d", score_p1,score_p2);
}

// update score
void updateScore(int ballX,int *score_p1,int *score_p2, int ballChangeX, int ballChangeY) 
{
    if (ballX < 330){
        ballX = 0;
        ballChangeX = 0;
        ballChangeY = 0;
        (*score_p1) ++;
    }
    else if (ballX > 0)
    {
        ballX = 30;
        ballChangeX = 0;
        ballChangeY = 0;
        (*score_p2) ++;
    }
    
}

// check winner
int winCon(int score_p1,int score_p2, int max_score){
    if (score_p1 >= max_score){
        return 1;
    } 
    else if(score_p2 >= max_score){
        return 2;
    }
    return 0;
}
///////////////////////////////score////////////////////////////////////
///////////////////////////////audio////////////////////////////////////
void playSound(float duration, float frequency)
{
    long int i,cycles;
    float half_period;
    float wavelength;

    wavelength = (1/frequency)*1000;
    cycles = duration/wavelength;
    half_period = wavelength/2;

    //pin for buzzer
    SPEAKER_DDR |= (1 << SPEAKER_PIN );

    for (i=0; i<cycles;i++)
    {
        _delay_ms(50);
        SPEAKER_PORT |= (1<< SPEAKER_PIN );
        _delay_ms(50);
        SPEAKER_PORT |= (1<< SPEAKER_PIN );
    }

    return;
}
///////////////////////////////audio////////////////////////////////////
