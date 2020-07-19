/*
 * IMCUnlock
 * Copyright (C) 2018-2020 skgleba
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/devctl.h>
#include <psp2/ctrl.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/io/stat.h>
#include <taihen.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "graphics.h"

#include "../plugin/imcunlock.h"

#define printf psvDebugScreenPrintf
#define ARRAYSIZE(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

int vshPowerRequestColdReset(void);
static uint32_t emmc_size = 0;
static unsigned char mbr_part[sizeof(mbr_part_t)];
static int nomc_disabled = 0;

enum {
	SCREEN_WIDTH = 960,
	SCREEN_HEIGHT = 544,
	PROGRESS_BAR_WIDTH = SCREEN_WIDTH,
	PROGRESS_BAR_HEIGHT = 10,
	LINE_SIZE = SCREEN_WIDTH,
};

static unsigned buttons[] = {
	SCE_CTRL_SELECT,
	SCE_CTRL_START,
	SCE_CTRL_UP,
	SCE_CTRL_RIGHT,
	SCE_CTRL_DOWN,
	SCE_CTRL_LEFT,
	SCE_CTRL_LTRIGGER,
	SCE_CTRL_RTRIGGER,
	SCE_CTRL_TRIANGLE,
	SCE_CTRL_CIRCLE,
	SCE_CTRL_CROSS,
	SCE_CTRL_SQUARE,
};

uint32_t getSz(const char *src) {
	FILE *fp = fopen(src, "rb");
	if (fp == NULL)
		return 0;
	fseek(fp, 0L, SEEK_END);
	uint32_t sz = ftell(fp);
	fclose(fp);
	return sz;
}

int readMbrPart(const char *src) {
	FILE *fp = fopen(src, "rb");
	if (fp == NULL)
		return -1;
	fread(mbr_part, 1, sizeof(mbr_part_t), fp);
	fclose(fp);
	return 0;
}

int applyMbrPart(const char *dst) {
	FILE *fp = fopen(dst, "wb");
	if (fp == NULL)
		return -1;
	fwrite(mbr_part, 1, sizeof(mbr_part_t), fp);
	fclose(fp);
	return 0;
}

uint32_t get_key(void) {
	static unsigned prev = 0;
	SceCtrlData pad;
	while (1) {
		memset(&pad, 0, sizeof(pad));
		sceCtrlPeekBufferPositive(0, &pad, 1);
		unsigned new = prev ^ (pad.buttons & prev);
		prev = pad.buttons;
		for (size_t i = 0; i < sizeof(buttons)/sizeof(*buttons); ++i)
			if (new & buttons[i])
				return buttons[i];

		sceKernelDelayThread(1000); // 1ms
	}
}

void press_exit(void) {
	printf("Press any key to exit this application.\n");
	get_key();
	sceKernelExitProcess(0);
}

void press_reboot(void) {
	printf("Press any key to reboot.\n");
	get_key();
	vshPowerRequestColdReset();
}

int check_safe_mode(void) {
	if (sceIoDevctl("ux0:", 0x3001, NULL, 0, NULL, 0) == 0x80010030) {
		return 1;
	} else {
		return 0;
	}
}

int dorfptable(const char *dst) {
	psvDebugScreenSetFgColor(COLOR_YELLOW);
	printf("\npress TRIANGLE to flash or any other key to dump\n");
	if (get_key() == SCE_CTRL_TRIANGLE) {
		if (readMbrPart(dst) < 0) {
			printf("could not find ux0:data/scembr.part\n");
			return 1;
		}
		if (kimcu_work_mbrpart(1, mbr_part) < 0)
			return -1;
		psvDebugScreenSetFgColor(COLOR_GREEN);
		printf("\ndone !\n");
		return 0;
	}
	if (kimcu_work_mbrpart(0, mbr_part) < 0)
		return -1;
	applyMbrPart(dst);
	psvDebugScreenSetFgColor(COLOR_GREEN);
	printf("\ndumped to ux0:data/scembr.part\n");
	return 0;
}

int rebuildPart(int mode) {
	psvDebugScreenSetFgColor(COLOR_PURPLE);
	if ((mode == 3) && (nomc_disabled)) {
		printf("\nplease insert a memory card to continue\n");
		return -1;
	}
	printf("\nstarting mbr rebuild\n\n");
	psvDebugScreenSetFgColor(COLOR_CYAN);
	memset(mbr_part, 0, sizeof(mbr_part_t));
	memcpy(mbr_part, part_magic, 9);
	mbr_part_t *pmbr = (mbr_part_t *)mbr_part;
	uint32_t emmcsz = (uint32_t)kimcu_emmc_size();
	uint32_t uruxsz = (emmcsz - 0x200000);
	// ud0
	pmbr->partitions[0].off = 0xE8000;
	pmbr->partitions[0].sz = 0x80000;
	pmbr->partitions[0].code = 0xB;
	pmbr->partitions[0].type = 0x6;
	pmbr->partitions[0].flags = 0x00000fff;
	printf("ud0- off:(MB)%d | sz:%d(MB)\n", CVMB(pmbr->partitions[0].off), CVMB(pmbr->partitions[0].sz));
	// pd0
	pmbr->partitions[1].off = 0x168000;
	pmbr->partitions[1].sz = 0x98000;
	pmbr->partitions[1].code = 0xE;
	pmbr->partitions[1].type = 0x7;
	pmbr->partitions[1].flags = 0x00000fff;
	printf("pd0- off:(MB)%d | sz:%d(MB)\n", CVMB(pmbr->partitions[1].off), CVMB(pmbr->partitions[1].sz));
	// ur0
	pmbr->partitions[2].off = (mode == 1) ? 0x168000 : 0x200000;
	if (mode == 1)
		pmbr->partitions[2].sz = 0x98000;
	else if (mode == 2)
		pmbr->partitions[2].sz = 0x300000;
	else
		pmbr->partitions[2].sz = uruxsz;
	pmbr->partitions[2].code = 0x7;
	pmbr->partitions[2].type = 0x7;
	pmbr->partitions[2].flags = 0x00000fff;
	printf("ur0- off:(MB)%d | sz:%d(MB)\n", CVMB(pmbr->partitions[2].off), CVMB(pmbr->partitions[2].sz));
	// ux0 or nuffin
	if (mode < 3) {
		pmbr->partitions[3].off = (pmbr->partitions[2].off + pmbr->partitions[2].sz);
		if (mode == 0)
			pmbr->partitions[3].off = (pmbr->partitions[3].off - MBCV(102));
		pmbr->partitions[3].sz = (emmcsz - pmbr->partitions[3].off);
		pmbr->partitions[3].code = 0x8;
		pmbr->partitions[3].type = 0x7;
		pmbr->partitions[3].flags = 0x00000fff;
		pmbr->partitions[4].sz = pmbr->partitions[3].sz;
		printf("ux0- off:(MB)%d | sz:%d(MB)\n", CVMB(pmbr->partitions[3].off), CVMB(pmbr->partitions[3].sz));
	} else {
		pmbr->partitions[3].sz = uruxsz;
		pmbr->partitions[4].sz = emmcsz;
	}
	// follow sonys pattern
	pmbr->partitions[5].sz = pmbr->partitions[6].sz = emmcsz;
	psvDebugScreenSetFgColor(COLOR_PURPLE);
	printf("\nflashing\n");
	psvDebugScreenSetFgColor(COLOR_YELLOW);
	return kimcu_work_mbrpart(1, mbr_part);
}

char mmit[][256] = {" -> 100MB ux0 partition"," -> Hybrid ur0-pd0 and 2.5GB ux0"," -> Default 2xxx storage configuration"," -> Default 1xxx storage configuration"," -> Dump/Flash the user partition table"," -> Exit"};
int sel = 0;
int optct = 6;

void smenu(){
	psvDebugScreenClear(COLOR_BLACK);
	psvDebugScreenSetFgColor(COLOR_CYAN);
	printf("                       IMCUnlock v4.0                            \n");
	printf("                         By SKGleba                              \n");
	psvDebugScreenSetFgColor(COLOR_RED);
	for(int i = 0; i < optct; i++){
		if(sel==i){
			psvDebugScreenSetFgColor(COLOR_GREEN);
		}
		printf("%s\n", mmit[i]);
		psvDebugScreenSetFgColor(COLOR_RED);
	}
	printf("\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	
	tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	SceUID mod_id = taiLoadStartKernelModuleForUser("ux0:app/SKGIMCULK/imcunlock.skprx", &argg);
	if (mod_id > 0)
		sceAppMgrLoadExec("app0:eboot.bin", NULL, NULL);

	int should_reboot = 0;
	int ret = 0;

	psvDebugScreenInit();

	psvDebugScreenSetFgColor(COLOR_RED);
	if (check_safe_mode()) {
		printf("Please disable HENkaku Safe Mode\n\n");
		press_exit();
	}
	
	nomc_disabled = kimcu_get_mc_rls();
	emmc_size = kimcu_emmc_size();
	if ((int)emmc_size <= 0) {
		printf("EUNKSZ\n\n");
		press_exit();
	}
	
	printf("This software will make PERMANENT modifications to your Vita\nIf anything goes wrong, there is NO RECOVERY.\n\n");
	
	psvDebugScreenSetFgColor(COLOR_GREEN);
	printf("\n\n -> I understood, continue.\n\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);

	if (get_key() != SCE_CTRL_CROSS) {
		press_exit();
	}
	
switch_opts:
	smenu();
	sceKernelDelayThread(0.3 * 1000 * 1000);
	switch (get_key()) {
		case SCE_CTRL_CROSS:
			if (sel == 5)
				goto cleanup;
			should_reboot = 1;
			if (sel == 4)
				ret = dorfptable("ux0:data/scembr.part");
			else
				ret = rebuildPart(sel);
			break;
		case SCE_CTRL_UP:
			if(sel!=0){
				sel--;
			}
			goto switch_opts;
		case SCE_CTRL_DOWN:
			if(sel+1<optct){
				sel++;
			}
			goto switch_opts;
		case SCE_CTRL_CIRCLE:
			break;
		default:
			goto switch_opts;
	}

	if (ret < 0) {
		psvDebugScreenSetFgColor(COLOR_RED);
		printf("\nAn error has occurred.\n\n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		printf("The log file can be found at ux0:data/imcunlock.log\n\n");
		should_reboot = 0;
	}

cleanup:
	if (should_reboot) {
		press_reboot();
	} else {
		press_exit();
	}

	return 0;
}
