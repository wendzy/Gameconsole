#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ili9341.h>

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
  while (1) 
  {
    _delay_ms(100);
}
 
  }
