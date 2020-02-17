/* 
 * Interface for Graphic Display DG-16080 160x80 with Controller LC7981 
 * 
 * Robert Merwa
 *
 * font5x7.h header file comes from Procyon AVRlib.
 */
#include "mylcd.h"

#ifndef GRAPHICS_H

#define GRAPHICS_H

// Draw Horizintal Line (x_N, y_N: Start Position; Length: length in Pixel; Color=1/0 (BLACK/WHITE))
void g_HLine(uint8_t x_N, uint8_t y_N, uint8_t Length, uint8_t Color);

// Draw Vertical Line (x_N, y_N: x/y-Start Position; Length: length in Pixel; Color=1/0 (BLACK/WHITE))
void g_VLine(uint8_t x_N, uint8_t y_N, uint8_t Length, uint8_t Color);

// Draw Rectangle (x_N, y_N: x/y-Start Position; Width: width in Pixel; Height: height in Pixel; Color=1/0 (BLACK/WHITE))
void g_Rect(uint8_t x_N, uint8_t y_N, uint8_t Width, uint8_t Height, uint8_t Color);

// Draw linear Interpolation between point x1/y1 and point x2/y2; Color=1/0 (BLACK/WHITE)
void g_Interp1(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t Color);

// Draw Circle (x_N/y_N: Center; Radius: radius in pixel; PhiStart: Start angle; PhiEnd: End angle; RatioWH: RatioWidth/Height of the Display; Color=1/0 (BLACK/WHITE))
void g_Circle(uint8_t x_N, uint8_t y_N, uint8_t Radius, double PhiStart, double PhiEnd, double dPhi, double RatioWH, uint8_t Color);

// Draw Koordinate System (x_N/y_N: Center; Length: Length of the half axis in Pixel; RatioWH: RatioWidth/Height of the Display; Color=1/0 (BLACK/WHITE))
void g_CartCoord(uint8_t x_N, uint8_t y_N, uint8_t Length, double RatioWH, uint8_t Color);

// Draw Vector (x_N/y_N: Center; Length: Length of the vector in Pixel; Phi: angle of the vector; RatioWH: RatioWidth/Height of the Display; Color=1/0 (BLACK/WHITE))
void g_Vector(uint8_t x_N, uint8_t y_N, uint8_t Length, double Phi, double RatioWH, uint8_t Color);


#endif


//Standard-Functions

// lcd_init();  								// Initializing Display
// lcd_clear(); 								// Clear Display

// lcd_set_pixel(x,y,Color);					// Draw Pixel at Position x,y  Color=1/0 (BLACK/WHITE)
// lcd_invert();								// Inverting Display

// lcd_set_cursor(0,0);							// Set Cursor to x,y-Position
// lcd_puts_p(font5x8,PSTR("Text from Flash"));	// Write Text (from flash memory) to x,y-Position

// lcd_putc(font5x8,'1');                		// Write single Character to x,y-Position

// lcd_puts(font5x8,"String using RAM");  		// Write Text to x,y-Position
