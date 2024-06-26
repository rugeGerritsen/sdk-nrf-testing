/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <zephyr.h>
#include <logging/log.h>
#include <dfu/mcuboot.h>
#include <dfu/dfu_target.h>

#define DEF_DFU_TARGET(name) \
static const struct dfu_target dfu_target_ ## name  = { \
	.init = dfu_target_ ## name ## _init, \
	.offset_get = dfu_target_## name ##_offset_get, \
	.write = dfu_target_ ## name ## _write, \
	.done = dfu_target_ ## name ## _done, \
	.schedule_update = dfu_target_ ## name ## _schedule_update, \
}

#ifdef CONFIG_DFU_TARGET_MODEM_DELTA
#include "dfu/dfu_target_modem_delta.h"
DEF_DFU_TARGET(modem_delta);
#endif
#ifdef CONFIG_DFU_TARGET_MCUBOOT
#include "dfu/dfu_target_mcuboot.h"
DEF_DFU_TARGET(mcuboot);
#endif
#ifdef CONFIG_DFU_TARGET_FULL_MODEM
#include "dfu/dfu_target_full_modem.h"
DEF_DFU_TARGET(full_modem);
#endif

#define MIN_SIZE_IDENTIFY_BUF 32

LOG_MODULE_REGISTER(dfu_target, CONFIG_DFU_TARGET_LOG_LEVEL);

static const struct dfu_target *current_target;

int dfu_target_img_type(const void *const buf, size_t len)
{
	if (len < MIN_SIZE_IDENTIFY_BUF) {
		return -EAGAIN;
	}
#ifdef CONFIG_DFU_TARGET_MCUBOOT
	if (dfu_target_mcuboot_identify(buf)) {
		return DFU_TARGET_IMAGE_TYPE_MCUBOOT;
	}
#endif
#ifdef CONFIG_DFU_TARGET_MODEM_DELTA
	if (dfu_target_modem_delta_identify(buf)) {
		return DFU_TARGET_IMAGE_TYPE_MODEM_DELTA;
	}
#endif
#ifdef CONFIG_DFU_TARGET_FULL_MODEM
	if (dfu_target_full_modem_identify(buf)) {
		return DFU_TARGET_IMAGE_TYPE_FULL_MODEM;
	}
#endif
	LOG_ERR("No supported image type found");
	return -ENOTSUP;
}

int dfu_target_init(int img_type, int img_num, size_t file_size, dfu_target_callback_t cb)
{
	const struct dfu_target *new_target = NULL;

#ifdef CONFIG_DFU_TARGET_MCUBOOT
	if (img_type == DFU_TARGET_IMAGE_TYPE_MCUBOOT) {
		new_target = &dfu_target_mcuboot;
	}
#endif
#ifdef CONFIG_DFU_TARGET_MODEM_DELTA
	if (img_type == DFU_TARGET_IMAGE_TYPE_MODEM_DELTA) {
		new_target = &dfu_target_modem_delta;
	}
#endif
#ifdef CONFIG_DFU_TARGET_FULL_MODEM
	if (img_type == DFU_TARGET_IMAGE_TYPE_FULL_MODEM) {
		new_target = &dfu_target_full_modem;
	}
#endif
	if (new_target == NULL) {
		LOG_ERR("Unknown image type");
		return -ENOTSUP;
	}

	/* The user is re-initializing with an previously aborted target.
	 * Avoid re-initializing generally to ensure that the download can
	 * continue where it left off. Re-initializing is required for
	 * modem_delta upgrades to re-open the DFU socket that is closed on
	 * abort.
	 */
	if (new_target == current_target
	   && img_type != DFU_TARGET_IMAGE_TYPE_MODEM_DELTA) {
		return 0;
	}

	current_target = new_target;

	return current_target->init(file_size, img_num, cb);
}

int dfu_target_offset_get(size_t *offset)
{
	if (current_target == NULL) {
		return -EACCES;
	}

	return current_target->offset_get(offset);
}

int dfu_target_write(const void *const buf, size_t len)
{
	if (current_target == NULL || buf == NULL) {
		return -EACCES;
	}

	return current_target->write(buf, len);
}

int dfu_target_done(bool successful)
{
	int err;

	if (current_target == NULL) {
		return -EACCES;
	}

	err = current_target->done(successful);
	if (err != 0) {
		LOG_ERR("Unable to clean up dfu_target");
		return err;
	}

	return 0;
}

int dfu_target_reset(void)
{
	if (current_target != NULL) {
		int err = current_target->done(false);

		if (err != 0) {
			LOG_ERR("Unable to clean up dfu_target");
			return err;
		}
	}
	current_target = NULL;
	return 0;
}

int dfu_target_schedule_update(int img_num)
{
	int err = 0;

	if (current_target == NULL) {
		return -EACCES;
	}

	err = current_target->schedule_update(img_num);
	current_target = NULL;

	return err;
}
