#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

FILE* loadFile(char* path)
{
	FILE* fileptr = fopen(path, "r");
	
	return fileptr; // return the file
}

char* removeExt(char* path)
{
	char* retStr = malloc(strlen(path) + 5);
	
	strcpy(retStr, path);
	
	char* lastExt = strrchr(retStr, '.');
	
	if (lastExt != NULL)
	{
		*lastExt = '\0';
	}
	
	return retStr;
}

int main(int argc, char** argv)
{
	FILE* inFile;
	
	if (argc > 1)
	{
		inFile = loadFile(argv[1]);
	}
	
	else
	{
		puts("E: no file inputted");
		exit(1);
	}
	
	char* outPath = strcat(removeExt(argv[1]), ".cub");
	FILE* outFile = fopen(outPath, "wb");
	
	char binary[16];
	int binaryIndex = 0;
	
	char* line = malloc(80);
	
	char pointerName[64];
	pointerName[0] = '\0';
	
	while (fgets(line, 80, inFile))
	{
		char mnem[30];
		mnem[0] = '\0';
		
		char reg[30];
		reg[0] = '\0';
		
		int value = -1;
		
		for (int i = 0; i < 80; i++)
		{
			switch (line[i])
			{
				case ':':
				{
					// Trim leading space
					int begIndex = 0;
					while (isspace((unsigned char) line[begIndex])) begIndex++;
					
					strncpy(pointerName, line + begIndex, i - begIndex);
					
					// Trim trailing space
					
					int endIndex = i;
					while(isspace((unsigned char) line[endIndex])) endIndex--;
					
					strncpy(pointerName, line + begIndex, endIndex - i);
					
					puts(pointerName);
					
					break;
				}
				
				case '\t':
				case ' ':
				{
					break;  // go to the next character
				}
				
				case '\n':
				case ';':
				{
					i = 80; // go to the next line you freak
					
					break;
				}
				
				default:
				{
					char* offset = line + i;
					
					size_t nextSpace = (size_t) (strchr(offset, ' ') - offset);
					size_t nextTab = (size_t) (strchr(offset, '\t') - offset);
					size_t nextSColon = (size_t) (strchr(offset, ';') - offset);
					size_t nextNewLine = (size_t) (strchr(offset, '\n') - offset);
					size_t eof = (size_t) (strchr(offset, '\0') - offset);
					// this is eof because it will come before a newline
					
					size_t endOfKey;
					
					// now we figure out which character came first
					endOfKey = MIN(nextSpace, nextTab);
					endOfKey = MIN(endOfKey, nextNewLine);
					endOfKey = MIN(endOfKey, nextSColon);
					endOfKey = MIN(endOfKey, eof);
					
					if (endOfKey < 10)
					{
						char keyword[10];
						
						strncpy(keyword, offset, endOfKey);
						
						keyword[endOfKey] = '\0';
						
						for (int i = 0; i < endOfKey; i++)
						{
							keyword[i] = tolower(keyword[i]);
						}
						
						if (mnem[0] == '\0')
						{
							strncat(mnem, keyword, endOfKey);
						}
						
						else if ((reg[0] == '\0') && (keyword[0] != '$'))
						{
							strncat(reg, keyword, 1);
						}
						
						else if (value == -1)
						{
							sprintf(keyword, "%c%c%c", offset[1], offset[2], '\0');
							
							value = (int) strtol(keyword, NULL, 16);
							
							i = 80;
						}
						
						i += endOfKey;
					}
					
					break;
				}
			}
		}
		
		if (strcmp(mnem, "add") == 0)
		{
			if (reg[0] == 'a')
			{
				binary[binaryIndex++] = 0x00;
				binary[binaryIndex++] = (char) value;
			}
		}
		
		else if (strcmp(mnem, "ld") == 0)
		{
			if (reg[0] == 'a')
			{
				binary[binaryIndex++] = 0x01;
				binary[binaryIndex++] = (char) value;
			}
		}
		
		else if (strcmp(mnem, "push") == 0)
		{
			if (reg[0] == 'a')
			{
				binary[binaryIndex++] = 0x02;
			}
			
			else
			{
				binary[binaryIndex++] = 0x04;
				binary[binaryIndex++] = (char) value;
			}
		}
		
		else if (strcmp(mnem, "pop") == 0)
		{
			if (reg[0] == 'a')
			{
				binary[binaryIndex++] = 0x03;
			}
		}
		
		else if (strcmp(mnem, "db") == 0)
		{
			binary[binaryIndex++] = (char) value;
		}
		
		else if (strcmp(mnem, "end") == 0)
		{
			binary[binaryIndex++] = 0x99;
		}
	}
	
	fwrite(binary, binaryIndex, 1, outFile);
}