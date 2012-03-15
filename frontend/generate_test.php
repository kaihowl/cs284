<?php

require ("SqlFunctions.php");
require ("communication.php");

$number_of_requests = $argv[1];
$mean_interarrival  = (float) $argv[2]; // in msec
$number_of_nodes    = $argv[3];

if ($argc != 4)
{
	echo "USAGE: > php generate-test.php number_of_requests mean_interarrival number_of_nodes\n";
	exit();
}

// 1. empty DB
connect();
dropNodeData();

// 2. generate requests
for ( $i=0 ; $i<$number_of_requests ; $i++ )
{
	echo $i.".";
	comm_recv_up(rand(1, $number_of_nodes));	
	usleep( 1000.0*getExpRand($mean_interarrival) );

}

?>
