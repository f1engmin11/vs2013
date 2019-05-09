#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

uint32_t crc32_table[256];

int make_crc32_table()
{
	uint32_t c;
	int i = 0;
	int bit = 0;

	for (i = 0; i < 256; i++)
	{
		c = (uint32_t)i;

		for (bit = 0; bit < 8; bit++)
		{
			if (c & 1)
			{
				c = (c >> 1) ^ (0xEDB88320);
			}
			else
			{
				c = c >> 1;
			}

		}
		crc32_table[i] = c;
	}


}

uint32_t make_crc(uint32_t crc, unsigned char *string, uint32_t size)
{

	while (size--)
		crc = (crc >> 8) ^ (crc32_table[(crc ^ *string++) & 0xff]);

	return crc;
}

void compare_crc(char *filename)
{
	FILE *sp = NULL;
	uint32_t srcCrc;
	uint32_t calcuCrc = 0xffffffff;
	unsigned char buf[1024];
	uint32_t count;

	if (filename == NULL)
	{
		printf("filename is null\n");
		exit(1);
	}

	sp = fopen(filename, "rb");
	if (sp == NULL)
	{
		printf("open file fail\n");
		exit(1);
	}

	fread(&srcCrc, 1, 4, sp);
	printf("In %s: src crc is 0x%x\n", __FUNCTION__, srcCrc);


	if (sp)
	{
		while (!feof(sp))
		{
			memset(buf, 0, sizeof(buf));
			count = fread(buf, 1, sizeof(buf), sp);
			calcuCrc = make_crc(calcuCrc, buf, count);
		}
	}
	printf("In %s: calcuCrc is 0x%x\n", __FUNCTION__, calcuCrc);

	fclose(sp);

	if (srcCrc == calcuCrc)
	{
		printf("In %s: the calculate crc equal the src crc in file \n", __FUNCTION__);

	}
	else
	{
		printf("In %s: the calculate crc not equal the src crc in file \n", __FUNCTION__);
	}

}


int main()
{
	int i;
	FILE *sp = NULL;
	FILE *dp = NULL;
	uint32_t count;
	uint32_t crc = 0xFFFFFFFF;
	unsigned char buf[1024];


	make_crc32_table();

	sp = fopen("/home/user/work_soft/crc_check/bak/test.txt", "rb");
	if (sp == NULL)
	{
		printf("open file error\n");
		return -1;
	}
	dp = fopen("/home/user/work_soft/crc_check/bak/testcrc.txt", "wb");
	if (dp == NULL)
	{
		printf("open file error\n");
		return -1;
	}

	if (sp)
	{
		while (!feof(sp))
		{
			memset(buf, 0, sizeof(buf));
			count = fread(buf, 1, sizeof(buf), sp);
			crc = make_crc(crc, buf, count);
		}
	}

	printf("In main: calculate crc is 0x%x\n", crc);
	if (dp)
	{
		fwrite(&crc, 1, 4, dp);//write the crc into the file testcrc.txt
		fseek(sp, 0, SEEK_SET);
		while (!feof(sp))
		{
			memset(buf, 0, sizeof(buf));
			count = fread(buf, 1, sizeof(buf), sp);
			fwrite(buf, 1, count, dp);
		}

		fclose(sp);
		fclose(dp);
	}

	/*compare crc*/
	compare_crc("/home/user/work_soft/crc_check/bak/testcrc.txt");


	return 0;

}
