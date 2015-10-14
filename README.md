delcom-stoplight-mac
====================

C++ USB HID code to drive a Delcom Gen2 three color indicator light on Mac OS X.

The C++ code produces an executable that will turn on and off each one of thethree lights. Data structures and explaination of the communcation to the device are inluded in the code.


A perl script is also included that will monitor Lync for Mac and update the light color based on the status of the user. Diagnostic logging must be enabled in Lync for Mac for this to work. The script requires the File::Tail module be installed on your system (e.g. cpan -e 'install File::Tail'). 

Usage
=====

The binary program takes a single command line argument with the english color of the light (red, yellow, green) or the keyword off. other arguments will produce an error message and not alter the state of the device.

The program also provides information about the delcom device that was detected and uses the first one found on the USB bus.  

The perl script, when run, reads the log file and interprets the numeric status then sets the correct LED. The script will run until interrupted. I suggest setting this up as a startup item alongside Lync. 

Output looks like the following:

setting Red LED...
Availability is 6500
Device Found
  type: 0fc5 b080
  path: USB_0fc5_b080_0x7fdcc1408100
  serial_number: 
  Manufacturer: Delcom Products Inc.
  Product:      USB IO Controller 
  Release:      1f

Installation
============
It is suggested to install the binary, the script, and the man page in the /usr/local tree.

Ackoledgements
==============
This code makes use of Alan Ott's HIDAPI - Multi-Platform library for communication with HID devices.
HIDAPI's public source code repository is located at: http://github.com/signal11/hidapi 

Details of the status codes were found on MSDN at rom http://msdn.microsoft.com/en-us/library/hh380072.aspx. More details are provided in the perl script comments.

The details of communcating with the Delcom device can be found in the USBIOHID datasheet at http://www.delcomproducts.com/downloads/usbiohid.pdf

