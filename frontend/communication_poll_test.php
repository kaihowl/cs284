<?php

include ("php-serial-read-only/php_serial.class.php");
include ("SqlFunctions.php");

connect();

$serial = new phpSerial();
 
$serial->deviceSet("/dev/ttyUSB0");

$serial->deviceOpen();

//$serial->sendMessage("G1234567890;");

//usleep(1000);

//$serial->serialFlush();

while (1==1)
{
	$read="";
	$return="";
	//while ($read!="\n"){
		//echo "B";
	back:
	while (($read = $serial->readPort()) != HexToStr("7E")  ){
		usleep(1);
	}
		
	$count = 0;
	$done = false;
	$return = "";
	while($read!=";"){
                //echo "($count)[$read:".strToHex($read)."]";
		while (($read = $serial->readPort()) == "" ){
			usleep(1);
		}
		$count++;
		if ($count==3){
			if ($read!=HexToStr("90")){
				goto back;
			}
		}
		if ($count>=15){
			
			//while(($read = $serial->readPort()) != ""){
				if($read==";"){
					$done = true;
				}
				$return .= $read;
			//}
		}
	
		 
	}	


		//echo "[".strToHex($read)."]";
		//$return .= $read;
	//}

	echo "return is: ".$return."\n";
	echo "is is: ".strToHex($return)."\n";

	$value = "XX";
	$value[0] = $return[0];
	$value[1] = $return[1];

	$ts = substr ($return, 3, strlen($return)-4);

        list($usec, $sec) = explode(" ", microtime());
        $tsn = (string) ((float)$usec + (float)$sec);

	echo "ts: $ts : ".((float) $ts)."\n";
	echo "nts: $tsn \n";
	$rtt = (float) $tsn - (float) $ts;

	echo "RTT: ".$rtt."\n\n\n";
	$nodeID = 1;
	if ($value < 50) $nodeID = 2;
	if($rtt < 10000.0 & $rtt > 0.0)
		setNodeData($nodeID,(int) $value,$rtt);
	

}
$serial->deviceClose();

function hexToStr($hex)
{
    $string='';
    for ($i=0; $i < strlen($hex)-1; $i+=2)
    {
        $string .= chr(hexdec($hex[$i].$hex[$i+1]));

    }

    return $string;
}

function strToHex($string)
{
    $hex='';
    for ($i=0; $i < strlen($string); $i++)
    {
        $thex = dechex(ord($string[$i]));
        if ( strlen($thex) == 1){
                $thex[1] = $thex[0];
                $thex[0] = "0";
        }
        $hex .= $thex;
    }
    return $hex;
}


?>
