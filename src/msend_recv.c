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
	char imei[5];
	
	if(argc < 4)
		error("Usage: msend_recv <serial_device> <dst_phone> <message>");
	
	strncpy(imei,argv[1]+strlen(argv[1])-4,4);
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
		printf("[INFO] %s: %s", imei, buff);
		if(!strncmp(buff,"AT+CMGS",7))
			break;
	}
	sprintf(str,"%s\032", argv[3]);
	fputs(str,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"+CMGS",5))
		{
			printf("[INFO] %s: +CMGS ... ", imei);
			break;
		}
		if(!strncmp(buff,"+CMS ERROR",10))
		{
			printf("[ERROR] %s: %s", imei, buff);
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
		if(!strncmp(buff,"+CMS ERROR",10))
		{
			printf("[ERROR] %s: %s", imei, buff);
			break;
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return 0;
}
