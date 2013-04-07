#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



char hexDigit(unsigned n)
{
    if (n < 10) {
        return n + '0';
    } else {
        return (n - 10) + 'A';
    }
}


void hexify(char ch, char hex[3])
{
    hex[0] = hexDigit(ch / 0x10);
    hex[1] = hexDigit(ch % 0x10);
    hex[2] = '\0';
}


void hexify_string(const char *str, char *hex_string)
{
    char hex_byte[3];
    int size = 0;
    int i = 0;
    int len = 0;
    memset(hex_string, '\0', strlen(hex_string));
    size = strlen(str);
    while((str != NULL ) && (i < size ))  {
        hexify(str[i], hex_byte);
        strcat(&hex_string[0]+2*i, hex_byte);
        i++;
    }
    hex_string[2*i] = '\0';
}

void execute_airdecap_ng(char *packet_file, char *hex_password)
{
    FILE *fp;
    int status;
    char path[200];
    char str[25];
    char buf[500];
    int count;
    //   sprintf(str, "airdecap-ng %s -w %s", packet_file, hex_password );
    sprintf(str, "ls -al" );
    fp = popen(str, "r");
    count = 0;
    while(fgets(path, 200, fp) != NULL ) {
        printf("1St %s", path );
        if( count == 5 )
            printf("***** %s\n", path );
        count++;
    }

    status = pclose(fp);

}

bool contains_non_alpha_chars(char *str)
{
    while(str) {
        if(!isalpha(*str))
            return false;
        str++;
    }
    return true;
}

int main(int argc, char **argv)
{
    char line[100];
    int  line_len = 0;
    char hexstring[100];
    char *packet_file;


    if( argc <= 3 ) {
        printf("Useage: argv[0] packet_file passwordfile number_of_chars\n");
        exit(-1);
    }

    packet_file = argv[1];

    FILE *f = fopen(argv[2], "r"); 
    if( f == NULL ) {
        printf("Error opening password dictionary - %s\n", argv[2]);
        exit(-1);
    }

    while(fgets(line, sizeof(line), f ) != NULL) {
        line_len = strlen(line);
        /**
         * The line read has \r character in addition to the terminating
         * null character.  So, for example if the strlen is 6 that means
         * the string has 5 characters+'\r'.  We need to remove this character
         */
        if((line_len > 0 ) && ( line_len == atoi(argv[3]) + 1)) {
            if(line[line_len -1] == '\n')   
                line[line_len-1] = '\0';
            if(contains_non_alpha_chars(line))
                continue;

            hexify_string(line, &hexstring[0]);
            printf("Trying password = %s hexstring = %s\n", line, hexstring );
            execute_airdecap_ng(packet_file, hexstring);
        }
    }
    fclose(f);

    return 0;
}
