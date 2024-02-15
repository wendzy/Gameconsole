#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ili9341.h>
#define SPEAKER_PORT PORTD
#define SPEAKER_DDR DDRD
#define SPEAKER_PIN 2
#define SERVE_PORT PORTD
#define SERVE_DDR DDRD
#define SERVE_PIN 0
#define SERVE2_PORT PORTD
#define SERVE2_DDR DDRD
#define SERVE2_PIN 1

//function prototype

///////////////////////////////score////////////////////////////////////
//score function
void displayScore( int score_p1, int score_p2);
void updateScore(int pos_ball,int *score_p1, int *score_p2);
int winCon(int score_p1, int score_p2, int max_score);
///////////////////////////////score////////////////////////////////////
///////////////////////////////Paddle//////////////////////////////////// 
void p1_pos(int p1y);
void p2_pos(int p2y);
int check_p(int x);
void drawPaddle(int x, int y, int s);
void drawBall(int x, int y);
///////////////////////////////Paddle////////////////////////////////////
///////////////////////////////audio////////////////////////////////////
// audio function
void playSound(float duration, float frequency)
///////////////////////////////audio////////////////////////////////////


int main(void)
{
///////////////////////////////Paddle////////////////////////////////////
  //paddle init 
  //variable for p1 and p2 position
  uint16_t p1x = 5;
  uint16_t p2x = 315;
  uint16_t p1y = 0;
  uint16_t p2y = 0;
  uint16_t batsize = 50;

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
//audio put in the function of ball paddle and ball later
    //wall sound (play when ball touch the wall)
   ball_wall_tone = playSound(16,226);
    //paddle sound (play when ball touch the paddle)
    serve_tone = playSound(96,459);
    //point sound
    point_tone = playSound(257,490);
///////////////////////////////audio////////////////////////////////////

ili9341_drawline(0,0,320,220,ili9341_color_white)


while (1) 
  {
///////////////////////////////ball////////////////////////////////////
//game start serve
if ((ballX == 0) && (ballChangeX == 0))
  {
    if (bit_is_clear(SERVE_PIN,0)) //analog input
    {
      ballChangeX = 1;
      ballChangeY = 0;
      serve_tone;
    } //serve

     if (bit_is_clear(SERVE2_PIN,1)) //analog input
    {
      ballChangeX = -1;
      ballChangeY = 0;
      serve_tone;
    } //serve


//ball position, courts hitting the bat
  if (ballX == 300)
  { //ball is in player 2's court
    if (abs(ballY - p2y - 1) < )
    {                               //ball is within p2's bat
      ballChangeX = -1;             //goes back left
      ballChangeY = ballY - p2y - 1; //change ball angle
      if (ballChangeY == 0)
      {
        ballChangeY = random(-1, 2);
      }                               //mix it up a bit
      serve_tone; //hit bat
    }
  
  if (ballX == 1)
  { //ball is in player 1's court
    if (abs(ballY - p1y - 1) < 3)
    {                               //ball is within p1's bat
      ballChangeX = 1;              //goes back right
      ballChangeY = ballY - p1y - 1; //change ball angle
      if (ballChangeY == 0)
      {
        ballChangeY = random(-1, 2);
      }                               //mix it up a bit
     serve_tone; //hit bat
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

  //ball hitting top/bottom walls
  if (ballY > 220)
  {
    ballY = 220;
    ballChangeY = -1;
    ball_wall_tone;
  } //hit top wall
  if (ballY < 0)
  {
    ballY = 0;
    ballChangeY = 1;
    ball_wall_tone;
  } //hit bottom wall

    ///////////////////////////////ball////////////////////////////////////

  ///////////////////////////////Paddle////////////////////////////////////
//check if paddle pos over boarder
p1_pos(p1y);
p2_pos(p2y);
p1y = check_p(p1y);
p2y = check_p(p2y);
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
        if (winner != 0){
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
///////////////////////////////ball////////////////////////////////////

void drawBall(int x, int y)
{ //draw 2x2 ball at x,y

    ili9341_fillrect(x,y,8,8,ILI9341_COLOR_WHITE);
}
///////////////////////////////ball////////////////////////////////////
///////////////////////////////Paddle////////////////////////////////////
  //paddle update position
  void p1_pos(int p1y) {
  if (bit_is_clear(PINC, 4)) //if button is pressed, check low
  { // shift up
    p1y += 1;
  }
  else if (bit_is_clear(PINC, 5))
  { // shift down
    p1y -= 1;
  }

  }
   void p2_pos(int p2y) {
  if (bit_is_clear(PINB, 3)) //if button is pressed, check low
  { // shift up
    p2y += 1;
  }
  else if (bit_is_clear(PINB, 4))
  { // shift down
    p2y -= 1;
  }
  }

  //check pos limit
  int check_p(int x){
  if (x > boarder height) {
  x = boarder height;
  }
  else if (x < boarder height){
  x = 0 + batsize;
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
void displayScore( int score_p1, int score_p2) {
  //give position to score board
    printf("Player 1: %d | Player 2: %d\n", score_p1,score_p2);
}

// update score
void updateScore(int pos_ball,int *score_p1,int *score_p2, int ballChangeX, int ballChangeY) {
    if (pos_ball < p1_bounds){
        pos_ball = 0;
        ballChangeX = 0;
        ballChangeY = 0;
        (*score_p1) ++;
    }
    else if (pos_ball > p2_bounds)
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
void playSound(float duration, float frequency){
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
        _delay_ms(half_period);
        SPEAKER_PORT |= (1<< SPEAKER_PIN );
        _delay_ms(half_period);
        SPEAKER_PORT |= (1<< SPEAKER_PIN );
    }

    return;

}
///////////////////////////////audio////////////////////////////////////
