//
//  main.c
//  EECS101_hw5
//
//  Created by Beckham Pan on 2023/2/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#define ROWS    480
#define COLUMNS    640
#define PI 3.14159265358979323846

#define sqr(x)    ((x)*(x))



void cclear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );
void fill_line_in_voting_array(unsigned char voting[180][400], int i, int j);
void find_top_4(unsigned char voting[180][400], int max[4], int index[4][2]);

int main( int argc, char** argv )
{
    int i,j;
    int max[3], localmaxima[4] = {0, 0, 0, 0}; // max[0] is max of Gx, max[1] is max of Gy, max[2] is max of SGM.
    int xtmp[ROWS][COLUMNS], tmp[ROWS][COLUMNS], ytmp[ROWS][COLUMNS];
    int index[4][2] = { 0, 0, 0, 0, 0, 0, 0, 0};
    int sgm_threshold = 140, hough_threshold;
    unsigned char ximage[ROWS][COLUMNS], yimage[ROWS][COLUMNS], sgmimage[ROWS][COLUMNS], image[ROWS][COLUMNS], bimage[ROWS][COLUMNS], outputimage[ROWS][COLUMNS], head[32], voting[180][400];
    char filename[50];
    FILE* fp;
    
    strcpy ( filename, "image.raw");
    //memset ( voting, 0, sizeof(int) * 180 * 400 );  //180 * 400 is the suggested size of voting array
    header(ROWS, COLUMNS, head);

    /* Read in the image */
    if (!( fp = fopen(filename, "rb" ) ))
    {
        fprintf( stderr, "error: couldn't open %s\n", argv[1]);
        exit(1);
    }

    for ( i = 0 ; i < ROWS ; i++ )
        if (!( COLUMNS == fread( image[i], sizeof(char), COLUMNS, fp ) ))
        {
            fprintf( stderr, "error: couldn't read %s\n", argv[1] );
            exit(1);
        }
    fclose(fp);


    cclear( ximage );
    cclear( yimage );
    cclear(sgmimage);
    cclear(bimage);
    cclear(outputimage);
    

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

    

    /* Save SGM to an image */
    strcpy(filename, "image");
    if (!(fp = fopen(strcat(filename, "-sgm.ras"), "wb")))
    {
        fprintf(stderr, "error: could not open %s\n", filename);
        exit(1);
    }
    fwrite(head, 4, 8, fp);
    for (i = 0; i < ROWS; i++)
        fwrite(sgmimage[i], sizeof(char), COLUMNS, fp);
    fclose(fp);


    /* Compute the binary image, thresholded SGM */
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
            bimage[i][j] = (sgmimage[i][j] <= sgm_threshold) ? 0 : 255;
        }
    }
    
    /*Compute the voting array*/
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
            if(bimage[i][j] == 255){
                fill_line_in_voting_array(voting, j, ROWS - i - 1);
            }
        }
    }
    

    /* Save the thresholded SGM to an image */
    strcpy(filename, "image");
    if (!(fp = fopen(strcat(filename, "-binary.ras"), "wb")))
    {
        fprintf(stderr, "error: could not open %s\n", filename);
        exit(1);
    }
    fwrite(head, 4, 8, fp);
    for (i = 0; i < ROWS; i++)
        fwrite(bimage[i], sizeof(char), COLUMNS, fp);
    fclose(fp);

    



    /* Save original voting array to an image */
    strcpy(filename, "image");
    header(180, 400, head);
    if (!(fp = fopen(strcat(filename, "-voting_array.ras"), "wb")))
    {
        fprintf(stderr, "error: could not open %s\n", filename);
        exit(1);
    }
    fwrite(head, 4, 8, fp);

    for (i = 0; i < 180; i++)
        fwrite(voting[i], sizeof(char), 400, fp);
    fclose(fp);
    
    
    /*find local maxima*/
    find_top_4(voting, localmaxima, index);
    
    /*find minimum of the local maxima, which will be the hough threshold value*/
    hough_threshold = localmaxima[2];
    
    /* Threshold the voting array */
    for(i = 0; i < 180; i++){
        for(j = 0; j < 400; j++){
            voting[i][j] = (voting[i][j] < hough_threshold) ? 0: 255;
        }
    }





    printf("Hough threshold: %d\n", hough_threshold);
    printf("%d %d %d\n%d %d %d\n%d %d %d\n", index[0][0], index[0][1], localmaxima[0],
                                            index[1][0], index[1][1] , localmaxima[1],
                                            index[2][0], index[2][1], localmaxima[2]);

    /* Reconstruct an image from the voting array */
 
    for(i = 0; i < ROWS-1; i++){
        for(j = 0; j < COLUMNS-1; j++){
            outputimage[i][j] = 0;
        }
    }

    for(i = 0; i < ROWS-1; i++){
        for(j = 0; j < COLUMNS-1; j++){
            float radians = (index[2][0] * PI) / 180;
            int rho = (index[2][1]-200)*3;
            if((int)((j*sinf(radians)) - ((ROWS-i-1)*cosf(radians)) + rho) == 0){
                outputimage[i][j] = 255;
            }
            
            radians = (index[1][0] * PI) / 180;
            rho = (index[1][1]-200)*3;
            if((int)((j*sinf(radians)) - ((ROWS-i-1)*cosf(radians)) + rho) == 0){
                outputimage[i][j] = 255;
            }
            
            radians = (index[0][0] * PI) / 180;
            rho = (index[0][1]-200)*3;
            if((int)((j*sinf(radians)) - ((ROWS-i-1)*cosf(radians)) + rho) == 0){
                outputimage[i][j] = 255;
            }
        }
    }



    /* Write the reconstructed figure to an image */
    strcpy(filename, "image");
    header(ROWS, COLUMNS, head);
    if (!(fp = fopen(strcat(filename, "-reconstructed_image.ras"), "wb")))
    {
        fprintf(stderr, "error: could not open %s\n", filename);
        exit(1);
    }
    fwrite(head, 4, 8, fp);
    for (i = 0; i < ROWS; i++)
        fwrite(outputimage[i], sizeof(char), COLUMNS, fp);
    fclose(fp);

    printf("Press ENTER key to exit: ");
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


void fill_line_in_voting_array(unsigned char voting[180][400], int x, int y){
    // aka Hough Transform
    int theta_index, rho_index;
    float rho;

    for (int theta = 0; theta < 180; theta++) {
        rho = (float)y * cosf(theta * PI / 180.0) - (float)x * sinf(theta * PI / 180.0);
        theta_index = theta;
        rho_index = (int)((rho / 4.0) + 200.0); //normailized rho
        voting[theta_index][rho_index]++;
    }
}

void find_top_4(unsigned char voting[180][400], int max[4], int index[4][2]) {
    // skips a max if the indicies are too close to existing ones stored in index[]
    max[0] = max[1] = max[2] = max[3] = -1;
    int i, j, k, idx, idy, dist;
    int temp_max, temp_idx = 0, temp_idy = 0;
    int max_dist = 10;
    bool is_far_enough;

    // find top 4 maxima
    for (k = 0; k < 4; k++) {
        temp_max = -1;
        dist = 180 * 180 + 400 * 400; // set dist to a large enough value

        for (i = 0; i < 180; i++) {
            for (j = 0; j < 400; j++) {
                // check if current value is greater than temp_max
                if (voting[i][j] > temp_max) {
                    // check if current value is far enough from existing maxima
                    is_far_enough = true;

                    for (idx = 0; idx < k; idx++) {
                        idy = index[idx][0];
                        if (idy < 0) break;

                        // check if current index is far enough from existing index
                        if (abs(i - idy) < max_dist && abs(j - index[idx][1]) < max_dist && abs(j - index[idx][1]) >= abs(i - idy)) {
                            is_far_enough = false;
                            break;
                        }
                    }

                    if (is_far_enough) {
                        temp_max = voting[i][j];
                        temp_idx = i;
                        temp_idy = j;
                    }
                }
            }
        }

        // update max and index arrays with new maximum
        max[k] = temp_max;
        index[k][0] = temp_idx;
        index[k][1] = temp_idy;
    }

}

