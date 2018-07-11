/*
IMCUnlocKERN by SKGleba
All Rights Reserved
*/
// Source needs to be cleaned, ill do it later
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/io/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
// locs
#define SLOC "ux0:app/SKGIMCULK/sig.enp"
#define ODEV "sdstor0:int-lp-act-entire"
#define DDEV "ux0:data/rmbr.img"
#define ODEV1 "sdstor0:int-lp-ign-vtrm"
#define DDEV1 "ux0:data/tm0.img"
#define ODEV2 "sdstor0:int-lp-ign-vshdata"
#define DDEV2 "ux0:data/vd0.img"
#define ODEV3 "sdstor0:int-lp-ign-pidata"
#define DDEV3 "ux0:data/pd0.img"
#define DDEV4 "ux0:data/id_data.img"
#define TLC1 "ux0:data/imdm"
#define TLC2 "ux0:data/imrt"
#define TLC3 "ux0:data/imwi"
#define TLC4 "ux0:data/imwu"
#define TLC5 "ux0:data/imws"
#define TLC6 "ux0:data/digntm"
#define TLC7 "ux0:data/dignvd"
#define TLC8 "ux0:data/dignpd"
#define TLC9 "ux0:data/dfs1st"
#define TLC10 "ux0:data/rigntm"
#define TLC11 "ux0:data/rignvd"
#define TLC12 "ux0:data/rignpd"
#define TLC13 "ux0:data/rfs1st"
// szs
#define BBSZ 0x8000
#define SBSZ 0x1
#define BDSZ 0x40000
// offs
// OF1 = SBSZ
#define OF2 0x2
#define OF3 0x3
#define OF4 0x4
#define OF5 0x5
#define OF7 0x7
#define OF8 0x9
#define OF9 0x8
#define OFM1 0x102
#define OFM2 0x113
#define OFM3 0x302
#define OFM4 0x313
#define OFM5 0x124
#define OFD1 0x2000000
#define OFD2 0x80000
// fdefs
#define i0p ksceIoOpen
#define i0c ksceIoClose
#define i0L ksceIoLseek
#define i0R ksceIoRead
#define i0r ksceIoRemove
#define i0W ksceIoWrite
#define aWO SCE_O_WRONLY
#define aRO SCE_O_RDONLY
#define aRW SCE_O_RDWR
#define fCR SCE_O_CREAT
#define fPP SCE_O_APPEND
#define fSX SCE_SEEK_SET

// Fix allowing to run with sceshell (thx mlcl)
int sifix(void *func) {
int ret = 0, res = 0, uid = 0;
ret = uid = ksceKernelCreateThread("sifix", func, 64, 0x10000, 0, 0, 0);
if (ret < 0) {ret = -1;goto cleanup;}
if ((ret = ksceKernelStartThread(uid, 0, NULL)) < 0) {ret = -1;goto cleanup;}
if ((ret = ksceKernelWaitThreadEnd(uid, &res, NULL)) < 0) {ret = -1;goto cleanup;}
ret = res;
cleanup:
if (uid > 0)ksceKernelDeleteThread(uid);
return ret;}
int ex(const char* flc) {
SceUID fd;
fd = i0p(flc, aRO, 0);
if (fd < 0) {
i0c(fd);
return 0;}
i0c(fd);
return 1;}
// Do it using files as flags, useful.
int d(void) {
if (ex(TLC1) == 1){
if (ex(DDEV) == 1) i0r(DDEV);
SceUID fd = i0p(ODEV, aRW, 0777);
SceUID wfd = i0p(DDEV, aWO | fCR | fPP, 6);
static char buf[BBSZ];
unsigned int i = 0;
i0L(fd, 0, fSX);
for(i=0;i<BDSZ;i=i+BBSZ){
i0R(fd, buf, BBSZ);
i0W(wfd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}
if (ex(TLC2) == 1){
if (ex(DDEV) == 1){
SceUID fd = i0p(DDEV, aRO, 0777);
SceUID wfd = i0p(ODEV, aRW, 0777);
static char buf[BBSZ];
unsigned int i = 0;
i0L(fd, 0, fSX);
for(i=0;i<BDSZ;i=i+BBSZ){
i0R(fd, buf, BBSZ);
i0W(wfd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);
i0r(DDEV);}}
if (ex(TLC3) == 1){
SceUID fd = i0p(SLOC, aRO, 0777);
SceUID wfd = i0p(DDEV, aRW, 0777);
static char buf[SBSZ];
unsigned int i = 0;
i0L(fd, SBSZ, fSX);
i0L(wfd, OFM1, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF2, fSX);
i0L(wfd, OFM2, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF3, fSX);
i0L(wfd, OFM5, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF4, fSX);
i0L(wfd, OFM3, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF5, fSX);
i0L(wfd, OFM4, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}
if (ex(TLC4) == 1){
SceUID fd = i0p(SLOC, aRO, 0777);
SceUID wfd = i0p(DDEV, aRW, 0777);
static char buf[SBSZ];
unsigned int i = 0;
i0L(wfd, OFM1, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, SBSZ, fSX);
i0L(wfd, OFM2, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF2, fSX);
i0L(wfd, OFM5, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF3, fSX);
i0L(wfd, OFM3, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF4, fSX);
i0L(wfd, OFM4, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}
if (ex(TLC5) == 1){
SceUID fd = i0p(SLOC, aRO, 0777);
SceUID wfd = i0p(DDEV, aRW, 0777);
static char buf[SBSZ];
unsigned int i = 0;
i0L(fd, OF2, fSX);
i0L(wfd, OFM1, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF4, fSX);
i0L(wfd, OFM2, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF5, fSX);
i0L(wfd, OFM5, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF5, fSX);
i0L(wfd, OFM3, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
i0L(fd, OF7, fSX);
i0L(wfd, OFM4, fSX);
for(i=0;i<SBSZ;i=i+SBSZ){
i0R(fd, buf, SBSZ);
i0W(wfd, buf, SBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}
if (ex(TLC6) == 1){
if (ex(DDEV1) == 1) i0r(DDEV1);
i0r(TLC6);
SceUID fd = i0p(ODEV1, aRO, 0777);
SceUID wfd = i0p(DDEV1, aWO | fCR | fPP, 6);
static char buf[BBSZ];
unsigned int i = 0;
for(i=0;i<OFD1;i=i+BBSZ){
i0R(fd, buf, BBSZ);
i0W(wfd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}
if (ex(TLC7) == 1){
if (ex(DDEV2) == 1) i0r(DDEV2);
i0r(TLC7);
SceUID fd = i0p(ODEV2, aRO, 0777);
SceUID wfd = i0p(DDEV2, aWO | fCR | fPP, 6);
static char buf[BBSZ];
unsigned int i = 0;
for(i=0;i<OFD1;i=i+BBSZ){
i0R(fd, buf, BBSZ);
i0W(wfd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}
if (ex(TLC8) == 1){
if (ex(DDEV3) == 1) i0r(DDEV3);
i0r(TLC8);
SceUID fd = i0p(ODEV3, aRO, 0777);
SceUID wfd = i0p(DDEV3, aWO | fCR | fPP, 6);
static char buf[BBSZ];
unsigned int i = 0;
for(i=0;i<OFD1;i=i+BBSZ){
i0R(fd, buf, BBSZ);
i0W(wfd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}
if (ex(TLC9) == 1){
if (ex(DDEV4) == 1) i0r(DDEV4);
i0r(TLC9);
SceUID fd = i0p(ODEV, aRO, 0777);
SceUID wfd = i0p(DDEV4, aWO | fCR | fPP, 6);
static char buf[BBSZ];
unsigned int i = 0;
i0L(fd, BDSZ, fSX);
for(i=0;i<OFD2;i=i+BBSZ){
i0R(fd, buf, BBSZ);
i0W(wfd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}
if (ex(TLC10) == 1){
i0r(TLC10);
if (ex(DDEV1) == 1){
SceUID fd = i0p(ODEV1, aRW, 0777);
SceUID wfd = i0p(DDEV1, aRO, 0777);
static char buf[BBSZ];
unsigned int i = 0;
for(i=0;i<OFD1;i=i+BBSZ){
i0R(wfd, buf, BBSZ);
i0W(fd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}}
if (ex(TLC11) == 1){
i0r(TLC11);
if (ex(DDEV2) == 1){
SceUID fd = i0p(ODEV2, aRW, 0777);
SceUID wfd = i0p(DDEV2, aRO, 0777);
static char buf[BBSZ];
unsigned int i = 0;
for(i=0;i<OFD1;i=i+BBSZ){
i0R(wfd, buf, BBSZ);
i0W(fd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}}
if (ex(TLC12) == 1){
i0r(TLC12);
if (ex(DDEV3) == 1){
SceUID fd = i0p(ODEV3, aRW, 0777);
SceUID wfd = i0p(DDEV3, aRO, 0777);
static char buf[BBSZ];
unsigned int i = 0;
for(i=0;i<OFD1;i=i+BBSZ){
i0R(wfd, buf, BBSZ);
i0W(fd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}}
if (ex(TLC13) == 1){
i0r(TLC13);
if (ex(DDEV4) == 1){
SceUID fd = i0p(ODEV, aRW, 0777);
SceUID wfd = i0p(DDEV4, aRO, 0777);
static char buf[BBSZ];
unsigned int i = 0;
i0L(fd, BDSZ, fSX);
for(i=0;i<OFD2;i=i+BBSZ){
i0R(wfd, buf, BBSZ);
i0W(fd, buf, BBSZ);}
if (fd > 0) i0c(fd);
if (wfd > 0) i0c(wfd);}}}
int pd(int dlc){
ENTER_SYSCALL(dlc);
sifix(d);
EXIT_SYSCALL(dlc);}
void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {
pd(0);
return SCE_KERNEL_START_SUCCESS;}
int module_stop(SceSize args, void *argp) {
return SCE_KERNEL_STOP_SUCCESS;}
