#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"

//-----------------------------------------------------------------------
//
// Remove leading and trailing whitespace from a string.


static char * trimWhiteSpace(char *string)
{
    if (string == NULL)
    {
        return NULL;
    }

    while (isspace(*string))
    {
        string++;
    }

    if (*string == '\0')
    {
        return string;
    }

    char *end = string;

    while (*end)
    {
        ++end;
    }
    --end;

    while ((end > string) && isspace(*end))
    {
        end--;
    }

    *(end + 1) = 0;
    return string;
}

//-----------------------------------------------------------------------

char* get_pi_serial(){

	uint32_t serial = 0;

	FILE *fp = fopen("/proc/cpuinfo", "r");

	if (fp == NULL)
		{
			perror("/proc/cpuinfo");
			exit(EXIT_FAILURE);
		}

	static char entry[80];
	char *rv;

	while (fgets(entry, sizeof(entry), fp) != NULL)
		{
			char* saveptr = NULL;

			char *key = trimWhiteSpace(strtok_r(entry, ":", &saveptr));
			char *value = trimWhiteSpace(strtok_r(NULL, ":", &saveptr));

			if (strcasecmp("Serial", key) == 0)
				{
					fclose(fp);
					return value;
				}
		}
}

char* get_sd_serial(){


	FILE *fp = fopen("/sys/block/mmcblk0/device/cid", "r");

	if (fp == NULL)
		{
			perror("/sys/block/mmcblk0/device/cid");
			exit(EXIT_FAILURE);
		}

	static char entry[100];
	fgets(entry, sizeof(entry), fp);
	char *value = trimWhiteSpace(entry);
	//printf("%s", value);

	return value;
}


int main(void)
{
		
	char* pi_serial = get_pi_serial();
	char* sd_serial = get_sd_serial();

	const char* s = pi_serial;

	std::string s_pi_serial = pi_serial;
	std::string s_sd_serial = sd_serial;
	std::string sha_input = s_pi_serial + "SALTYDOG" + s_sd_serial;
	std::string rv = sha256(sha_input);
	printf("%s", rv.c_str());
		
	return 0;
}
