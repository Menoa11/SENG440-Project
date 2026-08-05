#include <stdint.h>

#define K 8 // bitwidth
#define UNITY (1 << K)

// YCC->RGB-specific shift: D1-D5 (below) are quantized at scale 2^K/4.0 =
// 2^6, not 2^8, so the largest coefficient (2.018 -> D5=129) still fits in
// an 8-bit unsigned representation. Only CSC_YCC_to_RGB_brute_force_int()
// (CSC_YCC_to_RGB_01.c) uses this; RGB->YCC keeps the global K=8.
#define K_YCC_TO_RGB 6

// Named constants for fixed-point arithmetic (Q8 format).
// Used across CSC_RGB_to_YCC and CSC_YCC_to_RGB to replace
// repeated expressions with a single named token — clearer intent,
// and guarantees the compiler folds them at compile time.
#define ROUND_K     (1 << (K-1))  // rounding constant added before >>K
#define BIAS_LUMA   (16  << K)    // Y  offset (16.0 in Q8), used in RGB->YCC
#define BIAS_CHROMA (128 << K)    // Cb/Cr offset (128.0 in Q8), used in RGB->YCC

#define IMAGE_ROW_SIZE 480
#define IMAGE_COL_SIZE 640

// Manual loop-unroll factor for the per-block loops in CSC_RGB_to_YCC()
// and CSC_YCC_to_RGB(): each unrolled iteration processes UNROLL_FACTOR
// consecutive 2x2 blocks (2*UNROLL_FACTOR columns), cutting the
// loop-control overhead (compare/increment/branch) by that factor and
// giving the compiler more independent block calls to schedule between.
#define UNROLL_FACTOR 4

#if (IMAGE_COL_SIZE % (2 * UNROLL_FACTOR)) != 0
#error "IMAGE_COL_SIZE must be a multiple of 2*UNROLL_FACTOR"
#endif

// RGB_to_YCC_ROUTINE
//     1 for CSC_RGB_to_YCC_brute_force_float()
//     2 for CSC_RGB_to_YCC_brute_force_int()       (NEON-vectorized)
//     3 for CSC_RGB_to_YCC_scalar_int()            (scalar baseline)
//     4 for CSC_RGB_to_YCC_custom_asm()            (custom CSC_Y/CB/CR;
//         does not assemble -- compile with -S only)
#define RGB_to_YCC_ROUTINE 3

// YCC_to_RGB_ROUTINE
//     1 for CSC_YCC_to_RGB_brute_force_float()
//     2 for CSC_YCC_to_RGB_brute_force_int()       (NEON-vectorized)
//     3 for CSC_YCC_to_RGB_scalar_int()            (scalar baseline)
//     4 (reserved / unused)
//     5 for CSC_YCC_to_RGB_custom_asm()            (custom RGB_R/G/B;
//         does not assemble -- compile with -S only)
#define YCC_to_RGB_ROUTINE 3

// CHROMINANCE_DOWNSAMPLING_MODE =
//     0 for returning zero (no chrominance)
//     1 for discarding three pixels and keeping one
//     2 for averaging four pixels
#define CHROMINANCE_DOWNSAMPLING_MODE 1

// CHROMINANCE_UPSAMPLING_MODE =
//     0 for returning zero (no chrominance)
//     1 for replicating one pixel into three
//     2 for interpolation with two pixels
#define CHROMINANCE_UPSAMPLING_MODE 1

// RGB-to-YCC coefficients in 8-bit representation
#define C11  66
#define C12 129
#define C13  25
#define C21  38
#define C22  74
#define C23 112
#define C31 112
#define C32  94
#define C33  18

// YCC-to-RGB coefficients in 8-bit representation
#define D1  74
#define D2 102
#define D3  52
#define D4  25
#define D5 129

/* choose between definition (GLOBAL is defined)      *
 * and declaration (GLOBAL is undefined)              *
 * GLOBAL is defined in exactly one file CSC_main.c)  */

#ifndef GLOBAL
#define EXTERN extern
#else
#define EXTERN
#endif

/* prototypes of global functions */
void CSC_RGB_to_YCC( void);
void CSC_YCC_to_RGB( void);

/* global variables */
EXTERN uint8_t R[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Red array pointer
EXTERN uint8_t G[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Green array pointer
EXTERN uint8_t B[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Blue array pointer
EXTERN uint8_t Y[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Luminance array pointer
EXTERN uint8_t Cb[IMAGE_ROW_SIZE >> 1][IMAGE_COL_SIZE >> 1]; // Chrominance (Cb) array pointer
EXTERN uint8_t Cr[IMAGE_ROW_SIZE >> 1][IMAGE_COL_SIZE >> 1]; // Chrominance (Cr) array pointer
// Cb_temp / Cr_temp arrays removed: chroma upsampling is now fused
// into the reconstruction loop (see CSC_YCC_to_RGB), eliminating the
// full-image intermediate buffers (2 * ROW * COL bytes).