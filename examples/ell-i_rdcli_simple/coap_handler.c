/*
 * Copyright (C) 2016 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "periph/adc.h"
#include "nanocoap.h"
#include "arduino_pinmap.h"
#include "net/gcoap.h"

#include "coap_functions.h"
#include "coap_handler.h"
#include "coap_observer.h"

static ssize_t _riot_board_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len,
				   void *ignored)
{
    (void)ignored;

    return coap_reply_simple(pkt, COAP_CODE_205, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)RIOT_BOARD, strlen(RIOT_BOARD));
}

static const coap_resource_t _resources[] = {
    COAP_RESOURCE( "/a/A0",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(0) ),
    COAP_RESOURCE( "/a/A1",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(1) ),
    COAP_RESOURCE( "/a/A2",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(2) ),
    COAP_RESOURCE( "/a/A3",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(3) ),
    COAP_RESOURCE( "/a/A4",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(4) ),
    COAP_RESOURCE( "/a/A5",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(5) ),
    COAP_RESOURCE( "/a/D0",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_0 ),
    /* D1 used for other purposes */
    COAP_RESOURCE( "/a/D2",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_2 ),
    COAP_RESOURCE( "/a/D3",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_3 ),
    COAP_RESOURCE( "/a/D4",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_4 ),
    COAP_RESOURCE( "/a/D5",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_5 ),
    COAP_RESOURCE( "/a/D6",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_6 ),
    COAP_RESOURCE( "/a/D7",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_7 ),
    COAP_RESOURCE( "/a/D8",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_8 ),
    COAP_RESOURCE( "/a/D9",  COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_9 ),
    COAP_RESOURCE( "/a/D10", COAP_NOTIFY | COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_10),
    COAP_RESOURCE( "/a/reboot", COAP_PUT,  NVIC_SystemReset, 0),
    /* D11-D13 used for Ethernet SPI */

    COAP_RESOURCE( "/lux1",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(2) ),
    COAP_RESOURCE( "/lux2",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(3) ),
    COAP_RESOURCE( "/relay_1", COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_0),
    COAP_RESOURCE( "/relay_2", COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_2),
    COAP_RESOURCE( "/relay_3", COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_3),
    COAP_RESOURCE( "/relay_4", COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_4),
    COAP_RESOURCE( "/relay_5", COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_5),
    COAP_RESOURCE( "/relay_6", COAP_GET | COAP_PUT, coap_arduino_digital_getput, ARDUINO_PIN_6),
    COAP_RESOURCE( "/riot/board",  COAP_GET, _riot_board_handler, 0 ),
    COAP_RESOURCE( "/tem1",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(0) ),
    COAP_RESOURCE( "/tem2",  COAP_NOTIFY | COAP_GET, coap_arduino_analog_get_not, ADC_LINE(1) ),
};

#define RESOURCE_COUNT (sizeof(_resources) / sizeof(_resources[0]))

static gcoap_listener_t _listener = {
    .resources     = _resources,
    .resources_len = RESOURCE_COUNT,
    .next          = NULL
};

void gcoap_register_handlers(void) {
    gcoap_register_listener(&_listener);
}

void gcoap_for_resources(void(*function)(const coap_resource_t *,void *), void *param) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
	function(&(_resources[i]), param);
    }
}
