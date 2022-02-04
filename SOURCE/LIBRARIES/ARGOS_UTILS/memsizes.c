#include <stdio.h>
#include <sys/sysinfo.h>

void printmemsize(char *str, unsigned long ramsize) {
        printf("%12s: %12ld in bytes / %12ld in KB / %12ld in MB / %12ld in GB\n",
               str, 
               ramsize, 
               ramsize/1024, 
               (ramsize/1024)/1024, 
               ((ramsize/1024)/1024)/1024
              );
}

void showMemSizes( void ) {
struct sysinfo info;

        sysinfo(&info);
        printf("uptime: %ld\n", info.uptime);
        // print total ram size
        printmemsize("totalram", info.totalram);
        printmemsize("freeram", info.freeram);
        printmemsize("sharedram", info.sharedram);
        printmemsize("bufferram", info.bufferram);
        printmemsize("freeswap", info.freeswap);
        printf("current running processes: %d\n", info.procs);
}

