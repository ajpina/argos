#ifndef PRIVATE_MODBUS_TCP_PLUGIN_HEADER
#define PRIVATE_MODBUS_TCP_PLUGIN_HEADER


#include "argos.h"

#include "argosPlugin.h"    /* This defines the Device Control Block  */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Here we define the Control Block of information that is unique to
 * a specific protocol and carrier. This Control Block will be pointed
 * to by the device.
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include "modbus.h"
#include "dynarray.h"
#include "json.h"

typedef struct dpoint {
                        char * address;    /* In the device */
                        int    dataType;
                        int    quantity;   /* How many contiguous values */
                        int    targetType;
                        char   writeable;
                     } dpoint;

typedef struct  mbTypes {
                          char  *  typeName;
                          int      enumValue;
                        } mbTypes;


enum targetTypes {
                    UNKNOWN_TARGET = 0,
                    ICOIL_TARGET,
                    OCOIL_TARGET,
                    IREG_TARGET,
                    HREG_TARGET
                 };


typedef struct  dataReg {
                           char     *  addressInDevice;
                           int         dataType;
                           int         howMany;
                           int         targetType;
                           char        writeable;
                         } dataReg;
                         

typedef struct  evalReg {
                           int          addressInDevice;
                           int          targetType;
                           dataValue    value;   
                        } evalReg;
                        
typedef struct  readPlan {
                           int          startAddr;
                           int          numberToAccess;
                           int          dataType;
                         } readPlan;


 typedef struct mbTcpControl {
                               char      *  ipAddress;
                               char      *  ipPort;
                               modbus_t  *  mbContext;
                               dataReg   *  regArray;          /* Dynamic Array  */
                               evalReg   *  expandedRegArray;  /* Dynamic Array  */
                               readPlan  *  readPlanArray;     /* Dynamic Array  */
                             }  mbTcpControl;


#endif
