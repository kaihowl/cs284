<?php

require ("util.php");
require ("SqlFunctions.php");

// $time is Hex
function set_command($nodeId, $command)
{
	connect();

	$packet = $command;
        $result = getMacAddrByNodeId($nodeId);
        $row = mysql_fetch_array($result);
        $packet = assemble_at_packet( $row[0], $packet, "66");

        $fp = fopen("/dev/ttyUSB0", "w") or die("unable to open serial");
        fwrite ($fp, $packet);

        fclose ($fp);

        return 1;

}

function assemble_at_packet($destination, $payload, $frameid="01")
{
        $packet = "";
        $packet .= hexToStr("7e00");
        $length = dechex( 13 + strlen($payload) );
        $packet .= hexToStr($length);
        $packet .= hexToStr("17");
        $packet .= hexToStr($frameid);
        $packet .= hexToStr($destination);
        $packet .= hexToStr("FFFE");
        $packet .= hexToStr("02");
        $packet .= $payload;
        $packet .= chr(my_checksum($packet));

        return $packet;
}

set_command(1, ("ST".hexToStr("66")) );


?>


