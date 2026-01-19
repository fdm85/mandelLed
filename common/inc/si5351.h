/*
 * si5351.h - Si5351 library for Arduino
 *
 * Copyright (C) 2015 - 2019 Jason Milldrum <milldrum@gmail.com>
 *                           Dana H. Myers <k6jq@comcast.net>
 *
 * Many defines derived from clk-si5351.h in the Linux kernel.
 * Sebastian Hesselbarth <sebastian.hesselbarth@gmail.com>
 * Rabeeh Khoury <rabeeh@solid-run.com>
 *
 * do_div() macro derived from /include/asm-generic/div64.h in
 * the Linux kernel.
 * Copyright (C) 2003 Bernardo Innocenti <bernie@develer.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SI5351_H_
#define SI5351_H_

#include <stdint.h>
#include <stdbool.h>
/* Define definitions */

#define SI5351_BUS_BASE_ADDR            0x60u
#define SI5351_XTAL_FREQ                25000000u
#define SI5351_PLL_FIXED                80000000000ULL
#define SI5351_FREQ_MULT                100ULL
#define SI5351_DEFAULT_CLK              1000000000ULL

#define SI5351_PLL_VCO_MIN              600000000u
#define SI5351_PLL_VCO_MAX              900000000u
#define SI5351_MULTISYNTH_MIN_FREQ      500000u
#define SI5351_MULTISYNTH_DIVBY4_FREQ   150000000u
#define SI5351_MULTISYNTH_MAX_FREQ      225000000u
#define SI5351_MULTISYNTH_SHARE_MAX     100000000u
#define SI5351_MULTISYNTH_SHARE_MIN     1024000u
#define SI5351_MULTISYNTH67_MAX_FREQ    SI5351_MULTISYNTH_DIVBY4_FREQ
#define SI5351_CLKOUT_MIN_FREQ          4000u
#define SI5351_CLKOUT_MAX_FREQ          SI5351_MULTISYNTH_MAX_FREQ
#define SI5351_CLKOUT67_MS_MIN          SI5351_PLL_VCO_MIN / SI5351_MULTISYNTH67_A_MAX
#define SI5351_CLKOUT67_MIN_FREQ        SI5351_CLKOUT67_MS_MIN / 128
#define SI5351_CLKOUT67_MAX_FREQ        SI5351_MULTISYNTH67_MAX_FREQ

#define SI5351_PLL_A_MIN                15u
#define SI5351_PLL_A_MAX                90u
#define SI5351_PLL_B_MAX                (SI5351_PLL_C_MAX-1)
#define SI5351_PLL_C_MAX                1048575u
#define SI5351_MULTISYNTH_A_MIN         6u
#define SI5351_MULTISYNTH_A_MAX         1800u
#define SI5351_MULTISYNTH67_A_MAX       254u
#define SI5351_MULTISYNTH_B_MAX         (SI5351_MULTISYNTH_C_MAX-1)
#define SI5351_MULTISYNTH_C_MAX         1048575
#define SI5351_MULTISYNTH_P1_MAX        ((1u<<18)-1)
#define SI5351_MULTISYNTH_P2_MAX        ((1u<<20)-1)
#define SI5351_MULTISYNTH_P3_MAX        ((1u<<20)-1)
#define SI5351_VCXO_PULL_MIN            30u
#define SI5351_VCXO_PULL_MAX            240u
#define SI5351_VCXO_MARGIN              103u

#define SI5351_DEVICE_STATUS            0u
#define SI5351_INTERRUPT_STATUS         1u
#define SI5351_INTERRUPT_MASK           2u
#define SI5351_STATUS_SYS_INIT          (1u<<7u)
#define SI5351_STATUS_LOL_B             (1u<<6u)
#define SI5351_STATUS_LOL_A             (1u<<5u)
#define SI5351_STATUS_LOS               (1u<<4u)
#define SI5351_OUTPUT_ENABLE_CTRL       3u
#define SI5351_OEB_PIN_ENABLE_CTRL      9u
#define SI5351_PLL_INPUT_SOURCE         15u
#define SI5351_CLKIN_DIV_MASK           (3u<<6u)
#define SI5351_CLKIN_DIV_1              (0u<<6u)
#define SI5351_CLKIN_DIV_2              (1u<<6u)
#define SI5351_CLKIN_DIV_4              (2u<<6u)
#define SI5351_CLKIN_DIV_8              (3u<<6u)
#define SI5351_PLLB_SOURCE              (1u<<3u)
#define SI5351_PLLA_SOURCE              (1u<<2u)

#define SI5351_CLK0_CTRL                16u
#define SI5351_CLK1_CTRL                17u
#define SI5351_CLK2_CTRL                18u
#define SI5351_CLK3_CTRL                19u
#define SI5351_CLK4_CTRL                20u
#define SI5351_CLK5_CTRL                21u
#define SI5351_CLK6_CTRL                22u
#define SI5351_CLK7_CTRL                23u
#define SI5351_CLK_POWERDOWN            (1u<<7u)
#define SI5351_CLK_INTEGER_MODE         (1u<<6u)
#define SI5351_CLK_PLL_SELECT           (1u<<5u)
#define SI5351_CLK_INVERT               (1u<<4u)
#define SI5351_CLK_INPUT_MASK           (3u<<2u)
#define SI5351_CLK_INPUT_XTAL           (0u<<2u)
#define SI5351_CLK_INPUT_CLKIN          (1u<<2u)
#define SI5351_CLK_INPUT_MULTISYNTH_0_4 (2u<<2u)
#define SI5351_CLK_INPUT_MULTISYNTH_N   (3u<<2u)
#define SI5351_CLK_DRIVE_STRENGTH_MASK  (3u<<0u)
#define SI5351_CLK_DRIVE_STRENGTH_2MA   (0u<<0u)
#define SI5351_CLK_DRIVE_STRENGTH_4MA   (1u<<0u)
#define SI5351_CLK_DRIVE_STRENGTH_6MA   (2u<<0u)
#define SI5351_CLK_DRIVE_STRENGTH_8MA   (3u<<0u)

#define SI5351_CLK3_0_DISABLE_STATE     24u
#define SI5351_CLK7_4_DISABLE_STATE     25u
#define SI5351_CLK_DISABLE_STATE_MASK   3u
#define SI5351_CLK_DISABLE_STATE_LOW    0u
#define SI5351_CLK_DISABLE_STATE_HIGH   1u
#define SI5351_CLK_DISABLE_STATE_FLOAT  2u
#define SI5351_CLK_DISABLE_STATE_NEVER  3u

#define SI5351_PARAMETERS_LENGTH        8u
#define SI5351_PLLA_PARAMETERS          26u
#define SI5351_PLLB_PARAMETERS          34u
#define SI5351_CLK0_PARAMETERS          42u
#define SI5351_CLK1_PARAMETERS          50u
#define SI5351_CLK2_PARAMETERS          58u
#define SI5351_CLK3_PARAMETERS          66u
#define SI5351_CLK4_PARAMETERS          74u
#define SI5351_CLK5_PARAMETERS          82u
#define SI5351_CLK6_PARAMETERS          90u
#define SI5351_CLK7_PARAMETERS          91u
#define SI5351_CLK6_7_OUTPUT_DIVIDER    92u
#define SI5351_OUTPUT_CLK_DIV_MASK      (7u << 4u)
#define SI5351_OUTPUT_CLK6_DIV_MASK     (7u << 0u)
#define SI5351_OUTPUT_CLK_DIV_SHIFT     4u
#define SI5351_OUTPUT_CLK_DIV6_SHIFT    0u
#define SI5351_OUTPUT_CLK_DIV_1         0u
#define SI5351_OUTPUT_CLK_DIV_2         1u
#define SI5351_OUTPUT_CLK_DIV_4         2u
#define SI5351_OUTPUT_CLK_DIV_8         3u
#define SI5351_OUTPUT_CLK_DIV_16        4u
#define SI5351_OUTPUT_CLK_DIV_32        5u
#define SI5351_OUTPUT_CLK_DIV_64        6u
#define SI5351_OUTPUT_CLK_DIV_128       7u
#define SI5351_OUTPUT_CLK_DIVBY4       (3u<<2u)

#define SI5351_SSC_PARAM0               149u
#define SI5351_SSC_PARAM1               150u
#define SI5351_SSC_PARAM2               151u
#define SI5351_SSC_PARAM3               152u
#define SI5351_SSC_PARAM4               153u
#define SI5351_SSC_PARAM5               154u
#define SI5351_SSC_PARAM6               155u
#define SI5351_SSC_PARAM7               156u
#define SI5351_SSC_PARAM8               157u
#define SI5351_SSC_PARAM9               158u
#define SI5351_SSC_PARAM10              159u
#define SI5351_SSC_PARAM11              160u
#define SI5351_SSC_PARAM12              161u

#define SI5351_VXCO_PARAMETERS_LOW      162u
#define SI5351_VXCO_PARAMETERS_MID      163u
#define SI5351_VXCO_PARAMETERS_HIGH     164u

#define SI5351_CLK0_PHASE_OFFSET        165u
#define SI5351_CLK1_PHASE_OFFSET        166u
#define SI5351_CLK2_PHASE_OFFSET        167u
#define SI5351_CLK3_PHASE_OFFSET        168u
#define SI5351_CLK4_PHASE_OFFSET        169u
#define SI5351_CLK5_PHASE_OFFSET        170u

#define SI5351_PLL_RESET                177u
#define SI5351_PLL_RESET_B              (1u<<7u)
#define SI5351_PLL_RESET_A              (1u<<5u)

#define SI5351_CRYSTAL_LOAD             183u
#define SI5351_CRYSTAL_LOAD_MASK        (3u<<6u)
#define SI5351_CRYSTAL_LOAD_0PF         (0u<<6u)
#define SI5351_CRYSTAL_LOAD_6PF         (1u<<6u)
#define SI5351_CRYSTAL_LOAD_8PF         (2u<<6u)
#define SI5351_CRYSTAL_LOAD_10PF        (3u<<6u)

#define SI5351_FANOUT_ENABLE            187u
#define SI5351_CLKIN_ENABLE             (1u<<7u)
#define SI5351_XTAL_ENABLE              (1u<<6u)
#define SI5351_MULTISYNTH_ENABLE        (1u<<4u)


/* Macro definitions */

//#define RFRAC_DENOM ((1L << 20) - 1)
#define RFRAC_DENOM 1000000ULL

/*
 * Based on former asm-ppc/div64.h and asm-m68knommu/div64.h
 *
 * The semantics of do_div() are:
 *
 * uint32_t do_div(uint64_t *n, uint32_t base)
 * {
 *      uint32_t remainder = *n % base;
 *      *n = *n / base;
 *      return remainder;
 * }
 *
 * NOTE: macro parameter n is evaluated multiple times,
 *       beware of side effects!
 */

# define do_div(n,base) ({                                      \
        uint64_t __base = (base);                               \
        uint64_t __rem;                                         \
        __rem = ((uint64_t)(n)) % __base;                       \
        (n) = ((uint64_t)(n)) / __base;                         \
        __rem;                                                  \
 })

/* Enum definitions */

/*
 * enum si5351_variant - SiLabs Si5351 chip variant
 * @SI5351_VARIANT_A: Si5351A (8 output clocks, XTAL input)
 * @SI5351_VARIANT_A3: Si5351A MSOP10 (3 output clocks, XTAL input)
 * @SI5351_VARIANT_B: Si5351B (8 output clocks, XTAL/VXCO input)
 * @SI5351_VARIANT_C: Si5351C (8 output clocks, XTAL/CLKIN input)
 */
/*
enum si5351_variant {
  SI5351_VARIANT_A = 1,
  SI5351_VARIANT_A3 = 2,
  SI5351_VARIANT_B = 3,
  SI5351_VARIANT_C = 4,
};
*/

enum si5351_clock {SI5351_CLK0, SI5351_CLK1, SI5351_CLK2, SI5351_CLK3,
  SI5351_CLK4, SI5351_CLK5, SI5351_CLK6, SI5351_CLK7};

enum si5351_pll {SI5351_PLLA, SI5351_PLLB};

enum si5351_drive {SI5351_DRIVE_2MA, SI5351_DRIVE_4MA, SI5351_DRIVE_6MA, SI5351_DRIVE_8MA};

enum si5351_clock_source {SI5351_CLK_SRC_XTAL, SI5351_CLK_SRC_CLKIN, SI5351_CLK_SRC_MS0, SI5351_CLK_SRC_MS};

enum si5351_clock_disable {SI5351_CLK_DISABLE_LOW, SI5351_CLK_DISABLE_HIGH, SI5351_CLK_DISABLE_HI_Z, SI5351_CLK_DISABLE_NEVER};

enum si5351_clock_fanout {SI5351_FANOUT_CLKIN, SI5351_FANOUT_XO, SI5351_FANOUT_MS};

enum si5351_pll_input {SI5351_PLL_INPUT_XO, SI5351_PLL_INPUT_CLKIN};

/* Struct definitions */

struct Si5351RegSet
{
  uint32_t p1;
  uint32_t p2;
  uint32_t p3;
};

struct Si5351Status
{
  uint8_t SYS_INIT;
  uint8_t LOL_B;
  uint8_t LOL_A;
  uint8_t LOS;
  uint8_t REVID;
};

struct Si5351IntStatus
{
  uint8_t SYS_INIT_STKY;
  uint8_t LOL_B_STKY;
  uint8_t LOL_A_STKY;
  uint8_t LOS_STKY;
};

  bool Si5351_init(uint8_t xtal_load_c, uint32_t xo_freq, uint32_t corr);
  void Si5351_reset(void);
  uint8_t Si5351_set_freq(uint64_t freq, enum si5351_clock clk);
  uint8_t Si5351_set_freq_manual(uint64_t freq, uint64_t pll_freq, enum si5351_clock clk);
  void Si5351_set_pll(uint64_t pll_freq, enum si5351_pll target_pll);
  void Si5351_set_ms(enum si5351_clock clk, struct Si5351RegSet ms_reg, uint8_t int_mode, uint8_t r_div, uint8_t div_by_4);
  void Si5351_output_enable(enum si5351_clock clk, uint8_t enable);
  void Si5351_drive_strength(enum si5351_clock clk, enum si5351_drive drive);
  void Si5351_update_status(void);
  void Si5351_set_correction(int32_t corr, enum si5351_pll_input ref_osc);
  void Si5351_set_phase(enum si5351_clock clk, uint8_t phase);
  int32_t Si5351_get_correction(enum si5351_pll_input ref_osc);
  void Si5351_pll_reset(enum si5351_pll target_pll);
  void Si5351_set_ms_source(enum si5351_clock clk, enum si5351_pll pll);
  void Si5351_set_int(enum si5351_clock clk, uint8_t enable);
  void Si5351_set_clock_pwr(enum si5351_clock clk, uint8_t pwr);
  void Si5351_set_clock_invert(enum si5351_clock clk, uint8_t pwr);
  void Si5351_set_clock_source(enum si5351_clock clk, enum si5351_clock_source src);
  void Si5351_set_clock_disable(enum si5351_clock clk, enum si5351_clock_disable dis_state);
  void Si5351_set_clock_fanout(enum si5351_clock_fanout fanout, uint8_t enable);
  void Si5351_set_pll_input(enum si5351_pll pll, enum si5351_pll_input input);
  void Si5351_set_vcxo(uint64_t pll_freq, uint8_t ppm);
  void Si5351_set_ref_freq(uint32_t ref_freq, enum si5351_pll_input ref_osc);
  uint8_t Si5351_write_bulk(uint8_t addr, uint8_t bytes, uint8_t *data);
  uint8_t Si5351_write(uint8_t addr, uint8_t data);
  uint8_t Si5351_read(uint8_t addr);

#endif /* SI5351_H_ */
