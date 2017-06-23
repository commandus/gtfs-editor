/*
 * stops.h
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#ifndef STOPS_H_
#define STOPS_H_

#include "gtfs-helper.h"

namespace gtfs
{

class Stop
{
public:
	Stop();
	Stop
	(
		const std::vector<std::string> &value
	);
	virtual ~Stop();
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);	
	
	ID stop_id;						///< S1
	std::string stop_name;			///< Mission St. & Silver Ave.
	double stop_lat;				///< 37.728631
	double stop_lon;				///< -122.431282
};

class Stops
{
public:
	Stops();
	Stops
	(
		std::istream &in
	);
	virtual ~Stops();
	void read(std::istream &strm);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	bool exists_id(ID id);
	
	std::vector<Stop> vals;
};

} /* namespace gtfs */

#endif /* STOPS_H_ */
