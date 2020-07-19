/*
 * FWTOOL@PSP2
 * Copyright (C) 2019-2020 skgleba
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#define BLOCK_SIZE 0x200

#define CVMB(x) (((x * BLOCK_SIZE) / 1024) / 1024)
#define MBCV(x) (((x * 1024) * 1024) / BLOCK_SIZE)

#define ARRAYSIZE(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

const char *part_code(int code) {
	static char *codes[] = {
		"empty",
		"idstorage",
		"slb2",
		"os0",
		"vs0",
		"vd0",
		"tm0",
		"ur0",
		"ux0",
		"gro0",
		"grw0",
		"ud0",
		"sa0",
		"mediaid",
		"pd0",
		"unused"
	};
	return codes[code];
}

const char *part_type(int type) {
	if (type == 6)
		return "FAT16";
	else if (type == 7)
		return "exFAT";
	else if (type == 0xDA)
		return "raw";
	return "unknown";
}

typedef struct {
	uint32_t off;
	uint32_t sz;
	uint8_t code;
	uint8_t type;
	uint8_t active;
	uint32_t flags;
	uint16_t unk;
} __attribute__((packed)) partition_t;

typedef struct {
	char prev_unus[9];
	partition_t partitions[7];
} __attribute__((packed)) mbr_part_t;

typedef struct {
	char magic[0x20];
	uint32_t version;
	uint32_t device_size;
	char unk1[0x28];
	partition_t partitions[0x10];
	char unk2[0x5e];
	char unk3[0x10 * 4];
	uint16_t sig;
} __attribute__((packed)) master_block_t;

static unsigned char mbr_part[sizeof(mbr_part_t)];
static unsigned char mbr_full[sizeof(master_block_t)];

const char part_magic[9] = { 0x05, 0x06, 0x00, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00 };
const char mbr_magic[0x20] = "Sony Computer Entertainment Inc.";

uint32_t getSz(const char *src) {
	FILE *fp = fopen(src, "rb");
	if (fp == NULL)
		return 0;
	fseek(fp, 0L, SEEK_END);
	uint32_t sz = ftell(fp);
	fclose(fp);
	return sz;
}

int readCheckMbrPart(const char *src) {
	FILE *fp = fopen(src, "rb");
	if (fp == NULL)
		return -1;
	fread(mbr_part, 1, sizeof(mbr_part_t), fp);
	fclose(fp);
	return memcmp(mbr_part, part_magic, 9);
}

void parseMbrPart(mbr_part_t *pmbr, int slot) {
	if (slot == 69) {
		printf("\nPossible device size: %d(MB)\n\n", CVMB(pmbr->partitions[6].sz));
		printf("Partitions:\n");
		for (int i = 0; i < 7; ++i) {
			partition_t *p = &pmbr->partitions[i];
			if (p->code > 0)
				printf("%d)\n name: %s(0x%X) active=%d\n offset=(MB)%d size=%d(MB)\n type=%s(0x%X) flags=0x%08x\n\n", i, part_code(p->code), p->code, p->active, CVMB(p->off), CVMB(p->sz), part_type(p->type), p->type, p->flags);
			else
				printf("%d is empty\n", i);
		}
	} else {
		partition_t *p = &pmbr->partitions[slot];
		if (p->code > 0)
			printf("%d)\n name: %s(0x%X) active=%d\n offset=(MB)%d size=%d(MB)\n type=%s(0x%X) flags=0x%08x\n\n", slot, part_code(p->code), p->code, p->active, CVMB(p->off), CVMB(p->sz), part_type(p->type), p->type, p->flags);
		else
			printf("%d is empty\n", slot);
	}
}

int applyMbrPart(const char *dst) {
	FILE *fp = fopen(dst, "wb");
	if (fp == NULL)
		return -1;
	fwrite(mbr_part, 1, sizeof(mbr_part_t), fp);
	fclose(fp);
	return 0;
}

int readCheckMbrFull(const char *src) {
	FILE *fp = fopen(src, "rb");
	if (fp == NULL)
		return -1;
	fread(mbr_full, 1, sizeof(master_block_t), fp);
	fclose(fp);
	return memcmp(mbr_full, mbr_magic, 0x20);
}

int extractPartFromFull(const char *dst) {
	FILE *fp = fopen(dst, "wb");
	if (fp == NULL)
		return -1;
	fwrite((mbr_full + 0xe0), 1, sizeof(mbr_part_t), fp);
	fclose(fp);
	return 0;
}

void parseMbrFull(master_block_t *fmbr, int slot) {
	if (slot == 69) {
		printf("\nMBR:\n %s\n version: %d\n device size: %d(MB)\n\n", fmbr->magic, fmbr->version, CVMB(fmbr->device_size));
		printf("Partitions:\n");
		for (int i = 0; i < ARRAYSIZE(fmbr->partitions); ++i) {
			partition_t *p = &fmbr->partitions[i];
			if (p->code > 0)
				printf("%d)\n name: %s(0x%X) active=%d\n offset=(MB)%d size=%d(MB)\n type=%s(0x%X) flags=0x%08x\n\n", i, part_code(p->code), p->code, p->active, CVMB(p->off), CVMB(p->sz), part_type(p->type), p->type, p->flags);
			else
				printf("%d is empty\n", i);
		}
	} else {
		partition_t *p = &fmbr->partitions[slot];
		if (p->code > 0)
			printf("%d)\n name: %s(0x%X) active=%d\n offset=(MB)%d size=%d(MB)\n type=%s(0x%X) flags=0x%08x\n\n", slot, part_code(p->code), p->code, p->active, CVMB(p->off), CVMB(p->sz), part_type(p->type), p->type, p->flags);
		else
			printf("%d is empty\n", slot);
	}
}

int applyMbrFull(const char *dst) {
	FILE *fp = fopen(dst, "wb");
	if (fp == NULL)
		return -1;
	fwrite(mbr_full, 1, sizeof(master_block_t), fp);
	fclose(fp);
	return 0;
}

int partialMain(int argc, char *argv[]) {
	
	int ret = readCheckMbrPart(argv[1]);
	if (ret != 0) {
		printf("file check error\n");
		return -1;
	}
	
	mbr_part_t *pmbr = (mbr_part_t *)mbr_part;
	
	if (strcmp("all", argv[2]) == 0) {
        parseMbrPart(pmbr, 69);
		return 0;
    }
	
	int opslot = atoi(argv[2]);
	if (opslot > 6) {
		printf("slot oob\n");
		return -1;
	}
	
	parseMbrPart(pmbr, opslot);
	partition_t *tpart = &pmbr->partitions[opslot];
	
	for (int i=3; i< argc; i-=-1) {
     	if (strcmp("-info", argv[i]) == 0)
       		return 0;
		else if (strcmp("-clone", argv[i]) == 0) {
			i-=-1;
			if (atoi(argv[i]) < 7)
				memcpy(&pmbr->partitions[opslot], &pmbr->partitions[atoi(argv[i])], sizeof(partition_t));
        } else if (strcmp("-offset", argv[i]) == 0 && i < argc) {
        	i-=-1;
        	tpart->off = MBCV((uint32_t)atoi(argv[i]));
        } else if (strcmp("-size", argv[i]) == 0 && i < argc) {
        	i-=-1;
        	tpart->sz = MBCV((uint32_t)atoi(argv[i]));
        } else if (strcmp("-type", argv[i]) == 0 && i < argc) {
        	i-=-1;
			tpart->type = (uint8_t)strtoul((argv[i] + 2), NULL, 16);
        } else if (strcmp("-code", argv[i]) == 0 && i < argc) {
        	i-=-1;
			tpart->code = (uint8_t)strtoul((argv[i] + 2), NULL, 16);
        } else if (strcmp("-active", argv[i]) == 0 && i < argc) {
        	i-=-1;
			tpart->active = (uint8_t)atoi(argv[i]);
        } else if (strcmp("-flags", argv[i]) == 0 && i < argc) {
        	i-=-1;
        	tpart->flags = (uint32_t)strtoul(argv[i], NULL, 16);
        }
 	}
	
	if (applyMbrPart(argv[1]) < 0)
		printf("write failed\n");
	
	printf("\nfinished: %s\n\noutput:\n", argv[1]);
	
	parseMbrPart(pmbr, opslot);
	
 	return 0;
}

int fullMain(int argc, char *argv[]) {
	
	int ret = readCheckMbrFull(argv[1]);
	if (ret != 0) {
		printf("file check error\n");
		return -1;
	}
	
	if (strcmp("extract", argv[2]) == 0)
        return extractPartFromFull(argv[3]);
	
	master_block_t *fmbr = (master_block_t *)mbr_full;
	
	if (strcmp("all", argv[2]) == 0) {
        parseMbrFull(fmbr, 69);
		return 0;
    }
	
	int opslot = atoi(argv[2]);
	if (opslot > ARRAYSIZE(fmbr->partitions)) {
		printf("slot oob\n");
		return -1;
	}
	
	parseMbrFull(fmbr, opslot);
	partition_t *tpart = &fmbr->partitions[opslot];
	
	for (int i=3; i< argc; i-=-1) {
     	if (strcmp("-info", argv[i]) == 0)
       		return 0;
		else if (strcmp("-clone", argv[i]) == 0) {
			i-=-1;
			if (atoi(argv[i]) < ARRAYSIZE(fmbr->partitions))
				memcpy(&fmbr->partitions[opslot], &fmbr->partitions[atoi(argv[i])], sizeof(partition_t));
        } else if (strcmp("-offset", argv[i]) == 0 && i < argc) {
        	i-=-1;
        	tpart->off = MBCV((uint32_t)atoi(argv[i]));
        } else if (strcmp("-size", argv[i]) == 0 && i < argc) {
        	i-=-1;
        	tpart->sz = MBCV((uint32_t)atoi(argv[i]));
        } else if (strcmp("-type", argv[i]) == 0 && i < argc) {
        	i-=-1;
			tpart->type = (uint8_t)strtoul((argv[i] + 2), NULL, 16);
        } else if (strcmp("-code", argv[i]) == 0 && i < argc) {
        	i-=-1;
			tpart->code = (uint8_t)strtoul((argv[i] + 2), NULL, 16);
        } else if (strcmp("-active", argv[i]) == 0 && i < argc) {
        	i-=-1;
			tpart->active = (uint8_t)atoi(argv[i]);
        } else if (strcmp("-flags", argv[i]) == 0 && i < argc) {
        	i-=-1;
        	tpart->flags = (uint32_t)strtoul(argv[i], NULL, 16);
        }
 	}
	
	if (applyMbrFull(argv[1]) < 0)
		printf("write failed\n");
	
	printf("\nfinished: %s\n\noutput:\n", argv[1]);
	
	parseMbrFull(fmbr, opslot);
	
 	return 0;
}

int main(int argc, char *argv[]) {
	
	if(argc < 4){
		printf("\nusage: ./[binname] [scembr] [slot] [opt ..]\n\n");
		printf("[scembr] = dumped SCE MBR (512B) or user partition table (128B)\n");
		printf("[slot] = partition slot number or 'all' to list all slots\n");
		printf("[opt] = optional args ('h' = hex and 'd' = dec):\n");
		printf(" '-info' displays [slot] contents\n");
		printf(" '-clone dX' copies slot 'X' to slot [slot]\n");
		printf(" '-offset dX' sets [slot]->offset to 'X'\n");
		printf(" '-size dX' sets [slot]->size to 'X'\n");
		printf(" '-type hX' sets (u8)[slot]->type to 'X'\n");
		printf(" '-code hX' sets (u8)[slot]->code to 'X'\n");
		printf(" '-active dX' sets [slot]->active to 'X'\n");
		printf(" '-flags hX' sets (u32)[slot]->flags to 'X'\n");
		printf("\npartition codes:\n");
		for (int i = 0; i < 16; ++i) {
				printf(" %s: 0x%X\n", part_code(i), i);
		}
		printf("\n'./mbrtool scembr.part 3 -clone 1 -code 0x8' - create a ux0 partition in slot 3 that shares its parameters with the one in slot 1 (hybrid)\n");
		return -1;
	}
	
	int ret = (int)getSz(argv[1]);
	if (ret == sizeof(mbr_part_t))
		ret = partialMain(argc, argv);
	else if (ret == sizeof(master_block_t))
		ret = fullMain(argc, argv);
	else
		printf("bad file size - %d\n", ret);
	
	printf("\nexit status: 0x%X\n", ret);
	
 	return ret;
}
