
#define ENABLE_DEBUG (1)
#include "debug.h"

#include "net/gcoap.h"

#include "coap_directory.h"

static void _directory_resp_handler(unsigned int req_state, coap_pkt_t* pkt) {

    switch (req_state) {
    case GCOAP_MEMO_ERR:
	/* Some obscure error.  gcoap.c as of Jun 26 2017 did not use this. */

	/* FALLTHROUGH */
    case GCOAP_MEMO_TIMEOUT:
	/*
	 * Timeout, not response.
	 */

	/*
	 * We didn't find any directory, got just timeout or error.
	 * We should retransmit, but since gCoAP does not implement
	 * that yet, we skip any retransmission here.
	 */
	break;

    case GCOAP_MEMO_WAIT:
	/*
	 * Should never be given as an argument, but instead return GCOAP_MEMO_RESP.
	 * But as of Jun 26 2017 gcoap.c does pass this one...
	 */

	/* FALLTHROUGH */
    case GCOAP_MEMO_RESP:
	/*
	 * Received a response.
	 */

	// XXX Continue here.  For now just ignore

    default:
	DEBUG("Unknown req_state=%d\n", req_state);
	abort();
    }
}

int gcoap_directory_register(
    coap_pkt_t *pkt, uint8_t *buf, size_t buflen, char *node_name, size_t node_name_len) {

    /* Build an initial packet */
    int res = gcoap_req_init(pkt, buf, buflen, COAP_METHOD_GET, COAP_RD_DIRECTORY_URL);
    if (res < 0) {
	return res;
    }

    /* Extend the URL path with the node_name.  XXX This is somewhat a hack... */
    memcpy(pkt->url + sizeof(COAP_RD_DIRECTORY_URL), node_name, node_name_len);
    pkt->payload += node_name_len;

    /* Finish, we don't have any payload */
    ssize_t len = gcoap_finish(pkt, 0, COAP_FORMAT_TEXT);

    /* Send to the link all nodes multicast address */
    static const sock_udp_ep_t remote = {
	.family    = AF_INET6,
	.netif     = SOCK_ADDR_ANY_NETIF,
	.addr      = IPV6_ADDR_ALL_NODES_LINK_LOCAL ,
	.port      = COAP_PORT,
    };

    return (int)gcoap_req_send2(buf, len, &remote, _directory_resp_handler);
}

int gcoap_directory_register_simple(char *node_name, size_t node_name_len) {
    /*
     * Allocate these from the stack, as these are used only
     * during boot time.  Hence, we save memory if we use
     * these at the boot time in the stack, then releasing the memory.
     */
    coap_pkt_t pkt;
    uint8_t buf[GCOAP_PDU_BUF_SIZE];

    return gcoap_directory_register(&pkt, buf, sizeof(buf), node_name, node_name_len);
}
