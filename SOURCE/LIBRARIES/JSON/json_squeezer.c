

#include "privateJsonHeader.h"

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * I have discovered that our Json parser will allow us to be
 * very agressive when compressing the source. It will allow us
 * to remove ALL whitespace characters form the source ...
 *
 * If your json parser will not allow this, simply undefine
 * the VERY_AGRESSIVE_COMPRESSION directive and re-compile
 * the library.
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#define VERY_AGRESSIVE_COMPRESSION


int  json_squeezer ( char * dd, char * fn ) {
char             * inFileName;
char             * outFileName;
char             * line = NULL;
char             * jsonSqueezed;
char             * p;

FILE             * fIn;
FILE             * fOut;

int                fd;
unsigned long int  n    =    0,
                   l    =    0;

   inFileName = ( char * ) calloc( 1024, 1 );
   strcpy( inFileName, dd );
   strcat( inFileName, fn );

   outFileName = ( char * ) calloc( 1024, 1 );
   strcpy( outFileName, dd );
   strcat( outFileName, fn );
   strcat( outFileName, ".sqzd" );

   fIn = fopen( inFileName, "r" );
   if ( fIn == NULL ) {
      printf("Unable to open file <%s>!\n",inFileName );
      exit(4);
   }

   fOut = fopen( outFileName, "w" );
   if ( fOut == NULL ) {
      printf("Unable to open file <%s>!\n",outFileName );
      exit(4);
   }

   while ( getline( &line, &n, fIn ) != -1 ) {
      while ( ( p = strchr( line, '\r' ) ) != NULL ) {  /* In case the file was created on Windows */
        *p = ' ';
      }
#ifdef  VERY_AGRESSIVE_COMPRESSION
      while ( ( p = strchr( line, ' ' ) ) != NULL ) {
         memmove( p, p+1, strlen( p) );
      }   
#else
      while ( ( p = strstr( line, "  ") ) != NULL ) {
         memmove( p+1, p+2, strlen(p) );
      }
#endif  
      p = strchr(line,'\n');
     *p = '\0';
      fputs( line, fOut );    
   }
   fclose( fIn  );
   fclose( fOut );
   free( line );   
   free( inFileName );

   fd = open( outFileName, O_RDONLY );
   l  = lseek(fd, 0L, SEEK_END );    // How big is the file
   jsonSqueezed = (char *)calloc(l+1,1);
   lseek(fd,0L,SEEK_SET);            // Back to the beginning 
   read(fd,jsonSqueezed,l);          // Read the data into our buffer
   close(fd);  

   int i = json_valid( jsonSqueezed );
   free( jsonSqueezed );
   free( outFileName );
   return i;
}
