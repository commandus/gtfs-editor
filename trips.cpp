/*
 * trips.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#include "trips.h"

namespace gtfs
{

Trip::Trip()
	:	
	route_id(NO_ID),
	service_id(NO_ID),
	trip_id(NO_ID)
{
}

Trip::Trip
(
	ID aroute_id,
	ID aservice_id,
	ID atrip_id
)
:
	route_id(aroute_id),
	service_id(aservice_id),
	trip_id(atrip_id)
{

}

Trip::Trip
(
	const std::vector<std::string> &value
)
{
	if (value.size() < 3)
		return;
	route_id = strtol(value[0].c_str(), NULL, 10);
	service_id = strtol(value[1].c_str(), NULL, 10);
	trip_id = strtol(value[2].c_str(), NULL, 10);
}

Trip::~Trip()
{
}

void Trip::write(std::ostream &strm)
{
	strm << route_id << ","
		<< service_id << ","
		<< trip_id
		<< std::endl;
}

void Trip::writeJSON(std::ostream &strm)
{
	strm 
		<< "{"
		<< "\"route_id\":" << route_id << ","
		<< "\"service_id\":" << service_id << ","
		<< "\"trip_id\":" << trip_id 
		<< "}";
}

Trips::Trips()
{
}

Trips::~Trips()
{
}

void Trips::read(std::istream &in)
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
		if (v[i].size() < 3)
			continue;
		Trip e(v[i]);
		vals.push_back(e);
	}
}

void Trips::write(std::ostream &strm)
{
	strm << "route_id,service_id,trip_id" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		vals[i].write(strm);
	}
}

void Trips::writeJSON(std::ostream &strm)
{
	strm << "\"trips\":[" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		if (i)
			strm << ",";
		vals[i].writeJSON(strm);
		strm << std::endl;
	}
	strm << "]" << std::endl;
}

} /* namespace gtfs */
