<?php

////////  Function to Connect to MySql Database ////////////////////////////////////////////////////////////

function connect() {
               require_once 'dbconfig.php';
               $mydbh=mysql_connect($ip, $username, $password)or die("Could not connect: ".mysql_error());
               $conn=mysql_select_db($database,$mydbh) or die(mysql_error());

               //Connect to database
               return $conn;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////  Function to fetch NodeId by FloorNumber,MacAddress,NodeName //////////////////////////////////////////////////////

function getAllNodeIds(){

 $sql="select * from Nodes where 1";
 $result=mysql_query($sql);
 return $result;
}


function setNodeData($nodeId,$queueLen,$responseTime){

  $phpDate=date( 'Y-m-d H:i:s',time());
  $sql="insert into NodeData (`NodeId`,`QueueLength`,`Timestamp`,`ResponseTime`) values('$nodeId','$queueLen','$phpDate','$responseTime')";
  $result=mysql_query($sql) or die(mysql_error());
}


function getNodeIdsByFloorNumber($floorNumber){

 $sql="select NodeId from Nodes where FloorNumber='".$floorNumber."'";
 $result=mysql_query($sql);
 return $result;
}

function getMacAddrByNodeId($nodeId){

 $sql="select NodeMacAddr from Nodes where NodeId='".$nodeId."'";
 $result=mysql_query($sql);
 return $result;
}

function getNodeIdByMacAddr($macAddr){

 $sql="select NodeId from Nodes where NodeMacAddr='".$macAddr."'";
 $result=mysql_query($sql);
 return $result;
}

function getNodeIdByNodeName($nodeName){

 $sql="select NodeId from Nodes where NodeName='".$nodeName."'";
 $result=mysql_query($sql);
 return $result;
}

function getQueueLengthByNodeId($queueLen){

 $sql="select NodeId from NodeData where QueueLength='".$queueLen."'";
 $result=mysql_query($sql);
 return $result;
}

function getNodeDataByNodeId($nodeId){

 $sql="select NodeId,QueueLength,Timestamp,ResponseTime from NodeData where NodeId='".$nodeId."' order by Timestamp desc";
 $result=mysql_query($sql);
 return $result;
}

function getNodeAttrByNodeId($nodeId){

 $sql="select Attribute from NodeAttr where NodeId='".$nodeId."'";
 $result=mysql_query($sql);
 return $result;
}

function getAvgResponseTime($nodeId){
 $sql="select avg(ResponseTime), variance(ResponseTime), max(ResponseTime), min(ResponseTime) from NodeData where NodeId='".$nodeId."'";
 $result=mysql_query($sql);
 $row=mysql_fetch_array($result);
 return $row;

}

function getNumDatapoints($nodeId){
  $sql="select ResponseTime from NodeData where NodeId='".$nodeId."'";
 $result=mysql_query($sql);
 $num=mysql_num_rows($result);
 return $num;
}

function dropNodeData($nodeId){
 $sql="TRUNCATE TABLE NodeData";
 $result=mysql_query($sql);
}

?>
