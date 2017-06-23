/*
 * stop_times.h
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#ifndef STOP_TIMES_H_
#define STOP_TIMES_H_

#include "gtfs-helper.h"

namespace gtfs
{

class StopTime
{
public:
	StopTime();
	virtual ~StopTime();
	StopTime
	(
		const std::vector<std::string> &value
	);
	StopTime
	(
		ID trip_id,
		int arrival_h,
		int arrival_m,
		int departure_h,
		int departure_m,
		ID stop_id,
		int stop_sequence
	);
	StopTime
	(
		ID trip_id,
		int arrival_seconds,
		int departure_seconds,
		ID stop_id,
		int stop_sequence
	);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	int toInt() const;

	ID trip_id;
	GTFSTime arrival_time;
	GTFSTime departure_time;
	ID stop_id;
	int stop_sequence;
};

class StopTimes
{
public:
	StopTimes();
	virtual ~StopTimes();
	StopTimes
	(
		std::istream &in
	);
	void read(std::istream &strm);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	
	/**
	 * @brief remove all stops in the route
	 */
	void rmByRoute(ID route_id);

	/**
	 * @brief remove all stops except start
	 */
	void rmExceptStart();
	
	/**
	 * @brief remove all stops in the route except start
	 */
	void rmByRouteExceptStart
	(
		ID route_id
	);

	/**
	 * @brief get list of initial bus stops of specofoed route
	 * @param route_id route identifier
	 * @return stop time list
	 */
	std::vector<StopTime> lsStartPoint
	(
		ID route_id
	);

	/**
	 * @brief find first stop of the trip
	 * @param trip_id trip identifier
	 * @return true if found
	 */
	bool find_trip_id_init_stop
	(
		StopTime &retval,
		ID trip_id,
		int stop_sequence
	) const;
	
	std::vector<StopTime> vals;
	

};

} /* namespace gtfs */

#endif /* STOP_TIMES_H_ */
