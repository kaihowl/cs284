<?php
require_once("SqlFunctions.php");
connect();
?>


<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Cloud Hosting</title>

<link rel="stylesheet" type="text/css" href="demo1.css" />


</head>

<body>
<?php

$result =getAllNodeIds();
$num_rows=mysql_num_rows($result);
$row=mysql_fetch_array($result);

    echo "<table id='table1' >";
      echo"<caption>Status</caption>";
      echo "<tr  valign='top'><td class='header'><b>Node Id</b></td><td class='header'><b>Node Name</b> </td><td class='header'><b>Node MacAddress</b></td><td class='header'><b>Floor Number</b></td></tr>";
      for($i=0;$i<$num_rows;$i++)
      {
       if($i%2){
       echo "<tr class='even'>";
       }else{
       echo "<tr class='odd'>";
       }
        echo "<td class='data'><a href=?id=".$row['NodeId'].">".$row['NodeId']."</a></td><td class='data'>".$row['NodeName']."</td><td class='data'>".$row['NodeMacAddr']."</td><td class='data'>".$row['FloorNumber']."</td></tr>";
$row=mysql_fetch_array($result);
      }
       echo "</table>";

if(isset($_GET['id']))
{
 $node=$_GET['id'];
 $result=getNodeDataByNodeId($node);
 $num_rows=mysql_num_rows($result);
 $num_rows=($num_rows>10)?10:$num_rows;
 if($num_rows!=0)
 {
  $row=mysql_fetch_array($result);
   $responseTime=getAvgResponseTime($node);

   echo "<br><br><table id='table2' >"; 
   echo "<tr ><td class='header' colspan='2' align='center'> Response Time Summary Report for Node: ".$node."</td></tr>";
   echo "<tr class='even' ><td> Maximum Response Time </td><td>".floatval($responseTime[2])." sec</td></tr>";
   echo "<tr class='odd'><td> Minimum Response Time </td><td>".$responseTime[3]." sec</td></tr>";
   echo "<tr class='even'><td> Average Response Time </td><td>".$responseTime[0]." sec</td></tr>";
   echo "<tr class='odd'><td> Variance </td><td>".$responseTime[1]."</td></tr>";
   echo "</table><br><br>";
    echo "<table id='table2' >";
    
   echo "<tr ><td class='header' colspan='3' align='center'> Response Time for Node: ".$node."</td></tr>";
    #echo"<caption>Status of : ".$row['NodeId'].".  Average Response Time: ".$responseTime[0]." seconds and variance= ".$responseTime[1]." Max= ".$responseTime[2]." Min= ".$responseTime[3]."</caption>";
    echo "<tr  valign='top'><td class='header'><b>Queue Length</b> </td><td class='header'><b>Timestamp</b></td><td class='header'><b>Response Time(sec)</b></td></tr>";
    
   for($i=0;$i<$num_rows;$i++)
      {
        if($i%2){
         echo "<tr class='even'>";
        }else{
         echo "<tr class='odd'>";
        }
         
        echo "<td class='data'>".$row['QueueLength']."</td><td class='data'>".$row['Timestamp']."</td><td class='data'>".$row['ResponseTime']."</td></tr>";
        echo "</tr>";
         $row=mysql_fetch_array($result);

      }
     echo "</table>";

 }else{
  echo "<div id='info'>No data present for Node $node</div>";
 }
}

?>
</body>
</html>
