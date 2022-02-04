/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          destroyDevice.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021
 *
 * @brief         This function close the MODBUS context and frees all memory
 *                and control blocks we have allocated.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include "privateModbusTcpHeader.h"

int ARGOS_MODBUS_TCP_destroyDevice( device * D ) {
 mbTcpControl * P;
    P = ( mbTcpControl * ) D->pluginControl;
    free( P->ipAddress );
    free( P->ipPort );
    dynarray_destroy( P->regArray );
    dynarray_destroy( P->expandedRegArray );
    modbus_free( P->mbContext );
    free( P );
    D->pluginControl = NULL;
    return 0;
 }


