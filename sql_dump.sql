--
-- Table structure for table `bans`
--

DROP TABLE IF EXISTS `bans`;
CREATE TABLE IF NOT EXISTS `bans` (
  `bID` int(11) NOT NULL AUTO_INCREMENT,
  `Player` varchar(24) NOT NULL,
  `Admin` varchar(24) DEFAULT NULL,
  `Reason` varchar(32) NOT NULL,
  `IP` varchar(16) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `Duration` int(11) NOT NULL DEFAULT '0',
  `Unbanned` tinyint(1) NOT NULL DEFAULT '0',
  `UnbannedDate` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`bID`),
  KEY `Player` (`Player`,`Admin`),
  KEY `Admin` (`Admin`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `bugs`
--

DROP TABLE IF EXISTS `bugs`;
CREATE TABLE IF NOT EXISTS `bugs` (
  `bID` int(11) NOT NULL AUTO_INCREMENT,
  `Playername` varchar(24) NOT NULL,
  `Bug` text NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`bID`),
  KEY `Playername` (`Playername`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `logs`
--

DROP TABLE IF EXISTS `logs`;
CREATE TABLE IF NOT EXISTS `logs` (
  `lID` int(11) NOT NULL AUTO_INCREMENT,
  `Type` tinyint(4) NOT NULL,
  `Player` varchar(24) DEFAULT NULL,
  `Target` varchar(24) DEFAULT NULL,
  `Command` varchar(32) DEFAULT NULL,
  `Params` varchar(64) DEFAULT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`lID`),
  KEY `Player` (`Player`),
  KEY `Target` (`Target`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `players`
--

DROP TABLE IF EXISTS `players`;
CREATE TABLE IF NOT EXISTS `players` (
  `Playername` varchar(24) NOT NULL,
  `Password` varchar(512) NOT NULL,
  `Score` int(11) NOT NULL DEFAULT '0',
  `Money` int(11) NOT NULL DEFAULT '0',
  `Adminlevel` tinyint(4) NOT NULL DEFAULT '0',
  `Warnings` tinyint(4) NOT NULL DEFAULT '0',
  `Muted` tinyint(4) NOT NULL DEFAULT '0',
  `OnlineTime` int(11) NOT NULL DEFAULT '0',
  `rIP` varchar(16) NOT NULL COMMENT 'Registered IP',
  `lIP` varchar(16) NOT NULL COMMENT 'Last IP',
  PRIMARY KEY (`Playername`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `suggestions`
--

DROP TABLE IF EXISTS `suggestions`;
CREATE TABLE IF NOT EXISTS `suggestions` (
  `sID` int(11) NOT NULL AUTO_INCREMENT,
  `Playername` varchar(24) NOT NULL,
  `Suggestion` text NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`sID`),
  KEY `Playername` (`Playername`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `vips`
--

DROP TABLE IF EXISTS `vips`;
CREATE TABLE IF NOT EXISTS `vips` (
  `Name` varchar(24) NOT NULL,
  `Duration` int(11) NOT NULL DEFAULT '0',
  `Toggle0` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Unlimited nitro',
  `Toggle1` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Carlock',
  `Toggle2` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Godcar',
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `Name` (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `bans`
--
ALTER TABLE `bans`
  ADD CONSTRAINT `Bans_ibfk_1` FOREIGN KEY (`Player`) REFERENCES `players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE,
  ADD CONSTRAINT `Bans_ibfk_2` FOREIGN KEY (`Admin`) REFERENCES `players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `bugs`
--
ALTER TABLE `bugs`
  ADD CONSTRAINT `bugs_ibfk_1` FOREIGN KEY (`Playername`) REFERENCES `players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `logs`
--
ALTER TABLE `logs`
  ADD CONSTRAINT `Logs_ibfk_1` FOREIGN KEY (`Player`) REFERENCES `players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE,
  ADD CONSTRAINT `Logs_ibfk_2` FOREIGN KEY (`Target`) REFERENCES `players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `suggestions`
--
ALTER TABLE `suggestions`
  ADD CONSTRAINT `suggestions_ibfk_1` FOREIGN KEY (`Playername`) REFERENCES `players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `vips`
--
ALTER TABLE `vips`
  ADD CONSTRAINT `Vips_ibfk_1` FOREIGN KEY (`Name`) REFERENCES `players` (`Playername`) ON DELETE CASCADE ON UPDATE CASCADE;