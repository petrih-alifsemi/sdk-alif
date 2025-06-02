/* Copyright (C) 2025 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 */

#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>

#define USE_DTS_PWMS 0

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#if USE_DTS_PWMS
static int set_my_pwm_cycle(const struct pwm_dt_spec *p_pwm, size_t const width)
{
	int err;

	err = width ? pwm_set_pulse_dt(p_pwm, width) : pwm_set_dt(p_pwm, 0, width);

	if (err) {
		LOG_ERR("%s: Failed to set pulse width! err %d", p_pwm->dev->name, err);
		return err;
	}
	LOG_INF("%s: Pulse width set to %d", p_pwm->dev->name, width);

	return 0;
}
#else
static int set_my_pwm_cycle(const struct device *p_pwm, size_t const channel, size_t const period,
			    size_t const width, size_t const flags)
{
	const uint32_t _period = width ? period : 0;

	//int err = pwm_set(p_pwm, channel, period, width, flags);
	int err = pwm_set_cycles(p_pwm, channel, _period, width, flags);

	if (err) {
		LOG_ERR("%s: Failed to set pulse width! err %d", p_pwm->name, err);
		return err;
	}
	LOG_INF("%s: Pulse width set to %d", p_pwm->name, width);

	return 0;
}
#endif

int main(void)
{
	size_t width, width2;
	int err = 0, err2 = 0;
	bool state = false;

#if USE_DTS_PWMS
	// const struct pwm_dt_spec pwm = PWM_DT_SPEC_GET(DT_PATH(zephyr_user));
	const struct pwm_dt_spec pwm = PWM_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), alpha);
	const struct pwm_dt_spec pwm_beta = PWM_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), beta);

	if (!pwm_is_ready_dt(&pwm)) {
		LOG_ERR("%s: device not ready", pwm.dev->name);
		return -1;
	}

	if (!pwm_is_ready_dt(&pwm_beta)) {
		LOG_ERR("%s: device not ready", pwm_beta.dev->name);
		return -1;
	}

	LOG_INF("pwm.dev: %p, pwm.channel: %d, pwm.period: %d, pwm.flags: %d", pwm.dev, pwm.channel,
		pwm.period, pwm.flags);
	LOG_INF("pwm_beta.dev: %p, pwm_beta.channel: %d, pwm_beta.period: %d, pwm_beta.flags: %d",
		pwm_beta.dev, pwm_beta.channel, pwm_beta.period, pwm_beta.flags);

	while (true) {
		width = state ? PWM_MSEC(300) : 0;
		width2 = state ? 0: PWM_MSEC(300);

		err = set_my_pwm_cycle(&pwm, width);
		err2 = set_my_pwm_cycle(&pwm_beta, width2);
		if (err || err2) {
			k_sleep(K_SECONDS(1));
			continue;
		}

		state = !state;
		k_sleep(K_SECONDS(10));
	}

#else

	const struct device * p_handle = device_get_binding("pwm4");

	const struct device *pwm_dev = DEVICE_DT_GET_OR_NULL(DT_ALIAS(test_pwm1));
	const struct device *pwm_dev_beta = DEVICE_DT_GET_OR_NULL(DT_ALIAS(test_pwm2));
	const size_t channel = 1;
	const size_t flags = PWM_POLARITY_NORMAL;
	uint64_t cycles_per_usec;

	if (!pwm_dev) {
		LOG_ERR("Devices not found");
		return -1;
	}

	if (!device_is_ready(pwm_dev)) {
		LOG_ERR("Device A not ready");
		return -1;
	}

	if (pwm_dev_beta && !device_is_ready(pwm_dev_beta)) {
		LOG_ERR("Device B not ready");
		return -1;
	}

	if (pwm_dev != p_handle) {
		LOG_ERR("Device A %p not same as handle %p!", pwm_dev, p_handle);
	}

	pwm_get_cycles_per_sec(pwm_dev, channel, &cycles_per_usec);
	cycles_per_usec /= 1000000;
	LOG_INF("Device A cycles per usec: %llu", cycles_per_usec);

	/* Set period to 1sec */
	const size_t period = 1000000 * cycles_per_usec;

	LOG_INF("Devices are ready");

	while (true) {
		width = state ? 500000 * cycles_per_usec : 0;
		width2 = state ? 0 : 300000 * cycles_per_usec;

		err = set_my_pwm_cycle(pwm_dev, channel, period, width, flags);

		if (pwm_dev_beta) {
			err2 = set_my_pwm_cycle(pwm_dev_beta, channel, period, width2, flags);
		}

		if (err || err2) {
			k_sleep(K_SECONDS(1));
			continue;
		}

		state = !state;
		k_sleep(K_SECONDS(10));
	}
#endif

	LOG_INF("Ready");

	return 0;
}
