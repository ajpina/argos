#ifndef ARGOS_PLUGIN_HEADER
#define ARGOS_PLUGIN_HEADER

typedef struct device device;

typedef int    ( * DEVICE_RTN )  ( device * D );
typedef void * ( * MANAGE_RTN )  ( void * V );
typedef void   ( * INIT_RTN   )  ( device * * cD, char * cN );

typedef struct device {
                           char         enabled;
                           mqd_t        mQ;      /* Message Queue Descriptor  */
                           int          shm_fd;
                           int          index;
                           int          devicePipe[2];
                           pthread_t    thread;
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Fields below here must be completed by the plugin as well as whatever
 * required fields in the pluginHandle that the plugin needs ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
                           char      *  deviceName;
                           int          sampleFrequency;   /* In microseconds   */
                           char         connected;         /* Are we connected? */
                           int          shm_size_required;
                           void      *  shmBlock;
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Routines in the plugin to perform different functions on the physical device
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
                           DEVICE_RTN   openConnection;
                           DEVICE_RTN   closeConnection;
                           MANAGE_RTN   manageDevice;
                           DEVICE_RTN   destroyDevice;
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Opaque Control Block used by the plugin. Contents known only to the plugin!
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
                           void      *  pluginControl;      /* Protocol Specific Control */
                      } device;



#endif

