#include <stdio.h>
#include <pcap.h>
#include <string.h>
#include <jansson.h>
#include <stdlib.h>
#include "main.h"



int main(int argc,char* argv[]) {

    struct Conf* conf = malloc(sizeof(struct Conf));
    if(!conf) {
        perror("Error");
    }

    readConfig(conf);
    sniff(conf);
    free(conf);

   
    

    return 0;   
}


