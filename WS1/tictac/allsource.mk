# ------------------- SOURCES ---------------------
INCLUDES_MY = -I$(BOOST_INCLUDE_PATH) -I. 
 
INCLUDES_MY   += -I./src/utils  
SOURCE_UTILS   = $(wildcard src/utils/*.cpp)
 
INCLUDES_MY += -I./src/netlist/devices -I./src/netlist/devices/linear 
INCLUDES_MY += -I./src/netlist/devices/nonlinear -I./src/netlist/devices/sources
SOURCE_DEVICES  = $(wildcard src/netlist/devices/*.cpp)
SOURCE_DEVICES += $(wildcard src/netlist/devices/linear/*.cpp)
SOURCE_DEVICES += $(wildcard src/netlist/devices/nonlinear/*.cpp)
SOURCE_DEVICES += $(wildcard src/netlist/devices/sources/*.cpp)

INCLUDES_MY += -I./src/netlist  
SOURCE_NETLIST = $(wildcard src/netlist/*.cpp)

INCLUDES_MY += -I./src/analysis -I./src/analysis/controllers  -I./src/analysis/analyfunctions
SOURCE_ANALYSIS  = $(wildcard src/analysis/*.cpp)
SOURCE_ANALYSIS += $(wildcard src/analysis/controllers/*.cpp)
SOURCE_ANALYSIS += $(wildcard src/analysis/analyfunctions/*.cpp)

INCLUDES_MY += -I./src/matrix_la 
SOURCE_LA = $(wildcard src/matrix_la/*.cpp)

INCLUDES_MY += -I./src/linear_solvers 
SOURCE_LS = $(wildcard src/linear_solvers/*.cpp)

INCLUDES_MY += -I./src/nonlinear_solvers 
SOURCE_NS = $(wildcard src/nonlinear_solvers/*.cpp)

INCLUDES_MY += -I./src/options  
SOURCE_OPTIONS = $(wildcard src/options/*.cpp)

INCLUDES_MY += -I./src/output  
SOURCE_OUTPUT = $(wildcard src/output/*.cpp)

# main of the simulator  
INCLUDES_MY += -I./src/computer  
SOURCE_PROC = $(wildcard src/computer/*.cpp) 

#------ for the netlist parser --- 
SOURCE_NETLIST_PARSER = $(wildcard src/parser_netlist/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/veriloga_structs/declarations/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/veriloga_structs/expressions/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/veriloga_structs/statements/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/veriloga_structs/titan_specific/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/veriloga_structs/utils/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/veriloga_structs/visitor/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/parser/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/netlist_struct/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/netlist_control/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/netlist_visitor/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/netlist_expansion/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/netlist_expression/*.cpp)
SOURCE_NETLIST_PARSER += $(wildcard src/parser_netlist/scanner/*.cpp)

INCLUDES_NETLIST_PARSER = -I./src/parser_netlist 
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/veriloga_structs/declarations
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/veriloga_structs/expressions
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/veriloga_structs/statements
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/veriloga_structs/titan_specific
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/veriloga_structs/utils
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/veriloga_structs/visitor  
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/parser 
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/netlist_struct
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/netlist_control
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/netlist_visitor
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/netlist_expansion
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/netlist_expression
INCLUDES_NETLIST_PARSER += -I./src/parser_netlist/scanner 

INCLUDES_MY += $(INCLUDES_NETLIST_PARSER)

# ----------------- END SOURCES ------------------

# sum up the sources and includes
SOURCES = $(SOURCE_UTILS) 
SOURCES += $(SOURCE_CDFREAD)   $(SOURCE_DEVICES)  $(SOURCE_NETLIST) 
SOURCES += $(SOURCE_ANALYSIS)  $(SOURCE_LA)       $(SOURCE_LS)      $(SOURCE_NS)
SOURCES += $(SOURCE_OPTIONS)   $(SOURCE_OUTPUT)   $(SOURCE_PROC)
INCLUDES = $(INCLUDES_MY) $(INCLUDES_NETLIST_PARSER)

SOURCES_NETLIST_PARSER = $(SOURCE_NETLIST_PARSER) 

LIBS = $(EXTLIBS)

OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
OBJECTS_PARSER = $(patsubst %.cpp, %.o, $(SOURCES_NETLIST_PARSER))