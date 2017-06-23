/*
 * gtfs-builder.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#include "gtfs-builder.h"
#include "platform.h"
#include <fstream>
#include <sstream>

namespace gtfs
{

GTFSBuilder::GTFSBuilder()
	: path("."), error(0), start(0), finish(0), time_zone_offset(0)
{
}

GTFSBuilder::~GTFSBuilder()
{
}

GTFSBuilder::GTFSBuilder
(
	const std::string &apath

)
	: path(apath), error(0), start(0), finish(0),  time_zone_offset(0)
{

}

GTFSBuilder::GTFSBuilder
(
	const std::string &apath,
	time_t astart,
	time_t afinish

)
	: path(apath), error(0), start(astart), finish(afinish), time_zone_offset(0)
{
	
}

GTFSBuilder::GTFSBuilder
(
	const std::string &apath,
	int atimezone_offset,
	time_t astart,
	time_t afinish

)
	: path(apath), error(0), start(astart), finish(afinish), time_zone_offset(atimezone_offset)
{
	
}

int GTFSBuilder::read()
{
	std::ifstream iagency(CONCAT_PATH(path, "agency.txt"));
	agencies.read(iagency);
	time_zone_offset = agencies.getDefaultTimeZoneOffset();
	std::ifstream istops(CONCAT_PATH(path, "stops.txt"));
	stops.read(istops);
	std::ifstream iroutes(CONCAT_PATH(path, "routes.txt"));
	routes.read(iroutes);
	std::ifstream itrips(CONCAT_PATH(path, "trips.txt"));
	trips.read(itrips);
	std::ifstream istop_times(CONCAT_PATH(path, "stop_times.txt"));
	stop_times.read(istop_times);
	std::ifstream icalendar(CONCAT_PATH(path, "calendar.txt"));
	calendar.read(icalendar);
	std::ifstream icalendar_dates(CONCAT_PATH(path, "calendar_dates.txt"));
	calendar_dates.read(icalendar_dates);
	return 0;
}

int GTFSBuilder::write()
{
	//
	std::ofstream iagency(CONCAT_PATH(path, "agency.txt"));
	agencies.write(iagency);
	std::ofstream istops(CONCAT_PATH(path, "stops.txt"));
	stops.write(istops);
	std::ofstream iroutes(CONCAT_PATH(path, "routes.txt"));
	routes.write(iroutes);
	std::ofstream itrips(CONCAT_PATH(path, "trips.txt"));
	//
	trips.write(itrips);
	std::ofstream istop_times(CONCAT_PATH(path, "stop_times.txt"));
	stop_times.write(istop_times);
	std::ofstream icalendar(CONCAT_PATH(path, "calendar.txt"));
	calendar.write(icalendar);
	std::ofstream icalendar_dates(CONCAT_PATH(path, "calendar_dates.txt"));
	calendar_dates.write(icalendar_dates);
	return 0;
}

int GTFSBuilder::errorCode()
{
	return error;	
}

std::string GTFSBuilder::errorDescription()
{
	return "";
}

std::string GTFSBuilder::toJSON()
{
	//
	std::stringstream ss;
	ss << "{\"gtfs\":{" << std::endl
		<< "\"start\":" << start << ","
		<< "\"finish\":" << finish << ","
		<< "\"time_zone_offset\":" << time_zone_offset << ",";
	
	agencies.writeJSON(ss);
	ss << "," << std::endl;
	stops.writeJSON(ss);
	ss << "," << std::endl;
	routes.writeJSON(ss);
	ss << "," << std::endl;
	trips.writeJSON(ss);
	ss << "," << std::endl;
	stop_times.writeJSON(ss);
	ss << "," << std::endl;
	calendar.writeJSON(ss);
	ss << "," << std::endl;
	calendar_dates.writeJSON(ss);
	ss << "}}" << std::endl;
	return ss.str();
}

} /* namespace gtfs */
