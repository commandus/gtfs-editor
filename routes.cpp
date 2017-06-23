/*
 * routes.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#include "routes.h"

namespace gtfs
{

Route::Route()
:
	route_id(NO_ID),
	route_short_name(""),
	route_long_name(""),
	route_type(3)					// 3: Bus. Used for short- and long-distance bus routes.
{
}

Route::Route
(
	ID aroute_id,
	const std::string &aroute_short_name,
	const std::string &aroute_long_name,
	int aroute_type
)
:
	route_id(aroute_id),
	route_short_name(aroute_short_name),
	route_long_name(aroute_long_name),
	route_type(aroute_type)					// 3: Bus. Used for short- and long-distance bus routes.
{

}

Route::Route
(
	const std::vector<std::string> &value
)
{
	if (value.size() < 4)
		return;
	route_id = strtol(value[0].c_str(), NULL, 10);
	route_short_name = value[1];
	route_long_name = value[2];
	if (route_long_name == route_short_name)
	{
		route_long_name = route_long_name + ".";
	}
	route_type = strtol(value[3].c_str(), NULL, 10);
}

void Route::write(std::ostream &strm)
{
	strm << route_id << "," 
		<< gtfs::csv_escape(route_short_name) << ","
		<< gtfs::csv_escape(route_long_name) << ","
		<< route_type
		<< std::endl; 
}

void Route::writeJSON(std::ostream &strm)
{
	strm 
		<< "{"
		<< "\"route_id\":" << route_id << "," 
		<< "\"route_short_name\":" << json_escape(route_short_name) << ","
		<< "\"route_long_name\":" << json_escape(route_long_name) << ","
		<< "\"route_type\":" << route_type
		<< "}";
}

Route::~Route()
{
}

Routes::Routes()
{
}

Routes::Routes
(
	std::istream &in
)
{
	read(in);
}

Routes::~Routes()
{
}

void Routes::read(std::istream &in)
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
		Route e(v[i]);
		vals.push_back(e);
	}
}

void Routes::write(std::ostream &strm)
{
	strm << "route_id,route_short_name,route_long_name,route_type" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		vals[i].write(strm);
	} 
}

void Routes::writeJSON(std::ostream &strm)
{
	strm << "\"routes\":[" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		if (i)
			strm << ",";
		vals[i].writeJSON(strm);
		strm << std::endl;
	}
	strm << "]" << std::endl;
}

bool Routes::exists_id(ID id)
{
	for (int i = 0; i < vals.size(); ++i)
	{
		if (vals[i].route_id == id)
			return true;
	}
	return false;
}


} /* namespace gtfs */
