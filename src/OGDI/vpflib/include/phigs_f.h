/*
@PHIGS Functions
*/

#ifndef H_PHIGS_F
#define H_PHIGS_F

#ifndef H_PHIGS_D
#include "phigs_d.h"
#endif          /* H_PHIGS_DEF */

#ifndef H_COLOR_D
#include "color_d.h"
#endif

/***************************************************************
@    pcell_array()
****************************************************************
Draw PHIGS Cell Array.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pcell_array(Prect *rectangle, Ppat_rep colr_array);
#else
ERRSTATUS MUSE_API pcell_array();
#endif
/*
Description:
The image contained in colr_array is stretched to fit the
rectangle.
*/

/***************************************************************
@    pclose_phigs()
****************************************************************
Close PHIGS Graphics.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pclose_phigs(void);
#else
ERRSTATUS MUSE_API pclose_phigs();
#endif
/*
Description:
Terminate PHIGS graphics.
*/

/***************************************************************
@    pclose_structure()
****************************************************************
Close a PHIGS structure.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pclose_struct(void);
#else
ERRSTATUS MUSE_API pclose_struct();
#endif
/*
Description:
Close a PHIGS graphics structurte.
*/

/***************************************************************
@    pclose_ws()
****************************************************************
Close a PHIGS Workstation.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pclose_ws(Pint ws_id);
#else
ERRSTATUS MUSE_API pclose_ws();
#endif
/*
Description:
A workstation is a XVT window.
*/

/***************************************************************
@    pdel_struct()
****************************************************************
Delete PHIGS structure.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pdel_struct(Pint struct_id);
#else
ERRSTATUS MUSE_API pdel_struct();
#endif
/*
Description:
Delete a PHIGS graphics structure.
*/

/***************************************************************
@    pemergency_close_phigs()
****************************************************************
Emergency PHIGS close.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pemergency_close_phigs(void);
#else
ERRSTATUS MUSE_API pemergency_close_phigs();
#endif
/*
Description:
Emergency program terminitation.
*/

/***************************************************************
@    pfill_area_set()
****************************************************************
PHIGS fill area set.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pfill_area_set(Ppoint_list_list *sets);
#else
ERRSTATUS MUSE_API pfill_area_set();
#endif
/*
Description:
Display a set of fill areas.  This has the
capability to display holes in fill areas.
*/

/***************************************************************
@    pinq_bitmap()
****************************************************************
Inquire bitmap from operating system.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pinq_bitmap(Pint ws_id , GHANDLE bitmap, PALETTE_USAGE palette_usage );
#else
ERRSTATUS MUSE_API pinq_bitmap();
#endif
/*
Description:
This function will return a bitmap of the screen
from the underlying graphics system.
*/

/***************************************************************
@    pinq_open_wss()
****************************************************************
Inquire open workstation setup.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pinq_open_wss(Pint length, Pint start, Pint *error,
		       Pint_list *ws_id_list, Pint *total_length);
#else
ERRSTATUS MUSE_API pinq_open_wss();
#endif
/*
Description:
This function will determine whether a workstation
is open or not.
*/

/***************************************************************
@    pinq_wss_trans()
****************************************************************
Inquire current PHIGS transformation.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pinq_wss_trans(Pint ws, Pint *error_ind, Pupd_st *upd_st,
		       Plimit *cur_win_lim, Plimit *cur_vp_lim,
		       Pvec *scale_vector, Pvec *trans_vector);
#else
ERRSTATUS MUSE_API pinq_wss_trans();
#endif
/*
Description:
Returns the current setup of the display
transformation.
*/

/***************************************************************
@    popen_phigs()
****************************************************************
Open PHIGS graphics.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API popen_phigs(char *error_file, size_t memory);
#else
ERRSTATUS MUSE_API popen_phigs();
#endif
/*
Description:
Must be the first PHIGS call to initialize PHIGS.
*/

/***************************************************************
@    popen_struct()
****************************************************************
Open PHIGS graphics structure.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API popen_struct(Pint struct_id);
#else
ERRSTATUS MUSE_API popen_struct();
#endif
/*
Description:
Open an existing structure or create a new one.
*/

/***************************************************************
@    popen_ws()
****************************************************************
Open PHIGS Workstation.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API popen_ws(Pint ws_id, Pconnid *conn_id, Pint ws_type);
#else
ERRSTATUS MUSE_API popen_ws();
#endif
/*
Description:
A PHIGS workstation is a XVT window.
*/

/***************************************************************
@    ppolyline()
****************************************************************
Draw PHIGS polyline.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API ppolyline(Ppoint_list *point_list);
#else
ERRSTATUS MUSE_API ppolyline();
#endif
/*
Description:
Point_list contains the points to be connected using the
current line style and color.
*/

/***************************************************************
@    ppolymarker()
****************************************************************
Draw PHIGS polymakers.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API ppolymarker(Ppoint_list *point_list);
#else
ERRSTATUS MUSE_API ppolymarker();
#endif
/*
Description:
Point_list contains the points at which to place the markers.
*/

/***************************************************************
@    ppost_struct()
****************************************************************
Post a PHIGS graphics structure.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API ppost_struct(Pint ws_id, Pint struct_id, Pfloat priority);
#else
ERRSTATUS MUSE_API ppost_struct();
#endif
/*
Description:
Sends the PHIGS structure to a workstation.
PHIGS structures are created by opening, drawing into , and
closing them.
*/

/***************************************************************
@    pput_pal()
****************************************************************
Put a PHIGS palette.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pput_pal(Pint ws_id, PALETTE *palette);
#else
ERRSTATUS MUSE_API pput_pal();
#endif
/*
Description:
Assigns the palette argument to the currently open
phigs workstation.  (A window is a PHIGS workstation.)
*/

/***************************************************************
@    pscale()
****************************************************************
PHIGS scale transformation. 
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pscale(Pvec scale_vector, Pint error_id, Pmatrix m);
#else
ERRSTATUS MUSE_API pscale();
#endif
/*
Description:
Used to zoom the image.
*/

/***************************************************************
@    pset_char_ht()
****************************************************************
Set PHIGS character height.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_char_ht(Pfloat height);
#else
ERRSTATUS MUSE_API pset_char_ht();
#endif
/*
Description:
Sets character height scale factor.
*/

/***************************************************************
@    pset_edge_colr_ind()
****************************************************************
Set PHIGS edge line colour index.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_edge_colr_ind (Pint index);
#else
ERRSTATUS MUSE_API pset_edge_colr_ind();
#endif
/*
Description:
Sets the edge line colour to one of the defined
line types.
*/


/***************************************************************
@    pset_edge_flag()
****************************************************************
Set to display PHIGS edges on or off.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_edge_flag(Pedge_flag edge_flag);
#else
ERRSTATUS MUSE_API pset_edge_flag();
#endif
/*
Description:
When diplaying fill areas this function determines
whether the edges are visible or not.
*/

/***************************************************************
@    pset_edgetype()
****************************************************************
Set PHIGS edge line type.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_edgetype(Pint edgetype);
#else
ERRSTATUS MUSE_API pset_edgetype();
#endif
/*
Description:
Sets the edge line type to one of the defined
line types.
*/

/***************************************************************
@    pset_edgewidth()
****************************************************************
Set PHIGS edge width.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_edgewidth(Pfloat width);
#else
ERRSTATUS MUSE_API pset_edgewidth();
#endif
/*
Description:
Sets edge line width.
*/

/***************************************************************
@    pset_int_style()
****************************************************************
Set PHIGS fill area interior style.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_int_style (Pint_style style);
#else
ERRSTATUS MUSE_API pset_int_style();
#endif
/*
Description:
Sets the fill area interior style to line one of the
defined types.
*/

/***************************************************************
@    pset_int_colr_ind()
****************************************************************
Set PHIGS fill area interior colour index.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_int_colr_ind(Pint index);
#else
ERRSTATUS MUSE_API pset_int_colr_ind();
#endif
/*
Description:
Sets the fill area interior color to one of the
defined types.
*/



/***************************************************************
@    pset_line_colr_ind()
****************************************************************
Set PHIGS line colour index.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_line_colr_ind (Pint index);
#else
ERRSTATUS MUSE_API pset_line_colr_ind();
#endif
/*
Description:
Sets the line colour to one of the defined types.
*/

/***************************************************************
@    pset_linetype()
****************************************************************
Set PHIGS line type.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_linetype(Pint linetype);
#else
ERRSTATUS MUSE_API pset_linetype();
#endif
/*
Description:
Sets the line type to one of the defined types.
*/

/***************************************************************
@    pset_linewidth()
****************************************************************
Set PHIGS line width.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_linewidth(Pfloat width);
#else
ERRSTATUS MUSE_API pset_linewidth();
#endif
/*
Description:
Sets polyline line width.
*/

/***************************************************************
@    pset_marker_colr_ind()
****************************************************************
Set PHIGS marker colour index.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_marker_colr_ind (Pint index);
#else
ERRSTATUS MUSE_API pset_marker_colr_ind();
#endif
/*
Description:
Sets the marker colour to one of the defined types.
*/

/***************************************************************
@    pset_marker_size()
****************************************************************
Set PHIGS marker size.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_marker_size(Pfloat size);
#else
ERRSTATUS MUSE_API pset_maker_size();
#endif
/*
Description:
Sets marker scale factor.
*/

/***************************************************************
@    pset_marker_type()
****************************************************************
Set PHIGS Set Marker Type.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_marker_type(Pint markertype);
#else
ERRSTATUS MUSE_API pset_marker_type();
#endif
/*
Description:
Sets the marker type to one of the defined types.
*/

/***************************************************************
@    pset_text_colr_ind()
****************************************************************
Set PHIGS Text Colour Index.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_text_colr_ind (Pint index);
#else
ERRSTATUS MUSE_API pset_text_colr_ind();
#endif
/*
Description:
Sets the text colour to one of the defined types.
*/

/***************************************************************
@    pset_ws_vp()
****************************************************************
Set PHIGS workstation viewport.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pset_ws_vp (Pint ws_id, Plimit *viewport);
#else
ERRSTATUS MUSE_API pset_ws_vp();
#endif
/*
Description:
Changes the area of the drawing surface.
*/

/***************************************************************
@    ptext()
****************************************************************
Draw PHIGS text.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API ptext(Ppoint *text_pt, char *text);
#else
ERRSTATUS MUSE_API ptext();
#endif
/*
Description:
Plots text at the text_pt position using the current
color and style.
*/

/***************************************************************
@    ptranslate()
****************************************************************
PHIGS translation transformation.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API ptranslate(Pvec trans_vector, Pint error_id, Pmatrix m);
#else
ERRSTATUS MUSE_API ptranslate();
#endif
/*
Description:
Scrolls the display.
*/

/***************************************************************
@    pupd_ws()
****************************************************************
Update the PHIGS workstation
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API pupd_ws(Pint ws, Pregen_flag regen_flag);
#else
ERRSTATUS MUSE_API pupd_ws();
#endif
/*
Description:
Updates lastest additions to graphics structures
to the screen.
*/

/***************************************************************
@    ws_is_open()
****************************************************************
Is PHIGS workstation is open.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API ws_is_open(Pint ws);
#else
ERRSTATUS MUSE_API ws_is_open();
#endif
/*
Description:
Commonly used non-phigs function to test to see if
workstation is open.
*/


#endif /* #ifndef H_PHIGS_F */
