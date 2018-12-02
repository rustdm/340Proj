#include "driver.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
    https://www.tutorialspoint.com/c_standard_library/
    The C Standard Library was referenced via tutorialspoint.com
*/

int main(){

  char* infileName = "tickerList.txt";
  FILE  *fpath;

  fpath = fopen(infileName, "r" );
  if( fpath == NULL ){perror( "ERROR OPENING FILE: %s\n" ); return FAIL;}


  char tickArray[MAX_TICKERS][MAX_TICKER_LENGTH];   //stores our ticker strings
  char *ticker = malloc( sizeof( strlen("XXXXX") ) );   //points to ticker being read in the ith line
  int i = 0;

  while ( fgets( ticker, MAX_TICKERS, fpath ) != NULL ){  // fgets() will read each line from our file stream either until EOF or an error occurance
    ticker[strlen(ticker)-1] = '\0';    // scrub our unwanted \n characters

    strcpy( tickArray[i], ticker );
    printf( "%s\n", ticker);
    i++;
  }

  int num_tickers = i;
  printf("%d\n", num_tickers);


  /* Testing our file input and ticker storage */
  int j=0;
  while(j<num_tickers && tickArray[j] != NULL){
    printf("Ticker Number %d is: %s\n", j , tickArray[j] );
    j++;
  }


/*

  pthread_t t_array[num_tickers];


  for( int i = 0; i < num_tickers; i++ ){

    if( pthread_create( &t_array[i], NULL, t_run, &i ) !=  0 ){ perror("THREAD CREATION FAILED: AT %d\n", i ); return FAIL;}
    printf("Thread made: %d\n", i );

  }


    if( pthread_join( t_array[i], NULL) != 0 ){perror("THREAD FAILED TO JOIN\n"); return FAIL;}
    printf("Thread joined: %d\n", i );

  }

*/

  fclose(fpath);

  return 0;


}