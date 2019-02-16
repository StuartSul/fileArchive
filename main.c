#include "archive.h"

int main(int argc, char *argv[])
{
	if (argc == 1)
		run();
	else
		printf("Usage: %s (no options)", argv[0]);
	
	return 0;
}

int run(void)
{
	int action;
	char input[LENGTH];
	Parchive FA;

	printf("\n________Version 1.00_________\n");
	printf("Welcome to the file archiver.\n\n");

	while (1)	
	{
		printf("What would you like to do?\n");
		printf("1. Create a new archive.\n");
		printf("2. Open an existing archive.\n");
		printf("3. Terminate the program.\n");
		printf("Enter the number: ");

		action = fgetc(stdin) - '0';
		stdinFlush();

		if (action == 1)
		{
			printf("Creating new archive . . .\n\n");
			printf("Please enter the name of new archive, less than %d characters: ", LENGTH - 6);

			getInput(input, LENGTH - 6);

			printf(". . .\n");

			strcat(input, EXTENSION);

			FA = createFA(input);
		}
		else if (action == 2)
		{
			printf("Opening an archive . . .\n\n");
			printf("Please enter the name of the archive, including the extension: ");

			getInput(input, LENGTH - 6);

			printf(". . .\n");

			if (isFile(input))
			{
				FA = openFA(input);
				
				if (FA->isFA == 0)
				{
					printf("The file is not an archive, or is corrupted.\n\n");
					continue;
				}
			}
			else
			{
				printf("The file does not exist, or you do not have the access to it.\n\n");
				continue;
			}
		}
		else if (action == 3)
		{
			printf("Program terminated.\n\n");
			exit(0);
		}
		else
		{
			printf("Wrong syntax.\n\n");
			continue;
		}

		printf("\n");

		while (1)
		{
			printf("The file archive is now created/opened. What would you like to do?\n");
			printf("1. Enlist all the files in the archive.\n");
			printf("2. Append a file to the archive.\n");
			printf("3. Extract a file from the archive.\n");
			printf("4. Remove a file from the archive.\n");
			printf("5. Close the archive.\n");
			printf("6. Terminate the program. (Archive may not be saved)\n");
			printf("Enter the number: ");

			action = fgetc(stdin) - '0';
			stdinFlush();

			if (action == 1)
			{
				printf("Enlisting files in the archive . . .\n\n");

				enlistFA(FA);

				printf("\n\n");

				continue;
			}
			else if (action == 2)
			{
				printf("Appending a file to the archive . . .\n\n");
				printf("Please enter the name of file to be added to the archive, ");
				printf("including the extension. If the file is in the current directory, ");
				printf("absolute path is not needed: ");

				getInput(input, LENGTH - 6);

				printf(". . .\n");

				if (isFile(input))
				{
					appendFA(FA, input);
					printf("File successfully appended.\n\n");
				}
				else
				{
					printf("The file does not exist, or you do not have the access to it.\n\n");
				}

				continue;
			}
			else if (action == 3)
			{
				printf("Extracting a file from the archive . . .\n\n");
				printf("Please enter the name of file to be extracted from the ");
				printf("archive, including the extension. ");
				printf("To extract all the files, enter \".all\": ");

				getInput(input, LENGTH - 6);

				printf(". . .\n");
				
				if (!strcmp(".all", input))
				{
					Pnode tempNode;

					for (tempNode = FA->fileHeaderList;
					tempNode != NULL; tempNode = tempNode->next)
						extractFA(FA, (char*)(tempNode->elem.fileName));

					printf("All files successfully extracted\n\n");
				}
				else if (extractFA(FA, input))
					printf("File successfully extracted.\n\n");
				else
					printf("The file does not exist.\n\n");

				continue;
			}
			else if (action == 4)
			{
				printf("Removing a file is not supported yet.\n\n");
				removeFA();
				continue;
			}
			else if (action == 5)
			{
				printf("Closing the archive . . .\n\n");

				closeFA(FA);

				printf("Archive closed.\n\n");

				break;
			}
			else if (action == 6)
			{
				printf("Program terminated.\n\n");
				exit(0);
			}
			else
			{
				printf("Wrong syntax.\n\n");
				continue;
			}
		}
	}
}

int getInput(char *input, int limit)
{
	char c;
	int i = 0;

	memset(input, '\0', LENGTH);

	while ((i < limit) && (c = fgetc(stdin)) != '\n')
		input[i++] = c;

	return 1;
}

int stdinFlush(void)
{
	char c;
	while ((c = fgetc(stdin)) != '\n' && c != EOF);
	return 1;
}
