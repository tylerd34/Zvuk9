/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

/*

  u8x8_debounce.c
  
  Key/button simple debounce algorithm (Addon for u8x8)
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/

#include "u8x8.h"

static uint8_t u8x8_read_pin_state(u8x8_t *u8x8)
{
  uint8_t i;
  uint8_t pin_state;
  
  pin_state = 255;	/* be compatible with the setup of the default pin setup, which is 255 */
  for( i = 0; i < U8X8_PIN_INPUT_CNT; i++ )
  {
    pin_state <<= 1;
    
    /* the callback function should put the return value into this variable */
    u8x8->gpio_result = 1;
    u8x8_gpio_call(u8x8, U8X8_MSG_GPIO(i+U8X8_PIN_OUTPUT_CNT), 0);
    pin_state |= u8x8->gpio_result & 1;
  }
  
  return pin_state;
}

/*
  return 0 to U8X8_PIN_INPUT_CNT-1 if there is a difference
  return U8X8_PIN_INPUT_CNT if there is no difference
*/
static uint8_t u8x8_find_first_diff(uint8_t a, uint8_t b)
{
  uint8_t mask;
  uint8_t i;
  mask = 1;
  i = U8X8_PIN_INPUT_CNT;
  do
  {
    i--;
    if ( (a & mask) != (b & mask) )
      return i;
    mask <<= 1;
  } while( i > 0 );
  return U8X8_PIN_INPUT_CNT;
}

/*
  State A:
    u8x8->debounce_last_pin_state == current_state 
      --> State A
    u8x8->debounce_last_pin_state != current_state 
      --> u8x8->debounce_last_pin_state = current_state 
      --> State B + cnt

  State B + cnt
    --> state--

  State B
    u8x8->debounce_last_pin_state == current_state 
      --> keypress detected
      --> State C
    u8x8->debounce_last_pin_state != current_state 
      --> State A

  State C
    u8x8->debounce_last_pin_state == current_state 
      --> State C
    u8x8->debounce_last_pin_state != current_state 
      --> State A

*/

#ifdef __unix__xxxxxx_THIS_IS_DISABLED

#include <stdio.h>
#include <stdlib.h>
uint8_t u8x8_GetMenuEvent(u8x8_t *u8x8)
{
    int c;
    c = getc(stdin);
    switch(c)
    {
        case 'n':
            return  U8X8_MSG_GPIO_MENU_NEXT;
        case 'p':
            return  U8X8_MSG_GPIO_MENU_PREV;
        case 's':
            return  U8X8_MSG_GPIO_MENU_SELECT;
        case 'h':
            return  U8X8_MSG_GPIO_MENU_HOME;
        case 'x':
            exit(0);
        default:
            puts("press n, p, s, h or x");
            break;
    }
    return 0;
}


#else  /* __unix__ */


#define U8X8_DEBOUNCE_WAIT 1
/* do debounce and return a GPIO msg which indicates the event */
/* returns 0, if there is no event */
uint8_t u8x8_GetMenuEvent(u8x8_t *u8x8)
{
  uint8_t pin_state;
  uint8_t result_msg = 0;	/* invalid message */
  
  pin_state = u8x8_read_pin_state(u8x8);
  
  /* States A, B, C & D are encoded in the upper 4 bit*/
  switch(u8x8->debounce_state)
  {
    case 0x00:	/* State A, default state */
      if ( u8x8->debounce_default_pin_state != pin_state )
      {
	u8x8->debounce_last_pin_state = pin_state;
	u8x8->debounce_state = 0x010 + U8X8_DEBOUNCE_WAIT;
      }
      break;
    case 0x10:	/* State B */
      if ( u8x8->debounce_last_pin_state != pin_state )
      {
	u8x8->debounce_state = 0x00;	/* back to state A */
      }
      else
      {
	/* keypress detected */
	u8x8->debounce_last_pin_state = pin_state;
	//result_msg = U8X8_MSG_GPIO_MENU_NEXT;
	u8x8->debounce_state = 0x020;	/* got to state C */	
      }
      break;
    case 0x20:	/* State C */
      /* wait until key release */
      if ( u8x8->debounce_default_pin_state == pin_state )
      {
	u8x8->debounce_state = 0x00;	/* back to state A */
	result_msg = U8X8_MSG_GPIO(u8x8_find_first_diff(u8x8->debounce_default_pin_state, u8x8->debounce_last_pin_state)+U8X8_PIN_OUTPUT_CNT);
      }
      else
      {
	//result_msg = U8X8_MSG_GPIO_MENU_NEXT;
	// maybe implement autorepeat here 
      }
      break;
    default:
      u8x8->debounce_state--;	/* count down, until there is a valid state */
      break;
  }
  return result_msg;
}

#endif /* __unix__ */