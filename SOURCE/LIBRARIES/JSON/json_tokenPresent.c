/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          json_tokenPresent.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021 
 *
 * @brief         Check that a given attribute is present in a JSON file    
 *
 * This function checks for the presence of a given attribute name in the
 * current SON file. 
 *        
 *
 * @param      T - the string name of the attribute to check for.
 * @return     1 if attribute present in file, 0 otherwise
 * 
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
 
 #include "privateJsonHeader.h"
 
 typedef struct _tokenSearch {
                                char * T;
                                int    found;
                             } tokenSearch;
                             
 void  json_tokenPresent_Callback( char * N, char * V, int level, void * userData ) {
 tokenSearch * tS;
    tS = ( tokenSearch * ) userData;
    if ( strcmp( N, tS->T ) == 0 ) { tS->found = 1; }
 }
  
 int json_tokenPresent( char * T ) {
 tokenSearch tS;
 char * wkString;
 char * X;
 
    wkString = ( char * ) calloc( 1024,1 );
    strcpy( wkString, "$" );
    if ( T[0] == ',' ) {
		strcat( wkString,T );
    } else {
	    strcat( wkString, "." );
	    strcat( wkString, T );	
    }
    X = strrchr( wkString,'.' );
    if ( X == NULL ) {
		tS.T     =  T;
		tS.found =  0;
		json_each("$", json_tokenPresent_Callback, ( void * ) &tS );
    } else {
		*X       = '\0';
		tS.T     = X+1;
		tS.found = 0;
		json_each( wkString, json_tokenPresent_Callback, ( void * ) &tS );
    }
    free( wkString );
    return tS.found; 
 }
 
