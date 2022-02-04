include ../../../project.mk
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Local modifications to standard tools parameters
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LIBS        +=  -l json -l dynarray -l argos -l m -l dl -l rt -lpthread
CFLAGS      +=  -DDEBUG
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Find all the C language source files in this directory
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SRCS := $(wildcard *.c)
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Make up the list of resultant object files & dependencies
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS := $(SRCS:%.c=$(DEP_DIR)/%.d)
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Define what we want to build and how to build it ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
TARGET := $(BIN_DIR)/argos_DeviceDriver

$(TARGET) : $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(COMPILER) $(CFLAGS) -I $(INC_DIR) -o $(TARGET) $(OBJS) $(LIBS)
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Over-ride some of make's internal implicit rules so that dependencies
# will be automatically and correctly re-made as required ...
#
# A lot of research and trial and error went into this so be VERY careful
# when modifying it. You have been warned!
#
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


POSTCOMPILE = mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

$(OBJ_DIR)/%.o : %.c

$(OBJ_DIR)/%.o : %.c $(DEP_DIR)/%.d 
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)
	$(COMPILER) $(DEPFLAGS) $(CFLAGS) -I $(INC_DIR) -c -o $@ $<
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






