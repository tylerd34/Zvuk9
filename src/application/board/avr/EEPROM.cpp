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

#include <avr/eeprom.h>
#include "../Board.h"

bool Board::memoryRead(uint32_t address, sectionParameterType_t type, int32_t &value)
{
    switch(type)
    {
        case BIT_PARAMETER:
        case BYTE_PARAMETER:
        case HALFBYTE_PARAMETER:
        value = eeprom_read_byte((uint8_t*)address);
        break;

        case WORD_PARAMETER:
        value = eeprom_read_word((uint16_t*)address);
        break;

        default:
        // case DWORD_PARAMETER:
        value = eeprom_read_dword((uint32_t*)address);
        break;
    }

    return true;
}

bool Board::memoryWrite(uint32_t address, int32_t value, sectionParameterType_t type)
{
    switch(type)
    {
        case BIT_PARAMETER:
        case BYTE_PARAMETER:
        case HALFBYTE_PARAMETER:
        eeprom_update_byte((uint8_t*)address, value);
        break;

        case WORD_PARAMETER:
        eeprom_update_word((uint16_t*)address, value);
        break;

        default:
        // case DWORD_PARAMETER:
        eeprom_update_dword((uint32_t*)address, value);
        break;
    }

    return true;
}