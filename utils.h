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
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/openpsid.h>
#include <vita2d.h>
#include <psp2/types.h>

int WriteFile(char *file, void *buf, int size);

int ex(const char *fname);
int hasEndSlash(const char *path);
int removePath(const char *path);
int fcp(const char *from, const char *to);
