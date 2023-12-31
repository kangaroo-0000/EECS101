//
//  main.c
//  EECS101_hw2
//
//  Created by Beckham Pan on 2023/1/27.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define ROWS 100
#define COLUMNS 100

int main( int argc, char **argv )
{
int i;
FILE *fp;
float u = 0.0, var = 0.0;
unsigned char image[ROWS][COLUMNS];
char *ifile = NULL;
char filename[4][50];
strcpy(filename[0], "image1.raw");
strcpy(filename[1], "image2.raw");
strcpy(filename[2], "image3.raw");
strcpy(filename[3], "image4.raw");
    
//Assign each image name in filename to ifile here
/* example: ifile = filename[k]; k=0,1,2,3; a loop might be needed*/
    
for(int k = 0; k < 4; k++){
    ifile = filename[k];
    if (( fp = fopen( ifile, "rb" )) == NULL )
    {
      fprintf( stderr, "error: couldn't open %s\n", ifile );
      exit( 1 );
    }
    for ( i = 0; i < ROWS ; i++ )
      if ( fread( image[i], 1, COLUMNS, fp ) != COLUMNS )
      {
        fprintf( stderr, "error: couldn't read enough stuff\n" );
        exit( 1 );
      }
    fclose( fp );
    
    //Calculate Mean for each image here
    float sum = 0;
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            sum += image[i][j];
        }
    }
    u = sum / (ROWS*COLUMNS);
    
    //Calculate Variance for each image here
    sum = 0;
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            sum += (image[i][j] - u)*(image[i][j] - u);
        }
    }
    var = sum / ((ROWS*COLUMNS) - 1);
    //Print mean and variance for each image
    printf("%s: %f %f\n", ifile, u, var);
}



printf("Press ENTER to exit. ");
getchar();
return 0;
}
