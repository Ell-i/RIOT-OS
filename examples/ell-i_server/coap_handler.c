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
#define RES             ADC_RES_10BIT

/* internal value that can be read/written via CoAP */
//static uint8_t internal_value = 0;


static ssize_t _riot_board_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
    return coap_reply_simple(pkt, COAP_CODE_205, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)RIOT_BOARD, strlen(RIOT_BOARD));
}

static ssize_t _riot_output_digital_pa10_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
    unsigned code = COAP_CODE_EMPTY;
    const char *rsp = NULL;
    gpio_init(GPIO_PIN(0, 10), GPIO_OUT);
    ssize_t p = 0;


    if (pkt->payload[0] == '1')
    {
        //light = '1';
        gpio_set(GPIO_PIN(0, 10));    
    }
    else
    {
        //light = '0';
        gpio_clear(GPIO_PIN(0, 10));    
    }
    return coap_reply_simple(pkt, code, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)rsp, p);
}


static ssize_t _riot_1_0_1_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
//    const char *teksti = "101_PA0";

    int i = 0;
   
    int sample = 0;

        if (adc_init(ADC_LINE(i)) < 0) {
            printf("Initialization of ADC_LINE(%i) failed\n", i);
            return 1;
        } else {
            printf("Successfully initialized ADC_LINE(%i)\n", i);
        }

    sample = adc_sample(ADC_LINE(i), RES);

    printf("ADC_LINE(i): %i\n", sample);

    unsigned char digits[5];
    digits[0] = sample / 1000 + 48;
    digits[1] = (sample / 100) % 10 + 48;
    digits[2] = (sample / 10) % 10 + 48;
    digits[3] = sample % 10 + 48;
    digits[4] = '\0';

    printf("ADC_LINE(i): %s\n", digits);

    return coap_reply_simple(pkt, COAP_CODE_205, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)digits,  strlen((const char *)digits));
}


static ssize_t _riot_state_PA10_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
    const char *teksti = "";
    const char *korkea = "HIGH";
    const char *matala = "LOW ";

    if (gpio_read(GPIO_PIN(0, 10))) {
       teksti = korkea;
    }
    else {
       teksti = matala;
    }

    printf("pinstate: %s\n", teksti);

    return coap_reply_simple(pkt, COAP_CODE_205, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)teksti,  strlen(teksti));

}

static ssize_t _riot_ana_PA0_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
//    const char *teksti = "ana_PA0";

    int i = 0;
   
    int sample = 0;

        if (adc_init(ADC_LINE(i)) < 0) {
            printf("Initialization of ADC_LINE(%i) failed\n", i);
            return 1;
        } else {
            printf("Successfully initialized ADC_LINE(%i)\n", i);
        }

    sample = adc_sample(ADC_LINE(i), RES);

    printf("ADC_LINE(i): %i\n", sample);

    unsigned char digits[5];
    digits[0] = sample / 1000 + 48;
    digits[1] = (sample / 100) % 10 + 48;
    digits[2] = (sample / 10) % 10 + 48;
    digits[3] = sample % 10 + 48;
    digits[4] = '\0';

    printf("ADC_LINE(i): %s\n", digits);

    return coap_reply_simple(pkt, COAP_CODE_205, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)digits,  strlen((const char *)digits));

}



/*

static ssize_t _riot_testi2_PA10_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
    const char *teksti = "testi2";
    return coap_reply_simple(pkt, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)teksti, strlen(teksti));
}


static ssize_t _riot_testi3_PA10_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
    const char *teksti = "testi3";
    return coap_reply_simple(pkt, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)teksti, strlen(teksti));
}



static ssize_t _riot_testi4_PA10_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
    const char *teksti = "testi4";
    return coap_reply_simple(pkt, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)teksti, strlen(teksti));
}
*/

const coap_resource_t coap_resources[] = {
    COAP_WELL_KNOWN_CORE_DEFAULT_HANDLER,
    { "/1/0/1", COAP_GET, _riot_1_0_1_handler },
    { "/ana/PA0", COAP_GET, _riot_ana_PA0_handler },
    { "/output_digital/PA10", COAP_PUT, _riot_output_digital_pa10_handler },
    { "/riot/board", COAP_GET, _riot_board_handler },
    { "/state/P10", COAP_GET, _riot_state_PA10_handler },
/*
    { "/testi2/PA10", COAP_GET, _riot_testi2_PA10_handler },
    { "/testi3/PA10", COAP_GET, _riot_testi3_PA10_handler },
    { "/testi4/PA10", COAP_GET, _riot_testi4_PA10_handler },
*/
};


const unsigned coap_resources_numof = sizeof(coap_resources) / sizeof(coap_resources[0]);

