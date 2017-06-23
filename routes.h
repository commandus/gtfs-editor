/*
 * routes.h
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#ifndef ROUTES_H_
#define ROUTES_H_

#include "gtfs-helper.h"

namespace gtfs
{

class Route
{
public:
	Route();
	Route
	(
		ID route_id,
		const std::string &route_short_name,
		const std::string &route_long_name,
		int route_type
	);

	Route
	(
		const std::vector<std::string> &value
	);
	virtual ~Route();
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);	
	
	ID route_id;
	std::string route_short_name;
	std::string route_long_name;
	int route_type;					///< 3: Bus. Used for short- and long-distance bus routes.
};

class Routes
{
public:
	Routes();
	Routes
	(
		std::istream &in
	);
	virtual ~Routes();
	void read(std::istream &strm);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	bool exists_id(ID id);
	std::vector<Route> vals;
};


} /* namespace gtfs */

#endif /* ROUTES_H_ */
