#
#  PROJ4.1 Projection transformation library.
#
#  Copyright (c) 1997 Logiciels et Applications Scientifiques (L.A.S.) Inc.
#  Il est strictement interdit de publier ou de devoiler le contenu de ce
#  programme sans avoir prealablement obtenu la permission de L.A.S. Inc.
#  It is strictly forbidden to publish or divulge the content of
#  these programs without the prior permission of L.A.S. Inc.
#

TOBEGEN = proj
TARGETGEN = $(ARCHGEN)

SOURCES	= pj_aea.c pj_aeqd.c pj_airy.c pj_aitoff.c \
	  pj_august.c pj_bacon.c pj_bipc.c pj_boggs.c \
	  pj_bonne.c pj_cass.c pj_cc.c pj_cea.c \
	  pj_collg.c pj_dense.c pj_eck1.c pj_eck2.c \
	  pj_eck3.c pj_eck4.c pj_eck5.c pj_eck6.c \
	  pj_eisen.c pj_eqc.c pj_eqdc.c pj_fourn.c \
	  pj_gall.c pj_gnom.c pj_hammer.c pj_hataea.c \
	  pj_laea.c pj_lagrng.c pj_lcc.c pj_loxim.c \
	  pj_mbtfpp.c pj_mbtfpq.c pj_mbtfps.c pj_merc.c \
	  pj_mill.c pj_moll.c pj_nocol.c pj_nsper.c \
	  pj_ocea.c pj_omerc.c pj_ortho.c pj_parab.c \
	  pj_pconic.c pj_poly.c pj_putp2.c pj_putp5.c \
	  pj_quau.c pj_robin.c pj_rpoly.c pj_sinu.c \
	  pj_stere.c pj_tcc.c pj_tcea.c pj_tmerc.c \
	  pj_vandg.c pj_vandg2.c pj_vandg4.c pj_wag7.c \
	  pj_wink1.c pj_goode.c pj_chamb.c pj_lsat.c \
	  pj_mod_ster.c pj_tpeqd.c pj_nzmg.c pj_labrd.c \
	  pj_ob_tran.c \
	  pj_auth.c pj_fwd.c pj_init.c pj_inv.c \
	  pj_msfn.c pj_mlfn.c pj_phi2.c pj_qsfn.c \
	  pj_tsfn.c mk_cheby.c pj_list.c vector1.c \
	  bchgen.c bch2bps.c biveval.c pj_open_lib.c \
	  pj_ellps.c pj_factors.c pj_deriv.c aasincos.c \
	  pj_param.c pj_ell_set.c pj_units.c pj_pr_list.c \
	  pj_zpoly1.c pj_malloc.c pj_strerrno.c nad_cvt.c \
	  nad_init.c nad_interp.c \
	  adjlon.c dmstor.c rtodms.c \
	  pj_errno.c $(HYPOT) $(STRTOD)

INCLUDES     	= $(CURRENT_INCLUDE)

CFLAGS 		= $(INCLUDES) $(COMMON_CFLAGS) 

include $(TOPDIR)/config/common.mak

all: MKOBJECTDIR 
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)

uninstall:
	-$(RM) $(PROJ_LIB)

clean: default-clean
