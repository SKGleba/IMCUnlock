/*
 * IMCUnlock
 * Copyright (C) 2018-2020 skgleba
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#include "tai_compat.h"
#include "imcunlock.h"
#include "logging.h"

static unsigned char mbr_part[sizeof(mbr_part_t)];
static unsigned char mbr_full[sizeof(master_block_t)];
static int is_enso = 0, emmc = 0, nomc_disabled = 0;

static int get_enso_state(void) { // checks if enso is installed
	int (* read_mmc)() = NULL;
	int mmctx = ksceSdifGetSdContextPartValidateMmc(0);
	char mbr_char[0x200], sbr_char[0x200], rmbr_char[0x200];
	tai_module_info_t info;			
	info.size = sizeof(info);
	ksceSdifReadSectorMmc(mmctx, 1, &sbr_char, 1);
	if (memcmp(&sbr_char, mbr_magic, 0x20) == 0) { // enso was installed at some point
		ksceSdifReadSectorMmc(mmctx, 0, &mbr_char, 1);
		if (memcmp(&mbr_char, &sbr_char, 0x200) == 0) { // enso may be even installed rn
			if (taiGetModuleInfoForKernel(KERNEL_PID, "SceSdif", &info) >= 0) {
				module_get_offset(KERNEL_PID, info.modid, 0, 0x3e7d, &read_mmc); // same for 3.60-3.73
				if (read_mmc == NULL)
					return -1;
				read_mmc(mmctx, 0, &rmbr_char, 1);
				if (memcmp(&mbr_char, &rmbr_char, 0x200) != 0) // enso is installed
					return 1;
			} else
				return -1;
		}
	}
	return 0;
}

static int checkPartition(uint8_t pcode, uint32_t minsize) {
	mbr_part_t *pmbr = (mbr_part_t *)mbr_part;
	for (int i = 0; i < 7; ++i) {
		partition_t *p = &pmbr->partitions[i];
		if (p->code == pcode) {
			if (p->sz < minsize)
				return -1;
			return 0;
		}
	}
	return -1;
}

static int verifyNotOOB(uint32_t emmcsz) {
	mbr_part_t *pmbr = (mbr_part_t *)mbr_part;
	for (int i = 0; i < 7; ++i) {
		partition_t *p = &pmbr->partitions[i];
		if ((p->sz + p->off) > emmcsz) {
			return -1;
		}
	}
	return 0;
}

static int updateMbr(void) {
	if (memcmp(mbr_full, mbr_magic, 0x20) != 0) {
		LOG("write_mbr failed - unknown mbr magic\n");
		return -1;
	}
	int ret = ksceSdifWriteSectorMmc(emmc, (is_enso) ? 1 : 0, mbr_full, 1);
	LOG("write finished 0x%X\n", ret);
	return ret;
}

int kimcu_get_mc_rls(void) {
	if (checkPartition(0x8, 0) == 0) {
		if (kscePervasiveRemovableMemoryGetCardInsertState())
			return 0;
		return 1;
	}
	return 0;
}

uint32_t kimcu_emmc_size(void) {
	master_block_t *mbr = (master_block_t *)mbr_full;
	return mbr->device_size;
}

int kimcu_work_mbrpart(int mode, void *buf) {
	LOG("work_mbr(%d)\n", mode);
	int state = 0;
	if (mode == 1) {
		ENTER_SYSCALL(state);
		ksceKernelMemcpyUserToKernel(mbr_part, buf, sizeof(mbr_part_t));
		EXIT_SYSCALL(state);
		LOG("checking the mbr part... ");
		if (memcmp(mbr_part, part_magic, 9) != 0) {
			LOG("work_mbr failed -  unknown part magic\n");
			return -1;
		}
		LOG("ok\nchecking ud0... ");
		if (checkPartition(0xB, MBCV(158)) < 0) {
			LOG("work_mbr failed -  ud0 is too small or not present\n");
			return -1;
		}
		LOG("ok\nchecking ur0... ");
		if (checkPartition(0x7, MBCV(64)) < 0) {
			LOG("work_mbr failed -  ur0 is too small or not present\n");
			return -1;
		}
		LOG("ok\nbound-checking all the partitions... ");
		if (verifyNotOOB(kimcu_emmc_size()) < 0) {
			LOG("work_mbr failed -  partition OOB\n");
			return -1;
		}
		LOG("ok\nchecking imc lock status... ");
		if ((checkPartition(0x8, 0) < 0) && (nomc_disabled)) {
			LOG("work_mbr failed -  removing imc is prohibited\n");
			return -1;
		}
		memcpy((mbr_full + 0xe0), mbr_part, sizeof(mbr_part_t));
		LOG("ok\nchecks done, flashing the MBR\n");
		return updateMbr();
	}
	memcpy(mbr_part, (mbr_full + 0xe0), sizeof(mbr_part_t));
	ENTER_SYSCALL(state);
	ksceKernelMemcpyKernelToUser(buf, mbr_part, sizeof(mbr_part_t));
	EXIT_SYSCALL(state);
	return 0;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args)
{
	LOG_START("\nimcunlocK started!\n");
	
	LOG("getting emmc ctx\n");
	emmc = ksceSdifGetSdContextPartValidateMmc(0);
	if (emmc == 0) {
		LOG("read_mbr failed - no ctx\n");
		return SCE_KERNEL_START_FAILED;
	}
	
	LOG("checking the mbr\n");
	ksceSdifReadSectorMmc(emmc, 0, mbr_full, 1);
	if (memcmp(mbr_full, mbr_magic, 0x20) != 0) {
		LOG("read_mbr failed - unknown mbr magic\n");
		return SCE_KERNEL_START_FAILED;
	}
	
	memcpy(mbr_part, (mbr_full + 0xe0), sizeof(mbr_part_t));
	if (memcmp(mbr_part, part_magic, 9) != 0) {
		LOG("read_mbr failed -  unknown part magic\n");
		return SCE_KERNEL_START_FAILED;
	}
	
	nomc_disabled = kimcu_get_mc_rls();
	is_enso = get_enso_state();
	LOG("imc remove lock status %d\n", nomc_disabled);
	LOG("enso status: %d\n", is_enso);
	LOG("emmc size %d(MB)\n", CVMB(kimcu_emmc_size()));
	
	LOG("\ninit complete, waiting for cmd\n");
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
	LOG("stopping K\n");
	return SCE_KERNEL_STOP_SUCCESS;
}
