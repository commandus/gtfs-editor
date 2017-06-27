/**
 *GTFS editor options
 *
 **/
#ifndef GTFS_EDITOR_CONFIG_H
#define GTFS_EDITOR_CONFIG_H

#include <stdio.h>
#include <vector>

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <sys/types.h>

/**
 * Command line interface (CLI) tool configuration structure
 */
struct Config
{
	uint64_t route_tag;								///< default 1
	// GTFS
	std::string gtfs_path;							///< GTFS path file(in, out)
	int timezone_offset;							///< 32400 Asia/Yakutsk YAKT UTC+9
	std::string unicode_range_name;					///< unicode range
	std::string font_name;							///< font file path
	int font_size;									///< font size
};

#endif
