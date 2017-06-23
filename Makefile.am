SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

if DEBUG
commoncppflags = -std=c++11 -g3 -O0 
else
commoncppflags = -std=c++11 -O2 
endif
 
commoncppflags = -g3 -std=c++11 -O2

#
#	Generate protobuf first, then grpc
#
proto = proto/onewayticket.proto
gengrpcs = onewayticket.pb.h onewayticket.pb.cc onewayticket.grpc.pb.h onewayticket.grpc.pb.cc
$(gengrpcs): Makefile
	protoc -I ./proto --cpp_out=. $(proto)
	tools/prepare-pb-odb onewayticket.pb.h
	protoc -I ./proto --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $(proto)
	protobuf-jsonschema $(proto) > $(proto).json-schema
	protoc -I ./proto --cpp_out=. proto/gtfs-realtime.proto

#
#	Generate ODB ORM 
#
genobds = onewayticket.pb-odb.cxx  onewayticket.pb-odb.hxx onewayticket.pb-odb.ixx  onewayticket.pb.sql \
	odb-views-odb.cxx odb-views-odb.hxx odb-views-odb.ixx
$(genobds): Makefile
	tools/rebuild-odb .
	tools/mkdb .
#
#	Exclude generated sources from distribution
#
nodist_onewayticketsvc_SOURCES = $(gengrpcs) $(genobds) 
BUILT_SOURCES = $(gengrpcs) $(genobds)
CLEANFILES = $(gengrpcs) $(genobds)

#
# Binaries
#
bin_PROGRAMS=onewayticketsvc onewayticketcli snmp-onewayticket
nobase_dist_include_HEADERS = \
TicketCredentials.h  SSLValidator.h  onewayticketImpl.h sslhelper.h \
cliconfig.h  cpp-syslog.h  daemonize.h  grpcclient.h  ieee754.h  platform.h  svcconfig.h  \
utilpriority.h  utilstring.h utilinet.h NanoMessage.h passphrase.h mqtt_client.h \
json/json.h  json/json-forwards.h odb-views.h booking-expire.h \
pbjson.hpp  bin2ascii.h get_rss.h utilsnmp.h snmpagentonewayticket.h \
rapidjson/allocators.h     rapidjson/encodings.h        rapidjson/fwd.h             rapidjson/memorystream.h    rapidjson/prettywriter.h  rapidjson/schema.h        rapidjson/writer.h \
rapidjson/document.h       rapidjson/filereadstream.h   rapidjson/istreamwrapper.h  rapidjson/ostreamwrapper.h  rapidjson/rapidjson.h     rapidjson/stream.h \
rapidjson/encodedstream.h  rapidjson/filewritestream.h  rapidjson/memorybuffer.h    rapidjson/pointer.h         rapidjson/reader.h        rapidjson/stringbuffer.h \
rapidjson/error/en.h  rapidjson/error/error.h \
rapidjson/internal/biginteger.h  rapidjson/internal/dtoa.h     rapidjson/internal/itoa.h  rapidjson/internal/pow10.h  rapidjson/internal/stack.h    rapidjson/internal/strtod.h \
rapidjson/internal/diyfp.h       rapidjson/internal/ieee754.h  rapidjson/internal/meta.h  rapidjson/internal/regex.h  rapidjson/internal/strfunc.h  rapidjson/internal/swap.h \
rapidjson/msinttypes/inttypes.h  rapidjson/msinttypes/stdint.h \
odb/options.pgsql  odb/pragmas.pgsql.hxx  odb/repeatedptrfield-traits.hxx \
gtfs-helper.h gtfs-builder.h agency.h stops.h routes.h trips.h stop_times.h calendar.h calendar_dates.h gtfs-onewayticket.h

common_src =

commonlibs = -L/usr/local/lib/ -lgrpc++ -lgrpc -lgpr -lprotobuf -lpthread -ldl -largtable2
#
# service
#

# SNMPLIBS = `net-snmp-config --agent-libs`
SNMPLIBS = -lnetsnmpagent -lnetsnmp

onewayticketsvc_SOURCES= \
	TicketCredentials.cpp  SSLValidator.cpp daemonize.cpp  ieee754.cpp  pbjson.cpp booking-expire.cpp \
	sslhelper.cpp  onewayticketImpl.cpp  onewayticketsvc.cpp  svcconfig.cpp passphrase.cpp \
	utilpriority.cpp  utilstring.cpp utilinet.cpp NanoMessage.cpp get_rss.c utilsnmp.cpp snmpagentonewayticket.c \
	mqtt_client.cpp \
	gtfs-helper.cpp \
	$(common_src)

onewayticketsvc_LDADD = $(commonlibs) -lodb-pgsql -lodb -lcrypto -lssl -lglog -lunwind -lnanomsg -lpaho-mqtt3c $(SNMPLIBS) 
onewayticketsvc_CPPFLAGS = -Iodb $(commoncppflags)

#
# Client
#
onewayticketcli_SOURCES= \
	TicketCredentials.cpp  grpcclient.cpp  jsoncpp.cpp  pbjson.cpp  onewayticketcli.cpp \
	sslhelper.cpp utilstring.cpp  onewayticket.grpc.pb.cc  onewayticket.pb.cc get_rss.c \
	gtfs-helper.cpp gtfs-builder.cpp gtfs-realtime.pb.cc agency.cpp stops.cpp routes.cpp trips.cpp \
	stop_times.cpp calendar.cpp calendar_dates.cpp gtfs-onewayticket.cpp \
	$(common_src)

onewayticketcli_LDADD = $(commonlibs) -lcrypto -lglog
onewayticketcli_CPPFLAGS = $(commoncppflags)

#
# SNMP client
#
snmp_onewayticket_SOURCES= \
	snmp-onewayticket.cpp \
	$(common_src)
snmp_onewayticket_LDADD = $(commonlibs) -lglog $(SNMPLIBS)
snmp_onewayticket_CPPFLAGS = $(commoncppflags)

#
# Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
README.md HISTORY \
proto/onewayticket.proto proto/gtfs-realtime.proto \
cert/ca.crt cert/ca.key cert/client.crt cert/client.key \
cert/roots.crt  cert/server.crt  cert/server.key  cert/sn.txt \
CMakeLists.txt cmake/FindArgtable2.cmake cmake/FindGlog.cmake cmake/FindGRPC.cmake cmake/FindNanomsg.cmake cmake/FindProtobuf.cmake \
tools/mkcert  tools/mkdb  tools/mkdependencies  tools/mkdocker tools/mkdocker.bat tools/mkdocker-bin tools/prepare-pb-odb tools/rebuild.bat tools/rebuild-odb \
tools/docker/ubuntu/Dockerfile tools/docker/ubuntu/Dockerfile.bin \
tools/adduser.sh tools/copy-binaries tools/mksnmp tools/mktools
