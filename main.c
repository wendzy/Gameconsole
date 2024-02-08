#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "ili9341.h"
//test 
int main(void)
{
  PORTD |= 1;  // pull-up on IRQ
  ili9341_init();//initial driver setup to drive ili9341

  ili9341_backlight(1);
  ili9341_clear(ILI9341_COLOR_BLACK);//fill screen with black colour
  ili9341_setRotation(1);//rotate screen
  _delay_ms(2);

  ili9341_settextcolor(ILI9341_COLOR_CYAN, ILI9341_COLOR_BLACK);
  ili9341_settextsize(4);

  while (1) 
  {
    ili9341_drawhline(0,0,25,ILI9341_COLOR_GREEN);
    ili9341_drawpixel(50,50,ILI9341_COLOR_PURPLE);
    _delay_ms(1000);
  }
}
hej