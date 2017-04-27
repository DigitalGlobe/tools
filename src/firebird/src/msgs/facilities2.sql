/* MAX_NUMBER is the next number to be used, always one more than the highest message number. */
set bulk_insert INSERT INTO FACILITIES (LAST_CHANGE, FACILITY, FAC_CODE, MAX_NUMBER) VALUES (?, ?, ?, ?);
--
-- 668-696 are skipped to be in sync with FB3
('2010-02-01 16:45:47', 'JRD', 0, 698)
-- Reserved 513-529 by CVC for FB3
('2009-07-16 05:41:59', 'QLI', 1, 530)
('2008-11-28 20:27:04', 'GDEF', 2, 346)
('2009-07-03 11:27:34', 'GFIX', 3, 121)
('1996-11-07 13:39:40', 'GPRE', 4, 1)
--
--('1996-11-07 13:39:40', 'GLTJ', 5, 1)
--('1996-11-07 13:39:40', 'GRST', 6, 1)
--
('2005-11-05 13:09:00', 'DSQL', 7, 32)
('2012-01-13 11:21:19', 'DYN', 8, 285)
--
--('1996-11-07 13:39:40', 'FRED', 9, 1)
--
('1996-11-07 13:39:40', 'INSTALL', 10, 1)
('1996-11-07 13:38:41', 'TEST', 11, 4)
-- Reserved 326-334 by CVC for FB3
-- Skipped 352-360 by hvlad
('2015-07-23 14:20:00', 'GBAK', 12, 370)
('2009-06-05 23:07:00', 'SQLERR', 13, 970)
('1996-11-07 13:38:42', 'SQLWARN', 14, 613)
('2006-09-10 03:04:31', 'JRD_BUGCHK', 15, 307)
--
--('1996-11-07 13:38:43', 'GJRN', 16, 241)
--
('2008-11-28 20:59:39', 'ISQL', 17, 165)
('2009-11-13 17:49:54', 'GSEC', 18, 104)
('2002-03-05 02:30:12', 'LICENSE', 19, 60)
('2002-03-05 02:31:54', 'DOS', 20, 74)
('2009-06-22 05:57:59', 'GSTAT', 21, 46)
-- Reserved 51 by CVC for FB3
('2009-12-17 16:02:59', 'FBSVCMGR', 22, 53)
('2007-12-21 19:03:07', 'UTL', 23, 2)
-- Reserved 24-69 by CVC for FB3
('2009-12-16 19:27:50', 'NBACKUP', 24, 71)
-- Reserved facility 25 for FBTRACEMGR 1-40 by CVC for FB3
stop

COMMIT WORK;
