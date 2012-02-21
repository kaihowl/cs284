<?php

include ("php-serial-read-only/php_serial.class.php");
include ("SqlFunctions.php");

connect();

$serial = new phpSerial();
 
$serial->deviceSet("/dev/ttyUSB0");

$serial->deviceOpen();

$serial->sendMessage("G1234567890;");

//usleep(1000);

//$serial->serialFlush();

while (1==1)
{
	$read="";
	$return="";
	while ($read!="\n"){
		//echo "B";
		while (($read = $serial->readPort()) == ""  ){
			usleep(10);
		}

		echo $read.'\n';
		$return .= $read;
	}

	echo "return is: ".$return."\n";

	$value = "XX";
	$value[0] = $return[0];
	$value[1] = $return[1];

	$ts = substr ($return, 3, strlen($return)-6);

        list($usec, $sec) = explode(" ", microtime());
        $tsn = (string) ((float)$usec + (float)$sec);

	echo "ts: $ts : ".((float) $ts)."\n";
	echo "nts: $tsn \n";
	$rtt = (float) $tsn - (float) $ts;

	echo "RTT: ".$rtt."\n\n\n";
	if($rtt < 1000.0 & $rtt > 0.0)
		setNodeData(1,(int) $value,$rtt);
	

}
$serial->deviceClose();



?>
