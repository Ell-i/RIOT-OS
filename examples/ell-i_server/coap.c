/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2016 Eero Hakala <eero.k.hakala@ell-i.org>    
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <coap.h>
#include <board.h>
#include <string.h>
#include "periph/gpio.h"
#include "periph/adc.h"
#define RES             ADC_RES_10BIT
#define DELAY           (1000 * 1000U)


/*
 * Nucleo/Arduino pin numbers
 *                                       D15 PB8
 *                                       D14 PB9
 *                                       D13 PA5
 *                                       D12 PA6
 *                                       D11 PA7
 *                                       D10 PB6
 *                                       D9  PC7
 *                                       D8  PA9
 *                                       D7  PA8
 *                                       D6  PB10
 *  PA0 A0  (pre def ADC-channel)        D5  PB4
 *  PA1 A1  (pre def ADC-channel)        D4  PB5
 *  PA4 A2  (pre def ADC-channel)        D3  PB3
 *  PB0 A3                               D2  PA10
 *  PC1 A4                               D1  PA2
 *  PC0 A5                               D0  PA3
 */

#define MAX_RESPONSE_LEN 500
static uint8_t response[MAX_RESPONSE_LEN] = { 0 };

static int handle_get_well_known_core(coap_rw_buffer_t *scratch,
                                      const coap_packet_t *inpkt,
                                      coap_packet_t *outpkt,
                                      uint8_t id_hi, uint8_t id_lo);

static int handle_output_digital_PA10(coap_rw_buffer_t *scratch,
                                 const coap_packet_t *inpkt,
                                 coap_packet_t *outpkt,
                                 uint8_t id_hi, uint8_t id_lo);

static int handle_input_digital_PA10(coap_rw_buffer_t *scratch,
                                 const coap_packet_t *inpkt,
                                 coap_packet_t *outpkt,
                                 uint8_t id_hi, uint8_t id_lo);

/*
 * PA0
 */
static int handle_input_analog_PA0(coap_rw_buffer_t *scratch,
                                 const coap_packet_t *inpkt,
                                 coap_packet_t *outpkt,
                                 uint8_t id_hi, uint8_t id_lo);

/*
 * PA1
 */
static int handle_input_analog_PA1(coap_rw_buffer_t *scratch,
                                 const coap_packet_t *inpkt,
                                 coap_packet_t *outpkt,
                                 uint8_t id_hi, uint8_t id_lo);

/*
 * PA_ALL
 */
static int handle_input_analog_PA_ALL(coap_rw_buffer_t *scratch,
                                 const coap_packet_t *inpkt,
                                 coap_packet_t *outpkt,
                                 uint8_t id_hi, uint8_t id_lo);




static const coap_endpoint_path_t path_well_known_core =
        { 2, { ".well-known", "core" } };

static const coap_endpoint_path_t path_output_digital_PA10 =
        { 2, { "output-digital", "PA10" } };

static const coap_endpoint_path_t path_input_digital_PA10 =
        { 2, { "input-digital", "PA10" } };

static const coap_endpoint_path_t path_input_analog_PA0 =
        { 2, { "input-analog", "PA0" } };

static const coap_endpoint_path_t path_input_analog_PA1 =
        { 2, { "input-analog", "PA1" } };

static const coap_endpoint_path_t path_input_analog_PA_ALL =
        { 2, { "input-analog", "PAALL" } };

const coap_endpoint_t endpoints[] =
{
    { COAP_METHOD_GET,	handle_get_well_known_core,
        &path_well_known_core, "ct=40" },
    { COAP_METHOD_PUT,	handle_output_digital_PA10,
        &path_output_digital_PA10,	   "ct=0"  },
    { COAP_METHOD_GET,	handle_input_digital_PA10,
        &path_input_digital_PA10,	   "ct=0"  },
    { COAP_METHOD_GET,	handle_input_analog_PA0,
        &path_input_analog_PA0,	   "ct=0"  },
    { COAP_METHOD_GET,	handle_input_analog_PA1,
        &path_input_analog_PA1,	   "ct=0"  },
    { COAP_METHOD_GET,	handle_input_analog_PA_ALL,
        &path_input_analog_PA_ALL,	   "ct=0"  },
    /* marks the end of the endpoints array: */
    { (coap_method_t)0, NULL, NULL, NULL }
};

static int handle_get_well_known_core(coap_rw_buffer_t *scratch,
        const coap_packet_t *inpkt, coap_packet_t *outpkt,
        uint8_t id_hi, uint8_t id_lo)
{
    char *rsp = (char *)response;
    int len = sizeof(response);
    const coap_endpoint_t *ep = endpoints;
    int i;

    len--; // Null-terminated string

    while (NULL != ep->handler) {
        if (NULL == ep->core_attr) {
            ep++;
            continue;
        }

        if (0 < strlen(rsp)) {
            strncat(rsp, ",", len);
            len--;
        }

        strncat(rsp, "<", len);
        len--;

        for (i = 0; i < ep->path->count; i++) {
            strncat(rsp, "/", len);
            len--;

            strncat(rsp, ep->path->elems[i], len);
            len -= strlen(ep->path->elems[i]);
        }

        strncat(rsp, ">;", len);
        len -= 2;

        strncat(rsp, ep->core_attr, len);
        len -= strlen(ep->core_attr);

        ep++;
    }

    return coap_make_response(scratch, outpkt, (const uint8_t *)rsp,
                              strlen(rsp), id_hi, id_lo, &inpkt->tok,
                              COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}

static int handle_output_digital_PA10(coap_rw_buffer_t *scratch,
        const coap_packet_t *inpkt, coap_packet_t *outpkt,
        uint8_t id_hi, uint8_t id_lo)
{
    const char *alkuteksti = "no command";
    const char *lopputeksti = "OK";
    int len = strlen(alkuteksti);

    gpio_init(GPIO_PIN(0, 10), GPIO_OUT);

    if (inpkt->payload.len == 0)
    {

       memcpy(response, alkuteksti, len);
       return coap_make_response(scratch, outpkt, (const uint8_t *)response, len,
                              id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_TEXT_PLAIN);
    }


    memcpy(response, lopputeksti, len);

    if (inpkt->payload.p[0] == '1')
    {
        //light = '1';
        gpio_set(GPIO_PIN(0, 10));    
    }
    else
    {
        //light = '0';
        gpio_clear(GPIO_PIN(0, 10));    
    }



    return coap_make_response(scratch, outpkt, (const uint8_t *)response, len,
                              id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_TEXT_PLAIN);
}


static int handle_input_digital_PA10(coap_rw_buffer_t *scratch,
        const coap_packet_t *inpkt, coap_packet_t *outpkt,
        uint8_t id_hi, uint8_t id_lo)
{
    //const char *riot_name = RIOT_BOARD;
    //int len = strlen(RIOT_BOARD);
    const char *teksti = "";
    const char *korkea = "HIGH";
    const char *matala = "LOW";

    if (gpio_read(GPIO_PIN(0, 10))) {
       teksti = korkea;
    }
    else {
       teksti = matala;
    }

    int len = strlen(teksti);
    memcpy(response, teksti, len);


    return coap_make_response(scratch, outpkt, (const uint8_t *)response, len,
                              id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_TEXT_PLAIN);
}


static int handle_input_analog_PA0(coap_rw_buffer_t *scratch,
        const coap_packet_t *inpkt, coap_packet_t *outpkt,
        uint8_t id_hi, uint8_t id_lo)
{
    //const char *riot_name = RIOT_BOARD;
    int len = 0;
    int i = 0;
    //int sample = 0;
   
    int sample = 0;
    //sample = adc_sample(ADC_LINE(0), RES);

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
    len = 4;
    memcpy(response, digits, len);

    printf("ADC_LINE(i): %s\n", digits);

    return coap_make_response(scratch, outpkt, (const uint8_t *)response, len,
                              id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_input_analog_PA1(coap_rw_buffer_t *scratch,
        const coap_packet_t *inpkt, coap_packet_t *outpkt,
        uint8_t id_hi, uint8_t id_lo)
{
    //const char *riot_name = RIOT_BOARD;
    int len = 0;
    int i = 1;
    //int sample = 0;
   
    int sample = 0;
    //sample = adc_sample(ADC_LINE(0), RES);

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
    len = 4;
    memcpy(response, digits, len);

    printf("ADC_LINE(i): %s\n", digits);

    return coap_make_response(scratch, outpkt, (const uint8_t *)response, len,
                              id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_input_analog_PA_ALL(coap_rw_buffer_t *scratch,
        const coap_packet_t *inpkt, coap_packet_t *outpkt,
        uint8_t id_hi, uint8_t id_lo)
{
    char *rsp = (char *)response;
    /* resetting the content of response message */
    memset(response, 0, sizeof(response));
    int len = sizeof(response);
    //int len = 0;
    //int i = 1;
    static int samp[ADC_NUMOF]; // Static to initialize to zero
   
    for (int i = 0; i < ADC_NUMOF; i++) {
	if (adc_init(ADC_LINE(i))) {
	    printf("inittipinitti %i poks\n", i);
	    //
	}
	samp[i] = adc_sample(ADC_LINE(i), RES);
	printf("ADC_LINE(%i):  ", i);

	char digits[5];
	digits[0] = samp[i] / 1000 + 48;
	digits[1] = (samp[i] / 100) % 10 + 48;
	digits[2] = (samp[i] / 10) % 10 + 48;
	digits[3] = samp[i] % 10 + 48;
	digits[4] = '\0';

	printf("%s ", digits);
	strncat(rsp, digits, 4);
	strncat(rsp, " ", 1);
    }

    memcpy(response, rsp, len);
    printf("\n"); 

    return coap_make_response(scratch, outpkt, (const uint8_t *)response, len,
                              id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_TEXT_PLAIN);
}


