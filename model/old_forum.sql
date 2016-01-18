-- MySQL dump 10.13  Distrib 5.5.46, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: forum
-- ------------------------------------------------------
-- Server version	5.5.46-0+deb8u1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `messages`
--

DROP TABLE IF EXISTS `messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `messages` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `reply_to` int(11) NOT NULL,
  `thread_id` int(11) NOT NULL,
  `author` varchar(256) NOT NULL,
  `content` text NOT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `file` varchar(256) DEFAULT NULL,
  `thumb` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `messages`
--

LOCK TABLES `messages` WRITE;
/*!40000 ALTER TABLE `messages` DISABLE KEYS */;
INSERT INTO `messages` VALUES (1,0,1,'Admin','ÐÐ° Ð±Ð¾Ñ€Ð´Ðµ Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÐµÑ‚ÑÑ Ð¼Ð¾Ð´Ð¸Ñ„Ð¸Ñ†Ð¸Ñ€Ð¾Ð²Ð°Ð½Ð½Ñ‹Ð¹ Ð´Ð²Ð¸Ð¶Ð¾Ðº ÑÐ¸Ð½Ñ‚Ð°ÐºÑÐ¸ÑÐ° Ñ€Ð°Ð·Ð¼ÐµÑ‚ÐºÐ¸ [markdown](http://daringfireball.net/projects/markdown/syntax).\r\n\r\nÐÐ¸Ð¶Ðµ Ð¿Ñ€Ð¸Ð²ÐµÐ´ÐµÐ½Ñ‹ Ð¿Ñ€Ð¸Ð¼ÐµÑ€Ñ‹ Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·Ð¾Ð²Ð°Ð½Ð¸Ñ.','2012-06-18 09:40:38','',0),(2,0,1,'Admin','Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²ÐºÐ¸:\r\n\r\n	Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 1-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ\r\n	=====================\r\n\r\n	Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 2-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ\r\n	---------------------\r\n\r\n	# Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 1-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ\r\n\r\n	### Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 3-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ ######\r\n\r\n	###### Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 6-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ\r\n\r\nÐ—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 1-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ\r\n=====================\r\n\r\nÐ—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 2-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ\r\n---------------------\r\n\r\n# Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 1-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ\r\n\r\n### Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 3-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ ######\r\n\r\n###### Ð—Ð°Ð³Ð¾Ð»Ð¾Ð²Ð¾Ðº 6-Ð³Ð¾ ÑƒÑ€Ð¾Ð²Ð½Ñ','2012-06-18 09:43:11','',0),(3,0,1,'Admin','Ð¦Ð¸Ñ‚Ð°Ñ‚Ñ‹, ÑÐ¿Ð¾Ð¹Ð»ÐµÑ€Ñ‹, Ñ€ÐµÑ„Ð»Ð¸Ð½ÐºÐ¸:\r\n\r\n	> Ð­Ñ‚Ð¾ Ñ†Ð¸Ñ‚Ð°Ñ‚Ð° Ñ€Ð°Ð·Ð¼ÐµÑ‰ÐµÐ½Ð½Ð°Ñ Ð² Ð½ÐµÑÐºÐ¾Ð»ÑŒÐºÐ¾ ÑÑ‚Ñ€Ð¾Ðº.\r\n\r\n	> ÐžÐ´Ð½Ð°ÐºÐ¾ ÑÑ‚Ð¾Ð¸Ñ‚ Ð·Ð°Ð¼ÐµÑ‚Ð¸Ñ‚ÑŒ, Ñ‡Ñ‚Ð¾ Ð² Ð´Ð°Ð½Ð½Ð¾Ð¹ Ñ€Ð°Ð·Ð¼ÐµÑ‚ÐºÐµ\r\n	> Ð²Ð¾Ð¾Ð±Ñ‰Ðµ Ð½Ðµ Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÑŽÑ‚ÑŒÑÑ Ð¶ÐµÑÑ‚ÐºÐ¸Ðµ Ñ€Ð°Ð·Ñ€Ñ‹Ð²Ñ‹ ÑÑ‚Ñ€Ð¾Ðº.\r\n	> Ð’Ð¼ÐµÑÑ‚Ð¾ ÑÑ‚Ð¾Ð³Ð¾ Ñ‚ÐµÐºÑÑ‚ Ð²ÑÐµÐ³Ð´Ð° Ð½Ð°Ñ…Ð¾Ð´Ð¸Ñ‚ÑŒÑÑ Ð² Ð¿Ð°Ñ€Ð°Ð³Ñ€Ð°Ñ„Ð°Ñ…,\r\n	> Ñ‡Ñ‚Ð¾ Ð´ÐµÐ»Ð°ÐµÑ‚ ÐµÐ³Ð¾ Ð±Ð¾Ð»ÐµÐµ Ñ‡Ð¸Ñ‚Ð°ÐµÐ¼Ñ‹Ð¼ Ð¿Ñ€Ð¸ Ð»ÑŽÐ±Ñ‹Ñ… Ñ€Ð°Ð·Ñ€ÐµÑˆÐµÐ½Ð¸ÑÑ….\r\n\r\n\r\n    >>1 - ÐŸÐ°ÑÑÐ¸Ð²Ð½Ð°Ñ ÑÑÑ‹Ð»ÐºÐ° Ð½Ð° ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ðµ Ð½Ð° ÑÑ‚Ð¾Ð¹ Ð´Ð¾ÑÐºÐµ\r\n    \r\n    %%ÐÐµÐ±Ð¾Ð»ÑŒÑˆÐ¾Ð¹ ÑÐ¿Ð¾Ð¹Ð»ÐµÑ€%%\r\n\r\n> Ð­Ñ‚Ð¾ Ñ†Ð¸Ñ‚Ð°Ñ‚Ð° Ñ€Ð°Ð·Ð¼ÐµÑ‰ÐµÐ½Ð½Ð°Ñ Ð² Ð½ÐµÑÐºÐ¾Ð»ÑŒÐºÐ¾ ÑÑ‚Ñ€Ð¾Ðº.\r\n\r\n> ÐžÐ´Ð½Ð°ÐºÐ¾ ÑÑ‚Ð¾Ð¸Ñ‚ Ð·Ð°Ð¼ÐµÑ‚Ð¸Ñ‚ÑŒ, Ñ‡Ñ‚Ð¾ Ð² Ð´Ð°Ð½Ð½Ð¾Ð¹ Ñ€Ð°Ð·Ð¼ÐµÑ‚ÐºÐµ\r\n> Ð²Ð¾Ð¾Ð±Ñ‰Ðµ Ð½Ðµ Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÑŽÑ‚ÑŒÑÑ Ð¶ÐµÑÑ‚ÐºÐ¸Ðµ Ñ€Ð°Ð·Ñ€Ñ‹Ð²Ñ‹ ÑÑ‚Ñ€Ð¾Ðº.\r\n> Ð’Ð¼ÐµÑÑ‚Ð¾ ÑÑ‚Ð¾Ð³Ð¾ Ñ‚ÐµÐºÑÑ‚ Ð²ÑÐµÐ³Ð´Ð° Ð½Ð°Ñ…Ð¾Ð´Ð¸Ñ‚ÑŒÑÑ Ð² Ð¿Ð°Ñ€Ð°Ð³Ñ€Ð°Ñ„Ð°Ñ…,\r\n> Ñ‡Ñ‚Ð¾ Ð´ÐµÐ»Ð°ÐµÑ‚ ÐµÐ³Ð¾ Ð±Ð¾Ð»ÐµÐµ Ñ‡Ð¸Ñ‚Ð°ÐµÐ¼Ñ‹Ð¼ Ð¿Ñ€Ð¸ Ð»ÑŽÐ±Ñ‹Ñ… Ñ€Ð°Ð·Ñ€ÐµÑˆÐµÐ½Ð¸ÑÑ….\r\n\r\n\r\n>>1 - ÐŸÐ°ÑÑÐ¸Ð²Ð½Ð°Ñ ÑÑÑ‹Ð»ÐºÐ° Ð½Ð° ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ðµ Ð½Ð° ÑÑ‚Ð¾Ð¹ Ð´Ð¾ÑÐºÐµ\r\n    \r\n%%ÐÐµÐ±Ð¾Ð»ÑŒÑˆÐ¾Ð¹ ÑÐ¿Ð¾Ð¹Ð»ÐµÑ€%%','2012-06-18 09:51:01','',0),(4,0,1,'Admin','Ð¡Ð¿Ð¸ÑÐºÐ¸ Ð¸ Ð¼Ð¾Ð½Ð¾ÑˆÐ¸Ñ€Ð½Ñ‹Ð¹ Ñ‚ÐµÐºÑÑ‚:\r\n\r\n    *   Red\r\n    *   Green\r\n    *   Blue\r\n\r\n    1.  Bird\r\n    2.  McHale\r\n    3.  Parish\r\n\r\n*   Red\r\n*   Green\r\n*   Blue\r\n\r\n\r\n1.  Bird\r\n2.  McHale\r\n3.  Parish\r\n\r\n\r\n	ÐŸÑ€Ð¸Ð¼ÐµÑ€ `ÐºÐ¾Ð´Ð°` Ð² Ð¾Ð´Ð½Ñƒ ÑÑ‚Ñ€Ð¾ÐºÑƒ.\r\n\r\n        Ð‘Ð»Ð¾Ðº ÐºÐ¾Ð´Ð°\r\n        Ñ Ð¶ÐµÑÑ‚ÐºÐ¸Ð¼Ð¸\r\n        Ñ€Ð°Ð·Ñ€Ñ‹Ð²Ð°Ð¼Ð¸ ÑÑ‚Ñ€Ð¾Ðº.\r\n    \r\nÐŸÑ€Ð¸Ð¼ÐµÑ€ `ÐºÐ¾Ð´Ð°` Ð² Ð¾Ð´Ð½Ñƒ ÑÑ‚Ñ€Ð¾ÐºÑƒ.\r\n\r\n    Ð‘Ð»Ð¾Ðº ÐºÐ¾Ð´Ð°\r\n    Ñ Ð¶ÐµÑÑ‚ÐºÐ¸Ð¼Ð¸\r\n    Ñ€Ð°Ð·Ñ€Ñ‹Ð²Ð°Ð¼Ð¸ ÑÑ‚Ñ€Ð¾Ðº.','2012-06-18 09:54:45','',0),(5,0,1,'Admin','Ð Ð°Ð·Ð´ÐµÐ»Ð¸Ñ‚ÐµÐ»Ð¸:\r\n\r\n    * * *\r\n\r\n    ***\r\n\r\n    ---------------------------------------\r\n\r\n* * *\r\n\r\n***\r\n\r\n---------------------------------------','2012-06-18 09:55:33','',0),(6,0,1,'Admin','Ð“Ð¸Ð¿ÐµÑ€ÑÑÑ‹Ð»ÐºÐ¸:\r\n\r\n    <http://example.com/>\r\n\r\n    <address@example.com>\r\n    \r\n    [Ð¡ÑÑ‹Ð»ÐºÐ°](http://example.net/) Ð½Ðµ Ð¸Ð¼ÐµÑŽÑ‰Ð°Ñ Ð¿Ð¾Ð´ÑÐºÐ°Ð·ÐºÐ¸.\r\n    \r\n    Ð ÑÑ‚Ð¾ [Ð¿Ñ€Ð¸Ð¼ÐµÑ€ ÑÑÑ‹Ð»ÐºÐ¸](http://example.com/ \"ÐŸÐžÐ”Ð¡ÐšÐÐ—ÐšÐ!\") ÑÐ¾Ð´ÐµÑ€Ð¶Ð°Ñ‰ÑƒÑŽ Ð¿Ð¾Ð´ÑÐºÐ°Ð·ÐºÑƒ.\r\n\r\n<http://example.com/>\r\n\r\n<address@example.com>\r\n\r\n[Ð¡ÑÑ‹Ð»ÐºÐ°](http://example.net/) Ð½Ðµ Ð¸Ð¼ÐµÑŽÑ‰Ð°Ñ Ð¿Ð¾Ð´ÑÐºÐ°Ð·ÐºÐ¸.\r\n\r\nÐ ÑÑ‚Ð¾ [Ð¿Ñ€Ð¸Ð¼ÐµÑ€ ÑÑÑ‹Ð»ÐºÐ¸](http://example.com/ \"ÐŸÐžÐ”Ð¡ÐšÐÐ—ÐšÐ!\") ÑÐ¾Ð´ÐµÑ€Ð¶Ð°Ñ‰ÑƒÑŽ Ð¿Ð¾Ð´ÑÐºÐ°Ð·ÐºÑƒ.\r\n','2012-06-18 09:59:07','',0),(7,0,1,'Admin','Ð’Ñ‹Ð´ÐµÐ»ÐµÐ½Ð¸Ðµ:\r\n\r\n    *single asterisks*\r\n\r\n    _single underscores_\r\n\r\n    **double asterisks**\r\n\r\n    __double underscores__\r\n\r\n    \\*this text is surrounded by literal asterisks\\*\r\n\r\n*single asterisks*\r\n\r\n_single underscores_\r\n\r\n**double asterisks**\r\n\r\n__double underscores__\r\n\r\n\\*this text is surrounded by literal asterisks\\*','2012-06-18 19:03:05','',0),(8,0,2,'Admin','ÐŸÑ€Ð¾Ð²ÐµÑ€ÑÐµÐ¼ **Ñ€Ð°Ð·Ð¼ÐµÑ‚ÐºÑƒ**, Ð¿Ñ€Ð¾Ð±ÑƒÐµÐ¼ Ð¿Ð¾ÑÑ‚Ð¸Ñ‚ÑŒ Ñ„Ð°Ð¹Ð»Ñ‹, ÑÐ¼Ð¾Ñ‚Ñ€Ð¸Ð¼ Ð½Ð°Ð³Ñ€ÑƒÐ·ÐºÑƒ.','2012-06-24 07:36:41','',0),(11,0,2,'Anon','Ñ‚ÐµÑÑ‚','2012-06-24 07:40:00','',0),(12,0,2,'Anon','Ð»ÐµÐ¿Ð¸Ð¼ Ñ‚ÐµÐºÑÑ‚Ð¾Ð²Ñ‹Ð¹ Ñ„Ð°Ð¹Ð»','2012-06-24 07:40:24','txt',0),(19,0,2,'Anon','sad','2012-06-24 08:23:51','mp3',0),(20,0,2,'Anon','Ð˜Ñ‚Ð¾Ð³Ð¾ Ð½Ð° Ð´Ð°Ð½Ð½Ñ‹Ð¹ Ð¼Ð¾Ð¼ÐµÐ½Ñ‚ Ð¿Ð¾Ð¿Ð°. Ð‘Ð¾Ñ€Ð´Ð° Ð¿Ð°Ð´Ð°ÐµÑ‚, ÐµÑÐ»Ð¸ Ð¿Ñ‹Ñ‚Ð°ÐµÑ‚ÑÑ Ð¾Ñ‚Ñ€Ð¸ÑÐ¾Ð²Ð°Ñ‚ÑŒ Ð¿Ñ€ÐµÐ²ÑŒÑŽÑˆÐºÑƒ Ð´Ð»Ñ Ð±Ð¾Ð»ÑŒÑˆÐ¸Ñ… ÐºÐ°Ñ€Ñ‚Ð¸Ð½Ð¾Ðº.\r\n\r\nÐ¡Ð°Ð¼Ð¾Ðµ Ð¸Ð½Ñ‚Ñ€ÐµÑÐ½Ð¾Ðµ Ñ‡Ñ‚Ð¾ Ð¿Ð¾ Ð¸Ð´ÐµÐµ Ð²Ñ‹Ð»ÐµÑ‚Ð° Ð±Ñ‹Ñ‚ÑŒ Ð½Ðµ Ð´Ð¾Ð»Ð¶Ð½Ð¾, Ð° ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ðµ Ð´Ð¾Ð»Ð¶Ð½Ð¾ Ð±Ñ‹ Ð¿Ñ€Ð¸Ð¹Ñ‚Ð¸, Ð½Ð¾ Ð±ÐµÐ· Ð¿Ñ€ÐµÐ²ÑŒÑŽÑˆÐºÐ¸, Ñ‡Ñ‚Ð¾ Ð»ÐµÐ³ÐºÐ¾ ÑÐ¿Ð°ÑÐµÑ‚ Ð·Ð°Ñ€Ð°Ð½ÐµÐµ Ð³Ð¾Ñ‚Ð¾Ð²Ð°Ñ Ð·Ð°Ð³Ð»ÑƒÑˆÐºÐ°.\r\n\r\nÐ›ÐµÐ³ÐºÐ¾ Ð¸Ð´ÑƒÑ‚ Ð¿Ð¸ÐºÑ‡Ð¸ Ð¼ÐµÐ½ÑŒÑˆÐµ 200Ñ…200, Ð¸ Ð²Ð¾Ð¾Ð±Ñ‰Ðµ Ð»ÑŽÐ±Ñ‹Ðµ Ð½Ðµ Ð¸Ð·Ð¾Ð±Ñ€Ð°Ð¶ÐµÐ½Ð¸Ñ Ð½Ð¸Ð¶Ðµ Ð»Ð¸Ð¼Ð¸Ñ‚Ð°. Ð’ÑÐµ ÐºÐ°Ðº Ð½Ð°Ð´Ð¾. ÐºÑ€Ð¾Ð¼Ðµ Ð²Ñ‹ÑˆÐµÐ¾Ð¿Ð¸ÑÐ°Ð½Ð½Ð¾Ð¹ Ð¿Ð¾Ð¿ÐºÐ¸.','2012-06-24 08:31:22','',0),(21,0,2,'Anon','>>20 \r\n\r\n\r\n### ÐŸÑ€Ð¾Ð±Ð»ÐµÐ¼Ð° Ð² Ð±Ð¸Ð±Ð»Ð¸Ð¾Ñ‚ÐµÐºÐµ magick++\r\n\r\nÐÐµÑÐ¼Ð¾Ñ‚Ñ€Ñ Ð½Ð° Ñ‚Ð¾, Ñ‡Ñ‚Ð¾ Ð² Ð¿ÐµÑÐ¾Ñ‡Ð½Ð¸Ñ†Ðµ Ð²ÑÐµ Ð³Ð»Ð°Ð´ÐºÐ¾. Ð—Ð´ÐµÑÑŒ Ñ‚Ð°ÐºÐ¾Ð¹ Ð¿Ñ€Ð¾ÑÑ‚Ð¾Ð¹ ÐºÐ¾Ð´ Ð»Ð¾Ð¼Ð°ÐµÑ‚ Ð²ÑÐµ Ð½Ð°Ð¿Ñ€Ð¾Ñ‡ÑŒ.\r\n\r\n	#include <Magick++.h>\r\n	using namespace Magick;\r\n	int main()\r\n	{\r\n		Image button;\r\n		button.read(\"my_image.jpg\");\r\n		button.resize(\"200x200>\");\r\n		button.write(\"my_image2.jpg\");\r\n		return 0;\r\n	}\r\n\r\nÐ’Ñ‹Ñ…Ð»Ð¾Ð¿: `segfault error 6 in libMagickCore.so.5.0.0`\r\n\r\nÐ¡Ð²ÐµÐ¶Ð¸Ðµ Ð¿Ð°ÐºÐµÑ‚Ð¸ÐºÐ¸ ÑÐ¸Ñ‚ÑƒÐ°Ñ†Ð¸ÑŽ Ð½Ðµ Ð¸Ð·Ð¼ÐµÐ½Ð¸Ð»Ð¸.','2012-06-24 10:54:54','',0),(22,0,2,'Anon','ÐžÐ»Ð¾Ð»Ð¾! Ð£Ð±Ñ€Ð°Ð»Ð¸ Ð£Ð¸Ð·Ð¸ Ñ€ÐµÐ¿Ð¾Ð·Ð¸Ñ‚Ð¾Ñ€Ð¸Ð¸, Ð¾Ð±Ð½Ð¾Ð²Ð¸Ð»Ð¸ÑÑŒ. Ð’ÑÐµ Ð¿Ð°ÑˆÐµÑ‚.','2012-06-24 11:38:02','jpg',0),(23,0,2,'Anon','>>22 \r\n\r\n> JPG\r\n\r\n\r\nÐžÑÑ‚Ð°Ð»Ð¾ÑÑŒ Ð¿Ð°ÐºÐµÑ‚ Ð¿ÐµÑ€ÐµÑÐ¾Ð±Ñ€Ð°Ñ‚ÑŒ. ÐžÐ¿ÑÑ‚ÑŒ Ð´Ð¾Ð±Ð°Ð²Ð¸Ñ‚ÑŒ Ð½Ðµ Ð¾Ñ‡ÐµÐ½ÑŒ ÑÑ‚Ð°Ð±Ð¸Ð»ÑŒÐ½ÑƒÑŽ Ð²ÐµÑ‚ÐºÑƒ Ð¸ Ð¿Ñ€Ð¾ÐºÐ°Ñ‚Ð¸Ñ‚ÑŒ ÑÐºÑÐ¿ÐµÑ€ÐµÐ¼ÐµÐ½Ñ‚. Ð¢.Ðº. ÑÐµÐ¹Ñ‡Ð°Ñ ÐµÑ‰Ðµ Ð½Ðµ Ð¾Ñ‡ÐµÐ½ÑŒ Ð¿Ñ€ÐµÐ²ÑŒÑŽÑˆÐºÐ¸ Ð´ÐµÐ»Ð°ÑŽÑ‚ÑÑ.','2012-06-24 11:53:42','',0),(24,0,2,'Anon','ÐŸÑ€Ð¾Ð²ÐµÑ€ÑÐµÐ¼ Ð¿ÐµÑ€ÐµÑÐ¾Ð±Ñ€Ð°Ð½Ñ‹Ð¹ Ð¿Ð°ÐºÐµÑ‚.','2012-06-24 11:45:34','jpg',1),(29,0,2,'Anon','ÐŸÑ€Ð¾Ð²ÐµÑ€ÑÐµÐ¼ Ñ€Ð°Ð±Ð¾Ñ‚Ñƒ Ð¿Ð¾ÑÐ»Ðµ Ð¾Ð±Ð½Ð¾Ð²Ð»ÐµÐ½Ð¸Ñ','2012-06-29 16:59:56','jpg',1),(30,0,2,'Anon','ÐŸÑ€Ð¾Ð²ÐµÑ€ÑÐµÐ¼ Ñ€ÐµÑ„Ð»Ð¸Ð½Ðº Ð½Ð° ÐºÐ¾Ð½Ñ†Ðµ ÑÑ‚Ñ€Ð¾ÐºÐ¸ >>12','2012-07-09 12:11:57','jpg',1),(31,0,2,'Anon','ÐŸÑ€Ð¾Ð²ÐµÑ€ÑÐµÐ¼ Ñ€Ð°Ð·ÑƒÐ¿Ð»Ð¾Ñ‚Ð½ÐµÐ½Ð¸Ðµ.\r\n*ololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololo*','2012-07-17 16:31:34','',0),(33,0,2,'Anon','Ð¡Ð¾Ð·Ð´Ð°ÐµÐ¼ Ð´Ð²Ð° ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ñ Ñ Ñ„Ð°Ð¹Ð»Ð°Ð¼Ð¸. (Ð­Ñ‚Ð¾ Ð²Ñ‚Ð¾Ñ€Ð¾Ð¹) ÐžÐ´Ð½Ð¾ ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ðµ ÑƒÐ´Ð°Ð»ÑÐµÐ¼, Ð¸Ð· Ð²Ñ‚Ð¾Ñ€Ð¾Ð³Ð¾ ÑƒÐ´Ð°Ð»ÑÐµÐ¼ Ñ„Ð°Ð¹Ð». ','2012-08-12 11:08:31','',0),(34,0,2,'Anon','ÐŸÑ€Ð¾Ð²ÐµÑ€ÑÐµÐ¼ Flash Ð²Ð»Ð¾Ð¶ÐµÐ½Ð¸Ðµ','2012-08-17 07:03:30','swf',0);
/*!40000 ALTER TABLE `messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `threads`
--

DROP TABLE IF EXISTS `threads`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `threads` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(256) NOT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `threads`
--

LOCK TABLES `threads` WRITE;
/*!40000 ALTER TABLE `threads` DISABLE KEYS */;
INSERT INTO `threads` VALUES (1,'Ð Ð°Ð·Ð¼ÐµÑ‚ÐºÐ° MarkDown','2012-06-18 19:03:05'),(2,'Ð¢Ñ€ÐµÐ´ Ð¾Ñ‚Ð»Ð°Ð´ÐºÐ¸','2012-12-04 08:39:21');
/*!40000 ALTER TABLE `threads` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-01-18  9:41:28
