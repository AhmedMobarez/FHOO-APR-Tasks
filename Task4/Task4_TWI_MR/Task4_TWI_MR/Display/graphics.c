#include <math.h>
#include "graphics.h"

void g_HLine(uint8_t x, uint8_t y, uint8_t length,uint8_t Color)
{
  uint8_t i;
  for (i = x; i < x+length; i++)
	lcd_set_pixel(i, y, Color);
}

void g_VLine(uint8_t x, uint8_t y, uint8_t length,uint8_t Color)
{
  uint8_t i;
  for (i = y; i < y+length; i++)
	lcd_set_pixel(x, i, Color);

}

void g_Rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height,uint8_t Color)
{
  g_HLine(x, y, width,Color);
  g_VLine(x, y, height,Color);
  g_VLine(x+width-1, y, height-1,Color);
  g_HLine(x+1, y+height-1, width-1,Color);
}

void g_Interp1(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t Color)
{
  float k,d;
  uint8_t x,y;
  
  if (x1==x2)
    if (y2>y1)
      for (y=y1;y<=y2;y++)
		lcd_set_pixel(x1, y, Color);
    else
      for (y=y1;y>=y2;y--)
		lcd_set_pixel(x1, y, Color);
  else
  {
    k=(float) (y2-y1)/(x2-x1);
	d=(float) y1-k*x1;


	if (fabs((int) y2-y1)<=fabs((int) x2-x1))
    {
      if (x1<x2)
        for (x=x1;x<=x2;x++)
        {
          y=round(k*x+d);
		  lcd_set_pixel(x, y, Color);
        }
      else
	  {
        for (x=x1;x>=x2;x--)
        {
          y=round(k*x+d);
		  lcd_set_pixel(x, y, Color);
        }
      }
    }
	else
    {
      if (y1<y2)
        for (y=y1;y<=y2;y++)
        {
          x=round((y-d)/k);
		  lcd_set_pixel(x, y, Color);
        }
      else
	  {
        for (y=y1;y>=y2;y--)
        {
          x=round((y-d)/k);
		  lcd_set_pixel(x, y, Color);
        }
      }
    }
  }
}


void g_Circle(uint8_t x_N,uint8_t y_N,uint8_t Radius, double PhiStart, double PhiEnd, double dPhi, double RatioWH, uint8_t Color)
{
  for (double Phi=PhiStart; Phi<=PhiEnd;Phi=Phi+dPhi)
  {
  //  lcd_set_pixel(x_N+(uint8_t)round(Radius*cos(Phi/180.0*M_PI)),y_N+(uint8_t)round((Radius*sin(Phi/180.0*M_PI))*RatioWH),Color);
  }
}


void g_CartCoord(uint8_t x_N, uint8_t y_N, uint8_t Length, double RatioWH, uint8_t Color)
{
  g_HLine(x_N-Length,y_N,2*Length, Color);
  g_VLine(x_N,y_N-round(Length*RatioWH),round(2*Length*RatioWH), Color);
}


void g_Vector(uint8_t x_N,uint8_t y_N,uint8_t Length,double Phi,double RatioWH, uint8_t Color)
{
  //g_Interp1(x_N,y_N,x_N+Length*cos(Phi/180.0*M_PI),y_N-(Length*sin(Phi/180.0*M_PI)*RatioWH),Color);
}


