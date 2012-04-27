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



while (1) { 
    $line = <>;
    if($line =~ /PublishOrRemoveState: \"availability - 3500/) 
    {
        print ("LINE: $_\n");
        system("/usr/local/bin/delcom-stoplight", "green");
		sleep 1;
    }
    
    /PublishOrRemoveState: \"availability -/ and print ("LINE: $_\n");
}

# line looks like: 	04/26/2012|16:16:14.998 143F:AC5642C0 INFO  :: PublishOrRemoveState: "availability - 6500, starttime - 0.000000, uri - , manual state - 1" "custom id - 0, activity count - 0"
# values are:
# 3500 - Available
# 6500 - Busy
# 9500 - DND
# 12500 - BRB
# 15500 - Off Work or Away
# 