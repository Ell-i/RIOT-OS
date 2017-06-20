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

static ssize_t _riot_board_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len,
				   void *ignored)
{
    (void)ignored;
    
    return coap_reply_simple(pkt, COAP_CODE_205, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)RIOT_BOARD, strlen(RIOT_BOARD));
}

static const coap_resource_t _resources[] = {
    { "/a/A0",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(0)    },
    { "/a/A1",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(1)    },
    { "/a/A2",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(2)    },
    { "/a/A3",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(3)    },
    { "/a/A4",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(4)    },
    { "/a/A5",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(5)    },
    { "/a/D0",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_0  },
    /* D1 used for other purposes */
    { "/a/D2",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_2  },
    { "/a/D3",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_3  },
    { "/a/D4",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_4  },
    { "/a/D5",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_5  },
    { "/a/D6",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_6  },
    { "/a/D7",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_7  },
    { "/a/D8",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_8  },
    { "/a/D9",  COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_9  },
    { "/a/D10", COAP_GET | COAP_PUT, coap_arduino_digital_getput, (void*)ARDUINO_PIN_10 },
    /* D11-D13 used for Ethernet SPI */

    { "/riot/board",  COAP_GET, _riot_board_handler, NULL },
};

static const unsigned _resources_numof = sizeof(_resources) / sizeof(_resources[0]);

static gcoap_listener_t _listener = {
    (coap_resource_t *)&_resources[0],
    sizeof(_resources) / sizeof(_resources[0]),
    NULL
};

void gcoap_register_handlers(void) {
    gcoap_register_listener(&_listener);
}
