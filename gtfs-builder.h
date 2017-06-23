/*
 * gtfs-builder.h
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#ifndef GTFS_BUILDER_H_
#define GTFS_BUILDER_H_

#include <string>
#include "gtfs-helper.h"
#include "gtfs-realtime.pb.h"
#include "agency.h"
#include "stops.h"
#include "routes.h"
#include "trips.h"
#include "stop_times.h"
#include "calendar.h"
#include "calendar_dates.h"

namespace gtfs
{

class GTFSBuilder
{
private:
	std::string path;
	int error;
public:
	time_t start;
	time_t finish;
	int time_zone_offset;		///< +32400 YAKT Asia/Yakutsk UTC+9
	Agencies agencies;
	Stops stops;
	Routes routes;
	Trips trips;
	StopTimes stop_times;
	Calendar calendar;
	CalendarDates calendar_dates;
	
	GTFSBuilder
	(
	);

	GTFSBuilder
	(
		const std::string &path
	);

	GTFSBuilder
	(
		const std::string &path,
		time_t start,
		time_t finish
	);

	GTFSBuilder
	(
		const std::string &path,
		int timezone_offset,
		time_t start,
		time_t finish
	);

	virtual ~GTFSBuilder();
	int read();
	int write();
	int errorCode();
	std::string errorDescription();
	
	std::string toJSON();
};

} /* namespace gtfs */

#endif /* GTFS_BUILDER_H_ */
