#include <Arduino.h>

/**
 * LiteSerialLogger - Lightweight serial logging for Arduino.
 * Copyright (C) 2016  Russell E Graves (Syonyk)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *   USA.
 */

// Format specifiers for conversions.  These are the same as in Print.h
#define HEX 16
#define DEC 10

class LiteSerialLogger {
public:
  // Initialize the serial port.  Call before any output.
  void begin(long baudrate);

  // All the print/println functions return the length written.

  // String handlers.  Print the characters.
  // __FlashStringHelper is a helpful wrapper for a char* in progmem.  You can
  // use F() when passing things to this to store the data in PROGMEM, and this
  // works properly.
  int print(const __FlashStringHelper* message);
  int print(const char *message);

  // 8 bit types, printed in specified format, with no newline appended.
  // This prints the decimal or hexadecimal value of the byte.
  // If you want to print a char as a character, use write().
  int print(const char &value, const byte base = DEC);
  int print(const byte &value, const byte base = DEC);

  // 16 bit types, printed in specified format.
  int print(const int &value, const byte base = DEC);
  int print(const word &value, const byte base = DEC);
  
  // 32-bit types, printed in specified format.
  int print(const long &value, const byte base = DEC);
  int print(const unsigned long &value, const byte base = DEC);
  int print(const float &value);

  // Same stuff, with a newline.
  int println(const __FlashStringHelper* message);
  int println(const char *message);
  int println(const char &value, const byte base = DEC);
  int println(const byte &value, const byte base = DEC);
  int println(const int &value, const byte base = DEC);
  int println(const word &value, const byte base = DEC);
  int println(const long &value, const byte base = DEC);
  int println(const unsigned long &value, const byte format = DEC);
  int println(const float &value);

  // Push a byte out onto the port as-is.
  int write(const char &value);

private:
  // Synchronous put of a character to the port.
  void put_char(const char c);
};

extern LiteSerialLogger LiteSerial;