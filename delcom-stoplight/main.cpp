//
//  main.cpp
//  delcom-stoplight
//
//  Created by Logan Browne on 4/25/12.
//  Copyright (c) 2012, All rights reserved.
//

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hidapi.h"

int main(int argc, const char * argv[])
{

    int res;
	hid_device *handle;
    
    unsigned char green[16] = {101,12,1,0};
    unsigned char yellow[16] = {101,12,2,0};
    unsigned char red[16] = {101,12,4,0};
    unsigned char green_off[16] = {101,12,0,1};
    unsigned char yellow_off[16] = {101,12,0,2};
    unsigned char red_off[16] = {101,12,0,4};
    
    
	struct hid_device_info *devs, *cur_dev;
    
	
	devs = hid_enumerate(0x0fc5, 0xb080);
	cur_dev = devs;	
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
    
    // Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	//handle = hid_open(0x4d8, 0x3f, NULL);
    // Delcom is 0fc5 b080
	handle = hid_open(0x0fc5, 0xb080, NULL);
	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}
    
    // Toggle LED 
	res = hid_write(handle, red, 16);
	if (res < 0) {
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
	}
    
    return 0;
}

