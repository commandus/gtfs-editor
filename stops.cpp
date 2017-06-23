/*
 * stops.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#include "stops.h"
#include <iomanip>

namespace gtfs
{

Stop::Stop()
	:
	stop_id(NO_ID),
	stop_name(""),
	stop_lat(0.0),
	stop_lon(0.0)
{
}

Stop::Stop
(
	const std::vector<std::string> &value
)
{
	if (value.size() < 4)
		return;
	stop_id = strtol(value[0].c_str(), NULL, 10);
	stop_name = value[1];
	stop_lat = strtod(value[2].c_str(), NULL);
	stop_lon = strtod(value[3].c_str(), NULL);
}

Stop::~Stop()
{
}

void Stop::write(std::ostream &strm)
{
	strm << stop_id << "," 
		<< gtfs::csv_escape(stop_name) << ","
		<< std::fixed << std::setprecision(10)
		<< stop_lat << ","
		<< stop_lon
		<< std::endl; 
}

void Stop::writeJSON(std::ostream &strm)
{
	strm 
		<< "{"
		<< "\"stop_id\":" << stop_id << "," 
		<< "\"stop_name\":" << json_escape(stop_name) << ","
		<< "\"stop_lat\":" << std::fixed << std::setprecision(10) << stop_lat << ","
		<< "\"stop_lon\":" << stop_lon
		<< "}";
}

Stops::Stops()
{
}

Stops::Stops
(
	std::istream &in
)
{
	read(in);
}

Stops::~Stops()
{
}

void Stops::read(std::istream &in)
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
		if (v[i].size() < 4)
			continue;
		Stop e(v[i]);
		vals.push_back(e);
	}
}

void Stops::write(std::ostream &strm)
{
	strm << "stop_id,stop_name,stop_lat,stop_lon" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		vals[i].write(strm);
	} 
}

void Stops::writeJSON(std::ostream &strm)
{
	strm << "\"stops\":[" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		if (i)
			strm << ",";
		vals[i].writeJSON(strm);
		strm << std::endl;
	}
	strm << "]" << std::endl;
}

bool Stops::exists_id(ID id)
{
	for (int i = 0; i < vals.size(); ++i)
	{
		if (vals[i].stop_id == id)
			return true;
	}
	return false;
}

} /* namespace gtfs */
