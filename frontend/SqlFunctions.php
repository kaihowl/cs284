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

function getNodeIdsByFloorNumber($floorNumber){

 $sql="select NodeId from Nodes where FloorNumber='".$floorNumber."'";
 $result=mysql_query($sql);
 $rows=mysql_fetch_array($result);
 return $rows;
}

function getNodeIdByMacAddr($macAddr){

 $sql="select NodeId from Nodes where NodeMacAddr='".$macAddr."'";
 $result=mysql_query($sql);
 $rows=mysql_fetch_array($result);
 return $rows;
}

function getNodeIdByNodeName($nodeName){

 $sql="select NodeId from Nodes where NodeName='".$nodeName."'";
 $result=mysql_query($sql);
 $rows=mysql_fetch_array($result);
 return $rows;
}

function getQueueLengthByNodeId($queueLen){

 $sql="select NodeId from NodeData where QueueLength='".$queueLen."'";
 $result=mysql_query($sql);
 $rows=mysql_fetch_array($result);
 return $rows;
}

function getNodeDataByNodeId($nodeId){

 $sql="select NodeId,QueueLength,Timestamp,ResponseTime from NodeData where NodeId='".$nodeId."'";
 $result=mysql_query($sql);
 $rows=mysql_fetch_array($result);
 return $rows;
}

function getNodeAttrByNodeId($nodeId){

 $sql="select Attribute from NodeAttr where NodeId='".$nodeId."'";
 $result=mysql_query($sql);
 $rows=mysql_fetch_array($result);
 return $rows;
}

?>
