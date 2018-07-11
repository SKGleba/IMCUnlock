/*
 IMCUnlock by SKGleba
 All Rights Reserved
*/
// UI based on voxel's TID
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
#define MOD_PATH "ux0:app/SKGIMCULK/kp"

int main() {
	SceCtrlData pad, old_pad;
	unsigned int keys_down;
	old_pad.buttons = 0;
	int col1 = 255;
	int col2 = 255;
	int col3 = 255;
	vita2d_pgf *font;
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	font = vita2d_load_default_pgf();
	memset(&pad, 0, sizeof(pad));
	
	const char Disclaimer[512] = "\nThis installer modifies boot regions of PSVita 1XXX.\nIt is a very dangerous process, so DO NOT interrupt it.\n\nI provide this tool \"as is\" without warranty of any kind.\n\n\nBy SKGleba (twitter.com/skgleba)";
	const char MainMenuOpts[4][256] = {"Enable internal ux storage", "Disable internal ux storage", "Options", "Exit this app"};
	const char CainMenuOpts[4][256] = {"Storage size", "Backup", "Restore", "Back"};
	const char DainMenuOpts[3][256] = {"300MB", "2.5GB", "Back"};
	const char WainMenuOpts[5][256] = {"Backup vd0", "Backup tm0", "Backup pd0", "Backup id data", "Back"};
	const char KainMenuOpts[5][256] = {"Restore vd0", "Restore tm0", "Restore pd0", "Restore id data", "Back"};
	const char YesNo[2][4] = {"No", "Yes"};
	int UIScreen = 0;
	int selected = 0;
	int doStuff = 0;
	int slimbin = 0;
	int fatbin = 0;
	
	while(1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		keys_down = pad.buttons & ~old_pad.buttons;
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		
		switch(UIScreen) {
			case 0: // Disclaimer Message
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v2.5 by SKGleba");
				vita2d_pgf_draw_text(font, 35, 100, RGBA8(255,255,255,255), 1.5f, Disclaimer);
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 1: // Main Menu
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v2.5 by SKGleba");
				if (selected < 0) selected = 0;
				if (selected > 3) selected = 3;
				for (int i = 0; i < 4; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, MainMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, MainMenuOpts[i]);
				}
				break;
			case 3: // Install Confirmation
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Are you sure you want to flash that mod?");
				if (selected < 0) selected = 0;
				if (selected > 1) selected = 1;
				for (int i = 0; i < 2; i++) {
					if (i == selected) {
						vita2d_draw_rectangle((200*i) + 302, 328, 145, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, (200*i) + 310, 373, RGBA8(0,0,0,255), 1.5f, YesNo[i]);
					} else vita2d_pgf_draw_text(font, (200*i) + 310, 373, RGBA8(255,255,255,255), 1.5f, YesNo[i]);
				}
				break;
			case 4: // Install Ongoing
				if (doStuff == 0) vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Dumping boot data...");
				if (doStuff == 1) {
				filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/imdm");
						SceUID mod_id;

	tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	mod_id = taiLoadStartKernelModuleForUser(MOD_PATH, &argg);

	if (mod_id < 0) {vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "KERNEL ERROR!");sceKernelDelayThread(2 * 1000 * 1000);sceKernelExitProcess(0);}
					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Writing and resigning...");
				} else if (doStuff == 2) {
					if (exists("ux0:data/rmbr.img") == 0) sceKernelExitProcess(0);
					if (slimbin == 1) sceIoRename("ux0:data/imdm", "ux0:data/imwi");
					if (slimbin == 2) sceIoRename("ux0:data/imdm", "ux0:data/imwu");
					if (slimbin == 3) sceIoRename("ux0:data/imdm", "ux0:data/imws");
						SceUID mod_id;

	tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	mod_id = taiLoadStartKernelModuleForUser(MOD_PATH, &argg);
	if (mod_id < 0) {vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "KERNEL ERROR!");sceKernelDelayThread(2 * 1000 * 1000);sceKernelExitProcess(0);}
					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Flashing...\nDo not turn off your Vita or exit the app!");
				} else if (doStuff == 3) {
					if (slimbin == 1) sceIoRename("ux0:data/imwi", "ux0:data/imrt");
					if (slimbin == 2) sceIoRename("ux0:data/imwu", "ux0:data/imrt");
					if (slimbin == 3) sceIoRename("ux0:data/imws", "ux0:data/imrt");
						SceUID mod_id;

	tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	mod_id = taiLoadStartKernelModuleForUser(MOD_PATH, &argg);

	if (mod_id < 0) {vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "KERNEL ERROR!");sceKernelDelayThread(2 * 1000 * 1000);sceKernelExitProcess(0);}

					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Cleaning up...");
				} else if (doStuff == 4) {
					sceIoRemove("ux0:data/imrt");
				} else if (doStuff == 5) UIScreen = 5;
				doStuff++;
				break;
			case 5: // Install Complete
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Flash Success.\n\n\n\nPress CROSS to restart the system.\n\nPress CIRCLE to launch the enso installer.\n\nPress TRIANGLE to exit this app.");
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 7: // Main Menu
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v2.5 by SKGleba");
				if (selected < 0) selected = 0;
				if (selected > 2) selected = 2;
				for (int i = 0; i < 3; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, DainMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, DainMenuOpts[i]);
				}
				break;
			case 8: // Main Menu
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v2.5 by SKGleba");
				if (selected < 0) selected = 0;
				if (selected > 3) selected = 3;
				for (int i = 0; i < 4; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, CainMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, CainMenuOpts[i]);
				}
				break;
			case 24: // Install Ongoing
				if (doStuff == 0) vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Preparing...");
				if (doStuff == 1) {
				if (fatbin == 1) filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/dignvd");
				if (fatbin == 2) filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/digntm");
				if (fatbin == 3) filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/dfs1st");
				if (fatbin == 4) filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/rignvd");
				if (fatbin == 5) filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/rigntm");
				if (fatbin == 6) filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/rfs1st");
				if (fatbin == 7) filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/dignpd");
				if (fatbin == 8) filecopy("ux0:app/SKGIMCULK/sig.enp", "ux0:data/rignpd");

					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Working...\nDo not turn off your Vita or exit the app!");
				} else if (doStuff == 2) {
						SceUID mod_id;

	tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	mod_id = taiLoadStartKernelModuleForUser(MOD_PATH, &argg);

	if (mod_id < 0) {vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "KERNEL ERROR!");sceKernelDelayThread(2 * 1000 * 1000);sceKernelExitProcess(0);}

					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Cleaning up...");
				} else if (doStuff == 3) { doStuff = 0; UIScreen = 25;}
				doStuff++;
				break;
			case 25: // Install Complete
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Success.\n\n\n\nPress CROSS to go back.");
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 27: // Main Menu
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v2.5 by SKGleba");
				if (selected < 0) selected = 0;
				if (selected > 4) selected = 4;
				for (int i = 0; i < 5; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, WainMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, WainMenuOpts[i]);
				}
				break;
			case 37: // Main Menu
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "IMCUnlock v2.5 by SKGleba");
				if (selected < 0) selected = 0;
				if (selected > 4) selected = 4;
				for (int i = 0; i < 5; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 125, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(0,0,0,255), 1.5f, KainMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 100, (80*i) + 170, RGBA8(255,255,255,255), 1.5f, KainMenuOpts[i]);
				}
				break;

		}
		
		if (keys_down & SCE_CTRL_CROSS) {
			switch(UIScreen) {
				case 0:
					UIScreen = 1;
					break;
				case 1:
					if (selected == 0) {slimbin = 1; UIScreen = 3;}
					if (selected == 1) {slimbin = 2; UIScreen = 3;}
					if (selected == 2) UIScreen = 8;
					if (selected == 3) sceKernelExitProcess(0);
					break;
				case 2:
					UIScreen = 1;
					break;
				case 3:
					if (selected == 0) UIScreen = 1;
					if (selected == 1) UIScreen = 4;
					break;
				case 5:
					scePowerRequestColdReset();
					break;
				case 7:
					if (selected == 0) {slimbin = 3; UIScreen = 3;}
					if (selected == 1) {slimbin = 1; UIScreen = 3;}
					if (selected == 2) UIScreen = 8;
					break;
				case 8:
					if (selected == 0) UIScreen = 7;
					if (selected == 1) UIScreen = 27;
					if (selected == 2) UIScreen = 37;
					if (selected == 3) UIScreen = 1;;
					break;
				case 25:
					UIScreen = 27;
					break;
				case 27:
					if (selected == 0) {fatbin = 1; UIScreen = 24;}
					if (selected == 1) {fatbin = 2; UIScreen = 24;}
					if (selected == 2) {fatbin = 3; UIScreen = 24;}
					if (selected == 3) {fatbin = 7; UIScreen = 24;}
					if (selected == 4) UIScreen = 8;
					break;
				case 37:
					if (selected == 0) {fatbin = 4; UIScreen = 24;}
					if (selected == 1) {fatbin = 5; UIScreen = 24;}
					if (selected == 2) {fatbin = 6; UIScreen = 24;}
					if (selected == 3) {fatbin = 8; UIScreen = 24;}
					if (selected == 4) UIScreen = 8;
					break;
			 }
		}
		
		if (keys_down & SCE_CTRL_UP) {
			switch(UIScreen) {
				case 1:
					selected -= 1;
					break;
				case 7:
					selected -= 1;
					break;
				case 8:
					selected -= 1;
					break;
				case 27:
					selected -= 1;
					break;
				case 37:
					selected -= 1;
					break;
			}
		} if (keys_down & SCE_CTRL_DOWN) {
			switch(UIScreen) {
				case 1:
					selected += 1;
					break;
				case 7:
					selected += 1;
					break;
				case 8:
					selected += 1;
					break;
				case 27:
					selected += 1;
					break;
				case 37:
					selected += 1;
					break;
			}
		} if (keys_down & SCE_CTRL_RIGHT) {
			switch(UIScreen) {
				case 3:
					selected += 1;
					break;
				case 6:
					selected += 1;
					break;
			}
		} if (keys_down & SCE_CTRL_LEFT) {
			switch(UIScreen) {
				case 3:
					selected -= 1;
					break;
				case 6:
					selected -= 1;
					break;
			}
		} if (keys_down & SCE_CTRL_TRIANGLE) {
			switch(UIScreen) {
				case 5:
					sceKernelExitProcess(0);
					break;
			}
		} if (keys_down & SCE_CTRL_CIRCLE) {
			switch(UIScreen) {
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
