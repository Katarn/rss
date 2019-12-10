-- phpMyAdmin SQL Dump
-- version 3.3.10.4
-- http://www.phpmyadmin.net

SET FOREIGN_KEY_CHECKS=0;
SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT=0;
START TRANSACTION;


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `rss`
--

-- --------------------------------------------------------

--
-- Структура таблицы `feeds`
--

DROP TABLE IF EXISTS `feeds`;
CREATE TABLE `feeds` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `parent_id` int(10) unsigned NOT NULL,
  `type` enum('folder','feed') NOT NULL,
  `url` varchar(255) NOT NULL,
  `feed_url` varchar(255) NOT NULL,
  `title` varchar(255) NOT NULL,
  `feed_title` varchar(255) NOT NULL,
  `feed_description` varchar(255) NOT NULL,
  `icon` mediumtext NOT NULL,
  `language` varchar(10) NOT NULL,
  `copyrights` varchar(255) NOT NULL,
  `author_name` varchar(100) NOT NULL,
  `author_email` varchar(100) NOT NULL,
  `author_uri` varchar(255) NOT NULL,
  `webmaster` varchar(100) NOT NULL,
  `category` varchar(255) NOT NULL,
  `contributor` varchar(255) NOT NULL,
  `generator` varchar(255) NOT NULL,
  `docs` varchar(255) NOT NULL,
  `new_count` int(10) unsigned NOT NULL,
  `news_count` int(10) unsigned NOT NULL,
  `unread_count` int(10) unsigned NOT NULL,
  `labels` varchar(255) NOT NULL,
  `tags` varchar(255) NOT NULL,
  `is_expanded` tinyint(1) unsigned NOT NULL,
  `sort` int(10) unsigned NOT NULL,
  `disable_update` tinyint(1) unsigned NOT NULL,
  `update_interval_enabled` tinyint(1) unsigned NOT NULL,
  `update_interval` smallint(6) unsigned NOT NULL,
  `update_interval_type` enum('mins','hours') NOT NULL,
  `delete_read` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `never_delete_unread` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `never_delete_starred` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `never_delete_labeled` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `updated` datetime NOT NULL,
  `created` datetime NOT NULL,
  `status` enum('idle','updating','error') NOT NULL DEFAULT 'idle',
  `statusMessage` varchar(255) NOT NULL,
  `authentication` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `delete_duplicates` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `javascript_enable` enum('global','yes','no') NOT NULL,
  `layout_direction` enum('left_to_right','right_to_left') NOT NULL DEFAULT 'left_to_right',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Структура таблицы `news`
--

DROP TABLE IF EXISTS `news`;
CREATE TABLE `news` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `feed_id` int(10) unsigned NOT NULL,
  `guid` varchar(255) NOT NULL,
  `link` varchar(255) NOT NULL,
  `title` varchar(255) CHARACTER SET utf8mb4 NOT NULL,
  `description` text CHARACTER SET utf8mb4 NOT NULL,
  `content` text CHARACTER SET utf8mb4 NOT NULL,
  `received` datetime NOT NULL,
  `published` datetime NOT NULL,
  `modified` datetime NOT NULL,
  `author_name` varchar(255) NOT NULL,
  `author_uri` varchar(255) NOT NULL,
  `author_email` varchar(255) NOT NULL,
  `category` varchar(255) NOT NULL,
  `label` varchar(255) NOT NULL,
  `comments` varchar(255) NOT NULL,
  `enclosure_url` varchar(255) NOT NULL,
  `enclosure_type` varchar(255) NOT NULL,
  `enclosure_length` int(11) unsigned NOT NULL DEFAULT '0',
  `is_new` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `is_read` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `is_starred` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `is_deleted` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `feed_deleted` (`feed_id`,`is_deleted`),
  KEY `published` (`published`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Структура таблицы `sessions`
--

DROP TABLE IF EXISTS `sessions`;
CREATE TABLE `sessions` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `token` char(32) NOT NULL,
  `expiration` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;
SET FOREIGN_KEY_CHECKS=1;
COMMIT;
