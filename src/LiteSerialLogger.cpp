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
 * 
 * 
 * Arduino Lightweight serial logger - zero static global SRAM use!
 */

#include <Arduino.h>

// Helper function for setting the serial baud rates.
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

// Store CRLF once for use in println.
static const char crlf[] PROGMEM = "\r\n";

#include "LiteSerialLogger.h"

// Include this to compile in some validation checks on buffer length.
//#define DEBUG_ASSERT

/**
 * This is based on the code from the Arduino HardwareSerial.begin() function.
 * 
 * It does assume 8N1, though that's likely to be the only configuration used
 * for logging.
 */
void LiteSerialLogger::begin(long baudrate) {
  byte config = 0x06; // 8N1

  // Try u2x mode first
  uint16_t baud_setting = (F_CPU / 4 / baudrate - 1) / 2;
  UCSR0A = 1 << U2X0;

  // hardcoded exception for 57600 for compatibility with the bootloader
  // shipped with the Duemilanove and previous boards and the firmware
  // on the 8U2 on the Uno and Mega 2560. Also, The baud_setting cannot
  // be > 4095, so switch back to non-u2x mode if the baud rate is too
  // low.
  if (((F_CPU == 16000000UL) && (baudrate == 57600)) || (baud_setting >4095))
  {
    UCSR0A = 0;
    baud_setting = (F_CPU / 8 / baudrate - 1) / 2;
  }

  // assign the baud_setting, a.k.a. ubrr (USART Baud Rate Register)
  UBRR0H = baud_setting >> 8;
  UBRR0L = baud_setting;

  //set the data bits, parity, and stop bits
#if defined(__AVR_ATmega8__)
  config |= 0x80; // select UCSRC register (shared with UBRRH)
#endif
  UCSR0C = config;
  
  // Class is transmit only - no receive, no interrupts.
  sbi(UCSR0B, TXEN0);
}

/**
 * There are two approaches to put_char: wait then write, or write then wait.
 *
 * I opt to go with wait then write, as this returns flow to the calling code
 * a tiny bit sooner after the last character is pushed into the serial buffer.
 * 
 * As the serial port will likely be empty when this is first called, this order
 * should be a tiny bit faster in operation.
 */
void LiteSerialLogger::put_char(const char c) {
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
}

// Write just pushes a byte onto the port as-is.  Length is always 1.
int LiteSerialLogger::write(const char &value) {
  put_char(value);
  return 1;
}

// __FlashStringHelper is just a class used to indicate a char* stored in
// PROGMEM.  Read it out character-at-a-time and push it to serial.
int LiteSerialLogger::print(const __FlashStringHelper* message) {
  int bytes_written = 0;
  int message_length;

  // Reinterpret the __FlashStringHelper as a char* for manipulation.
  const char *p = reinterpret_cast<const char *>(message);

  // The _P functions operate on strings in program memory.
  message_length = strlen_P(p);

  for (int i = 0; i < message_length; i++) {
    char c = pgm_read_byte_near(p);
    put_char(c);
    p++;
    bytes_written++;
  }

  return bytes_written;
}

// Print a straight up string in SRAM.
int LiteSerialLogger::print(const char *message) {
  int bytes_written = 0;

  const char *p = message;

  // Loop until a null is found.
  while (*p) {
    put_char(*p);
    p++;
    bytes_written++;
  }
  
  return bytes_written;
}

int LiteSerialLogger::print(const char &value, const byte base) {
  // Char is between -128 and 128.  So, 4 characters plus a null.
  // Hex can print ff : 2 + null.
  char buffer[5];
  itoa(value, buffer, base);
#ifdef DEBUG_ASSERT
  if (strlen(buffer) >= sizeof(buffer)) {
      this->println(F("ERROR: String length exceeds buffer size!"));
  }
#endif
  return print(buffer);
}

int LiteSerialLogger::print(const byte &value, const byte base) {
  // Byte is between 0 and 255.  So, 3 characters plus a null.
  // Hex, though - ff - 2 characters plus a null
  char buffer[4];
  utoa(value, buffer, base);
#ifdef DEBUG_ASSERT
  if (strlen(buffer) >= sizeof(buffer)) {
      this->println(F("ERROR: String length exceeds buffer size!"));
  }
#endif
  return print(buffer);
}

int LiteSerialLogger::print(const int &value, const byte base) {
  // Int: -32768 to 32767: 6 plus null.
  // Hex: ffff - 4 plus null.
  char buffer[7];
  itoa(value, buffer, base);
#ifdef DEBUG_ASSERT
  if (strlen(buffer) >= sizeof(buffer)) {
      this->println(F("ERROR: String length exceeds buffer size!"));
  }
#endif
  return print(buffer);
}

int LiteSerialLogger::print(const word &value, const byte base) {
  // Word: 0 to 65535: 5 plus null.
  // Hex: ffff: 4 plus null.
  char buffer[6];
  utoa(value, buffer, base);
#ifdef DEBUG_ASSERT
  if (strlen(buffer) >= sizeof(buffer)) {
      this->println(F("ERROR: String length exceeds buffer size!"));
  }
#endif
  return print(buffer);
}

int LiteSerialLogger::print(const long &value, const byte base) {
  // Long: -2147483648 to 2147483647: 11 plus null 
  // Hex: ffffffff: 8 plus null.
  char buffer[12];
  ltoa(value, buffer, base);
#ifdef DEBUG_ASSERT
  if (strlen(buffer) >= sizeof(buffer)) {
      this->println(F("ERROR: String length exceeds buffer size!"));
  }
#endif
  return print(buffer);
}

int LiteSerialLogger::print(const unsigned long &value, const byte base) {
  // Unsigned Long: 0 to 4294967295: 10 plus null 
  // Hex: ffffffff: 8 plus null.
  char buffer[11];
  ultoa(value, buffer, base);
#ifdef DEBUG_ASSERT
  if (strlen(buffer) >= sizeof(buffer)) {
      this->println(F("ERROR: String length exceeds buffer size!"));
  }
#endif
  return print(buffer);
}

int LiteSerialLogger::print(const float &value) {
  // Float.  You shouldn't be using these.  But I'll print it.
  // Handle... oh, some precision.

  // It will be space padded.  Deal.

  // No, really.  Why are you using a float in an Arduino sketch?
  // Formatting floats is complex, so I'm using an overly large buffer here.
  // Seriously.  Don't use this in your code!
  char buffer[64];
  dtostrf(value, 15, 5, buffer);
#ifdef DEBUG_ASSERT
  if (strlen(buffer) >= sizeof(buffer)) {
      this->println(F("ERROR: String length exceeds buffer size!"));
  }
#endif
  return print(buffer);
}

// The println series: Same thing as above, with a CRLF appended.
// The CRLF in progmem is explicitly cast to a __FlashStringHelper so the proper
// function to extract it from program memory is called.

int LiteSerialLogger::println(const __FlashStringHelper* message) {
  int bytes_written = print(message);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

int LiteSerialLogger::println(const char *message) {
  int bytes_written = print(message);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

int LiteSerialLogger::println(const char &value, const byte base) {
  int bytes_written = print(value, base);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

int LiteSerialLogger::println(const byte &value, const byte base) {
  int bytes_written = print(value, base);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

int LiteSerialLogger::println(const int &value, const byte base) {
  int bytes_written = print(value, base);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

int LiteSerialLogger::println(const word &value, const byte base) {
  int bytes_written = print(value, base);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

int LiteSerialLogger::println(const long &value, const byte base) {
  int bytes_written = print(value, base);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

int LiteSerialLogger::println(const unsigned long &value, const byte base) {
  int bytes_written = print(value, base);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

int LiteSerialLogger::println(const float &value) {
  int bytes_written = print(value);
  bytes_written += print(reinterpret_cast<const __FlashStringHelper*>(crlf));
  return bytes_written;
}

// Instantiation of the class for use.  Note that this does not use memory.
LiteSerialLogger LiteSerial;