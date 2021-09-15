#include <jansson.h>
#include <unistd.h>
#include "main.h"
#include <string.h>
#include <pcap.h>
#include <time.h>
#include <arpa/inet.h>

void readConfig(struct Conf* c) {
    json_t* config = json_object();
    json_error_t* err;

    FILE* f = fopen("./config.json","r");
    if (!f) {
        perror("Error");
    }
    config = json_loadf(f,JSON_DECODE_ANY,err);
    if(err) {
        fprintf(stderr,"%s\n",err->text);
        _exit(-1);
    }
    
    json_t* interface = json_string(NULL);
    interface = json_object_get(config,"interface");
    if (!interface) {
        printf("Failed to get interface string\n");
    }
    json_t* filter = json_string(NULL);
    filter = json_object_get(config,"filter");
    if (!filter) {
        printf("Failed to get filter string\n");
    }

    char* conf_interface = malloc(100);
    memcpy(conf_interface,json_string_value(interface),strlen(json_string_value(interface)));

    c->interface = conf_interface;

    char* conf_filter = malloc(100);
    memcpy(conf_filter,json_string_value(filter),strlen(json_string_value(filter)));

    c->filter= conf_filter;

    json_decref(config);
    fclose(f);
    free(conf_interface);
    free(conf_filter);
}

void getVersion() {
    char libversion[25];   
    strcpy(libversion,pcap_lib_version());
    printf("%s\n",libversion);
}

void sniff(struct Conf* conf) {
    
        pcap_t* handle = init(conf);
    
        int data_link = pcap_datalink(handle);
        printf("Device: %s\n",conf->interface);
        printf("Data link header type: %d\n",data_link);
        printf("Data link header description: %s\n",pcap_datalink_val_to_description(data_link)); // get L2 info
        getInterfaceInfo(conf);

        struct bpf_program pf; 
        printf("Filter: %s\n",conf->filter);
    
        int res = pcap_compile(handle,&pf,conf->filter,1,PCAP_NETMASK_UNKNOWN);  // compile filter expression

        if ( res == -1) {
            fprintf(stderr,"%s\n",pcap_geterr(handle));
        } else {
            printf("Created BPF struct\n");
        }
        

        res = pcap_setfilter(handle,&pf); // apply filter expression
        if ( res == -1) {
             fprintf(stderr,"%s\n",pcap_geterr(handle));
        } else {
            printf("Set filter\n");
        }
    

        struct pcap_pkthdr header;
        const u_char* packet;

        packet = pcap_next(handle,&header); // capture packet

        if (!packet) {
            perror(getTime());
            pcap_close(handle);
           _exit(-1);
        }
        printf("Len: %d\n",header.len);
        printf("Cap len: %d\n",header.caplen);

        const struct sniff_ethernet* ethernet; /* The ethernet header */
        const struct sniff_ip *ip; /* The IP header */
	    const struct sniff_tcp *tcp; /* The TCP header */
	    const u_char *payload; /* Packet payload */
        int payload_len;

	    u_int size_ip;
	    u_int size_tcp;

        /* Initialize packet headers + payload */
        ethernet = (struct sniff_ethernet*)(packet);   
        ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	    size_ip = IP_HL(ip)*4;
        tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	    size_tcp = TH_OFF(tcp)*4;
        payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

        payload_len = header.caplen - (size_ip + size_tcp-14);

    
        getL2Header(ethernet);

        printf("IP Protocol: %d\n",ip->ip_p); // implement translation of protocol no to protocol name using /etc/protocols table
        printf("Size IP: %d\n",size_ip);
        printf("Size TCP: %d\n",size_tcp);
        printf("TCP dport: %d\n",ntohs(tcp->th_dport));
        printf("TCP sport: %d\n",ntohs(tcp->th_sport));
        printf("Payload len: %d\n",payload_len);

       
        if (payload_len > 0) {      // print payload as ASCII
            const u_char *temp_pointer = payload;
            int byte_count = 0;
         while (byte_count++ < payload_len) {
                printf("%c", *temp_pointer);
                temp_pointer++;
        }
        printf("\n");
        }

        pcap_close(handle);
    
}

void packetHandler(u_char *args, const struct pcap_pkthdr *header,const u_char *packet) {
    // handler for pcap_loop()
}

char* getTime() {
    time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    return time_str;
}

void getL2Header(const struct sniff_ethernet* ethernet) {
      printf("Dest MAC: ");
      for ( int i=0; i< ETHER_ADDR_LEN;i++)
      {
          printf("%x:",ethernet->ether_dhost[i]);
      }

     printf("\n");

    printf("Source MAC: ");
     for ( int i=0; i< ETHER_ADDR_LEN;i++)
      {
          printf("%x:",ethernet->ether_shost[i]);
      }
     printf("\n");

    printf("Ethernet type: %d\n",ethernet->ether_type);

}

void getInterfaceInfo(struct Conf* conf) {
    char* ip = malloc(13);
    char* mask = malloc(13);
    bpf_u_int32 ip_raw; /* IP address as integer */
    bpf_u_int32 subnet_mask_raw; /* Subnet mask as integer */
    int lookup_return_code;
    char error_buffer[PCAP_ERRBUF_SIZE]; /* Size defined in pcap.h */
    struct in_addr address; /* Used for both ip & subnet */

    lookup_return_code = pcap_lookupnet(
        conf->interface,
        &ip_raw,
        &subnet_mask_raw,
        error_buffer
    );
    if (lookup_return_code == -1) {
        printf("Error: %s\n", error_buffer);
        exit(-1);
    }

    address.s_addr = ip_raw;
    strcpy(ip, inet_ntoa(address));
    if (!ip) {
        perror("inet_ntoa error:"); /* print error */
        exit(-1);
    }
    address.s_addr = subnet_mask_raw;
    strcpy(mask, inet_ntoa(address));
    if (!mask) {
        perror("inet_ntoa error:"); /* print error */
        exit(-1);
    }

    
    printf("IP address: %s\n", ip);
    printf("Netmask: %s\n",mask);
    free(ip);
    free(mask);

}

 pcap_t* init(struct Conf* conf) {
    pcap_t* handle;
    char* err = malloc(PCAP_ERRBUF_SIZE);
    handle = pcap_open_live(conf->interface,PCAP_BUF_SIZE,10,TIMEOUT,err); // get pcap handle; require big timeout to get a response
    if (!handle) {
        fprintf(stderr,"%s\n",err);
        return NULL;
    }
        printf("Got pcap handle\n");
        return handle;

}