#include "driver.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

Stock equityArray[MAX_TICKERS];


/*
    https://www.tutorialspoint.com/c_standard_library/
    The C Standard Library was referenced via tutorialspoint.com
*/

// To compile run this command
// gcc driver.c -lcurl -pthread -o driver
// To run
// ./driver

void* t_run(void* i)
{
    // creates a string of the GET request using the tickers located in the infile
    char* currTicker = (char*)i;
    char* request = malloc(sizeof(char)*1000);
    strcpy(request, "https://cloud.iexapis.com/stable/stock/");
    strcat(request, currTicker);
    //   printf("current ticker is %s\n", currTicker);
    strcat(request, "/quote?token=pk_3a3d74105f3c4e0996ccb90192014c97");
    //  strcat(request, ",news,chart&range=1m&last=10/"); //Should we decide to do more than just basic price info
    //  printf("Thread GET request :  %s\n",request);

    // creates a curl object and gives it the CURLOPT_URL of the GET request
    // then returns the JSON of the GET request
    CURL *curl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, request);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    curl_easy_perform(curl);
    printf("\n");

    curl_easy_cleanup(curl); // Some kind of curl cleanup command that needs to be run afterwards apparently

}



int main(){

  char* infileName = "tickerList.txt";
  char* outfileName = "result.txt";
  FILE  *fpath;

  fpath = fopen(infileName, "r" );
  if( fpath == NULL ){perror( "ERROR OPENING FILE: %s\n"); return FAIL;}


  char tickArray[MAX_TICKERS][MAX_TICKER_LENGTH];   //stores our ticker strings
  char *ticker = malloc( sizeof( strlen("XXXXX") ) );   //points to ticker being read in the ith line
  int i = 0;

  while ( fgets( ticker, MAX_TICKERS, fpath ) != NULL ){  // fgets() will read each line from our file stream either until EOF or an error occurance
    ticker[strlen(ticker)-1] = '\0';    // scrub our unwanted \n characters

    strcpy( tickArray[i], ticker );
    //printf( "%s\n", ticker);
    i++;
  }

  int num_tickers = i;
  printf("Num Tickers: %d\n", num_tickers);



  // Testing our file input and ticker storage
  int j=0;
  while(j<num_tickers && tickArray[j] != NULL){
    printf("Ticker Number %d is: %s\n", j , tickArray[j] );
    j++;
  }
  //

    curl_global_init(CURL_GLOBAL_DEFAULT); // Initializes the global curl variable (not thread safe)


  pthread_t t_array[num_tickers];

  freopen(outfileName,"w", stdout);


  for( int i = 0; i < num_tickers; i++ ){

    if( pthread_create( &t_array[i], NULL, t_run, &tickArray[i] ) !=  0 ){ perror("THREAD CREATION FAILED: \n" ); return FAIL;}
    //printf("Thread made: %d\n", i );

  }

  for( int i = 0; i < num_tickers; i++ ){

    if( pthread_join( t_array[i], NULL) != 0 ){perror("THREAD FAILED TO JOIN\n"); return FAIL;}
    //printf("Thread joined: %d\n", i );

  }



    fclose(fpath);
    fclose(stdout);
    return 0;

}
