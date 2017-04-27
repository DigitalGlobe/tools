!  The contents of this file are subject to the Interbase Public
!  License Version 1.0 (the "License"); you may not use this file
!  except in compliance with the License. You may obtain a copy
!  of the License at http://www.Inprise.com/IPL.html
!
!  Software distributed under the License is distributed on an
!  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
!  or implied. See the License for the specific language governing
!  rights and limitations under the License.
!
!  The Original Code was created by Inprise Corporation
!  and its predecessors. Portions created by Inprise Corporation are
!  Copyright (C) Inprise Corporation.
!
!  All Rights Reserved.
!  Contributor(s): ______________________________________.

	!*
	!*	PROGRAM:	BASIC preprocessor
	!*	MODULE:		gds.bas
	!*	DESCRIPTION:	BLR constants
	!*
	
! Error codes

	DECLARE LONG CONSTANT gds_facility = 20
	DECLARE LONG CONSTANT gds_err_base = 335544320
	DECLARE LONG CONSTANT gds_err_factor = 1
	DECLARE LONG CONSTANT    gds_arg_end  = 0 
	DECLARE LONG CONSTANT    gds_arg_gds  = 1 
	DECLARE LONG CONSTANT    gds_arg_string = 2 
	DECLARE LONG CONSTANT    gds_arg_cstring = 3 
	DECLARE LONG CONSTANT    gds_arg_number = 4 
	DECLARE LONG CONSTANT    gds_arg_interpreted = 5 
	DECLARE LONG CONSTANT    gds_arg_vms  = 6 
	DECLARE LONG CONSTANT    gds_arg_unix = 7 
	DECLARE LONG CONSTANT    gds_arg_domain = 8 
        DECLARE LONG CONSTANT    gds_err_max = 138

	DECLARE LONG CONSTANT gds__arith_except        	= 335544321
	DECLARE LONG CONSTANT gds__bad_dbkey           	= 335544322
	DECLARE LONG CONSTANT gds__bad_db_format       	= 335544323
	DECLARE LONG CONSTANT gds__bad_db_handle       	= 335544324
	DECLARE LONG CONSTANT gds__bad_dpb_content     	= 335544325
	DECLARE LONG CONSTANT gds__bad_dpb_form        	= 335544326
	DECLARE LONG CONSTANT gds__bad_req_handle      	= 335544327
	DECLARE LONG CONSTANT gds__bad_segstr_handle   	= 335544328
	DECLARE LONG CONSTANT gds__bad_segstr_id       	= 335544329
	DECLARE LONG CONSTANT gds__bad_tpb_content     	= 335544330
	DECLARE LONG CONSTANT gds__bad_tpb_form        	= 335544331
	DECLARE LONG CONSTANT gds__bad_trans_handle    	= 335544332
	DECLARE LONG CONSTANT gds__bug_check           	= 335544333
	DECLARE LONG CONSTANT gds__convert_error       	= 335544334
	DECLARE LONG CONSTANT gds__db_corrupt          	= 335544335
	DECLARE LONG CONSTANT gds__deadlock            	= 335544336
	DECLARE LONG CONSTANT gds__excess_trans        	= 335544337
	DECLARE LONG CONSTANT gds__from_no_match       	= 335544338
	DECLARE LONG CONSTANT gds__infinap             	= 335544339
	DECLARE LONG CONSTANT gds__infona              	= 335544340
	DECLARE LONG CONSTANT gds__infunk              	= 335544341
	DECLARE LONG CONSTANT gds__integ_fail          	= 335544342
	DECLARE LONG CONSTANT gds__invalid_blr         	= 335544343
	DECLARE LONG CONSTANT gds__io_error            	= 335544344
	DECLARE LONG CONSTANT gds__lock_conflict       	= 335544345
	DECLARE LONG CONSTANT gds__metadata_corrupt    	= 335544346
	DECLARE LONG CONSTANT gds__not_valid           	= 335544347
	DECLARE LONG CONSTANT gds__no_cur_rec          	= 335544348
	DECLARE LONG CONSTANT gds__no_dup              	= 335544349
	DECLARE LONG CONSTANT gds__no_finish           	= 335544350
	DECLARE LONG CONSTANT gds__no_meta_update      	= 335544351
	DECLARE LONG CONSTANT gds__no_priv             	= 335544352
	DECLARE LONG CONSTANT gds__no_recon            	= 335544353
	DECLARE LONG CONSTANT gds__no_record           	= 335544354
	DECLARE LONG CONSTANT gds__no_segstr_close     	= 335544355
	DECLARE LONG CONSTANT gds__obsolete_metadata   	= 335544356
	DECLARE LONG CONSTANT gds__open_trans          	= 335544357
	DECLARE LONG CONSTANT gds__port_len            	= 335544358
	DECLARE LONG CONSTANT gds__read_only_field     	= 335544359
	DECLARE LONG CONSTANT gds__read_only_rel       	= 335544360
	DECLARE LONG CONSTANT gds__read_only_trans     	= 335544361
	DECLARE LONG CONSTANT gds__read_only_view      	= 335544362
	DECLARE LONG CONSTANT gds__req_no_trans        	= 335544363
	DECLARE LONG CONSTANT gds__req_sync            	= 335544364
	DECLARE LONG CONSTANT gds__req_wrong_db        	= 335544365
	DECLARE LONG CONSTANT gds__segment             	= 335544366
	DECLARE LONG CONSTANT gds__segstr_eof          	= 335544367
	DECLARE LONG CONSTANT gds__segstr_no_op        	= 335544368
	DECLARE LONG CONSTANT gds__segstr_no_read      	= 335544369
	DECLARE LONG CONSTANT gds__segstr_no_trans     	= 335544370
	DECLARE LONG CONSTANT gds__segstr_no_write     	= 335544371
	DECLARE LONG CONSTANT gds__segstr_wrong_db     	= 335544372
	DECLARE LONG CONSTANT gds__sys_request         	= 335544373
	DECLARE LONG CONSTANT gds__unavailable         	= 335544375
	DECLARE LONG CONSTANT gds__unres_rel           	= 335544376
	DECLARE LONG CONSTANT gds__uns_ext             	= 335544377
	DECLARE LONG CONSTANT gds__wish_list           	= 335544378
	DECLARE LONG CONSTANT gds__wrong_ods           	= 335544379
	DECLARE LONG CONSTANT gds__wronumarg           	= 335544380
	DECLARE LONG CONSTANT gds__imp_exc             	= 335544381
	DECLARE LONG CONSTANT gds__random              	= 335544382
	DECLARE LONG CONSTANT gds__fatal_conflict      	= 335544383

! Minor codes subject to change

	DECLARE LONG CONSTANT gds__badblk              	= 335544384
	DECLARE LONG CONSTANT gds__invpoolcl           	= 335544385
	DECLARE LONG CONSTANT gds__nopoolids           	= 335544386
	DECLARE LONG CONSTANT gds__relbadblk           	= 335544387
	DECLARE LONG CONSTANT gds__blktoobig           	= 335544388
	DECLARE LONG CONSTANT gds__bufexh              	= 335544389
	DECLARE LONG CONSTANT gds__syntaxerr           	= 335544390
	DECLARE LONG CONSTANT gds__bufinuse            	= 335544391
	DECLARE LONG CONSTANT gds__bdbincon            	= 335544392
	DECLARE LONG CONSTANT gds__reqinuse            	= 335544393
	DECLARE LONG CONSTANT gds__badodsver           	= 335544394
	DECLARE LONG CONSTANT gds__relnotdef           	= 335544395
	DECLARE LONG CONSTANT gds__fldnotdef           	= 335544396
	DECLARE LONG CONSTANT gds__dirtypage           	= 335544397
	DECLARE LONG CONSTANT gds__waifortra           	= 335544398
	DECLARE LONG CONSTANT gds__doubleloc           	= 335544399
	DECLARE LONG CONSTANT gds__nodnotfnd           	= 335544400
	DECLARE LONG CONSTANT gds__dupnodfnd           	= 335544401
	DECLARE LONG CONSTANT gds__locnotmar           	= 335544402
	DECLARE LONG CONSTANT gds__badpagtyp           	= 335544403
	DECLARE LONG CONSTANT gds__corrupt             	= 335544404
	DECLARE LONG CONSTANT gds__badpage             	= 335544405
	DECLARE LONG CONSTANT gds__badindex            	= 335544406
	DECLARE LONG CONSTANT gds__dbbnotzer           	= 335544407
	DECLARE LONG CONSTANT gds__tranotzer           	= 335544408
	DECLARE LONG CONSTANT gds__trareqmis           	= 335544409
	DECLARE LONG CONSTANT gds__badhndcnt           	= 335544410
	DECLARE LONG CONSTANT gds__wrotpbver           	= 335544411
	DECLARE LONG CONSTANT gds__wroblrver           	= 335544412
	DECLARE LONG CONSTANT gds__wrodpbver           	= 335544413
	DECLARE LONG CONSTANT gds__blobnotsup          	= 335544414
	DECLARE LONG CONSTANT gds__badrelation         	= 335544415
	DECLARE LONG CONSTANT gds__nodetach            	= 335544416
	DECLARE LONG CONSTANT gds__notremote           	= 335544417
	DECLARE LONG CONSTANT gds__trainlim            	= 335544418
	DECLARE LONG CONSTANT gds__notinlim            	= 335544419
	DECLARE LONG CONSTANT gds__traoutsta           	= 335544420
	DECLARE LONG CONSTANT gds__connect_reject      	= 335544421
	DECLARE LONG CONSTANT gds__dbfile              	= 335544422
	DECLARE LONG CONSTANT gds__orphan              	= 335544423
	DECLARE LONG CONSTANT gds__no_lock_mgr         	= 335544424
	DECLARE LONG CONSTANT gds__ctxinuse            	= 335544425
	DECLARE LONG CONSTANT gds__ctxnotdef           	= 335544426
	DECLARE LONG CONSTANT gds__datnotsup           	= 335544427
	DECLARE LONG CONSTANT gds__badmsgnum           	= 335544428
	DECLARE LONG CONSTANT gds__badparnum           	= 335544429
	DECLARE LONG CONSTANT gds__virmemexh           	= 335544430
	DECLARE LONG CONSTANT gds__blocking_signal     	= 335544431
	DECLARE LONG CONSTANT gds__lockmanerr          	= 335544432
	DECLARE LONG CONSTANT gds__journerr            	= 335544433
	DECLARE LONG CONSTANT gds__keytoobig           	= 335544434
	DECLARE LONG CONSTANT gds__nullsegkey          	= 335544435
	DECLARE LONG CONSTANT gds__sqlerr              	= 335544436	
	DECLARE LONG CONSTANT gds__wrodynver           	= 335544437
        DECLARE LONG CONSTANT gds__obj_in_use           = 335544453
        DECLARE LONG CONSTANT gds__nofilter             = 335544454
        DECLARE LONG CONSTANT gds__shadow_accessed      = 335544455
        DECLARE LONG CONSTANT gds__invalid_sdl          = 335544456
        DECLARE LONG CONSTANT gds__out_of_bounds        = 335544457
        DECLARE LONG CONSTANT gds__invalid_dimension    = 335544458

! Database parameter block stuff 

	DECLARE BYTE CONSTANT gds__dpb_version1			= 1

	DECLARE BYTE CONSTANT gds__dpb_cdd_pathname		= 1
	DECLARE BYTE CONSTANT gds__dpb_allocation		= 2
	DECLARE BYTE CONSTANT gds__dpb_journal 			= 3
	DECLARE BYTE CONSTANT gds__dpb_page_size 		= 4
	DECLARE BYTE CONSTANT gds__dpb_num_buffers 		= 5
	DECLARE BYTE CONSTANT gds__dpb_buffer_length		= 6
	DECLARE BYTE CONSTANT gds__dpb_debug 			= 7
	DECLARE BYTE CONSTANT gds__dpb_garbage_collect		= 8
	DECLARE BYTE CONSTANT gds__dpb_verify 			= 9
	DECLARE BYTE CONSTANT gds__dpb_sweep 			= 10
	DECLARE BYTE CONSTANT gds__dpb_enable_journal		= 11
	DECLARE BYTE CONSTANT gds__dpb_disable_journal		= 12
	DECLARE BYTE CONSTANT gds__dpb_dbkey_scope		= 13
	DECLARE BYTE CONSTANT gds__dpb_number_of_users		= 14
	DECLARE BYTE CONSTANT gds__dpb_trace			= 15
	DECLARE BYTE CONSTANT gds__dpb_no_garbage_collect	= 16
	DECLARE BYTE CONSTANT gds__dpb_damaged			= 17
	DECLARE BYTE CONSTANT gds__dpb_license			= 18
	DECLARE BYTE CONSTANT gds__dpb_sys_user_name		= 19
	DECLARE BYTE CONSTANT gds__dpb_encrypt_key		= 20
	DECLARE BYTE CONSTANT gds__dpb_activate_shadow		= 21
	DECLARE BYTE CONSTANT gds__dpb_sweep_interval		= 22
	DECLARE BYTE CONSTANT gds__dpb_delete_shadow		= 23

	DECLARE BYTE CONSTANT gds__dpb_pages			= 1
	DECLARE BYTE CONSTANT gds__dpb_records			= 2
	DECLARE BYTE CONSTANT gds__dpb_indices			= 4
	DECLARE BYTE CONSTANT gds__dpb_transactions		= 8
	DECLARE BYTE CONSTANT gds__dpb_no_update		= 16
	DECLARE BYTE CONSTANT gds__dpb_repair			= 32
	DECLARE BYTE CONSTANT gds__dpb_ignore			= 64

! Bit assignments in RDB$SYSTEM_FLAG 

	DECLARE BYTE CONSTANT RDB_system		= 1
	DECLARE BYTE CONSTANT RDB_id_assigned		= 2

! Transaction parameter blob stuff 

	DECLARE BYTE CONSTANT gds__tpb_version1		= 1
	DECLARE BYTE CONSTANT gds__tpb_version3		= 3
	DECLARE BYTE CONSTANT gds__tpb_consistency	= 1
	DECLARE BYTE CONSTANT gds__tpb_concurrency	= 2
	DECLARE BYTE CONSTANT gds__tpb_shared		= 3
	DECLARE BYTE CONSTANT gds__tpb_protected	= 4
	DECLARE BYTE CONSTANT gds__tpb_exclusive	= 5
	DECLARE BYTE CONSTANT gds__tpb_wait		= 6
	DECLARE BYTE CONSTANT gds__tpb_nowait		= 7
	DECLARE BYTE CONSTANT gds__tpb_read		= 8
	DECLARE BYTE CONSTANT gds__tpb_write		= 9
	DECLARE BYTE CONSTANT gds__tpb_lock_read	= 10
	DECLARE BYTE CONSTANT gds__tpb_lock_write	= 11
	DECLARE BYTE CONSTANT gds__tpb_verb_time	= 12
	DECLARE BYTE CONSTANT gds__tpb_commit_time	= 13
	DECLARE BYTE CONSTANT gds__tpb_ignore_limbo	= 14

! Blob Parameter Block

	DECLARE BYTE CONSTANT gds__bpb_version1		= 1
	DECLARE BYTE CONSTANT gds__bpb_source_type	= 1
	DECLARE BYTE CONSTANT gds__bpb_target_type	= 2
	DECLARE BYTE CONSTANT gds__bpb_type		= 3

	DECLARE BYTE CONSTANT gds__bpb_type_segmented	= 0
	DECLARE BYTE CONSTANT gds__bpb_type_stream	= 1


! Dynamic SQL datatypes 

	DECLARE WORD CONSTANT SQL_TEXT		= 452
	DECLARE WORD CONSTANT SQL_VARYING	= 448
	DECLARE WORD CONSTANT SQL_SHORT		= 500
	DECLARE WORD CONSTANT SQL_LONG		= 496
	DECLARE WORD CONSTANT SQL_DOUBLE	= 480
	DECLARE WORD CONSTANT SQL_D_FLOAT	= 530
	DECLARE WORD CONSTANT SQL_FLOAT		= 482
	DECLARE WORD CONSTANT SQL_DATE		= 510
	DECLARE WORD CONSTANT SQL_BLOB		= 520

! Forms Package definitions 

! Map definition block definitions 

	DECLARE BYTE CONSTANT PYXIS__MAP_VERSION1		= 1
	DECLARE BYTE CONSTANT PYXIS__MAP_FIELD2			= 2
	DECLARE BYTE CONSTANT PYXIS__MAP_FIELD1			= 3
	DECLARE BYTE CONSTANT PYXIS__MAP_MESSAGE		= 4
	DECLARE BYTE CONSTANT PYXIS__MAP_TERMINATOR		= 5
	DECLARE BYTE CONSTANT PYXIS__MAP_TERMINATING_FIELD	= 6
	DECLARE BYTE CONSTANT PYXIS__MAP_OPAQUE			= 7
	DECLARE BYTE CONSTANT PYXIS__MAP_TRANSPARENT		= 8
	DECLARE BYTE CONSTANT PYXIS__MAP_TAG			= 9
	DECLARE BYTE CONSTANT PYXIS__MAP_SUB_FORM		= 10
	DECLARE BYTE CONSTANT PYXIS__MAP_ITEM_INDEX		= 11
	DECLARE BYTE CONSTANT PYXIS__MAP_SUB_FIELD		= 12
	DECLARE BYTE CONSTANT PYXIS__MAP_END			= -1

! Field option flags for display options 

	DECLARE WORD CONSTANT PYXIS__OPT_DISPLAY	= 1
	DECLARE WORD CONSTANT PYXIS__OPT_UPDATE		= 2
	DECLARE WORD CONSTANT PYXIS__OPT_WAKEUP		= 4
	DECLARE WORD CONSTANT PYXIS__OPT_POSITION	= 8

! Field option values following display 

	DECLARE WORD CONSTANT PYXIS__OPT_NULL		= 1
	DECLARE WORD CONSTANT PYXIS__OPT_DEFAULT	= 2
	DECLARE WORD CONSTANT PYXIS__OPT_INITIAL	= 3
	DECLARE WORD CONSTANT PYXIS__OPT_USER_DATA	= 4

! Pseudo key definitions 

	DECLARE WORD CONSTANT PYXIS__KEY_DELETE		= 127
	DECLARE WORD CONSTANT PYXIS__KEY_UP		= 128
	DECLARE WORD CONSTANT PYXIS__KEY_DOWN		= 129
	DECLARE WORD CONSTANT PYXIS__KEY_RIGHT		= 130
	DECLARE WORD CONSTANT PYXIS__KEY_LEFT		= 131
	DECLARE WORD CONSTANT PYXIS__KEY_PF1		= 132
	DECLARE WORD CONSTANT PYXIS__KEY_PF2		= 133
	DECLARE WORD CONSTANT PYXIS__KEY_PF3		= 134
	DECLARE WORD CONSTANT PYXIS__KEY_PF4		= 135
	DECLARE WORD CONSTANT PYXIS__KEY_PF5		= 136
	DECLARE WORD CONSTANT PYXIS__KEY_PF6		= 137
	DECLARE WORD CONSTANT PYXIS__KEY_PF7		= 138
	DECLARE WORD CONSTANT PYXIS__KEY_PF8		= 139
	DECLARE WORD CONSTANT PYXIS__KEY_PF9		= 140
	DECLARE WORD CONSTANT PYXIS__KEY_ENTER		= 141
	DECLARE WORD CONSTANT PYXIS__KEY_SCROLL_TOP	= 146
	DECLARE WORD CONSTANT PYXIS__KEY_SCROLL_BOTTOM	= 147

! Menu definition stuff 

	DECLARE BYTE CONSTANT PYXIS__MENU_VERSION1	= 1
	DECLARE BYTE CONSTANT PYXIS__MENU_LABEL		= 2
	DECLARE BYTE CONSTANT PYXIS__MENU_ENTREE	= 3
	DECLARE BYTE CONSTANT PYXIS__MENU_OPAQUE	= 4
	DECLARE BYTE CONSTANT PYXIS__MENU_TRANSPARENT	= 5
	DECLARE BYTE CONSTANT PYXIS__MENU_HORIZONTAL	= 6
	DECLARE BYTE CONSTANT PYXIS__MENU_VERTICAL	= 7
	DECLARE BYTE CONSTANT PYXIS__MENU_END		= -1


	EXTERNAL LONG FUNCTION GDS__SQLCODE BY REF
	EXTERNAL SUB GDS__ATTACH_DATABASE BY REF
	EXTERNAL SUB GDS__CANCEL_BLOB BY REF
        EXTERNAL SUB GDS__CANCEL_EVENTS BY REF
	EXTERNAL SUB GDS__CLOSE_BLOB BY REF
        EXTERNAL SUB GDS__COMMIT_RETAINING BY REF
	EXTERNAL SUB GDS__COMMIT_TRANSACTION BY REF
	EXTERNAL SUB GDS__COMPILE_REQUEST BY REF
	EXTERNAL SUB GDS__CREATE_BLOB BY REF
        EXTERNAL SUB GDS__CREATE_BLOB2 BY REF
	EXTERNAL SUB GDS__CREATE_DATABASE BY REF
	EXTERNAL SUB GDS__DETACH_DATABASE BY REF
        EXTERNAL SUB GDS_EVENT_WAIT BY REF
	EXTERNAL LONG FUNCTION GDS__GET_SEGMENT BY REF
	EXTERNAL SUB GDS__OPEN_BLOB BY REF
        EXTERNAL SUB GDS__OPEN_BLOB2 BY REF
	EXTERNAL SUB GDS__PREPARE_TRANSACTION BY REF
        EXTERNAL SUB GDS__PREPARE_TRANSACTION2 BY REF
	EXTERNAL LONG FUNCTION GDS__PUT_SEGMENT BY REF
        EXTERNAL SUB GDS__QUE_EVENTS BY REF
	EXTERNAL SUB GDS__RECEIVE BY REF
	EXTERNAL SUB GDS__RELEASE_REQUEST BY REF
	EXTERNAL SUB GDS__ROLLBACK_TRANSACTION BY REF
	EXTERNAL SUB GDS__SEND BY REF
        EXTERNAL SUB gds__set_debug BY REF
	EXTERNAL SUB GDS__START_AND_SEND BY REF
	EXTERNAL SUB GDS__START_REQUEST BY REF
	EXTERNAL SUB GDS__START_MULTIPLE BY REF
	EXTERNAL SUB GDS__START_TRANSACTION BY REF
	EXTERNAL SUB GDS__UNWIND_REQUEST BY REF
	EXTERNAL SUB gds__print_status BY REF
	EXTERNAL SUB gds__encode_date BY REF
	EXTERNAL SUB gds__decode_date BY REF
	EXTERNAL SUB blob__display BY REF
	EXTERNAL SUB blob__dump BY REF
	EXTERNAL SUB blob__edit BY REF
	EXTERNAL SUB blob__load BY REF
	EXTERNAL SUB gds__close BY REF
	EXTERNAL SUB gds__declare BY REF
	EXTERNAL SUB gds__describe BY REF
	EXTERNAL SUB gds__dsql_finish BY REF
	EXTERNAL SUB gds__execute BY REF
	EXTERNAL SUB gds__execute_immediate BY REF
	EXTERNAL LONG FUNCTION gds__fetch BY REF
	EXTERNAL SUB gds__open BY REF
	EXTERNAL SUB gds__prepare BY REF
        EXTERNAL SUB gds__open_blob_filtered BY REF
        EXTERNAL SUB gds__get_segment_filtered BY REF
        EXTERNAL SUB gds__close_blob_filtered BY REF
        EXTERNAL SUB gds__event_counts BY REF
        EXTERNAL SUB gds__event_block BY REF
        EXTERNAL SUB gds__get_slice BY REF
        EXTERNAL SUB gds__put_slice BY REF
        EXTERNAL SUB gds__seek_blob BY REF
        EXTERNAL SUB gds__ddl BY REF
	EXTERNAL SUB pyxis__compile_map BY REF
	EXTERNAL SUB pyxis__compile_sub_map BY REF
	EXTERNAL SUB pyxis__create_window BY REF
	EXTERNAL SUB pyxis__delete_window BY REF
	EXTERNAL SUB pyxis__drive_form BY REF
	EXTERNAL SUB pyxis__fetch BY REF
	EXTERNAL SUB pyxis__insert BY REF
	EXTERNAL SUB pyxis__load_form BY REF
	EXTERNAL WORD FUNCTION pyxis__menu BY REF
	EXTERNAL SUB pyxis__pop_window BY REF
	EXTERNAL SUB pyxis__reset_form BY REF
	EXTERNAL SUB pyxis__drive_menu BY REF
	EXTERNAL SUB pyxis__initialize_menu BY REF
	EXTERNAL SUB pyxis__get_entree BY REF
	EXTERNAL SUB pyxis__put_entree BY REF


