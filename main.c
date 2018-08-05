/*
	IMCUnlock by SKGleba
	All Rights Reserved
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/shellutil.h>
#include <psp2/power.h>
#include <psp2/kernel/processmgr.h>
#include <vita2d.h>
#include <taihen.h>

#include "utils.h"
#define KPPATH "ux0:app/SKGIMCULK/kp"

int scr = 0;

int callkp(int mode, const char *chsznm) {
if (mode == 1){
char frpath[28];
fcp("ux0:app/SKGIMCULK/kp", "ur0:temp/dmtfrel");
SceUID mod_id;
tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	mod_id = taiLoadStartKernelModuleForUser(KPPATH, &argg);
if (mod_id < 0)sceKernelExitProcess(0);
		argg.size = sizeof(argg);
		argg.pid = KERNEL_PID;
		argg.args = 0;
		argg.argp = NULL;
		argg.flags = 0;
		taiStopUnloadKernelModuleForUser(mod_id, &argg, NULL, NULL);
if (ex("ur0:temp/q") == 1){sprintf(frpath, "ux0:app/SKGIMCULK/q/%s", chsznm); sceIoRemove("ur0:temp/q");}
if (ex("ur0:temp/p") == 1){sprintf(frpath, "ux0:app/SKGIMCULK/p/%s", chsznm); sceIoRemove("ur0:temp/p");}
if (ex("ur0:temp/o") == 1){sprintf(frpath, "ux0:app/SKGIMCULK/o/%s", chsznm); sceIoRemove("ur0:temp/o");}
if (ex(frpath) == 0) scr = 55;
if (ex(frpath) == 1) fcp(frpath, "ur0:temp/wroptable.img");}
if (mode == 2){
fcp("ux0:app/SKGIMCULK/kp", "ur0:temp/wptronl");
SceUID mod_id;
tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	mod_id = taiLoadStartKernelModuleForUser(KPPATH, &argg);
if (mod_id < 0)sceKernelExitProcess(0);
		argg.size = sizeof(argg);
		argg.pid = KERNEL_PID;
		argg.args = 0;
		argg.argp = NULL;
		argg.flags = 0;
		taiStopUnloadKernelModuleForUser(mod_id, &argg, NULL, NULL);}
if (mode == 3){
char bdpath[15];
sprintf(bdpath, "ur0:temp/%s", chsznm);
fcp("ux0:app/SKGIMCULK/kp", bdpath);
fcp("ux0:app/SKGIMCULK/kp", "ur0:temp/dev2img");
SceUID mod_id;
tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	mod_id = taiLoadStartKernelModuleForUser(KPPATH, &argg);
if (mod_id < 0)sceKernelExitProcess(0);
		argg.size = sizeof(argg);
		argg.pid = KERNEL_PID;
		argg.args = 0;
		argg.argp = NULL;
		argg.flags = 0;
		taiStopUnloadKernelModuleForUser(mod_id, &argg, NULL, NULL);}
return 1;
}

int main() {
if (ex("ur0:temp/wroptable.img") == 1) sceIoRemove("ur0:temp/wroptable.img");
	if (ex("ur0:temp/dmtfrel") == 1) sceIoRemove("ur0:temp/dmtfrel");
	if (ex("ur0:temp/wptronl") == 1) sceIoRemove("ur0:temp/wptronl");
	SceCtrlData pad, old_pad;
	unsigned int kd;
	old_pad.buttons = 0;
	int col1 = 255;
	int col2 = 255;
	int col3 = 255;
	int devn = 0;
	char bouf[32];
	vita2d_pgf *font;
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	font = vita2d_load_default_pgf();
	memset(&pad, 0, sizeof(pad));
	
	const char din[512] = "\nThis installer modifies boot regions of PSP2.\nIt is a very dangerous process, so DO NOT interrupt it.\n\nI provide this tool \"as is\" without warranty of any kind.\n\n\nBy SKGleba (twitter.com/skgleba)";
	const char enots[512] = "\nThis EMMC type is not supported, please contact me:\n\ngithub.com/skgleba\nor\ntwitter.com/skgleba";
	const char cmo[4][256] = {"Set IMC storage size", "Backup", "Restore", "Exit this app"};
	const char mmo[4][256] = {"List IMC sizes < 1GB", "List IMC sizes > 1GB", "Create a hybrid partition", "Back"};
	const char dmo[4][256] = {"0MB (ur0 2.5GB)", "100MB (ur0 2.5GB)", "300MB (ur0 2.5GB, no pd0)", "Back"};
	const char rmo[4][256] = {"Fix second block", "Flash custom roptable", "Dump roptable", "Dump MBR"};
	const char nmo[5][256] = {"1GB (ur0 1.5GB)", "1.5GB (ur0 1GB)", "2.5GB (ur0 300MB, no pd0)", "2.8GB (ur0 150MB, no pd0)", "Back"};
	const char imo[5][256] = {"pd0 and IMC", "pd0 and ur0, IMC 2.5GB", "ud0 and IMC", "ud0 and ur0, IMC 2.5GB", "Back"};
	const char wmo[5][256] = {"Backup vd0", "Backup tm0", "Backup pd0", "Backup id data", "Back"};
	const char kmo[5][256] = {"Restore vd0", "Restore tm0", "Restore pd0", "Restore id data", "Back"};
	const char ny[2][4] = {"No", "Yes"};
	int sel = 0;
	int do_shit = 0;
	int wfl = 0;
	int brfl = 0;
	
	while(1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		kd = pad.buttons & ~old_pad.buttons;
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		
		switch(scr) {
			case 0:
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v3 by SKGleba");
				vita2d_pgf_draw_text(font, 35, 100, RGBA8(255,255,255,255), 1.5f, din);
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 1: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v3 by SKGleba");
				if (sel < 0) sel = 0;
				if (sel > 3) sel = 3;
				for (int i = 0; i < 4; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, cmo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, cmo[i]);
				}
				break;
			case 3:

				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Are you sure that you want to flash that mod?");
				if (sel < 0) sel = 0;
				if (sel > 1) sel = 1;
				for (int i = 0; i < 2; i++) {
					if (i == sel) {
						vita2d_draw_rectangle((200*i) + 302, 328, 145, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, (200*i) + 310, 373, RGBA8(0,0,0,255), 1.5f, ny[i]);
					} else vita2d_pgf_draw_text(font, (200*i) + 310, 373, RGBA8(255,255,255,255), 1.5f, ny[i]);
				}
				break;
			case 4: 
				if (do_shit == 0) vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Preparing...");
				if (do_shit == 1) {
				if (wfl == 1) callkp(1, "0000.img");
				if (wfl == 2) callkp(1, "0100.img");
				if (wfl == 3) callkp(1, "0300.img");
				if (wfl == 4) callkp(1, "1000.img");
				if (wfl == 5) callkp(1, "1500.img");
				if (wfl == 6) callkp(1, "2500.img");
				if (wfl == 7) callkp(1, "2800.img");
				if (wfl == 8) callkp(1, "pd0h.img");
				if (wfl == 9) callkp(1, "pr0h.img");
				if (wfl == 10) callkp(1, "ud0h.img");
				if (wfl == 69) callkp(1, "rd0h.img");
				} else if (do_shit == 2) scr = 3;
				do_shit++;
				break;
			case 5: 
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Flash Success.\n\n\n\nPress CROSS to restart the system.\n\nPress CIRCLE to launch the enso installer.\n\nPress TRIANGLE to exit this app.");
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 55: 
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, enots);
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 7: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "Choose list mode");
				if (sel < 0) sel = 0;
				if (sel > 3) sel = 3;
				for (int i = 0; i < 4; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, mmo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, mmo[i]);
				}
				break;
			case 8: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v3 by SKGleba");
				if (sel < 0) sel = 0;
				if (sel > 3) sel = 3;
				for (int i = 0; i < 4; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, cmo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, cmo[i]);
				}
				break;
			case 24: 
				if (do_shit == 0) vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Working...\nDo not turn off your Vita or exit the app!");
				if (do_shit == 1) {
				if (brfl == 1) callkp(3, "dignvd");
				if (brfl == 2) callkp(3, "digntm");
				if (brfl == 3) callkp(3, "dignpd");
				if (brfl == 4) callkp(3, "rignvd");
				if (brfl == 5) callkp(3, "rigntm");
				if (brfl == 6) callkp(3, "rignpd");
				if (brfl == 7) callkp(3, "dfs1st");
				if (brfl == 8) callkp(3, "rfs1st");
				if (brfl == 9) callkp(3, "dfsmbr");
				if (brfl == 10) callkp(3, "wrotbl");
				if (brfl == 11) callkp(3, "drotbl");
				if (brfl == 12) {fcp("ux0:app/SKGIMCULK/zsb.img", "ur0:temp/wroptable.img"); callkp(3, "zerosb");}
					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Cleaning up...");
				} else if (do_shit == 2) { do_shit = 0; scr = 25;}
				do_shit++;
				break;
			case 25: 
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Success.\n\n\n\nPress CROSS to go back.");
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 27: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v3 by SKGleba");
				if (sel < 0) sel = 0;
				if (sel > 4) sel = 4;
				for (int i = 0; i < 5; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, wmo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, wmo[i]);
				}
				break;
			case 37: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v3 by SKGleba");
				if (sel < 0) sel = 0;
				if (sel > 4) sel = 4;
				for (int i = 0; i < 5; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, kmo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, kmo[i]);
				}
				break;


			case 69: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "Advanced options");
				if (sel < 0) sel = 0;
				if (sel > 3) sel = 3;
				for (int i = 0; i < 4; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, rmo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, rmo[i]);
				}
				break;


			case 70: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "Choose IMC size");
				if (sel < 0) sel = 0;
				if (sel > 3) sel = 3;
				for (int i = 0; i < 4; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, dmo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, dmo[i]);
				}
				break;

			case 71: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "Choose IMC size");
				if (sel < 0) sel = 0;
				if (sel > 4) sel = 4;
				for (int i = 0; i < 5; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, nmo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, nmo[i]);
				}
				break;

			case 72: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "Create a hybrid partition");
				if (sel < 0) sel = 0;
				if (sel > 4) sel = 4;
				for (int i = 0; i < 5; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, imo[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, imo[i]);
				}
				break;

		}
		
		if (kd & SCE_CTRL_CROSS) {
			switch(scr) {
				case 0:
					scr = 1;
					break;
				case 1:
					do_shit = 0;
					if (sel == 0) scr = 7;
					if (sel == 1) scr = 27;
					if (sel == 2) scr = 37;
					if (sel == 3) sceKernelExitProcess(0);
					break;
				case 55:
					sceKernelExitProcess(0);
					break;
				case 3:
					do_shit = 0;
					if (sel == 0) scr = 1;
					if (sel == 1) {callkp(2, "kappa"); scr = 5;}
					break;
				case 5:
					scePowerRequestColdReset();
					break;
				case 7:
					do_shit = 0;
					if (sel == 0) scr = 70;
					if (sel == 1) scr = 71;
					if (sel == 2) scr = 72;
					if (sel == 3) scr = 1;
					break;
				case 8:
					do_shit = 0;
					if (sel == 0) scr = 7;
					if (sel == 1) scr = 27;
					if (sel == 2) scr = 37;
					if (sel == 3) scr = 1;
					break;
				case 25:
					scr = 1;
					break;
				case 27:
					do_shit = 0;
					if (sel == 0) {brfl = 1; scr = 24;}
					if (sel == 1) {brfl = 2; scr = 24;}
					if (sel == 2) {brfl = 3; scr = 24;}
					if (sel == 3) {brfl = 7; scr = 24;}
					if (sel == 4) scr = 8;
					break;
				case 37:
					do_shit = 0;
					if (sel == 0) {brfl = 4; scr = 24;}
					if (sel == 1) {brfl = 5; scr = 24;}
					if (sel == 2) {brfl = 6; scr = 24;}
					if (sel == 3) {brfl = 8; scr = 24;}
					if (sel == 4) scr = 8;
					break;
				case 69:
					do_shit = 0;
					if (sel == 0) {brfl = 12; scr = 24;}
					if (sel == 1) {brfl = 10; scr = 24;}
					if (sel == 2) {brfl = 11; scr = 24;}
					if (sel == 3) {brfl = 9; scr = 24;}
					break;
				case 70:
					do_shit = 0;
					if (sel == 0) {wfl = 1; scr = 4;}
					if (sel == 1) {wfl = 2; scr = 4;}
					if (sel == 2) {wfl = 3; scr = 4;}
					if (sel == 3) scr = 7;
				break;
				case 71:
					do_shit = 0;
					if (sel == 0) {wfl = 4; scr = 4;}
					if (sel == 1) {wfl = 5; scr = 4;}
					if (sel == 2) {wfl = 6; scr = 4;}
					if (sel == 3) {wfl = 7; scr = 4;}
					if (sel == 4) scr = 7;
				break;
				case 72:
					do_shit = 0;
					if (sel == 0) {wfl = 8; scr = 4;}
					if (sel == 1) {wfl = 9; scr = 4;}
					if (sel == 2) {wfl = 10; scr = 4;}
					if (sel == 3) {wfl = 69; scr = 4;}
					if (sel == 4) scr = 7;
				break;
			 }
		}
		
		if (kd & SCE_CTRL_UP) {
			switch(scr) {
				case 1:
					sel -= 1;
					break;
				case 7:
					sel -= 1;
					break;
				case 8:
					sel -= 1;
					break;
				case 27:
					sel -= 1;
					break;
				case 37:
					sel -= 1;
					break;
				case 69:
					sel -= 1;
					break;
				case 70:
					sel -= 1;
					break;
				case 71:
					sel -= 1;
					break;
				case 72:
					sel -= 1;
					break;
			}
		} if (kd & SCE_CTRL_DOWN) {
			switch(scr) {
				case 1:
					sel += 1;
					break;
				case 7:
					sel += 1;
					break;
				case 8:
					sel += 1;
					break;
				case 27:
					sel += 1;
					break;
				case 37:
					sel += 1;
					break;
				case 69:
					sel += 1;
					break;
				case 70:
					sel += 1;
					break;
				case 71:
					sel += 1;
					break;
				case 72:
					sel += 1;
					break;
			}
		} if (kd & SCE_CTRL_RIGHT) {
			switch(scr) {
				case 3:
					sel += 1;
					break;
				case 6:
					sel += 1;
					break;
			}
		} if (kd & SCE_CTRL_LEFT) {
			switch(scr) {
				case 3:
					sel -= 1;
					break;
				case 6:
					sel -= 1;
					break;
			}
		} if (kd & SCE_CTRL_TRIANGLE) {
			switch(scr) {
				case 0:
					scr = 69;
					break;
				case 69:
					scr = 0;
					break;
				case 5:
					sceKernelExitProcess(0);
					break;
			}
		} if (kd & SCE_CTRL_CIRCLE) {
			switch(scr) {
				case 5:
					sceAppMgrLaunchAppByUri(0xFFFFF, "psgm:play?titleid=MLCL00003");
					sceKernelExitProcess(0);
					break;
			}
		}
		
		old_pad = pad;
		
		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	vita2d_fini();
	vita2d_free_pgf(font);
	
	sceKernelExitProcess(0);
	return 0;
}
