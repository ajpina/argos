include ../../../project.mk
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
	
all : $(LIB_DIR)/STATIC/libargos.a $(LIB_DIR)/SHARED/libargos.so $(INC_DIR)/argos.h $(INC_DIR)/argosPlugin.h


$(LIB_DIR)/SHARED/libargos.so : $(OBJS)
	@mkdir -p $(LIB_DIR)/SHARED
	$(COMPILER) $(CFLAGS) -I $(INC_DIR) -shared -o $(LIB_DIR)/SHARED/libargos.so $(OBJS)
	
$(LIB_DIR)/STATIC/libargos.a :  $(OBJS)
	@mkdir -p $(LIB_DIR)/STATIC
	$(AR) $(ARFLAGS) $(LIB_DIR)/STATIC/libargos.a $(OBJS)
	
$(INC_DIR)/argos.h : ./argos.h
	cp ./argos.h $(INC_DIR)/argos.h 
	
$(INC_DIR)/argosPlugin.h : ./argosPlugin.h
	cp ./argosPlugin.h $(INC_DIR)/argosPlugin.h 
	
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
	@rm -f $(LIB_DIR)/STATIC/libargos.*
	@rm -f $(LIB_DIR)/SHARED/libargos.*


	




