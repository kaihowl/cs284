<?php

require ("SqlFunctions.php");
require ("communication.php");

$number_of_requests = 100;
$mean_interarrival  = 100000.0; // in usec

// 1. empty DB
connect();
dropNodeData();

// 2. generate requests
for ( $i=0 ; $i<1000 ; $i++ )
{
	comm_recv_up(1);	
	usleep(rand ( $mean_interarrival-5, $mean_interarrival+5 ));

}


