/*
 * Copyright (C) 2017 Ell-i open source co-operative
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "net/gcoap.h"

typedef struct {
    coap_resource_t; // Unnamed, depends on -fplan9-extensions GCC flag
    uint32_t item;
} elli_coap_resource_t;

ssize_t coap_arduino_digital_get(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *p);
ssize_t coap_arduino_digital_put(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *p);
ssize_t coap_arduino_digital_getput(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *p);
ssize_t coap_arduino_analog_get (
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *p);
/* XXX TBD
ssize_t coap_arduino_pwm_put(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *p);
*/
