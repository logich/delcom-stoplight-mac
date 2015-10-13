#!/usr/bin/perl

#  Lync-Availability-Monitor.pl
#  delcom-stoplight
#
#   Enable logging in Lync
#   then point this file at that log
#   default location is /Library/Logs/Microsoft-Lync-0.log
#
#  Created by Logan Browne on 4/26/12.
#  Copyright (c) 2012, All rights reserved.

# from http://stackoverflow.com/questions/8070280/perl-reading-from-a-tail-f-pipe-via-stdin
# example working line is 
#     perl -e 'open($h,$ARGV[0]); while (1) { /IPaddress/ and print "LINE: $_" for <$h>; sleep 1 }' snmplistener.log

# line looks like: 	04/26/2012|16:16:14.998 143F:AC5642C0 INFO  :: PublishOrRemoveState: "availability - 6500, starttime - 0.000000, uri - , manual state - 1" "custom id - 0, activity count - 0"

# From http://msdn.microsoft.com/en-us/library/hh380072.aspx
# Enhanced Presence values are:
# 3500 - Available
# 6500 - Busy
# 9500 - Do Not Disturb
# 12500 - Be Right Back
# 15500 - Away
# 18500 - Offline

# however Lync sets ranges
# http://msdn.microsoft.com/en-us/library/hh379878.aspx
# Lync 2010 interprets the received availability number as follows:
#
#Availability number range	Availability mode	Description
#0-2999				Undefined		The contact is not found in the network.
#3000-4499			Available		The contact is willing and able to communicate.
#4500-5999			Available - Idle	The contact is willing but may be unable to communicate.
#6000-7499			Busy			The contact is able but may be unwilling to communicate.
#7500-8999			Busy - Idle		The contact is able but may be unwilling to communicate
#9000-11999			Do Not Disturb		The contact is able but unwilling to communicate.
#12000-14999			Be Right Back		The contact is willing but unable to communicate.
#15000-17999			Away			The contact is unable to communicate.
#18000 and higher		Offline			The contact is not connected to the network.

use File::Tail;
use Data::Dumper;
use List::Util qw(any);

$file=File::Tail->new("$ENV{HOME}/Library/Logs/Microsoft-Lync-0.log");

@val_avail = (3000..5999);
@val_busy  = (6000..11999);
@val_away  = (12000..17999);

while (defined($line=$file->read)) {


    if($line =~ /PublishOrRemoveState: \"availability/) 
    {
    	chomp($line);
    	@line = split(/\W/,$line);
	print "Availability is $line[20]\n";


	if (@line[20] <= 2999){
	# undefined
            system("/usr/local/bin/delcom-stoplight", "off");
        }

	#smartmatch is experimental in perl 5.18 and does not work
	#if (@line[20] ~~ [3000..5999]){
	#replace it with List::Util any

	if (any {$_ == $line[20]} @val_avail){
	# available 
            system("/usr/local/bin/delcom-stoplight", "green");
        }

	if (any {$_ == $line[20]} @val_busy){
	# busy or dnd
            system("/usr/local/bin/delcom-stoplight", "red");
        }

	if (any {$_ == $line[20]} @val_away){
	# away
            system("/usr/local/bin/delcom-stoplight", "yellow");
        }

	if (@line[20] >= 18000){
	# offline
            system("/usr/local/bin/delcom-stoplight", "off");
        }
    }
    
    
#    /PublishOrRemoveState: \"availability - / and print ("LINE: $_\n");
}

