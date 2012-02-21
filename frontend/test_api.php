<?php

require("communication.php");

$packet = assemble_packet("0013a20040813d35", "G1111;", "01");
echo strToHex($packet);
transmit($packet);

function assemble_packet($destination, $payload, $frameid="01")
{
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

function my_checksum($packet){

	$sum = 0;
	for($i=3;$i<strlen($packet);$i++)
	{
		$sum += ord($packet[$i]);
	}

	$sum = $sum & 0xFF;

	return 0xFF-$sum;
}

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

