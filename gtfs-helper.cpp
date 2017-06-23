/*
 * gtfs-helper.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#include "gtfs-helper.h"

#include <sstream>
#include <iomanip>

namespace gtfs
{

GTFSTime::GTFSTime(int ah, int am)
	: h(ah), m(am)
{

}

GTFSTime::GTFSTime()
	: h(0), m(0)
{

}

GTFSTime::GTFSTime
(
	const std::string &line
)
{
	if (line.length() < 5)
	{
		h = 0;
		m = 0;
		return;
	}
	h = strtol(line.substr(0, 2).c_str(), NULL, 10);
	m = strtol(line.substr(3, 2).c_str(), NULL, 10);
}

std::string GTFSTime::toString() const
{
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << h << ":" << std::setw(2) << m << ":00";
	return ss.str();
}

time_t GTFSTime::toSeconds() const
{
	return (h * 3600) + (m * 60);
}

GTFSTime::~GTFSTime()
{

}


GTFSDate::GTFSDate()
	: year(0), month(0), day(0)
{

}

GTFSDate::GTFSDate
(
	const std::string &line
)
{
	set(line);
}

void GTFSDate::set(const std::string &line)
{
	if (line.length() < 8)
	{
		year = 0;
		month = 0;
		day = 0;
		return;
	}
	year = strtol(line.substr(0, 4).c_str(), NULL, 10);
	month = strtol(line.substr(4, 2).c_str(), NULL, 10);
	day = strtol(line.substr(6, 2).c_str(), NULL, 10);
}

GTFSDate::GTFSDate(int y, int m, int d)
	: year(y), month(m), day(d)
{

}

/**
	* @brief return as unix time epoch
	* @param timezone_offset time zone offset in seconds
	*/
time_t GTFSDate::toTime(int timezone_offset) const
{
	struct tm t;
	t.tm_year = year - 1900;
	t.tm_mon = month - 1;			// Month, 0 - jan
	t.tm_mday = day;				// Day of the month
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	t.tm_isdst = -1;				// Is DST on? 1 = yes, 0 = no, -1 = unknown
	t.tm_gmtoff = timezone_offset;
	return mktime(&t);
}

time_t GTFSDate::toTime
(
	int gtfsdate, 
	int timezone_offset
)
{
	int y = gtfsdate / 10000;
	int m = (gtfsdate / 100) % 100;
	int d = gtfsdate % 100;
	struct tm t;
	t.tm_year = y - 1900;
	t.tm_mon = m - 1;				// Month, 0 - jan
	t.tm_mday = d;				// Day of the month
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	t.tm_isdst = -1;				// Is DST on? 1 = yes, 0 = no, -1 = unknown
	t.tm_gmtoff = timezone_offset;
	return mktime(&t);
}

/**
	* @brief return week of day in specified time zone
	* @param t time unix epoch
	* @param  timezone_offset time zone offset, seconds
	* @return day of week 0- Sunday, 6- Saturday
	*/
int GTFSDate::wdayInTimezone
(
	time_t tim, 
	int timezone_offset
)
{
	// get week day in UTC time
	struct tm *t = gmtime(&tim);
	int r = t->tm_wday;
	// get extra seconds (from midnight)
	uint32_t extra_secs = t->tm_hour * 3600 + t->tm_min * 60 + t->tm_sec;
	// correct week day if cross day bounds
	int add_days = (extra_secs + timezone_offset) / 86400;
	r += add_days;
	// normalize to week
	if (r < 0)
		r = 7 + (r % 7);
	else
		r = r % 7;
	return r;
}

uint32_t GTFSDate::toInt() const
{
	return year * 10000 + month * 100 + day;
}

/**
* @brief check is date in the period of time
* @param timezone_offset time zone offset in seconds
* @param start start with
* @param finish end with
* @return true if date in the period of time
*/
bool GTFSDate::isInPeriodOfTime 
(
	int timezone_offset, 
	uint32_t start, 
	uint32_t finish
) const
{
	time_t tt = toTime(timezone_offset);
	return (tt >= start) && (tt <= finish);
}

GTFSDate::~GTFSDate()
{

}

std::string GTFSDate::toString() const
{
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(4) << year << std::setw(2) << month << std::setw(2) << day;
	return ss.str();
}

GTFSHelper::GTFSHelper()
{
}

GTFSHelper::~GTFSHelper()
{
}

/**
 * Sorry ;(
 * @param val value
 * @return escaped string
 */
std::string csv_escape(const std::string &val)
{
	std::string v(val);
	std::string::size_type pos = 0u;
	bool need_quotes = false;
	while((pos = v.find("\"", pos)) != std::string::npos)
	{
		need_quotes = true;
		v.replace(pos, 1, "\"\"");
		pos += 2;
	}

	if (!need_quotes)
	{
		if (v.find(",", pos) != std::string::npos)
		{
			need_quotes = true;
		}
	}

	if (need_quotes)
	{
		v = "\"" + v + "\"";
	}
	return v;
}

/**
 * Sorry ;(
 * @param val value
 * @return escaped string
 */
std::string json_escape(const std::string &val)
{
	std::string v(val);
	std::string::size_type pos = 0u;
	while((pos = v.find("\"", pos)) != std::string::npos)
	{
		v.replace(pos, 1, "\"\"");
		pos += 2;
	}
	return "\"" + v + "\"";
}

enum CSVState
{
	UnquotedField,
	QuotedField,
	QuotedQuote
};

/**
  * @brief parse SCV record
  */
static std::vector<std::string> readCSVRow
(
	const std::string &row
)
{
    CSVState state = UnquotedField;
    std::vector<std::string> fields;
	fields.push_back("");
    size_t i = 0; // index of the current field
    for (int r = 0; r < row.length(); r++)
    {
		char c = row.at(r);
		switch (state) {
		case UnquotedField:
			switch (c) {
			case ',': // end of field
				fields.push_back("");
				i++;
				break;
			case '"':
				state = QuotedField;
				break;
			default:
				fields[i].push_back(c);
				break;
			}
			break;
		case QuotedField:
			switch (c) {
			case '"':
				state = QuotedQuote;
				break;
			default:
				fields[i].push_back(c);
				break;
			}
			break;
		case QuotedQuote:
			switch (c) {
			case ',': // , after closing quote
				fields.push_back("");
				i++;
				state = UnquotedField;
				break;
			case '"': // "" -> "
				fields[i].push_back('"');
				state = QuotedField;
				break;
			default:  // end of quote
				state = UnquotedField;
				break;
			}
			break;
		}
	}
	return fields;
}

/**
  * @brief Read CSV file
  * http://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
  */
void readCSV
(
	std::istream &in,
	std::vector<std::vector<std::string> > &retval
)
{
	std::string row;
	if (!in)
		return;
	while ((!in.bad()) && (!in.eof()))
	{
		std::getline(in, row);
		retval.push_back(readCSVRow(row));
    }
}

} /* namespace gtfs */
