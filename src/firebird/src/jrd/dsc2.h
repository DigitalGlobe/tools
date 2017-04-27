/*
 *	PROGRAM:	JRD access method
 *	MODULE:		dsc2.h
 *	DESCRIPTION:	Definitions associated with descriptors.
 *
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
 * 2002.04.16  Paul Beach - HP10 Define changed from -4 to (-4) to make it
 *             compatible with the HP Compiler
 */

#ifndef JRD_DSC_H
#define JRD_DSC_H

#include "../jrd/gdsassert.h"

// Descriptor format

// WARNING !
//   This run-time structure is stored in RDB$FORMATS.RDB$DESCRIPTORS.
//   Any modification to this structure is tantamount to an ODS change.
//   See MET_format() and make_format() in MET.EPP for enlightenment.


struct dsc
{
	typedef UCHAR dtype_t;
	typedef SCHAR scale_t;
	typedef USHORT length_t;
	typedef SSHORT sub_type_t;
	typedef USHORT flags_t;
	typedef UCHAR* address_t;

	dsc()
		: dsc_dtype(0),
		  dsc_scale(0),
		  dsc_length(0),
		  dsc_sub_type(0),
		  dsc_flags(0),
		  dsc_address(0)
	{}

	explicit dsc(const dtype_t dtype) :
		dsc_dtype(dtype), dsc_scale(0),  dsc_length(0), dsc_sub_type(0),
		dsc_flags(0), dsc_address(0)
	{}

	dsc(const dtype_t dtype, const scale_t scale, const length_t length = 0,
	    const address_t address = 0) :
		dsc_dtype(dtype), dsc_scale(scale), dsc_length(length), dsc_sub_type(0),
		dsc_flags(0), dsc_address(address)
	{}

	dtype_t		dsc_dtype;
	scale_t		dsc_scale;
	length_t	dsc_length;
	sub_type_t	dsc_sub_type;
	flags_t		dsc_flags;
	address_t	dsc_address; // Used either as offset in a message or as a pointer

	// CVC: Some proposed functionality. Unused for now. To be enhanced.
	// Ugly macros should be the last resort in a C++ project.
	dtype_t		getDataType() const {return dsc_dtype;}
	scale_t		getRawScale() const {return dsc_scale;}
	length_t	getRawLength() const {return dsc_length;} // bytes
	sub_type_t	getRawSubType() const {return dsc_sub_type;}
	flags_t		getRawFlags() const {return dsc_flags;}
	address_t	getRawAddress() {return dsc_address;} // this is not const for now!
	const char*	getCharAddress() const {return reinterpret_cast<const char*>(dsc_address);}

	// Is there any data?
	bool		isVarNull() const;
	bool		isUnknown() const;
	bool		isEmpty() const;

	// These depend on the dtype
	sub_type_t	getTextType() const;
	sub_type_t	getCharset() const;
	sub_type_t	getCollate() const;
	length_t	getTextLen(bool validate) const; // bytes, not characters

	bool		isDscEquiv(const dsc* d2) const;
	bool		isText() const;
	bool		isDate() const; //date, time, timestamp
	bool		isBlob() const;
	bool		isBinaryBlob() const;
	bool		isTextBlob() const;
	bool		isMetadataBlob() const;
	bool		isReservedBlob() const;
	bool		isUserDefinedBlob() const;
	bool		isExact() const;
	bool		isSqlNumeric() const;
	bool		isSqlDecimal() const;
	bool		isApprox() const;
	bool		isANumber() const;

	// These are mostly for dsql/make.cpp & jrd/cmp.cpp.
	// Not sure if GPRE can be integrated.
	bool		canNegate() const;
	bool		canAverage() const;
	bool		canDivide() const;
	bool		canMultiply() const;

	//bool		mayExactMulDivFit(const dsc& d2) const;

	// Some conversion routines that assume NULL is tested separately.
	address_t	asUText();
	const address_t	asUText() const;
	char*		asText();
	const char*	asText() const;
	BYTE		asByte() const;
	SSHORT		asSShort() const;
	USHORT		asUShort() const;
	SLONG		asSLong() const;
	ULONG		asULong() const;
	GDS_QUAD	asQuad() const;
	float		asReal() const;
	double		asDouble() const;
	GDS_DATE	asSqlDate() const;
	GDS_TIME	asSqlTime() const;
	GDS_TIMESTAMP asSqlTimestamp() const;
	// The Blob id is basically two unsigned long's.
	//bid		asBlobId() const;
	//bid		asArrayId() const;
	SINT64		asSBigInt() const;
	FB_UINT64	asUBigInt() const;
};

typedef dsc DSC;

// Data types

// WARNING: if you add another manifest constant to this group, then you
// must add another entry to the array compare_priority in jrd/cvt2.cpp.


// Note that dtype_null actually means that we do not yet know the
// dtype for this descriptor.  A nice cleanup item would be to globally
// change it to dtype_unknown.  --chrisj 1999-02-17
// Name changed on 2003.12.17 by CVC.

const dsc::dtype_t	dtype_unknown	= 0;
const dsc::dtype_t	dtype_text		= 1;
const dsc::dtype_t	dtype_cstring	= 2;
const dsc::dtype_t	dtype_varying	= 3;

const dsc::dtype_t	dtype_packed	= 6;
const dsc::dtype_t	dtype_byte		= 7;
const dsc::dtype_t	dtype_short		= 8;
const dsc::dtype_t	dtype_long		= 9;
const dsc::dtype_t	dtype_quad		= 10;
const dsc::dtype_t	dtype_real		= 11;
const dsc::dtype_t	dtype_double	= 12;
const dsc::dtype_t	dtype_d_float	= 13;
const dsc::dtype_t	dtype_sql_date	= 14;
const dsc::dtype_t	dtype_sql_time	= 15;
const dsc::dtype_t	dtype_timestamp	= 16;
const dsc::dtype_t	dtype_blob		= 17;
const dsc::dtype_t	dtype_array		= 18;
const dsc::dtype_t	dtype_int64		= 19;
const dsc::dtype_t  dtype_dbkey		= 20;

const dsc::dtype_t	DTYPE_TYPE_MAX	= 21;

// In DSC_*_result tables, DTYPE_CANNOT means that the two operands
//   cannot participate together in the requested operation.

const UCHAR DTYPE_CANNOT	= 127;

// Historical alias definition
const UCHAR dtype_date		= dtype_timestamp;

const UCHAR dtype_aligned	= dtype_varying;
const UCHAR dtype_any_text	= dtype_varying;
const UCHAR dtype_min_comp	= dtype_packed;
const UCHAR dtype_max_comp	= dtype_d_float;


// values for dsc_flags

// Note: DSC_null is only reliably set for local variables  (blr_variable)
const dsc::flags_t	DSC_null		= 1;
// This seems a legacy flag for the ASCII-only engine prior to v3.2J.
// dsc has no sub type specified
const dsc::flags_t	DSC_no_subtype	= 2;
// not stored. instead, is derived from metadata primarily to flag
//   SQLDA (in DSQL)
const dsc::flags_t	DSC_nullable	= 4;


inline bool dsc::isVarNull() const
{
	return dsc_flags & DSC_null;
}

inline bool dsc::isUnknown() const
{
	return !dsc_dtype || dsc_dtype >= DTYPE_TYPE_MAX;
}

inline bool dsc::isEmpty() const
{
	return !dsc_address;
}


// Overload text typing information into the dsc_sub_type field.
//   See intl.h for definitions of text types

inline dsc::sub_type_t dsc::getTextType() const
{
	if (isText())
		return dsc_sub_type;
	if (isBlob())
		return dsc_scale | (dsc_flags & 0xFF00);
	return 0; // throw an exception?
}


//#define DSC_GET_CHARSET(dsc)	(((dsc)->dsc_sub_type) & 0x00FF)
//#define DSC_GET_COLLATE(dsc)	(((dsc)->dsc_sub_type) >> 8)

inline dsc::sub_type_t dsc::getCharset() const
{
	if (isText())
		return dsc_sub_type & 0x00FF;
	if (isBlob())
		return dsc_scale;
	return 0; // throw an exception?
}

inline dsc::sub_type_t dsc::getCollate() const
{
	if (isText())
		return dsc_sub_type >> 8;
	if (isBlob())
		return dsc_flags >> 8;
	return 0; // throw an exception?
}


// Notice this struct assumes that UCHAR + SCHAR + USHORT == SLONG
// Hence, beware of the alignment used by the HW.
struct alt_dsc
{
	SLONG dsc_combined_type;	// dtype + scale + length
	SSHORT dsc_sub_type;
	USHORT dsc_flags;			// Not currently used
	dsc::address_t* addr;			// Not currently used
};


//#define DSC_EQUIV(d1,d2) ((((alt_dsc*) d1)->dsc_combined_type == ((alt_dsc*) d2)->dsc_combined_type) &&
//			  ((DSC_GET_CHARSET (d1) == DSC_GET_CHARSET (d2)) || d1->dsc_dtype > dtype_any_text))

inline bool dsc::isDscEquiv(const dsc* d2) const
{
	fb_assert(sizeof(alt_dsc) == sizeof(dsc)); // may fail with different alignment!
	const alt_dsc* v1 = reinterpret_cast<const alt_dsc*>(this);
	const alt_dsc* v2 = reinterpret_cast<const alt_dsc*>(d2);
	return v1->dsc_combined_type == v2->dsc_combined_type &&
		(this->getDataType() > dtype_any_text || getCharset() == d2->getCharset());
	// Reversed the OR condition in case getCharset() throws an exception for > dtype_any_text
}


// NOTE: For types <= dtype_any_text the dsc_sub_type field defines
//   the text type

//#define TEXT_LEN(d)   ((d->dsc_dtype == dtype_text) ? d->dsc_length : (d->dsc_dtype == dtype_cstring) ? d->dsc_length - 1 : d->dsc_length - sizeof(USHORT))

// Don't use validate=true if you have a cstring with MBCS because it
// may detect prematurely a null terminator in a multi-byte character.
dsc::length_t dsc::getTextLen(bool validate) const
{
	fb_assert(dsc_dtype <= dtype_any_text);
	if (validate)
	{
		length_t len, len2;
		switch (dsc_dtype)
		{
		case dtype_text:
			len = len2 = dsc_length;
			break;
		case dtype_cstring:
			len = dsc_length - 1;
			//len2 = strlen(getCharAddress());
			// strlen might crash on bad data; we only test up to len
			for (len2 = 0; len2 < len && dsc_address[len2]; ++len2); // no body
			break;
		case dtype_varying:
			len = dsc_length - sizeof(USHORT);
			len2 = asUShort(); // equivalent to vary->vary_length
			break;
		default:
			len = len2 = 0; // throw an exception?
			break;
		}
		if (len2 < len)
			return len2;
		return len;
	}

	switch (dsc_dtype)
	{
	case dtype_text: return dsc_length;
	case dtype_cstring: return dsc_length - 1;
	case dtype_varying: return dsc_length - sizeof(USHORT);
	default: return 0; // ????? throw an exception?
	}
}


// Text subtypes, distinct from character sets & collations
// They are used exclusively by ini.epp and fields.h.

const dsc::sub_type_t	dsc_text_type_none		= 0;	// Normal text
const dsc::sub_type_t	dsc_text_type_fixed		= 1;	// strings can contain null bytes
const dsc::sub_type_t   dsc_text_type_binary	= 1;    // synonym for the prior
const dsc::sub_type_t	dsc_text_type_metadata	= 3;	// string represents system metadata
const dsc::sub_type_t   dsc_text_type_unicode	= 3;    // synonym for the prior


// Exact numeric subtypes: with ODS >= 10, these apply when dtype
//   is short, long, or int64/longlong. Borland didn't reuse the quad type.

const dsc::sub_type_t	dsc_num_type_none		= 0;	// defined as SMALLINT or INTEGER or BIGINT
const dsc::sub_type_t	dsc_num_type_numeric	= 1;	// defined as NUMERIC(n,m)
const dsc::sub_type_t	dsc_num_type_decimal	= 2;	// defined as DECIMAL(n,m)



// Date type information

//#define DTYPE_IS_TEXT(d)	(((d) >= dtype_text) && ((d) <= dtype_varying))
//#define DTYPE_IS_DATE(t)	(((t) >= dtype_sql_date) && ((t) <= dtype_timestamp))

inline bool dsc::isText() const
{
	return dsc_dtype >= dtype_text && dsc_dtype <= dtype_varying;
}

inline bool dsc::isDate() const
{
	return dsc_dtype >= dtype_sql_date && dsc_dtype <= dtype_timestamp;
}


// DTYPE_IS_BLOB includes both BLOB and ARRAY since array's are implemented over blobs.
//#define DTYPE_IS_BLOB(d)        (((d) == dtype_blob) || ((d) == dtype_array))

inline bool dsc::isBlob() const
{
	return dsc_dtype == dtype_blob || dsc_dtype == dtype_array;
}

inline bool dsc::isBinaryBlob() const
{
	return isBlob() && dsc_sub_type == isc_blob_untyped;
}

inline bool dsc::isTextBlob() const
{
	return isBlob() && dsc_sub_type == isc_blob_text;
}

inline bool dsc::isMetadataBlob() const
{
	return isBlob() && dsc_sub_type >= isc_blob_blr &&
		dsc_sub_type < isc_blob_max_predefined_subtype;
}

inline bool dsc::isReservedBlob() const
{
	return isBlob() && dsc_sub_type >= isc_blob_max_predefined_subtype;
}

inline bool dsc::isUserDefinedBlob() const
{
	return isBlob() && dsc_sub_type < isc_blob_untyped;
}


// Exact numeric?

//#define DTYPE_IS_EXACT(d)       (((d) == dtype_int64) ||
//				 ((d) == dtype_long)  ||
//				 ((d) == dtype_short))

inline bool dsc::isExact() const
{
	switch (dsc_dtype)
	{
	case dtype_int64:
	case dtype_long:
	case dtype_short:
		return true;
	default:
		return false;
	}
}

inline bool dsc::isSqlNumeric() const
{
	return isExact() && dsc_sub_type == dsc_num_type_numeric;
}

inline bool dsc::isSqlDecimal() const
{
	return isExact() && dsc_sub_type == dsc_num_type_decimal;
}


// Floating point types?

//#define DTYPE_IS_APPROX(d)       (((d) == dtype_double) ||
//				  ((d) == dtype_real))

inline bool dsc::isApprox() const
{
	switch (dsc_dtype)
	{
	case dtype_real:
	case dtype_double:
		return true;
	default:
		return false;
	}
}


// Beware, this is not SQL numeric(p, s), but a test for number data types
// Strangely, the original macro doesn't test for VMS even though
// the dtype_d_float follows dtype_double.
//#define DTYPE_IS_NUMERIC(d)	((((d) >= dtype_byte) &&
//				  ((d) <= dtype_d_float)) ||
//				 ((d)  == dtype_int64))

// To avoid confusion, the new function was renamed.
inline bool dsc::isANumber() const
{
	return dsc_dtype >= dtype_byte && dsc_dtype <= dtype_double || dsc_dtype == dtype_int64;
}


// Dubious function's check.
//#define NUMERIC_SCALE(desc)	((DTYPE_IS_TEXT((desc).dsc_dtype)) ? 0 : (desc).dsc_scale)

inline dsc::scale_t numeric_scale(const dsc& desc)
{
	return desc.isText() ? 0 : desc.getRawScale();
}


// Macros defining what operations are legal on data types
//#define DTYPE_CAN_NEGATE(d)	DTYPE_IS_NUMERIC(d)
//#define DTYPE_CAN_AVERAGE(d)	DTYPE_IS_NUMERIC(d)
//#define DTYPE_CAN_DIVIDE(d)	DTYPE_IS_NUMERIC(d)
//#define DTYPE_CAN_MULTIPLY(d)	DTYPE_IS_NUMERIC(d)

inline bool dsc::canNegate() const {return isANumber();}
inline bool dsc::canAverage() const {return isANumber();}
inline bool dsc::canDivide() const {return isANumber();}
inline bool dsc::canMultiply() const {return isANumber();}


const int ISC_TIME_SECONDS_PRECISION		= 10000;
const int ISC_TIME_SECONDS_PRECISION_SCALE	= -4;


// It's not a guarantee that it fits.
// It's only an exercise, not to be taken as useful or accurate.
// Notice the decimal places are considered only rounding.
/* Should be moved to dsc.cpp since it's not inline.
bool dsc::mayExactMulDivFit(const dsc& d2) const
{
	if (isVarNull() || d2.isVarNull() || !isExact() || !d2.isExact())
		return false;
	const int sc = dsc_scale + d2.dsc_scale;
	if (sc > 19)
		return false;
	ISC_INT64 v1 = asSBigInt(), v2 = d2.asSBigInt();
	for (int s1 = dsc_scale; s1 < 0; ++s1) {
		if (s1 == -1 && v1 % 10 >= 5)
			v1 += 10;
		v1 /= 10;
	}
	for (int s2 = d2.dsc_scale; s2 < 0; ++s2) {
		if (s2 == -1 && v2 % 10 >= 5)
			v2 += 10;
		v2 /= 10;
	}
	// Overflow prone.
	ISC_INT64 res = v1 * v2, sb = v1 ^ v2;
	if ((res ^ sb) < 0)
		return false;
	int c = 0;
	while (res != 0) {
		res /= 10;
		++c;
	}
	return c + sc < 19; // approx
}
*/

// Conversion routines.

// asUText() and asText don't attempt to represent as a string the content
// of non-text dtypes.
// Warning: always call getTextLength() before retrieving CHAR and VARCHAR,
// as the pointer returned doesn't point to a null terminated string!!!

inline dsc::address_t dsc::asUText()
{
	switch (dsc_dtype)
	{
	case dtype_text:
	case dtype_cstring:
		return dsc_address;
	case dtype_varying:
		return dsc_address + sizeof(UCHAR); // equivalent to vary->vary_data
	}
	return 0;
}

inline const dsc::address_t dsc::asUText() const
{
	return asUText();
}

inline char* dsc::asText()
{
	return reinterpret_cast<char*>(asUText());
}

inline const char* dsc::asText() const
{
	return reinterpret_cast<const char*>(asUText());
}

inline BYTE dsc::asByte() const
{
	if (dsc_dtype == dtype_byte)
		return dsc_address[0];
	return 0;
}

inline SSHORT dsc::asSShort() const
{
	switch (dsc_dtype)
	{
	case dtype_byte:
		return asByte();
	case dtype_short:
		return *reinterpret_cast<SSHORT*>(dsc_address);
	default:
		return 0;
	}
}

inline USHORT dsc::asUShort() const
{
	switch (dsc_dtype)
	{
	case dtype_byte:
		return asByte();
	case dtype_short:
		return *reinterpret_cast<USHORT*>(dsc_address);
	default:
		return 0;
	}
}

inline SLONG dsc::asSLong() const
{
	switch (dsc_dtype)
	{
	case dtype_byte:
		return asByte();
	case dtype_short:
		return asSShort();
	case dtype_long:
		return *reinterpret_cast<SLONG*>(dsc_address);
	default:
		return 0;
	}
}

inline ULONG dsc::asULong() const
{
	switch (dsc_dtype)
	{
	case dtype_byte:
		return asByte();
	case dtype_short:
		return asUShort();
	case dtype_long:
		return *reinterpret_cast<ULONG*>(dsc_address);
	default:
		return 0;
	}
}

inline GDS_QUAD dsc::asQuad() const
{
	if (dsc_dtype != dtype_quad)
	{
		GDS_QUAD aux_quad = {0, 0};
		return aux_quad;
	}
	return *reinterpret_cast<GDS_QUAD*>(dsc_address);
}

inline float dsc::asReal() const
{
	if (dsc_dtype == dtype_real)
		return *reinterpret_cast<float*>(dsc_address);
	return 0;
}

inline double dsc::asDouble() const
{
	if (dsc_dtype == dtype_double)
		return *reinterpret_cast<double*>(dsc_address);
	return 0;
}

inline GDS_DATE dsc::asSqlDate() const
{
	if (dsc_dtype == dtype_sql_date)
		return *reinterpret_cast<GDS_DATE*>(dsc_address);
	return 0;
}

inline GDS_TIME dsc::asSqlTime() const
{
	if (dsc_dtype == dtype_sql_time)
		return *reinterpret_cast<GDS_TIME*>(dsc_address);
	return 0;
}

inline GDS_TIMESTAMP dsc::asSqlTimestamp() const
{
	if (dsc_dtype != dtype_timestamp)
	{
		GDS_TIMESTAMP aux_tt = {0, 0};
		return aux_tt;
	}
	return *reinterpret_cast<GDS_TIMESTAMP*>(dsc_address);
}

//inline bid asBlobId() const
//{
//	if (dsc_dtype != dtype_blob)
//	{
//		bid aux_bid = {0, 0}
//		return aux_bid;
//	}
//	return *reinterpret_cast<bid*>(dsc_address);
//}

//inline bid asArrayId() const
//{
//	if (dsc_dtype != dtype_array)
//	{
//		bid aux_bid = {0, 0}
//		return aux_bid;
//	}
//	return *reinterpret_cast<bid*>(dsc_address);
//}

inline SINT64 dsc::asSBigInt() const
{
	switch (dsc_dtype)
	{
	case dtype_byte:
		return asByte();
	case dtype_short:
		return asSShort();
	case dtype_long:
		return asSLong();
	case dtype_int64:
		return *reinterpret_cast<SINT64*>(dsc_address);
	default:
		return 0;
	}
}

inline FB_UINT64 dsc::asUBigInt() const
{
	switch (dsc_dtype)
	{
	case dtype_byte:
		return asByte();
	case dtype_short:
		return asUShort();
	case dtype_long:
		return asULong();
	case dtype_int64:
		return *reinterpret_cast<FB_UINT64*>(dsc_address);
	default:
		return 0;
	}
}

#endif // JRD_DSC_H

