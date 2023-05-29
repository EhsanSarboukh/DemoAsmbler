#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int is_comand(char* str);// Check if the str is a legal command in asm
void A_Comand(FILE* asembler, char* string);// A instruction
void C_Comand(FILE* asembler, char* string);// C instruction without jmp
void CJMP_Comand(FILE* asembler, char* string);// C instruction with jmp 
char* Space_free(char* string);// removes all the spaces from the string

int main()
{
	FILE* in, * out;
	char file_name[21], file_name_hack[21], line[300],string[300];
	int i, len;
	printf("Enter the name of the file :\n");
	scanf("%s", file_name);// gets the file name from the user. example Name.asm
	if ((in = fopen(file_name, "r")) == NULL)
	{
		printf("The file is not found");
		exit(1);
	}
	len = strlen(file_name) - 5;
	for (i = 0; i <= len; i++)// copying the Name of the file without the ending
	{
		file_name_hack[i] = file_name[i];
	}
	file_name_hack[len + 1] = '\0';
	strcat(file_name_hack, ".hack");// creating a Name.hack file for writing
	if ((out = fopen(file_name_hack, "w+")) == NULL)// in case the file didnt open successfully print a msg
	{
		printf("The file is not found");
		exit(1);
	}
	while (fgets(line, 300, in) != NULL)// gets a string from the input file
	{
		strcpy(string,Space_free(line));// remove space and copying the new string in string
		if (is_comand(string) != 0)
		{
			if (string[0] == '@')
			{
				A_Comand(out, string);
			}
			else
			{
				if (strchr(string, ';') == NULL)//check if the instruction is Cjmp or C 
					C_Comand(out, string);
				else
					CJMP_Comand(out, string);
			}
		}

	}
	fclose(in);
	fclose(out);
	printf("The file %s is now avaliable!", file_name_hack);
}
//remove spaces from the string 
char* Space_free(char* string)
{
	int i = 0, j = 0;
	char Restring[300];
	*Restring = '\0';
	while (string[i] != '\0')
	{
		if (string[i] != ' ')
		{
			Restring[j] = string[i];
			j++;
		}
		i++;
	}
	Restring[j] = '\0';
	return Restring;
}
// check if the command is legal command
int is_comand(char* str)
{
	int len, i = 0;
	if ((str[i] == '/' && str[i + 1] == '/') || (str[i] == '/' && str[i + 1] == '*'))
		return 0;
	if (str[i] == '\0' || str[i] == '\n'|| str[i]=='(')
		return 0;
	if (str[i] == ' ')
		return 0;
	return 1;
}
// Convert A instruction command into bits 
void A_Comand(FILE* asembler, char* string)
{
	char* binary = '\0';
	char num[15];
	int numvalue = 0;
	int binaryx = 0, mul = 1, rem;
	int count = 0;
	int i = 1, j = 0;
	while (string[i] != '\0')
	{
		if (string[i] >= '0' && string[i] <= '9')
			num[j] = string[i];
		i++;
		j++;
	}
	num[i] = '\0';
	numvalue = atoi(num);
	if (numvalue == 0)
	{
		fputs("0000000000000000",asembler);
		fputs("\n", asembler);
	}
	else
	{
		while (numvalue > 0)// Convert a decimal number into binary
		{
			rem = numvalue % 2;
			binaryx = binaryx + (rem * mul);
			mul = mul * 10;
			numvalue = numvalue / 2;
			count++;
		}
		count = 16 - count;
		for (i = 0; i < count; i++)
		{
			fputs("0", asembler);
		}
		fprintf(asembler, "%d", binaryx);
		fputs("\n", asembler);
	}
}
//Convert C instruction without JMP into bits
void C_Comand(FILE* asembler, char* string)
{
	char* dest[8] = { "","M=","D=","MD=","A=","AM=","AD=","AMD=" };
	char* Bdest[8] = { "000","001","010","011","100","101","110","111" };
	char* comp[28] = { "0","1","-1","D","A","!D","!A","-D","-A","D+1","A+1","D-1","A-1","D+A","D-A","A-D",
		"D&A","D|A","M","!M","-M","M+1","M-1","D+M","D-M","M-D","D&M","D|M" };
	char* Bcomp[28] = { "0101010","0111111","0111010","0001100","0110000","0001101","0110001","0001111","0110011","0011111",
	"0110111","0001110","0110010","0000010","0010011","0000111","0000000","0010101","1110000","1110001",
	"1110011","1110111","1110010","1000010",
	"1010011","1000111","1000000","1010101" };
	
	int index, i, j, k;
	char binarydest[5], binarycomp[8];
	char cins[17] = "111";
	char xdest[5], xcomp[5];
	if (index = strstr(string, "//") != NULL)
	{
		i = 0;
		while (string[i] != '/')
			i++;
		string[i] = '\0';
	}
	j = 0;
	i = 0;
	while (string[j] != '=')
	{

		xdest[i] = string[j];
		i++;
		j++;

	}
	xdest[i] = string[j];
	xdest[i+1] = '\0';
	j = 0;
	while ((strcmp(xdest, dest[j]) != 0) && i < 8)
		j++;
	strcpy(binarydest, Bdest[j]);
	k = 0;
	for (int j = i + 1; j < strlen(string); j++)
	{
		if((string[j] != '\n')&&(string[j]!=' '))
		{
			xcomp[k] = string[j];
			k++;
		}
	}
	xcomp[k] = '\0';
	i = 0;
	while ((strcmp(xcomp, comp[i]) != 0) && i < 28)
		i++;
	// Comblining all the strings togather in order to print a C ins command in bits
	strcpy(binarycomp, Bcomp[i]);
	strcat(cins, binarycomp);
	strcat(cins, binarydest);
	strcat(cins, "000");
	fputs(cins, asembler);
	fputs("\n", asembler);
}
// Convert C instruction with JMP 
void CJMP_Comand(FILE* asembler, char* string)
{
	char* dest[8] = { "","M=","D=","MD=","A=","AM=","AD=","AMD=" };
	char* Bdest[8] = { "000","001","010","011","100","101","110","111" };
	char* comp[28] = { "0","1","-1","D","A","!D","!A","-D","-A","D+1","A+1","D-1","A-1","D+A","D-A","A-D",
		"D&A","D|A","M","!M","-M","M+1","M-1","D+M","D-M","M-D","D&M","D|M" };
	char* Bcomp[28] = { "0101010","0111111","0111010","0001100","0110000","0001101","0110001","0001111","0110011","0011111",
	"0110111","0001110","0110010","0000010","0010011","0000111","0000000","0010101","1110000","1110001",
	"1110011","1110111","1110010","1000010",
	"1010011","1000111","1000000","1010101" };
	char* jmp[8] = { "","JGT","JEQ","JGE","JLT","JNE","JLE","JMP" };
	char* Bjmp[8] = { "000","001","010","011","100","101","110","111" };
	int noNeed;
	char xjmp[8], binaryjmp[4], xdest[5], xcomp[5];
	char binarydest[5], binarycomp[8];
	int i = 0, j = 0;
	char finalres[17] = "111\0";
	if ((noNeed = strstr(string, "//")) != NULL)
	{
		while (string[i] != '/')
			i++;
		string[i] = '\0';
		i = 0;
	}
	if (strchr(string, '=') != NULL)
	{
		i = 0;
		while (string[j] != '=')
		{
			if ((string[j] != '\n') && (string[j] != ' '))
			{
				xdest[i] = string[j];
				i++;
			}
			j++;
		}
		xdest[i] = string[j];
		j++;
		xdest[i + 1] = '\0';
		i = 0;
		while ((strcmp(xdest, dest[i]) != 0) && i < 8)
			i++;
		strcpy(binarydest, Bdest[i]);
	}
	else {
		strcpy(binarydest, Bdest[i]);
	}

	i = 0;
	while (string[j] != ';')
	{
		xcomp[i] = string[j];
		j++;
		i++;
	}
	xcomp[i] = '\0';
	i = 0;
	while ((strcmp(xcomp, comp[i]) != 0) && i < 28)
		i++;
	strcpy(binarycomp, Bcomp[i]);
	i = 0;
	j++;
	while (string[j] != '\0')
	{
		if (string[j] != '\n')
		{
			xjmp[i] = string[j];
			i++;
		}
		j++;
	}
	xjmp[i] = '\0';
	i = 0;
	while ((strcmp(xjmp, jmp[i]) != 0) && i < 8)
		i++;
	// Comblining all the strings togather in order to print a C ins command in bits
	strcpy(binaryjmp, Bjmp[i]);
	strcat(finalres, binarycomp);
	strcat(finalres, binarydest);
	strcat(finalres, binaryjmp);
	fputs(finalres, asembler);
	fputs("\n", asembler);
}
