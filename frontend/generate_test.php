<?php

require ("SqlFunctions.php");
require ("communication.php");

$number_of_requests = 300;
$mean_interarrival  = 100000.0; // in usec

// 1. empty DB
connect();
dropNodeData();

// 2. generate requests
for ( $i=0 ; $i<$number_of_requests ; $i++ )
{
	echo $i.".";
	comm_recv_up(rand(1, 2));	
	usleep(rand ( $mean_interarrival, $mean_interarrival+700000 ));

}


