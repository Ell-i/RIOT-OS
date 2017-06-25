
#include <stdlib.h>
#include "net/gcoap.h"
#include "fmt.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

ssize_t gcoap_observe_notify_uint32(
    coap_pkt_t *pkt, uint8_t *buf, coap_resource_t *resource, uint32_t value) {

    int err = gcoap_obs_init(pkt, buf, sizeof(buf), resource);

    switch (err) {
    case GCOAP_OBS_INIT_OK:
	DEBUG("coap: sending an observe notification\n");
	const size_t payload_len = fmt_u16_dec((char *)pkt->payload, value);
	return gcoap_finish(pkt, payload_len, COAP_FORMAT_TEXT);
    case GCOAP_OBS_INIT_UNUSED:
	DEBUG("coap: observer not found\n");
	return -ENOTCONN;
    case GCOAP_OBS_INIT_ERR:
	DEBUG("coap: error initializing observe notification\n");
	return -ENOMEM;
    }
    abort();
    /* NOTREACHED */
}
