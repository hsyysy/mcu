#include <sd_io.h>
#include <debug.h>
#include <string.h>

static SD_DEV sd;

int sd_init(void)
{
	return SD_Init(&sd) == SD_OK ? 0 : -1;
}

int sd_read(void *data, unsigned long sector, unsigned long num)
{
	return SD_Read(&sd, data, sector, 0, num * 512) == SD_OK ? 0 : -1;
}

int sd_write(void *data, unsigned long sector, unsigned long num)
{
	unsigned long i;

	for (i = 0; i < num; ++i, ++sector)
		if (SD_Write(&sd, (uint8_t *)data + i * 512, sector) != SD_OK)
			return -1;

	return 0;
}

int sd_test(void)
{
	uint8_t rd[512];
	uint8_t wr[512];
	memset(rd, 0xff, sizeof(rd));
	memset(wr, 0x5a, sizeof(wr));

	if (sd_init()) {
		debug("sd card init failed\n");
		return -1;
	}

	if (sd_read(rd, 0, 1)) {
		debug("card read failed\n");
		return -1;
	}

	debug("original first sector data:\n");
	dump(rd, sizeof(rd));

	if (sd_write(wr, 0, 1)) {
		debug("card write failed\n");
		return -1;
	}

	debug("rewrite first sector with 0x5a\n");

	if (sd_read(rd, 0, 1)) {
		debug("card read failed\n");
		return -1;
	}
	dump(rd, sizeof(rd));

	if (memcmp(rd, wr, sizeof(rd))) {
		debug("data write error\n");
		return -1;
	}

	debug("seems good\n");
	return 0;
}