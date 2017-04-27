/*********** Preprocessed module -- do not edit ***************/
/*********** Preprocessed module -- do not edit ***************/
/*********** Preprocessed module -- do not edit ***************/
/*********** Preprocessed module -- do not edit ***************/
/*********** Preprocessed module -- do not edit ***************/
/***************** gpre version WI-V2.5.7.27050 Firebird 2.5 **********************/
/*
 * tab=4
 *____________________________________________________________
 *
 *		PROGRAM:	C preprocessor
 *		MODULE:		gpre_meta.epp
 *		DESCRIPTION:	Meta data interface to system
 *
 *  The contents of this file are subject to the Interbase Public
 *  License Version 1.0 (the "License"); you may not use this file
 *  except in compliance with the License. You may obtain a copy
 *  of the License at http://www.Inprise.com/IPL.html
 *
 *  Software distributed under the License is distributed on an
 *  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 *  or implied. See the License for the specific language governing
 *  rights and limitations under the License.
 *
 *  The Original Code was created by Inprise Corporation
 *  and its predecessors. Portions created by Inprise Corporation are
 *  Copyright (C) Inprise Corporation.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *____________________________________________________________
 *
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/ibase.h"
#include "../gpre/gpre.h"
//#include "../jrd/license.h"
#include "../jrd/intl.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/hsh_proto.h"
#include "../gpre/jrdme_proto.h"
#include "../gpre/gpre_meta.h"
#include "../gpre/msc_proto.h"
#include "../gpre/par_proto.h"
#include "../common/utils_proto.h"
#include "../common/classes/ClumpletWriter.h"


const char NEED_IB4_AT_LEAST[] = "Databases before IB4 are not supported";


/*DATABASE DB = FILENAME "yachts.lnk" RUNTIME database->dbb_filename;*/
/**** GDS Preprocessor Definitions ****/
#ifndef JRD_IBASE_H
#include <ibase.h>
#endif

static const ISC_QUAD
   isc_blob_null = {0, 0};	/* initializer for blobs */
isc_db_handle
   DB = 0;		/* database handle */

isc_tr_handle
   gds_trans = 0;		/* default transaction handle */
ISC_STATUS
   isc_status [20],	/* status vector */
   isc_status2 [20];	/* status vector */
ISC_LONG
   isc_array_length, 	/* array return size */
   SQLCODE;		/* SQL status code */
static const short
   isc_0l = 328;
static const char
   isc_0 [] = {
   4,2,4,1,2,0,7,0,7,0,4,0,2,0,41,3,0,32,0,41,3,0,32,0,12,0,2,7,
   'C',3,'J',18,'R','D','B','$','C','H','A','R','A','C','T','E',
   'R','_','S','E','T','S',0,'J',14,'R','D','B','$','C','O','L',
   'L','A','T','I','O','N','S',1,'J',9,'R','D','B','$','T','Y',
   'P','E','S',2,'D',21,8,0,1,0,0,0,'G',58,47,23,1,20,'R','D','B',
   '$','C','H','A','R','A','C','T','E','R','_','S','E','T','_',
   'I','D',23,0,20,'R','D','B','$','C','H','A','R','A','C','T',
   'E','R','_','S','E','T','_','I','D',58,47,23,1,18,'R','D','B',
   '$','C','O','L','L','A','T','I','O','N','_','N','A','M','E',
   25,0,1,0,58,47,23,2,13,'R','D','B','$','T','Y','P','E','_','N',
   'A','M','E',25,0,0,0,58,47,23,2,14,'R','D','B','$','F','I','E',
   'L','D','_','N','A','M','E',21,14,22,0,'R','D','B','$','C','H',
   'A','R','A','C','T','E','R','_','S','E','T','_','N','A','M',
   'E',47,23,2,8,'R','D','B','$','T','Y','P','E',23,0,20,'R','D',
   'B','$','C','H','A','R','A','C','T','E','R','_','S','E','T',
   '_','I','D',-1,14,1,2,1,21,8,0,1,0,0,0,25,1,0,0,1,23,0,20,'R',
   'D','B','$','C','H','A','R','A','C','T','E','R','_','S','E',
   'T','_','I','D',25,1,1,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,0,0,-1,
   -1,'L'
   };	/* end of blr string for request isc_0 */

static const short
   isc_7l = 208;
static const char
   isc_7 [] = {
   4,2,4,1,2,0,7,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',2,'J',18,
   'R','D','B','$','C','H','A','R','A','C','T','E','R','_','S',
   'E','T','S',0,'J',14,'R','D','B','$','C','O','L','L','A','T',
   'I','O','N','S',1,'D',21,8,0,1,0,0,0,'G',58,47,23,1,20,'R','D',
   'B','$','C','H','A','R','A','C','T','E','R','_','S','E','T',
   '_','I','D',23,0,20,'R','D','B','$','C','H','A','R','A','C',
   'T','E','R','_','S','E','T','_','I','D',47,23,1,18,'R','D','B',
   '$','C','O','L','L','A','T','I','O','N','_','N','A','M','E',
   25,0,0,0,-1,14,1,2,1,21,8,0,1,0,0,0,25,1,0,0,1,23,0,20,'R','D',
   'B','$','C','H','A','R','A','C','T','E','R','_','S','E','T',
   '_','I','D',25,1,1,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,0,0,-1,-1,
   'L'
   };	/* end of blr string for request isc_7 */

static const short
   isc_13l = 298;
static const char
   isc_13 [] = {
   4,2,4,1,2,0,7,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',3,'J',18,
   'R','D','B','$','C','H','A','R','A','C','T','E','R','_','S',
   'E','T','S',0,'J',14,'R','D','B','$','C','O','L','L','A','T',
   'I','O','N','S',1,'J',9,'R','D','B','$','T','Y','P','E','S',
   2,'D',21,8,0,1,0,0,0,'G',58,47,23,2,13,'R','D','B','$','T','Y',
   'P','E','_','N','A','M','E',25,0,0,0,58,47,23,2,14,'R','D','B',
   '$','F','I','E','L','D','_','N','A','M','E',21,14,22,0,'R','D',
   'B','$','C','H','A','R','A','C','T','E','R','_','S','E','T',
   '_','N','A','M','E',58,47,23,2,8,'R','D','B','$','T','Y','P',
   'E',23,0,20,'R','D','B','$','C','H','A','R','A','C','T','E',
   'R','_','S','E','T','_','I','D',47,23,0,24,'R','D','B','$','D',
   'E','F','A','U','L','T','_','C','O','L','L','A','T','E','_',
   'N','A','M','E',23,1,18,'R','D','B','$','C','O','L','L','A',
   'T','I','O','N','_','N','A','M','E',-1,14,1,2,1,21,8,0,1,0,0,
   0,25,1,0,0,1,23,0,20,'R','D','B','$','C','H','A','R','A','C',
   'T','E','R','_','S','E','T','_','I','D',25,1,1,0,-1,14,1,1,21,
   8,0,0,0,0,0,25,1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_13 */

static const short
   isc_19l = 164;
static const char
   isc_19 [] = {
   4,2,4,0,2,0,41,3,0,32,0,7,0,2,7,'C',1,'J',12,'R','D','B','$',
   'D','A','T','A','B','A','S','E',0,'D',21,8,0,1,0,0,0,'G',58,
   59,61,23,0,22,'R','D','B','$','C','H','A','R','A','C','T','E',
   'R','_','S','E','T','_','N','A','M','E',48,23,0,22,'R','D','B',
   '$','C','H','A','R','A','C','T','E','R','_','S','E','T','_',
   'N','A','M','E',21,14,1,0,32,-1,14,0,2,1,23,0,22,'R','D','B',
   '$','C','H','A','R','A','C','T','E','R','_','S','E','T','_',
   'N','A','M','E',25,0,0,0,1,21,8,0,1,0,0,0,25,0,1,0,-1,14,0,1,
   21,8,0,0,0,0,0,25,0,1,0,-1,-1,'L'
   };	/* end of blr string for request isc_19 */

static const short
   isc_23l = 195;
static const char
   isc_23 [] = {
   4,2,4,1,4,0,8,0,8,0,7,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',
   1,'J',20,'R','D','B','$','F','I','E','L','D','_','D','I','M',
   'E','N','S','I','O','N','S',0,'G',47,23,0,14,'R','D','B','$',
   'F','I','E','L','D','_','N','A','M','E',25,0,0,0,'F',1,'H',23,
   0,13,'R','D','B','$','D','I','M','E','N','S','I','O','N',-1,
   14,1,2,1,23,0,15,'R','D','B','$','U','P','P','E','R','_','B',
   'O','U','N','D',25,1,0,0,1,23,0,15,'R','D','B','$','L','O','W',
   'E','R','_','B','O','U','N','D',25,1,1,0,1,21,8,0,1,0,0,0,25,
   1,2,0,1,23,0,13,'R','D','B','$','D','I','M','E','N','S','I',
   'O','N',25,1,3,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,2,0,-1,-1,'L'
   
   };	/* end of blr string for request isc_23 */

static const short
   isc_31l = 117;
static const char
   isc_31 [] = {
   4,2,4,1,2,0,7,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',1,'J',10,
   'R','D','B','$','F','I','E','L','D','S',0,'G',47,23,0,14,'R',
   'D','B','$','F','I','E','L','D','_','N','A','M','E',25,0,0,0,
   -1,14,1,2,1,21,8,0,1,0,0,0,25,1,0,0,1,23,0,14,'R','D','B','$',
   'D','I','M','E','N','S','I','O','N','S',25,1,1,0,-1,14,1,1,21,
   8,0,0,0,0,0,25,1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_31 */

static const short
   isc_37l = 97;
static const char
   isc_37 [] = {
   4,2,4,1,1,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',1,'J',12,'R',
   'D','B','$','T','R','I','G','G','E','R','S',0,'G',47,23,0,16,
   'R','D','B','$','T','R','I','G','G','E','R','_','N','A','M',
   'E',25,0,0,0,-1,14,1,2,1,21,8,0,1,0,0,0,25,1,0,0,-1,14,1,1,21,
   8,0,0,0,0,0,25,1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_37 */

static const short
   isc_42l = 137;
static const char
   isc_42 [] = {
   4,2,4,1,2,0,7,0,7,0,4,0,2,0,41,3,0,32,0,41,3,0,32,0,12,0,2,7,
   'C',1,'J',9,'R','D','B','$','T','Y','P','E','S',0,'G',58,47,
   23,0,14,'R','D','B','$','F','I','E','L','D','_','N','A','M',
   'E',25,0,1,0,47,23,0,13,'R','D','B','$','T','Y','P','E','_',
   'N','A','M','E',25,0,0,0,-1,14,1,2,1,21,8,0,1,0,0,0,25,1,0,0,
   1,23,0,8,'R','D','B','$','T','Y','P','E',25,1,1,0,-1,14,1,1,
   21,8,0,0,0,0,0,25,1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_42 */

static const short
   isc_49l = 94;
static const char
   isc_49 [] = {
   4,2,4,0,2,0,41,3,0,32,0,7,0,2,7,'C',1,'J',14,'R','D','B','$',
   'G','E','N','E','R','A','T','O','R','S',0,-1,14,0,2,1,23,0,18,
   'R','D','B','$','G','E','N','E','R','A','T','O','R','_','N',
   'A','M','E',25,0,0,0,1,21,8,0,1,0,0,0,25,0,1,0,-1,14,0,1,21,
   8,0,0,0,0,0,25,0,1,0,-1,-1,'L'
   };	/* end of blr string for request isc_49 */

static const short
   isc_53l = 132;
static const char
   isc_53 [] = {
   4,2,4,0,2,0,41,3,0,32,0,7,0,2,7,'C',1,'J',12,'R','D','B','$',
   'D','A','T','A','B','A','S','E',0,'D',21,8,0,1,0,0,0,'G',59,
   61,23,0,22,'R','D','B','$','C','H','A','R','A','C','T','E','R',
   '_','S','E','T','_','N','A','M','E',-1,14,0,2,1,23,0,22,'R',
   'D','B','$','C','H','A','R','A','C','T','E','R','_','S','E',
   'T','_','N','A','M','E',25,0,0,0,1,21,8,0,1,0,0,0,25,0,1,0,-1,
   14,0,1,21,8,0,0,0,0,0,25,0,1,0,-1,-1,'L'
   };	/* end of blr string for request isc_53 */

static const short
   isc_57l = 181;
static const char
   isc_57 [] = {
   4,2,4,1,2,0,41,3,0,32,0,7,0,4,0,2,0,41,3,0,32,0,7,0,12,0,2,7,
   'C',1,'J',9,'R','D','B','$','T','Y','P','E','S',0,'G',58,47,
   23,0,14,'R','D','B','$','F','I','E','L','D','_','N','A','M',
   'E',21,14,22,0,'R','D','B','$','C','H','A','R','A','C','T','E',
   'R','_','S','E','T','_','N','A','M','E',58,47,23,0,8,'R','D',
   'B','$','T','Y','P','E',25,0,1,0,48,23,0,13,'R','D','B','$',
   'T','Y','P','E','_','N','A','M','E',25,0,0,0,-1,14,1,2,1,23,
   0,13,'R','D','B','$','T','Y','P','E','_','N','A','M','E',25,
   1,0,0,1,21,8,0,1,0,0,0,25,1,1,0,-1,14,1,1,21,8,0,0,0,0,0,25,
   1,1,0,-1,-1,'L'
   };	/* end of blr string for request isc_57 */

static const short
   isc_64l = 292;
static const char
   isc_64 [] = {
   4,2,4,0,7,0,41,3,0,32,0,7,0,7,0,7,0,7,0,7,0,7,0,2,7,'C',2,'J',
   18,'R','D','B','$','C','H','A','R','A','C','T','E','R','_','S',
   'E','T','S',0,'J',14,'R','D','B','$','C','O','L','L','A','T',
   'I','O','N','S',1,'G',47,23,0,24,'R','D','B','$','D','E','F',
   'A','U','L','T','_','C','O','L','L','A','T','E','_','N','A',
   'M','E',23,1,18,'R','D','B','$','C','O','L','L','A','T','I',
   'O','N','_','N','A','M','E',-1,14,0,2,1,23,0,22,'R','D','B',
   '$','C','H','A','R','A','C','T','E','R','_','S','E','T','_',
   'N','A','M','E',25,0,0,0,1,21,8,0,1,0,0,0,25,0,1,0,1,23,0,20,
   'R','D','B','$','C','H','A','R','A','C','T','E','R','_','S',
   'E','T','_','I','D',25,0,2,0,1,23,0,23,'R','D','B','$','B','Y',
   'T','E','S','_','P','E','R','_','C','H','A','R','A','C','T',
   'E','R',41,0,4,0,3,0,1,23,1,16,'R','D','B','$','C','O','L','L',
   'A','T','I','O','N','_','I','D',25,0,5,0,1,23,1,20,'R','D','B',
   '$','C','H','A','R','A','C','T','E','R','_','S','E','T','_',
   'I','D',25,0,6,0,-1,14,0,1,21,8,0,0,0,0,0,25,0,1,0,-1,-1,'L'
   
   };	/* end of blr string for request isc_64 */

static const short
   isc_73l = 177;
static const char
   isc_73 [] = {
   4,2,4,1,2,0,41,3,0,32,0,7,0,4,0,2,0,41,3,0,32,0,7,0,12,0,2,7,
   'C',1,'J',9,'R','D','B','$','T','Y','P','E','S',0,'G',58,47,
   23,0,14,'R','D','B','$','F','I','E','L','D','_','N','A','M',
   'E',21,14,18,0,'R','D','B','$','C','O','L','L','A','T','I','O',
   'N','_','N','A','M','E',58,47,23,0,8,'R','D','B','$','T','Y',
   'P','E',25,0,1,0,48,23,0,13,'R','D','B','$','T','Y','P','E',
   '_','N','A','M','E',25,0,0,0,-1,14,1,2,1,23,0,13,'R','D','B',
   '$','T','Y','P','E','_','N','A','M','E',25,1,0,0,1,21,8,0,1,
   0,0,0,25,1,1,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,1,0,-1,-1,'L'
   };	/* end of blr string for request isc_73 */

static const short
   isc_80l = 256;
static const char
   isc_80 [] = {
   4,2,4,0,6,0,41,3,0,32,0,7,0,7,0,7,0,7,0,7,0,2,7,'C',2,'J',18,
   'R','D','B','$','C','H','A','R','A','C','T','E','R','_','S',
   'E','T','S',0,'J',14,'R','D','B','$','C','O','L','L','A','T',
   'I','O','N','S',1,'G',47,23,1,20,'R','D','B','$','C','H','A',
   'R','A','C','T','E','R','_','S','E','T','_','I','D',23,0,20,
   'R','D','B','$','C','H','A','R','A','C','T','E','R','_','S',
   'E','T','_','I','D',-1,14,0,2,1,23,1,18,'R','D','B','$','C',
   'O','L','L','A','T','I','O','N','_','N','A','M','E',25,0,0,0,
   1,21,8,0,1,0,0,0,25,0,1,0,1,23,0,23,'R','D','B','$','B','Y',
   'T','E','S','_','P','E','R','_','C','H','A','R','A','C','T',
   'E','R',41,0,3,0,2,0,1,23,1,16,'R','D','B','$','C','O','L','L',
   'A','T','I','O','N','_','I','D',25,0,4,0,1,23,1,20,'R','D','B',
   '$','C','H','A','R','A','C','T','E','R','_','S','E','T','_',
   'I','D',25,0,5,0,-1,14,0,1,21,8,0,0,0,0,0,25,0,1,0,-1,-1,'L'
   
   };	/* end of blr string for request isc_80 */

static const short
   isc_88l = 358;
static const char
   isc_88 [] = {
   4,2,4,1,3,0,7,0,7,0,7,0,4,0,2,0,41,3,0,32,0,7,0,12,0,2,7,'C',
   3,'J',22,'R','D','B','$','F','U','N','C','T','I','O','N','_',
   'A','R','G','U','M','E','N','T','S',0,'J',18,'R','D','B','$',
   'C','H','A','R','A','C','T','E','R','_','S','E','T','S',1,'J',
   14,'R','D','B','$','C','O','L','L','A','T','I','O','N','S',2,
   'G',58,47,23,0,20,'R','D','B','$','C','H','A','R','A','C','T',
   'E','R','_','S','E','T','_','I','D',23,1,20,'R','D','B','$',
   'C','H','A','R','A','C','T','E','R','_','S','E','T','_','I',
   'D',58,47,23,2,18,'R','D','B','$','C','O','L','L','A','T','I',
   'O','N','_','N','A','M','E',23,1,24,'R','D','B','$','D','E',
   'F','A','U','L','T','_','C','O','L','L','A','T','E','_','N',
   'A','M','E',58,59,61,23,0,20,'R','D','B','$','C','H','A','R',
   'A','C','T','E','R','_','S','E','T','_','I','D',58,47,23,0,17,
   'R','D','B','$','F','U','N','C','T','I','O','N','_','N','A',
   'M','E',25,0,0,0,47,23,0,21,'R','D','B','$','A','R','G','U',
   'M','E','N','T','_','P','O','S','I','T','I','O','N',25,0,1,0,
   -1,14,1,2,1,21,8,0,1,0,0,0,25,1,0,0,1,23,2,16,'R','D','B','$',
   'C','O','L','L','A','T','I','O','N','_','I','D',25,1,1,0,1,23,
   0,20,'R','D','B','$','C','H','A','R','A','C','T','E','R','_',
   'S','E','T','_','I','D',25,1,2,0,-1,14,1,1,21,8,0,0,0,0,0,25,
   1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_88 */

static const short
   isc_96l = 425;
static const char
   isc_96 [] = {
   4,2,4,0,10,0,41,3,0,32,0,41,3,0,32,0,41,3,0,32,0,7,0,7,0,7,0,
   7,0,7,0,7,0,7,0,2,7,'C',2,'J',13,'R','D','B','$','F','U','N',
   'C','T','I','O','N','S',0,'J',22,'R','D','B','$','F','U','N',
   'C','T','I','O','N','_','A','R','G','U','M','E','N','T','S',
   1,'G',58,47,23,0,17,'R','D','B','$','F','U','N','C','T','I',
   'O','N','_','N','A','M','E',23,1,17,'R','D','B','$','F','U',
   'N','C','T','I','O','N','_','N','A','M','E',47,23,0,19,'R','D',
   'B','$','R','E','T','U','R','N','_','A','R','G','U','M','E',
   'N','T',23,1,21,'R','D','B','$','A','R','G','U','M','E','N',
   'T','_','P','O','S','I','T','I','O','N',-1,14,0,2,1,23,1,17,
   'R','D','B','$','F','U','N','C','T','I','O','N','_','N','A',
   'M','E',25,0,0,0,1,23,0,14,'R','D','B','$','Q','U','E','R','Y',
   '_','N','A','M','E',25,0,1,0,1,23,0,17,'R','D','B','$','F','U',
   'N','C','T','I','O','N','_','N','A','M','E',25,0,2,0,1,21,8,
   0,1,0,0,0,25,0,3,0,1,23,1,21,'R','D','B','$','A','R','G','U',
   'M','E','N','T','_','P','O','S','I','T','I','O','N',25,0,4,0,
   1,23,1,14,'R','D','B','$','F','I','E','L','D','_','T','Y','P',
   'E',25,0,5,0,1,23,1,18,'R','D','B','$','F','I','E','L','D','_',
   'S','U','B','_','T','Y','P','E',25,0,6,0,1,23,1,15,'R','D','B',
   '$','F','I','E','L','D','_','S','C','A','L','E',25,0,7,0,1,23,
   1,16,'R','D','B','$','F','I','E','L','D','_','L','E','N','G',
   'T','H',25,0,8,0,1,23,0,17,'R','D','B','$','F','U','N','C','T',
   'I','O','N','_','T','Y','P','E',25,0,9,0,-1,14,0,1,21,8,0,0,
   0,0,0,25,0,3,0,-1,-1,'L'
   };	/* end of blr string for request isc_96 */

static const short
   isc_108l = 151;
static const char
   isc_108 [] = {
   4,2,4,0,5,0,41,3,0,32,0,41,3,0,32,0,7,0,7,0,7,0,2,7,'C',1,'J',
   14,'R','D','B','$','P','R','O','C','E','D','U','R','E','S',0,
   -1,14,0,2,1,23,0,14,'R','D','B','$','O','W','N','E','R','_',
   'N','A','M','E',41,0,0,0,3,0,1,23,0,18,'R','D','B','$','P','R',
   'O','C','E','D','U','R','E','_','N','A','M','E',25,0,1,0,1,21,
   8,0,1,0,0,0,25,0,2,0,1,23,0,16,'R','D','B','$','P','R','O','C',
   'E','D','U','R','E','_','I','D',25,0,4,0,-1,14,0,1,21,8,0,0,
   0,0,0,25,0,2,0,-1,-1,'L'
   };	/* end of blr string for request isc_108 */

static const short
   isc_115l = 174;
static const char
   isc_115 [] = {
   4,2,4,0,6,0,41,3,0,32,0,41,3,0,32,0,7,0,7,0,7,0,7,0,2,7,'C',
   1,'J',13,'R','D','B','$','R','E','L','A','T','I','O','N','S',
   0,-1,14,0,2,1,23,0,14,'R','D','B','$','O','W','N','E','R','_',
   'N','A','M','E',41,0,0,0,3,0,1,23,0,17,'R','D','B','$','R','E',
   'L','A','T','I','O','N','_','N','A','M','E',25,0,1,0,1,21,8,
   0,1,0,0,0,25,0,2,0,1,23,0,16,'R','D','B','$','D','B','K','E',
   'Y','_','L','E','N','G','T','H',25,0,4,0,1,23,0,15,'R','D','B',
   '$','R','E','L','A','T','I','O','N','_','I','D',25,0,5,0,-1,
   14,0,1,21,8,0,0,0,0,0,25,0,2,0,-1,-1,'L'
   };	/* end of blr string for request isc_115 */

static const short
   isc_123l = 129;
static const char
   isc_123 [] = {
   4,2,4,0,1,0,7,0,2,7,'C',1,'J',13,'R','D','B','$','R','E','L',
   'A','T','I','O','N','S',0,'G',58,47,23,0,17,'R','D','B','$',
   'R','E','L','A','T','I','O','N','_','N','A','M','E',21,14,14,
   0,'R','D','B','$','P','R','O','C','E','D','U','R','E','S',47,
   23,0,15,'R','D','B','$','S','Y','S','T','E','M','_','F','L',
   'A','G',21,8,0,1,0,0,0,-1,14,0,2,1,21,8,0,1,0,0,0,25,0,0,0,-1,
   14,0,1,21,8,0,0,0,0,0,25,0,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_123 */

static const short
   isc_126l = 124;
static const char
   isc_126 [] = {
   4,2,4,1,2,0,41,3,0,32,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',
   1,'J',11,'R','D','B','$','I','N','D','I','C','E','S',0,'G',47,
   23,0,14,'R','D','B','$','I','N','D','E','X','_','N','A','M',
   'E',25,0,0,0,-1,14,1,2,1,23,0,17,'R','D','B','$','R','E','L',
   'A','T','I','O','N','_','N','A','M','E',25,1,0,0,1,21,8,0,1,
   0,0,0,25,1,1,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,1,0,-1,-1,'L'
   };	/* end of blr string for request isc_126 */

static const short
   isc_132l = 159;
static const char
   isc_132 [] = {
   4,2,4,1,3,0,41,3,0,32,0,41,3,0,0,1,7,0,4,0,1,0,41,3,0,32,0,12,
   0,2,7,'C',1,'J',18,'R','D','B','$','V','I','E','W','_','R','E',
   'L','A','T','I','O','N','S',0,'G',47,23,0,13,'R','D','B','$',
   'V','I','E','W','_','N','A','M','E',25,0,0,0,-1,14,1,2,1,23,
   0,17,'R','D','B','$','R','E','L','A','T','I','O','N','_','N',
   'A','M','E',25,1,0,0,1,23,0,16,'R','D','B','$','C','O','N','T',
   'E','X','T','_','N','A','M','E',25,1,1,0,1,21,8,0,1,0,0,0,25,
   1,2,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,2,0,-1,-1,'L'
   };	/* end of blr string for request isc_132 */

static const short
   isc_139l = 409;
static const char
   isc_139 [] = {
   4,2,4,1,8,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,4,0,1,0,41,3,0,32,
   0,12,0,2,7,'C',2,'J',13,'R','D','B','$','F','U','N','C','T',
   'I','O','N','S',0,'J',22,'R','D','B','$','F','U','N','C','T',
   'I','O','N','_','A','R','G','U','M','E','N','T','S',1,'G',58,
   47,23,0,17,'R','D','B','$','F','U','N','C','T','I','O','N','_',
   'N','A','M','E',25,0,0,0,58,47,23,0,17,'R','D','B','$','F','U',
   'N','C','T','I','O','N','_','N','A','M','E',23,1,17,'R','D',
   'B','$','F','U','N','C','T','I','O','N','_','N','A','M','E',
   48,23,0,19,'R','D','B','$','R','E','T','U','R','N','_','A','R',
   'G','U','M','E','N','T',23,1,21,'R','D','B','$','A','R','G',
   'U','M','E','N','T','_','P','O','S','I','T','I','O','N','F',
   1,'I',23,1,21,'R','D','B','$','A','R','G','U','M','E','N','T',
   '_','P','O','S','I','T','I','O','N',-1,14,1,2,1,21,8,0,1,0,0,
   0,25,1,0,0,1,23,1,20,'R','D','B','$','C','H','A','R','A','C',
   'T','E','R','_','S','E','T','_','I','D',41,1,2,0,1,0,1,23,1,
   14,'R','D','B','$','F','I','E','L','D','_','T','Y','P','E',25,
   1,3,0,1,23,1,18,'R','D','B','$','F','I','E','L','D','_','S',
   'U','B','_','T','Y','P','E',25,1,4,0,1,23,1,15,'R','D','B','$',
   'F','I','E','L','D','_','S','C','A','L','E',25,1,5,0,1,23,1,
   16,'R','D','B','$','F','I','E','L','D','_','L','E','N','G','T',
   'H',25,1,6,0,1,23,1,21,'R','D','B','$','A','R','G','U','M','E',
   'N','T','_','P','O','S','I','T','I','O','N',25,1,7,0,-1,14,1,
   1,21,8,0,0,0,0,0,25,1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_139 */

static const short
   isc_151l = 322;
static const char
   isc_151 [] = {
   4,2,4,1,12,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,
   4,0,1,0,41,3,0,32,0,12,0,2,7,'C',1,'J',10,'R','D','B','$','F',
   'I','E','L','D','S',0,'G',47,25,0,0,0,23,0,14,'R','D','B','$',
   'F','I','E','L','D','_','N','A','M','E',-1,14,1,2,1,21,8,0,1,
   0,0,0,25,1,0,0,1,23,0,18,'R','D','B','$','S','E','G','M','E',
   'N','T','_','L','E','N','G','T','H',25,1,1,0,1,23,0,16,'R','D',
   'B','$','C','O','L','L','A','T','I','O','N','_','I','D',41,1,
   3,0,2,0,1,23,0,20,'R','D','B','$','C','H','A','R','A','C','T',
   'E','R','_','S','E','T','_','I','D',41,1,5,0,4,0,1,23,0,20,'R',
   'D','B','$','C','H','A','R','A','C','T','E','R','_','L','E',
   'N','G','T','H',41,1,7,0,6,0,1,23,0,14,'R','D','B','$','F','I',
   'E','L','D','_','T','Y','P','E',25,1,8,0,1,23,0,18,'R','D','B',
   '$','F','I','E','L','D','_','S','U','B','_','T','Y','P','E',
   25,1,9,0,1,23,0,15,'R','D','B','$','F','I','E','L','D','_','S',
   'C','A','L','E',25,1,10,0,1,23,0,16,'R','D','B','$','F','I',
   'E','L','D','_','L','E','N','G','T','H',25,1,11,0,-1,14,1,1,
   21,8,0,0,0,0,0,25,1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_151 */

static const short
   isc_167l = 256;
static const char
   isc_167 [] = {
   4,2,4,1,4,0,41,3,0,32,0,41,3,0,32,0,7,0,7,0,4,0,2,0,41,3,0,32,
   0,7,0,12,0,2,7,'C',1,'J',24,'R','D','B','$','P','R','O','C',
   'E','D','U','R','E','_','P','A','R','A','M','E','T','E','R',
   'S',0,'G',58,47,23,0,18,'R','D','B','$','P','R','O','C','E',
   'D','U','R','E','_','N','A','M','E',25,0,0,0,47,23,0,18,'R',
   'D','B','$','P','A','R','A','M','E','T','E','R','_','T','Y',
   'P','E',25,0,1,0,'F',1,'I',23,0,20,'R','D','B','$','P','A','R',
   'A','M','E','T','E','R','_','N','U','M','B','E','R',-1,14,1,
   2,1,23,0,18,'R','D','B','$','P','A','R','A','M','E','T','E',
   'R','_','N','A','M','E',25,1,0,0,1,23,0,16,'R','D','B','$','F',
   'I','E','L','D','_','S','O','U','R','C','E',25,1,1,0,1,21,8,
   0,1,0,0,0,25,1,2,0,1,23,0,20,'R','D','B','$','P','A','R','A',
   'M','E','T','E','R','_','N','U','M','B','E','R',25,1,3,0,-1,
   14,1,1,21,8,0,0,0,0,0,25,1,2,0,-1,-1,'L'
   };	/* end of blr string for request isc_167 */

static const short
   isc_176l = 96;
static const char
   isc_176 [] = {
   4,2,4,1,1,0,7,0,4,0,1,0,7,0,12,0,2,7,'C',1,'J',14,'R','D','B',
   '$','P','R','O','C','E','D','U','R','E','S',0,'G',47,23,0,16,
   'R','D','B','$','P','R','O','C','E','D','U','R','E','_','I',
   'D',25,0,0,0,-1,14,1,2,1,21,8,0,1,0,0,0,25,1,0,0,-1,14,1,1,21,
   8,0,0,0,0,0,25,1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_176 */

static const short
   isc_181l = 207;
static const char
   isc_181 [] = {
   4,2,4,1,2,0,41,3,0,32,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',
   2,'J',19,'R','D','B','$','R','E','L','A','T','I','O','N','_',
   'F','I','E','L','D','S',0,'J',10,'R','D','B','$','F','I','E',
   'L','D','S',1,'G',58,47,23,0,16,'R','D','B','$','F','I','E',
   'L','D','_','S','O','U','R','C','E',23,1,14,'R','D','B','$',
   'F','I','E','L','D','_','N','A','M','E',47,23,0,17,'R','D','B',
   '$','R','E','L','A','T','I','O','N','_','N','A','M','E',25,0,
   0,0,'F',1,'H',23,0,18,'R','D','B','$','F','I','E','L','D','_',
   'P','O','S','I','T','I','O','N',-1,14,1,2,1,23,0,14,'R','D',
   'B','$','F','I','E','L','D','_','N','A','M','E',25,1,0,0,1,21,
   8,0,1,0,0,0,25,1,1,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,1,0,-1,-1,
   'L'
   };	/* end of blr string for request isc_181 */

static const short
   isc_187l = 573;
static const char
   isc_187 [] = {
   4,2,4,1,19,0,41,3,0,32,0,41,3,0,32,0,7,0,7,0,7,0,7,0,7,0,7,0,
   7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,4,0,2,0,41,3,0,32,
   0,41,3,0,32,0,12,0,2,7,'C',2,'J',19,'R','D','B','$','R','E',
   'L','A','T','I','O','N','_','F','I','E','L','D','S',0,'J',10,
   'R','D','B','$','F','I','E','L','D','S',1,'G',58,47,23,0,16,
   'R','D','B','$','F','I','E','L','D','_','S','O','U','R','C',
   'E',23,1,14,'R','D','B','$','F','I','E','L','D','_','N','A',
   'M','E',58,47,23,0,14,'R','D','B','$','F','I','E','L','D','_',
   'N','A','M','E',25,0,1,0,47,23,0,17,'R','D','B','$','R','E',
   'L','A','T','I','O','N','_','N','A','M','E',25,0,0,0,-1,14,1,
   2,1,23,0,16,'R','D','B','$','F','I','E','L','D','_','S','O',
   'U','R','C','E',25,1,0,0,1,23,1,14,'R','D','B','$','F','I','E',
   'L','D','_','N','A','M','E',25,1,1,0,1,21,8,0,1,0,0,0,25,1,2,
   0,1,23,1,16,'R','D','B','$','C','O','L','L','A','T','I','O',
   'N','_','I','D',41,1,4,0,3,0,1,23,0,16,'R','D','B','$','C','O',
   'L','L','A','T','I','O','N','_','I','D',41,1,6,0,5,0,1,23,1,
   20,'R','D','B','$','C','H','A','R','A','C','T','E','R','_','S',
   'E','T','_','I','D',41,1,8,0,7,0,1,23,1,20,'R','D','B','$','C',
   'H','A','R','A','C','T','E','R','_','L','E','N','G','T','H',
   41,1,10,0,9,0,1,23,1,14,'R','D','B','$','D','I','M','E','N',
   'S','I','O','N','S',25,1,11,0,1,23,1,18,'R','D','B','$','S',
   'E','G','M','E','N','T','_','L','E','N','G','T','H',25,1,12,
   0,1,23,1,14,'R','D','B','$','F','I','E','L','D','_','T','Y',
   'P','E',25,1,13,0,1,23,1,18,'R','D','B','$','F','I','E','L',
   'D','_','S','U','B','_','T','Y','P','E',25,1,14,0,1,23,0,18,
   'R','D','B','$','F','I','E','L','D','_','P','O','S','I','T',
   'I','O','N',25,1,15,0,1,23,1,15,'R','D','B','$','F','I','E',
   'L','D','_','S','C','A','L','E',25,1,16,0,1,23,1,16,'R','D',
   'B','$','F','I','E','L','D','_','L','E','N','G','T','H',25,1,
   17,0,1,23,0,12,'R','D','B','$','F','I','E','L','D','_','I','D',
   25,1,18,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,2,0,-1,-1,'L'
   };	/* end of blr string for request isc_187 */

static const short
   isc_211l = 307;
static const char
   isc_211 [] = {
   4,2,4,1,2,0,41,3,0,32,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',
   3,'J',11,'R','D','B','$','I','N','D','I','C','E','S',0,'J',18,
   'R','D','B','$','I','N','D','E','X','_','S','E','G','M','E',
   'N','T','S',1,'J',24,'R','D','B','$','R','E','L','A','T','I',
   'O','N','_','C','O','N','S','T','R','A','I','N','T','S',2,'G',
   58,58,47,23,1,14,'R','D','B','$','I','N','D','E','X','_','N',
   'A','M','E',23,0,14,'R','D','B','$','I','N','D','E','X','_',
   'N','A','M','E',47,23,2,14,'R','D','B','$','I','N','D','E','X',
   '_','N','A','M','E',23,1,14,'R','D','B','$','I','N','D','E',
   'X','_','N','A','M','E',58,47,23,2,17,'R','D','B','$','R','E',
   'L','A','T','I','O','N','_','N','A','M','E',25,0,0,0,47,23,2,
   19,'R','D','B','$','C','O','N','S','T','R','A','I','N','T','_',
   'T','Y','P','E',21,14,11,0,'P','R','I','M','A','R','Y',32,'K',
   'E','Y','F',1,'H',23,1,18,'R','D','B','$','F','I','E','L','D',
   '_','P','O','S','I','T','I','O','N',-1,14,1,2,1,23,1,14,'R',
   'D','B','$','F','I','E','L','D','_','N','A','M','E',25,1,0,0,
   1,21,8,0,1,0,0,0,25,1,1,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,1,0,
   -1,-1,'L'
   };	/* end of blr string for request isc_211 */

static const short
   isc_217l = 246;
static const char
   isc_217 [] = {
   4,2,4,1,5,0,9,0,9,0,7,0,7,0,7,0,4,0,2,0,41,3,0,32,0,41,3,0,32,
   0,12,0,2,7,'C',2,'J',19,'R','D','B','$','R','E','L','A','T',
   'I','O','N','_','F','I','E','L','D','S',0,'J',10,'R','D','B',
   '$','F','I','E','L','D','S',1,'G',58,47,23,0,16,'R','D','B',
   '$','F','I','E','L','D','_','S','O','U','R','C','E',23,1,14,
   'R','D','B','$','F','I','E','L','D','_','N','A','M','E',58,47,
   23,0,14,'R','D','B','$','F','I','E','L','D','_','N','A','M',
   'E',25,0,1,0,47,23,0,17,'R','D','B','$','R','E','L','A','T',
   'I','O','N','_','N','A','M','E',25,0,0,0,-1,14,1,2,1,23,1,17,
   'R','D','B','$','D','E','F','A','U','L','T','_','V','A','L',
   'U','E',41,1,0,0,3,0,1,23,0,17,'R','D','B','$','D','E','F','A',
   'U','L','T','_','V','A','L','U','E',41,1,1,0,4,0,1,21,8,0,1,
   0,0,0,25,1,2,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,2,0,-1,-1,'L'
   };	/* end of blr string for request isc_217 */

static const short
   isc_227l = 124;
static const char
   isc_227 [] = {
   4,2,4,1,3,0,9,0,7,0,7,0,4,0,1,0,41,3,0,32,0,12,0,2,7,'C',1,'J',
   10,'R','D','B','$','F','I','E','L','D','S',0,'G',47,23,0,14,
   'R','D','B','$','F','I','E','L','D','_','N','A','M','E',25,0,
   0,0,-1,14,1,2,1,23,0,17,'R','D','B','$','D','E','F','A','U',
   'L','T','_','V','A','L','U','E',41,1,0,0,2,0,1,21,8,0,1,0,0,
   0,25,1,1,0,-1,14,1,1,21,8,0,0,0,0,0,25,1,1,0,-1,-1,'L'
   };	/* end of blr string for request isc_227 */

static const short
   isc_234l = 322;
static const char
   isc_234 [] = {
   4,2,4,1,12,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,
   4,0,1,0,41,3,0,32,0,12,0,2,7,'C',1,'J',10,'R','D','B','$','F',
   'I','E','L','D','S',0,'G',47,23,0,14,'R','D','B','$','F','I',
   'E','L','D','_','N','A','M','E',25,0,0,0,-1,14,1,2,1,21,8,0,
   1,0,0,0,25,1,0,0,1,23,0,16,'R','D','B','$','C','O','L','L','A',
   'T','I','O','N','_','I','D',41,1,2,0,1,0,1,23,0,20,'R','D','B',
   '$','C','H','A','R','A','C','T','E','R','_','S','E','T','_',
   'I','D',41,1,4,0,3,0,1,23,0,20,'R','D','B','$','C','H','A','R',
   'A','C','T','E','R','_','L','E','N','G','T','H',41,1,6,0,5,0,
   1,23,0,18,'R','D','B','$','S','E','G','M','E','N','T','_','L',
   'E','N','G','T','H',25,1,7,0,1,23,0,14,'R','D','B','$','F','I',
   'E','L','D','_','T','Y','P','E',25,1,8,0,1,23,0,18,'R','D','B',
   '$','F','I','E','L','D','_','S','U','B','_','T','Y','P','E',
   25,1,9,0,1,23,0,15,'R','D','B','$','F','I','E','L','D','_','S',
   'C','A','L','E',25,1,10,0,1,23,0,16,'R','D','B','$','F','I',
   'E','L','D','_','L','E','N','G','T','H',25,1,11,0,-1,14,1,1,
   21,8,0,0,0,0,0,25,1,0,0,-1,-1,'L'
   };	/* end of blr string for request isc_234 */


#define gds_blob_null	isc_blob_null	/* compatibility symbols */
#define gds_status	isc_status
#define gds_status2	isc_status2
#define gds_array_length	isc_array_length
#define gds_count	isc_count
#define gds_slack	isc_slack
#define gds_utility	isc_utility	/* end of compatibility symbols */

#ifndef isc_version4
    Generate a compile-time error.
    Picking up a V3 include file after preprocessing with V4 GPRE.
#endif

/**** end of GPRE definitions ****/


static const UCHAR blr_bpb[] =
{
	isc_bpb_version1,
	isc_bpb_source_type, 1, isc_blob_blr,
	isc_bpb_target_type, 1, isc_blob_blr
};

#ifdef SCROLLABLE_CURSORS
static const SCHAR db_version_info[] = { isc_info_base_level };
#endif

// CVC: I made this class as a quick solution for the many buffer overruns
// that would result in erratic behavior.
class MetIdentifier
{
	char name[NAME_SIZE];
public:
	explicit MetIdentifier(const char* s)
	{
		strncpy(name, s, NAME_SIZE);
		name[NAME_SIZE - 1] = 0;
	}
	operator char*() {return name;}
	operator const char*() const {return name;}
};


static SLONG array_size(const gpre_fld*);
static void get_array(gpre_dbb*, const TEXT*, gpre_fld*);
static bool get_intl_char_subtype(SSHORT*, const UCHAR*, USHORT, gpre_dbb*);
static bool resolve_charset_and_collation(SSHORT*, const UCHAR*, const UCHAR*);
#ifdef NOT_USED_OR_REPLACED
static int upcase(const TEXT*, TEXT* const);
#endif

/*____________________________________________________________
 *
 *		Lookup a field by name in a context.
 *		If found, return field block.  If not, return NULL.
 */

gpre_fld* MET_context_field( gpre_ctx* context, const char* string)
{
	if (context->ctx_relation) {
		return MET_field(context->ctx_relation, string);
	}

	if (!context->ctx_procedure) {
		return NULL;
	}

	const MetIdentifier name(string);
	gpre_prc* procedure = context->ctx_procedure;

	// At this point the procedure should have been scanned, so all
	// its fields are in the symbol table.

	gpre_fld* field = NULL;
	for (gpre_sym* symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
	{
		if (symbol->sym_type == SYM_field && (field = (gpre_fld*) symbol->sym_object) &&
			field->fld_procedure == procedure)
		{
			return field;
		}
	}

	return field;
}


/*____________________________________________________________
 *
 *		Initialize meta data access to database.  If the
 *		database can't be opened, return false.
 */

bool MET_database(gpre_dbb* database, bool print_version)
{
	static const UCHAR sql_version_info[] =
	{
		isc_info_base_level,
		isc_info_ods_version,
		isc_info_db_sql_dialect,
		isc_info_end
	};

	// Each info item requested will return
	//
	//     1 byte for the info item tag
	//     2 bytes for the length of the information that follows
	//     1 to 4 bytes of integer information
	//
	// isc_info_end will not have a 2-byte length - which gives us
	// some padding in the buffer.

	UCHAR sql_buffer[sizeof(sql_version_info) * (1 + 2 + 4)];

	if (gpreGlob.sw_language == lang_internal)
	{
		JRDMET_init(database);
		return true;
	}

	DB = 0;

	if (!database->dbb_filename)
	{
		CPR_error("No database specified");
		return false;
	}

	// generate a dpb for the attach from the specified
	// compiletime user name and password

	Firebird::ClumpletWriter dpb(Firebird::ClumpletReader::Tagged, MAX_DPB_SIZE, isc_dpb_version1);

	const TEXT* p;
	if ((p = database->dbb_c_user)) {
		dpb.insertString(isc_dpb_user_name, p, strlen(p));
	}
	if ((p = database->dbb_c_password)) {
		dpb.insertString(isc_dpb_password, p, strlen(p));
	}
#ifdef TRUSTED_AUTH
	if (gpreGlob.trusted_auth) {
		dpb.insertTag(isc_dpb_trusted_auth);
	}
#endif

	if (isc_attach_database(gds_status, 0, database->dbb_filename, &DB,
							dpb.getBufferLength(),
							reinterpret_cast<const char*>(dpb.getBuffer())))
	{
		// We failed to attach, try in read only mode just in case
		//dpb.insertByte(isc_dpb_set_db_readonly, true);
		//if (isc_attach_database(gds_status, 0, database->dbb_filename, &DB,
		//						dpb.getBufferLength(),
		//						reinterpret_cast<const char*>(dpb.getBuffer())))
		//{
			isc_print_status(gds_status);
			return false;
		//}
	}

	database->dbb_handle = DB;

	if (gpreGlob.sw_version && print_version)
	{
		printf("    Version(s) for database \"%s\"\n", database->dbb_filename);
		isc_version(&DB, NULL, NULL);
	}

	gpreGlob.sw_ods_version = 0;
	gpreGlob.sw_server_version = 0;
	if (isc_database_info(isc_status, &DB, sizeof(sql_version_info),
		reinterpret_cast<const char*>(sql_version_info), sizeof(sql_buffer),
		reinterpret_cast<char*>(sql_buffer)))
	{
		isc_print_status(isc_status);
		return false;
	}

	const UCHAR* ptr = sql_buffer;
	while (*ptr != isc_info_end)
	{
		const UCHAR item = *ptr++;
		const USHORT length = gds__vax_integer(ptr, sizeof(USHORT));
		ptr += sizeof(USHORT);

		switch (item)
		{
		case isc_info_base_level:
			gpreGlob.sw_server_version = (USHORT) ptr[1];
			break;

		case isc_info_ods_version:
			gpreGlob.sw_ods_version = gds__vax_integer(ptr, length);
			break;

		case isc_info_db_sql_dialect:
			gpreGlob.compiletime_db_dialect = gds__vax_integer(ptr, length);
			break;

		case isc_info_error:
			// Error indicates that one of the  option
			// was not understood by the server. Make sure it was
			// isc_info_db_sql_dialect and assume
			// that the database dialect is SQL_DIALECT_V5
			if ((gpreGlob.sw_server_version != 0) && (gpreGlob.sw_ods_version != 0))
				gpreGlob.compiletime_db_dialect = SQL_DIALECT_V5;
			else
				printf("Internal error: Unexpected isc_info_value %d\n", item);
			break;

		default:
			printf("Internal error: Unexpected isc_info_value %d\n", item);
			break;
		}
		ptr += length;
	}

	if (!gpreGlob.dialect_specified)
		gpreGlob.sw_sql_dialect = gpreGlob.compiletime_db_dialect;

	if (gpreGlob.sw_ods_version < 8)
	{
		CPR_error(NEED_IB4_AT_LEAST);
		CPR_abort();
	}

	if (gpreGlob.sw_ods_version < 10)
	{
		if (gpreGlob.sw_sql_dialect != gpreGlob.compiletime_db_dialect)
		{
			char warn_mesg[100];
			sprintf(warn_mesg, "Pre 6.0 database. Cannot use dialect %d, Resetting to %d\n",
					gpreGlob.sw_sql_dialect, gpreGlob.compiletime_db_dialect);
			CPR_warn(warn_mesg);
		}
		gpreGlob.sw_sql_dialect = gpreGlob.compiletime_db_dialect;
	}
	else if (gpreGlob.sw_sql_dialect != gpreGlob.compiletime_db_dialect)
	{
		char warn_mesg[100];
		sprintf(warn_mesg, "Client dialect set to %d. Compiletime database dialect is %d\n",
				gpreGlob.sw_sql_dialect, gpreGlob.compiletime_db_dialect);
		CPR_warn(warn_mesg);
	}

#ifdef DEBUG
	if (gpreGlob.sw_version && print_version)
		printf("Gpre Start up values. \
	\n\tServer Ver                   : %d\
        \n\tCompile time db Ver          : %d\
	\n\tCompile time db Sql Dialect  : %d\
	\n\tClient Sql dialect set to    : %d\n\n",
	gpreGlob.sw_server_version, gpreGlob.sw_ods_version, gpreGlob.compiletime_db_dialect, gpreGlob.sw_sql_dialect);
#endif

#ifdef SCROLLABLE_CURSORS

	SCHAR buffer[16];

	// get the base level of the engine

	if (isc_database_info(gds_status, &DB, sizeof(db_version_info),
						  db_version_info, sizeof(buffer), buffer))
	{
		isc_print_status(gds_status);
		return false;
	}

	// this seems like a lot of rigamarole to read one info item,
	// but it provides for easy extensibility in case we need
	// more info items later

	const SCHAR* data = buffer;
	while (*data != isc_info_end)
	{
	    const SCHAR item = *data++;
		const USHORT l = isc_vax_integer(data, sizeof(USHORT));
		data += sizeof(USHORT);

		switch (item)
		{
			// This flag indicates the version level of the engine
			// itself, so we can tell what capabilities the engine
			// code itself (as opposed to the on-disk structure).
			// Apparently the base level up to now indicated the major
			// version number, but for 4.1 the base level is being
			// incremented, so the base level indicates an engine version
			// as follows:
			// 1 == v1.x
			// 2 == v2.x
			// 3 == v3.x
			// 4 == v4.0 only
			// 5 == v4.1. (v5, too)
			// 6 == v6, FB1
			// Note: this info item is so old it apparently uses an
			// archaic format, not a standard vax integer format.

		case isc_info_base_level:
			fb_assert(l == 2 && data[0] == UCHAR(1));
			database->dbb_base_level = (USHORT) data[1];
			break;

		default:
			;
		}

		data += l;
	}
#endif

	return true;
}


/*____________________________________________________________
 *
 *		Lookup a domain by name.
 *		Initialize the size of the field.
 */

bool MET_domain_lookup(gpre_req* request, gpre_fld* field, const char* string)
{
   struct isc_237_struct {
          short isc_238;	/* isc_utility */
          short isc_239;	/* gds__null_flag */
          short isc_240;	/* RDB$COLLATION_ID */
          short isc_241;	/* gds__null_flag */
          short isc_242;	/* RDB$CHARACTER_SET_ID */
          short isc_243;	/* gds__null_flag */
          short isc_244;	/* RDB$CHARACTER_LENGTH */
          short isc_245;	/* RDB$SEGMENT_LENGTH */
          short isc_246;	/* RDB$FIELD_TYPE */
          short isc_247;	/* RDB$FIELD_SUB_TYPE */
          short isc_248;	/* RDB$FIELD_SCALE */
          short isc_249;	/* RDB$FIELD_LENGTH */
   } isc_237;
   struct isc_235_struct {
          char  isc_236 [32];	/* RDB$FIELD_NAME */
   } isc_235;
	const MetIdentifier name(string);
	bool found = false;

	// Lookup domain.  If we find it in the hash table, and it is not the
	// field we a currently looking at, use it.  Else look it up from the
	// database.

	for (gpre_sym* symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
	{
		gpre_fld* d_field;
		if ((symbol->sym_type == SYM_field) &&
			((d_field = (gpre_fld*) symbol->sym_object) && (d_field != field)))
		{
			field->fld_length = d_field->fld_length;
			field->fld_scale = d_field->fld_scale;
			field->fld_sub_type = d_field->fld_sub_type;
			field->fld_dtype = d_field->fld_dtype;
			field->fld_ttype = d_field->fld_ttype;
			field->fld_charset_id = d_field->fld_charset_id;
			field->fld_collate_id = d_field->fld_collate_id;
			field->fld_char_length = d_field->fld_char_length;
			return true;
		}
	}

	if (!request)
		return false;

	gpre_dbb* database = request->req_database;
	if (database->dbb_flags & DBB_sqlca)
		return false;

	DB = database->dbb_handle;
	gds_trans = database->dbb_transaction;

	/*FOR(REQUEST_HANDLE database->dbb_domain_request)
	F IN RDB$FIELDS WITH F.RDB$FIELD_NAME EQ name*/
	{
        if (!database->dbb_domain_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_domain_request, (short) sizeof(isc_234), (char*) isc_234);
	isc_vtov ((const char*) name, (char*) isc_235.isc_236, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_domain_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_235, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_domain_request, (short) 1, (short) 24, &isc_237, (short) 0);
	   if (!isc_237.isc_238) break;
		found = true;
		field->fld_length = /*F.RDB$FIELD_LENGTH*/
				    isc_237.isc_249;
		field->fld_scale = /*F.RDB$FIELD_SCALE*/
				   isc_237.isc_248;
		field->fld_sub_type = /*F.RDB$FIELD_SUB_TYPE*/
				      isc_237.isc_247;
		field->fld_dtype =
			MET_get_dtype(/*F.RDB$FIELD_TYPE*/
				      isc_237.isc_246, /*F.RDB$FIELD_SUB_TYPE*/
  isc_237.isc_247, &field->fld_length);

		switch (field->fld_dtype)
		{
		case dtype_text:
		case dtype_cstring:
			field->fld_flags |= FLD_text;
			break;

		case dtype_blob:
			field->fld_flags |= FLD_blob;
			field->fld_seg_length = /*F.RDB$SEGMENT_LENGTH*/
						isc_237.isc_245;
			break;
		}

		if (field->fld_dtype <= dtype_any_text)
		{
			if (!/*F.RDB$CHARACTER_LENGTH.NULL*/
			     isc_237.isc_243)
				field->fld_char_length = /*F.RDB$CHARACTER_LENGTH*/
							 isc_237.isc_244;
			if (!/*F.RDB$CHARACTER_SET_ID.NULL*/
			     isc_237.isc_241)
				field->fld_charset_id = /*F.RDB$CHARACTER_SET_ID*/
							isc_237.isc_242;
			if (!/*F.RDB$COLLATION_ID.NULL*/
			     isc_237.isc_239)
				field->fld_collate_id = /*F.RDB$COLLATION_ID*/
							isc_237.isc_240;
		}

		field->fld_ttype = INTL_CS_COLL_TO_TTYPE(field->fld_charset_id, field->fld_collate_id);

	/*END_FOR;*/
	   }
	}

	return found;
}


/*____________________________________________________________
 *
 *		Gets the default value for a domain of an existing table
 */

bool MET_get_domain_default(gpre_dbb* database,
							const TEXT* domain_name,
							TEXT* buffer,
							USHORT buff_length)
{
   struct isc_230_struct {
          ISC_QUAD isc_231;	/* RDB$DEFAULT_VALUE */
          short isc_232;	/* isc_utility */
          short isc_233;	/* gds__null_flag */
   } isc_230;
   struct isc_228_struct {
          char  isc_229 [32];	/* RDB$FIELD_NAME */
   } isc_228;
	if (database == NULL)
		return false;

	if (database->dbb_handle == 0 && !MET_database(database, false))
		CPR_exit(FINI_ERROR);

	fb_assert(database->dbb_transaction == 0);
	const MetIdentifier name(domain_name);
	gds_trans = 0;
	DB = database->dbb_handle;
	ISC_STATUS_ARRAY status_vect;

	/*START_TRANSACTION;*/
	{
	{
	isc_start_transaction (NULL, (FB_API_HANDLE*) &gds_trans, (short) 1, &DB, (short) 0, (char*) 0);
	}
	}

	bool has_default = false;
	/*FOR(REQUEST_HANDLE database->dbb_domain_request)
	GLOB_FLD IN RDB$FIELDS WITH
		GLOB_FLD.RDB$FIELD_NAME EQ name*/
	{
        if (!database->dbb_domain_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_domain_request, (short) sizeof(isc_227), (char*) isc_227);
	isc_vtov ((const char*) name, (char*) isc_228.isc_229, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_domain_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_228, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_domain_request, (short) 1, (short) 12, &isc_230, (short) 0);
	   if (!isc_230.isc_232) break;
		if (!/*GLOB_FLD.RDB$DEFAULT_VALUE.NULL*/
		     isc_230.isc_233)
		{
			ISC_QUAD* blob_id = &/*GLOB_FLD.RDB$DEFAULT_VALUE*/
					     isc_230.isc_231;

			// open the blob
			isc_blob_handle blob_handle = 0;
			ISC_STATUS stat = isc_open_blob2(status_vect, &DB, &gds_trans,
									&blob_handle, blob_id, sizeof(blr_bpb), blr_bpb);
			if (stat)
			{
				isc_print_status(status_vect);
				CPR_exit(FINI_ERROR);
			}
			// fetch segments. Assume buffer is big enough.
			TEXT* ptr_in_buffer = buffer;
			while (true)
			{
				SSHORT length;
				stat = isc_get_segment(status_vect, &blob_handle, (USHORT*) &length,
									   buff_length, ptr_in_buffer);
				ptr_in_buffer = ptr_in_buffer + length;
				buff_length = buff_length - length;

				if (!stat)
					continue;
				if (stat == isc_segstr_eof)
				{
					// null terminate the buffer
					*ptr_in_buffer = 0;
					break;
				}
				CPR_exit(FINI_ERROR);
			}
			isc_close_blob(status_vect, &blob_handle);

			// the default string must be of the form:
			// blr_version4 blr_literal ..... blr_eoc OR
			// blr_version4 blr_null blr_eoc OR
			// blr_version4 blr_user_name  blr_eoc
			fb_assert(buffer[0] == blr_version4 || buffer[0] == blr_version5);
			fb_assert(buffer[1] == blr_literal || buffer[1] == blr_null || buffer[1] == blr_user_name);

			has_default = true;
		}
		else
		{						// default not found

			if (gpreGlob.sw_sql_dialect > SQL_DIALECT_V5)
				buffer[0] = blr_version5;
			else
				buffer[0] = blr_version4;
			buffer[1] = blr_eoc;
		}
	/*END_FOR*/
	   }
	}
	/*COMMIT;*/
	{
	isc_commit_transaction (NULL, (FB_API_HANDLE*) &gds_trans);
	}
	return has_default;
}


/*____________________________________________________________
 *
 *		Gets the default value for a column of an existing table.
 *		Will check the default for the column of the table, if that is
 *		not present, will check for the default of the relevant domain
 *
 *		The default blr is returned in buffer. The blr is of the form
 *		blr_version4 blr_literal ..... blr_eoc
 *
 *		Reads the system tables RDB$FIELDS and RDB$RELATION_FIELDS.
 */

bool MET_get_column_default(const gpre_rel* relation,
							const TEXT* column_name,
							TEXT* buffer,
							USHORT buff_length)
{
   struct isc_221_struct {
          ISC_QUAD isc_222;	/* RDB$DEFAULT_VALUE */
          ISC_QUAD isc_223;	/* RDB$DEFAULT_VALUE */
          short isc_224;	/* isc_utility */
          short isc_225;	/* gds__null_flag */
          short isc_226;	/* gds__null_flag */
   } isc_221;
   struct isc_218_struct {
          char  isc_219 [32];	/* RDB$RELATION_NAME */
          char  isc_220 [32];	/* RDB$FIELD_NAME */
   } isc_218;
	const MetIdentifier name(column_name);

	gpre_dbb* database = relation->rel_database;
	if (database == NULL)
		return false;

	if (database->dbb_handle == 0 && !MET_database(database, false))
		CPR_exit(FINI_ERROR);

	fb_assert(database->dbb_transaction == 0);
	gds_trans = 0;
	DB = database->dbb_handle;
	ISC_STATUS_ARRAY status_vect;

	/*START_TRANSACTION;*/
	{
	{
	isc_start_transaction (NULL, (FB_API_HANDLE*) &gds_trans, (short) 1, &DB, (short) 0, (char*) 0);
	}
	}

	bool has_default = false;
	/*FOR(REQUEST_HANDLE database->dbb_field_request)
	RFR IN RDB$RELATION_FIELDS CROSS F IN RDB$FIELDS WITH
		RFR.RDB$FIELD_SOURCE EQ F.RDB$FIELD_NAME AND
		RFR.RDB$FIELD_NAME EQ name AND
		RFR.RDB$RELATION_NAME EQ relation->rel_symbol->sym_string*/
	{
        if (!database->dbb_field_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_field_request, (short) sizeof(isc_217), (char*) isc_217);
	isc_vtov ((const char*) relation->rel_symbol->sym_string, (char*) isc_218.isc_219, 32);
	isc_vtov ((const char*) name, (char*) isc_218.isc_220, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_field_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 64, &isc_218, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_field_request, (short) 1, (short) 22, &isc_221, (short) 0);
	   if (!isc_221.isc_224) break;

		ISC_QUAD* blob_id;
		if (!/*RFR.RDB$DEFAULT_VALUE.NULL*/
		     isc_221.isc_226)
		{
			blob_id = &/*RFR.RDB$DEFAULT_VALUE*/
				   isc_221.isc_223;
			has_default = true;
		}
		else if (!/*F.RDB$DEFAULT_VALUE.NULL*/
			  isc_221.isc_225)
		{
			blob_id = &/*F.RDB$DEFAULT_VALUE*/
				   isc_221.isc_222;
			has_default = true;
		}
		else {
			blob_id = NULL; // silence non initialized warning
		}

		if (has_default)
		{
			// open the blob
			isc_blob_handle blob_handle = 0;
			ISC_STATUS stat = isc_open_blob2(status_vect, &DB, &gds_trans,
								  &blob_handle, blob_id, sizeof(blr_bpb), blr_bpb);
			if (stat)
			{
				isc_print_status(status_vect);
				CPR_exit(FINI_ERROR);
			}

			// fetch segments. Assuming here that the buffer is big enough.
			TEXT* ptr_in_buffer = buffer;
			while (true)
			{
				SSHORT length;
				stat = isc_get_segment(status_vect, &blob_handle,
									   (USHORT*)&length, buff_length, ptr_in_buffer);
				ptr_in_buffer = ptr_in_buffer + length;
				buff_length = buff_length - length;
				if (!stat)
					continue;
				if (stat == isc_segstr_eof)
				{
					// null terminate the buffer
					*ptr_in_buffer = 0;
					break;
				}
				CPR_exit(FINI_ERROR);
			}
			isc_close_blob(status_vect, &blob_handle);

			// the default string must be of the form:
			// blr_version4 blr_literal ..... blr_eoc OR
			// blr_version4 blr_null blr_eoc OR
			// blr_version4 blr_user_name blr_eoc

			fb_assert(buffer[0] == blr_version4 || buffer[0] == blr_version5);
			fb_assert(buffer[1] == blr_literal || buffer[1] == blr_null || buffer[1] == blr_user_name);
		}
		else
		{
			if (gpreGlob.sw_sql_dialect > SQL_DIALECT_V5)
				buffer[0] = blr_version5;
			else
				buffer[0] = blr_version4;
			buffer[1] = blr_eoc;
		}

	/*END_FOR;*/
	   }
	}
	/*COMMIT;*/
	{
	isc_commit_transaction (NULL, (FB_API_HANDLE*) &gds_trans);
	}
	return has_default;
}


/*____________________________________________________________
 *
 *		Lookup the fields for the primary key
 *		index on a relation, returning a list
 *		of the fields.
 */

gpre_lls* MET_get_primary_key(gpre_dbb* database, const TEXT* relation_name)
{
   struct isc_214_struct {
          char  isc_215 [32];	/* RDB$FIELD_NAME */
          short isc_216;	/* isc_utility */
   } isc_214;
   struct isc_212_struct {
          char  isc_213 [32];	/* RDB$RELATION_NAME */
   } isc_212;
	const MetIdentifier name(relation_name);

	if (database == NULL)
		return NULL;

	if (database->dbb_handle == 0 && !MET_database(database, false))
		CPR_exit(FINI_ERROR);

	fb_assert(database->dbb_transaction == 0);
	gds_trans = 0;
	DB = database->dbb_handle;

	/*START_TRANSACTION;*/
	{
	{
	isc_start_transaction (NULL, (FB_API_HANDLE*) &gds_trans, (short) 1, &DB, (short) 0, (char*) 0);
	}
	}

    gpre_lls* fields = NULL;
	gpre_lls** ptr_fields = &fields;

	/*FOR(REQUEST_HANDLE database->dbb_primary_key_request)
		X IN RDB$INDICES CROSS
		Y IN RDB$INDEX_SEGMENTS
		OVER RDB$INDEX_NAME CROSS
		Z IN RDB$RELATION_CONSTRAINTS
		OVER RDB$INDEX_NAME
		WITH Z.RDB$RELATION_NAME EQ name
		AND Z.RDB$CONSTRAINT_TYPE EQ "PRIMARY KEY"
		SORTED BY Y.RDB$FIELD_POSITION*/
	{
        if (!database->dbb_primary_key_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_primary_key_request, (short) sizeof(isc_211), (char*) isc_211);
	isc_vtov ((const char*) name, (char*) isc_212.isc_213, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_primary_key_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_212, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_primary_key_request, (short) 1, (short) 34, &isc_214, (short) 0);
	   if (!isc_214.isc_216) break;

		str* field_name = (str*) MSC_string(/*Y.RDB$FIELD_NAME*/
						    isc_214.isc_215);
		// Strip off any trailing spaces from field name
		// CVC: This code stops at the first space, even in the middle.
		TEXT* tmp = (TEXT*) field_name;
		while (*tmp && *tmp != ' ')
			*tmp++;
		*tmp = '\0';

		MSC_push((gpre_nod*) field_name, ptr_fields);
		ptr_fields = &(*ptr_fields)->lls_next;

	/*END_FOR*/
	   }
	}
	/*COMMIT;*/
	{
	isc_commit_transaction (NULL, (FB_API_HANDLE*) &gds_trans);
	}

	return fields;
}


/*____________________________________________________________
 *
 *		Lookup a field by name in a relation.
 *		If found, return field block.  If not, return NULL.
 */

gpre_fld* MET_field(gpre_rel* relation, const char* string)
{
   struct isc_191_struct {
          char  isc_192 [32];	/* RDB$FIELD_SOURCE */
          char  isc_193 [32];	/* RDB$FIELD_NAME */
          short isc_194;	/* isc_utility */
          short isc_195;	/* gds__null_flag */
          short isc_196;	/* RDB$COLLATION_ID */
          short isc_197;	/* gds__null_flag */
          short isc_198;	/* RDB$COLLATION_ID */
          short isc_199;	/* gds__null_flag */
          short isc_200;	/* RDB$CHARACTER_SET_ID */
          short isc_201;	/* gds__null_flag */
          short isc_202;	/* RDB$CHARACTER_LENGTH */
          short isc_203;	/* RDB$DIMENSIONS */
          short isc_204;	/* RDB$SEGMENT_LENGTH */
          short isc_205;	/* RDB$FIELD_TYPE */
          short isc_206;	/* RDB$FIELD_SUB_TYPE */
          short isc_207;	/* RDB$FIELD_POSITION */
          short isc_208;	/* RDB$FIELD_SCALE */
          short isc_209;	/* RDB$FIELD_LENGTH */
          short isc_210;	/* RDB$FIELD_ID */
   } isc_191;
   struct isc_188_struct {
          char  isc_189 [32];	/* RDB$RELATION_NAME */
          char  isc_190 [32];	/* RDB$FIELD_NAME */
   } isc_188;
	const MetIdentifier name(string);
	const SSHORT length = strlen(name);

	// Lookup field.  If we find it, nifty.  If not, look it up in the
	// database.

	gpre_sym* symbol;
	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
	{
		if (symbol->sym_type == SYM_keyword && symbol->sym_keyword == (int) KW_DBKEY)
		{
			return relation->rel_dbkey;
		}

		gpre_fld* field;
		if (symbol->sym_type == SYM_field && (field = (gpre_fld*) symbol->sym_object) &&
			field->fld_relation == relation)
		{
			return field;
		}
	}

	if (gpreGlob.sw_language == lang_internal)
		return NULL;

	gpre_dbb* database = relation->rel_database;

	if (database->dbb_flags & DBB_sqlca)
		return NULL;

	DB = database->dbb_handle;
	gds_trans = database->dbb_transaction;

	gpre_fld* field = NULL;
	/*FOR(REQUEST_HANDLE database->dbb_field_request)
	RFR IN RDB$RELATION_FIELDS CROSS F IN RDB$FIELDS WITH
		RFR.RDB$FIELD_SOURCE EQ F.RDB$FIELD_NAME AND
		RFR.RDB$FIELD_NAME EQ name AND
		RFR.RDB$RELATION_NAME EQ relation->rel_symbol->sym_string*/
	{
        if (!database->dbb_field_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_field_request, (short) sizeof(isc_187), (char*) isc_187);
	isc_vtov ((const char*) relation->rel_symbol->sym_string, (char*) isc_188.isc_189, 32);
	isc_vtov ((const char*) name, (char*) isc_188.isc_190, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_field_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 64, &isc_188, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_field_request, (short) 1, (short) 98, &isc_191, (short) 0);
	   if (!isc_191.isc_194) break;
		field = (gpre_fld*) MSC_alloc(FLD_LEN);
		field->fld_relation = relation;
		field->fld_next = relation->rel_fields;
		relation->rel_fields = field;
		field->fld_id = /*RFR.RDB$FIELD_ID*/
				isc_191.isc_210;
		field->fld_length = /*F.RDB$FIELD_LENGTH*/
				    isc_191.isc_209;
		field->fld_scale = /*F.RDB$FIELD_SCALE*/
				   isc_191.isc_208;
		field->fld_position = /*RFR.RDB$FIELD_POSITION*/
				      isc_191.isc_207;
		field->fld_sub_type = /*F.RDB$FIELD_SUB_TYPE*/
				      isc_191.isc_206;
		field->fld_dtype =
			MET_get_dtype(/*F.RDB$FIELD_TYPE*/
				      isc_191.isc_205, /*F.RDB$FIELD_SUB_TYPE*/
  isc_191.isc_206, &field->fld_length);

		switch (field->fld_dtype)
		{
		case dtype_text:
		case dtype_cstring:
			field->fld_flags |= FLD_text;
			break;

		case dtype_blob:
			field->fld_flags |= FLD_blob;
			field->fld_seg_length = /*F.RDB$SEGMENT_LENGTH*/
						isc_191.isc_204;
			break;
		}

		if (/*F.RDB$DIMENSIONS*/
		    isc_191.isc_203)
			get_array(database, /*F.RDB$FIELD_NAME*/
					    isc_191.isc_193, field);

		if ((field->fld_dtype <= dtype_any_text) || (field->fld_dtype == dtype_blob))
		{
			if (!/*F.RDB$CHARACTER_LENGTH.NULL*/
			     isc_191.isc_201)
				field->fld_char_length = /*F.RDB$CHARACTER_LENGTH*/
							 isc_191.isc_202;
			if (!/*F.RDB$CHARACTER_SET_ID.NULL*/
			     isc_191.isc_199)
				field->fld_charset_id = /*F.RDB$CHARACTER_SET_ID*/
							isc_191.isc_200;
			if (!/*RFR.RDB$COLLATION_ID.NULL*/
			     isc_191.isc_197)
				field->fld_collate_id = /*RFR.RDB$COLLATION_ID*/
							isc_191.isc_198;
			else if (!/*F.RDB$COLLATION_ID.NULL*/
				  isc_191.isc_195)
				field->fld_collate_id = /*F.RDB$COLLATION_ID*/
							isc_191.isc_196;
		}

		field->fld_ttype =
			INTL_CS_COLL_TO_TTYPE(field->fld_charset_id, field->fld_collate_id);
		field->fld_symbol = symbol = MSC_symbol(SYM_field, name, length, (gpre_ctx*) field);
		HSH_insert(symbol);
		field->fld_global = symbol =
			MSC_symbol(SYM_field, /*RFR.RDB$FIELD_SOURCE*/
					      isc_191.isc_192,
					   fb_utils::name_length(/*RFR.RDB$FIELD_SOURCE*/
								 isc_191.isc_192), (gpre_ctx*) field);
	/*END_FOR;*/
	   }
	}

	return field;
}


/*____________________________________________________________
 *
 *     Return a list of the fields in a relation
 */

gpre_nod* MET_fields(gpre_ctx* context)
{
   struct isc_184_struct {
          char  isc_185 [32];	/* RDB$FIELD_NAME */
          short isc_186;	/* isc_utility */
   } isc_184;
   struct isc_182_struct {
          char  isc_183 [32];	/* RDB$RELATION_NAME */
   } isc_182;
	gpre_fld* field;
	gpre_nod* field_node;
	ref* reference;

	gpre_prc* procedure = context->ctx_procedure;
	if (procedure)
	{
		gpre_nod* node = MSC_node(nod_list, procedure->prc_out_count);
		//count = 0;
		for (field = procedure->prc_outputs; field; field = field->fld_next)
		{
			reference = (ref*) MSC_alloc(REF_LEN);
			reference->ref_field = field;
			reference->ref_context = context;
			field_node = MSC_unary(nod_field, (gpre_nod*) reference);
			node->nod_arg[field->fld_position] = field_node;
			//count++;
		}
		return node;
	}

	gpre_rel* relation = context->ctx_relation;
	if (relation->rel_meta)
	{
		int count = 0;
		for (field = relation->rel_fields; field; field = field->fld_next) {
				++count;
		}
		gpre_nod* anode = MSC_node(nod_list, count);
		//count = 0;
		for (field = relation->rel_fields; field; field = field->fld_next)
		{
			reference = (ref*) MSC_alloc(REF_LEN);
			reference->ref_field = field;
			reference->ref_context = context;
			field_node = MSC_unary(nod_field, (gpre_nod*) reference);
			anode->nod_arg[field->fld_position] = field_node;
			//count++;
		}
		return anode;
	}

	if (gpreGlob.sw_language == lang_internal)
		return NULL;

	gpre_dbb* database = relation->rel_database;
	DB = database->dbb_handle;
	gds_trans = database->dbb_transaction;

	int count = 0;
	gpre_lls* stack = NULL;
	/*FOR(REQUEST_HANDLE database->dbb_flds_request)
	RFR IN RDB$RELATION_FIELDS CROSS GLOB_FLD IN RDB$FIELDS WITH
		RFR.RDB$FIELD_SOURCE EQ GLOB_FLD.RDB$FIELD_NAME AND
		RFR.RDB$RELATION_NAME EQ relation->rel_symbol->sym_string
		SORTED BY RFR.RDB$FIELD_POSITION*/
	{
        if (!database->dbb_flds_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_flds_request, (short) sizeof(isc_181), (char*) isc_181);
	isc_vtov ((const char*) relation->rel_symbol->sym_string, (char*) isc_182.isc_183, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_flds_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_182, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_flds_request, (short) 1, (short) 34, &isc_184, (short) 0);
	   if (!isc_184.isc_186) break;

		TEXT* p;
		// This code stops at the first blank.
		for (p = /*RFR.RDB$FIELD_NAME*/
			 isc_184.isc_185; *p && *p != ' '; p++);
		*p = 0;
		MSC_push((gpre_nod*) MET_field(relation, /*RFR.RDB$FIELD_NAME*/
							 isc_184.isc_185), &stack);
		count++;
	/*END_FOR;*/
	   }
	}

	gpre_nod* node = MSC_node(nod_list, count);

	while (stack)
	{
		field = (gpre_fld*) MSC_pop(&stack);
		reference = (ref*) MSC_alloc(REF_LEN);
		reference->ref_field = field;
		reference->ref_context = context;
		field_node = MSC_unary(nod_field, (gpre_nod*) reference);
		node->nod_arg[--count] = field_node;
	}

	return node;
}


/*____________________________________________________________
 *
 *		Shutdown all attached databases.
 */

void MET_fini(gpre_dbb* end)
{
	for (gpre_dbb* database = gpreGlob.isc_databases;
		 database && database != end;
		 database = database->dbb_next)
	{
		if (DB = database->dbb_handle)
		{
			if (gds_trans = database->dbb_transaction)
				/*COMMIT;*/
				{
				isc_commit_transaction (NULL, (FB_API_HANDLE*) &gds_trans);
				}
			/*FINISH*/
			{
			if (DB)
			   isc_detach_database (NULL, &DB);
			};
			database->dbb_handle = 0;
			database->dbb_transaction = 0;

			database->dbb_field_request = 0;
			database->dbb_flds_request = 0;
			//database->dbb_relation_request = 0;
			database->dbb_procedure_request = 0;
			database->dbb_udf_request = 0;
			database->dbb_trigger_request = 0;
			database->dbb_proc_prms_request = 0;
			database->dbb_proc_prm_fld_request = 0;
			database->dbb_index_request = 0;
			database->dbb_type_request = 0;
			database->dbb_array_request = 0;
			database->dbb_dimension_request = 0;
			database->dbb_domain_request = 0;
			//database->dbb_generator_request = 0;
			database->dbb_view_request = 0;
			database->dbb_primary_key_request = 0;
		}
	}
}


/*____________________________________________________________
 *
 *		Lookup a generator by name.
 *		If found, return string. If not, return NULL.
 */

const SCHAR* MET_generator(const TEXT* string, const gpre_dbb* database)
{
	const MetIdentifier name(string);

	for (gpre_sym* symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if ((symbol->sym_type == SYM_generator) && (database == (gpre_dbb*) symbol->sym_object))
		{
			return symbol->sym_string;
		}

	return NULL;
}


/*____________________________________________________________
 *
 *		Compute internal datatype and length based on system relation field values.
 */

USHORT MET_get_dtype(USHORT blr_dtype, USHORT sub_type, USHORT* length)
{
	USHORT dtype;

	USHORT l = *length;

	switch (blr_dtype)
	{
	case blr_varying:
	case blr_text:
		dtype = dtype_text;
		if (gpreGlob.sw_cstring && sub_type != dsc_text_type_fixed)
		{
			++l;
			dtype = dtype_cstring;
		}
		break;

	case blr_cstring:
		dtype = dtype_cstring;
		++l;
		break;

	case blr_short:
		dtype = dtype_short;
		l = sizeof(SSHORT);
		break;

	case blr_long:
		dtype = dtype_long;
		l = sizeof(SLONG);
		break;

	case blr_quad:
		dtype = dtype_quad;
		l = sizeof(ISC_QUAD);
		break;

	case blr_float:
		dtype = dtype_real;
		l = sizeof(float);
		break;

	case blr_double:
		dtype = dtype_double;
		l = sizeof(double);
		break;

	case blr_blob:
		dtype = dtype_blob;
		l = sizeof(ISC_QUAD);
		break;

	// Begin sql date/time/timestamp
	case blr_sql_date:
		dtype = dtype_sql_date;
		l = sizeof(ISC_DATE);
		break;

	case blr_sql_time:
		dtype = dtype_sql_time;
		l = sizeof(ISC_TIME);
		break;

	case blr_timestamp:
		dtype = dtype_timestamp;
		l = sizeof(ISC_TIMESTAMP);
		break;
	// End sql date/time/timestamp

	case blr_int64:
		dtype = dtype_int64;
		l = sizeof(ISC_INT64);
		break;

	default:
		CPR_error("datatype not supported");
		return 0; // silence non initialized warning
	}

	*length = l;

	return dtype;
}


/*____________________________________________________________
 *
 *		Lookup a procedure (represented by a token) in a database.
 *		Return a procedure block (if name is found) or NULL.
 *
 *		This function has been cloned into MET_get_udf
 */

gpre_prc* MET_get_procedure(gpre_dbb* database, const TEXT* string, const TEXT* owner_name)
{
   struct isc_154_struct {
          short isc_155;	/* isc_utility */
          short isc_156;	/* RDB$SEGMENT_LENGTH */
          short isc_157;	/* gds__null_flag */
          short isc_158;	/* RDB$COLLATION_ID */
          short isc_159;	/* gds__null_flag */
          short isc_160;	/* RDB$CHARACTER_SET_ID */
          short isc_161;	/* gds__null_flag */
          short isc_162;	/* RDB$CHARACTER_LENGTH */
          short isc_163;	/* RDB$FIELD_TYPE */
          short isc_164;	/* RDB$FIELD_SUB_TYPE */
          short isc_165;	/* RDB$FIELD_SCALE */
          short isc_166;	/* RDB$FIELD_LENGTH */
   } isc_154;
   struct isc_152_struct {
          char  isc_153 [32];	/* RDB$FIELD_SOURCE */
   } isc_152;
   struct isc_171_struct {
          char  isc_172 [32];	/* RDB$PARAMETER_NAME */
          char  isc_173 [32];	/* RDB$FIELD_SOURCE */
          short isc_174;	/* isc_utility */
          short isc_175;	/* RDB$PARAMETER_NUMBER */
   } isc_171;
   struct isc_168_struct {
          char  isc_169 [32];	/* RDB$PROCEDURE_NAME */
          short isc_170;	/* RDB$PARAMETER_TYPE */
   } isc_168;
   struct isc_179_struct {
          short isc_180;	/* isc_utility */
   } isc_179;
   struct isc_177_struct {
          short isc_178;	/* RDB$PROCEDURE_ID */
   } isc_177;
	const MetIdentifier name(string), owner(owner_name);

	gpre_prc* procedure = NULL;

	for (gpre_sym* symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
	{
		if (symbol->sym_type == SYM_procedure && (procedure = (gpre_prc*) symbol->sym_object) &&
			procedure->prc_database == database &&
			(!owner[0] || (procedure->prc_owner && !strcmp(owner, procedure->prc_owner->sym_string))))
		{
			break;
		}
	}

	if (!procedure)
		return NULL;

	if (procedure->prc_flags & PRC_scanned)
		return procedure;

	/*FOR(REQUEST_HANDLE database->dbb_procedure_request)
	X IN RDB$PROCEDURES WITH X.RDB$PROCEDURE_ID = procedure->prc_id*/
	{
        if (!database->dbb_procedure_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_procedure_request, (short) sizeof(isc_176), (char*) isc_176);
	isc_177.isc_178 = procedure->prc_id;
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_procedure_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 2, &isc_177, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_procedure_request, (short) 1, (short) 2, &isc_179, (short) 0);
	   if (!isc_179.isc_180) break;;

	for (USHORT type = 0; type < 2; type++)
	{
		USHORT count = 0;
		gpre_fld** fld_list;
		if (type)
			fld_list = &procedure->prc_outputs;
		else
			fld_list = &procedure->prc_inputs;

		/*FOR(REQUEST_HANDLE database->dbb_proc_prms_request)
		Y IN RDB$PROCEDURE_PARAMETERS WITH
			Y.RDB$PROCEDURE_NAME EQ name AND
			Y.RDB$PARAMETER_TYPE EQ type
			SORTED BY DESCENDING Y.RDB$PARAMETER_NUMBER*/
		{
                if (!database->dbb_proc_prms_request)
                   isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_proc_prms_request, (short) sizeof(isc_167), (char*) isc_167);
		isc_vtov ((const char*) name, (char*) isc_168.isc_169, 32);
		isc_168.isc_170 = type;
                isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_proc_prms_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 34, &isc_168, (short) 0);
		while (1)
		   {
                   isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_proc_prms_request, (short) 1, (short) 68, &isc_171, (short) 0);
		   if (!isc_171.isc_174) break;
			count++;

			/*FOR(REQUEST_HANDLE database->dbb_proc_prm_fld_request)
			F IN RDB$FIELDS WITH
				Y.RDB$FIELD_SOURCE EQ F.RDB$FIELD_NAME*/
			{
                        if (!database->dbb_proc_prm_fld_request)
                           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_proc_prm_fld_request, (short) sizeof(isc_151), (char*) isc_151);
			isc_vtov ((const char*) isc_171.isc_173, (char*) isc_152.isc_153, 32);
                        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_proc_prm_fld_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_152, (short) 0);
			while (1)
			   {
                           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_proc_prm_fld_request, (short) 1, (short) 24, &isc_154, (short) 0);
			   if (!isc_154.isc_155) break;
				gpre_fld* field = (gpre_fld*) MSC_alloc(FLD_LEN);
				field->fld_procedure = procedure;
				field->fld_next = *fld_list;
				*fld_list = field;
				field->fld_position = /*Y.RDB$PARAMETER_NUMBER*/
						      isc_171.isc_175;
				field->fld_length = /*F.RDB$FIELD_LENGTH*/
						    isc_154.isc_166;
				field->fld_scale = /*F.RDB$FIELD_SCALE*/
						   isc_154.isc_165;
				field->fld_sub_type = /*F.RDB$FIELD_SUB_TYPE*/
						      isc_154.isc_164;
				field->fld_dtype =
					MET_get_dtype(/*F.RDB$FIELD_TYPE*/
						      isc_154.isc_163, /*F.RDB$FIELD_SUB_TYPE*/
  isc_154.isc_164, &field->fld_length);
				switch (field->fld_dtype)
				{
				case dtype_text:
				case dtype_cstring:
					field->fld_flags |= FLD_text;
					if (!/*F.RDB$CHARACTER_LENGTH.NULL*/
					     isc_154.isc_161)
						field->fld_char_length = /*F.RDB$CHARACTER_LENGTH*/
									 isc_154.isc_162;
					if (!/*F.RDB$CHARACTER_SET_ID.NULL*/
					     isc_154.isc_159)
						field->fld_charset_id = /*F.RDB$CHARACTER_SET_ID*/
									isc_154.isc_160;
					if (!/*F.RDB$COLLATION_ID.NULL*/
					     isc_154.isc_157)
						field->fld_collate_id = /*F.RDB$COLLATION_ID*/
									isc_154.isc_158;
					field->fld_ttype =
						INTL_CS_COLL_TO_TTYPE(field->fld_charset_id, field->fld_collate_id);
					break;

				case dtype_blob:
					field->fld_flags |= FLD_blob;
					field->fld_seg_length = /*F.RDB$SEGMENT_LENGTH*/
								isc_154.isc_156;
					if (!/*F.RDB$CHARACTER_SET_ID.NULL*/
					     isc_154.isc_159)
						field->fld_charset_id = /*F.RDB$CHARACTER_SET_ID*/
									isc_154.isc_160;
					break;
				}
				field->fld_symbol = MSC_symbol(SYM_field,
											   /*Y.RDB$PARAMETER_NAME*/
											   isc_171.isc_172,
											   fb_utils::name_length(/*Y.RDB$PARAMETER_NAME*/
														 isc_171.isc_172),
											   (gpre_ctx*) field);
				// If output parameter, insert in symbol table as a field.
				if (type)
					HSH_insert(field->fld_symbol);

			/*END_FOR;*/
			   }
			}

		/*END_FOR;*/
		   }
		}

		if (type)
			procedure->prc_out_count = count;
		else
			procedure->prc_in_count = count;
	}

	/*END_FOR;*/
	   }
	}
	procedure->prc_flags |= PRC_scanned;

	// Generate a dummy relation to point to the procedure to use when procedure
	// is used as a view.
	return procedure;
}


/*____________________________________________________________
 *
 *		Lookup a relation (represented by a token) in a database.
 *		Return a relation block (if name is found) or NULL.
 */

gpre_rel* MET_get_relation(gpre_dbb* database, const TEXT* string, const TEXT* owner_name)
{
	gpre_rel* relation;
	const MetIdentifier name(string), owner(owner_name);

	for (gpre_sym* symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
	{
		if (symbol->sym_type == SYM_relation && (relation = (gpre_rel*) symbol->sym_object) &&
			relation->rel_database == database &&
			(!owner[0] || (relation->rel_owner && !strcmp(owner, relation->rel_owner->sym_string))))
		{
			return relation;
		}
	}

	return NULL;
}


/*____________________________________________________________
 *
 */

intlsym* MET_get_text_subtype(SSHORT ttype)
{
	for (intlsym* p = gpreGlob.text_subtypes; p; p = p->intlsym_next)
	{
		if (p->intlsym_ttype == ttype)
			return p;
	}

	return NULL;
}


/*____________________________________________________________
 *
 *		Lookup a udf (represented by a token) in a database.
 *		Return a udf block (if name is found) or NULL.
 *
 *		This function was cloned from MET_get_procedure
 */

udf* MET_get_udf(gpre_dbb* database, const TEXT* string)
{
   struct isc_142_struct {
          short isc_143;	/* isc_utility */
          short isc_144;	/* gds__null_flag */
          short isc_145;	/* RDB$CHARACTER_SET_ID */
          short isc_146;	/* RDB$FIELD_TYPE */
          short isc_147;	/* RDB$FIELD_SUB_TYPE */
          short isc_148;	/* RDB$FIELD_SCALE */
          short isc_149;	/* RDB$FIELD_LENGTH */
          short isc_150;	/* RDB$ARGUMENT_POSITION */
   } isc_142;
   struct isc_140_struct {
          char  isc_141 [32];	/* RDB$FUNCTION_NAME */
   } isc_140;
	const MetIdentifier name(string);

	udf* the_udf = NULL;
	for (gpre_sym* symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
	{
		if (symbol->sym_type == SYM_udf &&
			(the_udf = (udf*) symbol->sym_object) && the_udf->udf_database == database)
		{
			break;
		}
	}

	if (!the_udf)
		return NULL;

	if (the_udf->udf_flags & UDF_scanned)
		return the_udf;

	/*FOR(REQUEST_HANDLE database->dbb_udf_request)
	UDF_DEF IN RDB$FUNCTIONS CROSS
		UDF_ARG IN RDB$FUNCTION_ARGUMENTS
		WITH UDF_DEF.RDB$FUNCTION_NAME EQ name AND
		UDF_DEF.RDB$FUNCTION_NAME EQ UDF_ARG.RDB$FUNCTION_NAME AND
		UDF_DEF.RDB$RETURN_ARGUMENT != UDF_ARG.RDB$ARGUMENT_POSITION
		SORTED BY DESCENDING UDF_ARG.RDB$ARGUMENT_POSITION*/
	{
        if (!database->dbb_udf_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_udf_request, (short) sizeof(isc_139), (char*) isc_139);
	isc_vtov ((const char*) name, (char*) isc_140.isc_141, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_udf_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_140, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_udf_request, (short) 1, (short) 16, &isc_142, (short) 0);
	   if (!isc_142.isc_143) break;;

		gpre_fld* field = (gpre_fld*) MSC_alloc(FLD_LEN);
		field->fld_next = the_udf->udf_inputs;
		the_udf->udf_inputs = field;
		the_udf->udf_args++;
		field->fld_position = /*UDF_ARG.RDB$ARGUMENT_POSITION*/
				      isc_142.isc_150;
		field->fld_length = /*UDF_ARG.RDB$FIELD_LENGTH*/
				    isc_142.isc_149;
		field->fld_scale = /*UDF_ARG.RDB$FIELD_SCALE*/
				   isc_142.isc_148;
		field->fld_sub_type = /*UDF_ARG.RDB$FIELD_SUB_TYPE*/
				      isc_142.isc_147;
		field->fld_dtype =
			MET_get_dtype(/*UDF_ARG.RDB$FIELD_TYPE*/
				      isc_142.isc_146, /*UDF_ARG.RDB$FIELD_SUB_TYPE*/
  isc_142.isc_147, &field->fld_length);
		switch (field->fld_dtype)
		{
		case dtype_text:
		case dtype_cstring:
			field->fld_flags |= FLD_text;
			if (!/*UDF_ARG.RDB$CHARACTER_SET_ID.NULL*/
			     isc_142.isc_144)
				field->fld_charset_id = /*UDF_ARG.RDB$CHARACTER_SET_ID*/
							isc_142.isc_145;
			field->fld_ttype = INTL_CS_COLL_TO_TTYPE(field->fld_charset_id, field->fld_collate_id);
			break;

		case dtype_blob:
			field->fld_flags |= FLD_blob;
			if (!/*UDF_ARG.RDB$CHARACTER_SET_ID.NULL*/
			     isc_142.isc_144)
				field->fld_charset_id = /*UDF_ARG.RDB$CHARACTER_SET_ID*/
							isc_142.isc_145;
			break;
		}
	/*END_FOR;*/
	   }
	}

	the_udf->udf_flags |= UDF_scanned;

	return the_udf;
}


/*____________________________________________________________
 *
 *		Return relation if the passed view_name represents a
 *		view with the passed relation as a base table
 *		(the relation could be an alias).
 */

gpre_rel* MET_get_view_relation(gpre_req* request,
							   const char* view_name,
							   const char* relation_or_alias,
							   USHORT level)
{
   struct isc_135_struct {
          char  isc_136 [32];	/* RDB$RELATION_NAME */
          char  isc_137 [256];	/* RDB$CONTEXT_NAME */
          short isc_138;	/* isc_utility */
   } isc_135;
   struct isc_133_struct {
          char  isc_134 [32];	/* RDB$VIEW_NAME */
   } isc_133;
	gpre_dbb* database = request->req_database;
	DB = database->dbb_handle;
	gds_trans = database->dbb_transaction;

	gpre_rel* relation = NULL;

	/*FOR(REQUEST_HANDLE database->dbb_view_request, LEVEL level)
	X IN RDB$VIEW_RELATIONS WITH
		X.RDB$VIEW_NAME EQ view_name*/
	{
        if (!database->dbb_view_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_view_request, (short) sizeof(isc_132), (char*) isc_132);
	isc_vtov ((const char*) view_name, (char*) isc_133.isc_134, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_view_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_133, (short) level);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_view_request, (short) 1, (short) 290, &isc_135, (short) level);
	   if (!isc_135.isc_138) break;
		TEXT* p;
		// This code stops at the first blank.
		for (p = /*X.RDB$CONTEXT_NAME*/
			 isc_135.isc_137; *p && *p != ' '; p++);
		*p = 0;

		for (p = /*X.RDB$RELATION_NAME*/
			 isc_135.isc_136; *p && *p != ' '; p++);
		*p = 0;

		if (!strcmp(/*X.RDB$RELATION_NAME*/
			    isc_135.isc_136, relation_or_alias) ||
			!strcmp(/*X.RDB$CONTEXT_NAME*/
				isc_135.isc_137, relation_or_alias))
		{
			return MET_get_relation(database, /*X.RDB$RELATION_NAME*/
							  isc_135.isc_136, "");
		}

		if (relation =
			MET_get_view_relation(request, /*X.RDB$RELATION_NAME*/
						       isc_135.isc_136, relation_or_alias, level + 1))
		{
			return relation;
		}

	/*END_FOR;*/
	   }
	}

	return NULL;
}


/*____________________________________________________________
 *
 *		Lookup an index for a database.
 *		Return an index block (if name is found) or NULL.
 */

gpre_index* MET_index(gpre_dbb* database, const TEXT* string)
{
   struct isc_129_struct {
          char  isc_130 [32];	/* RDB$RELATION_NAME */
          short isc_131;	/* isc_utility */
   } isc_129;
   struct isc_127_struct {
          char  isc_128 [32];	/* RDB$INDEX_NAME */
   } isc_127;
	gpre_index* index;
	const MetIdentifier name(string);
	const USHORT length = strlen(name);

	gpre_sym* symbol;
	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
	{
		if (symbol->sym_type == SYM_index && (index = (gpre_index*) symbol->sym_object) &&
			index->ind_relation->rel_database == database)
		{
			return index;
		}
	}

	if (gpreGlob.sw_language == lang_internal)
		return NULL;

	if (database->dbb_flags & DBB_sqlca)
		return NULL;

	DB = database->dbb_handle;
	gds_trans = database->dbb_transaction;
	index = NULL;

	/*FOR(REQUEST_HANDLE database->dbb_index_request)
	X IN RDB$INDICES WITH X.RDB$INDEX_NAME EQ name*/
	{
        if (!database->dbb_index_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_index_request, (short) sizeof(isc_126), (char*) isc_126);
	isc_vtov ((const char*) name, (char*) isc_127.isc_128, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_index_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_127, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_index_request, (short) 1, (short) 34, &isc_129, (short) 0);
	   if (!isc_129.isc_131) break;
		index = (gpre_index*) MSC_alloc(IND_LEN);
		index->ind_symbol = symbol = MSC_symbol(SYM_index, name, length, (gpre_ctx*) index);
		index->ind_relation = MET_get_relation(database, /*X.RDB$RELATION_NAME*/
								 isc_129.isc_130, "");
		HSH_insert(symbol);

	/*END_FOR;*/
	   }
	}

	return index;
}


/*____________________________________________________________
 *
 *		Load all of the relation names, procedure names
 *       and user defined function names
 *       into the symbol (hash) table.
 *      Includes also charsets and collations.
 */

void MET_load_hash_table(gpre_dbb* database)
{
   struct isc_50_struct {
          char  isc_51 [32];	/* RDB$GENERATOR_NAME */
          short isc_52;	/* isc_utility */
   } isc_50;
   struct isc_54_struct {
          char  isc_55 [32];	/* RDB$CHARACTER_SET_NAME */
          short isc_56;	/* isc_utility */
   } isc_54;
   struct isc_61_struct {
          char  isc_62 [32];	/* RDB$TYPE_NAME */
          short isc_63;	/* isc_utility */
   } isc_61;
   struct isc_58_struct {
          char  isc_59 [32];	/* RDB$CHARACTER_SET_NAME */
          short isc_60;	/* RDB$CHARACTER_SET_ID */
   } isc_58;
   struct isc_65_struct {
          char  isc_66 [32];	/* RDB$CHARACTER_SET_NAME */
          short isc_67;	/* isc_utility */
          short isc_68;	/* RDB$CHARACTER_SET_ID */
          short isc_69;	/* gds__null_flag */
          short isc_70;	/* RDB$BYTES_PER_CHARACTER */
          short isc_71;	/* RDB$COLLATION_ID */
          short isc_72;	/* RDB$CHARACTER_SET_ID */
   } isc_65;
   struct isc_77_struct {
          char  isc_78 [32];	/* RDB$TYPE_NAME */
          short isc_79;	/* isc_utility */
   } isc_77;
   struct isc_74_struct {
          char  isc_75 [32];	/* RDB$COLLATION_NAME */
          short isc_76;	/* RDB$COLLATION_ID */
   } isc_74;
   struct isc_81_struct {
          char  isc_82 [32];	/* RDB$COLLATION_NAME */
          short isc_83;	/* isc_utility */
          short isc_84;	/* gds__null_flag */
          short isc_85;	/* RDB$BYTES_PER_CHARACTER */
          short isc_86;	/* RDB$COLLATION_ID */
          short isc_87;	/* RDB$CHARACTER_SET_ID */
   } isc_81;
   struct isc_92_struct {
          short isc_93;	/* isc_utility */
          short isc_94;	/* RDB$COLLATION_ID */
          short isc_95;	/* RDB$CHARACTER_SET_ID */
   } isc_92;
   struct isc_89_struct {
          char  isc_90 [32];	/* RDB$FUNCTION_NAME */
          short isc_91;	/* RDB$ARGUMENT_POSITION */
   } isc_89;
   struct isc_97_struct {
          char  isc_98 [32];	/* RDB$FUNCTION_NAME */
          char  isc_99 [32];	/* RDB$QUERY_NAME */
          char  isc_100 [32];	/* RDB$FUNCTION_NAME */
          short isc_101;	/* isc_utility */
          short isc_102;	/* RDB$ARGUMENT_POSITION */
          short isc_103;	/* RDB$FIELD_TYPE */
          short isc_104;	/* RDB$FIELD_SUB_TYPE */
          short isc_105;	/* RDB$FIELD_SCALE */
          short isc_106;	/* RDB$FIELD_LENGTH */
          short isc_107;	/* RDB$FUNCTION_TYPE */
   } isc_97;
   struct isc_109_struct {
          char  isc_110 [32];	/* RDB$OWNER_NAME */
          char  isc_111 [32];	/* RDB$PROCEDURE_NAME */
          short isc_112;	/* isc_utility */
          short isc_113;	/* gds__null_flag */
          short isc_114;	/* RDB$PROCEDURE_ID */
   } isc_109;
   struct isc_116_struct {
          char  isc_117 [32];	/* RDB$OWNER_NAME */
          char  isc_118 [32];	/* RDB$RELATION_NAME */
          short isc_119;	/* isc_utility */
          short isc_120;	/* gds__null_flag */
          short isc_121;	/* RDB$DBKEY_LENGTH */
          short isc_122;	/* RDB$RELATION_ID */
   } isc_116;
   struct isc_124_struct {
          short isc_125;	/* isc_utility */
   } isc_124;
	// If this is an internal ISC access method invocation, don't do any of this stuff

	if (gpreGlob.sw_language == lang_internal)
		return;

	if (!database->dbb_handle)
	{
		if (!MET_database(database, false))
			CPR_exit(FINI_ERROR);
	}

	if (database->dbb_transaction)
	{
		// we must have already loaded this one
		return;
	}

	gds_trans = 0;
	DB = database->dbb_handle;

	/*START_TRANSACTION;*/
	{
	{
	isc_start_transaction (NULL, (FB_API_HANDLE*) &gds_trans, (short) 1, &DB, (short) 0, (char*) 0);
	}
	}

	database->dbb_transaction = gds_trans;

	// Determine if the database is V3.

	bool post_v3_flag = false;
	FB_API_HANDLE handle = 0;
	/*FOR(REQUEST_HANDLE handle)
	X IN RDB$RELATIONS WITH
		X.RDB$RELATION_NAME = 'RDB$PROCEDURES' AND
		X.RDB$SYSTEM_FLAG = 1*/
	{
        if (!handle)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle, (short) sizeof(isc_123), (char*) isc_123);
        isc_start_request (NULL, (FB_API_HANDLE*) &handle, (FB_API_HANDLE*) &gds_trans, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &handle, (short) 0, (short) 2, &isc_124, (short) 0);
	   if (!isc_124.isc_125) break;
		post_v3_flag = true;

	/*END_FOR;*/
	   }
	}
	isc_release_request(gds_status, &handle);

	if (!post_v3_flag)
	{
		CPR_error(NEED_IB4_AT_LEAST);
		CPR_abort();
	}

	SLONG length;

	// Pick up all relations (necessary to parse parts of the GDML grammar)

	/*FOR(REQUEST_HANDLE handle)
	X IN RDB$RELATIONS*/
	{
        if (!handle)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle, (short) sizeof(isc_115), (char*) isc_115);
        isc_start_request (NULL, (FB_API_HANDLE*) &handle, (FB_API_HANDLE*) &gds_trans, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &handle, (short) 0, (short) 72, &isc_116, (short) 0);
	   if (!isc_116.isc_119) break;
		gpre_rel* relation = (gpre_rel*) MSC_alloc(REL_LEN);
		relation->rel_database = database;
		relation->rel_next = database->dbb_relations;
		database->dbb_relations = relation;
		relation->rel_id = /*X.RDB$RELATION_ID*/
				   isc_116.isc_122;
		gpre_sym* symbol = relation->rel_symbol =
			MSC_symbol(SYM_relation, /*X.RDB$RELATION_NAME*/
						 isc_116.isc_118,
					   fb_utils::name_length(/*X.RDB$RELATION_NAME*/
								 isc_116.isc_118), (gpre_ctx*) relation);
		HSH_insert(symbol);
		const USHORT dbk_len = (/*X.RDB$DBKEY_LENGTH*/
					isc_116.isc_121) ? /*X.RDB$DBKEY_LENGTH*/
    isc_116.isc_121 : 8;
		gpre_fld* dbkey = MET_make_field("rdb$db_key", dtype_text, dbk_len, false);
		relation->rel_dbkey = dbkey;
		dbkey->fld_flags |= FLD_dbkey | FLD_text | FLD_charset;
		dbkey->fld_ttype = ttype_binary;

		if (!/*X.RDB$OWNER_NAME.NULL*/
		     isc_116.isc_120 && (length = fb_utils::name_length(/*X.RDB$OWNER_NAME*/
				    isc_116.isc_117)))
		{
			relation->rel_owner = MSC_symbol(SYM_username, /*X.RDB$OWNER_NAME*/
								       isc_116.isc_117, length, NULL);
		}

	/*END_FOR;*/
	   }
	}

	isc_release_request(gds_status, &handle);

	// Pick up all procedures (necessary to parse parts of the GDML grammar)

	/*FOR(REQUEST_HANDLE handle)
	X IN RDB$PROCEDURES*/
	{
        if (!handle)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle, (short) sizeof(isc_108), (char*) isc_108);
	if (handle)
	   {
           isc_start_request (isc_status, (FB_API_HANDLE*) &handle, (FB_API_HANDLE*) &gds_trans, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &handle, (short) 0, (short) 70, &isc_109, (short) 0);
	   if (!isc_109.isc_112 || isc_status [1]) break;
		gpre_prc* procedure = (gpre_prc*) MSC_alloc(REL_LEN);
		procedure->prc_database = database;
		procedure->prc_next = database->dbb_procedures;
		database->dbb_procedures = procedure;
		procedure->prc_id = /*X.RDB$PROCEDURE_ID*/
				    isc_109.isc_114;
		gpre_sym* symbol = procedure->prc_symbol =
			MSC_symbol(SYM_procedure, /*X.RDB$PROCEDURE_NAME*/
						  isc_109.isc_111,
					   fb_utils::name_length(/*X.RDB$PROCEDURE_NAME*/
								 isc_109.isc_111), (gpre_ctx*) procedure);
		HSH_insert(symbol);
		if (!/*X.RDB$OWNER_NAME.NULL*/
		     isc_109.isc_113 && (length = fb_utils::name_length(/*X.RDB$OWNER_NAME*/
				    isc_109.isc_110)))
			procedure->prc_owner = MSC_symbol(SYM_username, /*X.RDB$OWNER_NAME*/
									isc_109.isc_110, length, NULL);
	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		CPR_error(NEED_IB4_AT_LEAST);
		CPR_abort();
	/*END_ERROR;*/
	   }
	}

	if (handle)
		isc_release_request(gds_status, &handle);

	// Pickup any user defined functions.  If the database does not support udf's,
	// this may fail

	TEXT* p;
	FB_API_HANDLE handle2 = 0;

	/*FOR(REQUEST_HANDLE handle)
	FUN IN RDB$FUNCTIONS CROSS ARG IN RDB$FUNCTION_ARGUMENTS WITH
		FUN.RDB$FUNCTION_NAME EQ ARG.RDB$FUNCTION_NAME AND
		FUN.RDB$RETURN_ARGUMENT EQ ARG.RDB$ARGUMENT_POSITION*/
	{
        if (!handle)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle, (short) sizeof(isc_96), (char*) isc_96);
	if (handle)
	   {
           isc_start_request (isc_status, (FB_API_HANDLE*) &handle, (FB_API_HANDLE*) &gds_trans, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &handle, (short) 0, (short) 110, &isc_97, (short) 0);
	   if (!isc_97.isc_101 || isc_status [1]) break;
		p = /*FUN.RDB$FUNCTION_NAME*/
		    isc_97.isc_100;
		length = fb_utils::name_length(p);
		p[length] = 0;

		udf* an_udf = (udf*) MSC_alloc(UDF_LEN + length);
		strcpy(an_udf->udf_function, p);
		an_udf->udf_database = database;
		an_udf->udf_type = /*FUN.RDB$FUNCTION_TYPE*/
				   isc_97.isc_107;

		if (length = fb_utils::name_length(/*FUN.RDB$QUERY_NAME*/
						   isc_97.isc_99))
		{
			p = /*FUN.RDB$QUERY_NAME*/
			    isc_97.isc_99;
			p[length] = 0;
		}
		gpre_sym* symbol = an_udf->udf_symbol = MSC_symbol(SYM_udf, p, strlen(p), (gpre_ctx*) an_udf);
		HSH_insert(symbol);

		an_udf->udf_length = /*ARG.RDB$FIELD_LENGTH*/
				     isc_97.isc_106;
		an_udf->udf_scale = /*ARG.RDB$FIELD_SCALE*/
				    isc_97.isc_105;
		an_udf->udf_sub_type = /*ARG.RDB$FIELD_SUB_TYPE*/
				       isc_97.isc_104;
		an_udf->udf_dtype =
			MET_get_dtype(/*ARG.RDB$FIELD_TYPE*/
				      isc_97.isc_103, /*ARG.RDB$FIELD_SUB_TYPE*/
  isc_97.isc_104, &an_udf->udf_length);

		if (an_udf->udf_dtype == dtype_text || an_udf->udf_dtype == dtype_cstring)
		{
			/*FOR(REQUEST_HANDLE handle2)
			V4ARG IN RDB$FUNCTION_ARGUMENTS
				CROSS CS IN RDB$CHARACTER_SETS
				CROSS COLL IN RDB$COLLATIONS WITH
				V4ARG.RDB$CHARACTER_SET_ID EQ CS.RDB$CHARACTER_SET_ID AND
				COLL.RDB$COLLATION_NAME EQ CS.RDB$DEFAULT_COLLATE_NAME AND
				V4ARG.RDB$CHARACTER_SET_ID NOT MISSING AND
				V4ARG.RDB$FUNCTION_NAME EQ ARG.RDB$FUNCTION_NAME AND
				V4ARG.RDB$ARGUMENT_POSITION EQ ARG.RDB$ARGUMENT_POSITION*/
			{
                        if (!handle2)
                           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle2, (short) sizeof(isc_88), (char*) isc_88);
			isc_vtov ((const char*) isc_97.isc_98, (char*) isc_89.isc_90, 32);
			isc_89.isc_91 = isc_97.isc_102;
                        isc_start_and_send (NULL, (FB_API_HANDLE*) &handle2, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 34, &isc_89, (short) 0);
			while (1)
			   {
                           isc_receive (NULL, (FB_API_HANDLE*) &handle2, (short) 1, (short) 6, &isc_92, (short) 0);
			   if (!isc_92.isc_93) break;;

				an_udf->udf_charset_id = /*V4ARG.RDB$CHARACTER_SET_ID*/
							 isc_92.isc_95;
				an_udf->udf_ttype =
					INTL_CS_COLL_TO_TTYPE(an_udf->udf_charset_id, /*COLL.RDB$COLLATION_ID*/
										      isc_92.isc_94);

			/*END_FOR*/
			   }
			}
		}
	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		CPR_error(NEED_IB4_AT_LEAST);
		CPR_abort();
	/*END_ERROR;*/
	   }
	}

	isc_release_request(gds_status, &handle);
	if (handle2)
		isc_release_request(gds_status, &handle2);

	// Pick up all Collation names, might have several collations
	// for a given character set.
	// There can also be several alias names for a character set.

	/*FOR(REQUEST_HANDLE handle)
	CHARSET IN RDB$CHARACTER_SETS CROSS COLL IN RDB$COLLATIONS OVER
		RDB$CHARACTER_SET_ID*/
	{
        if (!handle)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle, (short) sizeof(isc_80), (char*) isc_80);
	if (handle)
	   {
           isc_start_request (isc_status, (FB_API_HANDLE*) &handle, (FB_API_HANDLE*) &gds_trans, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &handle, (short) 0, (short) 42, &isc_81, (short) 0);
	   if (!isc_81.isc_83 || isc_status [1]) break;;

		p = /*COLL.RDB$COLLATION_NAME*/
		    isc_81.isc_82;
		length = fb_utils::name_length(p);
		p[length] = 0;
		intlsym* iname = (intlsym*) MSC_alloc(INTLSYM_LEN + length);
		strcpy(iname->intlsym_name, p);
		iname->intlsym_database = database;
		gpre_sym* symbol = iname->intlsym_symbol =
			MSC_symbol(SYM_collate, p, strlen(p), (gpre_ctx*) iname);
		HSH_insert(symbol);
		iname->intlsym_type = INTLSYM_collation;
		iname->intlsym_flags = 0;
		iname->intlsym_charset_id = /*COLL.RDB$CHARACTER_SET_ID*/
					    isc_81.isc_87;
		iname->intlsym_collate_id = /*COLL.RDB$COLLATION_ID*/
					    isc_81.isc_86;
		iname->intlsym_ttype =
			INTL_CS_COLL_TO_TTYPE(iname->intlsym_charset_id, iname->intlsym_collate_id);
		iname->intlsym_bytes_per_char =
			(/*CHARSET.RDB$BYTES_PER_CHARACTER.NULL*/
			 isc_81.isc_84) ? 1 : (/*CHARSET.RDB$BYTES_PER_CHARACTER*/
	 isc_81.isc_85);
		iname->intlsym_next = gpreGlob.text_subtypes;
		gpreGlob.text_subtypes = iname;

		/*FOR(REQUEST_HANDLE handle2)
		TYPE IN RDB$TYPES
			WITH TYPE.RDB$FIELD_NAME = "RDB$COLLATION_NAME"
			AND TYPE.RDB$TYPE = COLL.RDB$COLLATION_ID
			AND TYPE.RDB$TYPE_NAME != COLL.RDB$COLLATION_NAME*/
		{
                if (!handle2)
                   isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle2, (short) sizeof(isc_73), (char*) isc_73);
		isc_vtov ((const char*) isc_81.isc_82, (char*) isc_74.isc_75, 32);
		isc_74.isc_76 = isc_81.isc_86;
		if (handle2)
		   {
                   isc_start_and_send (isc_status, (FB_API_HANDLE*) &handle2, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 34, &isc_74, (short) 0);
		   }
		if (!isc_status [1]) {
		while (1)
		   {
                   isc_receive (isc_status, (FB_API_HANDLE*) &handle2, (short) 1, (short) 34, &isc_77, (short) 0);
		   if (!isc_77.isc_79 || isc_status [1]) break;;

			p = /*TYPE.RDB$TYPE_NAME*/
			    isc_77.isc_78;
			length = fb_utils::name_length(p);
			p[length] = 0;
			symbol = MSC_symbol(SYM_collate, p, length, (gpre_ctx*) iname);
			HSH_insert(symbol);
		/*END_FOR*/
		   }
		   };
		/*ON_ERROR*/
		if (isc_status [1])
		   {
			// Nothing
		/*END_ERROR;*/
		   }
		}

	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		CPR_error(NEED_IB4_AT_LEAST);
		CPR_abort();
	/*END_ERROR;*/
	   }
	}

	isc_release_request(gds_status, &handle);
	if (handle2)
		isc_release_request(gds_status, &handle2);

	// Now pick up all character set names - with the subtype set to
	// the type of the default collation for the character set.

	/*FOR(REQUEST_HANDLE handle)
	CHARSET IN RDB$CHARACTER_SETS CROSS COLL IN RDB$COLLATIONS
		WITH CHARSET.RDB$DEFAULT_COLLATE_NAME EQ COLL.RDB$COLLATION_NAME*/
	{
        if (!handle)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle, (short) sizeof(isc_64), (char*) isc_64);
	if (handle)
	   {
           isc_start_request (isc_status, (FB_API_HANDLE*) &handle, (FB_API_HANDLE*) &gds_trans, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &handle, (short) 0, (short) 44, &isc_65, (short) 0);
	   if (!isc_65.isc_67 || isc_status [1]) break;;

		p = /*CHARSET.RDB$CHARACTER_SET_NAME*/
		    isc_65.isc_66;
		length = fb_utils::name_length(p);
		p[length] = 0;
		intlsym* iname = (intlsym*) MSC_alloc(INTLSYM_LEN + length);
		strcpy(iname->intlsym_name, p);
		iname->intlsym_database = database;
		gpre_sym* symbol = iname->intlsym_symbol =
			MSC_symbol(SYM_charset, p, strlen(p), (gpre_ctx*) iname);
		HSH_insert(symbol);
		iname->intlsym_type = INTLSYM_collation;
		iname->intlsym_flags = 0;
		iname->intlsym_charset_id = /*COLL.RDB$CHARACTER_SET_ID*/
					    isc_65.isc_72;
		iname->intlsym_collate_id = /*COLL.RDB$COLLATION_ID*/
					    isc_65.isc_71;
		iname->intlsym_ttype =
			INTL_CS_COLL_TO_TTYPE(iname->intlsym_charset_id, iname->intlsym_collate_id);
		iname->intlsym_bytes_per_char =
			(/*CHARSET.RDB$BYTES_PER_CHARACTER.NULL*/
			 isc_65.isc_69) ? 1 : (/*CHARSET.RDB$BYTES_PER_CHARACTER*/
	 isc_65.isc_70);

		/*FOR(REQUEST_HANDLE handle2)
		TYPE IN RDB$TYPES
			WITH TYPE.RDB$FIELD_NAME = "RDB$CHARACTER_SET_NAME"
			AND TYPE.RDB$TYPE = CHARSET.RDB$CHARACTER_SET_ID
			AND TYPE.RDB$TYPE_NAME != CHARSET.RDB$CHARACTER_SET_NAME*/
		{
                if (!handle2)
                   isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle2, (short) sizeof(isc_57), (char*) isc_57);
		isc_vtov ((const char*) isc_65.isc_66, (char*) isc_58.isc_59, 32);
		isc_58.isc_60 = isc_65.isc_68;
		if (handle2)
		   {
                   isc_start_and_send (isc_status, (FB_API_HANDLE*) &handle2, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 34, &isc_58, (short) 0);
		   }
		if (!isc_status [1]) {
		while (1)
		   {
                   isc_receive (isc_status, (FB_API_HANDLE*) &handle2, (short) 1, (short) 34, &isc_61, (short) 0);
		   if (!isc_61.isc_63 || isc_status [1]) break;;

			p = /*TYPE.RDB$TYPE_NAME*/
			    isc_61.isc_62;
			length = fb_utils::name_length(p);
			p[length] = 0;
			symbol = MSC_symbol(SYM_charset, p, length, (gpre_ctx*) iname);
			HSH_insert(symbol);
		/*END_FOR*/
		   }
		   };
		/*ON_ERROR*/
		if (isc_status [1])
		   {
			// Nothing
		/*END_ERROR;*/
		   }
		}

	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		CPR_error(NEED_IB4_AT_LEAST);
		CPR_abort();
	/*END_ERROR;*/
	   }
	}

	isc_release_request(gds_status, &handle);
	if (handle2)
		isc_release_request(gds_status, &handle2);

	// Pick up name of database default character set for SQL

	/*FOR(REQUEST_HANDLE handle)
		FIRST 1 DBB IN RDB$DATABASE
		WITH DBB.RDB$CHARACTER_SET_NAME NOT MISSING*/
	{
        if (!handle)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle, (short) sizeof(isc_53), (char*) isc_53);
	if (handle)
	   {
           isc_start_request (isc_status, (FB_API_HANDLE*) &handle, (FB_API_HANDLE*) &gds_trans, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &handle, (short) 0, (short) 34, &isc_54, (short) 0);
	   if (!isc_54.isc_56 || isc_status [1]) break;
		p = /*DBB.RDB$CHARACTER_SET_NAME*/
		    isc_54.isc_55;
		length = fb_utils::name_length(p);
		p[length] = 0;
		TEXT* s = (TEXT*) MSC_alloc(length + 1);
		strcpy(s, p);
		database->dbb_def_charset = s;
		if (!MSC_find_symbol(HSH_lookup(database->dbb_def_charset), SYM_charset))
			CPR_warn("Default character set for database is not known");
	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		CPR_error(NEED_IB4_AT_LEAST);
		CPR_abort();
	/*END_ERROR;*/
	   }
	}

	isc_release_request(gds_status, &handle);

	// Pick up all generators for the database

	/*FOR(REQUEST_HANDLE handle)
	X IN RDB$GENERATORS*/
	{
        if (!handle)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &handle, (short) sizeof(isc_49), (char*) isc_49);
	if (handle)
	   {
           isc_start_request (isc_status, (FB_API_HANDLE*) &handle, (FB_API_HANDLE*) &gds_trans, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &handle, (short) 0, (short) 34, &isc_50, (short) 0);
	   if (!isc_50.isc_52 || isc_status [1]) break;
		gpre_sym* symbol = MSC_symbol(SYM_generator, /*X.RDB$GENERATOR_NAME*/
							     isc_50.isc_51,
							fb_utils::name_length(/*X.RDB$GENERATOR_NAME*/
									      isc_50.isc_51), (gpre_ctx*) database);
		HSH_insert(symbol);

	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		// Nothing
	/*END_ERROR;*/
	   }
	}

	isc_release_request(gds_status, &handle);

	// now that we have attached to the database, resolve the character set
	// request (if any) (and if we can)

	if (database->dbb_c_lc_ctype)
	{
		if (get_intl_char_subtype(&database->dbb_char_subtype, (UCHAR*) database->dbb_c_lc_ctype,
			 					  strlen(database->dbb_c_lc_ctype), database))
		{
			database->dbb_know_subtype = 1;
		}
		else
		{
			TEXT buffer[200];

			sprintf(buffer, "Cannot recognize character set '%s'", database->dbb_c_lc_ctype);
			PAR_error(buffer);
		}
		gpreGlob.sw_know_interp = database->dbb_know_subtype != 0;
		gpreGlob.sw_interp = database->dbb_char_subtype;
	}
}


/*____________________________________________________________
 *
 *		Make a field symbol.
 */

gpre_fld* MET_make_field(const SCHAR* name, SSHORT dtype, SSHORT length, bool insert_flag)
{
	gpre_fld* field = (gpre_fld*) MSC_alloc(FLD_LEN);
	field->fld_length = length;
	field->fld_dtype = dtype;
	gpre_sym* symbol = MSC_symbol(SYM_field, name, strlen(name), (gpre_ctx*) field);
	field->fld_symbol = symbol;
	if (insert_flag)
		HSH_insert(symbol);

	return field;
}


/*____________________________________________________________
 *
 *		Make an index symbol.
 */

gpre_index* MET_make_index(const SCHAR* name)
{
	gpre_index* index = (gpre_index*) MSC_alloc(IND_LEN);
	index->ind_symbol = MSC_symbol(SYM_index, name, strlen(name), (gpre_ctx*) index);

	return index;
}


/*____________________________________________________________
 *
 *		Make an relation symbol.
 */

gpre_rel* MET_make_relation(const SCHAR* name)
{
	gpre_rel* relation = (gpre_rel*) MSC_alloc(REL_LEN);
	relation->rel_symbol = MSC_symbol(SYM_relation, name, strlen(name), (gpre_ctx*) relation);

	return relation;
}


/*____________________________________________________________
 *
 *		Lookup a type name for a field.
 */

bool MET_type(gpre_fld* field, const TEXT* string, SSHORT* ptr)
{
   struct isc_46_struct {
          short isc_47;	/* isc_utility */
          short isc_48;	/* RDB$TYPE */
   } isc_46;
   struct isc_43_struct {
          char  isc_44 [32];	/* RDB$TYPE_NAME */
          char  isc_45 [32];	/* RDB$FIELD_NAME */
   } isc_43;
	UCHAR buffer[32];			// BASED ON RDB$TYPES.RDB$TYPE_NAME

	gpre_sym* symbol;
	for (symbol = HSH_lookup(string); symbol; symbol = symbol->sym_homonym)
	{
		field_type* type;
		if (symbol->sym_type == SYM_type && (type = (field_type*) symbol->sym_object) &&
			(!type->typ_field || type->typ_field == field))
		{
			*ptr = type->typ_value;
			return true;
		}
	}

	gpre_rel* relation = field->fld_relation;
	gpre_dbb* database = relation->rel_database;
	DB = database->dbb_handle;
	gds_trans = database->dbb_transaction;

	// Force the name to uppercase, using C locale rules for uppercasing
	UCHAR* p;
	for (p = buffer; *string && p < &buffer[sizeof(buffer) - 1]; ++p, ++string)
	{
		*p = UPPER7(*string);
	}
	*p = '\0';

	/*FOR(REQUEST_HANDLE database->dbb_type_request)
	X IN RDB$TYPES WITH X.RDB$FIELD_NAME EQ field->fld_global->sym_string AND
		X.RDB$TYPE_NAME EQ buffer*/
	{
        if (!database->dbb_type_request)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_type_request, (short) sizeof(isc_42), (char*) isc_42);
	isc_vtov ((const char*) buffer, (char*) isc_43.isc_44, 32);
	isc_vtov ((const char*) field->fld_global->sym_string, (char*) isc_43.isc_45, 32);
	if (database->dbb_type_request)
	   {
           isc_start_and_send (isc_status, (FB_API_HANDLE*) &database->dbb_type_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 64, &isc_43, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &database->dbb_type_request, (short) 1, (short) 4, &isc_46, (short) 0);
	   if (!isc_46.isc_47 || isc_status [1]) break;
		field_type* type = (field_type*) MSC_alloc(TYP_LEN);
		type->typ_field = field;
		*ptr = type->typ_value = /*X.RDB$TYPE*/
					 isc_46.isc_48;
		type->typ_symbol = symbol = MSC_symbol(SYM_type, string, strlen(string), (gpre_ctx*) type);
		HSH_insert(symbol);
		return true;
	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		// Nothing
	/*END_ERROR;*/
	   }
	}

	return false;
}


/*____________________________________________________________
 *
 *		Lookup an index for a database.
 *
 *  Return: true if the trigger exists
 *		   false otherwise
 */

bool MET_trigger_exists(gpre_dbb* database, const TEXT* trigger_name)
{
   struct isc_40_struct {
          short isc_41;	/* isc_utility */
   } isc_40;
   struct isc_38_struct {
          char  isc_39 [32];	/* RDB$TRIGGER_NAME */
   } isc_38;
	const MetIdentifier name(trigger_name);

	DB = database->dbb_handle;
	gds_trans = database->dbb_transaction;

	/*FOR(REQUEST_HANDLE database->dbb_trigger_request)
	TRIG IN RDB$TRIGGERS WITH TRIG.RDB$TRIGGER_NAME EQ name*/
	{
        if (!database->dbb_trigger_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_trigger_request, (short) sizeof(isc_37), (char*) isc_37);
	isc_vtov ((const char*) name, (char*) isc_38.isc_39, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_trigger_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_38, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_trigger_request, (short) 1, (short) 2, &isc_40, (short) 0);
	   if (!isc_40.isc_41) break;
		return true;
	/*END_FOR;*/
	   }
	}

	return false;
}


/*____________________________________________________________
 *
 *		Compute and return the size of the array.
 */

static SLONG array_size(const gpre_fld* field)
{
	const ary* array_block = field->fld_array_info;
	SLONG count = field->fld_array->fld_length;
	for (dim* dimension = array_block->ary_dimension; dimension; dimension = dimension->dim_next)
	{
		count = count * (dimension->dim_upper - dimension->dim_lower + 1);
	}

	return count;
}


/*____________________________________________________________
 *
 *		See if field is array.
 */

static void get_array(gpre_dbb* database, const TEXT* field_name, gpre_fld* field)
{
   struct isc_26_struct {
          ISC_LONG isc_27;	/* RDB$UPPER_BOUND */
          ISC_LONG isc_28;	/* RDB$LOWER_BOUND */
          short isc_29;	/* isc_utility */
          short isc_30;	/* RDB$DIMENSION */
   } isc_26;
   struct isc_24_struct {
          char  isc_25 [32];	/* RDB$FIELD_NAME */
   } isc_24;
   struct isc_34_struct {
          short isc_35;	/* isc_utility */
          short isc_36;	/* RDB$DIMENSIONS */
   } isc_34;
   struct isc_32_struct {
          char  isc_33 [32];	/* RDB$FIELD_NAME */
   } isc_32;
	ary* array_block = NULL;
	USHORT field_dimensions = 0;

	/*FOR(REQUEST_HANDLE database->dbb_array_request)

	F IN RDB$FIELDS WITH F.RDB$FIELD_NAME EQ field_name*/
	{
        if (!database->dbb_array_request)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_array_request, (short) sizeof(isc_31), (char*) isc_31);
	isc_vtov ((const char*) field_name, (char*) isc_32.isc_33, 32);
	if (database->dbb_array_request)
	   {
           isc_start_and_send (isc_status, (FB_API_HANDLE*) &database->dbb_array_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_32, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &database->dbb_array_request, (short) 1, (short) 4, &isc_34, (short) 0);
	   if (!isc_34.isc_35 || isc_status [1]) break;
		if (/*F.RDB$DIMENSIONS*/
		    isc_34.isc_36)
		{
			gpre_fld* sub_field = (gpre_fld*) MSC_alloc(FLD_LEN);
			*sub_field = *field;
			// CVC: beware, the above line is far from deep copy;
			// there are only 13 non-pointer members in gpre_fld.
			// The 14 pointer members share target locations with "field".
			field->fld_array = sub_field;
			field->fld_dtype = dtype_blob;
			field->fld_flags |= FLD_blob;
			field->fld_length = 8;
			// CVC: Someone allocated the the max size inside the ary structure,
			// so the dynamic length is irrelevant to create the array_block.
			field_dimensions = /*F.RDB$DIMENSIONS*/
					   isc_34.isc_36;
			fb_assert(field_dimensions <= MAX_ARRAY_DIMENSIONS);
			fb_assert(field_dimensions > 0); // Sometimes, sys tables screw
			field->fld_array_info = array_block = (ary*) MSC_alloc(ARY_LEN);
				//(ary*) MSC_alloc(ARY_LEN(F.RDB$DIMENSIONS));
			array_block->ary_dtype = sub_field->fld_dtype;
		}
	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		CPR_error(NEED_IB4_AT_LEAST);
		CPR_abort();
	/*END_ERROR;*/
	   }
	}

	if (!array_block)
		return;

    dim* last_dimension_block = 0;
    USHORT dimension_counter = 0;
	/*FOR(REQUEST_HANDLE database->dbb_dimension_request)
	D IN RDB$FIELD_DIMENSIONS WITH D.RDB$FIELD_NAME EQ field_name
		SORTED BY ASCENDING D.RDB$DIMENSION*/
	{
        if (!database->dbb_dimension_request)
           isc_compile_request (NULL, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &database->dbb_dimension_request, (short) sizeof(isc_23), (char*) isc_23);
	isc_vtov ((const char*) field_name, (char*) isc_24.isc_25, 32);
        isc_start_and_send (NULL, (FB_API_HANDLE*) &database->dbb_dimension_request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_24, (short) 0);
	while (1)
	   {
           isc_receive (NULL, (FB_API_HANDLE*) &database->dbb_dimension_request, (short) 1, (short) 12, &isc_26, (short) 0);
	   if (!isc_26.isc_29) break;
		array_block->ary_rpt[/*D.RDB$DIMENSION*/
				     isc_26.isc_30].ary_lower = /*D.RDB$LOWER_BOUND*/
	      isc_26.isc_28;
		array_block->ary_rpt[/*D.RDB$DIMENSION*/
				     isc_26.isc_30].ary_upper = /*D.RDB$UPPER_BOUND*/
	      isc_26.isc_27;
		dim* dimension_block = (dim*) MSC_alloc(DIM_LEN);
		dimension_block->dim_number = /*D.RDB$DIMENSION*/
					      isc_26.isc_30;
		dimension_block->dim_lower = /*D.RDB$LOWER_BOUND*/
					     isc_26.isc_28;
		dimension_block->dim_upper = /*D.RDB$UPPER_BOUND*/
					     isc_26.isc_27;

		if (/*D.RDB$DIMENSION*/
		    isc_26.isc_30 != 0)
		{
			last_dimension_block->dim_next = dimension_block;
			dimension_block->dim_previous = last_dimension_block;
		}
		else
			array_block->ary_dimension = dimension_block;
		last_dimension_block = dimension_block;
		++dimension_counter;
	/*END_FOR;*/
	   }
	}

    // Check for lack of sync between RDB$FIELDS & RDB$FIELD_DIMENSIONS,
    // it happened in the past due to buggy DYN code.
	fb_assert(last_dimension_block);
	fb_assert(dimension_counter == field_dimensions);

	array_block->ary_dimension_count = last_dimension_block->dim_number + 1;
	array_block->ary_size = array_size(field);
}


/*____________________________________________________________
 *
 *		Character types can be specified as either:
 *		   b) A POSIX style locale name "<collation>.<characterset>"
 *		   or
 *		   c) A simple <characterset> name (using default collation)
 *		   d) A simple <collation> name    (use charset for collation)
 *
 *		Given an ASCII7 string which could be any of the above, try to
 *		resolve the name in the order b, c, d.
 *		b) is only tried iff the name contains a period.
 *		(in which case c) and d) are not tried).
 *
 *  Return:
 *		true if no errors (and *id is set).
 *		false if the name could not be resolved.
 */

static bool get_intl_char_subtype(SSHORT* id, const UCHAR* name, USHORT length, gpre_dbb* database)
{
	UCHAR buffer[32];			// BASED ON RDB$COLLATION_NAME

	fb_assert(id != NULL);
	fb_assert(name != NULL);
	fb_assert(database != NULL);


	DB = database->dbb_handle;
	if (!DB)
		return false;
	gds_trans = database->dbb_transaction;

	const UCHAR* const end_name = name + length;
	// Force key to uppercase, following C locale rules for uppercasing
	// At the same time, search for the first period in the string (if any)

	UCHAR* period = NULL;
	UCHAR* p;
	for (p = buffer; name < end_name && p < (buffer + sizeof(buffer) - 1); p++, name++)
	{
		*p = UPPER7(*name);
		if ((*p == '.') && !period)
			period = p;
	}
	*p = 0;

	// Is there a period, separating collation name from character set?
	if (period)
	{
		*period = 0;
		return resolve_charset_and_collation(id, period + 1, buffer);
	}

	// Is it a character set name (implying charset default collation sequence)
	bool res = resolve_charset_and_collation(id, buffer, NULL);
	if (!res)
	{
		// Is it a collation name (implying implementation-default character set)
		res = resolve_charset_and_collation(id, NULL, buffer);
	}
	return res;
}


/*____________________________________________________________
 *
 *		Given ASCII7 name of charset & collation
 *		resolve the specification to a ttype (id) that implements
 *		it.
 *
 *  Inputs:
 *		(charset)
 *			ASCII7z name of characterset.
 *			NULL (implying unspecified) means use the character set
 *		        for defined for (collation).
 *
 *		(collation)
 *			ASCII7z name of collation.
 *			NULL means use the default collation for (charset).
 *
 *  Outputs:
 *		(*id)
 *			Set to subtype specified by this name.
 *
 *  Return:
 *		true if no errors (and *id is set).
 *		false if either name not found.
 *		  or if names found, but the collation isn't for the specified
 *		  character set.
 */

static bool resolve_charset_and_collation(SSHORT* id, const UCHAR* charset, const UCHAR* collation)
{
   struct isc_4_struct {
          short isc_5;	/* isc_utility */
          short isc_6;	/* RDB$CHARACTER_SET_ID */
   } isc_4;
   struct isc_1_struct {
          char  isc_2 [32];	/* RDB$TYPE_NAME */
          char  isc_3 [32];	/* RDB$COLLATION_NAME */
   } isc_1;
   struct isc_10_struct {
          short isc_11;	/* isc_utility */
          short isc_12;	/* RDB$CHARACTER_SET_ID */
   } isc_10;
   struct isc_8_struct {
          char  isc_9 [32];	/* RDB$COLLATION_NAME */
   } isc_8;
   struct isc_16_struct {
          short isc_17;	/* isc_utility */
          short isc_18;	/* RDB$CHARACTER_SET_ID */
   } isc_16;
   struct isc_14_struct {
          char  isc_15 [32];	/* RDB$TYPE_NAME */
   } isc_14;
   struct isc_20_struct {
          char  isc_21 [32];	/* RDB$CHARACTER_SET_NAME */
          short isc_22;	/* isc_utility */
   } isc_20;
	fb_assert(id != NULL);

	if (!DB)
		return false;

	bool found = false;
	FB_API_HANDLE request = 0;

	if (collation == NULL)
	{
		if (charset == NULL)
		{
			/*FOR(REQUEST_HANDLE request)
				FIRST 1 DBB IN RDB$DATABASE
				WITH DBB.RDB$CHARACTER_SET_NAME NOT MISSING
				AND DBB.RDB$CHARACTER_SET_NAME != " "*/
			{
                        if (!request)
                           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &request, (short) sizeof(isc_19), (char*) isc_19);
			if (request)
			   {
                           isc_start_request (isc_status, (FB_API_HANDLE*) &request, (FB_API_HANDLE*) &gds_trans, (short) 0);
			   }
			if (!isc_status [1]) {
			while (1)
			   {
                           isc_receive (isc_status, (FB_API_HANDLE*) &request, (short) 0, (short) 34, &isc_20, (short) 0);
			   if (!isc_20.isc_22 || isc_status [1]) break;;

				charset = (UCHAR*) /*DBB.RDB$CHARACTER_SET_NAME*/
						   isc_20.isc_21;

			/*END_FOR*/
			   }
			   };
			/*ON_ERROR*/
			if (isc_status [1])
			   {
				CPR_error(NEED_IB4_AT_LEAST);
				CPR_abort();
			/*END_ERROR;*/
			   }
			}

			isc_release_request(gds_status, &request);

			if (charset == NULL)
				charset = (UCHAR*) DEFAULT_CHARACTER_SET_NAME;
		}

		/*FOR(REQUEST_HANDLE request)
			FIRST 1 CS IN RDB$CHARACTER_SETS
			CROSS COLL IN RDB$COLLATIONS
			CROSS TYPE IN RDB$TYPES
			WITH TYPE.RDB$TYPE_NAME EQ charset
			AND TYPE.RDB$FIELD_NAME EQ "RDB$CHARACTER_SET_NAME"
			AND TYPE.RDB$TYPE EQ CS.RDB$CHARACTER_SET_ID
			AND CS.RDB$DEFAULT_COLLATE_NAME EQ COLL.RDB$COLLATION_NAME*/
		{
                if (!request)
                   isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &request, (short) sizeof(isc_13), (char*) isc_13);
		isc_vtov ((const char*) charset, (char*) isc_14.isc_15, 32);
		if (request)
		   {
                   isc_start_and_send (isc_status, (FB_API_HANDLE*) &request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_14, (short) 0);
		   }
		if (!isc_status [1]) {
		while (1)
		   {
                   isc_receive (isc_status, (FB_API_HANDLE*) &request, (short) 1, (short) 4, &isc_16, (short) 0);
		   if (!isc_16.isc_17 || isc_status [1]) break;;

			found = true;
			*id = MAP_CHARSET_TO_TTYPE(/*CS.RDB$CHARACTER_SET_ID*/
						   isc_16.isc_18);

		/*END_FOR*/
		   }
		   };
		/*ON_ERROR*/
		if (isc_status [1])
		   {
			// Nothing
		/*END_ERROR;*/
		   }
		}

		isc_release_request(gds_status, &request);

		return found;
	}

	if (charset == NULL)
	{
		/*FOR(REQUEST_HANDLE request)
			FIRST 1 CS IN RDB$CHARACTER_SETS
			CROSS COLL IN RDB$COLLATIONS
			OVER RDB$CHARACTER_SET_ID
			WITH COLL.RDB$COLLATION_NAME EQ collation*/
		{
                if (!request)
                   isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &request, (short) sizeof(isc_7), (char*) isc_7);
		isc_vtov ((const char*) collation, (char*) isc_8.isc_9, 32);
		if (request)
		   {
                   isc_start_and_send (isc_status, (FB_API_HANDLE*) &request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 32, &isc_8, (short) 0);
		   }
		if (!isc_status [1]) {
		while (1)
		   {
                   isc_receive (isc_status, (FB_API_HANDLE*) &request, (short) 1, (short) 4, &isc_10, (short) 0);
		   if (!isc_10.isc_11 || isc_status [1]) break;;

			found = true;
			*id = MAP_CHARSET_TO_TTYPE(/*CS.RDB$CHARACTER_SET_ID*/
						   isc_10.isc_12);
		/*END_FOR*/
		   }
		   };
		/*ON_ERROR*/
		if (isc_status [1])
		   {
			// Nothing
		/*END_ERROR;*/
		   }
		}

		isc_release_request(gds_status, &request);

		return found;
	}

	/*FOR(REQUEST_HANDLE request)
		FIRST 1 CS IN RDB$CHARACTER_SETS
		CROSS COL IN RDB$COLLATIONS OVER RDB$CHARACTER_SET_ID
		CROSS NAME2 IN RDB$TYPES
		WITH COL.RDB$COLLATION_NAME EQ collation
		AND NAME2.RDB$TYPE_NAME EQ charset
		AND NAME2.RDB$FIELD_NAME EQ "RDB$CHARACTER_SET_NAME"
		AND NAME2.RDB$TYPE EQ CS.RDB$CHARACTER_SET_ID*/
	{
        if (!request)
           isc_compile_request (isc_status, (FB_API_HANDLE*) &DB, (FB_API_HANDLE*) &request, (short) sizeof(isc_0), (char*) isc_0);
	isc_vtov ((const char*) charset, (char*) isc_1.isc_2, 32);
	isc_vtov ((const char*) collation, (char*) isc_1.isc_3, 32);
	if (request)
	   {
           isc_start_and_send (isc_status, (FB_API_HANDLE*) &request, (FB_API_HANDLE*) &gds_trans, (short) 0, (short) 64, &isc_1, (short) 0);
	   }
	if (!isc_status [1]) {
	while (1)
	   {
           isc_receive (isc_status, (FB_API_HANDLE*) &request, (short) 1, (short) 4, &isc_4, (short) 0);
	   if (!isc_4.isc_5 || isc_status [1]) break;;

		found = true;
		*id = MAP_CHARSET_TO_TTYPE(/*CS.RDB$CHARACTER_SET_ID*/
					   isc_4.isc_6);
	/*END_FOR*/
	   }
	   };
	/*ON_ERROR*/
	if (isc_status [1])
	   {
		// Nothing
	/*END_ERROR;*/
	   }
	}

	isc_release_request(gds_status, &request);

	return found;
}


#ifdef NOT_USED_OR_REPLACED
/*____________________________________________________________
 *
 *		Upcase a string into another string.  Return
 *		length of string.
 */

static int upcase(const TEXT* from, TEXT* const to)
{
	TEXT* p = to;
	const TEXT* const end = to + NAME_SIZE;

	TEXT c;
	while (p < end && (c = *from++)) {
		*p++ = UPPER(c);
	}

	*p = 0;

	return p - to;
}
#endif

