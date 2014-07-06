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
	
	if(argc < 4)
		error("Usage: msend_sms <serial_device> <dst_phone> <message>");
	
	ifile = fopen(argv[1], "r");
	ofile = fopen(argv[1], "a+");
	if(ifile == NULL)
		error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		error("Couldn't open device file for writing.");
	}
	
	sprintf(str, "AT+CMGS=\"%s\"\r", argv[2]);
	fputs(str,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncpy(buff,">",1))
			break;
	}
	sprintf(str,"%s%c",argv[2], char(32));
	fputs(str,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"+CMGS",5))
		{
			printf("%s ... ",buff);
			break;
		}
	}
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
