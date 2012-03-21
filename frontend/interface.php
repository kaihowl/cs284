<?php

require ("SqlFunctions.php");
require ("communication.php");

/**
Supported queries:
1: SELECT temperature FROM $all WHERE floor = [floor number]
*/

$floor = $argv[1];

connect();
$result = getNodeIdsByFloorNumber($floor);

$exist = False;
while ($row = mysql_fetch_assoc($result) ){
	$exist = True;
	$nid = $row['NodeId'];
	echo "Sending to node ".$nid."\n";
	comm_recv_up ( $nid );
}

if (!$exist) 
	echo "no nodes in floor ".$floor."\n";








?>

