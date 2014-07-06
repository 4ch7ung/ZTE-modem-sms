#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void error(const char* message)
{
	printf("%s\n", message);
	exit(1);
}

int main(int argc, char * argv[])
{
	FILE * ifile;
	FILE * ofile;
	char buff[256];
	char str[256];

	if(argc < 2)
		error("Usage: minit_session <serial_device>");

	ifile = fopen(argv[1], "r");
	ofile = fopen(argv[1], "a+");
	if(ifile == NULL)
		error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		error("Couldn't open device file for writing.");
	}

	fputs("AT+CMGF=1\r",ofile);
	printf("AT+CMGF=1 ... ");
	while(ifile)
	{
		fgets(buff, 256, ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("OK!\n");
			break;
		}
	}
	
	fputs("AT+CPMS=\"SM\",\"SM\",\"SM\"\r", ofile);
	printf("AT+CPMS=\"SM\",\"SM\",\"SM\" ... ");
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("OK!\n");
			break;
		}
	}

	fputs("AT+CNMI=1,2,0,0,0\r",ofile);
	printf("AT+CNMI=1,2,0,0,0 ... ");
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("OK!\n");
			break;
		}
	}

	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return 0;
}
