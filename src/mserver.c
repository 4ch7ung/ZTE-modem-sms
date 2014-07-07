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
	char msg[256];
	char phone[256];
	int stop = 0;
	
	if(argc < 2)
		error("Usage: mserver <serial_device>");
	
	ifile = fopen(argv[1], "r");
	ofile = fopen(argv[1], "a+");
	if(ifile == NULL)
		error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		error("Couldn't open device file for writing.");
	}
	
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"+CMT",4))
		{
			strncpy(phone,buff+7,12);
			printf("[INFO] %s: Message from %s\n", argv[1], phone);
			fgets(buff,256,ifile);
			buff[strlen(buff)-2] = 0;
			printf("[INFO] %s: %s\n", argv[1], buff); 
			sprintf(cmd,"AT+CMGS=\"%s\"\r",phone);
			fputs(cmd,ofile);
			while(ifile)
			{
				fgets(buff,256,ifile);
				if(!strncmp(buff,"AT+CMGS",7))
					break;
			}
			sprintf(cmd,"%s\032","Some tea");
			fputs(cmd,ofile);
			while(ifile)
			{
				fgets(buff,256,ifile);
				if(!strncmp(buff,"+CMGS",5))
				{
					printf("[INFO] %s: +CMGS %s ", argv[1], phone);
					break;
				}
				if(!strncmp(buff,"+CMS ERROR",10))
				{
					printf("[ERROR] %s: %s", argv[1], buff);
					exit(1);
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
					printf("[ERROR] %s: %s", argv[1], buff);
					exit(1);
				}
			}				
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return 0;
}
