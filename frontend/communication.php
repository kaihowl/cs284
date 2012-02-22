<?php 

require("util.php");

/*
This function should be called from the query processor.
It specify the node_id that is needed for a sensor read.
*/
function comm_recv_up ($nodeId)
{
	//echo "received a request for: ".$nodeId."\n<br>";
	$packet = prepare_packet($nodeId);
	transmit ($packet);
	add_to_waiting_queue($nodeId);

	return 1;
}

function prepare_packet($nodeId)
{
	// XXX prepare packet

	list($usec, $sec) = explode(" ", microtime());
    	$ts = (string) ((float)$usec + (float)$sec);
	if($ts < 100000000)
		echo "ERROR: timestamp in frontend/communication.php:prepare_packet() is not correct: ".$ts;
	$payload = "G" . $ts . ";";
	$result = getMacAddrByNodeId($nodeId);
	$row = mysql_fetch_array($result);
	$packet = assemble_packet( $row[0], $payload, "66");
	//echo $payload."\n";
	return $packet;
}

function assemble_packet($destination, $payload, $frameid="01")
{
	//echo $destination."]";
        $packet = "";
        $packet .= hexToStr("7e00");
        $length = dechex( 14 + strlen($payload) );
        $packet .= hexToStr($length);
        $packet .= hexToStr("10");
        $packet .= hexToStr($frameid);
        $packet .= hexToStr($destination);
        $packet .= hexToStr("FFFE");
        $packet .= hexToStr("0000");
        $packet .= $payload;
        $packet .= chr(my_checksum($packet));

        return $packet;
}


function transmit($packet)
{
	// send through serial port
	$fp = fopen("/dev/ttyUSB0", "w") or die("unable to open serial"); 
	fwrite ($fp, $packet);

	fclose ($fp);

	return 1;
}

function add_to_waiting_queue($nodeId)
{
	// add to waiting queue

	return 1;
}

function comm_recv_down ($packet)
{
	$info = get_info ($packet);
	
	$nodeId = $info[0];
	$value = $info[1];
	//$delay = calculate XXX
	
	delete_from_waiting_queue ($info);
	qprocc_recv_down ($info);

	return 1;
}


?>
