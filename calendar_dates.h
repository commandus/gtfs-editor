/*
 * calendar_dates.h
 *
 *  Created on: Jun 9, 2017
 *      Author: andrei
 */

#ifndef CALENDAR_DATES_H_
#define CALENDAR_DATES_H_

#include "gtfs-helper.h"
#include "calendar.h"

namespace gtfs
{

class CalendarDate
{
public:
	CalendarDate();
	CalendarDate(ID service_id,	const GTFSDate &date, int exception_type);
	CalendarDate
	(
		const std::vector<std::string> &value
	);

	virtual ~CalendarDate();
	ID service_id;
	GTFSDate date;
	int exception_type;			///< 1- added, 2- removed
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
};

/**
 * exception_type: service on- 1, off- 2
 * service_id,date,exception_type
	1,20170101,2
	1,20170102,2
	1,20170103,2
	1,20170104,2
	1,20170105,2
	1,20170106,2
	1,20170107,2
	1,20170108,2
	1,20170223,2
	1,20170224,2
	1,20170308,2
	1,20170501,2
	1,20170508,2
	1,20170509,2
	1,20170612,2
	1,20171104,2
 *
 */
class CalendarDates
{
public:
	CalendarDates();
	CalendarDates
	(
		std::istream &in
	);

	virtual ~CalendarDates();
	std::vector<CalendarDate> vals;
	void read(std::istream &strm);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	void find
	(
		std::vector<CalendarDate> &retval,
		ID service_id
	) const;

};

} /* namespace gtfs */

#endif /* CALENDAR_DATES_H_ */
