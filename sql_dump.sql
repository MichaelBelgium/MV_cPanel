--
-- Table structure for table `Logs`
--

CREATE TABLE IF NOT EXISTS `Logs` (
  `lID` int(11) NOT NULL AUTO_INCREMENT,
  `Type` tinyint(4) NOT NULL,
  `Player` varchar(24) NOT NULL,
  `Target` varchar(24) DEFAULT NULL,
  `Command` varchar(32) DEFAULT NULL,
  `Params` varchar(64) DEFAULT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`lID`),
  KEY `Player` (`Player`),
  KEY `Target` (`Target`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

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

-- --------------------------------------------------------

--
-- Table structure for table `Bans`
--

CREATE TABLE IF NOT EXISTS `Bans` (
  `bID` int(11) NOT NULL AUTO_INCREMENT,
  `Player` varchar(24) NOT NULL,
  `Admin` varchar(24) NOT NULL,
  `Reason` varchar(32) NOT NULL,
  `IP` varchar(16) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`bID`),
  KEY `Player` (`Player`,`Admin`),
  KEY `Admin` (`Admin`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `Logs`
--
ALTER TABLE `Logs`
  ADD CONSTRAINT `Logs_ibfk_1` FOREIGN KEY (`Player`) REFERENCES `Players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE,
  ADD CONSTRAINT `Logs_ibfk_2` FOREIGN KEY (`Target`) REFERENCES `Players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Vips`
--
ALTER TABLE `Vips`
  ADD CONSTRAINT `Vips_ibfk_1` FOREIGN KEY (`Name`) REFERENCES `Players` (`Playername`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `Bans`
--
ALTER TABLE `Bans`
  ADD CONSTRAINT `Bans_ibfk_2` FOREIGN KEY (`Admin`) REFERENCES `Players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE,
  ADD CONSTRAINT `Bans_ibfk_1` FOREIGN KEY (`Player`) REFERENCES `Players` (`Playername`) ON DELETE NO ACTION ON UPDATE CASCADE;