// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// YCC to RGB conversion

//#include <stdio.h>
#include <stdint.h>
#include "CSC_global.h"

#if YCC_to_RGB_ROUTINE == 2
#include <arm_neon.h>
#endif

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
  // Branchless clamp to [0, 255]. Not called from the NEON path below
  // (that path never clamped -- see neon_finish()'s note); kept here
  // unused, same as it was before vectorization.
  argument = (argument > 255) ? 255 : argument;
  argument = (argument < 0)   ? 0   : argument;
  return( (uint8_t)argument);
} // END of saturation_int()

// NEON helpers for CSC_YCC_to_RGB_brute_force_int() below. Each takes
// and returns a single named vector value (never an array of vector
// types looped over), so GCC keeps every intermediate in a register.
static inline int32x4_t neon_mac( int32x4_t acc, int16x4_t v, int16_t coeff) {
  return( vmlal_n_s16( acc, v, coeff));
} // END of neon_mac()

static inline int32x4_t neon_mls( int32x4_t acc, int16x4_t v, int16_t coeff) {
  return( vmlsl_n_s16( acc, v, coeff));
} // END of neon_mls()

// Rounding-shift-right by K folded with a plain truncating narrow --
// vrshrn_n_s32 does the "+= ROUND_K; >>= K" rounding step and the
// narrow to 16-bit in one instruction. Not saturating, same as the
// (uint8_t) cast this routine has always used (no clamp).
static inline int16x4_t neon_finish( int32x4_t acc) {
  return( vrshrn_n_s32( acc, K));
} // END of neon_finish()

// =======
static void CSC_YCC_to_RGB_brute_force_int( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11) {
//
  // Chroma values come in as parameters (upsampled on the fly by the
  // caller). Layout: xx_00 = source, xx_01 = top-neighbor,
  // xx_10 = left-neighbor, xx_11 = middle (diagonal) -- same as
  // before vectorization. Lane order for every vector below is
  // [00, 01, 10, 11], i.e. the 4 pixels of this one 2x2 block.

  const int16_t y_bias[4]  = { (int16_t)Y[row+0][col+0] - 16,
                                (int16_t)Y[row+0][col+1] - 16,
                                (int16_t)Y[row+1][col+0] - 16,
                                (int16_t)Y[row+1][col+1] - 16 };
  const int16_t cb_bias[4] = { (int16_t)cb_00 - 128, (int16_t)cb_01 - 128,
                                (int16_t)cb_10 - 128, (int16_t)cb_11 - 128 };
  const int16_t cr_bias[4] = { (int16_t)cr_00 - 128, (int16_t)cr_01 - 128,
                                (int16_t)cr_10 - 128, (int16_t)cr_11 - 128 };

  int16x4_t y_pixel  = vld1_s16( y_bias);
  int16x4_t cb_pixel = vld1_s16( cb_bias);
  int16x4_t cr_pixel = vld1_s16( cr_bias);

  // CSE, vectorized: D1*Y computed once and reused across R, G, and B
  // -- one vector multiply covers all 4 pixels of the block instead
  // of the 4 separate scalar multiplies the pre-NEON luma_00..luma_11
  // terms used.
  int32x4_t luma = vmull_n_s16( y_pixel, D1);

  int16x4_t R_pixel = neon_finish( neon_mac( luma, cr_pixel, D2));
  int16x4_t G_pixel = neon_finish( neon_mls( neon_mls( luma, cr_pixel, D3),
                                              cb_pixel, D4));
  int16x4_t B_pixel = neon_finish( neon_mac( luma, cb_pixel, D5));

  int16_t R_out[4], G_out[4], B_out[4];
  vst1_s16( R_out, R_pixel);
  vst1_s16( G_out, G_pixel);
  vst1_s16( B_out, B_pixel);

  R[row+0][col+0] = (uint8_t)R_out[0];
  R[row+0][col+1] = (uint8_t)R_out[1];
  R[row+1][col+0] = (uint8_t)R_out[2];
  R[row+1][col+1] = (uint8_t)R_out[3];

  G[row+0][col+0] = (uint8_t)G_out[0];
  G[row+0][col+1] = (uint8_t)G_out[1];
  G[row+1][col+0] = (uint8_t)G_out[2];
  G[row+1][col+1] = (uint8_t)G_out[3];

  B[row+0][col+0] = (uint8_t)B_out[0];
  B[row+0][col+1] = (uint8_t)B_out[1];
  B[row+1][col+0] = (uint8_t)B_out[2];
  B[row+1][col+1] = (uint8_t)B_out[3];

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

// One-block worker: reads the 4 source chroma pixels for a single
// 2x2 luma block (with edge replication via the caller's row
// pointers), upsamples them, and calls the reconstruction routine.
// Factored out of the col loop so CSC_YCC_to_RGB()'s unrolling below
// can call it several times per iteration without duplicating this
// logic at each unrolled offset.
static inline void CSC_YCC_to_RGB_block( int row, int col,
    const uint8_t * restrict cb_row_ptr, const uint8_t * restrict cb_row_next_ptr,
    const uint8_t * restrict cr_row_ptr, const uint8_t * restrict cr_row_next_ptr) {
  int cb_col, cb_col_next;

  // Chroma source pixels for this 2x2 luma block, with boundary
  // replication at the right/bottom edges of the chroma plane
  // (matches the original chrominance_array_upsample() boundary
  // behaviour: last-row/last-col loops fed duplicated neighbours to
  // chrominance_upsample).
  uint8_t cb_src_00, cb_src_01, cb_src_10, cb_src_11;
  uint8_t cr_src_00, cr_src_01, cr_src_10, cr_src_11;

  // Upsampled chroma values for this 2x2 block.
  uint8_t cb_top, cb_left, cb_middle;
  uint8_t cr_top, cr_left, cr_middle;

  cb_col      = col >> 1;
  cb_col_next = (cb_col + 1 < (IMAGE_COL_SIZE >> 1)) ? cb_col + 1
                                                     : cb_col;

  // Read 4 source chroma pixels (with edge replication) via the
  // per-row pointer aliases passed in by the caller.
  cb_src_00 = cb_row_ptr     [cb_col     ];
  cb_src_01 = cb_row_ptr     [cb_col_next];
  cb_src_10 = cb_row_next_ptr[cb_col     ];
  cb_src_11 = cb_row_next_ptr[cb_col_next];

  cr_src_00 = cr_row_ptr     [cb_col     ];
  cr_src_01 = cr_row_ptr     [cb_col_next];
  cr_src_10 = cr_row_next_ptr[cb_col     ];
  cr_src_11 = cr_row_next_ptr[cb_col_next];

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
} // END of CSC_YCC_to_RGB_block()

// =======
void CSC_YCC_to_RGB( void) {
  int row, col; // indices for row and column
  int cb_row, cb_row_next;

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

    // Local pointer aliases: hoist the per-row base address out of
    // the inner column loop so `Cb[cb_row][cb_col]` doesn't recompute
    // `&Cb[0][0] + cb_row * (IMAGE_COL_SIZE >> 1)` on every access.
    // `restrict` tells GCC that reads/writes through these pointers
    // do not alias with any other restrict-qualified pointer, which
    // lets it reorder loads and schedule the upsample+reconstruction
    // more aggressively (Cb and Cr are distinct globals in
    // CSC_global.h; `Cb` is not written during CSC_YCC_to_RGB, so
    // even the edge case where cb_row_next == cb_row is safe).
    const uint8_t * restrict cb_row_ptr      = &Cb[cb_row     ][0];
    const uint8_t * restrict cb_row_next_ptr = &Cb[cb_row_next][0];
    const uint8_t * restrict cr_row_ptr      = &Cr[cb_row     ][0];
    const uint8_t * restrict cr_row_next_ptr = &Cr[cb_row_next][0];

    // Loop unrolled by UNROLL_FACTOR: each iteration processes
    // UNROLL_FACTOR consecutive 2x2 blocks instead of one, so the
    // col/=2-step compare+increment+branch runs 1/UNROLL_FACTOR as
    // often. IMAGE_COL_SIZE is checked at compile time (CSC_global.h)
    // to be a multiple of 2*UNROLL_FACTOR, so no remainder loop is
    // needed.
    for( col=0; col<IMAGE_COL_SIZE; col+=2*UNROLL_FACTOR) {
      CSC_YCC_to_RGB_block( row, col+0,
          cb_row_ptr, cb_row_next_ptr, cr_row_ptr, cr_row_next_ptr);
      CSC_YCC_to_RGB_block( row, col+2,
          cb_row_ptr, cb_row_next_ptr, cr_row_ptr, cr_row_next_ptr);
      CSC_YCC_to_RGB_block( row, col+4,
          cb_row_ptr, cb_row_next_ptr, cr_row_ptr, cr_row_next_ptr);
      CSC_YCC_to_RGB_block( row, col+6,
          cb_row_ptr, cb_row_next_ptr, cr_row_ptr, cr_row_next_ptr);
    }
  }

} // END of CSC_YCC_to_RGB()