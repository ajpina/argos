# ARGOS
## About the ARGOS project

Argos was an open source SCADA system originally written in C++ with all documentation and program comments in Spanish. The programs were configured and controlled by XML configuration files. As an open source project it failed to attract many developers and the project was largely abandoned by it's originators and little or no development was done on the system for a few years. Recently however the system has attracted some attention because it is small and is easy to implement and house on Linux based systems.

This current project has as it's primary focus the following goals

* to re-write the core functionality of Argos in C
* to document the system in English
* to control program configuration and execution with JSON configuration files
* to provide tools to automate as much of the setup and configuration of the system as possible
* to complete the HMI portions of the system in an appropriate graphical system

## Project Source Repository

At the moment the project is hosted in a _**private**_ repository managed by the Fossil Distributed Version Control System. We intend to provide a GitHub mirror repository for any developers who prefer to use Git. Because the repository is private and not visible publically, developers who wish to participate and contribute code or fixes or suggestions or documentation will need to be authorized to push their changes to the repository. This can be achieved by ????

## Build Environment

The project is being actively developed on Linux systems ( Ubuntu and Debian ) using a standard toolchain of __gcc__, __gdb__ and __make__. Visual debugging is facilitated with the 3rd party __ddd__ debugger. Third party open-source libraries MODBUS and MUPARSER are also used. A classical recursive make is used to run over the project tree and build the required sub-projects.

Three small modifications to the normal Linux environment need to be made to facilitate Argos developement -

1. *The Linux kernel needs to be told to create core dump files.* The command _sudo sysctl -w kernel.core_pattern="core"_ will tell the Linux kernel to create dump files named _core_ in the current working directory.
2. *The ulimit for core dumps need to be set at unlimited.* The command to do this is _ulimit -c unlimited_ in the BASH shell.
3. *The LOCAL_3 system logs need to be setup to log into separate files to receive Argos messages* On Ubuntu systems this is done as follows:
  *  *cd /var/log*
  *  *sudo mkdir argos*
  *  *cd argos*
  *  *sudo touch argos.log*
  *  *sudo chmod a+rw argos.log*
  *  *cd /etc/syslog.d*
  *  *sudo gedit 50-default.conf*
  *  *Add a line "local3.\* ~/var/log/argos/argos.log" to the file*
  *  *save the file*
  *  *cd ~*
  *  *sudo service rsyslog restart*


## Build Instructions
Because this is a classical recursive make build environment you can go to any directory in the project source tree ( i.e. the SOURCE directory ) and type __make__. This will result in the code in that directory being built if necessary and all the sub-projects further down the tree also being built.

However, if you have just checked out the repository into your private sandbox, it is necessary before any other command to enter the project directory and type __make__. This will cause some directories under ARCH to be built for your particular machine's architecture.

All the makefiles in the project support the __all__ and __clean__ targets.

## Runtime Environment
Most of the programs in the ARGOS system require command line arguments to locate and process their json configuration files. For details about the unique command line parameters of each program consult the source code. Many programs will operate as system daemons and log their messages to the argos.log file set up previously.

## Project Timeline & Milestones
1. Establish project repository
2. Establish project tree
3. Establish project buid environment
2. Convert MODBUS emulator
3. Convert MODBUS driver
4. ???
5. ???
