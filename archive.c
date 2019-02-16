#include "archive.h"

Parchive createFA(char *archiveName)
{
	Parchive FA = (Parchive)malloc_s(sizeof(archive));

	FA->fp = fopen_s(archiveName, "wb+");
	FA->header.magic = MAGIC;
	FA->header.version = VERSION;
	FA->header.fileNumber = 0;
	FA->header.archiveSize = sizeof(archiveHeader);
	FA->fileHeaderList = NULL;
	FA->isFA = 1;

	fwrite(&(FA->header), sizeof(archiveHeader), 1, FA->fp);

	return FA;
}

Parchive openFA(char *archiveName)
{
	Parchive FA = (Parchive)malloc_s(sizeof(archive));
	fileHeader temp;

	FA->fp = fopen_s(archiveName, "rb+");

	if (fread(&(FA->header), sizeof(archiveHeader), 1, FA->fp) != 1)
	{
		FA->isFA = 0;
		return FA;
	}

	if (FA->header.magic != MAGIC)
	{
		FA->isFA = 0;
		return FA;
	}

	FA->fileHeaderList = NULL;

	while (ftell(FA->fp) < FA->header.archiveSize)
	{
		if (fread(&temp, sizeof(fileHeader), 1, FA->fp) != 1)
		{
			FA->isFA = 0;
			return FA;
		}

		FA->fileHeaderList = Linsert(FA->fileHeaderList, temp);

		fseek(FA->fp, temp.fileSize, SEEK_CUR);
	}

	FA->isFA = 1;
	return FA;
}

int enlistFA(Parchive FA)
{
	Pnode temp;
	int i = 1;

	if (FA->header.fileNumber == 0)
	{
		printf("The archive is empty.\n");
		return 1;
	}

	printf("%3s %20s %20s\n", "No.", "File Name", "Size (in bytes)");

	for (temp = FA->fileHeaderList; temp != NULL; temp = temp->next)
	{
		printf("%2d. %20s %20llu\n", i++, temp->elem.fileName, temp->elem.fileSize);
	}

	return 1;
}

int appendFA(Parchive FA, char *fileName)
{
	FILE *fp = fopen_s(fileName, "rb");
	fileHeader tempHeader;
	Pnode tempNode;

	memset(tempHeader.fileName, 0, sizeof(tempHeader.fileName));
	strcpy((char*)tempHeader.fileName, fileName);
	tempHeader.fileSize = getFileSize(fp);
	tempHeader.fileOffset = sizeof(archiveHeader);

	for (tempNode = FA->fileHeaderList; tempNode != NULL; tempNode = tempNode->next)
	{
		tempHeader.fileOffset += sizeof(fileHeader);
		tempHeader.fileOffset += tempNode->elem.fileSize;
	}

	fseek(FA->fp, tempHeader.fileOffset, SEEK_SET);
	tempHeader.fileOffset += sizeof(fileHeader);
	fwrite(&tempHeader, sizeof(fileHeader), 1, FA->fp);
	fcopy(FA->fp, tempHeader.fileSize, fp);
	fclose(fp);	

	FA->header.fileNumber++;
	FA->fileHeaderList = Linsert(FA->fileHeaderList, tempHeader);

	return 1;
}

int extractFA(Parchive FA, char *fileName)
{
	Pnode temp = Lsearch(FA->fileHeaderList, fileName);

	if (temp == NULL)
		return 0;
	
	FILE *fp = fopen_s(fileName, "wb");

	fseek(FA->fp, temp->elem.fileOffset, SEEK_SET);

	fcopy(fp, temp->elem.fileSize, FA->fp);
	
        fclose(fp);

	return 1;
}

int removeFA(void)
{
	// Not implemented yet
	return 0;
}

int closeFA(Parchive FA)
{
	FA->header.archiveSize = getFileSize(FA->fp);

	fseek(FA->fp, 0L, SEEK_SET);

	fwrite(&(FA->header), 1, sizeof(archiveHeader), FA->fp);

	fclose(FA->fp);
	Lfree(FA->fileHeaderList);
	free(FA);

	return 1;
}
