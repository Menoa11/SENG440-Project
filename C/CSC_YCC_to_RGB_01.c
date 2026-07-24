// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// YCC to RGB conversion

//#include <stdio.h>
#include <stdint.h>
#include "CSC_global.h"

// private data

// private prototypes / definitions
// The routine implementations (and their saturation helpers) are
// compiled in only for the selected YCC_to_RGB_ROUTINE, so unused
// variants are completely absent from the object file
// (true dead-code elimination, not just link-time).
//
// The block workers now accept the four upsampled chroma values as
// parameters (cb_00/01/10/11 and cr_00/01/10/11) instead of reading
// them from the deleted Cb_temp/Cr_temp arrays. The upsample is
// fused into the outer CSC_YCC_to_RGB loop and computed per block.

#if YCC_to_RGB_ROUTINE == 1
// =======
static inline uint8_t saturation_float( float argument);
static void CSC_YCC_to_RGB_brute_force_float( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11);
#elif YCC_to_RGB_ROUTINE == 2
// =======
static inline uint8_t saturation_int( int argument);
static void CSC_YCC_to_RGB_brute_force_int( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11);
#endif

// =======
static inline void chrominance_upsample(
    uint8_t C_pixel_1, uint8_t C_pixel_2,
    uint8_t C_pixel_3, uint8_t C_pixel_4,
    uint8_t *top, uint8_t *left, uint8_t *middle);

// private definitions

#if YCC_to_RGB_ROUTINE == 1
// =======
static inline uint8_t saturation_float( float argument) {
  // Branchless clamp to [0.0, 255.0].
  // Ternaries let the compiler emit predicated moves (CSEL on
  // AArch64, conditional MOV on ARMv5/v7) instead of a conditional
  // branch, avoiding pipeline stalls on misprediction. Equivalent
  // to the original if/else form; output is bit-identical.
  argument = (argument > 255.0) ? 255.0 : argument;
  argument = (argument < 0.0)   ? 0.0   : argument;
  return( (uint8_t)argument);
} // END of saturation_float()

// =======
static void CSC_YCC_to_RGB_brute_force_float( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11) {
//
  float R_pixel_00, R_pixel_01, R_pixel_10, R_pixel_11;
  float G_pixel_00, G_pixel_01, G_pixel_10, G_pixel_11;
  float B_pixel_00, B_pixel_01, B_pixel_10, B_pixel_11;

  // Chroma values now come in as parameters (upsampled on the fly by
  // the caller). Layout: xx_00 = source, xx_01 = top-neighbor,
  // xx_10 = left-neighbor, xx_11 = middle (diagonal).

  R_pixel_00 =   1.164*(Y[row+0][col+0] - 16.0)
               + 1.596*(cr_00 - 128.0);
  R[row+0][col+0] = saturation_float( R_pixel_00);
//
  R_pixel_01 =   1.164*(Y[row+0][col+1] - 16.0)
               + 1.596*(cr_01 - 128.0);
  R[row+0][col+1] = saturation_float( R_pixel_01);
//
  R_pixel_10 =   1.164*(Y[row+1][col+0] - 16.0)
               + 1.596*(cr_10 - 128.0);
  R[row+1][col+0] = saturation_float( R_pixel_10);
//
  R_pixel_11 =   1.164*(Y[row+1][col+1] - 16.0)
               + 1.596*(cr_11 - 128.0);
  R[row+1][col+1] = saturation_float( R_pixel_11);

  G_pixel_00 =   1.164*(Y[row+0][col+0] - 16.0)
               - 0.813*(cr_00 - 128.0)
               - 0.391*(cb_00 - 128.0);
  G[row+0][col+0] = saturation_float( G_pixel_00);
//
  G_pixel_01 =   1.164*(Y[row+0][col+1] - 16.0)
               - 0.813*(cr_01 - 128.0)
               - 0.391*(cb_01 - 128.0);
  G[row+0][col+1] = saturation_float( G_pixel_01);
//
  G_pixel_10 =   1.164*(Y[row+1][col+0] - 16.0)
               - 0.813*(cr_10 - 128.0)
               - 0.391*(cb_10 - 128.0);
  G[row+1][col+0] = saturation_float( G_pixel_10);
//
  G_pixel_11 =   1.164*(Y[row+1][col+1] - 16.0)
               - 0.813*(cr_11 - 128.0)
               - 0.391*(cb_11 - 128.0);
  G[row+1][col+1] = saturation_float( G_pixel_11);

  B_pixel_00 =   1.164*(Y[row+0][col+0] - 16.0)
               + 2.018*(cb_00 - 128.0);
  B[row+0][col+0] = saturation_float( B_pixel_00);
//
  B_pixel_01 =   1.164*(Y[row+0][col+1] - 16.0)
               + 2.018*(cb_01 - 128.0);
  B[row+0][col+1] = saturation_float( B_pixel_01);
//
  B_pixel_10 =   1.164*(Y[row+1][col+0] - 16.0)
               + 2.018*(cb_10 - 128.0);
  B[row+1][col+0] = saturation_float( B_pixel_10);
//
  B_pixel_11 =   1.164*(Y[row+1][col+1] - 16.0)
               + 2.018*(cb_11 - 128.0);
  B[row+1][col+1] = saturation_float( B_pixel_11);
} // END of CSC_YCC_to_RGB_brute_force_float()
#endif // YCC_to_RGB_ROUTINE == 1

#if YCC_to_RGB_ROUTINE == 2
// =======
static inline uint8_t saturation_int( int argument) {
  // Branchless clamp to [0, 255].
  // Ternaries let the compiler emit predicated moves (CSEL on
  // AArch64, USAT or conditional MOV on ARMv5/v7) instead of a
  // conditional branch, avoiding pipeline stalls on misprediction.
  // In the NEON path this scalar clamp is replaced entirely by
  // SQXTUN (saturating narrow) as a free byproduct of packing.
  argument = (argument > 255) ? 255 : argument;
  argument = (argument < 0)   ? 0   : argument;
  return( (uint8_t)argument);
} // END of saturation_int()

// =======
static void CSC_YCC_to_RGB_brute_force_int( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11) {
//
  int R_pixel_00, R_pixel_01, R_pixel_10, R_pixel_11;
  int G_pixel_00, G_pixel_01, G_pixel_10, G_pixel_11;
  int B_pixel_00, B_pixel_01, B_pixel_10, B_pixel_11;

  int  Y_pixel_00,  Y_pixel_01,  Y_pixel_10,  Y_pixel_11;
  int Cb_pixel_00, Cb_pixel_01, Cb_pixel_10, Cb_pixel_11;
  int Cr_pixel_00, Cr_pixel_01, Cr_pixel_10, Cr_pixel_11;

  // Common subexpression: D1 * Y_pixel_XX is reused across
  // R, G, and B channel calculations for each of the 4 pixels.
  int luma_00, luma_01, luma_10, luma_11;

  // Chroma values now come in as parameters (upsampled on the fly by
  // the caller). Layout: xx_00 = source, xx_01 = top-neighbor,
  // xx_10 = left-neighbor, xx_11 = middle (diagonal).

  Y_pixel_00 = (int)Y[row+0][col+0];
  Y_pixel_01 = (int)Y[row+0][col+1];
  Y_pixel_10 = (int)Y[row+1][col+0];
  Y_pixel_11 = (int)Y[row+1][col+1];

  Cb_pixel_00 = (int)cb_00;
  Cb_pixel_01 = (int)cb_01;
  Cb_pixel_10 = (int)cb_10;
  Cb_pixel_11 = (int)cb_11;

  Cr_pixel_00 = (int)cr_00;
  Cr_pixel_01 = (int)cr_01;
  Cr_pixel_10 = (int)cr_10;
  Cr_pixel_11 = (int)cr_11;

  Y_pixel_00 = Y_pixel_00 - 16;
  Y_pixel_01 = Y_pixel_01 - 16;
  Y_pixel_10 = Y_pixel_10 - 16;
  Y_pixel_11 = Y_pixel_11 - 16;

  Cb_pixel_00 = Cb_pixel_00 - 128;
  Cb_pixel_01 = Cb_pixel_01 - 128;
  Cb_pixel_10 = Cb_pixel_10 - 128;
  Cb_pixel_11 = Cb_pixel_11 - 128;

  Cr_pixel_00 = Cr_pixel_00 - 128;
  Cr_pixel_01 = Cr_pixel_01 - 128;
  Cr_pixel_10 = Cr_pixel_10 - 128;
  Cr_pixel_11 = Cr_pixel_11 - 128;

  // CSE: compute the luma term (D1 * Y_pixel_XX) once per pixel and
  // reuse it across the R, G, and B channel calculations below.
  // This eliminates 8 redundant multiplications per 2x2 block
  // (originally 3 D1*Y products per pixel x 4 pixels = 12; now 4).
  luma_00 = D1 * Y_pixel_00;
  luma_01 = D1 * Y_pixel_01;
  luma_10 = D1 * Y_pixel_10;
  luma_11 = D1 * Y_pixel_11;

  R_pixel_00 = luma_00 + D2 * Cr_pixel_00;
  R_pixel_00 += ROUND_K; // rounding
  R_pixel_00 = R_pixel_00 >> K;

  R_pixel_01 = luma_01 + D2 * Cr_pixel_01;
  R_pixel_01 += ROUND_K; // rounding
  R_pixel_01 = R_pixel_01 >> K;

  R_pixel_10 = luma_10 + D2 * Cr_pixel_10;
  R_pixel_10 += ROUND_K; // rounding
  R_pixel_10 = R_pixel_10 >> K;

  R_pixel_11 = luma_11 + D2 * Cr_pixel_11;
  R_pixel_11 += ROUND_K; // rounding
  R_pixel_11 = R_pixel_11 >> K;

  R[row+0][col+0] = (uint8_t)R_pixel_00;
  R[row+0][col+1] = (uint8_t)R_pixel_01;
  R[row+1][col+0] = (uint8_t)R_pixel_10;
  R[row+1][col+1] = (uint8_t)R_pixel_11;

  G_pixel_00 = luma_00 - D3 * Cr_pixel_00
                       - D4 * Cb_pixel_00;
  G_pixel_00 += ROUND_K; // rounding
  G_pixel_00 = G_pixel_00 >> K;

  G_pixel_01 = luma_01 - D3 * Cr_pixel_01
                       - D4 * Cb_pixel_01;
  G_pixel_01 += ROUND_K; // rounding
  G_pixel_01 = G_pixel_01 >> K;

  G_pixel_10 = luma_10 - D3 * Cr_pixel_10
                       - D4 * Cb_pixel_10;
  G_pixel_10 += ROUND_K; // rounding
  G_pixel_10 = G_pixel_10 >> K;

  G_pixel_11 = luma_11 - D3 * Cr_pixel_11
                       - D4 * Cb_pixel_11;
  G_pixel_11 += ROUND_K; // rounding
  G_pixel_11 = G_pixel_11 >> K;

  G[row+0][col+0] = (uint8_t)G_pixel_00;
  G[row+0][col+1] = (uint8_t)G_pixel_01;
  G[row+1][col+0] = (uint8_t)G_pixel_10;
  G[row+1][col+1] = (uint8_t)G_pixel_11;

  B_pixel_00 = luma_00 + D5 * Cb_pixel_00;
  B_pixel_00 += ROUND_K; // rounding
  B_pixel_00 = B_pixel_00 >> K;

  B_pixel_01 = luma_01 + D5 * Cb_pixel_01;
  B_pixel_01 += ROUND_K; // rounding
  B_pixel_01 = B_pixel_01 >> K;

  B_pixel_10 = luma_10 + D5 * Cb_pixel_10;
  B_pixel_10 += ROUND_K; // rounding
  B_pixel_10 = B_pixel_10 >> K;

  B_pixel_11 = luma_11 + D5 * Cb_pixel_11;
  B_pixel_11 += ROUND_K; // rounding
  B_pixel_11 = B_pixel_11 >> K;

  B[row+0][col+0] = (uint8_t)B_pixel_00;
  B[row+0][col+1] = (uint8_t)B_pixel_01;
  B[row+1][col+0] = (uint8_t)B_pixel_10;
  B[row+1][col+1] = (uint8_t)B_pixel_11;

} // END of CSC_YCC_to_RGB_brute_force_int()
#endif // YCC_to_RGB_ROUTINE == 2

// =======
static inline void chrominance_upsample(
    uint8_t C_pixel_00, uint8_t C_pixel_01,
    uint8_t C_pixel_10, uint8_t C_pixel_11,
    uint8_t *top, uint8_t *left, uint8_t *middle) {

  int temp_top;
  int temp_left;
  int temp_middle;

  switch (CHROMINANCE_UPSAMPLING_MODE) {
    case 0:
      *top = 0;
      *left = 0;
      *middle = 0;
      break;
    case 1:
      *top = (uint8_t)C_pixel_00;
      *left = (uint8_t)C_pixel_00;
      *middle = (uint8_t)C_pixel_00;
      break;
    case 2:
      temp_top = (int)C_pixel_00 + (int)C_pixel_01;
      temp_top += (1 << 0); // rounding
      *top = (uint8_t)(temp_top >> 1);
//
      temp_left = (int)C_pixel_00 + (int)C_pixel_10;
      temp_left += (1 << 0); // rounding
      *left = (uint8_t)(temp_left >> 1);
//
      temp_middle = (int)C_pixel_00 + (int)C_pixel_01 +
                    (int)C_pixel_10 + (int)C_pixel_11;
      temp_middle += (1 << 1); // rounding
      *middle = (uint8_t)(temp_middle >> 2);
      break;
    default:
      break;
  }
} // END of chrominance_upsample()

// =======
void CSC_YCC_to_RGB( void) {
  int row, col; // indices for row and column
  int cb_row, cb_row_next;
  int cb_col, cb_col_next;

  // Chroma source pixels for the current 2x2 luma block, with
  // boundary replication at the right/bottom edges of the chroma
  // plane (matches the original chrominance_array_upsample()
  // boundary behaviour: last-row/last-col loops fed duplicated
  // neighbours to chrominance_upsample).
  uint8_t cb_src_00, cb_src_01, cb_src_10, cb_src_11;
  uint8_t cr_src_00, cr_src_01, cr_src_10, cr_src_11;

  // Upsampled chroma values for this 2x2 block.
  uint8_t cb_top, cb_left, cb_middle;
  uint8_t cr_top, cr_left, cr_middle;

  // Loop fusion: chroma upsampling is computed on the fly, per
  // block, instead of running a full-image pass into Cb_temp/Cr_temp
  // beforehand. Halves the number of passes over the chroma data,
  // eliminates two full-image (ROW x COL) temp buffers, and gives
  // better cache behaviour on the reconstruction hot path.
  //
  for( row=0; row<IMAGE_ROW_SIZE; row+=2) {
    cb_row      = row >> 1;
    cb_row_next = (cb_row + 1 < (IMAGE_ROW_SIZE >> 1)) ? cb_row + 1
                                                       : cb_row;

    for( col=0; col<IMAGE_COL_SIZE; col+=2) {
      cb_col      = col >> 1;
      cb_col_next = (cb_col + 1 < (IMAGE_COL_SIZE >> 1)) ? cb_col + 1
                                                         : cb_col;

      // Read 4 source chroma pixels (with edge replication).
      cb_src_00 = Cb[cb_row     ][cb_col     ];
      cb_src_01 = Cb[cb_row     ][cb_col_next];
      cb_src_10 = Cb[cb_row_next][cb_col     ];
      cb_src_11 = Cb[cb_row_next][cb_col_next];

      cr_src_00 = Cr[cb_row     ][cb_col     ];
      cr_src_01 = Cr[cb_row     ][cb_col_next];
      cr_src_10 = Cr[cb_row_next][cb_col     ];
      cr_src_11 = Cr[cb_row_next][cb_col_next];

      // Upsample -> {top, left, middle} for both Cb and Cr.
      chrominance_upsample( cb_src_00, cb_src_01,
                            cb_src_10, cb_src_11,
                            &cb_top, &cb_left, &cb_middle);
      chrominance_upsample( cr_src_00, cr_src_01,
                            cr_src_10, cr_src_11,
                            &cr_top, &cr_left, &cr_middle);

      // Reconstruct RGB. Chroma layout for the 2x2 block:
      //   [+0][+0] = source, [+0][+1] = top-neighbour,
      //   [+1][+0] = left,   [+1][+1] = middle (diagonal).
#if YCC_to_RGB_ROUTINE == 1
      CSC_YCC_to_RGB_brute_force_float( row, col,
          cb_src_00, cb_top, cb_left, cb_middle,
          cr_src_00, cr_top, cr_left, cr_middle);
#elif YCC_to_RGB_ROUTINE == 2
      CSC_YCC_to_RGB_brute_force_int( row, col,
          cb_src_00, cb_top, cb_left, cb_middle,
          cr_src_00, cr_top, cr_left, cr_middle);
#endif
    }
  }

} // END of CSC_YCC_to_RGB()