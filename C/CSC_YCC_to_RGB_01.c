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
#elif YCC_to_RGB_ROUTINE == 3
// =======
static inline uint8_t saturation_int( int argument);
static void CSC_YCC_to_RGB_scalar_int( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11);
#elif YCC_to_RGB_ROUTINE == 5
// =======
// (routine 4 intentionally unused -- reserved to keep the inverse-direction
// custom-instruction variant numerically distinct from the forward-direction
// custom instruction, which lives at RGB_to_YCC_ROUTINE == 4. The two
// optimizations are fully independent and can be enabled in any combination.)
static void CSC_YCC_to_RGB_custom_asm( int row, int col,
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

  R_pixel_00 =   1.164*(Y[row+0][col+0] - 16.0) + 1.596*(cr_00 - 128.0);
  R[row+0][col+0] = saturation_float( R_pixel_00);
  R_pixel_01 =   1.164*(Y[row+0][col+1] - 16.0) + 1.596*(cr_01 - 128.0);
  R[row+0][col+1] = saturation_float( R_pixel_01);
  R_pixel_10 =   1.164*(Y[row+1][col+0] - 16.0) + 1.596*(cr_10 - 128.0);
  R[row+1][col+0] = saturation_float( R_pixel_10);
  R_pixel_11 =   1.164*(Y[row+1][col+1] - 16.0) + 1.596*(cr_11 - 128.0);
  R[row+1][col+1] = saturation_float( R_pixel_11);

  G_pixel_00 =   1.164*(Y[row+0][col+0] - 16.0)
               - 0.813*(cr_00 - 128.0) - 0.391*(cb_00 - 128.0);
  G[row+0][col+0] = saturation_float( G_pixel_00);
  G_pixel_01 =   1.164*(Y[row+0][col+1] - 16.0)
               - 0.813*(cr_01 - 128.0) - 0.391*(cb_01 - 128.0);
  G[row+0][col+1] = saturation_float( G_pixel_01);
  G_pixel_10 =   1.164*(Y[row+1][col+0] - 16.0)
               - 0.813*(cr_10 - 128.0) - 0.391*(cb_10 - 128.0);
  G[row+1][col+0] = saturation_float( G_pixel_10);
  G_pixel_11 =   1.164*(Y[row+1][col+1] - 16.0)
               - 0.813*(cr_11 - 128.0) - 0.391*(cb_11 - 128.0);
  G[row+1][col+1] = saturation_float( G_pixel_11);

  B_pixel_00 =   1.164*(Y[row+0][col+0] - 16.0) + 2.018*(cb_00 - 128.0);
  B[row+0][col+0] = saturation_float( B_pixel_00);
  B_pixel_01 =   1.164*(Y[row+0][col+1] - 16.0) + 2.018*(cb_01 - 128.0);
  B[row+0][col+1] = saturation_float( B_pixel_01);
  B_pixel_10 =   1.164*(Y[row+1][col+0] - 16.0) + 2.018*(cb_10 - 128.0);
  B[row+1][col+0] = saturation_float( B_pixel_10);
  B_pixel_11 =   1.164*(Y[row+1][col+1] - 16.0) + 2.018*(cb_11 - 128.0);
  B[row+1][col+1] = saturation_float( B_pixel_11);
} // END of CSC_YCC_to_RGB_brute_force_float()
#endif // YCC_to_RGB_ROUTINE == 1

#if YCC_to_RGB_ROUTINE == 2
// =======
static inline uint8_t saturation_int( int argument) {
  argument = (argument > 255) ? 255 : argument;
  argument = (argument < 0)   ? 0   : argument;
  return( (uint8_t)argument);
} // END of saturation_int()

static inline int32x4_t neon_mac( int32x4_t acc, int16x4_t v, int16_t coeff) {
  return( vmlal_n_s16( acc, v, coeff));
} // END of neon_mac()

static inline int32x4_t neon_mls( int32x4_t acc, int16x4_t v, int16_t coeff) {
  return( vmlsl_n_s16( acc, v, coeff));
} // END of neon_mls()

static inline uint16x4_t neon_finish( int32x4_t acc) {
  return( vqrshrun_n_s32( acc, K_YCC_TO_RGB));
} // END of neon_finish()

// =======
static void CSC_YCC_to_RGB_brute_force_int( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11) {
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

  int32x4_t luma = vmull_n_s16( y_pixel, D1);

  uint16x4_t R_pixel = neon_finish( neon_mac( luma, cr_pixel, D2));
  uint16x4_t G_pixel = neon_finish( neon_mls( neon_mls( luma, cr_pixel, D3),
                                               cb_pixel, D4));
  uint16x4_t B_pixel = neon_finish( neon_mac( luma, cb_pixel, D5));

  uint16_t R_out[4], G_out[4], B_out[4];
  vst1_u16( R_out, R_pixel);
  vst1_u16( G_out, G_pixel);
  vst1_u16( B_out, B_pixel);

  R[row+0][col+0] = saturation_int( R_out[0]);
  R[row+0][col+1] = saturation_int( R_out[1]);
  R[row+1][col+0] = saturation_int( R_out[2]);
  R[row+1][col+1] = saturation_int( R_out[3]);
  G[row+0][col+0] = saturation_int( G_out[0]);
  G[row+0][col+1] = saturation_int( G_out[1]);
  G[row+1][col+0] = saturation_int( G_out[2]);
  G[row+1][col+1] = saturation_int( G_out[3]);
  B[row+0][col+0] = saturation_int( B_out[0]);
  B[row+0][col+1] = saturation_int( B_out[1]);
  B[row+1][col+0] = saturation_int( B_out[2]);
  B[row+1][col+1] = saturation_int( B_out[3]);
} // END of CSC_YCC_to_RGB_brute_force_int()
#endif // YCC_to_RGB_ROUTINE == 2

#if YCC_to_RGB_ROUTINE == 3
// =======
// SCALAR baseline for the custom-instruction comparison (inverse direction).
// Pure integer, no NEON. Matches the textbook implementation from Lesson 103
// slide 28. Each pixel is one MAC chain per output, with saturation to [0,255]
// (needed for YCC->RGB per slide 27 -- the round-trip table shows R=256 and
// G=-1 values that must be clamped).
//
// Coefficients D1..D5 are quantized at scale 2^K_YCC_TO_RGB = 2^6, not 2^8;
// see comment in CSC_global.h. Rounding constant is 1 << (K_YCC_TO_RGB - 1).

static inline uint8_t saturation_int( int argument) {
  argument = (argument > 255) ? 255 : argument;
  argument = (argument < 0)   ? 0   : argument;
  return( (uint8_t)argument);
} // END of saturation_int()

#define ROUND_KY (1 << (K_YCC_TO_RGB - 1))

static inline uint8_t csc_r_scalar( uint8_t y, uint8_t cr) {
  int y_bias  = (int)y  - 16;
  int cr_bias = (int)cr - 128;
  int acc = D1 * y_bias + D2 * cr_bias;
  return saturation_int( (acc + ROUND_KY) >> K_YCC_TO_RGB);
} // END of csc_r_scalar()

static inline uint8_t csc_g_scalar( uint8_t y, uint8_t cb, uint8_t cr) {
  int y_bias  = (int)y  - 16;
  int cb_bias = (int)cb - 128;
  int cr_bias = (int)cr - 128;
  int acc = D1 * y_bias - D3 * cr_bias - D4 * cb_bias;
  return saturation_int( (acc + ROUND_KY) >> K_YCC_TO_RGB);
} // END of csc_g_scalar()

static inline uint8_t csc_b_scalar( uint8_t y, uint8_t cb) {
  int y_bias  = (int)y  - 16;
  int cb_bias = (int)cb - 128;
  int acc = D1 * y_bias + D5 * cb_bias;
  return saturation_int( (acc + ROUND_KY) >> K_YCC_TO_RGB);
} // END of csc_b_scalar()

static void CSC_YCC_to_RGB_scalar_int( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11) {
  R[row+0][col+0] = csc_r_scalar( Y[row+0][col+0], cr_00);
  R[row+0][col+1] = csc_r_scalar( Y[row+0][col+1], cr_01);
  R[row+1][col+0] = csc_r_scalar( Y[row+1][col+0], cr_10);
  R[row+1][col+1] = csc_r_scalar( Y[row+1][col+1], cr_11);

  G[row+0][col+0] = csc_g_scalar( Y[row+0][col+0], cb_00, cr_00);
  G[row+0][col+1] = csc_g_scalar( Y[row+0][col+1], cb_01, cr_01);
  G[row+1][col+0] = csc_g_scalar( Y[row+1][col+0], cb_10, cr_10);
  G[row+1][col+1] = csc_g_scalar( Y[row+1][col+1], cb_11, cr_11);

  B[row+0][col+0] = csc_b_scalar( Y[row+0][col+0], cb_00);
  B[row+0][col+1] = csc_b_scalar( Y[row+0][col+1], cb_01);
  B[row+1][col+0] = csc_b_scalar( Y[row+1][col+0], cb_10);
  B[row+1][col+1] = csc_b_scalar( Y[row+1][col+1], cb_11);
} // END of CSC_YCC_to_RGB_scalar_int()
#endif // YCC_to_RGB_ROUTINE == 3

#if YCC_to_RGB_ROUTINE == 5
// =======
// CUSTOM INSTRUCTION version for the inverse direction.
// Numbered 5 (not 4) so the inverse-direction custom instruction is
// numerically distinct from the forward-direction custom instruction
// (RGB_to_YCC_ROUTINE == 4). The two are independent optimizations --
// enable either, both, or neither by setting the respective ROUTINE flags.
// Three theoretical instructions replace the scalar MAC chains:
//   RGB_R Rd, Rn  -- sat( (D1*(Y-16) + D2*(Cr-128) + round) >> K_YCC_TO_RGB )
//   RGB_G Rd, Rn  -- sat( (D1*(Y-16) - D3*(Cr-128) - D4*(Cb-128) + round) >> K_YCC_TO_RGB )
//   RGB_B Rd, Rn  -- sat( (D1*(Y-16) + D5*(Cb-128) + round) >> K_YCC_TO_RGB )
// Input Rn is a packed 32-bit word: 0x00CrCbYy (Y in bits 0-7, Cb in
// bits 8-15, Cr in bits 16-23). Output Rd holds the 8-bit result
// (already saturated to [0,255]) in its low byte.
// Coefficients D1..D5, K_YCC_TO_RGB, biases, and saturation logic
// are hardwired inside the computing unit.
//
// Compiles with -S (producing a listing showing RGB_R / RGB_G / RGB_B)
// but does NOT assemble to an object file. Per Lesson 100 slide 27.
//
// To validate correctness of the surrounding code without the custom
// instruction, define CSC_ASM_EMULATE at compile time and the three
// inline-asm blocks fall back to the scalar C implementation.

#ifndef ROUND_KY
#define ROUND_KY (1 << (K_YCC_TO_RGB - 1))
#endif

static inline uint8_t saturation_int( int argument) {
  argument = (argument > 255) ? 255 : argument;
  argument = (argument < 0)   ? 0   : argument;
  return( (uint8_t)argument);
} // END of saturation_int()

#ifdef CSC_ASM_EMULATE
  #define RGB_R_ASM( ycc, out) \
    do { uint8_t y_ = (ycc)&0xFF, cr_ = ((ycc)>>16)&0xFF; \
         int a_ = D1 * ((int)y_ - 16) + D2 * ((int)cr_ - 128); \
         (out) = (uint32_t)saturation_int( (a_ + ROUND_KY) >> K_YCC_TO_RGB); } while(0)
  #define RGB_G_ASM( ycc, out) \
    do { uint8_t y_ = (ycc)&0xFF, cb_ = ((ycc)>>8)&0xFF, cr_ = ((ycc)>>16)&0xFF; \
         int a_ = D1 * ((int)y_ - 16) - D3 * ((int)cr_ - 128) - D4 * ((int)cb_ - 128); \
         (out) = (uint32_t)saturation_int( (a_ + ROUND_KY) >> K_YCC_TO_RGB); } while(0)
  #define RGB_B_ASM( ycc, out) \
    do { uint8_t y_ = (ycc)&0xFF, cb_ = ((ycc)>>8)&0xFF; \
         int a_ = D1 * ((int)y_ - 16) + D5 * ((int)cb_ - 128); \
         (out) = (uint32_t)saturation_int( (a_ + ROUND_KY) >> K_YCC_TO_RGB); } while(0)
#else
  #define RGB_R_ASM( ycc, out) __asm__("RGB_R %0, %1" : "=r"(out) : "r"(ycc))
  #define RGB_G_ASM( ycc, out) __asm__("RGB_G %0, %1" : "=r"(out) : "r"(ycc))
  #define RGB_B_ASM( ycc, out) __asm__("RGB_B %0, %1" : "=r"(out) : "r"(ycc))
#endif

// Pack Y, Cb, Cr into one 32-bit word: 0x00CrCbYy.
static inline uint32_t pack_ycc( uint8_t y, uint8_t cb, uint8_t cr) {
  return ((uint32_t)y) | ((uint32_t)cb << 8) | ((uint32_t)cr << 16);
} // END of pack_ycc()

static void CSC_YCC_to_RGB_custom_asm( int row, int col,
    uint8_t cb_00, uint8_t cb_01, uint8_t cb_10, uint8_t cb_11,
    uint8_t cr_00, uint8_t cr_01, uint8_t cr_10, uint8_t cr_11) {
  uint32_t ycc, r_out, g_out, b_out;

  // Pixel (0,0)
  ycc = pack_ycc( Y[row+0][col+0], cb_00, cr_00);
  RGB_R_ASM( ycc, r_out); R[row+0][col+0] = (uint8_t)r_out;
  RGB_G_ASM( ycc, g_out); G[row+0][col+0] = (uint8_t)g_out;
  RGB_B_ASM( ycc, b_out); B[row+0][col+0] = (uint8_t)b_out;

  // Pixel (0,1)
  ycc = pack_ycc( Y[row+0][col+1], cb_01, cr_01);
  RGB_R_ASM( ycc, r_out); R[row+0][col+1] = (uint8_t)r_out;
  RGB_G_ASM( ycc, g_out); G[row+0][col+1] = (uint8_t)g_out;
  RGB_B_ASM( ycc, b_out); B[row+0][col+1] = (uint8_t)b_out;

  // Pixel (1,0)
  ycc = pack_ycc( Y[row+1][col+0], cb_10, cr_10);
  RGB_R_ASM( ycc, r_out); R[row+1][col+0] = (uint8_t)r_out;
  RGB_G_ASM( ycc, g_out); G[row+1][col+0] = (uint8_t)g_out;
  RGB_B_ASM( ycc, b_out); B[row+1][col+0] = (uint8_t)b_out;

  // Pixel (1,1)
  ycc = pack_ycc( Y[row+1][col+1], cb_11, cr_11);
  RGB_R_ASM( ycc, r_out); R[row+1][col+1] = (uint8_t)r_out;
  RGB_G_ASM( ycc, g_out); G[row+1][col+1] = (uint8_t)g_out;
  RGB_B_ASM( ycc, b_out); B[row+1][col+1] = (uint8_t)b_out;
} // END of CSC_YCC_to_RGB_custom_asm()
#endif // YCC_to_RGB_ROUTINE == 5

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

static inline void CSC_YCC_to_RGB_block( int row, int col,
    const uint8_t * restrict cb_row_ptr, const uint8_t * restrict cb_row_next_ptr,
    const uint8_t * restrict cr_row_ptr, const uint8_t * restrict cr_row_next_ptr) {
  int cb_col, cb_col_next;

  uint8_t cb_src_00, cb_src_01, cb_src_10, cb_src_11;
  uint8_t cr_src_00, cr_src_01, cr_src_10, cr_src_11;

  uint8_t cb_top, cb_left, cb_middle;
  uint8_t cr_top, cr_left, cr_middle;

  cb_col      = col >> 1;
  cb_col_next = (cb_col + 1 < (IMAGE_COL_SIZE >> 1)) ? cb_col + 1
                                                     : cb_col;

  cb_src_00 = cb_row_ptr     [cb_col     ];
  cb_src_01 = cb_row_ptr     [cb_col_next];
  cb_src_10 = cb_row_next_ptr[cb_col     ];
  cb_src_11 = cb_row_next_ptr[cb_col_next];

  cr_src_00 = cr_row_ptr     [cb_col     ];
  cr_src_01 = cr_row_ptr     [cb_col_next];
  cr_src_10 = cr_row_next_ptr[cb_col     ];
  cr_src_11 = cr_row_next_ptr[cb_col_next];

  chrominance_upsample( cb_src_00, cb_src_01,
                        cb_src_10, cb_src_11,
                        &cb_top, &cb_left, &cb_middle);
  chrominance_upsample( cr_src_00, cr_src_01,
                        cr_src_10, cr_src_11,
                        &cr_top, &cr_left, &cr_middle);

#if YCC_to_RGB_ROUTINE == 1
  CSC_YCC_to_RGB_brute_force_float( row, col,
      cb_src_00, cb_top, cb_left, cb_middle,
      cr_src_00, cr_top, cr_left, cr_middle);
#elif YCC_to_RGB_ROUTINE == 2
  CSC_YCC_to_RGB_brute_force_int( row, col,
      cb_src_00, cb_top, cb_left, cb_middle,
      cr_src_00, cr_top, cr_left, cr_middle);
#elif YCC_to_RGB_ROUTINE == 3
  CSC_YCC_to_RGB_scalar_int( row, col,
      cb_src_00, cb_top, cb_left, cb_middle,
      cr_src_00, cr_top, cr_left, cr_middle);
#elif YCC_to_RGB_ROUTINE == 5
  CSC_YCC_to_RGB_custom_asm( row, col,
      cb_src_00, cb_top, cb_left, cb_middle,
      cr_src_00, cr_top, cr_left, cr_middle);
#endif
} // END of CSC_YCC_to_RGB_block()

// =======
void CSC_YCC_to_RGB( void) {
  int row, col; // indices for row and column
  int cb_row, cb_row_next;

  for( row=0; row<IMAGE_ROW_SIZE; row+=2) {
    cb_row      = row >> 1;
    cb_row_next = (cb_row + 1 < (IMAGE_ROW_SIZE >> 1)) ? cb_row + 1
                                                       : cb_row;

    const uint8_t * restrict cb_row_ptr      = &Cb[cb_row     ][0];
    const uint8_t * restrict cb_row_next_ptr = &Cb[cb_row_next][0];
    const uint8_t * restrict cr_row_ptr      = &Cr[cb_row     ][0];
    const uint8_t * restrict cr_row_next_ptr = &Cr[cb_row_next][0];

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