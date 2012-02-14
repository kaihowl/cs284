-- phpMyAdmin SQL Dump
-- version 3.4.5deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Feb 09, 2012 at 04:16 PM
-- Server version: 5.1.58
-- PHP Version: 5.3.6-13ubuntu3.2

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `NodeDB`
--

-- --------------------------------------------------------

--
-- Table structure for table `NodeAttr`
--

CREATE TABLE IF NOT EXISTS `NodeAttr` (
  `NodeId` bigint(128) NOT NULL,
  `Attribute` varchar(100) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `NodeData`
--

CREATE TABLE IF NOT EXISTS `NodeData` (
  `NodeId` bigint(128) NOT NULL,
  `QueueLength` bigint(10) NOT NULL,
  `Timestamp` datetime NOT NULL,
  `ResponseTime` float(8) NOT NULL,
  KEY `NodeId` (`NodeId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Nodes`
--

CREATE TABLE IF NOT EXISTS `Nodes` (
  `NodeId` bigint(64) NOT NULL,
  `NodeName` varchar(128) NOT NULL,
  `NodeMacAddr` varchar(128) NOT NULL,
  `FloorNumber` varchar(128) NOT NULL,
  PRIMARY KEY (`NodeId`),
  KEY `FloorNumber` (`FloorNumber`),
  KEY `NodeName` (`NodeName`),
  KEY `NodeMacAddr` (`NodeMacAddr`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
