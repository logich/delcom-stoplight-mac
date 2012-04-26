//
//  main.cpp
//  delcom-stoplight
//
//  Created by Logan Browne on 4/25/12.
//  Copyright (c) 2012, All rights reserved.
//

// standard libraries
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// local HIDAPI library
#include "hidapi.h"



/* Delcom Gen2 TX comamnd format
 Major Command    1 Byte
 Minor Command    1 Byte  
 Data LSB         1 Byte
 Data MSB         1 Byte
 DataHID[0..3]    4 Bytes
 DataExt[0..7]    8 Bytes (optional)
 
 A major command of 101 will send an 8 byte write command. 
 A major command of 102 will send a 16 byte write command. 
 
 see USBIOHID datasheet at http://www.delcomproducts.com/downloads/usbiohid.pdf
 */

/* LED color settings - format {Major, Minor, LSB, MSB}
 Command 101-12 : 
 Sets or resets the port 1 pins individually. 
 The LSB resets the corresponding port pin(s) and 
 the MSB sets the corresponding port pin(s) on port 1. 
 Resetting the port pin(s) takes precedence over setting.
 
 For example, on model 907241
 Pin 1 is Green
 Pin 2 is Yellow
 Pin 4 is Red
 */

static unsigned char green[16] = {101,12,1,0};
static unsigned char yellow[16] = {101,12,2,0};
static unsigned char red[16] = {101,12,4,0};
static unsigned char green_off[16] = {101,12,0,1};
static unsigned char yellow_off[16] = {101,12,0,2};
static unsigned char red_off[16] = {101,12,0,4};


int main(int argc, const char * argv[])
{
    // handle command line args
    char *cvalue = NULL;
    int index;
    char *command;

    // buffer for TX command to device
    static unsigned char buf[16];
    
    // references to device
    int res;
    hid_device *handle;
	struct hid_device_info *devs, *cur_dev;
	
    // find all devices matching Vendor ID and Product ID
	devs = hid_enumerate(0x0fc5, 0xb080);
	cur_dev = devs;	

    // display all matching devices
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", 
                cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	
    hid_free_enumeration(devs);
    
    
    /* Open the device using the VID, PID,
       and optionally the Serial number.
       
     // handle = hid_open(0x4d8, 0x3f, "12345");
        OR
     // handle = hid_open(0x4d8, 0x3f, NULL);
    
     My Delcom device is 0fc5 b080
	 */
    
    handle = hid_open(0x0fc5, 0xb080, NULL);
	
    if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}

    // Begin parsing the command line options
    if (argc != 2)
		fprintf(stderr, "Usage: %s <color>\n", argv[0]);
	else {
    
        // Toggle LED 
        if (strcmp(argv[1],"green") == 0) {
            // turn others off 
            res = hid_write(handle, red_off, 16);
            res = hid_write(handle, yellow_off, 16);
            //set green
            res = hid_write(handle, green, 16);
            printf("setting Green LED...\n");
        }
        if (strcmp(argv[1],"yellow") == 0) {
            // turn others off 
            res = hid_write(handle, green_off, 16);
            res = hid_write(handle, red_off, 16);
            //set yellow
            res = hid_write(handle, yellow, 16);
            printf("setting Yellow LED...\n");
        }
        if (strcmp(argv[1],"red") == 0) {    
            // turn others off 
            res = hid_write(handle, green_off, 16);
            res = hid_write(handle, yellow_off, 16);
            //set red
            res = hid_write(handle, red, 16);
            printf("setting Red LED...\n");
        }
        if (strcmp(argv[1],"off") == 0) {    
            // all off
            res = hid_write(handle, green_off, 16);
            res = hid_write(handle, yellow_off, 16);
            res = hid_write(handle, red_off, 16);
            printf("setting all LED off...\n");
        }    
        
        if (res < 0) {
            printf("Unable to set LED\n");
            printf("Error: %ls\n", hid_error(handle));
        }
    }         
    return 0;
}
