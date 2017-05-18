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

#include "periph/gpio.h"
#include "periph/adc.h"
#include "fmt.h"
#include "nanocoap.h"
#include "arduino_pinmap.h"

#include "coap_functions.h"

static ssize_t _riot_board_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len,
				   void *ignored)
{
    (void)ignored;
    
    return coap_reply_simple(pkt, COAP_CODE_205, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)RIOT_BOARD, strlen(RIOT_BOARD));
}

const coap_resource_t coap_resources[] = {
    COAP_WELL_KNOWN_CORE_DEFAULT_HANDLER,

    { "/arduino/A0",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(0)    },
    { "/arduino/A1",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(1)    },
    { "/arduino/A2",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(2)    },
    { "/arduino/A3",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(3)    },
    { "/arduino/A4",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(4)    },
    { "/arduino/A5",  COAP_GET, coap_arduino_analog_get,  (void*)ADC_LINE(5)    },
    { "/arduino/D0",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_0  },
    /* D1 used for other purposes */
    { "/arduino/D2",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_2  },
    { "/arduino/D3",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_3  },
    { "/arduino/D4",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_4  },
    { "/arduino/D5",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_5  },
    { "/arduino/D6",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_6  },
    { "/arduino/D7",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_7  },
    { "/arduino/D8",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_8  },
    { "/arduino/D9",  COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_9  },
    { "/arduino/D10", COAP_GET, coap_arduino_digital_get, (void*)ARDUINO_PIN_10 },
    /* D11-D13 used for Ethernet SPI */

    { "/arduino/D0",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_0  },
    /* D1 used for other purposes */
    { "/arduino/D2",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_2  },
    { "/arduino/D3",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_3  },
    { "/arduino/D4",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_4  },
    { "/arduino/D5",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_5  },
    { "/arduino/D6",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_6  },
    { "/arduino/D7",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_7  },
    { "/arduino/D8",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_8  },
    { "/arduino/D9",  COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_9  },
    { "/arduino/D10", COAP_PUT, coap_arduino_digital_put, (void*)ARDUINO_PIN_10 },
    /* D11-D13 used for Ethernet SPI */

    { "/riot/board",  COAP_GET, _riot_board_handler, NULL },
};

const unsigned coap_resources_numof = sizeof(coap_resources) / sizeof(coap_resources[0]);

