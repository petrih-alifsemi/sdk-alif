/* Copyright (C) Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 */

#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <stdio.h>
#include "gapm.h"
#include "../src/main.h"

LOG_MODULE_REGISTER(test_shell, LOG_LEVEL_ERR);

static const char *param_get_str(size_t const argc, char **argv, char *p_param,
				 const char *p_def_value)
{
	if (p_param && argc > 1) {
		for (int n = 0; n < (argc - 1); n++) {
			if (strcmp(argv[n], p_param) == 0) {
				return argv[n + 1];
			}
		}
	}
	return p_def_value;
}

static int32_t param_get_int(size_t const argc, char **argv, char *p_param, int const def_value)
{
	const char *p_value = param_get_str(argc, argv, p_param, NULL);

	if (p_value) {
		return strtol(p_value, NULL, 0);
	}

	return def_value;
}

static int cmd_status(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	gap_bdaddr_t identity;
	char addr_str[18];

	gapm_get_identity(&identity);

	snprintf(addr_str, sizeof(addr_str), "%02X:%02X:%02X:%02X:%02X:%02X", identity.addr[5],
		 identity.addr[4], identity.addr[3], identity.addr[2], identity.addr[1],
		 identity.addr[0]);

	shell_fprintf(shell, SHELL_VT100_COLOR_YELLOW, "Current config:\n");
	shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "  Device Name: ");
	shell_fprintf(shell, SHELL_VT100_COLOR_DEFAULT, CONFIG_BLE_DEVICE_NAME "\n");
	shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "  Device Addr: ");
	shell_fprintf(shell, SHELL_VT100_COLOR_DEFAULT, "%s\n", addr_str);
	shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "  Bonding supported: ");
	shell_fprintf(shell, SHELL_VT100_COLOR_DEFAULT,
		      IS_ENABLED(CONFIG_BONDING_ALLOWED) ? "Yes\n" : "No\n");

	return 0;
}

static int cmd_start_scan(const struct shell *shell, size_t const argc, char **argv)
{
	ARG_UNUSED(shell);
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	return scan_start();
}

static int cmd_start_stream(const struct shell *shell, size_t const argc, char **argv)
{
	ARG_UNUSED(shell);
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	return stream_start();
}

static int cmd_stop_stream(const struct shell *shell, size_t const argc, char **argv)
{
	ARG_UNUSED(shell);
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	return stream_stop();
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_cfg,
	SHELL_CMD_ARG(status, NULL, "Print current status", cmd_status, 1, 0),
	SHELL_CMD_ARG(scan, NULL, "Start scanning", cmd_start_scan, 1, 0),
	SHELL_CMD_ARG(start, NULL, "Start streaming", cmd_start_stream, 1, 0),
	SHELL_CMD_ARG(stop, NULL, "Stop streaming", cmd_stop_stream, 1, 0),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);

SHELL_CMD_REGISTER(unicast, &sub_cfg, "LE Audio Unicast Source Control", NULL);
