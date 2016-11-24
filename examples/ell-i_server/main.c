/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2016 Eero Hakala <eero.k.hakala@ell-i.org>    
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
 * @brief       CoAP example server application (using microcoap)
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @}
 */

#include <stdio.h>
#include "msg.h"
#include "periph/gpio.h" 
/*
 * below taken from periph adc
 */
#include "board.h"
#include "xtimer.h"
#include "periph/adc.h"
#define RES             ADC_RES_10BIT
#define DELAY           (1000 * 1000U)



#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
/*
 * Nucleo/Arduino pin numbers
 *                 D15 PB8
 *                 D14 PB9
 *                 D13 PA5
 *                 D12 PA6
 *                 D11 PA7
 *                 D10 PB6
 *                 D9  PC7
 *                 D8  PA9
 *                 D7  PA8
 *                 D6  PB10
 *  PA0 A0         D5  PB4
 *  PA1 A1         D4  PB5
 *  PA4 A2         D3  PB3
 *  PB0 A3         D2  PA10
 *  PC1 A4         D1  PA2
 *  PC0 A5         D0  PA3
 */

void microcoap_server_loop(void);

/* import "ifconfig" shell command, used for printing addresses */
extern int _netif_config(int argc, char **argv);

int main(void)
{
    gpio_init(GPIO_PIN(1, 10), GPIO_OUT);
    gpio_init(GPIO_PIN(1, 4), GPIO_OUT);
    gpio_init(GPIO_PIN(1, 5), GPIO_OUT);
    gpio_init(GPIO_PIN(0, 10), GPIO_OUT);
    gpio_init(GPIO_PIN(2, 1), GPIO_OUT); 


    puts("ELL-i microcoap example application");

    /* microcoap_server uses conn which uses gnrc which needs a msg queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* print network addresses */
    puts("Configured network interfaces:");
    _netif_config(0, NULL); 

    /* start coap server loop */
    microcoap_server_loop();

    /* should be never reached */
    return 0;
}
