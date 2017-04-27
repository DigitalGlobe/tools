#ifndef H_MUSECONV

#define H_MUSECONV

#ifndef H_VECTOR
#include "vector.h"
#endif

#if XVT_CC_PROTO
MUSE_API        encode_raster_hdr(unsigned char *record, RASTER * raster);
MUSE_API        decode_raster_hdr(unsigned char *record, RASTER * raster);
MUSE_API        encode_lut(unsigned char *record, LUT * lut);
MUSE_API        decode_lut(unsigned char *record, LUT * lut);
MUSE_API        encode_palette(unsigned char *record, PALETTE * palette);
MUSE_API        decode_palette(unsigned char *record, PALETTE * palette);
MUSE_API        encode_mapdoc(unsigned char *record, MAP_DOC * mapdoc);
MUSE_API        decode_mapdoc(unsigned char *record, MAP_DOC * mapdoc);
MUSE_API        encode_mgm(unsigned char *record, MGM * mgm);
MUSE_API        decode_mgm(unsigned char *record, MGM * mgm);
MUSE_API        encode_units(unsigned char *record, UNITS * units);
MUSE_API        decode_units(unsigned char *record, UNITS * units);
MUSE_API        encode_products(unsigned char *record, PRODUCTS * products);
MUSE_API        decode_products(unsigned char *record, PRODUCTS * products);
MUSE_API        encode_view(unsigned char *record, VIEW * view);
MUSE_API        decode_view(unsigned char *record, VIEW * view);
MUSE_API        encode_defaults(unsigned char *record, DEFAULTS * defaults);
MUSE_API        decode_defaults(unsigned char *record, DEFAULTS * defaults);
MUSE_API        encode_basemap(unsigned char *record, BASEMAP * basemap);
MUSE_API        decode_basemap(unsigned char *record, BASEMAP * basemap);
MUSE_API        encode_grat(unsigned char *record, GRAT_DATA * grat);
MUSE_API        decode_grat(unsigned char *record, GRAT_DATA * grat, int32 version);
MUSE_API        encode_vec_data(unsigned char *record, VEC_DATA * vec_data);
MUSE_API        decode_vec_data(unsigned char *record, VEC_DATA * vec_data);
MUSE_API        long_to_char(unsigned char *record, int32 *l, short big_endian, int32 *c);
MUSE_API        short_to_char(unsigned char *record, short *s, short big_endian, int32 *c);
MUSE_API        char_to_double(unsigned char *record, double *d, short big_endian, int32 *c);
MUSE_API        char_to_long(unsigned char *record, int32 *l, short big_endian, int32 *c);
MUSE_API        char_to_short(unsigned char *record, short *s, short big_endian, int32 *c);
#else
MUSE_API        encode_raster_hdr();
MUSE_API        decode_raster_hdr();
MUSE_API        encode_lut();
MUSE_API        decode_lut();
MUSE_API        encode_palette();
MUSE_API        decode_palette();
MUSE_API        encode_mapdoc();
MUSE_API        decode_mapdoc();
MUSE_API        encode_mgm();
MUSE_API        decode_mgm();
MUSE_API        encode_units();
MUSE_API        decode_units();
MUSE_API        encode_products();
MUSE_API        decode_products();
MUSE_API        encode_view();
MUSE_API        decode_view();
MUSE_API        encode_defaults();
MUSE_API        decode_defaults();
MUSE_API        encode_basemap();
MUSE_API        decode_basemap();
MUSE_API        encode_grat();
MUSE_API        decode_grat();
MUSE_API        encode_vec_data();
MUSE_API        decode_vec_data();
MUSE_API        long_to_char();
MUSE_API        short_to_char();
MUSE_API        char_to_double();
MUSE_API        char_to_long();
MUSE_API        char_to_short();
#endif

#endif /* H_MUSECONV */
