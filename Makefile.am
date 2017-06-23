SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

if DEBUG
commoncppflags = -std=c++11 -g3 -O0 
else
commoncppflags = -std=c++11 -O2 
endif

#
#	Exclude generated sources from distribution
#
nodist_gtfs_editor_SOURCES = 
BUILT_SOURCES =
CLEANFILES = 

#
# Binaries
#
bin_PROGRAMS=gtfs-editor
nobase_dist_include_HEADERS = \
gtfs-editor-config.h \
gtfs-helper.h gtfs-builder.h agency.h stops.h routes.h trips.h stop_times.h calendar.h calendar_dates.h

common_src =

commonlibs = -L/usr/local/lib/ -largtable2

#
# gui
#

gtfs_editor_SOURCES= \
	gtfs-editor.cpp gtfs-builder.cpp gtfs-helper.cpp \
	agency.cpp stops.cpp routes.cpp trips.cpp stop_times.cpp calendar.cpp calendar_dates.cpp \
	utilstring.cpp \
	$(common_src)

gtfs_editor_LDADD = $(commonlibs)
gtfs_editor_CPPFLAGS = $(commoncppflags)

#
# Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
README.md HISTORY