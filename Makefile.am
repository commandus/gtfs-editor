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
gtfs-helper.h gtfs-builder.h agency.h stops.h routes.h trips.h stop_times.h calendar.h calendar_dates.h \
ext/imgui/imconfig.h ext/imgui/imgui.h ext/imgui/imgui_internal.h ext/imgui/stb_rect_pack.h \
ext/imgui/stb_textedit.h ext/imgui/stb_truetype.h

common_src =

commonlibs = -L/usr/local/lib/ -largtable2 -lglfw -lGL

#
# gui
#

gtfs_editor_SOURCES= \
	gtfs-editor.cpp gtfs-builder.cpp gtfs-helper.cpp \
	agency.cpp stops.cpp routes.cpp trips.cpp stop_times.cpp calendar.cpp calendar_dates.cpp \
	utilstring.cpp \
	ext/imgui/imgui.cpp ext/imgui/imgui_draw.cpp \
	$(common_src)

gtfs_editor_LDADD = $(commonlibs)
gtfs_editor_CPPFLAGS = $(commoncppflags) -I ext/imgui

#
# Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
README.md HISTORY