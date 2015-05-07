# ---------- Makefile of the tictac prototype project ----------

# HERE YOU NEED TO CHANGE TO YOUR BOOST DIRECTORY
BOOST_INCLUDE_PATH = ./

# include the stuff for flags & libs & ...
include cflags.mk

# --- in the following we set all sources ---- 
include allsource.mk

# This define is IMPORTANT for student version
FLAGS += -DSTUDENT_VERSION 

# now only the main file file the binary needs to be specified 
SOURCE_MAIN_SIMULATOR = src/main_simulator/main_simulator.cpp

OBJECT_MAIN_SIMULATOR = $(patsubst %.cpp, %.o, $(SOURCE_MAIN_SIMULATOR))

#------------ name of the binary file ------------------
BINARYTARGET_SIMULATOR = ./bin/tictac_simulator.$(OS)

##########################################################################
# Now the make rules 

default: all

$(BINARYTARGET_SIMULATOR) : $(OBJECTS) $(OBJECTS_PARSER) $(OBJECT_MAIN_SIMULATOR)
	$(CXX) $(CXXFLAGS) $(CLINKS) -o $(BINARYTARGET_SIMULATOR) $(OBJECTS) $(OBJECTS_PARSER) $(OBJECT_MAIN_SIMULATOR) $(LIBS)  
	
all : $(BINARYTARGET_SIMULATOR)  

%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $^ -o $@
 
clean:
	rm -f $(LINKTARGET) $(OBJECTS) $(OBJECT_MAIN) $(OBJECTS_PARSER) $(OBJECT_MAIN_SIMULATOR)