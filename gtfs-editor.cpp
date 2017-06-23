/**
 * ./gtfs-editor
 */
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* for tm_gmtoff and tm_zone */
#endif
#include <time.h>

#include <argtable2.h>
#include "utilstring.h"
#include "gtfs-builder.h"
#include "gtfs-editor-config.h"

const char* progname = "gtfs-editor";

#define DEF_GTFS_FOLDER			"gtfs"

/**
 * Parse command line into struct ClientConfig
 * Return 0- success
 *        1- show help and exit, or command syntax error
 *        2- output file does not exists or can not open to write
 **/
int parseCmd
(
	int argc,
	char* argv[],
	struct ClientConfig *value
)
{
	// GTFS https://developers.google.com/transit/gtfs/reference/?csw=1
	struct arg_file *a_gtfs_folder = arg_file0("G", "gtfs-folder", "<GTFS folder>", "Default " DEF_GTFS_FOLDER);
	struct arg_int *a_time_zone_offset = arg_int0(NULL, "tzo", "<seconds>", "Time zone for GTFS. 32400 for Asia/Yakutsk. Default- your time zone");

	struct arg_lit *a_help = arg_lit0("h", "help", "Show this help");
	struct arg_end *a_end = arg_end(20);

	void* argtable[] = { 
		a_gtfs_folder, a_time_zone_offset,
		a_help, a_end };

	int nerrors;

	// verify the argtable[] entries were allocated successfully
	if (arg_nullcheck(argtable) != 0)
	{
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}
	// Parse the command line as defined by argtable[]
	nerrors = arg_parse(argc, argv, argtable);

	// special case: '--help' takes precedence over error reporting
	if ((a_help->count) || nerrors)
	{
		if (nerrors)
			arg_print_errors(stderr, a_end, progname);
		printf("Usage: %s\n",  progname);
		arg_print_syntax(stdout, argtable, "\n");
		printf("GTFS editor\n");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}

	if (a_time_zone_offset->count)
		value->timezone_offset = *a_time_zone_offset->ival;
	else
		value->timezone_offset = getCurrentTimeOffset();
	
	if (a_gtfs_folder->count)
		value->gtfs_path = *a_gtfs_folder->filename;
	else
		value->gtfs_path = DEF_GTFS_FOLDER;

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return 0;
}

/**
 * Open file to read, skip UTF-8 BOM if exists.
 */
std::ifstream *openUtf8BOM(const std::string &fn)
{
	std::ifstream *ret = new std::ifstream(fn, std::ifstream::in);
	// remove byte order mark (BOM) 0xef 0xbb 0xbf
	unsigned char bom[3];
	ret->read((char*) bom, 3);
	if (!((bom[0] == 0xef) && (bom[1] == 0xbb) && (bom[2] == 0xbf)))
		ret->seekg(0);
	return ret;
}

int main(int argc, char** argv)
{
	struct ClientConfig config;
	int r;
	if (r = parseCmd(argc, argv, &config))
		exit(r);
	return 0;
}
