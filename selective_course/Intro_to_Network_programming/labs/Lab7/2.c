#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static int oseed = 0x455118d2; // change this when getting the seed from first oracle
int r = 0;

char *
gen_secret(int k1, int k2) {
	static int len;
	static char buf[64];
    if(!r){
        srand(oseed);
        r = 1;
    }
	printf("** Thank you for your message, let me generate a secret for you.\n");
	len  = snprintf(buf,     10, "%x", rand());
	len += snprintf(buf+len, 10, "%x", rand());
	len += snprintf(buf+len, 10, "%x", rand());
	len += snprintf(buf+len, 10, "%x", rand());
	buf[len] = '\0';
	return buf;
}

void init(char* s){
    char* secret;
    secret = gen_secret(1, 1);
    s = strdup(secret);
    printf("%s\n", s);
}

int main(){
    char* s1;
    char* s2;
    init(s1);
    rand();
    init(s2);
}