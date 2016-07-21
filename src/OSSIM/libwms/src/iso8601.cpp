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
#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <cstdio>
#include "wms/iso8601.h"

bool operator<(const wmsMyTime & a, const wmsMyTime & b)
{
	if (a.year != b.year) return (a.year < b.year);
	if (a.month != b.month) return (a.month < b.month);
	if (a.day != b.day) return (a.day < b.day);
	if (a.hour != b.hour) return (a.hour < b.hour);
	if (a.min != b.min) return (a.min < b.min);
	if (a.sec != b.sec) return (a.sec < b.sec);

	return false;
}

bool operator==(const wmsMyTime & a, const wmsMyTime & b)
{
	if (a.year == b.year &&
		a.month == b.month &&
		a.day == b.day &&
		a.hour == b.hour &&
		a.min == b.min &&
		a.sec == b.sec)
	{
		return true;
	}

	return false;
}

bool operator!=(const wmsMyTime & a, const wmsMyTime & b)
{
    return ! (a == b);
}

// month is in range [1-12].  Returns 1 if there is a leap day this month/year.
int wms_leap_days(int year, int month)
{
	int result = 0;
	if ((month-1) % 12 == 1)
	{
		if (year % 4 == 0)
		{
			// If the year is divisible by 4, this is a leap year.
			result = 1;
			if (year % 100 == 0)
			{
				// If the year is divisible by 100, this is not a leap year.
				result = 0;
				if (year % 400 == 0)
				{
					// If the year is divisible by 400, this is a leap year.
					result = 1;
				}
			}
		}
	}

	return result;
}

static int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

wmsMyTime operator+(const wmsMyTime & a, const wmsMyTime & b)
{
	wmsMyTime result(a);
	result.year += b.year;
	result.month += b.month;
	result.day += b.day;
	result.hour += b.hour;
	result.min += b.min;
	result.sec += b.sec;

	while (result.sec >= 60)
		result.sec -= 60, result.min++;
	while (result.min >= 60)
		result.min -= 60, result.hour++;
	while (result.hour >= 24)
		result.hour -= 24, result.day++;


	while (result.day > (days[(result.month-1)%12] + wms_leap_days(result.year, result.month)))
	{
		result.day -= days[(result.month-1)%12] + wms_leap_days(result.year, result.month);
		result.month++;
	}

	while (result.month > 12)
		result.month -= 12, result.year++;

	return result;
}

/* Returns the absolute difference between the two times */
wmsMyTime operator-(const wmsMyTime & a_src, const wmsMyTime & b_src)
{
	if (a_src < b_src)
		return b_src - a_src;

	wmsMyTime result;
	wmsMyTime a(a_src);
	wmsMyTime b(b_src);

    // Debugging
	//fprintf(stderr, "a-b: (%d %d %d %d %d %.3g)-(%d %d %d %d %d %.3g)=",
	//	a.year, a.month, a.day, a.hour, a.min, a.sec,
	//	b.year, b.month, b.day, b.hour, b.min, b.sec);

	while (a.sec < b.sec)
		a.min--, a.sec += 60;
	result.sec = a.sec - b.sec;

	while (a.min < b.min)
		a.hour--, a.min += 60;
	result.min = a.min - b.min;

	while (a.hour < b.hour)
		a.day--, a.hour += 24;
	result.hour = a.hour - b.hour;

	while (a.day < b.day)
	{
		a.month--;
		a.day += days[(a.month+11)%12] + wms_leap_days(a.year, a.month);
	}
	result.day = a.day - b.day;

	while (a.month < b.month)
		a.year--, a.month += 12;
	result.month = a.month - b.month;

	result.year = a.year - b.year;

	// Debugging
	//fprintf(stderr, "(%d %d %d %d %d %.3g)\n", result.year,
	//	result.month, result.day, result.hour, result.min, result.sec);

	return result;
}

void wms_parse_time(wmsMyTime & t, const std::string & str)
{
	int loc, loc2;

	// year
	loc = str.find_first_of("0123456789");
	loc2 = str.find_first_not_of("0123456789", loc);
	if (loc < 0 || loc == loc2)
		return;
	t.year = atoi(str.substr(loc, loc2-loc).c_str());

	// month
	//loc = strcspn(cp, "- \t");
	loc = str.find_first_of("0123456789", loc2);
	loc2 = str.find_first_not_of("0123456789", loc);
	if (loc < 0 || loc == loc2)
		return;
	t.month = atoi(str.substr(loc, loc2-loc).c_str());

	// day
	loc = str.find_first_of("0123456789", loc2);
	loc2 = str.find_first_not_of("0123456789", loc);
	if (loc < 0 || loc == loc2)
		return;
	t.day = atoi(str.substr(loc, loc2-loc).c_str());

	// hour
	loc = str.find_first_of("0123456789", loc2);
	loc2 = str.find_first_not_of("0123456789", loc);
	if (loc < 0 || loc == loc2)
		return;
	t.hour = atoi(str.substr(loc, loc2-loc).c_str());

	// minute
	loc = str.find_first_of("0123456789", loc2);
	loc2 = str.find_first_not_of("0123456789", loc);
	if (loc < 0 || loc == loc2)
		return;
	t.min = atoi(str.substr(loc, loc2-loc).c_str());

	// second
	loc = str.find_first_of("0123456789.", loc2);
	loc2 = str.find_first_not_of("0123456789.", loc);
	if (loc < 0 || loc == loc2)
		return;
	t.sec = atof(str.substr(loc, loc2-loc).c_str());
}

void wms_parse_period(wmsMyTime & t, const std::string & str)
{
	const char *cp = str.c_str();

	bool t_flag = false; // true after a T is parsed

	while (cp && *cp)
	{
		int loc = strspn(cp, "0123456789.");
		std::string str(cp, loc);
		cp += loc;

		switch(*cp)
		{
		case 'P':
			break;
		case 'Y':
			t.year = atoi(str.c_str());
			break;
		case 'M':
			if (t_flag)
				t.min = atoi(str.c_str());
			else
				t.month = atoi(str.c_str());
			break;
		case 'D':
			t.day = atoi(str.c_str());
			break;
		case 'T':
			t_flag = true;
			break;
		case 'H':
			t.hour = atoi(str.c_str());
			break;
		case 'S':
			t.sec = atof(str.c_str());
			break;
		default:
			// Illegal character; ignore
			break;
		}

		if (*cp) cp++;
	}
}



std::string wms_iso8601_time(const wmsMyTime & t)
{
	// Debugging
	//printf("iso8601_time:  %dy %dm %dd %dh %dm %.6gs\n",
	//	t.year, t.month, t.day, t.hour, t.min, t.sec);

	char buf[400];
	std::string result;

	snprintf(buf, sizeof(buf), "%04d-%02d-%02d", t.year, t.month, t.day);
	result = buf;

	if (t.hour >= 0)
	{
		snprintf(buf, sizeof(buf), "T%02d", t.hour);
		result += buf;

		if (t.min >= 0)
		{
			snprintf(buf, sizeof(buf), ":%02d", t.min);
			result += buf;

			if (t.sec >= 0)
			{
				int int_sec = (int) floor(t.sec);
				snprintf(buf, sizeof(buf), ":%02d", int_sec);
				result += buf;

				int frac = (int) (floor((t.sec - int_sec) * 1000.0));
				if (frac > 0)
				{
					snprintf(buf, sizeof(buf), ".%03d", frac);
					result += frac;
				}
			}
		}
		result += "Z";
	}

	return result;
}

std::string wms_iso8601_time(const std::string & str)
{
	wmsMyTime t = { -1, -1, -1, -1, -1, -1 };
	wms_parse_time(t, str);
	return wms_iso8601_time(t);
}

std::string wms_iso8601_period(const wmsMyTime & t)
{
    std::string first;  // Contains years, months, days
    std::string second;  // Hours, minutes, seconds
    char buf[400];

    if (t.year != 0)
    {
        snprintf(buf, 400, "%d", t.year);
        first += buf;
        first += "Y";
    }

    if (t.month != 0)
    {
        snprintf(buf, 400, "%d", t.month);
        first += buf;
        first += "M";
    }

    if (t.day != 0)
    {
        snprintf(buf, 400, "%d", t.day);
        first += buf;
        first += "D";
    }

    if (t.hour != 0)
    {
        snprintf(buf, 400, "%d", t.hour);
        second += buf;
        second += "H";
    }

    if (t.min != 0)
    {
        snprintf(buf, 400, "%d", t.min);
        second += buf;
        second += "M";
    }

    if (t.sec != 0)
    {
        snprintf(buf, 400, "%.3g", t.sec);
        second += buf;
        second += "S";
    }

    std::string result = first;
    if (second != "")
    {
        result += "T";
        result += second;
    }

    return result;
}

