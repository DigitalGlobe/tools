#include "ossimWorldWindJpegImage.h"
#include <fstream>
#include <cstring>
extern "C" {
#include <jpeglib.h>
#include <setjmp.h>
#define JPEG_IO_BUFFER_SIZE   2048
#define OUTPUT_BUF_SIZE  4096    /* choose an efficiently fwrite'able size */
  
typedef struct {
    struct jpeg_source_mgr pub;   /* public fields */

    JOCTET* buffer;               /* start of buffer */
   std::istream *stream;
} ossimWorldWind_source_mgr;
   
typedef struct {
    struct jpeg_destination_mgr pub;

   std::ostream *stream;
    JOCTET * buffer;
} ossimWorldWind_destination_mgr;

typedef ossimWorldWind_destination_mgr * ossimWorldWind_dest_ptr;


typedef ossimWorldWind_source_mgr * ossimWorldWind_src_ptr;

   
void ossimWorldWind_init_source ( j_decompress_ptr )
{
}
#define OUTPUT_BUF_SIZE  4096    /* choose an efficiently fwrite'able size */

void init_destination (j_compress_ptr cinfo)
{
    ossimWorldWind_dest_ptr dest = (ossimWorldWind_dest_ptr) cinfo->dest;

    /* Allocate the output buffer --- it will be released when done with image */
    dest->buffer = (JOCTET *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
        OUTPUT_BUF_SIZE * sizeof(JOCTET));
    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

boolean empty_output_buffer (j_compress_ptr cinfo)
{
    ossimWorldWind_dest_ptr dest = (ossimWorldWind_dest_ptr) cinfo->dest;

    dest->stream->write((char*)dest->buffer, OUTPUT_BUF_SIZE);
    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
    return TRUE;
}
void term_destination (j_compress_ptr cinfo)
{
    ossimWorldWind_dest_ptr dest = (ossimWorldWind_dest_ptr) cinfo->dest;
    size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;
    /* Write any data remaining in the buffer */
    if (datacount > 0)
       dest->stream->write((char*)dest->buffer, datacount);
}

boolean ossimWorldWind_fill_input_buffer ( j_decompress_ptr cinfo )
{
    ossimWorldWind_src_ptr src = (ossimWorldWind_src_ptr) cinfo->src;

    src->pub.next_input_byte = src->buffer;
    std::streamsize start = src->stream->tellg();
    src->stream->read((char*)(src->buffer),
                      JPEG_IO_BUFFER_SIZE).tellg();
    std::streamsize end = src->stream->tellg();
    
    src->pub.bytes_in_buffer = end - start;

    if (src->pub.bytes_in_buffer == 0) // check for end-of-stream
    {
        // Insert a fake EOI marker
        src->buffer[0] = 0xFF;
        src->buffer[1] = JPEG_EOI;
        src->pub.bytes_in_buffer = 2;
    }
    return TRUE;
}

void ossimWorldWind_skip_input_data ( j_decompress_ptr cinfo, long num_bytes )
{
    if (num_bytes > 0)
    {
        ossimWorldWind_src_ptr src = (ossimWorldWind_src_ptr) cinfo->src;

        while (num_bytes > (long)src->pub.bytes_in_buffer)
        {
            num_bytes -= (long) src->pub.bytes_in_buffer;
            src->pub.fill_input_buffer(cinfo);
        }
        src->pub.next_input_byte += (size_t) num_bytes;
        src->pub.bytes_in_buffer -= (size_t) num_bytes;
    }
}

void ossimWorldWind_term_source ( j_decompress_ptr cinfo )
{
    ossimWorldWind_src_ptr src = (ossimWorldWind_src_ptr) cinfo->src;

    if (src->pub.bytes_in_buffer > 0)
       src->stream->seekg(-(long)src->pub.bytes_in_buffer, std::ios_base::cur);
    delete[] src->buffer;
}


// JPEG error manager:

struct ossimWorldWind_error_mgr {
  struct jpeg_error_mgr pub;    /* "public" fields */

  jmp_buf setjmp_buffer;    /* for return to caller */
};

typedef struct ossimWorldWind_error_mgr * ossimWorldWind_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

void ossimWorldWind_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a ossimWorldWind_error_mgr struct, so coerce pointer */
  ossimWorldWind_error_ptr myerr = (ossimWorldWind_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  if (cinfo->err->output_message) (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

   
void ossimWorldWind_jpeg_io_src( j_decompress_ptr cinfo, std::istream& infile )
{
    ossimWorldWind_src_ptr src;

    if (cinfo->src == NULL) {    /* first time for this JPEG object? */
        cinfo->src = (struct jpeg_source_mgr *)
            (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
            sizeof(ossimWorldWind_source_mgr));
    }
    src = (ossimWorldWind_src_ptr) cinfo->src;
    src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
    src->buffer = new JOCTET[JPEG_IO_BUFFER_SIZE];
    src->pub.next_input_byte = NULL; /* until buffer loaded */
    src->stream = &infile;

    src->pub.init_source = ossimWorldWind_init_source;
    src->pub.fill_input_buffer = ossimWorldWind_fill_input_buffer;
    src->pub.skip_input_data = ossimWorldWind_skip_input_data;
    src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
    src->pub.term_source = ossimWorldWind_term_source;
}

void ossimWorldWind_init_destination (j_compress_ptr cinfo)
{
    ossimWorldWind_dest_ptr dest = (ossimWorldWind_dest_ptr) cinfo->dest;

    /* Allocate the output buffer --- it will be released when done with image */
    dest->buffer = (JOCTET *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
        OUTPUT_BUF_SIZE * sizeof(JOCTET));
    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

boolean ossimWorldWind_empty_output_buffer (j_compress_ptr cinfo)
{
    ossimWorldWind_dest_ptr dest = (ossimWorldWind_dest_ptr) cinfo->dest;

    dest->stream->write((char*)(dest->buffer), OUTPUT_BUF_SIZE);
    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
    return TRUE;
}

void ossimWorldWind_term_destination (j_compress_ptr cinfo)
{
    ossimWorldWind_dest_ptr dest = (ossimWorldWind_dest_ptr) cinfo->dest;
    size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;
    /* Write any data remaining in the buffer */
    if (datacount > 0)
       dest->stream->write((char*)(dest->buffer), datacount);
}

void ossimWorldWind_jpeg_io_dest (j_compress_ptr cinfo, std::ostream& outfile)
{
    ossimWorldWind_dest_ptr dest;

    if (cinfo->dest == NULL) {    /* first time for this JPEG object? */
        cinfo->dest = (struct jpeg_destination_mgr *)
            (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
            sizeof(ossimWorldWind_destination_mgr));
    }

    dest = (ossimWorldWind_dest_ptr) cinfo->dest;
    dest->pub.init_destination = ossimWorldWind_init_destination;
    dest->pub.empty_output_buffer = ossimWorldWind_empty_output_buffer;
    dest->pub.term_destination = ossimWorldWind_term_destination;
    dest->stream = &outfile;
}
} // end extern

int ossimWorldWindJpegImage::decode(std::istream& inputStream,
                                    ossim_uint8* destBuffer)
{
   struct jpeg_decompress_struct cinfo;
   struct ossimWorldWind_error_mgr jerr;
   JSAMPARRAY tempbuf;
   unsigned char *ptr;
   unsigned stride;
   
   cinfo.err = jpeg_std_error( &jerr.pub );
   jerr.pub.error_exit = ossimWorldWind_error_exit;
   
   cinfo.err->output_message=NULL;
   
   /* Establish the setjmp return context for ossimWorldWind_error_exit to use. */
   if (setjmp(jerr.setjmp_buffer))
   {
      /* If we get here, the JPEG code has signaled an error.
       * We need to clean up the JPEG object, close the input file, and return.
       */
      (cinfo.src->term_source)(&cinfo);
      jpeg_destroy_decompress(&cinfo);
      return FALSE;
   }
   
   jpeg_create_decompress( &cinfo );
   ossimWorldWind_jpeg_io_src( &cinfo, inputStream );
   jpeg_read_header( &cinfo, TRUE );
   cinfo.out_color_space = JCS_RGB;
   jpeg_start_decompress( &cinfo );
   
   ptr = destBuffer;

   stride = cinfo.output_width * 3;
   tempbuf = (*cinfo.mem->alloc_sarray)
      ((j_common_ptr) &cinfo, JPOOL_IMAGE, stride, 1 );
   
   while ( cinfo.output_scanline < cinfo.output_height )
   {
      jpeg_read_scanlines( &cinfo, tempbuf, 1 );
      memcpy( ptr, tempbuf[0], stride );
      ptr += stride;
   }
   jpeg_finish_decompress( &cinfo );
   jpeg_destroy_decompress( &cinfo );
   
   return TRUE;
   
}
