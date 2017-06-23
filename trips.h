/*
 * trips.h
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#ifndef TRIPS_H_
#define TRIPS_H_

#include "gtfs-helper.h"

namespace gtfs
{

class Trip
{
public:
	Trip();
	Trip
	(
		ID route_id,
		ID service_id,
		ID trip_id
	);
	Trip
	(
		const std::vector<std::string> &value
	);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	virtual ~Trip();
	ID route_id;
	ID service_id;
	ID trip_id;
};

class Trips
{
public:
	Trips();
	Trips
	(
		std::istream &in
	);
	virtual ~Trips();

	void read(std::istream &strm);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	
	std::vector<Trip> vals;

};

} /* namespace gtfs */

#endif /* TRIPS_H_ */
