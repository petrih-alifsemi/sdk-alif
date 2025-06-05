#include <zephyr/logging/log.h>
#include <soc.h>

#include "power_config.h"
#include "se_service.h"

LOG_MODULE_REGISTER(power_config, CONFIG_MAIN_LOG_LEVEL);

#define DCDC_VOLTAGE_REG1 0x1a60a030
#define DCDC_VOLTAGE_REG2 0x1a60a034

union dcdc_voltage_reg1 {
	uint32_t raw;
	struct {
		uint32_t standby_mode_bypass: 1;
		uint32_t standby_mode_bypass_val: 1;
		uint32_t force_LS_on: 1;
		uint32_t dcdc_trim_vout: 6;
		uint32_t dcdc_osc2M_ibias: 4;
		uint32_t reserved: 4;
		uint32_t dcdc_en_xtal_clk: 1;
		uint32_t en_drv_dly_cont: 4;
		uint32_t en_ss_dly_cont: 4;
		uint32_t ss_done_dly_cont: 4;
		uint32_t soc_ena_0p8_bypass: 1;
		uint32_t soc_ena_0p8_bypass_val: 1;
	};
};

union dcdc_voltage_reg2 {
	uint32_t raw;
	struct {
		uint32_t trim_ss: 3;
		uint32_t trim_ZCD: 3;
		uint32_t tst_oneshot: 1;
		uint32_t oneshot_I_cont: 4;
		uint32_t phy_ldo_cont: 4;
		uint32_t trim_oneshot_cap: 4;
		uint32_t reserved: 6;
		uint32_t dcdc_xtal_div_cont: 6;
		uint32_t dcdc_xtal_div_en: 1;
	};
};

static int dcdc_reg_print(void)
{
	union dcdc_voltage_reg1 const reg1 = {
		.raw = sys_read32(DCDC_VOLTAGE_REG1),
	};
	union dcdc_voltage_reg2 const reg2 = {
		.raw = sys_read32(DCDC_VOLTAGE_REG2),
	};

	LOG_INF("DCDC REG1: %d", reg1.raw);
	LOG_INF("  soc_ena_0p8_bypass_val: %d", reg1.soc_ena_0p8_bypass_val);
	LOG_INF("  soc_ena_0p8_bypass: %d", reg1.soc_ena_0p8_bypass);
	LOG_INF("  ss_done_dly_cont: %d", reg1.ss_done_dly_cont);
	LOG_INF("  en_ss_dly_cont: %d", reg1.en_ss_dly_cont);
	LOG_INF("  en_drv_dly_cont: %d", reg1.en_drv_dly_cont);
	LOG_INF("  dcdc_en_xtal_clk: %d", reg1.dcdc_en_xtal_clk);
	LOG_INF("  reserved: %d", reg1.reserved);
	LOG_INF("  dcdc_osc2M_ibias: %d", reg1.dcdc_osc2M_ibias);
	LOG_INF("  dcdc_trim_vout: %d", reg1.dcdc_trim_vout);
	LOG_INF("  force_LS_on: %d", reg1.force_LS_on);
	LOG_INF("  standby_mode_bypass_val: %d", reg1.standby_mode_bypass_val);
	LOG_INF("  standby_mode_bypass: %d", reg1.standby_mode_bypass);

	LOG_INF("DCDC REG2: %d", reg2.raw);
	LOG_INF("  dcdc_xtal_div_en: %d", reg2.dcdc_xtal_div_en);
	LOG_INF("  dcdc_xtal_div_cont: %d", reg2.dcdc_xtal_div_cont);
	LOG_INF("  reserved: %d", reg2.reserved);
	LOG_INF("  trim_oneshot_cap: %d", reg2.trim_oneshot_cap);
	LOG_INF("  phy_ldo_cont: %d", reg2.phy_ldo_cont);
	LOG_INF("  oneshot_I_cont: %d", reg2.oneshot_I_cont);
	LOG_INF("  tst_oneshot: %d", reg2.tst_oneshot);
	LOG_INF("  trim_ZCD: %d", reg2.trim_ZCD);
	LOG_INF("  trim_ss: %d", reg2.trim_ss);

	return 0;
}

static int power_config_print(run_profile_t const *const profile)
{
	LOG_INF("-------------------------------");
	LOG_INF("power domains: 0x%x", profile->power_domains);
	if (profile->power_domains & PD_VBAT_AON_MASK) {
		LOG_INF("  PD_VBAT_AON");
	}
	if (profile->power_domains & PD_SRAM_CTRL_AON_MASK) {
		LOG_INF("  PD_SRAM_CTRL_AON");
	}
	if (profile->power_domains & PD_SSE700_AON_MASK) {
		LOG_INF("  PD_SSE700_AON");
	}
	if (profile->power_domains & PD_RTSS_HE_MASK) {
		LOG_INF("  PD_RTSS_HE");
	}
	if (profile->power_domains & PD_SRAMS_MASK) {
		LOG_INF("  PD_SRAMS");
	}
	if (profile->power_domains & PD_SESS_MASK) {
		LOG_INF("  PD_SESS");
	}
	if (profile->power_domains & PD_SYST_MASK) {
		LOG_INF("  PD_SYST");
	}
	if (profile->power_domains & PD_RTSS_HP_MASK) {
		LOG_INF("  PD_RTSS_HP");
	}
	if (profile->power_domains & PD_DBSS_MASK) {
		LOG_INF("  PD_DBSS");
	}
	if (profile->power_domains & PD2_APPS_MASK) {
		LOG_INF("  PD2_APPS");
	}

	LOG_INF("memory blocks: 0x%x", profile->memory_blocks);
	if (profile->memory_blocks & SRAM2_MASK) {
		LOG_INF("  SRAM2");
	}
	if (profile->memory_blocks & SRAM3_MASK) {
		LOG_INF("  SRAM3");
	}
	if (profile->memory_blocks & SRAM4_1_MASK) {
		LOG_INF("  SRAM4_1");
	}
	if (profile->memory_blocks & SRAM4_2_MASK) {
		LOG_INF("  SRAM4_2");
	}
	if (profile->memory_blocks & SRAM4_3_MASK) {
		LOG_INF("  SRAM4_3");
	}
	if (profile->memory_blocks & SRAM4_4_MASK) {
		LOG_INF("  SRAM4_4");
	}
	if (profile->memory_blocks & SRAM5_1_MASK) {
		LOG_INF("  SRAM5_1");
	}
	if (profile->memory_blocks & SRAM5_2_MASK) {
		LOG_INF("  SRAM5_2");
	}
	if (profile->memory_blocks & SRAM5_3_MASK) {
		LOG_INF("  SRAM5_3");
	}
	if (profile->memory_blocks & SRAM5_4_MASK) {
		LOG_INF("  SRAM5_4");
	}
	if (profile->memory_blocks & SRAM5_5_MASK) {
		LOG_INF("  SRAM5_5");
	}
	if (profile->memory_blocks & MRAM_MASK) {
		LOG_INF("  MRAM");
	}
	if (profile->memory_blocks & OSPI0_MASK) {
		LOG_INF("  OSPI0");
	}
	if (profile->memory_blocks & OSPI1_MASK) {
		LOG_INF("  OSPI1");
	}
	if (profile->memory_blocks & SERAM_1_MASK) {
		LOG_INF("  SERAM_1");
	}
	if (profile->memory_blocks & SERAM_2_MASK) {
		LOG_INF("  SERAM_2");
	}
	if (profile->memory_blocks & SERAM_3_MASK) {
		LOG_INF("  SERAM_3");
	}
	if (profile->memory_blocks & SERAM_4_MASK) {
		LOG_INF("  SERAM_4");
	}
	if (profile->memory_blocks & FWRAM_MASK) {
		LOG_INF("  FWRAM");
	}
	if (profile->memory_blocks & BACKUP4K_MASK) {
		LOG_INF("  BACKUP4K");
	}

	LOG_INF("ip clock gating: 0x%x", profile->ip_clock_gating);
	LOG_INF("phy power gating: 0x%x", profile->phy_pwr_gating);
	LOG_INF("vdd io flex: %s", profile->vdd_ioflex_3V3 == IOFLEX_LEVEL_1V8 ? "1V8" : "3V3");
	LOG_INF("dcdc_voltage %d", profile->dcdc_voltage);

	switch (profile->dcdc_mode) {
	case DCDC_MODE_OFF:
		LOG_INF("DCDC mode: OFF");
		break;
	case DCDC_MODE_PFM_AUTO:
		LOG_INF("DCDC mode: PFM_AUTO");
		break;
	case DCDC_MODE_PFM_FORCED:
		LOG_INF("DCDC mode: PFM_FORCED");
		break;
	case DCDC_MODE_PWM:
		LOG_INF("DCDC mode: PWM");
		break;
	default:
		LOG_INF("DCDC mode: unknown");
		break;
	}

	switch (profile->aon_clk_src) {
	case CLK_SRC_LFRC:
		LOG_INF("AON clock source: LFRC");
		break;
	case CLK_SRC_LFXO:
		LOG_INF("AON clock source: LFXO");
		break;
	default:
		LOG_INF("AON clock source: unknown");
		break;
	}

	switch (profile->run_clk_src) {
	case CLK_SRC_HFRC:
		LOG_INF("Run clock source: HFRC");
		break;
	case CLK_SRC_HFXO:
		LOG_INF("Run clock source: HFXO");
		break;
	case CLK_SRC_PLL:
		LOG_INF("Run clock source: PLL");
		break;
	default:
		LOG_INF("Run clock source: unknown");
		break;
	}

	static const char *cpu_clk_freq_str[] = {
		"800", "400", "300", "200",     "160",     "120",     "80",      "60",       "100",
		"50",  "20",  "10",  "RC 76.8", "RC 38.4", "XO 76.8", "XO 38.4", "DISABLED",
	};

	if (profile->cpu_clk_freq < ARRAY_SIZE(cpu_clk_freq_str)) {
		LOG_INF("CPU clock frequency: %s MHz", cpu_clk_freq_str[profile->cpu_clk_freq]);
	} else {
		LOG_INF("CPU clock frequency: unknown");
	}

	static const char *scaled_clk_freq_str[] = {
		"RC ACTV 76.8",  "RC ACTV 38.4",   "RC ACTV 19.2", "RC ACTV 9.6",   "RC ACTV 4.8",
		"RC ACTV 2.4",   "RC ACTV 1.2",    "RC ACTV 0.6",  "RC STDBY 76.8", "RC STDBY 38.4",
		"RC STDBY 19.2", "RC STDBY 9.6",   "RC STDBY 4.8", "RC STDBY 2.4",  "RC STDBY 1.2",
		"RC STDBY 0.6",  "XO LOW 38.4",    "XO LOW 19.2",  "XO LOW 9.6",    "XO LOW 4.8",
		"XO LOW 2.4",    "XO LOW 1.2",     "XO LOW 0.6",   "XO LOW 0.3",    "XO HIGH 38.4",
		"XO HIGH 19.2",  "XO HIGH 9.6",    "XO HIGH 2.4",  "XO HIGH 0.6",   "XO HIGH 0.3",
		"XO HIGH 0.15",  "XO HIGH 0.0375",
	};

	if (profile->scaled_clk_freq < ARRAY_SIZE(scaled_clk_freq_str)) {
		LOG_INF("Scaled clock frequency: %s",
			scaled_clk_freq_str[profile->scaled_clk_freq]);
	} else {
		LOG_INF("Scaled clock frequency: unknown");
	}
	LOG_INF("-------------------------------");

	return 0;
}

int power_config_get_and_print(void)
{
	int err;
	run_profile_t profile = {0};

	err = se_service_sync();
	if (err) {
		LOG_ERR("SE: not responding to service calls %d", err);
		return err;
	}

	err = se_service_get_run_cfg(&profile);
	if (err) {
		LOG_ERR("se_get_run_profile failed");
		return err;
	}

	power_config_print(&profile);

	dcdc_reg_print();

	return 0;
}

#if CONFIG_POWER_CONFIGURATION
#define TEST_SCENARIO 2
#else
#define TEST_SCENARIO 0
#endif

#if TEST_SCENARIO == 0 /*default*/
#define ACLK_REDUCED 0
#define SE_SHUTDOWN 0
#define SYSTOP_SHUTDOWN 0
#define DBG_SHUTDOWN 0
#elif TEST_SCENARIO == 1
#define ACLK_REDUCED 0
#define SE_SHUTDOWN 0
#define SYSTOP_SHUTDOWN 0
#define DBG_SHUTDOWN 1
#elif TEST_SCENARIO == 2
#define ACLK_REDUCED 0
#define SE_SHUTDOWN 0
#define SYSTOP_SHUTDOWN 1
#define DBG_SHUTDOWN 1
#elif TEST_SCENARIO == 3
#define ACLK_REDUCED 0
#define SE_SHUTDOWN 1
#define SYSTOP_SHUTDOWN 1
#define DBG_SHUTDOWN 1
#elif TEST_SCENARIO == 4
#define ACLK_REDUCED 1
#define SE_SHUTDOWN 1
#define SYSTOP_SHUTDOWN 1
#define DBG_SHUTDOWN 1
#elif TEST_SCENARIO == 5
#define ACLK_REDUCED 1
#define SE_SHUTDOWN 0
#define SYSTOP_SHUTDOWN 1
#define DBG_SHUTDOWN 1
#else
#define ACLK_REDUCED 1
#define SE_SHUTDOWN 1
#define SYSTOP_SHUTDOWN 1
#define DBG_SHUTDOWN 0
#endif



/* BLE MODEM Power and Control and BLE Radio */
#define BLE_DOMAIN (PD1_MASK | PD7_MASK)

#define SE_DOMAIN     0 // (PD_SESS_MASK)
#define DEBUG_DOMAIN  (DBG_SHUTDOWN ? 0 : PD_DBSS_MASK)
#define SYSTOP_DOMAIN (SYSTOP_SHUTDOWN ? 0 : PD_SYST_MASK)

// #define USE_DEFAULT_DOMAINS 1

void power_config_set_sys_top(bool const systop_en, bool const dbgtop_en)
{
#define BSYS_PWR_REQ_OFFSET 0x400

	uint32_t pwr_req = sys_read32(HOST_BASE_SYS_CTRL + BSYS_PWR_REQ_OFFSET);

	pwr_req &= ~((0x7 << 3) + (0x1 << 2)); /* Clear SYSTOP_PWR_REQ & DBGTOP_PWR_REQ */
	pwr_req |= ((systop_en ? 0x2 : 0) << 3);  /* Set SYSTOP_PWR_REQ */
	pwr_req |= ((dbgtop_en ? 0x1 : 0) << 2);  /* Set DBGTOP_PWR_REQ */

	sys_write32(pwr_req, HOST_BASE_SYS_CTRL + BSYS_PWR_REQ_OFFSET);
}

#if DT_PROP(DT_CHOSEN(zephyr_hci_uart), clock_frequency) == 160000000
#define HE_CORE_CLOCK CLOCK_FREQUENCY_160MHZ
#elif DT_PROP(DT_CHOSEN(zephyr_hci_uart), clock_frequency) == 120000000
#define HE_CORE_CLOCK CLOCK_FREQUENCY_120MHZ
#elif DT_PROP(DT_CHOSEN(zephyr_hci_uart), clock_frequency) == 80000000
#define HE_CORE_CLOCK CLOCK_FREQUENCY_80MHZ
#elif DT_PROP(DT_CHOSEN(zephyr_hci_uart), clock_frequency) == 60000000
#define HE_CORE_CLOCK CLOCK_FREQUENCY_60MHZ
#else
#error "Unknown clock frequency"
#endif

struct CGU_Regs {
        union {
                uint32_t val;
                struct {
                        uint32_t SYS_XTAL_SEL : 1;
                        uint32_t RESERVED1 : 3;
                        uint32_t PERIPH_XTAL_SEL : 1;
                        uint32_t RESERVED2 : 11;
                        uint32_t CLKMON_ENA: 1;
                        uint32_t RESERVED3 : 3;
                        uint32_t XTAL_DEAD : 1;
                        uint32_t RESERVED4 : 11;
                };
        } OSC_CTRL;
        union {
                uint32_t val;
                struct {
                        uint32_t PLL_LOCK : 1;
                        uint32_t RESERVED1 : 3;
                        uint32_t PLL_CALIB : 1;
                        uint32_t RESERVED2 : 27;
                };
        } PLL_LOCK_CTRL;
        union {
                uint32_t val;
                struct {
                        uint32_t SYSREF : 1;
                        uint32_t RESERVED1 : 3;
                        uint32_t SYS : 1;
                        uint32_t RESERVED2 : 15;
                        uint32_t ES1 : 1;
                        uint32_t RESERVED3 : 11;
                };
        } PLL_CLK_SEL;
        uint32_t RESERVED;
        union {
                uint32_t val;
                struct {
                        uint32_t RESERVED1 : 4;
                        uint32_t ES1_PLL : 2;
                        uint32_t RESERVED2 : 6;
                        uint32_t ES1_OSC : 2;
                        uint32_t RESERVED3 : 2;
                        uint32_t REF_DIV_VAL : 11;
                        uint32_t REF_DIV_ENA : 1;
                        uint32_t RESERVED4 : 4;

                };
        } ESCLK_SEL;
        union {
                uint32_t val;
                struct {
                        uint32_t SYSPLL : 1;
                        uint32_t RESERVED1 : 12;
                        uint32_t ES1 : 1;
                        uint32_t RESERVED2 : 4;
                        uint32_t HFXO : 1;
                        uint32_t RESERVED3 : 1;
                        uint32_t CLK160M : 1;
                        uint32_t RESERVED4 : 2;
                        uint32_t CLK38P4M : 1;
                        uint32_t RESERVED5 : 8;
                };
        } CLK_ENA;
};

volatile struct CGU_Regs *cgu_regs = (volatile struct CGU_Regs *)CGU_BASE;

volatile struct CGU_Regs cgu_regs_start;
volatile struct CGU_Regs cgu_regs_start2;
volatile struct CGU_Regs cgu_regs_stop;

int power_config_set_default(void)
{
        cgu_regs_start.OSC_CTRL.val = sys_read32(CGU_BASE + 0x0);
        cgu_regs_start.PLL_LOCK_CTRL.val = sys_read32(CGU_BASE + 0x4);
        cgu_regs_start.PLL_CLK_SEL.val = sys_read32(CGU_BASE + 0x8);
        cgu_regs_start.ESCLK_SEL.val = sys_read32(CGU_BASE + 0x10);
        cgu_regs_start.CLK_ENA.val = sys_read32(CGU_BASE + 0x14);

	int err;
	run_profile_t profile = {
		.power_domains = PD_VBAT_AON_MASK | PD_SSE700_AON_MASK | PD_RTSS_HE_MASK |
				 PD_SESS_MASK | PD_SYST_MASK | PD_DBSS_MASK,
		.dcdc_voltage = 800, /* default = 825 */
		.dcdc_mode = DCDC_MODE_PFM_FORCED,
		.aon_clk_src = CLK_SRC_LFXO,
		.run_clk_src = CLK_SRC_PLL,
		.cpu_clk_freq = HE_CORE_CLOCK,
		.scaled_clk_freq = SCALED_FREQ_XO_LOW_DIV_38_4_MHZ,
		.memory_blocks = MRAM_MASK,
		.ip_clock_gating = 0, /* deprecated */
		.phy_pwr_gating = 0,
		.vdd_ioflex_3V3 = IOFLEX_LEVEL_1V8,
	};

	err = se_service_set_run_cfg(&profile);

        /* Fix DCDC since DK has no proper trims */
	sys_write32(0x0a004411, 0x1a60a034);
	sys_write32(0x1e11e701, 0x1a60a030);

        if (err) {
		LOG_ERR("Failed to set run profile, err %d", err);
	}

        cgu_regs_start2.OSC_CTRL.val = sys_read32(CGU_BASE + 0x0);
        cgu_regs_start2.PLL_LOCK_CTRL.val = sys_read32(CGU_BASE + 0x4);
        cgu_regs_start2.PLL_CLK_SEL.val = sys_read32(CGU_BASE + 0x8);
        cgu_regs_start2.ESCLK_SEL.val = sys_read32(CGU_BASE + 0x10);
        cgu_regs_start2.CLK_ENA.val = sys_read32(CGU_BASE + 0x14);

        return err;
}

int power_config_set(void)
{
	/* Write to BSYS_PWR_REQ at 0x1a010400 first,
	 * then use the service for turning off systop & dbgtop,
	 * then the service for turning off the SE.
	 */
	//power_config_set_sys_top(true, !DBG_SHUTDOWN);

	int err;
	run_profile_t profile = {
#if USE_DEFAULT_DOMAINS
		.power_domains = PD_SYST_MASK | PD_DBSS_MASK,
#else
		.power_domains = PD_VBAT_AON_MASK | PD_SSE700_AON_MASK | PD_RTSS_HE_MASK |
				 SYSTOP_DOMAIN | DEBUG_DOMAIN | BLE_DOMAIN | SE_DOMAIN,
#endif
		.dcdc_voltage = 800, /* default = 825 */
		.dcdc_mode = DCDC_MODE_PFM_FORCED,
		.aon_clk_src = CLK_SRC_LFXO,
#if RC_CLOCK_ENABLED
		.run_clk_src = CLK_SRC_HFRC,
		.cpu_clk_freq = CLOCK_FREQUENCY_76_8_RC_MHZ,
		.scaled_clk_freq = SCALED_FREQ_RC_ACTIVE_76_8_MHZ, // divider. adjust this...
#elif HFXO_ENABLED
		.run_clk_src = CLK_SRC_HFXO,
		.cpu_clk_freq = CLOCK_FREQUENCY_76_8_XO_MHZ,
		.scaled_clk_freq = SCALED_FREQ_RC_ACTIVE_38_4_MHZ,
#else
		.run_clk_src = CLK_SRC_PLL,
		.cpu_clk_freq = HE_CORE_CLOCK,
		.scaled_clk_freq = SCALED_FREQ_XO_LOW_DIV_38_4_MHZ,
#endif
		.memory_blocks = 0,
		.ip_clock_gating = 0, /* deprecated */
		.phy_pwr_gating = 0,
		.vdd_ioflex_3V3 = IOFLEX_LEVEL_1V8,
	};

#if HE_ITCM_RETENTION
	profile.memory_blocks |= SRAM4_1_MASK | SRAM4_2_MASK | SRAM4_3_MASK | SRAM4_4_MASK;
#endif
#if HE_DTCM_RETENTION
	profile.memory_blocks |= SRAM5_1_MASK | SRAM5_2_MASK | SRAM5_3_MASK | SRAM5_4_MASK |
				 SRAM5_5_MASK | SRAM2_MASK | SRAM3_MASK;
#endif
#if SE_RAM_RETENTION
	profile.memory_blocks |= SERAM_1_MASK | SERAM_2_MASK | SERAM_3_MASK | SERAM_4_MASK;
#endif
#if MRAM_RETENTION || TEST_SCENARIO == 0
	profile.memory_blocks |= MRAM_MASK;
#endif

	err = se_service_set_run_cfg(&profile);
	if (err) {
		LOG_ERR("Failed to set run profile, err %d", err);
		return err;
	}
	// LOG_INF("Run profile set successfully");

	// power_config_print(&profile);

	// dcdc_reg_print();

#define SYST_ACLK_CTRL 0x820
#define SYST_ACLK_DIV0 0x824

#if ACLK_REDUCED
        /* SYSPLL_CLK divided by ACLK_DIV0[CLKDIV] */
	// sys_write32(0x2, HOST_BASE_SYS_CTRL + SYST_ACLK_CTRL);
	sys_write32(ACLK_REDUCED, HOST_BASE_SYS_CTRL + SYST_ACLK_DIV0);
#endif

#if SE_SHUTDOWN
	err = se_service_se_sleep_req(0 /* se_params */);
	if (err) {
		LOG_ERR("Failed to set SE to sleep, err %d", err);
		return err;
	}
	//LOG_INF("SE set to sleep successfully");
#endif

        union dcdc_voltage_reg1 reg1 = {
                .raw = 0x1e11e701,
        };

#if (TEST_SCENARIO == 3 || TEST_SCENARIO == 4) && 0
        /* adjust trim a bit to get it back on .792V */
        reg1.dcdc_trim_vout -= 1;
#endif
        /* Fix DCDC since DK has no proper trims */
	sys_write32(0x0a004411, DCDC_VOLTAGE_REG2);
	sys_write32(reg1.raw, DCDC_VOLTAGE_REG1);

	//power_config_set_sys_top(!SYSTOP_SHUTDOWN, !DBG_SHUTDOWN);

        cgu_regs_stop.OSC_CTRL.val = sys_read32(CGU_BASE + 0x0);
        cgu_regs_stop.PLL_LOCK_CTRL.val = sys_read32(CGU_BASE + 0x4);
        cgu_regs_stop.PLL_CLK_SEL.val = sys_read32(CGU_BASE + 0x8);
        cgu_regs_stop.ESCLK_SEL.val = sys_read32(CGU_BASE + 0x10);
        cgu_regs_stop.CLK_ENA.val = sys_read32(CGU_BASE + 0x14);

#if CONFIG_USE_DYNAMIC_CLOCK_ADJUSTMENT
        uint32_t reg;
#if 0
        #define ESCLK_SEL (CGU_BASE + 0x10)
        #define ESCLK_SEL_ES1_OSC_MASK (0x3 << 12)

        reg = sys_read32(ESCLK_SEL);
        reg &= ~ESCLK_SEL_ES1_OSC_MASK;
        reg |= ESCLK_SEL_ES1_OSC_MASK;
        sys_write32(reg, ESCLK_SEL);
#endif

        #define MISC_REG1 (AON_BASE + 0x30)
        #define MISC_REG1_CLKDIV (0xF << 12)
#if 0
        reg = sys_read32(MISC_REG1);
        reg &= ~MISC_REG1_CLKDIV;
        reg |= (0x3 << 12); /* divide by 8 */
        //reg |= (0x4 << 12); /* divide by 16 */
        sys_write32(reg, MISC_REG1);
#endif

        (void)reg;
#endif

	return 0;
}
