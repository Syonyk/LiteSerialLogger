#include "LiteSerialLogger.h"

void setup() {
  // put your setup code here, to run once:
  LiteSerial.begin(9600);

  //LiteSerial.print("Hi!\n");
  LiteSerial.print(F("Hello from PROGMEM (print).\n"));
  LiteSerial.print((char)-7);
  LiteSerial.print(F("\n"));
  LiteSerial.print((byte)243);
  LiteSerial.print(F("\n"));
  LiteSerial.print((int)-2700);
  LiteSerial.print(F("\n"));
  LiteSerial.print((word)50000);
  LiteSerial.print(F("\n"));
  LiteSerial.print((long)-1234567);
  LiteSerial.print(F("\n"));
  LiteSerial.print((unsigned long)4294967295);
  LiteSerial.print(F("\n"));
  LiteSerial.print((float)12345.678);
  LiteSerial.print(F("\n"));

  LiteSerial.print(F("Hello from PROGMEM (print HEX).\n"));
  LiteSerial.print((char)-7, HEX);
  LiteSerial.print(F("\n"));
  LiteSerial.print((byte)243, HEX);
  LiteSerial.print(F("\n"));
  LiteSerial.print((int)-2700, HEX);
  LiteSerial.print(F("\n"));
  LiteSerial.print((word)50000, HEX);
  LiteSerial.print(F("\n"));
  LiteSerial.print((long)-1234567, HEX);
  LiteSerial.print(F("\n"));
  LiteSerial.print((unsigned long)4294967295, HEX);
  LiteSerial.print(F("\n"));


  LiteSerial.println(F("Hello from PROGMEM. (println)"));
  LiteSerial.println((char)-7);
  LiteSerial.println((byte)243);
  LiteSerial.println((int)-2700);
  LiteSerial.println((word)50000);
  LiteSerial.println((long)-1234567);
  LiteSerial.println((unsigned long)4294967295);
  LiteSerial.println((float)12345.678);

  LiteSerial.println(F("Hello from PROGMEM. (println HEX)"));
  LiteSerial.println((char)-7, HEX);
  LiteSerial.println((byte)243, HEX);
  LiteSerial.println((int)-2700, HEX);
  LiteSerial.println((word)50000, HEX);
  LiteSerial.println((long)-1234567, HEX);
  LiteSerial.println((unsigned long)4294967295, HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
