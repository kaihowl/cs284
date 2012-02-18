<?php

require("communication.php");

//$packet = chr(hexdec("7e")).chr(hexdec("00")).chr(hexdec("04")).chr(hexdec("08")).chr(hexdec("52")).chr(hexdec("4e")).chr(hexdec("4a")).chr(hexdec("0d"));

$packet = chr(hexdec("7e")).
	chr(hexdec("00")).chr(hexdec("16")).
	chr(hexdec("10")).
	chr(hexdec("01")).
	chr(hexdec("00")).chr(hexdec("13")).chr(hexdec("A2")).chr(hexdec("00")).chr(hexdec(40)).chr(hexdec("81")).chr(hexdec("3d")).chr(hexdec("35")).
	chr(hexdec("FF")).chr(hexdec("FE")).
	chr(hexdec("00")).
	chr(hexdec("00")).
	"G111111;".
	//chr(hexdec("54")).chr(hexdec("78")).chr(hexdec("44")).chr(hexdec("61")).chr(hexdec("74")).chr(hexdec("61")).chr(hexdec("30")).chr(hexdec("41")).
	chr(hexdec("61"));

echo $packet;

transmit($packet);


?>

