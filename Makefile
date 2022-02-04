include project.mk
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Define what we want to build and how to build it ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
all :
	make -C $(SRC_DIR)
	@echo "Build complete!"

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Finally the project actions which are not based on actual files ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.PHONY :  clean scrub 


scrub : clean
	find ./          -name "*.d"    -print | xargs rm -f
	find ./          -name "*.o"    -print | xargs rm -f
	find ./          -name tags     -print | xargs rm -f
	find ./          -name core     -print | xargs rm -f
	find /dev/shm    -name "ARGOS*" -print | xargs rm -f
	find /dev/mqueue -name "ARGOS*" -print | xargs rm -f
	
clean :	
	@rm -f  $(BIN_DIR)/*
	@rm -rf $(PRJ_DIR)/DEPENDS/*
	@rm -rf $(PRJ_DIR)/OBJECT/*
	@rm -rf $(LIB_DIR)/*
	make -C $(SRC_DIR) clean





