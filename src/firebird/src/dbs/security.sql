/*
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 * 
 * 2004.09.14 Alex Peshkoff - security changes, preventing ordinary users
 *		from access to other users crypted passwords and enabling modification
 *		of there own password. Originally suggested by Ivan Prenosil
 *		(see http://www.volny.cz/iprenosil/interbase/ for details).
 */

/* Domain definitions */
CREATE DOMAIN RDB$COMMENT AS BLOB SUB_TYPE TEXT SEGMENT SIZE 80 CHARACTER SET UNICODE_FSS;
CREATE DOMAIN RDB$NAME_PART AS VARCHAR(32) CHARACTER SET UNICODE_FSS DEFAULT _UNICODE_FSS '';
CREATE DOMAIN RDB$GID AS INTEGER;
CREATE DOMAIN RDB$PASSWD AS VARCHAR(64) CHARACTER SET BINARY;
CREATE DOMAIN RDB$UID AS INTEGER;
CREATE DOMAIN RDB$USER_NAME AS VARCHAR(128) CHARACTER SET UNICODE_FSS;
CREATE DOMAIN RDB$USER_PRIVILEGE AS INTEGER;


/*  Table: RDB$USERS */
CREATE TABLE RDB$USERS (
	RDB$USER_NAME 		RDB$USER_NAME NOT NULL PRIMARY KEY,
	/* local system user name for setuid for file permissions */
	RDB$SYS_USER_NAME	RDB$USER_NAME,
	RDB$GROUP_NAME		RDB$USER_NAME,
	RDB$UID 			RDB$UID,
	RDB$GID 			RDB$GID,
	RDB$PASSWD 			RDB$PASSWD NOT NULL,

	/* Privilege level of user - mark a user as having DBA privilege */
	RDB$PRIVILEGE 		RDB$USER_PRIVILEGE,

	RDB$COMMENT 		RDB$COMMENT,
	RDB$FIRST_NAME 		RDB$NAME_PART,
	RDB$MIDDLE_NAME		RDB$NAME_PART,
	RDB$LAST_NAME		RDB$NAME_PART);

COMMIT;

/*  View: USERS. Let's user modify his own password. */
CREATE VIEW USERS (USER_NAME, SYS_USER_NAME, GROUP_NAME, UID, GID, PASSWD,
		PRIVILEGE, COMMENT, FIRST_NAME, MIDDLE_NAME, LAST_NAME, FULL_NAME) AS 
	SELECT RDB$USER_NAME, RDB$SYS_USER_NAME, RDB$GROUP_NAME, RDB$UID, RDB$GID, RDB$PASSWD, 
		RDB$PRIVILEGE, RDB$COMMENT, RDB$FIRST_NAME, RDB$MIDDLE_NAME, RDB$LAST_NAME, 
		COALESCE (RDB$first_name || _UNICODE_FSS ' ', '') || 
		COALESCE (RDB$middle_name || _UNICODE_FSS ' ', '') || 
		COALESCE (RDB$last_name, '')
	FROM RDB$USERS
	WHERE CURRENT_USER = 'SYSDBA'
	   OR CURRENT_ROLE = 'RDB$ADMIN'
	   OR CURRENT_USER = RDB$USERS.RDB$USER_NAME;

/*	Access rights */
GRANT ALL ON RDB$USERS to VIEW USERS;
GRANT SELECT ON USERS to PUBLIC;
GRANT UPDATE(PASSWD, GROUP_NAME, UID, GID, FIRST_NAME, MIDDLE_NAME, LAST_NAME)
	ON USERS TO PUBLIC;

COMMIT;

/*	Needed record - with PASSWD = random + SHA1 (random + 'SYSDBA' + crypt('masterke')) */
INSERT INTO RDB$USERS(RDB$USER_NAME, RDB$PASSWD, RDB$FIRST_NAME, RDB$MIDDLE_NAME, RDB$LAST_NAME)
	VALUES ('SYSDBA', 'NLtwcs9LrxLMOYhG0uGM9i6KS7mf3QAKvFVpmRg=', 'Sql', 'Server', 'Administrator');

COMMIT;
