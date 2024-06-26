/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <autoconf.h>
#include <secure_services.h>
#include <toolchain.h>
#include <fw_info.h>
#include <arch/arm/aarch32/cortex_m/fpu.h>

static struct fpu_ctx_full context_buffer;

void before_nse(void)
{
	k_sched_lock();
	z_arm_save_fp_context(&context_buffer);
}

void after_nse(void)
{
	z_arm_restore_fp_context(&context_buffer);
	k_sched_unlock();
}

#ifdef CONFIG_SPM_SERVICE_REBOOT
NRF_NSE(void, spm_request_system_reboot);

/* Overrides the weak ARM implementation:
 * Call into secure firmware if in the non-secure firmware since the non-secure
 * firmware is not allowed to directly reboot the system.
 */
void sys_arch_reboot(int type)
{
	ARG_UNUSED(type);
	spm_request_system_reboot();
}
#endif /* CONFIG_SPM_SERVICE_REBOOT */

#ifdef CONFIG_SPM_SERVICE_RNG
NRF_NSE(int, spm_request_random_number, uint8_t *output, size_t len,
					size_t *olen);
#endif /* CONFIG_SPM_SERVICE_RNG */

#ifdef CONFIG_SPM_SERVICE_READ
NRF_NSE(int, spm_request_read, void *destination, uint32_t addr, size_t len);
#endif /* CONFIG_SPM_SERVICE_READ */

#ifdef CONFIG_SPM_SERVICE_FIND_FIRMWARE_INFO
NRF_NSE(int, spm_firmware_info, uint32_t fw_address, struct fw_info *info);
#endif /* CONFIG_SPM_SERVICE_FIND_FIRMWARE_INFO */

#ifdef CONFIG_SPM_SERVICE_PREVALIDATE
NRF_NSE(int, spm_prevalidate_b1_upgrade, uint32_t dst_addr, uint32_t src_addr);
#endif /* CONFIG_SPM_SERVICE_PREVALIDATE */

#ifdef CONFIG_SPM_SERVICE_BUSY_WAIT
NRF_NSE(void, spm_busy_wait, uint32_t busy_wait_us);
#endif /* CONFIG_SPM_SERVICE_BUSY_WAIT */

#ifdef CONFIG_SPM_SERVICE_NS_HANDLER_FROM_SPM_FAULT
NRF_NSE(int, spm_set_ns_fatal_error_handler, spm_ns_on_fatal_error_t handler);
#endif /* CONFIG_SPM_SERVICE_NS_HANDLER_FROM_SPM_FAULT */
