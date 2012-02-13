<?php

include ("php-serial-read-only/php_serial.class.php");

$serial = new phpSerial();
 
$serial->deviceSet("/dev/ttyUSB0");

$serial->deviceOpen();

$serial->sendMessage("G;");

usleep(1000);

while (1==1)
{
	while (($read = $serial->readPort()) == ""  );


	echo $read;
}
$serial->deviceClose();



?>
