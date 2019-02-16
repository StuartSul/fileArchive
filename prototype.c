#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC 0xCBFA
#define BUFFERSIZE 1048576

#pragma pack(1)

typedef struct _archiveHeader {
	uint16_t magic;
	uint16_t version;
	uint32_t fileNumber;
	uint64_t archiveSize;
} archiveHeader, *ParchiveHeader;

typedef struct _fileHeader {
	char fileName[256];
	uint64_t fileSize;
	uint64_t fileOffset;
} fileHeader, *PfileHeader;

typedef struct _node {
	fileHeader elem;
	struct _node *next;
} node, *Pnode;

Pnode Linsert(Pnode list, fileHeader new)
{
	Pnode newNode = malloc(sizeof(node));
	newNode->elem = new;
	newNode->next = list;
	return newNode;
}

Pnode Lsearch(Pnode list, char *searchName)
{
	Pnode temp;

	for (temp = list; temp != NULL; temp = temp->next)
		{
			if (!strcmp(temp->elem.fileName, searchName))
				return temp;
		}

	return NULL;
}

int Lfree(Pnode list)
{
	Pnode temp = list;

	while((list = temp))
	{
		temp = list->next;
		free(list);
	}

	return 1;
}

int main(void)
{
	archiveHeader Ver1;
	Ver1.magic = MAGIC;
	Ver1.version = 0x0100;
	Ver1.fileNumber = 0;
	printf("The program will open test.txt\n");

	FILE *fp = fopen("test.txt", "rb");

	if (fp)
		printf("File successfully opened!\n");
	else
		return -1;
	
	printf("New file archive will be created in current directory\n");

	FILE *fa = fopen("FileArchive.sfa", "wb+");

	if (fa)
		printf("File Archive successfully created!\n");
	else
		return -1;

	printf("File archive header will be added to the archive\n");

	fwrite(&Ver1, sizeof(Ver1), 1, fa);

	printf("Now the program will add test.txt to the file archive\n");

	fileHeader newfile;
	
	memset(newfile.fileName, 0, sizeof(newfile.fileName));

	strcpy(newfile.fileName, "test.txt");
	
	fseek(fp, 0L, SEEK_END);

	newfile.fileSize = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	
	newfile.fileOffset = sizeof(Ver1) + sizeof(newfile);

	printf("File header has beeen created. Now it will be added to the archive\n");

	fwrite(&newfile, sizeof(newfile), 1, fa);

	printf("Now the file will be added to the archive.\n");

	void *buffer = malloc(BUFFERSIZE);
	size_t num;

	while((num = fread(buffer, 1, BUFFERSIZE, fp)))
		fwrite(buffer, 1, num, fa);	

	Ver1.fileNumber++;

	printf("Writing done.\n");

	fclose(fp);

	printf("Linked list of file headers will be created\n");

	Pnode Hlist = NULL;

	Hlist = Linsert(Hlist, newfile);

	printf("Now the program will add test2.txt to the archive\n");

	fp = fopen("test2.txt", "rb");

	memset(newfile.fileName, 0, sizeof(newfile.fileName));
	strcpy(newfile.fileName, "test2.txt");
	fseek(fp, 0L, SEEK_END);
	newfile.fileSize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	newfile.fileOffset = sizeof(archiveHeader) + sizeof(fileHeader) * (Ver1.fileNumber + 1) + Hlist->elem.fileSize;
	fwrite(&newfile, sizeof(newfile), 1, fa);

	while ((num = fread(buffer, 1, BUFFERSIZE, fp)))
		fwrite(buffer, 1, num, fa);

	Ver1.fileNumber++;

	Hlist = Linsert(Hlist, newfile);

	printf("Writing done.\n");

	printf("Program will close the file archive file\n");

	Ver1.archiveSize = ftell(fa);

	fseek(fa, 0L, SEEK_SET);
	fwrite(&Ver1, sizeof(Ver1), 1, fa);

	fclose(fa);
	Lfree(Hlist);

	printf("Archive closed. Now the program will re-open the archive.\n");

	fa = fopen("FileArchive.sfa", "rb+");

	printf("The program will check if the magic number and version matches\n");

	archiveHeader header;
	if (fread(&header, sizeof(header), 1, fa))
	{
		printf("The header from the file was read into the memory\n");
		if (header.magic == MAGIC)
			printf("The magic number matches.\n");
		else
			return -1;

		if (header.version == 0x0100)
			printf("The version matches.\n");
		else
			return -1;
	}
	else
		return -1;

	printf("There are %d files in this archive.\n", header.fileNumber);

	printf("The program will create a linked list of all the file headers\n");

	Hlist = NULL;

	while(ftell(fa) < header.archiveSize)
	{
		fread(&newfile, sizeof(newfile), 1, fa);

		Hlist = Linsert(Hlist, newfile);

		fseek(fa, newfile.fileSize, SEEK_CUR);
	}

	printf("The program will enlist all the file names: \n");

	printf("%s %s\n", Hlist->elem.fileName, Hlist->next->elem.fileName);

	printf("The program will extract test2.txt as \"testextracted.txt\"\n");

	Pnode tempnode;

	if ((tempnode = Lsearch(Hlist, "test2.txt")) != NULL)
		printf("test2.txt was found in the linked list\n");
	else
		return -1;

	FILE *newfp = fopen("testextracted.txt", "wb+");

	fseek(fa, tempnode->elem.fileOffset, SEEK_SET);

	uint64_t sizetemp = tempnode->elem.fileSize;

	while ((num = fread(buffer, 1, BUFFERSIZE, fa)))
	{
		if (num >= sizetemp)
		{
			fwrite(buffer, 1, sizetemp, newfp);
			break;
		}
		else if (num < sizetemp)
		{
			fwrite(buffer, 1, num, newfp);
			sizetemp -= num;
		}
	}

	fclose(newfp);

	printf("Successfully extracted!\n");


	return 0;
}
