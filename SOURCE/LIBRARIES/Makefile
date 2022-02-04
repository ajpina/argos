include ../../project.mk
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Define what we want to build and how to build it ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
all  :
	make -C ARGOS_UTILS
	make -C JSON
	make -C MODBUS
	make -C DYNARRAY
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Finally the project actions which are not based on actual files ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.PHONY :  clean

clean :
	make -C ARGOS_UTILS    clean
	make -C JSON           clean
	make -C MODBUS         clean
	make -C DYNARRAY       clean




