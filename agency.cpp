/*
 * agency.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#include "agency.h"
#include "utilstring.h"

namespace gtfs
{

Agency::Agency()
	: 
	agency_id(NO_ID),
	agency_name(""),
	agency_url(""),
	agency_timezone(""),
	agency_phone(""),
	agency_lang("")
{
}

Agency::Agency(
	ID aagency_id,
	std::string aagency_name,
	std::string aagency_url,
	std::string aagency_timezone,
	std::string aagency_phone, 
	std::string aagency_lang
)
: 
	agency_id(aagency_id),
	agency_name(aagency_name),
	agency_url(aagency_url),
	agency_timezone(aagency_timezone),
	agency_phone(aagency_phone),
	agency_lang(aagency_lang)
{
	
}

Agency::Agency
(
	const std::vector<std::string> &value
)
{
	if (value.size() < 6)
	{
		return;
	}

	agency_id = strtol(value[0].c_str(), NULL, 10);
	agency_name = value[1];
	agency_url = value[2];
	agency_timezone = value[3];
	agency_phone = value[4];
	agency_lang = value[5];
}

Agency::~Agency()
{
}

void Agency::write(std::ostream &strm)
{
	strm
		<< agency_id << "," << csv_escape(agency_name) << "," << agency_url << "," << agency_timezone 
		<< "," << csv_escape(agency_phone) << "," << agency_lang << std::endl;
}

void Agency::writeJSON(std::ostream &strm)
{
	strm 
		<< "{"
		<< "\"agency_id\":" << agency_id << "," 
		<< "\"agency_name\":" << json_escape(agency_name) << ","
		<< "\"agency_url\":" << json_escape(agency_url) << ","
		<< "\"agency_timezone\":" << json_escape(agency_timezone) << "," 
		<< "\"agency_phone\":" << json_escape(agency_phone) << ","
		<< "\"agency_lang\":" << json_escape(agency_lang)
		<< "}";
}

int Agency::getTimeZoneOffset()
{
	if (agency_timezone.empty())
		return 0;
}

Agencies::Agencies()
{
}

Agencies::Agencies
(
	std::istream &in
)
{
	read(in);
}

Agencies::~Agencies()
{
}

void Agencies::read(std::istream &in)
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
		if (v[i].size() < 6)
			continue;
		Agency a(v[i]);
		vals.push_back(a);
	}
}

void Agencies::write(std::ostream &strm)
{
	strm << "agency_id,agency_name,agency_url,agency_timezone,agency_phone,agency_lang" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		vals[i].write(strm);
	}
}

void Agencies::writeJSON(std::ostream &strm)
{
	strm << "\"agencies\":[" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		if (i)
			strm << ",";
		vals[i].writeJSON(strm);
		strm << std::endl;
	}
	strm << "]" << std::endl;
}

bool Agencies::exists_id(ID id)
{
	for (int i = 0; i < vals.size(); ++i)
	{
		if (vals[i].agency_id == id)
			return true;
	}
	return false;
}

int Agencies::getDefaultTimeZoneOffset()
{
	int r = 0;
	
	for (int i = 0; i < vals.size(); ++i)
	{
		r = vals[i].getTimeZoneOffset();
		if (r != 0)
			break;
	}
	if (r == 0)
	{
		r = getCurrentTimeOffset();
	}
	return r;
}

} /* namespace gtfs */
