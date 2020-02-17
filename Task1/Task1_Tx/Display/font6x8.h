 /* Created by Robert Merwa
 * File Name           : font6x8.h
 * Date                : 06.12.2010
 * Font size in bytes  : 671
 * Font width          : 5
 * Font height         : 8
 * Font first char     : 32
 * Font last char      : 128
 * Font used chars     : 95
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */
//#include <avr/pgmspace.h>

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef FONT6X8_H
#define FONT6X8_H

#define FONT6X8_WIDTH 6
#define FONT6X8_HEIGHT 8

const uint8_t font6x8[];

#endif
