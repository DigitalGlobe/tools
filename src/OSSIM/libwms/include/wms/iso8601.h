/* -*-c++-*- libwms - Copyright (C) since 2004 Garrett Potts 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the libwms Public License (WMSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * libwms Public License for more details.
*/


#ifndef WMS_ISO8601_H
#define WMS_ISO8601_H

#include <string>

typedef struct wmsMyTime_t {
	int year;
	int month;
	int day;
	int hour;
	int min;
	float sec;
} wmsMyTime;

// Returns true if the first date is less than the second date.
bool operator<(const wmsMyTime & a, const wmsMyTime & b);

// Are two times equal?
bool operator==(const wmsMyTime & a, const wmsMyTime & b);
bool operator!=(const wmsMyTime & a, const wmsMyTime & b);

// month is in range [1-12].  Returns 1 if there is a leap day this month/year.
int wms_leap_days(int year, int month);

// Adds two times together.  This is more useful if one of the times
// is actually an interval.
wmsMyTime operator+(const wmsMyTime & a, const wmsMyTime & b);

// Find the absolute difference between two times.  Returns the interval
// between the two times.
wmsMyTime operator-(const wmsMyTime & a, const wmsMyTime & b);


// Convert a time in string format to a wmsMyTime struct.  It's pretty 
// forgiving to what it calls a time.
void wms_parse_time(wmsMyTime & t, const std::string & str);

// Convert a period or interval into a wmsMyTime struct.
void wms_parse_period(wmsMyTime & t, const std::string & str);


// Produce a date/time in standard ISO8601 form from a wmsMyTime struct.
std::string wms_iso8601_time(const wmsMyTime & t);

// Produce a date/time in standard ISO8601 form from a string.
std::string wms_iso8601_time(const std::string & str);

// Produce a time period from a wmsMyTime struct.
std::string wms_iso8601_period(const wmsMyTime & t);

#endif
