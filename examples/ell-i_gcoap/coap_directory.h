
#define COAP_RD_DIRECTORY_URL "/.well-known/core?ep="

extern int gcoap_directory_register(
    coap_pkt_t *pkt, uint8_t *buf, size_t buflen, char *node_name, size_t node_name_len);
extern int gcoap_directory_register_simple(char *node_name, size_t node_name_len);
