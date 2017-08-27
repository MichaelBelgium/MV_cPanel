--
-- Table structure for table `Players`
--

CREATE TABLE IF NOT EXISTS `Players` (
  `Playername` varchar(24) NOT NULL,
  `Password` varchar(512) NOT NULL,
  `Score` int(11) NOT NULL DEFAULT '0',
  `Money` int(11) NOT NULL DEFAULT '0',
  `Adminlevel` tinyint(4) NOT NULL DEFAULT '0',
  `Kills` int(11) NOT NULL DEFAULT '0',
  `Deaths` int(11) NOT NULL DEFAULT '0',
  `Warnings` tinyint(4) NOT NULL DEFAULT '0',
  `OnlineTime` int(11) NOT NULL DEFAULT '0',
  `rIP` varchar(16) NOT NULL COMMENT 'Registered IP',
  `lIP` varchar(16) NOT NULL COMMENT 'Last IP',
  PRIMARY KEY (`Playername`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- --------------------------------------------------------

--
-- Table structure for table `Vips`
--

CREATE TABLE IF NOT EXISTS `Vips` (
  `Name` varchar(24) NOT NULL,
  `Duration` int(11) NOT NULL DEFAULT '0',
  `Toggle0` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Unlimited nitro',
  `Toggle1` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Carlock',
  `Toggle2` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Godcar',
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `Name` (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Constraints for table `Vips`
--
ALTER TABLE `Vips`
  ADD CONSTRAINT `Vips_ibfk_1` FOREIGN KEY (`Name`) REFERENCES `Players` (`Playername`) ON DELETE CASCADE ON UPDATE CASCADE;