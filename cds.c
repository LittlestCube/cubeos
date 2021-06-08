#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char rom[16];

int loadRom(char* path)
{
	FILE *fileptr;
	long filelen;
	
	fileptr = fopen(path, "rb");
	fseek(fileptr, 0, SEEK_END);
	filelen = ftell(fileptr);
	rewind(fileptr);
	
	fread(rom, filelen, 1, fileptr);
	fclose(fileptr);
	
	return filelen; // return length of file
}

char* removeExt(char* path)
{
	char *retStr = malloc(strlen(path) + 3);
	
	strcpy(retStr, path);
	
	char *lastExt = strrchr(retStr, '.');
	
	if (lastExt != NULL)
	{
		*lastExt = '\0';
	}
	
	return retStr;
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		char *outPath = removeExt(argv[1]);
		
		strcat(outPath, ".s");
		
		FILE *outFile = fopen(outPath, "w");
		
		if (outFile == NULL)
		{
			printf("E: file '%s' can't be opened\n", outPath);
			exit(1);
		}
		
		int filelen = loadRom(argv[1]);
		
		fprintf(outFile, "main:\n");
		
		for (int i = 0; i < filelen;)
		{
			switch (rom[i])
			{
				case 0x00:
				{
					fprintf(outFile, "  ADD A, $%02X\t; Add 0x%02X to the A register.\n", rom[i + 1], rom[i + 1]);
					i += 2;
					
					break;
				}
				
				case 0x01:
				{
					fprintf(outFile, "  LD  A, $%02X\t; Load 0x%02X into the A register.\n", rom[i + 1], rom[i + 1]);
					i += 2;
					
					break;
				}
				
				case 0x99:
				{
					fprintf(outFile, "  END\t\t\t; End the program\n");
					i += 1;
					
					break;
				}
			}
		}
	}
	
	else
	{
		printf("E: no rom\n");
	}
}