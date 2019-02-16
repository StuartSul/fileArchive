#include "archive.h"

int isFile(char *fileName)
{
	FILE *fp = fopen(fileName, "rb");

	if (fp)
	{
		fclose(fp);
		return 1;
	}
	else
		return 0;
}

int fcopy(FILE *dst, size_t size, FILE *src)
{
	void *buffer;

	if (size <= BUFFERSIZE)
	{
		buffer = malloc_s(size);
		fread(buffer, 1, size, src);
		fwrite(buffer, 1, size, dst);
	}
	else
	{
		size_t num = 0;
		buffer = malloc_s(BUFFERSIZE);

		while ((num = fread(buffer, 1, BUFFERSIZE, src)))
		{
			if (num < size)
			{
				fwrite(buffer, 1, num, dst);
				size -= num;
			}
			else if (size <= num)
			{
				fwrite(buffer, 1, size, dst);
				fseek(src, (long)size - (long)num, SEEK_CUR);
				break;
			}
		}
	}

	free(buffer);
	return 1;
}

uint64_t getFileSize(FILE *fp)
{
	uint64_t fileSize = 0;
	long curPos = ftell(fp);

	fseek(fp, 0L, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, curPos, SEEK_SET);

	return fileSize;
}

FILE *fopen_s(const char *fileName, const char *mode)
{
	FILE *fp = fopen(fileName, mode);

	if (fp)
		return fp;
	else
	{
		printf("Error occured while opening %s in \"%s\" mode. Program terminated.\n", fileName, mode);
		exit(-1);
		return NULL;
	}
}

void *malloc_s(size_t size)
{
	void *p = malloc(size);

	if (p == NULL)
	{
		printf("Dynamic memory allocation error. Program terminated.\n");
		exit(-1);
	}

	return p;
}
