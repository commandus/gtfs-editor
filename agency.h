/*
 * agency.h
 *
 *  Created on: Jun 8, 2017
 *      Author: andrei
 */

#ifndef AGENCY_H_
#define AGENCY_H_

#include "gtfs-helper.h"

namespace gtfs
{

class Agency
{
public:
	Agency();
	Agency(
		ID agency_id,
		std::string agency_name,
		std::string agency_url,
		std::string agency_timezone,
		std::string agency_phone, 
		std::string agency_lang
	);
	Agency
	(
		const std::vector<std::string> &value
	);

	virtual ~Agency();
	void read(std::istream &strm);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);
	int getTimeZoneOffset();
	
	ID agency_id;					///< 1
	std::string agency_name;		///< АО "Автовокзал"
	std::string agency_url;			///< https://avtovokzal-yakutsk.ru/
	std::string agency_timezone;	///< Asia/Yakutsk
	std::string agency_phone;		///< +7(411)236-4766 
	std::string agency_lang;		///< ru
};

class Agencies
{
public:
	Agencies();
	Agencies
	(
		std::istream &in
	);
	virtual ~Agencies();
	void read(std::istream &strm);
	void write(std::ostream &strm);
	void writeJSON(std::ostream &strm);

	std::vector<Agency> vals;
	bool exists_id(ID id);
	int getDefaultTimeZoneOffset();
};

} /* namespace gtfs */

#endif /* AGENCY_H_ */
