<?php

require ("SqlFunctions.php");

$numnodes = $argv[1];

connect();

if ($numnodes==1){
	$average = getAvgResponseTime(1);
	$numpoints = getNumDataPoints(1);

	echo $average[0]."\t".$numpoints;
}else if ($numnodes==2){
        $average1 = getAvgResponseTime(1);
        $numpoints1 = getNumDataPoints(1);

        $average2 = getAvgResponseTime(2);
        $numpoints2 = getNumDataPoints(2);

	$average = ($average1[0]+$average2[0])/2.0;
	$numpoints = ($numpoints1+$numpoints2);

	echo $average."\t".$numpoints."\t".$average1[0]."\t".$numpoints1."\t".$average2[0]."\t".$numpoints2;
}else if($numnodes==3){
        $average1 = getAvgResponseTime(1);
        $numpoints1 = getNumDataPoints(1);

        $average2 = getAvgResponseTime(2);
        $numpoints2 = getNumDataPoints(2);

        $average3 = getAvgResponseTime(3);
        $numpoints3 = getNumDataPoints(3);

        $average = ($average1[0]+$average2[0]+$average3[0])/3.0;
        $numpoints = ($numpoints1+$numpoints2+$numpoints3);

        echo $average."\t".$numpoints."\t".$average1[0]."\t".$numpoints1."\t".$average2[0]."\t".$numpoints2."\t".$average3[0]."\t".$numpoints3;;


}




?>



