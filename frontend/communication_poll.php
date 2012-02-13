<?php

include ("php-serial-read-only/php_serial.class.php");
include ("SqlFunctions.php");

$serial = new phpSerial();
 
$serial->deviceSet("/dev/ttyUSB0");

$serial->deviceOpen();

$serial->sendMessage("G1234567890;");

usleep(1000);

$serial->serialFlush();

while (1==1)
{
	while (($read = $serial->readPort()) == ""  );
	echo "AAA".$read;

	$value = "XX";
	$value[0] = $read[0];
	$value[1] = $read[1];

	$ts = substr ($read, 2, strlen($read));

        list($usec, $sec) = explode(" ", microtime());
        $tsn = (string) ((float)$usec + (float)$sec);


	$rtt = (float) $tsn - (float) $ts;

	setNodeData(1,(int) $value,$rtt);
	

}
$serial->deviceClose();



?>
