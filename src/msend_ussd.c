#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>

void error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void convertUTF16BE_to_UTF8(const char *instr, char *outstr)
{
	char buff[4];
	int i;
	int j;
	int k;
	int instrlen;
	int outchar;
	
	instrlen = strlen(instr);
	if(instrlen % 4)
	{
		printf("Seems like a mistake, string has Odd number of symbols\n");
		outstr[0] = 0;
		return;
	}
	k = 0;
	for(i = 0; i < instrlen; i+=4)
	{
		for(j = 0; j < 4; ++j)
			buff[j] = instr[i+j];
		outchar = 0;
		if(buff[2] >= '0' && buff[2] <= '9') outchar += (buff[2]-'0')*16;
		if(buff[2] >= 'A' && buff[2] <= 'F') outchar += (buff[2]-'A' + 10)*16;
		if(buff[3] >= '0' && buff[3] <= '9') outchar += (buff[3]-'0');
		if(buff[3] >= 'A' && buff[3] <= 'F') outchar += (buff[3]-'A' + 10);
		outstr[k++] = (char)outchar;
	}
	outstr[k] = 0;
}

int main(int argc, char * argv[])
{
	FILE * ifile;
	FILE * ofile;
	FILE * savefile;
	char buff[1000];
	char cmd[256];
	char * msg;
	char ussd[256];
	char rphone[256];
	char imei[5];
	char savedir[256];
	iconv_t conv_dc;
	
	if(argc < 3)
		error("Usage: msend_recv <serial_device> <ussd>");
	
	strncpy(imei,argv[1]+strlen(argv[1])-4,4);
	strcpy(ussd,argv[2]);
	ifile = fopen(argv[1], "r");
	ofile = fopen(argv[1], "a+");
	if(ifile == NULL)
		error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		error("Couldn't open device file for writing.");
	}
// Start of USSD send	
	sprintf(cmd,"AT+CUSD=1,%s,15\r",argv[2]);
	fputs(cmd,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
// 		printf("[INFO] %s: %s", imei, buff);
		if(!strncmp(buff,"AT+CUSD",7))
			break;
		if(!strncmp(buff,"ERROR",10))
		{
			printf("[ERROR] %s: %s", imei, buff);
			exit(1);
		}
	}
// End of USSD send
// Start of USSD wait
	printf("[INFO] %s: Waiting for USSD response ... \n", imei);
	while(ifile)
	{
		fgets(buff,1000,ifile);
		if(!strncmp(buff,"+CUSD",5))
		{
			strtok(buff,"\"");
			msg = strtok(NULL, "\"");
			//convertUTF16BE_to_UTF8(msg, cmd);
			printf("[INFO] %s: %s\n", imei, msg);
			break;
		}
	}
// End of USSD wait
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return 0;
}
