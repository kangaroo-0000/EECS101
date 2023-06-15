//
//  main.c
//  EECS101_hw4
//
//  Created by Beckham Pan on 2023/2/7.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define ROWS        (int)480
#define COLUMNS        (int)640

void cclear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );
int convolve_with_x_kernel(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h, unsigned char i);
int convolve_with_y_kernel(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h, unsigned char i);

int main( int argc, char **argv )
{

    int                i, j, k;
    int max[3];  // max[0] is max of Gx, max[1] is max of Gy, max[2] is max of SGM
    FILE            *fp;
    unsigned char    image[ROWS][COLUMNS], ximage[ROWS][COLUMNS], yimage[ROWS][COLUMNS], sgmimage[ROWS][COLUMNS], head[32];
    int xtmp[ROWS][COLUMNS], tmp[ROWS][COLUMNS], ytmp[ROWS][COLUMNS];
    char            filename[6][50], ifilename[50];

    strcpy( filename[0], "image1" );
    strcpy( filename[1], "image2" );
    strcpy( filename[2], "image3" );
    header ( ROWS, COLUMNS, head );

    int threshold[] = { 140, 16, 44 }; // choose thresholds values for each image for generate binary images

    printf( "Maximum of Gx, Gy, SGM\n" );

    for ( k = 0; k < 3; k ++)
    {
        cclear( ximage );
        cclear( yimage );
        cclear(sgmimage);

        /* Read in the image */
        strcpy( ifilename, filename[k] );
        if (( fp = fopen( strcat(ifilename, ".raw"), "rb" )) == NULL )
        {
          fprintf( stderr, "error: couldn't open %s\n", ifilename );
          exit( 1 );
        }
        for ( i = 0; i < ROWS ; i++ )
          if ( fread( image[i], sizeof(char), COLUMNS, fp ) != COLUMNS )
          {
            fprintf( stderr, "error: couldn't read enough stuff\n" );
            exit( 1 );
          }
        fclose( fp );

        max[0] = 0; //maximum of Gx, initialize
        max[1] = 0; //maximum of Gy, initialize
        max[2] = 0; //maximum of SGM, initialize

        for( i = 1; i < ROWS-1; i++){
            for( j = 1; j < COLUMNS-1; j++){
                // compute Gx and max[0]
                xtmp[i][j] = abs(image[i-1][j-1] + 2*image[i][j-1] + image[i+1][j-1] - image[i-1][j+1] - 2*image[i][j+1] - image[i+1][j+1]);
                max[0] = (xtmp[i][j] > max[0]) ? xtmp[i][j] : max[0];
                // compute Gy and max[1]
                ytmp[i][j] = abs(image[i+1][j-1] + 2*image[i+1][j] + image[i+1][j+1] - image[i-1][j-1] - 2*image[i-1][j] - image[i-1][j+1]);
                max[1] = (ytmp[i][j] > max[1]) ? ytmp[i][j] : max[1];
                //compute SGM and max[2]
                tmp[i][j] = sqrt((xtmp[i][j]*xtmp[i][j]) + (ytmp[i][j] * ytmp[i][j]));
                max[2] = (tmp[i][j] > max[2]) ? tmp[i][j] : max[2];
                
            }
        }
                
        for( i = 1; i< ROWS-1; i++){
            for( j = 1; j< COLUMNS-1; j++){
                // normalize Gx, Gy, SGM
                ximage[i][j] = ((float)(xtmp[i][j])/max[0] * 255);
                yimage[i][j] = ((float)(ytmp[i][j])/max[1] * 255);
                sgmimage[i][j] = ((float)(tmp[i][j])/max[2] * 255);
            }
        }


        /* Write Gx to a new image*/
        strcpy( ifilename, filename[k] );
        if (!( fp = fopen( strcat( ifilename, "-x.ras" ), "wb" ) ))
        {
          fprintf( stderr, "error: could not open %s\n", ifilename );
          exit( 1 );
        }
        fwrite( head, 4, 8, fp );

        for (i = 0; i < ROWS; i++)
            fwrite(ximage[i], sizeof(char), COLUMNS, fp);
        
        fclose( fp );
                    
        /* Write Gy to a new image */
        strcpy( ifilename, filename[k] );
        if (!( fp = fopen( strcat( ifilename, "-y.ras" ), "wb" ) ))
        {
          fprintf( stderr, "error: could not open %s\n", ifilename );
          exit( 1 );
        }
        fwrite( head, 4, 8, fp );

        for (i = 0; i < ROWS; i++)
            fwrite(yimage[i], sizeof(char), COLUMNS, fp);

        fclose( fp );
    
        /* Write SGM to a new image */
        strcpy( ifilename, filename[k] );
        if (!( fp = fopen( strcat( ifilename, "-s.ras" ), "wb" ) ))
        {
          fprintf( stderr, "error: could not open %s\n", ifilename );
          exit( 1 );
        }
        fwrite( head, 4, 8, fp );
        for (i = 0; i < ROWS; i++)
            fwrite(sgmimage[i], sizeof(char), COLUMNS, fp);
        fclose( fp );
        
        /* Compute the binary image */ // need to check histogram and select threshold
        // Write your code here ... you can reuse variable 'ximage' to store values for binary image
        cclear(ximage);
        for(i = 0; i < ROWS; i++){
            for(j = 0; j < COLUMNS; j++){
                ximage[i][j] = (sgmimage[i][j] < threshold[k]) ? 0 : 255;
            }
        }

        
        /* Write the binary image to a new image */
        strcpy( ifilename, filename[k] );
        if (!( fp = fopen( strcat( ifilename, "-b.ras" ), "wb" ) ))
        {
          fprintf( stderr, "error: could not open %s\n", ifilename );
          exit( 1 );
        }
        fwrite( head, 4, 8, fp );
        for (i = 0; i < ROWS; i++)
            fwrite(ximage[i], sizeof(char), COLUMNS, fp);
        fclose( fp );

        printf( "%d %d %d\n", max[0], max[1], max[2] );

    }

    printf( "Press ENTER to exit: " );
    getchar();
    return 0;
}

void cclear( unsigned char image[][COLUMNS] )
{
    int    i,j;
    for ( i = 0 ; i < ROWS ; i++ )
        for ( j = 0 ; j < COLUMNS ; j++ ) image[i][j] = 0;
}

void header( int row, int col, unsigned char head[32] )
{
    int *p = (int *)head;
    char *ch;
    int num = row * col;

    /* Choose little-endian or big-endian header depending on the machine. Don't modify this */
    /* Little-endian for PC */
    
    *p = 0x956aa659;
    *(p + 3) = 0x08000000;
    *(p + 5) = 0x01000000;
    *(p + 6) = 0x0;
    *(p + 7) = 0xf8000000;

    ch = (char*)&col;
    head[7] = *ch;
    ch ++;
    head[6] = *ch;
    ch ++;
    head[5] = *ch;
    ch ++;
    head[4] = *ch;

    ch = (char*)&row;
    head[11] = *ch;
    ch ++;
    head[10] = *ch;
    ch ++;
    head[9] = *ch;
    ch ++;
    head[8] = *ch;
    
    ch = (char*)&num;
    head[19] = *ch;
    ch ++;
    head[18] = *ch;
    ch ++;
    head[17] = *ch;
    ch ++;
    head[16] = *ch;
    

    /* Big-endian for unix */
    /*
    *p = 0x59a66a95;
    *(p + 1) = col;
    *(p + 2) = row;
    *(p + 3) = 0x8;
    *(p + 4) = num;
    *(p + 5) = 0x1;
    *(p + 6) = 0x0;
    *(p + 7) = 0xf8;
*/
}

int convolve_with_x_kernel(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h, unsigned char i){
    return abs(a*(-1) + b*0 + c*1 + d*(-2) + e*0 + f*2 + g*(-1) + h*0 + i*1);
}
int convolve_with_y_kernel(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h, unsigned char i){
    return abs(a*(-1) + b*(-2) + c*(-1) + d*0 + e*0 + f*0 + g*1 + h*2 + i*1);
}
