/*
	IMCTool plugin v3.0
	By SKGleba
	All Rights Reserved
*/

#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/io/fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

int siofix(void *func) {
	int ret = 0;
	int res = 0;
	int uid = 0;
	ret = uid = ksceKernelCreateThread("siofix", func, 64, 0x10000, 0, 0, 0);
	if (ret < 0){ret = -1; goto cleanup;}
	if ((ret = ksceKernelStartThread(uid, 0, NULL)) < 0) {ret = -1; goto cleanup;}
	if ((ret = ksceKernelWaitThreadEnd(uid, &res, NULL)) < 0) {ret = -1; goto cleanup;}
	ret = res;
cleanup:
	if (uid > 0) ksceKernelDeleteThread(uid);
	return ret;}

int ex(const char* filloc) {
  SceUID fd;
  fd = ksceIoOpen(filloc, SCE_O_RDONLY, 0);
  if (fd < 0) {ksceIoClose(fd); return 0;}
ksceIoClose(fd); return 1;}

int dump(void){
if (ex("ur0:temp/dmtfrel") == 1){
ksceIoRemove("ur0:temp/dmtfrel");
if (ex("ur0:temp/tidn.img") == 1) ksceIoRemove("ur0:temp/tidn.img");
static char buffer[0x8000];
unsigned int i = 0;
	SceUID dfd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDONLY, 0777);
	SceUID efd = ksceIoOpen("ur0:temp/tidn.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
ksceIoLseek(dfd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x8000){
		ksceIoRead(dfd, buffer, 0x8000);
		ksceIoWrite(efd, buffer, 0x8000);}
	if (dfd > 0) ksceIoClose(dfd);
	if (efd > 0) ksceIoClose(efd);
	static char vsbuf[1];
	SceUID ffd = ksceIoOpen("ur0:temp/tidn.img", SCE_O_RDONLY, 0777);
if (ffd >= 0){
    ksceIoLseek(ffd, 0x26, SCE_SEEK_SET);
	ksceIoRead(ffd, vsbuf, 1);
	ksceIoClose(ffd);}
ksceIoRename("ur0:temp/tidn.img", vsbuf);}
if (ex("ur0:temp/wptronl") == 1){
ksceIoRemove("ur0:temp/wptronl");
if (ex("ur0:temp/wroptable.img") == 1){
if (ex("ur0:temp/tmbr.img") == 1) ksceIoRemove("ur0:temp/tmbr.img");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x8000];
	unsigned int i = 0;
ksceIoLseek(fd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x8000){
		ksceIoRead(fd, buffer, 0x8000);
		ksceIoWrite(wfd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);
	static char vsbuf[0x80];
	SceUID cfd = ksceIoOpen("ur0:temp/wroptable.img", SCE_O_RDONLY, 0777);
	SceUID dfd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_RDWR, 0777);
    ksceIoLseek(dfd, 0xE0, SCE_SEEK_SET);
	for(i=0;i<0x80;i=i+0x80){
		ksceIoRead(cfd, vsbuf, 0x80);
		ksceIoWrite(dfd, vsbuf, 0x80);}
    ksceIoLseek(dfd, 0x2E0, SCE_SEEK_SET);
	for(i=0;i<0x80;i=i+0x80){
		ksceIoRead(cfd, vsbuf, 0x80);
		ksceIoWrite(dfd, vsbuf, 0x80);}
	if (cfd > 0) ksceIoClose(cfd);
	if (dfd > 0) ksceIoClose(dfd);
	SceUID afd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_RDONLY, 0777);
	SceUID bfd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDWR, 0777);
ksceIoLseek(afd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x8000){
		ksceIoRead(afd, buffer, 0x8000);
		ksceIoWrite(bfd, buffer, 0x8000);}
	if (afd > 0) ksceIoClose(afd);
	if (bfd > 0) ksceIoClose(bfd);
ksceIoRemove("ur0:temp/tmbr.img");
ksceIoRemove("ur0:temp/wroptable.img");}}
if (ex("ur0:temp/dev2img") == 1){
ksceIoRemove("ur0:temp/dev2img");
if (ex("ur0:temp/dfsmbr") == 1){
if (ex("ux0:data/mbr.img") == 1) ksceIoRemove("ux0:data/mbr.img");
	ksceIoRemove("ur0:temp/dfsmbr");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/mbr.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x4000];
	unsigned int i = 0;
ksceIoLseek(fd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x4000){
		ksceIoRead(fd, buffer, 0x4000);
		ksceIoWrite(wfd, buffer, 0x4000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}
if (ex("ur0:temp/wrotbl") == 1){
ksceIoRemove("ur0:temp/wrotbl");
if (ex("ur0:temp/wroptable.img") == 1){
if (ex("ur0:temp/tmbr.img") == 1) ksceIoRemove("ur0:temp/tmbr.img");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x8000];
	unsigned int i = 0;
ksceIoLseek(fd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x8000){
		ksceIoRead(fd, buffer, 0x8000);
		ksceIoWrite(wfd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);
	static char vsbuf[0x80];
	SceUID cfd = ksceIoOpen("ur0:temp/wroptable.img", SCE_O_RDONLY, 0777);
	SceUID dfd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_RDWR, 0777);
    ksceIoLseek(dfd, 0xE0, SCE_SEEK_SET);
	for(i=0;i<0x80;i=i+0x80){
		ksceIoRead(cfd, vsbuf, 0x80);
		ksceIoWrite(dfd, vsbuf, 0x80);}
	if (cfd > 0) ksceIoClose(cfd);
	if (dfd > 0) ksceIoClose(dfd);
	SceUID afd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_RDONLY, 0777);
	SceUID bfd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDWR, 0777);
ksceIoLseek(afd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x8000){
		ksceIoRead(afd, buffer, 0x8000);
		ksceIoWrite(bfd, buffer, 0x8000);}
	if (afd > 0) ksceIoClose(afd);
	if (bfd > 0) ksceIoClose(bfd);
ksceIoRemove("ur0:temp/tmbr.img");}}
if (ex("ur0:temp/drotbl") == 1){
ksceIoRemove("ur0:temp/drotbl");
if (ex("ur0:temp/tmbr.img") == 1) ksceIoRemove("ur0:temp/tmbr.img");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x8000];
	unsigned int i = 0;
ksceIoLseek(fd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x8000){
		ksceIoRead(fd, buffer, 0x8000);
		ksceIoWrite(wfd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);
	static char vsbuf[0x80];
	SceUID cfd = ksceIoOpen("ux0:data/wroptable.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	SceUID dfd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_RDONLY, 0777);
    ksceIoLseek(dfd, 0xE0, SCE_SEEK_SET);
	for(i=0;i<0x80;i=i+0x80){
		ksceIoRead(dfd, vsbuf, 0x80);
		ksceIoWrite(cfd, vsbuf, 0x80);}
	if (cfd > 0) ksceIoClose(cfd);
	if (dfd > 0) ksceIoClose(dfd);
ksceIoRemove("ur0:temp/tmbr.img");}
if (ex("ur0:temp/zerosb") == 1){
ksceIoRemove("ur0:temp/zerosb");
if (ex("ur0:temp/wroptable.img") == 1){
if (ex("ur0:temp/tmbr.img") == 1) ksceIoRemove("ur0:temp/tmbr.img");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x8000];
	unsigned int i = 0;
ksceIoLseek(fd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x8000){
		ksceIoRead(fd, buffer, 0x8000);
		ksceIoWrite(wfd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);
	static char vsbuf[0x80];
	SceUID cfd = ksceIoOpen("ur0:temp/wroptable.img", SCE_O_RDONLY, 0777);
	SceUID dfd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_RDWR, 0777);
    ksceIoLseek(dfd, 0x2E0, SCE_SEEK_SET);
	for(i=0;i<0x80;i=i+0x80){
		ksceIoRead(cfd, vsbuf, 0x80);
		ksceIoWrite(dfd, vsbuf, 0x80);}
	if (cfd > 0) ksceIoClose(cfd);
	if (dfd > 0) ksceIoClose(dfd);
	SceUID afd = ksceIoOpen("ur0:temp/tmbr.img", SCE_O_RDONLY, 0777);
	SceUID bfd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDWR, 0777);
ksceIoLseek(afd, 0, SCE_SEEK_SET);
	for(i=0;i<0x40000;i=i+0x8000){
		ksceIoRead(afd, buffer, 0x8000);
		ksceIoWrite(bfd, buffer, 0x8000);}
	if (afd > 0) ksceIoClose(afd);
	if (bfd > 0) ksceIoClose(bfd);
ksceIoRemove("ur0:temp/tmbr.img");}}
if (ex("ur0:temp/dignvd") == 1){
if (ex("ux0:data/vd0.img") == 1) ksceIoRemove("ux0:data/vd0.img");
	ksceIoRemove("ur0:temp/dignvd");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-ign-vshdata", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/vd0.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x8000];
	unsigned int i = 0;
	for(i=0;i<0x2000000;i=i+0x8000){
		ksceIoRead(fd, buffer, 0x8000);
		ksceIoWrite(wfd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}
if (ex("ur0:temp/rignvd") == 1){
	ksceIoRemove("ur0:temp/rignvd");
if (ex("ux0:data/vd0.img") == 1){
	SceUID fd = ksceIoOpen("sdstor0:int-lp-ign-vshdata", SCE_O_RDWR, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/vd0.img", SCE_O_RDONLY, 0777);
	static char buffer[0x8000];
	unsigned int i = 0;
	for(i=0;i<0x2000000;i=i+0x8000){
		ksceIoRead(wfd, buffer, 0x8000);
		ksceIoWrite(fd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}}
if (ex("ur0:temp/digntm") == 1){
if (ex("ux0:data/tm0.img") == 1) ksceIoRemove("ux0:data/tm0.img");
	ksceIoRemove("ur0:temp/digntm");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-ign-vtrm", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/tm0.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x8000];
	unsigned int i = 0;
	for(i=0;i<0x2000000;i=i+0x8000){
		ksceIoRead(fd, buffer, 0x8000);
		ksceIoWrite(wfd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}
if (ex("ur0:temp/rigntm") == 1){
	ksceIoRemove("ur0:temp/rigntm");
if (ex("ux0:data/tm0.img") == 1){
	SceUID fd = ksceIoOpen("sdstor0:int-lp-ign-vtrm", SCE_O_RDWR, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/tm0.img", SCE_O_RDONLY, 0777);
	static char buffer[0x8000];
	unsigned int i = 0;
	for(i=0;i<0x2000000;i=i+0x8000){
		ksceIoRead(wfd, buffer, 0x8000);
		ksceIoWrite(fd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}}
if (ex("ur0:temp/dignpd") == 1){
if (ex("ux0:data/pd0.img") == 1) ksceIoRemove("ux0:data/pd0.img");
	ksceIoRemove("ur0:temp/dignpd");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-ign-pidata", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/pd0.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x8000];
	unsigned int i = 0;
	for(i=0;i<0x13000000;i=i+0x8000){
		ksceIoRead(fd, buffer, 0x8000);
		ksceIoWrite(wfd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}
if (ex("ur0:temp/rignpd") == 1){
	ksceIoRemove("ur0:temp/rignpd");
if (ex("ux0:data/pd0.img") == 1){
	SceUID fd = ksceIoOpen("sdstor0:int-lp-ign-pidata", SCE_O_RDWR, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/pd0.img", SCE_O_RDONLY, 0777);
	static char buffer[0x8000];
	unsigned int i = 0;
	for(i=0;i<0x13000000;i=i+0x8000){
		ksceIoRead(wfd, buffer, 0x8000);
		ksceIoWrite(fd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}}
if (ex("ur0:temp/dfs1st") == 1){
if (ex("ux0:data/id_data.img") == 1) ksceIoRemove("ux0:data/id_data.img");
	ksceIoRemove("ur0:temp/dfs1st");
	SceUID fd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDONLY, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/id_data.img", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
	static char buffer[0x8000];
	unsigned int i = 0;
ksceIoLseek(fd, 0x40000, SCE_SEEK_SET);
	for(i=0;i<0x80000;i=i+0x8000){
		ksceIoRead(fd, buffer, 0x8000);
		ksceIoWrite(wfd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}
if (ex("ur0:temp/rfs1st") == 1){
	ksceIoRemove("ur0:temp/rfs1st");
if (ex("ux0:data/id_data.img") == 1){
	SceUID fd = ksceIoOpen("sdstor0:int-lp-act-entire", SCE_O_RDWR, 0777);
	SceUID wfd = ksceIoOpen("ux0:data/id_data.img", SCE_O_RDONLY, 0777);
	static char buffer[0x8000];
	unsigned int i = 0;
ksceIoLseek(fd, 0x40000, SCE_SEEK_SET);
	for(i=0;i<0x80000;i=i+0x8000){
		ksceIoRead(wfd, buffer, 0x8000);
		ksceIoWrite(fd, buffer, 0x8000);}
	if (fd > 0) ksceIoClose(fd);
	if (wfd > 0) ksceIoClose(wfd);}}}}

int pdump(int dloc){
	int ret = 0;
	int state = 0;
	ENTER_SYSCALL(state);
	ret = siofix(dump);
	EXIT_SYSCALL(state);
return ret;}

void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {
pdump(0);
	return SCE_KERNEL_START_SUCCESS;}

int module_stop(SceSize args, void *argp) {
	return SCE_KERNEL_STOP_SUCCESS;}
