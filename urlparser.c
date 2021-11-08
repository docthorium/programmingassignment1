#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "version.h"

static char version[] = "1.2";

// Called when there is a formatting error in the input
void formatError(char c, int counter)
{
	printf("Invalid URL due to invalid character %c at position %d\n", c, counter);
}

int main(int argc, char *argv[])
{
	displayVersion(version);
	char* url = argv[1];
	int counter = 0;
	bool protocolError;

	while (url[counter] != ':')
	{
		if (counter > 4)
		{
			formatError(url[counter], counter);
			return 1;
		}
		counter++;
	}

	char protocol[counter + 1];
	protocol[counter] = '\0';

	for (int i = 0; i < counter; i++)
	{
		protocol[i] = url[i];
	}
	
	protocolError = strcmp(protocol, "http") != 0 && strcmp(protocol, "https") != 0 &&
		strcmp(protocol, "ftp") != 0 && strcmp(protocol, "ftps") != 0;

	for (int i = 0; i < 2; i++)
	{
		counter++;
		if (url[counter] != '/')
		{
			formatError(url[counter], counter);
			return 1;
		}
	}

	counter++;

	int startDomain1 = counter;

	while (url[counter] != '.')
	{
		if (url[counter] < 97 || url[counter] > 122)
		{
			formatError(url[counter], counter);
			return 1;
		}
		counter++;
	}
	
	int sizeDomain1 = counter - startDomain1;
	char domain1[sizeDomain1 + 1];
	domain1[sizeDomain1] = '\0';

	for (int i = 0; i < sizeDomain1; i++)
	{
		domain1[i] = url[i + startDomain1];
	}

	counter++;

	int startDomain2 = counter;
	
	while (url[counter] != '.')
	{
		if (url[counter] < 97 || url[counter] > 122)
		{
			formatError(url[counter], counter);
			return 1;
		}
		counter++;
	}

	int sizeDomain2 = counter - startDomain2;
	char domain2[sizeDomain2 + 1];
	domain2[sizeDomain2] = '\0';

	for (int i = 0; i < sizeDomain2; i++)
	{
		domain2[i] = url[i + startDomain2];
		//printf("%s\n", domain2);
	}

	counter++;

	int starttld = counter;

	while (url[counter] != '/' && url[counter] != ':')
	{
		if (url[counter] < 97 || url[counter] > 122)
		{
			formatError(url[counter], counter);
			return 1;
		}
		counter++;
	}

	int sizetld = counter - starttld;
	char tld[sizetld + 1];
	tld[sizetld] = '\0';

	for (int i = 0; i < sizetld; i++)
	{
		tld[i] = url[i + starttld];
	}

	bool tldError = strcmp(tld, "com") != 0 && strcmp(tld, "net") != 0 &&
	       	strcmp(tld, "biz") != 0 && strcmp(tld, "edu") != 0 && strcmp(tld, "gov") != 0;

	bool hasPort = url[counter] == ':';
	int startPort = counter + 1;

	if (hasPort)
	{
		counter++;
		while (url[counter] != '/')
		{
			if (url[counter] < 48 || url[counter] > 57)
			{
				formatError(url[counter], counter);
				return 1;
			}
			counter++;
		}
	}

	int portSize = counter - startPort;
	char port[portSize + 1];
	if (hasPort)
		port[portSize] = '\0';

	for (int i = 0; i < portSize; i++)
	{
		port[i] = url[i + startPort];
	}

	bool portError = hasPort && atoi(port) < 1 || atoi(port) > 65535;

	bool filePathError = false;
	int startFilePath = counter;

	while (url[counter] != '.')
	{
		if (url[counter + 1] == '?' || url[counter + 1] == '\0')
		{
			filePathError = true;
			break;
		}
		counter++;
	}

	int sizeFilePath = counter - startFilePath;
	char filePath[sizeFilePath + 1];
	filePath[sizeFilePath] = '\0';
	
	for (int i = 0; i < sizeFilePath; i++)
	{
		filePath[i] = url[i + startFilePath];
	}

	counter++;
	int startFileEnding = counter;
	
	while (url[counter] != '\0' && url[counter] != '?')
	{
		counter++;
	}

	int sizeFileEnding = counter - startFileEnding;
	char fileEnding[sizeFileEnding + 1];
	fileEnding[sizeFileEnding] = '\0';

	for (int i = 0; i < sizeFileEnding; i++)
	{
		fileEnding[i] = url[i + startFileEnding];
	}

	filePathError = filePathError || (strcmp(fileEnding, "htm") != 0 && strcmp(fileEnding, "html"));

	bool hasParams = url[counter] == '?';
	counter++;
	int startParams = counter;

	if (hasParams)
	{
		while (url[counter] != '\0')
		{
			counter++;
		}
	}

	int sizeParams = counter - startParams;
	char params[sizeParams + 1];
	params[sizeParams] = '\0';
	
	for (int i = 0; i < sizeParams; i++)
	{
		params[i] = url[i + startParams];
	}
	
	if (!protocolError && !tldError && !portError && !filePathError)
	{
		printf("Protocol:	%s\n", protocol);
		printf("Domain:		%s.%s.%s\n", domain1, domain2, tld);
		if (hasPort)
		{
			printf("Port:		%s\n", port);
		}
		printf("File Path:	%s.%s\n", filePath, fileEnding);
		if (hasParams)
		{
			printf("Paramenters:	%s\n", params);
		}
	} else
	{
		printf("Invalid URL with the following erroneous components:\n");
		if (protocolError)
		{
			printf("Protocol:	%s is not a valid protocol.\n", protocol);
		}
		if (tldError)
		{
			printf("Domain:		%s is not a valid domain name.\n", tld);
		}
		if (portError)
		{
			printf("Port:		Port must be a number between 1 and 65535.\n");
		}
		if (filePathError)
		{
			printf("File Path:	File path must end in .htm or .html.\n");
		}
	}
	return 0;
}

