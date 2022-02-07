include ../../../project.mk
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Local modifications to standard tools parameters
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LIBS        +=  -l json -l dynarray -l argos -l m -l dl -l rt -lpthread

CFLAGS      +=  -D_DEBUG_
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Find all the C language source files in this directory
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SRCS := $(wildcard *.cpp)
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Make up the list of resultant object files & dependencies
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(SRCS:%.cpp=$(DEP_DIR)/%.d)
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Define what we want to build and how to build it ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# Not Yet (ajpina)
TARGET := $(BIN_DIR)/argos_Scanner

$(TARGET) : $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(GPP) $(CFLAGS) -I $(INC_DIR) $(OBJS) -o $(TARGET) $(LIBS)


# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Over-ride some of make's internal implicit rules so that dependencies
# will be automatically and correctly re-made as required ...
#
# A lot of research and trial and error went into this so be VERY careful
# when modifying it. You have been warned!
#
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


POSTCOMPILE = mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

$(OBJ_DIR)/%.o : %.cpp

$(OBJ_DIR)/%.o : %.cpp $(DEP_DIR)/%.d
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)
	$(GPP) $(DEPFLAGS) $(CFLAGS) -I $(INC_DIR) -c -o $@ $<
	$(POSTCOMPILE)

$(DEPS):
include $(wildcard $(DEPS))

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Finally the project actions which are not based on actual files ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.PHONY :  clean

clean :
	@rm -f $(OBJS)
	@rm -f $(DEPS)
	@rm -f $(TARGET)






