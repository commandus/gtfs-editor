/*
 * calendar.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#include "calendar.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace gtfs
{

CalendarEntry::CalendarEntry()
:	service_id(NO_ID),
	monday(0),
	tuesday(0),
	wednesday(0),
	thursday(0),
	friday(0),
	saturday(0),
	sunday(0)
{
}

CalendarEntry::CalendarEntry
(
	ID aservice_id,
	bool amonday,
	bool atuesday,
	bool awednesday,
	bool athursday,
	bool afriday,
	bool asaturday,
	bool asunday,
	const GTFSDate &astart_date,
	const GTFSDate &aend_date
)
:	service_id(aservice_id),
	monday(amonday),
	tuesday(atuesday),
	wednesday(awednesday),
	thursday(athursday),
	friday(afriday),
	saturday(asaturday),
	sunday(asunday),
	start_date(astart_date),
	end_date(aend_date)
{
	
}

CalendarEntry::CalendarEntry
(
	const std::vector<std::string> &value
)
{
	if (value.size() < 10)
		return;
	service_id = strtol(value[0].c_str(), NULL, 10);
	monday = value[1] == "1";
	tuesday = value[2] == "1";
	wednesday = value[3] == "1";
	thursday = value[4] == "1";
	friday = value[5] == "1";
	saturday = value[6] == "1";
	sunday = value[7] == "1";
	start_date.set(value[8]);
	end_date.set(value[9]);
}

CalendarEntry::~CalendarEntry()
{
}

bool CalendarEntry::operator== (const CalendarEntry &ce1)
{
	return ce1.service_id == service_id;
}

bool CalendarEntry::operator== (ID id)
{
	return id == service_id;
}

Calendar::Calendar()
{
}

Calendar::~Calendar()
{
}

Calendar::Calendar
(
	std::istream &in
)
{
	read(in);
}

void Calendar::read
(
	std::istream &in
)
{
	std::vector<std::vector<std::string> > v;
	gtfs::readCSV(in, v);
	if (v.size() < 2)
	{
		return;
	}
	
	vals.clear();
	for (int i = 1; i < v.size(); ++i)
	{
		if (v[i].size() < 10)
			continue;
		CalendarEntry e(v[i]);
		vals.push_back(e);
	}
}


void CalendarEntry::write(std::ostream &strm)
{
	strm << service_id << "," 
		<< (monday?"1":"0") << ","
		<< (tuesday?"1":"0") << ","
		<< (wednesday?"1":"0") << ","
		<< (thursday?"1":"0") << ","
		<< (friday?"1":"0") << ","
		<< (saturday?"1":"0") << ","
		<< (sunday?"1":"0") << ","
		<< start_date.toString() << ","
		<< end_date.toString() << std::endl; 
}

void CalendarEntry::writeJSON(std::ostream &strm)
{
	strm 
		<< "{"
		<< "\"service_id\":" << service_id << "," 
		<< "\"start_date\":" << json_escape(start_date.toString()) << ","
		<< "\"end_date\":" << json_escape(end_date.toString()) << ","
		<< "\"weekdays_flags\":" << weekdays_flags()
		<< "}";
}

uint32_t CalendarEntry::weekdays_flags()
{
	return (monday << 0) | (tuesday << 1) | (wednesday << 2) | (thursday << 3) | (friday << 4) | (saturday << 5) | (sunday << 6);
}

void CalendarEntry::set_weekdays_flags(uint32_t value)
{
	monday = value & (1 << 0);
	tuesday = value & (1 << 1);
	wednesday = value & (1 << 2);
	thursday = value & (1 << 3);
	friday = value & (1 << 4);
	saturday = value & (1 << 5);
	sunday = value & (1 << 6);
}

void Calendar::write(std::ostream &strm)
{
	strm << "service_id,monday,tuesday,wednesday,thursday,friday,saturday,sunday,start_date,end_date" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		vals[i].write(strm);
	} 
}

void Calendar::writeJSON(std::ostream &strm)
{
	strm << "\"calendar\":[" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		if (i)
			strm << ",";
		vals[i].writeJSON(strm);
		strm << std::endl;
	}
	strm << "]" << std::endl;
}

/**
 * @brief find out service by identifier
 * @param service_id service identifier
 * @return NULL if not found
 */
bool Calendar::find1
(
	CalendarEntry &retval,
	ID service_id
) const
{
	for (std::vector<CalendarEntry>::const_iterator it(vals.begin()); it != vals.end(); ++it)
	{
		if (it->service_id == service_id)
		{
			retval = *it;
			return true;
		}
	}
	return false;
}

} /* namespace gtfs */
