<?php

require ("SqlFunctions.php");
require ("communication.php");

$number_of_requests = 300;
$mean_interarrival  = 1000000.0; // in usec

// 1. empty DB
connect();
dropNodeData();

// 2. generate requests
for ( $i=0 ; $i<$number_of_requests ; $i++ )
{
	comm_recv_up(1);	
	usleep(rand ( $mean_interarrival-5, $mean_interarrival+5 ));

}


