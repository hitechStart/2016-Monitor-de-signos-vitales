# MicroTone: A high-performance library with super tiny flash usage to create a pulse train without any CPU usage




<h2>Install</h2>

The library can be installed **manually** (by unzipping and copying the files to the "libraries" directory) or using the **Arduino Library installer**, there is nothing to care about.

<h2>Supported Platforms</h2>

this library was designed for attiny13A/L and Atmega8/A MCU

<h2>functions</h2>

This function initializes the library
MicroTone.begin();

you can also use the begin function to setting the prescaler (default = 3)
MicroTone.begin();//1-5
___________________________________

This function creates a pulse train on the OCR0A pin
note that the parameter (like any write() function) MUST be an 8-bit value (0-255) lower value gives highier frequency

MicroTone.write(value);
___________________________________

This function stops the pulse train (like noTone() function)

MicroTone.stop();

____________________________________

This function changes the prescaler anytime you want!
the given value must be from 1 to 5 for Attiny13 and 1 to 7 for Atmega8


MicroTone.setPrescaler();
