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
	FILE * savefile;
	char buff[256];
	char cmd[256];
	char msg[256];
	char sphone[256];
	char rphone[256];
	char imei[5];
	char savedir[256];
	
	if(argc < 5)
		error("Usage: msend_recv <serial_device> <dst_phone> <message> <save_dir>");
	
	strncpy(imei,argv[1]+strlen(argv[1])-4,4);
	strcpy(sphone,argv[2]);
	strcpy(savedir,argv[4]);
	ifile = fopen(argv[1], "r");
	ofile = fopen(argv[1], "a+");
	if(ifile == NULL)
		error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		error("Couldn't open device file for writing.");
	}
// Start of SMS send	
	sprintf(cmd,"AT+CMGS=\"%s\"\r",sphone);
	fputs(cmd,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		printf("[INFO] %s: %s", imei, buff);
		if(!strncmp(buff,"AT+CMGS",7))
			break;
	}
	sprintf(msg,"%s\032",argv[3]);
	fputs(msg,ofile);
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
			printf("[ERROR] %s: %s", imei, buff);
			exit(1);
		}
	}
// End of SMS send
// Start of SMS wait
	printf("[INFO] %s: Waiting for SMS reaponse ... ", imei);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"+CMT",4))
		{
			strncpy(rphone,buff+7,strlen(sphone));
			if(strcmp(rphone,sphone))
			{
				printf("NO\n[INFO] %s: Recieved message from %s\n", imei, rphone);
				fgets(buff,256,ifile);
				printf("\t%s",buff);
			}
			else
			{
				printf("YES\n[INFO] %s: Received message from %s\n", imei, rphone);
				fgets(buff,256,ifile);
				printf("\t%s",buff);
				//buff[strlen(buff)-2] = 0;
				strcat(savedir,"/");
				strcat(savedir,imei);
				savefile = fopen(savedir, "w+");
				if(savefile)
				{
					fputs(buff,savefile);
					fclose(savefile);
					break;
				}
				else
				{
					printf("[ERROR] %s: Fail to open save file for writing\n",imei);
					break;
				}
			}
		}
	}
// End of SMS wait
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return 0;
}
