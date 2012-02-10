<? 

/*
This function should be called from the query processor.
It specify the node_id that is needed for a sensor read.
*/
function comm_recv_up ($nodeId)
{
	$packet = prepare_packet($nodeId);
	transmit ($packet);
	add_to_waiting_queue($nodeId);

	return 1;
}

function prepare_packet($nodeId)
{
	// XXX prepare packet
	$packet = "G;"
	return $packet;
}

function transmit($packet)
{
	// send through serial port

	return 1;
}

function add_to_waiting_queue($nodeId)
{
	// add to waiting queue
}

bool comm_recv_down ($packet)
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