/*

*/

%module pyossim


%constant   PYOSSIM_SCALAR_UNKNOWN    =  0;
%constant   PYOSSIM_UINT8             =  1;/**< 8 bit unsigned integer        */
%constant   PYOSSIM_SINT8             =  2;/**< 8 bit signed integer          */
%constant   PYOSSIM_UINT16            =  3;/**< 16 bit unsigned integer       */
%constant   PYOSSIM_SINT16            =  4;/**< 16 bit signed integer         */
%constant   PYOSSIM_UINT32            =  5;/**< 32 bit unsigned integer       */
%constant   PYOSSIM_SINT32            =  6;/**< 32 bit signed integer         */
%constant   PYOSSIM_FLOAT32           =  7;/**< 32 bit floating point         */
%constant   PYOSSIM_FLOAT64           =  8;/**< 64 bit floating point         */
%constant   PYOSSIM_CINT16            =  9;/**< 16 bit complex integer        */
%constant   PYOSSIM_CINT32            = 10;/**< 32 bit complex integer        */
%constant   PYOSSIM_CFLOAT32          = 11;/**< 32 bit complex floating point */
%constant   PYOSSIM_CFLOAT64          = 12;/**< 64 bit complex floating point */

   //---
   // Below for backward compatibility only.  Please use above enums in
   // conjunction with null;min;max settings to determine bit depth.
   //---
%constant   PYOSSIM_UCHAR             = 1;  /**< 8 bit unsigned iteger */
%constant   PYOSSIM_USHORT16          = 3;  /**< 16 bit unsigned iteger */
%constant   PYOSSIM_SSHORT16          = 4;  /**< 16 bit signed integer */
%constant   PYOSSIM_USHORT11          = 13; /**< 16 bit unsigned integer (11 bits used) */
%constant   PYOSSIM_FLOAT             = 7;  /**< 32 bit floating point */
%constant   PYOSSIM_NORMALIZED_FLOAT  = 18; /**< normalized floating point  32 bit */
%constant   PYOSSIM_DOUBLE            = 8;  /**< double 64 bit    */
%constant   PYOSSIM_NORMALIZED_DOUBLE = 20; /**< Normalized double 64 bit */

