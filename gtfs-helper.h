/*
 * gtfs-helper.h
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#ifndef GTFS_HELPER_H_
#define GTFS_HELPER_H_

#include <string>
#include <vector>
#include <inttypes.h>
#include <ostream>

#define ID 		uint64_t
#define NO_ID	0
 
namespace gtfs
{

class GTFSDate
{
private:
	int year;
	int month;
	int day;
public:
	GTFSDate();
	GTFSDate(const std::string &line);
	GTFSDate(int year, int month, int day);
	virtual ~GTFSDate();
	void set(const std::string &line);
	std::string toString() const;
	/**
	 * @brief return as unix time epoch
	 * @param timezone_offset time zone offset in seconds
	 */
	time_t toTime(int timezone_offset) const;
	static time_t toTime(int gtfsdate, int timezone_offset);
	
	/**
	* @brief return week of day in specified time zone
	* @param t time unix epoch
	* @param  timezone_offset time zone offset, seconds
	* @return day of week 0- Sunday, 6- Saturday
	*/
	static int wdayInTimezone
	(
		time_t tim, 
		int timezone_offset
	);

	/**
	 * @brief return as int e.g. 20170622
	 */
	uint32_t toInt() const;
	/**
	 * @brief check is dat in the period of time
	 * @param timezone_offset time zone offset in seconds
	 * @param start start with
	 * @param finish end with
	 * @return true if date in the period of time
	 */
	bool isInPeriodOfTime(int timezone_offset, uint32_t start, uint32_t finish) const;
	
	/**
	 * @brief retuen week of day
	 * @param t time unix epoch
	 * @param  timezone_offset time zone offset, seconds
	 * @return day of week 0- Sunday, 6- Saturday
	 */
	static int wday
	(
		time_t t, 
		int timezone_offset
	);
};

class GTFSTime
{
public:
	int h;
	int m;
	GTFSTime(int h, int m);
	GTFSTime();
	GTFSTime
	(
		const std::string &line
	);
	virtual ~GTFSTime();
	std::string toString() const;
	time_t toSeconds() const;
};

class GTFSHelper
{
public:
	GTFSHelper();
	virtual ~GTFSHelper();
};

std::string csv_escape(const std::string &val);

/**
 * @brief escape JSON string
 * @param val value
 * @return escaped string
 */
std::string json_escape(const std::string &val);

void readCSV
(
	std::istream &in,
	std::vector<std::vector<std::string> > &retval
);

} /* namespace gtfs */

#endif /* GTFS_HELPER_H_ */
