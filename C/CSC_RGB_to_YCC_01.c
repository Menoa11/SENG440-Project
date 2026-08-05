// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// RGB to YCC conversion

//#include <stdio.h>
#include <stdint.h>
#include "CSC_global.h"

#if RGB_to_YCC_ROUTINE == 2
#include <arm_neon.h>
#endif

// private data

// private prototypes / definitions
// The routine implementations are compiled in only for the selected
// RGB_to_YCC_ROUTINE, so unused variants are completely absent from
// the object file (true dead-code elimination, not just link-time).

#if RGB_to_YCC_ROUTINE == 1
// =======
static void CSC_RGB_to_YCC_brute_force_float( unsigned int row, unsigned int col);
#elif RGB_to_YCC_ROUTINE == 2
// =======
static void CSC_RGB_to_YCC_brute_force_int( unsigned int row, unsigned int col);
#elif RGB_to_YCC_ROUTINE == 3
// =======
static void CSC_RGB_to_YCC_scalar_int( unsigned int row, unsigned int col);
#elif RGB_to_YCC_ROUTINE == 4
// =======
static void CSC_RGB_to_YCC_custom_asm( unsigned int row, unsigned int col);
#endif

// =======
static inline uint8_t chrominance_downsample(
    uint8_t C_pixel_1, uint8_t C_pixel_2,
    uint8_t C_pixel_3, uint8_t C_pixel_4);

// private definitions

#if RGB_to_YCC_ROUTINE == 1
// =======
static void CSC_RGB_to_YCC_brute_force_float( unsigned int row, unsigned int col) {
//
  uint8_t Cb_pixel_00, Cb_pixel_01;
  uint8_t Cb_pixel_10, Cb_pixel_11;
  uint8_t Cr_pixel_00, Cr_pixel_01;
  uint8_t Cr_pixel_10, Cr_pixel_11;

  Y[row+0][col+0] = (uint8_t)(16.0 + 0.257*R[row+0][col+0]
                                   + 0.504*G[row+0][col+0]
                                   + 0.098*B[row+0][col+0]);
  Y[row+0][col+1] = (uint8_t)(16.0 + 0.257*R[row+0][col+1]
                                   + 0.504*G[row+0][col+1]
                                   + 0.098*B[row+0][col+1]);
  Y[row+1][col+0] = (uint8_t)(16.0 + 0.257*R[row+1][col+0]
                                   + 0.504*G[row+1][col+0]
                                   + 0.098*B[row+1][col+0]);
  Y[row+1][col+1] = (uint8_t)(16.0 + 0.257*R[row+1][col+1]
                                   + 0.504*G[row+1][col+1]
                                   + 0.098*B[row+1][col+1]);

  Cb_pixel_00 = (uint8_t)(128.0 - 0.148*R[row+0][col+0]
                                - 0.291*G[row+0][col+0]
                                + 0.439*B[row+0][col+0]);
  Cb_pixel_01 = (uint8_t)(128.0 - 0.148*R[row+0][col+1]
                                - 0.291*G[row+0][col+1]
                                + 0.439*B[row+0][col+1]);
  Cb_pixel_10 = (uint8_t)(128.0 - 0.148*R[row+1][col+0]
                                - 0.291*G[row+1][col+0]
                                + 0.439*B[row+1][col+0]);
  Cb_pixel_11 = (uint8_t)(128.0 - 0.148*R[row+1][col+1]
                                - 0.291*G[row+1][col+1]
                                + 0.439*B[row+1][col+1]);

  Cr_pixel_00 = (uint8_t)(128.0 + 0.439*R[row+0][col+0]
                                - 0.368*G[row+0][col+0]
                                - 0.071*B[row+0][col+0]);
  Cr_pixel_01 = (uint8_t)(128.0 + 0.439*R[row+0][col+1]
                                - 0.368*G[row+0][col+1]
                                - 0.071*B[row+0][col+1]);
  Cr_pixel_10 = (uint8_t)(128.0 + 0.439*R[row+1][col+0]
                                - 0.368*G[row+1][col+0]
                                - 0.071*B[row+1][col+0]);
  Cr_pixel_11 = (uint8_t)(128.0 + 0.439*R[row+1][col+1]
                                - 0.368*G[row+1][col+1]
                                - 0.071*B[row+1][col+1]);

  Cb[row>>1][col>>1] = chrominance_downsample( Cb_pixel_00,
                                               Cb_pixel_01,
                                               Cb_pixel_10,
                                               Cb_pixel_11);

  Cr[row>>1][col>>1] = chrominance_downsample( Cr_pixel_00,
                                               Cr_pixel_01,
                                               Cr_pixel_10,
                                               Cr_pixel_11);
} // END of CSC_RGB_to_YCC_brute_force_float()
#endif // RGB_to_YCC_ROUTINE == 1

#if RGB_to_YCC_ROUTINE == 2
// NEON helpers for CSC_RGB_to_YCC_brute_force_int() below. Each takes
// and returns a single named vector value (never an array of vector
// types looped over), so GCC keeps every intermediate in a register
// -- same convention as the YCC_to_RGB NEON helpers.
static inline int32x4_t neon_mac( int32x4_t acc, int16x4_t v, int16_t coeff) {
  return( vmlal_n_s16( acc, v, coeff));
} // END of neon_mac()

static inline int32x4_t neon_mls( int32x4_t acc, int16x4_t v, int16_t coeff) {
  return( vmlsl_n_s16( acc, v, coeff));
} // END of neon_mls()

// Rounding-shift-right by K folded with a plain truncating narrow --
// vrshrn_n_s32 does the "+= ROUND_K; >>= K" rounding step and the
// narrow to 16-bit in one instruction. Not saturating, matching the
// (uint8_t) cast this routine has always used (no clamp).
static inline int16x4_t neon_finish( int32x4_t acc) {
  return( vrshrn_n_s32( acc, K));
} // END of neon_finish()

// =======
static void CSC_RGB_to_YCC_brute_force_int( unsigned int row, unsigned int col) {
//
  // Lane order for every vector below is [00, 01, 10, 11], i.e. the
  // 4 pixels of this one 2x2 block -- same layout/order as the
  // pre-NEON xx_pixel_00..xx_pixel_11 scalars.

  const int16_t r_in[4] = { (int16_t)R[row+0][col+0], (int16_t)R[row+0][col+1],
                             (int16_t)R[row+1][col+0], (int16_t)R[row+1][col+1] };
  const int16_t g_in[4] = { (int16_t)G[row+0][col+0], (int16_t)G[row+0][col+1],
                             (int16_t)G[row+1][col+0], (int16_t)G[row+1][col+1] };
  const int16_t b_in[4] = { (int16_t)B[row+0][col+0], (int16_t)B[row+0][col+1],
                             (int16_t)B[row+1][col+0], (int16_t)B[row+1][col+1] };

  int16x4_t r_pixel = vld1_s16( r_in);
  int16x4_t g_pixel = vld1_s16( g_in);
  int16x4_t b_pixel = vld1_s16( b_in);

  // Y   = BIAS_LUMA   + C11*R + C12*G + C13*B
  int32x4_t y_acc = vdupq_n_s32( BIAS_LUMA);
  y_acc = neon_mac( y_acc, r_pixel, C11);
  y_acc = neon_mac( y_acc, g_pixel, C12);
  y_acc = neon_mac( y_acc, b_pixel, C13);
  int16x4_t y_pixel = neon_finish( y_acc);

  // Cb  = BIAS_CHROMA - C21*R - C22*G + C23*B
  int32x4_t cb_acc = vdupq_n_s32( BIAS_CHROMA);
  cb_acc = neon_mls( cb_acc, r_pixel, C21);
  cb_acc = neon_mls( cb_acc, g_pixel, C22);
  cb_acc = neon_mac( cb_acc, b_pixel, C23);
  int16x4_t cb_pixel = neon_finish( cb_acc);

  // Cr  = BIAS_CHROMA + C31*R - C32*G - C33*B
  int32x4_t cr_acc = vdupq_n_s32( BIAS_CHROMA);
  cr_acc = neon_mac( cr_acc, r_pixel, C31);
  cr_acc = neon_mls( cr_acc, g_pixel, C32);
  cr_acc = neon_mls( cr_acc, b_pixel, C33);
  int16x4_t cr_pixel = neon_finish( cr_acc);

  int16_t y_out[4], cb_out[4], cr_out[4];
  vst1_s16( y_out,  y_pixel);
  vst1_s16( cb_out, cb_pixel);
  vst1_s16( cr_out, cr_pixel);

  Y[row+0][col+0] = (uint8_t)y_out[0];
  Y[row+0][col+1] = (uint8_t)y_out[1];
  Y[row+1][col+0] = (uint8_t)y_out[2];
  Y[row+1][col+1] = (uint8_t)y_out[3];

  Cb[row>>1][col>>1] = chrominance_downsample( (uint8_t)cb_out[0],
                                               (uint8_t)cb_out[1],
                                               (uint8_t)cb_out[2],
                                               (uint8_t)cb_out[3]);

  Cr[row>>1][col>>1] = chrominance_downsample( (uint8_t)cr_out[0],
                                               (uint8_t)cr_out[1],
                                               (uint8_t)cr_out[2],
                                               (uint8_t)cr_out[3]);
} // END of CSC_RGB_to_YCC_brute_force_int()
#endif // RGB_to_YCC_ROUTINE == 2

#if RGB_to_YCC_ROUTINE == 3
// =======
// SCALAR baseline for the custom-instruction comparison.
// Pure integer, no NEON. Matches the textbook implementation described
// in Lesson 103 slide 22. Each pixel is one MAC chain per output.
// This is the "before" version -- profile it, count its instructions,
// then compare against routine 4 which replaces the MAC chain with the
// custom CSC_Y / CSC_CB / CSC_CR instructions.

static inline uint8_t csc_y_scalar( uint8_t r, uint8_t g, uint8_t b) {
  int acc = BIAS_LUMA
          + C11 * (int)r
          + C12 * (int)g
          + C13 * (int)b;
  return (uint8_t)((acc + ROUND_K) >> K);
} // END of csc_y_scalar()

static inline uint8_t csc_cb_scalar( uint8_t r, uint8_t g, uint8_t b) {
  int acc = BIAS_CHROMA
          - C21 * (int)r
          - C22 * (int)g
          + C23 * (int)b;
  return (uint8_t)((acc + ROUND_K) >> K);
} // END of csc_cb_scalar()

static inline uint8_t csc_cr_scalar( uint8_t r, uint8_t g, uint8_t b) {
  int acc = BIAS_CHROMA
          + C31 * (int)r
          - C32 * (int)g
          - C33 * (int)b;
  return (uint8_t)((acc + ROUND_K) >> K);
} // END of csc_cr_scalar()

static void CSC_RGB_to_YCC_scalar_int( unsigned int row, unsigned int col) {
  uint8_t cb00, cb01, cb10, cb11;
  uint8_t cr00, cr01, cr10, cr11;

  // 4 pixels of the 2x2 block
  Y [row+0][col+0] = csc_y_scalar ( R[row+0][col+0], G[row+0][col+0], B[row+0][col+0]);
  Y [row+0][col+1] = csc_y_scalar ( R[row+0][col+1], G[row+0][col+1], B[row+0][col+1]);
  Y [row+1][col+0] = csc_y_scalar ( R[row+1][col+0], G[row+1][col+0], B[row+1][col+0]);
  Y [row+1][col+1] = csc_y_scalar ( R[row+1][col+1], G[row+1][col+1], B[row+1][col+1]);

  cb00 = csc_cb_scalar( R[row+0][col+0], G[row+0][col+0], B[row+0][col+0]);
  cb01 = csc_cb_scalar( R[row+0][col+1], G[row+0][col+1], B[row+0][col+1]);
  cb10 = csc_cb_scalar( R[row+1][col+0], G[row+1][col+0], B[row+1][col+0]);
  cb11 = csc_cb_scalar( R[row+1][col+1], G[row+1][col+1], B[row+1][col+1]);

  cr00 = csc_cr_scalar( R[row+0][col+0], G[row+0][col+0], B[row+0][col+0]);
  cr01 = csc_cr_scalar( R[row+0][col+1], G[row+0][col+1], B[row+0][col+1]);
  cr10 = csc_cr_scalar( R[row+1][col+0], G[row+1][col+0], B[row+1][col+0]);
  cr11 = csc_cr_scalar( R[row+1][col+1], G[row+1][col+1], B[row+1][col+1]);

  Cb[row>>1][col>>1] = chrominance_downsample( cb00, cb01, cb10, cb11);
  Cr[row>>1][col>>1] = chrominance_downsample( cr00, cr01, cr10, cr11);
} // END of CSC_RGB_to_YCC_scalar_int()
#endif // RGB_to_YCC_ROUTINE == 3

#if RGB_to_YCC_ROUTINE == 4
// =======
// CUSTOM INSTRUCTION version (the "after" for the ASIP comparison).
// Uses inline assembly to instantiate three theoretical instructions:
//   CSC_Y  Rd, Rn   -- (BIAS_LUMA   + C11*R + C12*G + C13*B + round) >> K
//   CSC_CB Rd, Rn   -- (BIAS_CHROMA - C21*R - C22*G + C23*B + round) >> K
//   CSC_CR Rd, Rn   -- (BIAS_CHROMA + C31*R - C32*G - C33*B + round) >> K
// Input Rn is a packed 32-bit word: 0x00BBGGRR (R in bits 0-7, G in
// bits 8-15, B in bits 16-23). Output Rd holds the 8-bit result in
// its low byte. Coefficients C11..C33, K, ROUND_K, BIAS_LUMA, and
// BIAS_CHROMA are hardwired inside the computing unit.
//
// The assembler does not know these opcodes, so this compiles with
// `-S` (producing a listing showing CSC_Y / CSC_CB / CSC_CR) but does
// NOT assemble to an object file. That is per Lesson 100 slide 27 --
// intended and expected behaviour.
//
// To validate correctness of the surrounding code without the
// custom instruction, define CSC_ASM_EMULATE at compile time and the
// three inline-asm blocks fall back to the scalar C implementation.

#ifdef CSC_ASM_EMULATE
  #define CSC_Y_ASM( rgb, out) \
    do { uint8_t r_ = (rgb)&0xFF, g_ = ((rgb)>>8)&0xFF, b_ = ((rgb)>>16)&0xFF; \
         int a_ = BIAS_LUMA + C11*(int)r_ + C12*(int)g_ + C13*(int)b_; \
         (out) = (uint32_t)((uint8_t)((a_ + ROUND_K) >> K)); } while(0)
  #define CSC_CB_ASM( rgb, out) \
    do { uint8_t r_ = (rgb)&0xFF, g_ = ((rgb)>>8)&0xFF, b_ = ((rgb)>>16)&0xFF; \
         int a_ = BIAS_CHROMA - C21*(int)r_ - C22*(int)g_ + C23*(int)b_; \
         (out) = (uint32_t)((uint8_t)((a_ + ROUND_K) >> K)); } while(0)
  #define CSC_CR_ASM( rgb, out) \
    do { uint8_t r_ = (rgb)&0xFF, g_ = ((rgb)>>8)&0xFF, b_ = ((rgb)>>16)&0xFF; \
         int a_ = BIAS_CHROMA + C31*(int)r_ - C32*(int)g_ - C33*(int)b_; \
         (out) = (uint32_t)((uint8_t)((a_ + ROUND_K) >> K)); } while(0)
#else
  #define CSC_Y_ASM( rgb, out)  __asm__("CSC_Y  %0, %1" : "=r"(out) : "r"(rgb))
  #define CSC_CB_ASM( rgb, out) __asm__("CSC_CB %0, %1" : "=r"(out) : "r"(rgb))
  #define CSC_CR_ASM( rgb, out) __asm__("CSC_CR %0, %1" : "=r"(out) : "r"(rgb))
#endif

// Pack R, G, B into one 32-bit word: 0x00BBGGRR.
// This is the "packing overhead" required by ARM's 2-in/1-out
// operand limit (Lesson 103 slide 34).
static inline uint32_t pack_rgb( uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r) | ((uint32_t)g << 8) | ((uint32_t)b << 16);
} // END of pack_rgb()

static void CSC_RGB_to_YCC_custom_asm( unsigned int row, unsigned int col) {
  uint32_t rgb, y_out, cb_out, cr_out;
  uint8_t cb00, cb01, cb10, cb11;
  uint8_t cr00, cr01, cr10, cr11;

  // Pixel (0,0)
  rgb = pack_rgb( R[row+0][col+0], G[row+0][col+0], B[row+0][col+0]);
  CSC_Y_ASM ( rgb, y_out);  Y [row+0][col+0] = (uint8_t)y_out;
  CSC_CB_ASM( rgb, cb_out); cb00 = (uint8_t)cb_out;
  CSC_CR_ASM( rgb, cr_out); cr00 = (uint8_t)cr_out;

  // Pixel (0,1)
  rgb = pack_rgb( R[row+0][col+1], G[row+0][col+1], B[row+0][col+1]);
  CSC_Y_ASM ( rgb, y_out);  Y [row+0][col+1] = (uint8_t)y_out;
  CSC_CB_ASM( rgb, cb_out); cb01 = (uint8_t)cb_out;
  CSC_CR_ASM( rgb, cr_out); cr01 = (uint8_t)cr_out;

  // Pixel (1,0)
  rgb = pack_rgb( R[row+1][col+0], G[row+1][col+0], B[row+1][col+0]);
  CSC_Y_ASM ( rgb, y_out);  Y [row+1][col+0] = (uint8_t)y_out;
  CSC_CB_ASM( rgb, cb_out); cb10 = (uint8_t)cb_out;
  CSC_CR_ASM( rgb, cr_out); cr10 = (uint8_t)cr_out;

  // Pixel (1,1)
  rgb = pack_rgb( R[row+1][col+1], G[row+1][col+1], B[row+1][col+1]);
  CSC_Y_ASM ( rgb, y_out);  Y [row+1][col+1] = (uint8_t)y_out;
  CSC_CB_ASM( rgb, cb_out); cb11 = (uint8_t)cb_out;
  CSC_CR_ASM( rgb, cr_out); cr11 = (uint8_t)cr_out;

  Cb[row>>1][col>>1] = chrominance_downsample( cb00, cb01, cb10, cb11);
  Cr[row>>1][col>>1] = chrominance_downsample( cr00, cr01, cr10, cr11);
} // END of CSC_RGB_to_YCC_custom_asm()
#endif // RGB_to_YCC_ROUTINE == 4

// =======
static inline uint8_t chrominance_downsample(
    uint8_t C_pixel_00, uint8_t C_pixel_01,
    uint8_t C_pixel_10, uint8_t C_pixel_11) {

  int temp;

  switch (CHROMINANCE_DOWNSAMPLING_MODE) {
    case 0:
      return( 0);
    case 1:
      return( C_pixel_00);
    case 2:
      temp = (int)C_pixel_00 + (int)C_pixel_01 +
             (int)C_pixel_10 + (int)C_pixel_11;
      temp += (1 << 1); // rounding
      temp = temp >> 2;
      return( (uint8_t)temp);
    default:
      return( 0);
  }
} // END of chrominance_downsample()

// =======
void CSC_RGB_to_YCC( void) {
  unsigned int row, col; // indices for row and column
//
  for( row=0; row<IMAGE_ROW_SIZE; row+=2) {
    // Loop unrolled by UNROLL_FACTOR: each iteration processes
    // UNROLL_FACTOR consecutive 2x2 blocks instead of one, so the
    // col/=2-step compare+increment+branch runs 1/UNROLL_FACTOR as
    // often. IMAGE_COL_SIZE is checked at compile time (CSC_global.h)
    // to be a multiple of 2*UNROLL_FACTOR, so no remainder loop is
    // needed.
    for( col=0; col<IMAGE_COL_SIZE; col+=2*UNROLL_FACTOR) {
      //printf( "\n[row,col] = [%02i,%02i]\n\n", row, col);
      // Preprocessor dispatch on the compile-time RGB_to_YCC_ROUTINE.
      // No runtime switch, no branch per iteration; the unselected
      // call site is not emitted at all.
#if RGB_to_YCC_ROUTINE == 1
      CSC_RGB_to_YCC_brute_force_float( row, col+0);
      CSC_RGB_to_YCC_brute_force_float( row, col+2);
      CSC_RGB_to_YCC_brute_force_float( row, col+4);
      CSC_RGB_to_YCC_brute_force_float( row, col+6);
#elif RGB_to_YCC_ROUTINE == 2
      CSC_RGB_to_YCC_brute_force_int( row, col+0);
      CSC_RGB_to_YCC_brute_force_int( row, col+2);
      CSC_RGB_to_YCC_brute_force_int( row, col+4);
      CSC_RGB_to_YCC_brute_force_int( row, col+6);
#elif RGB_to_YCC_ROUTINE == 3
      CSC_RGB_to_YCC_scalar_int( row, col+0);
      CSC_RGB_to_YCC_scalar_int( row, col+2);
      CSC_RGB_to_YCC_scalar_int( row, col+4);
      CSC_RGB_to_YCC_scalar_int( row, col+6);
#elif RGB_to_YCC_ROUTINE == 4
      CSC_RGB_to_YCC_custom_asm( row, col+0);
      CSC_RGB_to_YCC_custom_asm( row, col+2);
      CSC_RGB_to_YCC_custom_asm( row, col+4);
      CSC_RGB_to_YCC_custom_asm( row, col+6);
#endif
    }
  }

} // END of CSC_RGB_to_YCC()