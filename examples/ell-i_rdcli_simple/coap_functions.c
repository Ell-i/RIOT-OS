/*
 * Copyright (C) 2017 Ell-i open source co-operative
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ENABLE_DEBUG (1)
#include <debug.h>

#include <periph/gpio.h>
#include <periph/adc.h>

#include "nanocoap.h"
#include "coap_functions.h"
#include "coap_observer.h"

ssize_t gcoap_reply_simple(
    coap_pkt_t *pkt,
    unsigned code,
    uint8_t *buf, size_t len,
    unsigned ct,
    const uint8_t *payload,
    uint8_t payload_len) {

    gcoap_resp_init(pkt, buf, len, code);
    memcpy(pkt->payload, payload, payload_len);
    return gcoap_finish(pkt, payload_len, ct);
}

ssize_t coap_arduino_digital_get(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {

    uint32_t gpio_pin = res->item;

    return gcoap_reply_simple(
	pkt, COAP_CODE_CONTENT, buf, len,
	COAP_FORMAT_TEXT, (unsigned char *) (gpio_read(gpio_pin)? "1": "0"), 2);
}

ssize_t coap_arduino_digital_notify(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {

    uint32_t gpio_pin = res->item;

    return gcoap_observe_notify_uint32(pkt, buf, res, gpio_read(gpio_pin));
}

ssize_t coap_arduino_digital_put(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {

    uint32_t gpio_pin = res->item;

    // Automatically convert PIN to output
    gpio_init(gpio_pin, GPIO_OUT); // XXX Should do only once, not always

    DEBUG("CaAP PUT: payload='%s'\n", pkt->payload);
    if (pkt->payload[0] == '1') {
	DEBUG("CoAP PUT: pin %lu set\n", gpio_pin);
        gpio_set(gpio_pin);
    } else {
	DEBUG("CoAP PUT: pin %lu clear\n", gpio_pin);
        gpio_clear(gpio_pin);
    }
    return gcoap_reply_simple(
	pkt, COAP_CODE_CHANGED, buf, len,
	COAP_FORMAT_NONE, NULL, 0);
}

ssize_t coap_arduino_digital_getput(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {

    switch (coap_get_code_detail(pkt)) {
    case COAP_METHOD_GET:
	return coap_arduino_digital_get(pkt, buf, len, res);
    case COAP_METHOD_PUT:
	return coap_arduino_digital_put(pkt, buf, len, res);
    case COAP_METHOD_NOTIFY:
	return coap_arduino_digital_notify(pkt, buf, len, res);
    }
    abort();
}

ssize_t coap_arduino_analog_get(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {

    uint32_t adc_line = res->item;

    const uint16_t sample = adc_sample(adc_line, ADC_RES_10BIT);

    DEBUG("CoAP GET: ADC_LINE(i): %i\n", sample);

    // XXX: Change to use fmt.h instead
    unsigned char digits[5];
    digits[0] = (sample / 1000) % 10 + '0';
    digits[1] = (sample /  100) % 10 + '0';
    digits[2] = (sample /   10) % 10 + '0';
    digits[3] = (sample /    1) % 10 + '0';
    digits[4] = '\0';

    return gcoap_reply_simple(
	pkt, COAP_CODE_CONTENT, buf, len,
	COAP_FORMAT_TEXT, (uint8_t*)digits, sizeof(digits)-1);
}

ssize_t coap_arduino_analog_notify(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {

    uint32_t adc_line = res->item;

    const uint16_t sample = adc_sample(adc_line, ADC_RES_10BIT);

    DEBUG("sample=%i...", sample);

    return gcoap_observe_notify_uint32(pkt, buf, res, sample);
}

ssize_t coap_arduino_analog_get_not(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {
    switch (coap_get_code_detail(pkt)) {
    case COAP_METHOD_GET:
	return coap_arduino_analog_get(pkt, buf, len, res);
    case COAP_METHOD_NOTIFY:
	return coap_arduino_analog_notify(pkt, buf, len, res);
    }
    abort();
}
