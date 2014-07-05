#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
	FILE * ifile;
	FILE * ofile;
	char buff[256];
	char str[256];
	int stop = 0;

	if(argc < 2)
	{
		printf("Usage: mget_imei <serial_device>\n");
		return 1;
	}
	ifile = fopen(argv[1], "r");
	ofile = fopen(argv[1], "a+");
	fputs("AT+CGSN\r",ofile);
	if(ifile == NULL || ofile == NULL)
	{
		printf("Couldn't open device file.\n");
		return 1;
	}
	while(ifile && stop == 0)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"AT+CGSN",7))
		{
			fgets(buff,256,ifile);
			strncpy(str,buff+strlen(buff)-6,4);
			printf("%s",str);
			stop = 1;
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return 0;
}
