#include <pcap.h>
#include <packet_structs.h>

#define TIMEOUT 10000

struct Conf {
    char* interface;
    char* filter;
};

pcap_t* init(struct Conf*);
char* getTime();
void getVersion();
void sniff(struct Conf*);
void readConfig(struct Conf* c);
void packetHandler(u_char*, const struct pcap_pkthdr *,const u_char *);
void getL2Header(const struct sniff_ethernet* ethernet);
void getInterfaceInfo(struct Conf* conf);

