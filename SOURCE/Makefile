include ../project.mk
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Define what we want to build and how to build it ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

all :
	make -C LIBRARIES
	make -C PLUGINS
	make -C EXECUTABLES
#	make -C TESTS


# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Finally the project actions which are not based on actual files ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.PHONY :  clean

clean :
	make -C LIBRARIES   clean
	make -C PLUGINS     clean
	make -C EXECUTABLES clean
#	make -C TESTS       clean





