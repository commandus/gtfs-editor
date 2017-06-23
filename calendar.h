/*
 * calendar.h
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#ifndef CALENDAR_H_
#define CALENDAR_H_

#include "gtfs-helper.h"

namespace gtfs
{

class CalendarEntry
{
public:
	CalendarEntry();
	CalendarEntry
	(
		ID service_id,
		bool monday,
		bool tuesday,
		bool wednesday,
		bool thursday,
		bool friday,
		bool saturday,
		bool sunday,
		const GTFSDate &start_date,
		const GTFSDate &end_date
	);
	
	CalendarEntry
	(
		const std::vector<std::string> &value
	);

	virtual ~CalendarEntry();
	bool operator== (const CalendarEntry &ce1);
	bool operator== (ID id);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);	
	
	ID service_id;
	bool monday;
	bool tuesday;
	bool wednesday;
	bool thursday;
	bool friday;
	bool saturday;
	bool sunday;
	GTFSDate start_date;
	GTFSDate end_date;
	
	uint32_t weekdays_flags();
	void set_weekdays_flags(uint32_t value);
};

class Calendar
{
public:
	Calendar();
	Calendar
	(
		std::istream &in
	);
	virtual ~Calendar();
	void read
	(
		std::istream &in
	);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	
	std::vector<CalendarEntry> vals;
	bool find1
	(
		CalendarEntry &retval,
		ID service_id
	) const;

};


} /* namespace gtfs */

#endif /* CALENDAR_H_ */
