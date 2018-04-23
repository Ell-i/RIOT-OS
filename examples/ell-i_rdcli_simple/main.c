/*
 * Copyright (C) 2017 Freie Universität Berlin
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
 * @brief       CoRE RD simple registration test application
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "shell.h"
#include "net/gcoap.h"
#include "net/rdcli_common.h"

#include "xtimer.h"
#include "periph/adc.h"
#include "luid.h"
#include "fmt.h"

#include "coap_handler.h"
#include "coap_observer.h"

#define BUFSIZE             (64U)

static char riot_info[BUFSIZE];

int main(void)
{
    puts("RIOT Ell-i gcoap application");
    puts("based on CoAP simplified RD registration example!\n");

    /* Initialise Arduino digital I/O pins, input by default */
    /* Do NOT initialise D0: shell thread will block on a mutex for an unknown reason */
    /* Do NOT initialise D1: reason unknown */
    gpio_init(ARDUINO_PIN_2,  GPIO_OUT);  gpio_set(ARDUINO_PIN_2);
    gpio_init(ARDUINO_PIN_3,  GPIO_OUT);  gpio_set(ARDUINO_PIN_3);
    gpio_init(ARDUINO_PIN_4,  GPIO_OUT);  gpio_set(ARDUINO_PIN_4);
    gpio_init(ARDUINO_PIN_5,  GPIO_OUT);  gpio_set(ARDUINO_PIN_5);
    gpio_init(ARDUINO_PIN_6,  GPIO_OUT);  gpio_set(ARDUINO_PIN_6);
    gpio_init(ARDUINO_PIN_7,  GPIO_OUT);  gpio_set(ARDUINO_PIN_7);
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

    /* fill riot info */
    sprintf(riot_info, "{\"lt\":%u}", RDCLI_LT);

    puts("Waiting for address autoconfiguration...");
    xtimer_sleep(3);

    /* print network addresses */
    extern int _netif_config(int argc, char **argv); // XXX Not in any headers ....
    puts("Configured network interfaces:");
    _netif_config(0, NULL);

    /* Register the CoAP resource handlers */
    gcoap_register_handlers();

    /* Start CoAP observer thread */
    (void)gcoap_observe_init();

    /* Start the coap listener thread */
    (void)gcoap_init();

    /* print RD client information */
    puts("Client information:");
    printf("  lt: %is\n", (int)RDCLI_LT);

    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
