
#include <stdlib.h>
#include "net/gcoap.h"
#include "fmt.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

#include "coap_observer.h"
#include "coap_handler.h"

/**
 * Try to send a notification on this resource.
 *
 * If there is no observer registered, do nothing.
 */
static void _try_send_notification(const coap_resource_t *res, void *param/*ignored*/) {
    (void) param;

    static uint8_t buf[GCOAP_PDU_BUF_SIZE];
    static coap_pkt_t pkt;

    /*
     * If the resource handler knows how to send notifications,
     * then ask it to generate one.  If the result is good,
     * try to send it
     */
    if (res->methods & COAP_NOTIFY) {
	DEBUG("CoAP: Considering observe of %s...", res->path);

	/* Minimally initialise a CoAP PDU as a NOTIFY pseudo method.
	 * The handler will call gcoap_obs_init, so we don't need to do much here.
	 */
	pkt.hdr = (coap_hdr_t *)buf;
	(void)coap_build_hdr(pkt.hdr, COAP_TYPE_NON, NULL, 0, COAP_METHOD_NOTIFY, 0);

	ssize_t pkt_len = res->handler(&pkt, buf, sizeof(buf), res);
        if (pkt_len <= 0) {
	    DEBUG("no observers.\n");
	    return;
	}
	gcoap_obs_send(buf, pkt_len, res);
	DEBUG("notification sent.\n");
    }
}

#ifndef GCOAP_NOTIFY_PERIOD
#define GCOAP_NOTIFY_PERIOD (10 * US_PER_SEC)
#endif

/**
 * Thread loop for gcoap observe _pid thread.
 */
static void *_thread_loop(void *arg)
{
    (void)arg;

    xtimer_ticks32_t last_wakeup = xtimer_now();

    DEBUG("CoAP: Starting observer thread.\n");
    for (;;) {
	gcoap_for_resources(_try_send_notification, NULL/*ignored*/);

	xtimer_periodic_wakeup(&last_wakeup, GCOAP_NOTIFY_PERIOD);
    }

    return 0;
}

#ifndef GCOAP_OBSERVE_STACK_SIZE
#define GCOAP_OBSERVE_STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#endif

static kernel_pid_t _pid = KERNEL_PID_UNDEF;
static char _msg_stack[GCOAP_OBSERVE_STACK_SIZE];

kernel_pid_t gcoap_observe_init(void) {

    if (_pid != KERNEL_PID_UNDEF) {
        return -EEXIST;
    }

    _pid = thread_create(_msg_stack, sizeof(_msg_stack), THREAD_PRIORITY_MAIN - 1,
			 THREAD_CREATE_STACKTEST, _thread_loop, NULL, "coap observe");

    return _pid;
}

/**
 * Build a notification packet, formatting the given unsigned
 * integer as plain text
 */
ssize_t gcoap_observe_notify_uint32(
    coap_pkt_t *pkt, uint8_t *buf, coap_resource_t *resource, uint32_t value) {

    int err = gcoap_obs_init(pkt, buf, sizeof(buf), resource);

    switch (err) {
    case GCOAP_OBS_INIT_OK: {
	const size_t payload_len = fmt_u16_dec((char *)pkt->payload, value);
	return gcoap_finish(pkt, payload_len, COAP_FORMAT_TEXT);
    }
    case GCOAP_OBS_INIT_UNUSED:
	return -ENOTCONN;
    case GCOAP_OBS_INIT_ERR:
	return -ENOMEM;
    }
    abort();
    /* NOTREACHED */
}
