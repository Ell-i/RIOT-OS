
/**
 * Define a pseudo-method, used by the handlers to distinguish
 * normal methods (sent by the client) from the internally
 * generated request to generate a notification.
 */

#ifndef COAP_METHOD_NOTIFY
#define COAP_METHOD_NOTIFY (COAP_METHOD_DELETE + 1)
#endif
#ifndef COAP_NOTIFY
#define COAP_NOTIFY (1 << (COAP_METHOD_NOTIFY - 1))
#endif

extern kernel_pid_t gcoap_observe_init(void);

extern ssize_t gcoap_observe_notify_uint32(
    coap_pkt_t *pkt, uint8_t *buf, coap_resource_t *resource, uint32_t value);

