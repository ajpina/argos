#include "argos.h"

int ARGOS_IS_DAEMONIZED  =  0;  /* Default is NO */

void daemonize( void ) {
        /* Our process ID and Session ID */
        pid_t pid, sid;
        
        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) {
                exit( 4 );
        }
        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) {
                exit( 0 );
        }
        /* Change the file mode mask */
        umask(0);       
        /* Open any logs here */        
                
        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) {
                /* Log the failure */
                exit( 4 );
        }
        /* Change the current working directory */
        if ((chdir("/")) < 0) {
                /* Log the failure */
                exit( 4 );
        }
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        
        ARGOS_IS_DAEMONIZED = 1;
}
