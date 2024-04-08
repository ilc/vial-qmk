/*
Copyright 2023 Morgan Venable @_claussen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* key matrix size */
// Rows are doubled-up



//PS2 Trackpoint Setup
#ifdef PS2_MOUSE_ENABLE
  //#define SERIAL_PIO_USE_PIO1
  #define PS2_PIO_USE_PIO1
  #define PS2_RESET_PIN GP25
  #define PS2_CLOCK_PIN GP24
  #define PS2_DATA_PIN GP23
  #define PS2_MOUSE_SCROLL_DIVISOR_H 4
  #define PS2_MOUSE_SCROLL_DIVISOR_V 4
  #ifndef PS2_MOUSE_ROTATE
  #define PS2_MOUSE_ROTATE 90 
  //default is N exit on right hand --270 for N on left see bsa.sh
  #endif
//  #define PS2_MOUSE_SCROLL_BTN_MASK 0  // just normal middle button for panning and relative scroll
  #define PS2_MOUSE_SCROLL_BTN_MASK (1<<PS2_MOUSE_BTN_MIDDLE) /* Default -- tap to enter relative scroll, hold to use direct scroll*/ 
#endif

#define SPLIT_POINTING_ENABLE
#define POINTING_DEVICE_COMBINED
#define POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE