#ifndef __POWER_CONFIG_H__
#define __POWER_CONFIG_H__

#if !CONFIG_XIP && (CONFIG_POWER_CONFIGURATION || CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC < 160000000)

#define button_set_power 0

int power_config_set(void);
int power_config_get_and_print(void);
int power_config_set_default(void);

#define power_config_set_wrapper() power_config_set()
#define power_config_get_and_print_wrapper() power_config_get_and_print()
#define power_config_set_default_wrapper() power_config_set_default()

#else
#define power_config_set_wrapper() 0
#define power_config_get_and_print_wrapper()
#define power_config_set_default_wrapper()
#endif

#endif /* __POWER_CONFIG_H__ */
