# Lite Serial Logger
### By Syonyk

# What Is it?
Lite Serial Logger is an Arduino serial logging class that uses *zero* global SRAM for logging.  It makes small stack allocations when running, and frees them immediately, never using any global memory on an ongoing basis.

This should save you about 177 bytes of your precious 2048 bytes of SRAM (for an Arduino Uno) if all you're doing is serial logging!

# How to Use
Import the library.  You can download a zip file of it above and import it in the Arduino IDE with Sketch -> Include Library -> Add Zip Library.

If you'd like to see the example, it should be in File -> Examples -> LiteSerialLogger -> LiteSerialLogger, but fundamentally, it works exactly the same as using the Serial library for logging.

The library creates a (memory-use-free) LiteSerial object you can use as a drop in replacement for the Serial library print/println/etc functions.

```
LiteSerial.begin(9600);

LiteSerial.print("Hi!\n");
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
LiteSerial.write('c');
LiteSerial.print(F("\n"));

// If you want hex output, try this!
LiteSerial.println(F("Hello from PROGMEM. (println HEX)"));
LiteSerial.println((char)-7, HEX);
LiteSerial.println((byte)243, HEX);
LiteSerial.println((int)-2700, HEX);
LiteSerial.println((word)50000, HEX);
LiteSerial.println((long)-1234567, HEX);
LiteSerial.println((unsigned long)4294967295, HEX);
```

Call begin() with your desired baud rate (it works the same as the Serial.begin() function), and go on your way, saving much memory!

# How Does it Work?
The main difference between the stock serial library and my library is that I designed mine to do one thing, and one thing only: Write to the serial port.  It cannot receive data.  It does not use the ring buffer and interrupt mechanism that the stock library uses (which means it's somewhat slower in returning to program execution if you're writing a lot of data).  It simply sits and spins on the "is ready to transmit" bit until the port is ready, then it writes a byte in, and waits again.  Simple, quick, low memory use.

The actual conversion of data from values to strings is done with the standard C itoa/utoa/ltoa/ftoa/etc functions - convert a value of that type into an ASCII representation on the given base.  This is done into a stack variable, sized for the data size being converted (see the code for details).  It's very memory efficient, and only uses memory when actually being called.

Finally, because Arduino uses an optimizing compiler, only the functions that are used are included.  You'll notice a ~1000 byte difference in program memory use if you try to print a float value, because that requires adding in a bunch more code.  And, really, why are you using a float variable in an Arduino sketch anyway?

If you have other questions about the behavior, the code should be reasonably well documented.  Failing that, file a bug!

Hopefully this is useful to you!