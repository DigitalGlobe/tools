#ifndef H_MUSERAS

#define H_MUSERAS

#if XVT_CC_PROTO
MUSE_API        raster_construct(FILE * file, RASTER ** pointer);
MUSE_API        raster_destruct(FILE * file, BOOLEAN destruct, RASTER ** raster);
MUSE_API        raster_xform(RASTER * in, RASTER * out, RENDER_INFO * render_info, LUT * default_lut, PALETTE * default_palette);
MUSE_API        palette_construct(FILE * file, PALETTE ** pointer);
MUSE_API        palette_destruct(FILE * file, BOOLEAN destruct, PALETTE ** palette);
MUSE_API        lut_construct(FILE * file, PALETTE * pal, LUT ** pointer);
MUSE_API        lut_destruct(FILE * file, BOOLEAN destruct, LUT ** lut);
MUSE_API        raster_draw(RASTER * raster, WINDOW map_window);
MUSE_API        pal_nearest_color(PALETTE * pal, RGB rgb, USHORT * pal_color);
MUSE_API        file_spec_to_string(FILE_SPEC * file_spec, char *string);
MUSE_API        string_to_file_spec(FILE_SPEC * file_spec, char *string);
MUSE_API        add_color_bias(GHANDLE bitmap);
MUSE_API        remove_color_bias(GHANDLE bitmap);
MUSE_API        muse_error(ERRSTATUS status);
MUSE_API        hsv2rgb(GFLOAT h, GFLOAT s, GFLOAT v, GFLOAT * r, GFLOAT * g, GFLOAT * b);
MUSE_API        pal_gen(PALETTE * pal);
#else
MUSE_API        raster_construct();
MUSE_API        raster_destruct();
MUSE_API        raster_xform();
MUSE_API        palette_construct();
MUSE_API        palette_destruct();
MUSE_API        lut_construct();
MUSE_API        lut_destruct();
MUSE_API        raster_draw();
MUSE_API        pal_nearest_color();
MUSE_API        file_spec_to_string();
MUSE_API        string_to_file_spec();
MUSE_API        add_color_bias();
MUSE_API        remove_color_bias();
MUSE_API        muse_error();
MUSE_API        hsv2rgb();
MUSE_API        pal_gen();
#endif

#endif
