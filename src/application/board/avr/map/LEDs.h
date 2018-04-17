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

#pragma once

///
/// \brief LED indexes.
/// \ingroup boardCommonMapAvr
/// @{

#define LED_NOTE_C_SHARP            12
#define LED_NOTE_D_SHARP            11
#define LED_NOTE_F_SHARP            10
#define LED_NOTE_G_SHARP            9
#define LED_NOTE_A_SHARP            8

#define LED_NOTE_C                  22
#define LED_NOTE_D                  21
#define LED_NOTE_E                  20
#define LED_NOTE_F                  19
#define LED_NOTE_G                  18
#define LED_NOTE_A                  17
#define LED_NOTE_B                  16

#define LED_ON_OFF_NOTES            2
#define LED_ON_OFF_AFTERTOUCH       3
#define LED_ON_OFF_X                5
#define LED_ON_OFF_Y                4
#define LED_ON_OFF_SPLIT            6

#define LED_TRANSPORT_PLAY          15
#define LED_TRANSPORT_STOP          14
#define LED_TRANSPORT_RECORD        13

#define LED_OCTAVE_DOWN             1
#define LED_OCTAVE_UP               0

/// @}