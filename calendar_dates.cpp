/*
 * calendar_dates.cpp
 *
 *  Created on: Jun 9, 2017
 *      Author: andrei
 */

#include "calendar_dates.h"

namespace gtfs
{

CalendarDate::CalendarDate()
	: service_id(NO_ID), exception_type(0)
{
	
}

CalendarDate::CalendarDate
(
	ID aservice_id,
	const GTFSDate &adate,
	int aexception_type
)
	: service_id(aservice_id), date(adate), exception_type(aexception_type)
{
	
}

CalendarDate::CalendarDate
(
	const std::vector<std::string> &value
)
{
	if (value.size() < 3)
		return;
	service_id = strtol(value[0].c_str(), NULL, 10);
	date.set(value[1]);
	exception_type = strtol(value[2].c_str(), NULL, 10);
}

CalendarDate::~CalendarDate()
{
	
}

void CalendarDate::write(std::ostream &strm)
{
	strm << service_id << "," << date.toString() << "," << exception_type << std::endl;
}

void CalendarDate::writeJSON(std::ostream &strm)
{
	strm 
		<< "{"
		<< "\"service_id\":" << service_id << "," 
		<< "\"date\":" << json_escape(date.toString()) << ","
		<< "\"exception_type\":" << exception_type
		<< "}";
}

CalendarDates::CalendarDates()
{
}

CalendarDates::~CalendarDates()
{
}

CalendarDates::CalendarDates
(
	std::istream &in
)
{
	read(in);
}

void CalendarDates::read
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
	
	for (int i = 1; i < v.size(); ++i)
	{
		if (v[i].size() < 3)
			continue;
		CalendarDate d(v[i]);
		vals.push_back(d);
	}
}

void CalendarDates::write(std::ostream &strm)
{
	strm << "service_id,date,exception_type" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		vals[i].write(strm);
	} 
}

void CalendarDates::writeJSON(std::ostream &strm)
{
	strm << "\"calendar_dates\":[" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		if (i)
			strm << ",";
		vals[i].writeJSON(strm);
		strm << std::endl;
	}
	strm << "]" << std::endl;
}

void CalendarDates::find
(
	std::vector<CalendarDate> &retval,
	ID service_id
) const
{
	for (std::vector<CalendarDate>::const_iterator it(vals.begin()); it != vals.end(); ++it)
	{
		if (it->service_id == service_id)
		{
			retval.push_back(*it);
		}
	}
}

} /* namespace gtfs */
