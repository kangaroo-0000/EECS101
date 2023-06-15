//
//  main.c
//  EECS_101_hw1
//
//  Created by Beckham Pan on 2023/1/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS                     128
#define COLS                 128
#define LOGICAL_X_MIN            -4.0
#define LOGICAL_X_MAX             4.0
#define LOGICAL_Y_MIN            -4.0
#define LOGICAL_Y_MAX             4.0

void read_image(unsigned char image[][COLS], char filename[]);
void write_image(unsigned char image[][COLS], char filename[]);
void do_orthogonal(float x0, float y0, float z0, float a, float b, float c, float fprime, float start, float end, unsigned char image[][COLS]);
void do_perspective(float x0, float y0, float z0, float a, float b, float c, float fprime, float start, float end, unsigned char image[][COLS]);
void cclear(unsigned char image[][COLS]);
int plot_logical_point(float x, float y, unsigned char image[][COLS]);
int plot_physical_point(int x, int y, unsigned char image[][COLS]);
int in_range(int x, int y);
void header(int row, int col, unsigned char head[32]);/*write header for the output images, do not modify this*/


int main(int argc, char **argv)
{
    unsigned char    image[ROWS][COLS];
    
    /*  1d*/
    cclear(image); // create image that is all black
    do_perspective(0.5, -1.0, 0.0, 0.0, 1.0, -1.0, 1.0, 0.01, 10000, image);
    write_image(image, "1d_perspective.ras");
    cclear(image);
    do_orthogonal(0.5, -1.0, 0.0, 0.0, 1.0, -1.0, 1.0, 0.01, 10000, image);
    write_image(image, "1d_orthogonal.ras");
    
    /*  2b*/
    cclear(image);
    do_orthogonal(0.5, -1, -1, 1, 1, 0, 1, 0.01, 10000, image);
    do_orthogonal(-0.5, -1, -1, 1, 1, 0, 1, 0.01, 10000, image);
    write_image(image, "2b_orthogonal_1.ras");
    cclear(image);
    do_perspective(0.5, -1, -1, 1, 1, 0, 1, 0.01, 10000, image);
    do_perspective(-0.5, -1, -1, 1, 1, 0, 1, 0.01, 10000, image);
    write_image(image, "2b_perspective_1.ras");
        
    cclear(image);
    do_orthogonal(0.5, -1, -2, 1, 1, 0, 1, 0.01, 10000, image);
    do_orthogonal(-0.5, -1, -2, 1, 1, 0, 1, 0.01, 10000, image);
    write_image(image, "2b_orthogonal_2.ras");
    cclear(image);
    do_perspective(0.5, -1, -2, 1, 1, 0, 1, 0.01, 10000, image);
    do_perspective(-0.5, -1, -2, 1, 1, 0, 1, 0.01, 10000, image);
    write_image(image, "2b_perspective_2.ras");
    
    cclear(image);
    do_orthogonal(0.5, -1, -3, 1, 1, 0, 1, 0.01, 10000, image);
    do_orthogonal(-0.5, -1, -3, 1, 1, 0, 1, 0.01, 10000, image);
    write_image(image, "2b_orthogonal_3.ras");
    cclear(image);
    do_perspective(0.5, -1, -3, 1, 1, 0, 1, 0.01, 10000, image);
    do_perspective(-0.5, -1, -3, 1, 1, 0, 1, 0.01, 10000, image);
    write_image(image, "2b_perspective_3.ras");
    
    /*  3b */
    cclear(image);
    do_orthogonal(-1, -1, 0, 0, 0, 1, 1, 0.01, 10000, image);
    do_orthogonal(1, -1, 0, 0, 0, 1, 1, 0.01, 10000, image);
    write_image(image, "3b_orthogonal_1.ras");
    cclear(image);
    do_perspective(-1, -1, 0, 0, 0, 1, 1, 0.01, 10000, image);
    do_perspective(1, -1, 0, 0, 0, 1, 1, 0.01, 10000, image);
    write_image(image, "3b_perspective_1.ras");

    cclear(image);
    do_orthogonal(-1, -1, 0, 0, 1, 1, 1, 0.01, 10000, image);
    do_orthogonal(1, -1, 0, 0, 1, 1, 1, 0.01, 10000, image);
    write_image(image, "3b_orthogonal_2.ras");
    cclear(image);
    do_perspective(-1, -1, 0, 0, 1, 1, 1, 0.01, 10000, image);
    do_perspective(1, -1, 0, 0, 1, 1, 1, 0.01, 10000, image);
    write_image(image, "3b_perspective_2.ras");

    cclear(image);
    do_orthogonal(-1, -1, 0, 0, -1, 1, 1, 0.01, 10000, image);
    do_orthogonal(1, -1, 0, 0, -1, 1, 1, 0.01, 10000, image);
    write_image(image, "3b_orthogonal_3.ras");
    cclear(image);
    do_perspective(-1, -1, 0, 0, -1, 1, 1, 0.01, 10000, image);
    do_perspective(1, -1, 0, 0, -1, 1, 1, 0.01, 10000, image);
    write_image(image, "3b_perspective_3.ras");

    cclear(image);
    do_orthogonal(-1, -1, 0, 0, 0, -1, 1, 0.01, 10000, image);
    do_orthogonal(1, -1, 0, 0, 0, 1, 1, 0.01, 10000, image);
    write_image(image, "3b_orthogonal_4.ras");
    cclear(image);
    do_perspective(-1, -1, 0, 0, 0, -1, 1, 0.01, 10000, image);
    do_perspective(1, -1, 0, 0, 0, -1, 1, 0.01, 10000, image);
    write_image(image, "3b_perspective_4.ras");

    cclear(image);
    do_orthogonal(-1, -1, 0, 0, 1, -1, 1, 0.01, 10000, image);
    do_orthogonal(1, -1, 0, 0, 1, -1, 1, 0.01, 10000, image);
    write_image(image, "3b_orthogonal_5.ras");
    cclear(image);
    do_perspective(-1, -1, 0, 0, 1, -1, 1, 0.01, 10000, image);
    do_perspective(1, -1, 0, 0, 1, -1, 1, 0.01, 10000, image);
    write_image(image, "3b_perspective_5.ras");

    cclear(image);
    do_orthogonal(-1, -1, 0, 0, -1, -1, 1, 0.01, 10000, image);
    do_orthogonal(1, -1, 0, 0, -1, -1, 1, 0.01, 10000, image);
    write_image(image, "3b_orthogonal_6.ras");
    cclear(image);
    do_perspective(-1, -1, 0, 0, -1, -1, 1, 0.01, 10000, image);
    do_perspective(1, -1, 0, 0, -1, -1, 1, 0.01, 10000, image);
    write_image(image, "3b_perspective_6.ras");


return 0;
}

void cclear(unsigned char image[][COLS])
{
    int    i, j;
    for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++) image[i][j] = 0;
}

void read_image(unsigned char image[][COLS], char filename[]){
    FILE *fp;
    
    if (!( fp = fopen( filename, "rb" )))
    {
        fprintf( stderr, "error: couldn't open %s\n", filename );
        exit( 1 );
    }

    for ( int i = 0; i < ROWS ; i++ ){
        if ( fread( image[i], 1, COLS, fp ) != COLS )
        {
            fprintf( stderr, "error: couldn't read enough stuff\n" );
            exit( 1 );
        }
    }
    fclose( fp );
}

void write_image(unsigned char image[][COLS], char filename[]){
    FILE *fp;
    unsigned char head[32];
    
    //_CRT_SECURE_NO_WARNINGS
    /* Create a header */
    header(ROWS, COLS, head);
    if (!(fp = fopen(filename, "wb"))){
        fprintf(stderr, "error: could not open %s\n", filename);
        exit(1);
    }

    /* Write the header */
    fwrite(head, 4, 8, fp);

    for (int i = 0; i < ROWS; i++) fwrite(image[i], 1, COLS, fp);
    fclose(fp);
}

int plot_logical_point(float x, float y, unsigned char image[][COLS])
{
    int    nx, ny;
    float    xc, yc;
    xc = COLS / ((float)LOGICAL_X_MAX - LOGICAL_X_MIN);
    yc = ROWS / ((float)LOGICAL_Y_MAX - LOGICAL_Y_MIN);
    nx = (x - LOGICAL_X_MIN) * xc;
    ny = (y - LOGICAL_Y_MIN) * yc;
    return plot_physical_point(nx, ny, image);
}

int plot_physical_point(int x, int y, unsigned char image[][COLS])
{
    if (in_range(x, y)) //return 0;
        return image[y][x] = 255;
    return 0;
}

int in_range(int x, int y)
{
    return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

void do_perspective(float x0, float y0, float z0, float a, float b, float c, float fprime, float start, float end, unsigned char image[][COLS]){
    float x, y, z, xprime, yprime;
    
    for (float t = start; t <= end; t += .01)  //change the t range
    {
        /*Modify the coefficiences according to s*/
        x = x0 + t*a;
        y = y0 + t*b;
        z = z0 + t*c;

        /* Modify these according to the projection */
        xprime = (fprime*x)/z;
        yprime = (fprime*y)/z;

        plot_logical_point(xprime, yprime, image);
    }
}

void do_orthogonal(float x0, float y0, float z0, float a, float b, float c, float fprime, float start, float end, unsigned char image[][COLS]){
    float x, y, z, xprime, yprime;
    
    for (float t = start; t <= end; t += .01)  //change the t range
    {
        /*Modify the coefficiences according to s*/
        x = x0 + t*a;
        y = y0 + t*b;
        z = z0 + t*c;

        /* Modify these according to the projection */
        xprime = x;
        yprime = y;

        plot_logical_point(xprime, yprime, image);
    }
}

void header(int row, int col, unsigned char head[32])
{
    int *p = (int *)head;
    char *ch;
    int num = row * col;

    /* Choose little-endian or big-endian header depending on the machine. Don't modify the contents */
    
    /* Little-endian for PC */
    *p = 0x956aa659;
    *(p + 3) = 0x08000000;
    *(p + 5) = 0x01000000;
    *(p + 6) = 0x0;
    *(p + 7) = 0xf8000000;

    ch = (char*)&col;
    head[7] = *ch;
    ch++;
    head[6] = *ch;
    ch++;
    head[5] = *ch;
    ch++;
    head[4] = *ch;

    ch = (char*)&row;
    head[11] = *ch;
    ch++;
    head[10] = *ch;
    ch++;
    head[9] = *ch;
    ch++;
    head[8] = *ch;

    ch = (char*)&num;
    head[19] = *ch;
    ch++;
    head[18] = *ch;
    ch++;
    head[17] = *ch;
    ch++;
    head[16] = *ch;

    /*
    // Big-endian for unix
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
