# CLAUDE.md

Guidance for Claude Code when working in this repository.

## Project Overview

SENG440 (Embedded Systems), Summer 2026, University of Victoria, Prof. Mihai Sima.
Group project (Owen + Menoa Brennan) implementing **Color Space Conversion (CSC)**
— RGB↔YCbCr — in C, progressively optimized for ARM targets. The project runs
through multiple phases: pure C → software optimization → ARM assembly →
SIMD/NEON intrinsics → custom instruction (ASIP) design → firmware microcode
scheduling → hardware description (VHDL/Verilog). We are currently in the
software-optimization phase (loop unrolling, pipelining, cache access, NEON
vectorization).

## Repository Layout

- `CSC_global.h` — shared constants, image dimensions, fixed-point coefficients,
  global R/G/B/Y/Cb/Cr arrays.
- `CSC_main.c` — driver `main()`: reads a raw RGB file, calls
  `CSC_RGB_to_YCC()`, writes Y/Cb/Cr, calls `CSC_YCC_to_RGB()`, writes RGB back out.
- `CSC_main_02.c` — alternate/variant driver.
- `CSC_RGB_to_YCC_01.c` — RGB→YCC conversion (float brute-force + int routines,
  chrominance downsampling).
- `CSC_YCC_to_RGB_01.c` — YCC→RGB conversion (float brute-force + int routines,
  chrominance upsampling).
- Course PDFs (`SENG440_2026_Lesson_003_Course_Notes.pdf`,
  `SENG440_2026_Lesson_004_Course_Notes.pdf`, `SENG440_2026_Lesson_005_Course_Notes.pdf`,
  `SENG440_2026_Lesson_100_Course_Notes (1).pdf`,
  `Embedded_Systems_Slides_WRAPON_lesson_103.pdf`)
  — source material for citations; `Lesson 100` = AArch64 hardware/project pilot,
  `103` = CSC/GCC flags, `3` = software opt I (inlining/unrolling/pipelining),
  `4` = software opt II (aliasing/loop index/cache), `5` = software opt III
  (dead code elim/CSE/loop fusion).
- `Progress_Report_Rubric`, `Course_Objectives` — grading and syllabus reference.

## Key Constants & Data Layout

- Image size: `IMAGE_ROW_SIZE = 64`, `IMAGE_COL_SIZE = 48`, processed in 2×2 pixel blocks.
- Fixed-point shift: `K = 8`, `UNITY = 1 << K`.
- RGB→YCC coefficients (8-bit fixed point): `C11..C33` (e.g. C11=66, C12=129, C13=25 for Y).
- YCC→RGB coefficients: `D1..D5` (D1=74, D2=102, D3=52, D4=25, D5=129).
- Note: the float brute-force routines use standard-range coefficients
  (0.257/0.504/0.098 for Y, etc.), distinct from the full-range 0.299/0.587/0.114
  ITU-R BT.601 coefficients discussed in some course slides — don't conflate the two
  when writing up coefficient derivations.
- Separate `R[][]`, `G[][]`, `B[][]` uint8 arrays (not interleaved) — this is the
  starting layout; interleaved `struct { uint8_t r,g,b; }` layout is a proposed
  optimization motivated by ARM64 NEON `VLD3`.
- `RGB_to_YCC_ROUTINE` / `YCC_to_RGB_ROUTINE` macros select float (1) vs int (2) implementation.
- `CHROMINANCE_DOWNSAMPLING_MODE` / `CHROMINANCE_UPSAMPLING_MODE`: 0=zero, 1=discard/replicate, 2=average/interpolate.

## Target Platforms

- **Primary**: AArch64 / ARMv8-A, 64-bit, running in a QEMU VM (`ARM64_VM`).
  - 31 general-purpose 64-bit registers (X0–X30) — not 13.
  - NEON is **mandatory** on ARMv8-A.
  - QEMU here is a **functional emulator only** (not cycle-accurate) and executes
    on the host pipeline — it validates correctness, not ARM pipeline/timing behavior.
    Real-hardware pipeline claims should reference an actual core (e.g. Cortex-A53,
    in-order), not QEMU.
- **Secondary (earlier phases)**: ARMv5, 32-bit VM.
  - No NEON (NEON introduced at ARMv7; mandatory from ARMv8-A). Never propose
    NEON intrinsics for the ARMv5 target.
  - `USAT` (saturating instruction) is an ARMv5/AArch32 idea — it does **not**
    exist on AArch64; use `CSEL`-based predicated moves for saturation there instead.
- VMs are accessed over the UVic VPN (Cisco Secure Client, `vpn.uvic.ca`).

## Toolchain

- AArch64: `aarch64-linux-gnu-gcc`
- ARMv5: `arm-linux-gnueabihf-gcc -static -march=armv5`
- Common flags: `-O1`, `-Os`, `-S`, `-Wall`, `-Wextra`, `-g`, `-marm`, `-mthumb`
- Profiling: Valgrind / callgrind
- NEON intrinsics: use **C intrinsic names**, never raw assembly mnemonics, e.g.
  `vqmovun_s16` (signed→unsigned narrowing, correct for signed intermediate
  accumulators), `vmovl_u8`, `vmulq_n_u16`. Don't use `vqmovn_u16` unless the
  input is already unsigned.

## Writing/Report Conventions

- Progress report: 20 pages max, 11–12pt, single-spaced, IEEE-style citations
  from the course slide decks (`[1]`–`[5]` for Lessons 100/103/3/4/5). Each
  unique source gets one entry in References; reuse the same bracket marker at
  every citing location rather than duplicating entries.
- Prefer tight, condensed technical prose over verbose multi-paragraph explanations.
- Owen drafts sections himself (processor background, compiler config, loop
  unrolling, software pipelining, integer types, cache access, NEON/SIMD,
  open questions) and brings them for validation/targeted edits — not full rewrites.
- Watch for structural bugs when merging sections: duplicate section numbers
  and duplicate question numbers (e.g. two "3.5"s, two "Q5"s) have occurred before.

## Open Questions for Prof. Sima (tracked, not yet resolved)

- Scope of assembly generation required.
- Correctness validation tolerance.
- Minimum image dimensions for reliable cache profiling.
- Possible inconsistency between global shift `K=8` and the `D` coefficients
  (which appear scaled at 2^6 rather than 2^8).
- Whether both conversion directions (RGB→YCC and YCC→RGB) require full
  optimization treatment or just one.

## Later Phases (not yet started)

- ASIP custom instruction design with inline assembly.
- Firmware microcode scheduling (vertical/horizontal, 1/2/3 issue slots).
- Hardware design in VHDL/Verilog.