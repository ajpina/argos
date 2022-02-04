include ../../../project.mk

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Local modification to basic tools
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
all : $(LIB_DIR)/STATIC/libdynarray.a $(LIB_DIR)/SHARED/libdynarray.so $(INC_DIR)/dynarray.h


$(LIB_DIR)/SHARED/libdynarray.so : $(OBJS)
	@mkdir -p $(LIB_DIR)/SHARED
	$(COMPILER) $(CFLAGS) -I $(INC_DIR) -shared -o $(LIB_DIR)/SHARED/libdynarray.so $(OBJS)

	
$(LIB_DIR)/STATIC/libdynarray.a :  $(OBJS)
	@mkdir -p $(LIB_DIR)/STATIC
	$(AR) $(ARFLAGS) $(LIB_DIR)/STATIC/libdynarray.a $(OBJS)

	
$(INC_DIR)/dynarray.h  :  dynarray.h
	@cp dynarray.h $(INC_DIR)
		
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
	mkdir -p $(OBJ_DIR) $(DEP_DIR)
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
	@rm -f $(LIB_DIR)/STATIC/libdynarray.*
	@rm -f $(LIB_DIR)/SHARED/libdynarray.*


	




