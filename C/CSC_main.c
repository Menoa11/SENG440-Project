// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// main() function

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include <string.h>
#include <math.h>

#define GLOBAL
#include "CSC_global.h"

int main( void) {
  unsigned int row, col;
  FILE *f_ID_input_RGB;
  FILE *f_ID_echo_R;
  FILE *f_ID_echo_G;
  FILE *f_ID_echo_B;
  FILE *f_ID_output_Y;
  FILE *f_ID_output_Cb;
  FILE *f_ID_output_Cr;
  FILE *f_ID_output_RGB;

  //f_ID_input_RGB = fopen( "./image_input_RGB_64_48_03.data", "rb");
  f_ID_input_RGB = fopen( "./image_input_RGB_640_480_02.data", "rb");

  if( f_ID_input_RGB == NULL) {
    printf( "Cannot open file.\n");
    return( 1);
  }

  //f_ID_echo_R = fopen( "./image_echo_R_64_48_03.data", "wb");
  f_ID_echo_R = fopen( "./image_echo_R_640_480_02.data", "wb");
  if( f_ID_echo_R == NULL) {
    printf( "Cannot open file.\n");
    return( 1);
  }

  //f_ID_echo_G = fopen( "./image_echo_G_64_48_03.data", "wb");
  f_ID_echo_G = fopen( "./image_echo_G_640_480_02.data", "wb");
  if( f_ID_echo_G == NULL) {
    printf( "Cannot open file.\n");
    return( 1);
  }

  //f_ID_echo_B = fopen( "./image_echo_B_64_48_03.data", "wb");
  f_ID_echo_B = fopen( "./image_echo_B_640_480_02.data", "wb");
  if( f_ID_echo_B == NULL) {
    printf( "Cannot open file.\n");
    return( 1);
  }

  // Loop unrolled by UNROLL_FACTOR: each iteration processes
  // UNROLL_FACTOR consecutive columns instead of one, cutting the
  // col compare+increment+branch overhead by that factor. IMAGE_COL_SIZE
  // is a multiple of UNROLL_FACTOR (enforced in CSC_global.h), so no
  // remainder loop is needed.
  for( row=0; row < IMAGE_ROW_SIZE; row++)
  for( col=0; col < IMAGE_COL_SIZE; col+=UNROLL_FACTOR) {
    R[row][col+0] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( R[row][col+0], f_ID_echo_R);
    G[row][col+0] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( G[row][col+0], f_ID_echo_G);
    B[row][col+0] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( B[row][col+0], f_ID_echo_B);

    R[row][col+1] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( R[row][col+1], f_ID_echo_R);
    G[row][col+1] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( G[row][col+1], f_ID_echo_G);
    B[row][col+1] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( B[row][col+1], f_ID_echo_B);

    R[row][col+2] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( R[row][col+2], f_ID_echo_R);
    G[row][col+2] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( G[row][col+2], f_ID_echo_G);
    B[row][col+2] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( B[row][col+2], f_ID_echo_B);

    R[row][col+3] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( R[row][col+3], f_ID_echo_R);
    G[row][col+3] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( G[row][col+3], f_ID_echo_G);
    B[row][col+3] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( B[row][col+3], f_ID_echo_B);
  }
  fclose( f_ID_echo_B);
  fclose( f_ID_echo_G);
  fclose( f_ID_echo_R);
  fclose( f_ID_input_RGB);

  CSC_RGB_to_YCC();

  //f_ID_output_Y = fopen( "./image_output_Y_64_48_03.data", "wb");
  f_ID_output_Y = fopen( "./image_output_Y_640_480_02.data", "wb");
  if( f_ID_output_Y == NULL) {
    fprintf( stderr, "Could not open %s\n", 
             "./image_output_Y_640_480_02.data");
    return( 1);
  }
  
  //f_ID_output_Cb = fopen( "./image_output_Cb_64_48_03.data", "wb");
  f_ID_output_Cb = fopen( "./image_output_Cb_640_480_02.data", "wb");
  if( f_ID_output_Cb == NULL) {
    fprintf( stderr, "Could not open %s\n", 
             "./image_output_Cb_640_480_02.data");
    return( 1);
  }
  
  //f_ID_output_Cr = fopen( "./image_output_Cr_64_48_03.data", "wb");
  f_ID_output_Cr = fopen( "./image_output_Cr_640_480_02.data", "wb");
  if( f_ID_output_Cr == NULL) {
    fprintf( stderr, "Could not open %s\n", 
             "./image_output_Cr_640_480_02.data");
    return( 1);
  }
  
  for( row=0; row < IMAGE_ROW_SIZE; row++)
  for( col=0; col < IMAGE_COL_SIZE; col+=UNROLL_FACTOR) {
    //fprintf( f_ID_output_Y, "%02hhx", Y[row][col]);
    fputc( Y[row][col+0], f_ID_output_Y);
    fputc( Y[row][col+1], f_ID_output_Y);
    fputc( Y[row][col+2], f_ID_output_Y);
    fputc( Y[row][col+3], f_ID_output_Y);
  }

  // (IMAGE_COL_SIZE >> 1) is a multiple of UNROLL_FACTOR too, since
  // IMAGE_COL_SIZE is a multiple of 2*UNROLL_FACTOR (CSC_global.h) --
  // no remainder loop needed here either.
  for( row=0; row < (IMAGE_ROW_SIZE >> 1); row++)
  for( col=0; col < (IMAGE_COL_SIZE >> 1); col+=UNROLL_FACTOR) {
    //fprintf( f_ID_output_Cb, "%02hhx", Cb[row][col]);
    //fprintf( f_ID_output_Cr, "%02hhx", Cr[row][col]);
    fputc( Cb[row][col+0], f_ID_output_Cb);
    fputc( Cr[row][col+0], f_ID_output_Cr);
    fputc( Cb[row][col+1], f_ID_output_Cb);
    fputc( Cr[row][col+1], f_ID_output_Cr);
    fputc( Cb[row][col+2], f_ID_output_Cb);
    fputc( Cr[row][col+2], f_ID_output_Cr);
    fputc( Cb[row][col+3], f_ID_output_Cb);
    fputc( Cr[row][col+3], f_ID_output_Cr);
  }

  fclose( f_ID_output_Cr);
  fclose( f_ID_output_Cb);
  fclose( f_ID_output_Y);

  CSC_YCC_to_RGB();

  //f_ID_output_RGB = fopen( "./image_output_RGB_64_48_03.data", "wb");
  f_ID_output_RGB = fopen( "./image_output_RGB_640_480_02.data", "wb");
  if( f_ID_output_RGB == NULL) {
    printf( "Cannot open file.\n");
    return( 1);
  }

  for( row=0; row < IMAGE_ROW_SIZE; row++)
  for( col=0; col < IMAGE_COL_SIZE; col+=UNROLL_FACTOR) {
    fputc( R[row][col+0], f_ID_output_RGB);
    fputc( G[row][col+0], f_ID_output_RGB);
    fputc( B[row][col+0], f_ID_output_RGB);

    fputc( R[row][col+1], f_ID_output_RGB);
    fputc( G[row][col+1], f_ID_output_RGB);
    fputc( B[row][col+1], f_ID_output_RGB);

    fputc( R[row][col+2], f_ID_output_RGB);
    fputc( G[row][col+2], f_ID_output_RGB);
    fputc( B[row][col+2], f_ID_output_RGB);

    fputc( R[row][col+3], f_ID_output_RGB);
    fputc( G[row][col+3], f_ID_output_RGB);
    fputc( B[row][col+3], f_ID_output_RGB);
  }
  fclose( f_ID_output_RGB);

} // END of main()

