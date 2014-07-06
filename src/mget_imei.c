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
		error("Usage: mget_imei <serial_device>");
	
	ifile = fopen(argv[1], "r");
	ofile = fopen(argv[1], "a+");
	if(ifile == NULL)
		error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		error("Couldn't open device file for writing.");
	}
	
	fputs("AT+CGSN\r",ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"AT+CGSN",7))
		{
			fgets(buff,256,ifile);
			strncpy(str,buff+strlen(buff)-6,4);
			printf("%s\n",str);
			break;
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return 0;
}
