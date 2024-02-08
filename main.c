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

//function prototype


//score function
void displayScore( int score_p1, int score_p2);
void updateScore(int pos_ball,int *score_p1, int *score_p2);
int winCon(int score_p1, int score_p2, int max_score);
// audio function
void playSound(float duration, float frequency)

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
//score function declaration
void displayScore( int score_p1, int score_p2);
void updateScore(int pos_ball,int *score_p1, int *score_p2);
int winCon(int score_p1, int score_p2, int max_score);
  

int main(void)
{
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
  drawPaddle(playeronex,100,50);
  drawPaddle(playertwox,100,50);
  drawBall(50,50);


  //paddle
  
  
  //ball

  //score
    uint16_t score_p1 = 0;
    uint16_t score_p2 = 0;
    uint16_t max_score = 10;
    // maximun bound for player1
    uint16_t p1_bounds =;
    // maximun bound for player2
    uint16_t p2_bounds =;

  //audio put in the function of ball paddle and ball later
    //wall sound (play when ball touch the wall)
    playSound(16,226);
    //paddle sound (play when ball touch the paddle)
    playSound(96,459);
    //point sound
    playSound(257,490);

///////////////////////////////ball////////////////////////////////////
//ball hitting bat
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
  if (ballX > 300)
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
  if (ballX < 20)
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


//ball position, courts
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

  //ball hitting top/bottom walls
  if (ballY > 220)
  {
    ballY = 220;
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
//display score
        displayScore(score_p1, score_p2);
        int pos_ball = ;
        //update score
        if (pos_ball <  p1_bounds || pos_ball > p2_bounds)
        {
            //point sound
            playSound(257,490);
            updateScore(pos_ball,&score_p1,&score_p2);
        }
        //display the winner
        int winner = winCon(score_p1,score_p2,max_score);
        if (winner != 0){
            printf("Game over!\n");
            if (winner == 1)
            {
                printf("Player 1 wins! \n");
            }
            else 
            {
                printf("Player 2 wins! \n");
            }
        break;
        }
    _delay_ms(100);
  }
 
}

///////////////////////////////score////////////////////////////////////
//function for score
//display score
void displayScore( int score_p1, int score_p2) {
  //give position to score board
    printf("Player 1: %d | Player 2: %d\n", score_p1,score_p2);
}

// update score
void updateScore(int pos_ball,int *score_p1,int *score_p2) {
    if (pos_ball < p1_bounds){
        (*score_p1) ++;
    }
    else if (pos_ball > p2_bounds)
    {
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
///////////////////////////////audio////////////////////////////////////
void playSound(float duration, float frequency){
    long int i,cycles;
    float half_period;
    float wavelength;

    wavelength = (1/frequency)*1000;
    cycles = duration/wavelength;
    half_period = wavelength/2;

    //pin for buzzer
    SPEAKER_DDR |= (1 << "Buzzer pin");

    for (i=0; i<cycles;i++)
    {
        _delay_ms(half_period);
        SPEAKER_PORT |= (1<< SPEAKER_PIN );
        _delay_ms(half_period);
        SPEAKER_PORT |= (1<< SPEAKER_PIN );
    }

    return;

}
