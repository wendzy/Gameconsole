//
// ILI9341 TFT driver
//
// by SHARANYADAS
// http://community.atmel.com/projects/ili9341-library-drive-22-tft-displayderived-adafruit-tft-library-ili9340-type-controller
//

#ifndef ILI9341_H
#define ILI9341_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <limits.h>
#include <inttypes.h>
#include <util/atomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  6x8 font
//    LSB is top
//    MSB is bottom
//
static const uint8_t smallFont[][6] PROGMEM =
#include "font_6x8_iso8859_1.h"

#define ILI9341_HW_SPI

#define ILI9341_DC_PORT PORTC
#define ILI9341_DC_DDR  DDRC
#define ILI9341_DC_PIN  PINC
#define ILI9341_DC      PC1

#define ILI9341_CS_PORT PORTC
#define ILI9341_CS_DDR  DDRC
#define ILI9341_CS_PIN  PINC
#define ILI9341_CS      PC0

#define ILI9341_RST_PORT PORTC
#define ILI9341_RST_DDR  DDRC
#define ILI9341_RST_PIN  PINC
#define ILI9341_RST      PC2

#define ILI9341_LED_PORT PORTB
#define ILI9341_LED_DDR  DDRB
#define ILI9341_LED_PIN  PINB
#define ILI9341_LED      PB0

#define ILI9341_TFTHEIGHT 240 
#define ILI9341_TFTWIDTH 320

#define ILI9341_COLOR(R,G,B)  (((R & 0b11111000)<<8) | ((G & 0b11111100) << 3) | ((B & 0b11111000) >> 3))

#define ILI9341_COLOR_BLACK       0x0000      
#define ILI9341_COLOR_NAVY        0x000F      
#define ILI9341_COLOR_DARKGREEN   0x03E0      
#define ILI9341_COLOR_DARKCYAN    0x03EF      
#define ILI9341_COLOR_MAROON      0x7800      
#define ILI9341_COLOR_PURPLE      0x780F      
#define ILI9341_COLOR_OLIVE       0x7BE0      
#define ILI9341_COLOR_LIGHTGREY   0xC618      
#define ILI9341_COLOR_DARKGREY    0x7BEF      
#define ILI9341_COLOR_BLUE        0x001F      
#define ILI9341_COLOR_GREEN       0x07E0      
#define ILI9341_COLOR_CYAN        0x07FF      
#define ILI9341_COLOR_RED         0xF800     
#define ILI9341_COLOR_MAGENTA     0xF81F      
#define ILI9341_COLOR_YELLOW      0xFFE0      
#define ILI9341_COLOR_WHITE       0xFFFF      
#define ILI9341_COLOR_ORANGE      0xFD20      
#define ILI9341_COLOR_GREENYELLOW 0xAFE5     
#define ILI9341_COLOR_PINK        0xF81F

#define ILI9341_ROTATE_PORTRAIT_DOWN   0
#define ILI9341_ROTATE_PORTRAIT_UP     2
#define ILI9341_ROTATE_LANDSCAPE_RIGHT 1
#define ILI9341_ROTATE_LANDSCAPE_LEFT  3

//
//  turn backlight on/off
//
void ili9341_backlight(uint8_t light);
void ili9341_hard_init(void);
void ili9341_spi_init(void);
void ili9341_spi_send(unsigned char spi_data);
void ili9341_writecommand8(uint8_t com);
void ili9341_writedata8(uint8_t data);
void ili9341_setaddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ili9341_hard_reset(void);
void ili9341_init(void);
void ili9341_setRotation(uint8_t x);

uint16_t ili9341_xsize(void);
uint16_t ili9341_ysize(void);

void ili9341_clear(uint16_t color);
void ili9341_pushcolor(uint16_t color);

void ili9341_drawpixel(uint16_t x3,uint16_t y3,uint16_t color1);
void ili9341_drawvline(uint16_t x,uint16_t y,uint16_t h,uint16_t color);
void ili9341_drawhline(uint16_t x,uint16_t y,uint16_t w,uint16_t color);
void ili9341_drawline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void ili9341_fillrect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color);

int16_t ili9341_putchar_printf(char var, FILE *stream);
void ili9341_drawchar(int16_t x, int16_t y, unsigned char c,uint16_t color, uint16_t bgcolor, uint8_t size);
void ili9341_setcursor(uint16_t x, uint16_t y);
void ili9341_settextcolor(uint16_t color, uint16_t bgcolor);
void ili9341_settextsize(uint8_t s);
void ili9341_write(uint8_t c);
#endif

