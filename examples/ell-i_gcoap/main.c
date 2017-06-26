/*
 * Copyright (c) 2015-2016 Ken Bannister. All rights reserved.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       gcoap example
 *
 * @author      Ken Bannister <kb2ma@runbox.com>
 *
 * @}
 */

#include <stdio.h>

#include "net/gcoap.h"
#include "kernel_types.h"

#include "xtimer.h"
#include "periph/adc.h"
#include "luid.h"
#include "fmt.h"

#include "coap_handler.h"
#include "coap_observer.h"
#include "coap_directory.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

/* import "ifconfig" shell command, used for printing addresses */
extern int _netif_config(int argc, char **argv);

int main(void)
{
    puts("RIOT Ell-i gcoap application");

    /* Initialise Arduino digital I/O pins, input by default */
    gpio_init(ARDUINO_PIN_0,  GPIO_IN);
    /* Do NOT initialise D1: reason unknown */
    gpio_init(ARDUINO_PIN_2,  GPIO_IN);
    gpio_init(ARDUINO_PIN_3,  GPIO_IN);
    gpio_init(ARDUINO_PIN_4,  GPIO_IN);
    gpio_init(ARDUINO_PIN_5,  GPIO_IN);
    gpio_init(ARDUINO_PIN_6,  GPIO_IN);
    gpio_init(ARDUINO_PIN_7,  GPIO_IN);
    gpio_init(ARDUINO_PIN_8,  GPIO_IN);
    gpio_init(ARDUINO_PIN_10, GPIO_IN);
    /* Do NOT initialise D11-D13, used for ENC28J60 SPI */

    /* Initialise Arduino analog I/O pins */
    adc_init(ADC_LINE(0));
    adc_init(ADC_LINE(1));
    adc_init(ADC_LINE(2));
    adc_init(ADC_LINE(3));
    adc_init(ADC_LINE(4));
    adc_init(ADC_LINE(5));

    /* nanocoap_server uses gnrc sock which uses gnrc which needs a msg queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    puts("Waiting for address autoconfiguration...");
    xtimer_sleep(3);

    /* print network addresses */
    puts("Configured network interfaces:");
    _netif_config(0, NULL);

    /* Register the CoAP resource handlers */
    gcoap_register_handlers();

    /* Start CoAP observer thread */
    (void)gcoap_observe_init();

    /* Start the coap listener thread */
    (void)gcoap_init();

    /* Register at a CoAP resource directory */
    uint64_t uuid;
    luid_get(&uuid, sizeof(uuid));
    char uuidbuf[sizeof(uuid) * 2];
    fmt_u64_hex(uuidbuf, uuid);
    gcoap_directory_register_simple(uuidbuf, sizeof(uuidbuf));

    /* Things will continue running even if we return */
    return 0;
}
