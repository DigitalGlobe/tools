/** \file sg_file.hxx
 * File I/O routines.
 */

// Written by Curtis Olson, started November 1999.
//
// Copyright (C) 1999  Curtis L. Olson - http://www.flightgear.org/~curt
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// $Id: sg_file.h 10524 2007-02-26 18:27:24Z gpotts $


#ifndef _SG_FILE_HXX
#define _SG_FILE_HXX


#ifndef __cplusplus
# error This library requires C++
#endif

#include <ossimPlanet/compiler.h>

#include <string>

#include <sys/types.h>		// for open(), read(), write(), close()
#include <sys/stat.h>		// for open(), read(), write(), close()
#include <fcntl.h>		// for open(), read(), write(), close()
#if !defined( _MSC_VER )
#  include <unistd.h>		// for open(), read(), write(), close()
#endif

#include "iochannel.h"

SG_USING_STD(string);


/**
 * A file I/O class based on SGIOChannel.
 */
class SGFile : public SGIOChannel {

    string file_name;
    int fp;
    bool eof_flag;

public:

    /**
     * Create an instance of SGFile.
     * When calling the constructor you need to provide a file
     * name. This file is not opened immediately, but instead will be
     * opened when the open() method is called.
     * @param file name of file to open
     */
    SGFile( const string& file );

    /** Destructor */
    ~SGFile();

    // open the file based on specified direction
    bool open( const SGProtocolDir dir );

    // read a block of data of specified size
    int read( char *buf, int length );

    // read a line of data, length is max size of input buffer
    int readline( char *buf, int length );

    // write data to a file
    int write( const char *buf, const int length );

    // write null terminated string to a file
    int writestring( const char *str );

    // close file
    bool close();

    /** @return the name of the file being manipulated. */
    inline string get_file_name() const { return file_name; }

    /** @return true of eof conditions exists */
    inline bool eof() const { return eof_flag; };
};


#endif // _SG_FILE_HXX


