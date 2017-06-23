/*
 * stop_times.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#include "stop_times.h"

namespace gtfs
{

StopTime::StopTime()
:	trip_id(NO_ID),
	stop_id(NO_ID),
	stop_sequence(0)
{
}

StopTime::StopTime
(
	const std::vector<std::string> &value
)
{
	if (value.size() < 3)
		return;
	trip_id = strtol(value[0].c_str(), NULL, 10);
	arrival_time = GTFSTime(value[1]);
	departure_time = GTFSTime(value[2]);
	stop_id = strtol(value[3].c_str(), NULL, 10);
	stop_sequence = strtol(value[4].c_str(), NULL, 10);
}

StopTime::StopTime
(
	ID atrip_id,
	int aarrival_h,
	int aarrival_m,
	int adeparture_h,
	int adeparture_m,
	ID astop_id,
	int astop_sequence
)
:	trip_id(atrip_id),
	stop_id(astop_id),
	stop_sequence(astop_sequence)
{
	arrival_time = GTFSTime(aarrival_h, aarrival_m);
	departure_time = GTFSTime(adeparture_h, adeparture_m);
}


StopTime::StopTime
(
	ID atrip_id,
	int arrival_seconds,
	int departure_seconds,
	ID astop_id,
	int astop_sequence
)
:	trip_id(atrip_id),
	stop_id(astop_id),
	stop_sequence(astop_sequence)
{
	arrival_time = GTFSTime(arrival_seconds / 3600, (arrival_seconds / 60) % 60);
	departure_time = GTFSTime(departure_seconds / 3600, (departure_seconds / 60) % 60);
}

StopTime::~StopTime()
{

}

int StopTime::toInt() const
{
	return departure_time.h * 100 + departure_time.m;
}

void StopTime::write(std::ostream &strm)
{
	strm << trip_id << ","
		<< arrival_time.toString() << ","
		<< departure_time.toString() << ","
		<< stop_id << ","
		<< stop_sequence
		<< std::endl;
}

void StopTime::writeJSON(std::ostream &strm)
{
	strm 
		<< "{"
		<< "\"trip_id\":" << trip_id << "," 
		<< "\"arrival_time\":" << json_escape(arrival_time.toString()) << ","
		<< "\"departure_time\":" << json_escape(departure_time.toString()) << ","
		<< "\"stop_id\":" << stop_id << ","
		<< "\"stop_sequence\":" << stop_sequence
		<< "}";
}

StopTimes::StopTimes()
{
}

StopTimes::~StopTimes()
{
}

StopTimes::StopTimes
(
	std::istream &in
)
{
	read(in);
}

void StopTimes::read(std::istream &in)
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
		StopTime e(v[i]);
		vals.push_back(e);
	}

}

void StopTimes::write(std::ostream &strm)
{
	strm << "trip_id,arrival_time,departure_time,stop_id,stop_sequence" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		vals[i].write(strm);
	}
}

void StopTimes::writeJSON(std::ostream &strm)
{
	strm << "\"stop_times\":[" << std::endl;
	for (int i = 0; i < vals.size(); ++i)
	{
		if (i)
			strm << ",";
		vals[i].writeJSON(strm);
		strm << std::endl;
	}
	strm << "]" << std::endl;
}

void StopTimes::rmByRoute(ID route_id)
{
	for (std::vector<StopTime>::iterator iter = vals.begin(); iter != vals.end(); ++iter )
	{
		if (iter->stop_id / 1000000 == route_id)
		{
			iter = vals.erase(iter);
			--iter;
	    }
	}
}

void StopTimes::rmByRouteExceptStart
(
	ID route_id
)
{
	for (std::vector<StopTime>::iterator iter = vals.begin(); iter != vals.end(); ++iter )
	{
		if (
				(iter->stop_id != route_id * 1000000)
				&&
				(iter->stop_id / 1000000 == route_id)
			)
		{
			iter = vals.erase(iter);
			--iter;
	    }
	}
}

void StopTimes::rmExceptStart()
{
	for (std::vector<StopTime>::iterator iter = vals.begin(); iter != vals.end(); ++iter )
	{
		if (iter->stop_sequence != 0)
		{
			iter = vals.erase(iter);
			--iter;
	    }
	}
}

std::vector<StopTime> StopTimes::lsStartPoint
(
	ID route_id
)
{
	std::vector<StopTime> r;
	for (std::vector<StopTime>::iterator iter = vals.begin(); iter != vals.end(); ++iter )
	{
		if (iter->stop_id == route_id * 1000000)
		{
			r.push_back(*iter);
	    }
	}
	return r;
}


/**
	* @brief find first stop of the trip
	* @param trip_id trip identifier
	* @return true if found
	*/
bool StopTimes::find_trip_id_init_stop
(
	StopTime &retval,
	ID trip_id,
	int stop_sequence
) const
{
	for (std::vector<StopTime>::const_iterator it(vals.begin()); it != vals.end(); ++it)
	{
		if ((it->trip_id == trip_id) && (it->stop_sequence == stop_sequence))
		{
			retval = *it;
			return true;
		}
	}
	return false;
}


} /* namespace gtfs */
