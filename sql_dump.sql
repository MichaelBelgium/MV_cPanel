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
  `OnlineTime` int(11) NOT NULL DEFAULT '0',
  `rIP` varchar(16) NOT NULL COMMENT 'Registered IP',
  `lIP` varchar(16) NOT NULL COMMENT 'Last IP',
  PRIMARY KEY (`Playername`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;