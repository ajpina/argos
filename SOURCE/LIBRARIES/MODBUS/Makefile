include ../../../project.mk
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Local modification to basic tools
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFLAGS += -I ./ -D _GNU_SOURCE
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
all : $(LIB_DIR)/STATIC/libmodbus.a $(LIB_DIR)/SHARED/libmodbus.so $(INC_DIR)/modbus.h $(INC_DIR)/modbus-rtu.h $(INC_DIR)/modbus-tcp.h $(INC_DIR)/modbus-version.h


$(LIB_DIR)/SHARED/libmodbus.so : $(OBJS)
	@mkdir -p $(LIB_DIR)/SHARED
	$(COMPILER) $(CFLAGS) -I $(INC_DIR) -shared -o $(LIB_DIR)/SHARED/libmodbus.so $(OBJS)
	@cp modbus*.h $(INC_DIR)
	@rm $(INC_DIR)/*-private*
	
$(LIB_DIR)/STATIC/libmodbus.a :  $(OBJS)
	@mkdir -p $(LIB_DIR)/STATIC
	$(AR) $(ARFLAGS) $(LIB_DIR)/STATIC/libmodbus.a $(OBJS)
	@cp modbus*.h $(INC_DIR)
	@rm $(INC_DIR)/*-private*
	
$(INC_DIR)/modbus.h : modbus.h
	@cp modbus.h $(INC_DIR)
	
$(INC_DIR)/modbus-rtu.h : modbus-rtu.h
	@cp modbus-rtu.h $(INC_DIR)
	
$(INC_DIR)/modbus-tcp.h : modbus-tcp.h
	@cp modbus-tcp.h $(INC_DIR)
	
$(INC_DIR)/modbus-version.h : modbus-version.h
	@cp modbus-version.h $(INC_DIR)
		
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
	@rm -f $(LIB_DIR)/STATIC/libmodbus.*
	@rm -f $(LIB_DIR)/SHARED/libmodbus.*


	




