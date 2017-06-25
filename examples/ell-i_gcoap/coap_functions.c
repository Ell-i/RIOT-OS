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

#include "periph/gpio.h"
#include "periph/adc.h"
#include "fmt.h"
#include "nanocoap.h"
#include "coap_functions.h"
#include "coap_observer.h"

#ifndef COAP_METHOD_NOTIFY
#define COAP_METHOD_NOTIFY (COAP_METHOD_DELETE + 1)
#endif

ssize_t coap_arduino_digital_get(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {

    uint32_t gpio_pin = res->item;

    return coap_reply_simple(
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

    if (pkt->payload[0] == '1') {
        gpio_set(GPIO_PIN(0, 10));    
    } else {
        gpio_clear(GPIO_PIN(0, 10));    
    }
    return coap_reply_simple(
	pkt, COAP_CODE_CHANGED, buf, len,
	COAP_FORMAT_TEXT, NULL, 0);
}

ssize_t coap_arduino_digital_getput(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res) {

    const unsigned method = coap_get_code_detail(pkt);
    if (COAP_METHOD_GET == method) {
	return coap_arduino_digital_get(pkt, buf, len, res);
    } else if (COAP_METHOD_PUT == method) {
	return coap_arduino_digital_put(pkt, buf, len, res);
    } else if (COAP_METHOD_NOTIFY == method) {
	return coap_arduino_digital_notify(pkt, buf, len, res);
    }
    abort();
}


ssize_t coap_arduino_analog_get(
    coap_pkt_t *pkt, uint8_t *buf, size_t len, elli_coap_resource_t *res)
{
    uint32_t adc_line = res->item;

    const uint16_t sample = adc_sample(adc_line, 12 /* Resolution in bits */);

    printf("ADC_LINE(i): %i\n", sample);

    unsigned char digits[5];
    digits[0] = (sample / 1000) % 10 + '0';
    digits[1] = (sample /  100) % 10 + '0';
    digits[2] = (sample /   10) % 10 + '0';
    digits[3] = (sample /    1) % 10 + '0';
    digits[4] = '\0';

    printf("ADC_LINE(i): %s\n", digits);

    return coap_reply_simple(
	pkt, COAP_CODE_CONTENT, buf, len,
	COAP_FORMAT_TEXT, (uint8_t*)digits, sizeof(digits)-1);
}


