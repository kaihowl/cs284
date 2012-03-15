<?php

function getExpRand($mean)
{
	$temp = `echo "exprnd ($mean)" | octave | grep ans`;
	list ($gar1, $value) = split ("=", $temp);
	return (float) $value;

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

function my_checksum($packet){

        $sum = 0;
        for($i=3;$i<strlen($packet);$i++)
        {
                $sum += ord($packet[$i]);
        }

        $sum = $sum & 0xFF;

        return 0xFF-$sum;
}



?>
