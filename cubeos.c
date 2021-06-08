#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned char memory[16];

unsigned char pc;
unsigned char sp;

unsigned char a;

void loadRom(char* path)
{
	FILE *fileptr;
	char *ROM;
	long filelen;
	
	fileptr = fopen(path, "rb");
	fseek(fileptr, 0, SEEK_END);
	filelen = ftell(fileptr);
	rewind(fileptr);
	
	ROM = (char *) malloc(filelen * sizeof(char));
	fread(ROM, filelen, 1, fileptr);
	fclose(fileptr);
	
	for (int i = 0; i < filelen; i++)
	{
		memory[i] = ROM[i];
	}
}

void push(unsigned char pushValue)
{
	memory[--sp] = pushValue;
}

unsigned char pop()
{
	return memory[sp++];
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		loadRom(argv[1]);
	}
	
	else
	{
		puts("E: no rom");
		exit(1);
	}
	
	pc = 0;
	sp = 15;
	a = 0;
	
	while (true)
	{
		switch (memory[pc])
		{
			case 0x00:
			{
				a += memory[pc + 1];
				pc += 2;
				
				break;
			}
			
			case 0x01:
			{
				a = memory[pc + 1];
				pc += 2;
				
				break;
			}
			
			case 0x02:
			{
				push(a);
				pc += 1;
				
				break;
			}
			
			case 0x03:
			{
				a = pop();
				pc += 1;
				
				break;
			}
			
			case 0x04:
			{
				push(memory[pc + 1]);
				pc += 2;
				
				break;
			}
			
			case 0x99:
			{
				printf("%d\n", a);
				exit(0);
				
				break;
			}
			
			default:
			{
				printf("E: unexpected opcode %02X at pc %02X\n", memory[pc], pc);
				exit(1);
				
				break;
			}
		}
	}
}