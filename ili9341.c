//
// ILI9341 TFT driver
//
// by SHARANYADAS
// http://community.atmel.com/projects/ili9341-library-drive-22-tft-displayderived-adafruit-tft-library-ili9340-type-controller
//

#include "ili9341.h"

volatile uint16_t ILI9341_SIZE_X = ILI9341_TFTWIDTH;
volatile uint16_t ILI9341_SIZE_Y = ILI9341_TFTHEIGHT;
volatile uint16_t cursor_x;
volatile uint16_t cursor_y;
volatile uint16_t textcolor;
volatile uint16_t textbgcolor;
volatile uint8_t textsize;

//
// mydata declaration and converting it into stream
//
static FILE ili9341_printf = FDEV_SETUP_STREAM(ili9341_putchar_printf, NULL, _FDEV_SETUP_WRITE);

//
//  init hardware
//
void ili9341_hard_init(void)
{
  ILI9341_RST_DDR  |= (1 << ILI9341_RST);     //output for reset
  ILI9341_RST_PORT |= (1 << ILI9341_RST);     //pull high for normal operation

  ILI9341_DC_DDR  |= (1 << ILI9341_DC);     //output for reset
  ILI9341_CS_DDR  |= (1 << ILI9341_CS);     //pull high for normal operation
}

//
//  turn backlight on/off
//
void ili9341_backlight(uint8_t light)
{
  uint8_t oldval;
  oldval = ILI9341_LED_PORT;
  ILI9341_LED_DDR  |= (1 << ILI9341_LED);
  oldval           &= ~(1 << ILI9341_LED);
  ILI9341_LED_PORT  = oldval | ((light & 1) << ILI9341_LED);
}

#ifdef ILI9341_HW_SPI
//
//  set spi speed and settings 
//
void ili9341_spi_init(void)
{
  DDRB  |= (1<<1)|(1<<2)|(1<<3)|(1<<5); //CS,SS,MISO,SCK as output(although SS will be unused throughout the program)
  SPCR   = (1<<SPE)|(1<<MSTR);          //mode 0,fosc/4
  SPSR  |= (1<<SPI2X);                  //doubling spi speed.i.e final spi speed-fosc/2
  PORTB |= (1<<1);                      //cs off during startup
}

//
//send spi data to display
//
void ili9341_spi_send(unsigned char spi_data)
{
  SPDR = spi_data;              //move data into spdr
  while (!(SPSR & (1<<SPIF)));  //wait until the transmission is finished
}
#endif

#ifdef ILI9341_SW_SPI
//
//  set spi speed and settings
//
void ili9341_spi_init(void)
{
  DDRB  |= (1<<2)|(1<<3)|(1<<5); //CS,SS,MOSI,SCK as output(although SS will be unused throughout the program)
  SPCR   = (1<<SPE)|(1<<MSTR);          //mode 0,fosc/4
  SPSR  |= (1<<SPI2X);                  //doubling spi speed.i.e final spi speed-fosc/2
  ILI9341_CS_PORT |= (1 << ILI9341_CS);                      //cs off during startup
}

//
//send spi data to display
//
void ili9341_spi_send(unsigned char spi_data)
{
  while (!(SPSR & (1<<SPIF)));  //SPI still busy?
  SPDR = spi_data;              //move data into spdr
}
#endif

//
// command write
//
void ili9341_writecommand8(uint8_t com)
{
  ILI9341_DC_PORT &= ~(1 << ILI9341_DC);       //dc low during command
  ILI9341_CS_PORT &= ~(1 << ILI9341_CS);       //cs low
  _delay_us(5);    //little delay
  ili9341_spi_send(com);
  ILI9341_CS_PORT |= (1 << ILI9341_CS);        //cs high
}

//
// data write
//
void ili9341_writedata8(uint8_t data)
{
  ILI9341_DC_PORT |= (1 << ILI9341_DC);       //dc high for data
  _delay_us(1);             //delay
  ILI9341_CS_PORT &= ~(1 << ILI9341_CS);      //cs low
  ili9341_spi_send(data);
  ILI9341_CS_PORT |= (1 << ILI9341_CS);        //cs high
}

//
//  set coordinate for print or other function
//
void ili9341_setaddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
  ili9341_writecommand8(0x2A);
  ili9341_writedata8(x1>>8);
  ili9341_writedata8(x1);
  ili9341_writedata8(x2>>8);
  ili9341_writedata8(x2);

  ili9341_writecommand8(0x2B);
  ili9341_writedata8(y1>>8);
  ili9341_writedata8(y1);
  ili9341_writedata8(y2);
  ili9341_writedata8(y2);

  ili9341_writecommand8(0x2C);//meory write
}

//
//  hard reset display
//
void ili9341_hard_reset(void)
{
  ILI9341_RST_PORT |=  (1 << ILI9341_RST);  //pull high if low previously
  _delay_ms(200);
  ILI9341_RST_PORT &= ~(1 << ILI9341_RST);  //low for reset
  _delay_ms(200);
  ILI9341_RST_PORT |=  (1 << ILI9341_RST);  //again pull high for normal operation
  _delay_ms(200);
}

//
//  set up display using predefined command sequence
//
void ili9341_init(void)
{
  ili9341_hard_init();
  ili9341_spi_init();
  ili9341_hard_reset();
  ili9341_writecommand8(0x01);  // soft reset
  _delay_ms(100);
  //power control A
  ili9341_writecommand8(0xCB);
  ili9341_writedata8(0x39);
  ili9341_writedata8(0x2C);
  ili9341_writedata8(0x00);
  ili9341_writedata8(0x34);
  ili9341_writedata8(0x02);

  //power control B
  ili9341_writecommand8(0xCF);
  ili9341_writedata8(0x00);
  ili9341_writedata8(0xC1);
  ili9341_writedata8(0x30);

  //driver timing control A
  ili9341_writecommand8(0xE8);
  ili9341_writedata8(0x85);
  ili9341_writedata8(0x00);
  ili9341_writedata8(0x78);

  //driver timing control B
  ili9341_writecommand8(0xEA);
  ili9341_writedata8(0x00);
  ili9341_writedata8(0x00);

  //power on sequence control
  ili9341_writecommand8(0xED);
  ili9341_writedata8(0x64);
  ili9341_writedata8(0x03);
  ili9341_writedata8(0x12);
  ili9341_writedata8(0x81);

  //pump ratio control
  ili9341_writecommand8(0xF7);
  ili9341_writedata8(0x20);

  //power control,VRH[5:0]
  ili9341_writecommand8(0xC0);
  ili9341_writedata8(0x23);

  //Power control,SAP[2:0];BT[3:0]
  ili9341_writecommand8(0xC1);
  ili9341_writedata8(0x10);

  //vcm control
  ili9341_writecommand8(0xC5);
  ili9341_writedata8(0x3E);
  ili9341_writedata8(0x28);

  //vcm control 2
  ili9341_writecommand8(0xC7);
  ili9341_writedata8(0x86);

  //memory access control
  ili9341_writecommand8(0x36);
  ili9341_writedata8(0x48);

  //pixel format
  ili9341_writecommand8(0x3A);
  ili9341_writedata8(0x55);

  //frameration control,normal mode full colors
  ili9341_writecommand8(0xB1);
  ili9341_writedata8(0x00);
  ili9341_writedata8(0x18);

  //display function control
  ili9341_writecommand8(0xB6);
  ili9341_writedata8(0x08);
  ili9341_writedata8(0x82);
  ili9341_writedata8(0x27);

  //3gamma function disable
  ili9341_writecommand8(0xF2);
  ili9341_writedata8(0x00);

  //gamma curve selected
  ili9341_writecommand8(0x26);
  ili9341_writedata8(0x01);

  //set positive gamma correction
  ili9341_writecommand8(0xE0);
  ili9341_writedata8(0x0F);
  ili9341_writedata8(0x31);
  ili9341_writedata8(0x2B);
  ili9341_writedata8(0x0C);
  ili9341_writedata8(0x0E);
  ili9341_writedata8(0x08);
  ili9341_writedata8(0x4E);
  ili9341_writedata8(0xF1);
  ili9341_writedata8(0x37);
  ili9341_writedata8(0x07);
  ili9341_writedata8(0x10);
  ili9341_writedata8(0x03);
  ili9341_writedata8(0x0E);
  ili9341_writedata8(0x09);
  ili9341_writedata8(0x00);

  //set negative gamma correction
  ili9341_writecommand8(0xE1);
  ili9341_writedata8(0x00);
  ili9341_writedata8(0x0E);
  ili9341_writedata8(0x14);
  ili9341_writedata8(0x03);
  ili9341_writedata8(0x11);
  ili9341_writedata8(0x07);
  ili9341_writedata8(0x31);
  ili9341_writedata8(0xC1);
  ili9341_writedata8(0x48);
  ili9341_writedata8(0x08);
  ili9341_writedata8(0x0F);
  ili9341_writedata8(0x0C);
  ili9341_writedata8(0x31);
  ili9341_writedata8(0x36);
  ili9341_writedata8(0x0F);

  //exit sleep
  ili9341_writecommand8(0x11);
  _delay_ms(120);
  //display on
  ili9341_writecommand8(0x29);

  stdout = &ili9341_printf;
}

//
//set color for drawing
//
void ili9341_pushcolor(uint16_t color)
{
  ili9341_writedata8(color>>8);
  ili9341_writedata8(color);
}

//
//clear lcd and fill with color
//
void ili9341_clear(uint16_t color)
{
  uint16_t i,j;
  ili9341_setaddress(0,0,ILI9341_SIZE_X-1,ILI9341_SIZE_Y-1);

  for(i=0;i<ILI9341_SIZE_X;i++)  
  {
    for(j=0;j<ILI9341_SIZE_Y;j++)
    {
      ili9341_pushcolor(color);
    }
  }
}

//
//draw pixel
//pixels will always be counted from right side.
//  x is representing LCD width which will always be less than 240.
//  Y is representing LCD height which will always be less than 320
//
void ili9341_drawpixel(uint16_t x3,uint16_t y3,uint16_t color1)
{
  if ( (x3 < 0) 
    || (x3 >= ILI9341_SIZE_X) 
    || (y3 < 0) 
    || (y3 >= ILI9341_SIZE_Y))
  {
    return;
  }  
  ili9341_setaddress(x3,y3,x3+1,y3+1);
  ili9341_pushcolor(color1);
}

//
//draw vertical line
//  basically we will see this line horizental if we see the display 320*240
//
void ili9341_drawvline(uint16_t x,uint16_t y,uint16_t h,uint16_t color)
{
  if ((x >= ILI9341_SIZE_X) || (y >= ILI9341_SIZE_Y)) 
  {
    return;
  }  
  if((y+h-1)>=ILI9341_SIZE_Y)
  {
    h = ILI9341_SIZE_Y - y;
  }  
  ili9341_setaddress(x,y,x,y+h-1);
  while(h--) 
  {
    ili9341_pushcolor(color);
  }
}

//
//draw horizontal line
//
void ili9341_drawhline(uint16_t x,uint16_t y,uint16_t w,uint16_t color)
{
  if ((x >=ILI9341_SIZE_X) || (y >=ILI9341_SIZE_Y)) 
  {
    return;
  }  
  if ((x+w-1) >= ILI9341_SIZE_X)
  {
    w = ILI9341_SIZE_X - x;
    ili9341_setaddress(x,y,x+w-1,y);
    while(w--)
    {
      ili9341_pushcolor(color);
    }
  }  
}


/*--------------------------------------------------------------------------------------------------
  Name         :  ili9341_drawline
  Description  :  draws a line 
  Argument(s)  :  x0, y0, x1, y1, color

  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void ili9341_drawline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  int16_t dx, dy;
  int16_t sx=0, sy=0, err, e2;

  dx = abs(x1-x0);
  dy = abs(y1-y0); 
  if (x0 < x1)
  {
    sx = 1;
  }
  else if (x0 > x1)
  { 
    sx = -1;
  }
  if (y0 < y1)
  {
    sy = 1; 
  }
  else if (y0 > y1)
  {
    sy = -1;
  }
  err = dx-dy;
 
  while ((x0 != x1) || (y0 != y1))
  {
    ili9341_drawpixel(x0, y0, color);
    e2 = 2*err;
    if (e2 > -dy)
    {
      err = err - dy;
      x0  = x0 + sx;
    }
    if (e2 <  dx)
    {
      err = err + dx;
      y0  = y0 + sy; 
    }
  }
}


/*--------------------------------------------------------------------------------------------------
  Name         :  ili9341_drawline
  Description  :  draws a line 
  Argument(s)  :  x, y, w, h, color
                  w - width
                  h - height
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void ili9341_fillrect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color)
{
  if ((x >=ILI9341_SIZE_X) || (y >=ILI9341_SIZE_Y)) 
  {
    return;
  }  
  if ((x+w-1) >= ILI9341_SIZE_X)
  { 
    w = ILI9341_SIZE_X - x;
  }  
  if ((y+h-1)>=ILI9341_SIZE_Y)
  {
    h = ILI9341_SIZE_Y - y;
  }
  ili9341_setaddress(x, y, x+w-1, y+h-1);

  for (y=h; y>0; y--) 
  {
    for(x=w; x>0; x--)
    {
      ili9341_pushcolor(color);
    }
  }
}

/*--------------------------------------------------------------------------------------------------
  Name         :  ili9341_setRotation(uint8_t orientation) 
  Description  :  sets screen orientation (memory access control 36h) 
  Argument(s)  :  orientation
                  0 - 36h: 0b01001000 MY=0 MX=1 MV=0 ML=0 BGR=0 MH=1 portrait with chip down
                  1 - 36h: 0b01001000 MY=0 MX=0 MV=1 ML=0 BGR=0 MH=1 landscape with chip right
                  2 - 36h: 0b01001000 MY=1 MX=0 MV=0 ML=0 BGR=0 MH=1 portrait with chip up
                  3 - 36h: 0b01001000 MY=1 MX=1 MV=1 ML=0 BGR=0 MH=1 landscape with chip left
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void ili9341_setRotation(uint8_t rotation) 
{
  ili9341_writecommand8(0x36);
  rotation %= 4;
  switch (rotation) 
  {
    case 0:
      ili9341_writedata8(0x40|0x08);
      ILI9341_SIZE_X = 240;
      ILI9341_SIZE_Y = 320;
      break;
    case 1:
      ili9341_writedata8(0x20|0x08);
      ILI9341_SIZE_X = 320;
      ILI9341_SIZE_Y = 240;
      break;
    case 2:
      ili9341_writedata8(0x80|0x08);
      ILI9341_SIZE_X = 240;
      ILI9341_SIZE_Y = 320;
      break;
    case 3:
      ili9341_writedata8(0x40|0x80|0x20|0x08);
      ILI9341_SIZE_X = 320;
      ILI9341_SIZE_Y = 240;
      break;
  }
}

uint16_t ili9341_xsize(void)
{
  return ILI9341_SIZE_X;
}

uint16_t ili9341_ysize(void)
{
  return ILI9341_SIZE_Y;
}


/*--------------------------------------------------------------------------------------------------
  Name         :  ili9341_putchar_printf(char var, FILE *stream)
  Description  :  hook-function for printf 
  Argument(s)  :  
  Return value :  
--------------------------------------------------------------------------------------------------*/
int16_t ili9341_putchar_printf(char var, FILE *stream)
{
  ili9341_write(var);
  return 0;
}

/*--------------------------------------------------------------------------------------------------
  Name         :  ili9341_drawchar(int16_t x, int16_t y, unsigned char c,uint16_t color, uint16_t bgcolor, uint8_t size)
  Description  :  draw a font character at a given position
  Argument(s)  :  x, y - position (upper left corner)
                  c    - character to write
                  color
                  bgcolor 
                  size
  Return value :  
--------------------------------------------------------------------------------------------------*/
void ili9341_drawchar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bgcolor, uint8_t size) 
{
  int8_t i, j;
  uint8_t line;
  if ((x >=ILI9341_SIZE_X)      // Clip right
   || (y >=ILI9341_SIZE_Y)      // Clip bottom
   || ((x + 6 * size - 1) < 0)  // Clip left
   || ((y + 8 * size - 1) < 0)) // Clip top
  {
    return;
  }
  for (i=0; i<6; i++)
  {
    {
      line = pgm_read_byte(&smallFont[(uint8_t)c][i]);
    }
    for (j = 0; j<8; j++)
    {
      if (line & 0x1)
      {
        if (size == 1) // default size
        {
          ili9341_drawpixel(x+i, y+j, color);
        }
        else
        {  // big size
          ili9341_fillrect(x+(i*size), y+(j*size), size, size, color);
        }
      }
      else
      {
        if (bgcolor != color)
        {
          if (size == 1) // default size
          {
            ili9341_drawpixel(x+i, y+j, bgcolor);
          }
          else
          {  // big size
            ili9341_fillrect(x+i*size, y+j*size, size, size, bgcolor);
          }
        }
      }
      line >>= 1;
    }
  }
}

/*--------------------------------------------------------------------------------------------------
  Name         :  ili9341_setcursor(uint16_t x,uint16_t y)
  Description  :  set text cursor position
  Argument(s)  :  x, y - position (upper left corner)
  Return value :  
--------------------------------------------------------------------------------------------------*/
void ili9341_setcursor(uint16_t x, uint16_t y)
{
  cursor_x = x;
  cursor_y = y;
}

/*--------------------------------------------------------------------------------------------------
  Name         :  ili9341_settextcolor(uint16_t color, uint16_t bgcolor)
  Description  :  set text color
  Argument(s)  :  color, bgcolor
  Return value :  
--------------------------------------------------------------------------------------------------*/
void ili9341_settextcolor(uint16_t color, uint16_t bgcolor)
{
  textcolor   = color;
  textbgcolor = bgcolor;
}

void ili9341_settextsize(uint8_t s)
{
  if(s<=8)
  {
    textsize = (s>0) ? s: 1; //this operation means if s greater than 0,then s=s,else s=1
  }
}

/*--------------------------------------------------------------------------------------------------
  Name         :  ili9341_write(uint8_t c)
  Description  :  write a character at current position in current color
  Argument(s)  :  c - character to write
  Return value :  
--------------------------------------------------------------------------------------------------*/
//
//  write a character at current coordinates after setting location and color
//
void ili9341_write(uint8_t c)
{
  if (c == '\n')
  {
    cursor_y += textsize*8;
    cursor_x  = 0;
  }
  else
  {
    if (c == '\r')
    {
      cursor_x  = 0;
    }
    else
    {
      ili9341_drawchar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
      cursor_x += textsize*6;
    }
  }
}
