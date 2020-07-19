#define LOG_LOC "ux0:data/imcunlock.log"
#define LOGGING_ENABLED 1

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

const char part_magic[9] = { 0x05, 0x06, 0x00, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00 };
const char mbr_magic[0x20] = "Sony Computer Entertainment Inc.";
