#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC 0xCBFA
#define VERSION 0x0001
#define EXTENSION ".sfa"

#define LENGTH 256
#define BUFFERSIZE 1048576

#pragma pack(1)


typedef struct _archiveHeader {
	uint16_t magic;
	uint16_t version;
	uint32_t fileNumber;
	uint64_t archiveSize;
} archiveHeader, *ParchiveHeader;

typedef struct _fileHeader {
	uint8_t fileName[LENGTH];
	uint64_t fileSize;
	uint64_t fileOffset;
} fileHeader, *PfileHeader;

typedef struct _node {
	fileHeader elem;
	struct _node *next;
} node, *Pnode;

typedef struct _archive {
	FILE *fp;
	archiveHeader header;
	Pnode fileHeaderList;
	int isFA;
} archive, *Parchive;


/* main.c */
int run(void);
int getInput(char *input, int limit);
int stdinFlush(void);


/* archive.c */
Parchive createFA(char *archiveName);
Parchive openFA(char *archiveName);
int enlistFA(Parchive FA);
int appendFA(Parchive FA, char *fileName);
int extractFA(Parchive FA, char *fileName);
int removeFA(void);
int closeFA(Parchive FA);


/* list.c */
Pnode Linsert(Pnode list, fileHeader newElem);
Pnode Lsearch(Pnode list, char *searchName);
int Lfree(Pnode list);


/* control.c */
int isFile(char *fileName);
int fcopy(FILE *dst, size_t size, FILE *src);
uint64_t getFileSize(FILE *fp);
FILE *fopen_s(const char *fileName, const char *mode);
void *malloc_s(size_t size);
