# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Define what we want to build and how to build it ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
all  :
	make -C JSON_SQUEEZER
	make -C MODBUS_EMULATOR
	make -C DRIVER
	make -C CORE
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Finally the project actions which are not based on actual files ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.PHONY :  clean

clean :
	make -C JSON_SQUEEZER    clean
	make -C MODBUS_EMULATOR  clean
	make -C DRIVER           clean
	make -C CORE             clean




