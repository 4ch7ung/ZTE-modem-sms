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
	char cmd[256];
	char msg[1000];
	char *pch;
	char rphone[256];
	char imei[5];
	
	if(argc < 2)
		error("Usage: msend_recv <serial_device>");
	
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
// Start of cmd send	
	sprintf(cmd,"AT+CMGL=\"ALL\"\r");
	fputs(cmd,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("Done\n");
			break;
		}
		if(!strncmp(buff,"+CMGL:",6))
		{
			pch = strtok(buff,"\",");
			printf("[INFO] %s:", imei);
			while(pch != NULL)
			{
				if(strcmp(pch,"") && strncmp("REC",pch,3))
					printf(" %s", pch);
				pch = strtok(NULL, "\",");
			}
			fgets(msg,1000,ifile);
			printf("{SIZE} %lu\n", strlen(msg));
			msg[strlen(msg)/4 * 4] = 0;
			printf("{NEW SIZE} %lu\n", strlen(msg));
			printf("{MSG} %s\n",msg);
		}
		if(!strncmp(buff,"+CMS ERROR",10))
		{
			printf("[ERROR] %s: %s", imei, buff);
			exit(1);
		}
	}
// End of cmd send
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return 0;
}
