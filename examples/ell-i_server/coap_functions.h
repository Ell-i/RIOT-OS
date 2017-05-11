/*
 * Copyright (C) 2017 Ell-i open source co-operative
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

ssize_t coap_arduino_digital_get(coap_pkt_t *pkt, uint8_t *buf, size_t len, void *p);
ssize_t coap_arduino_digital_put(coap_pkt_t *pkt, uint8_t *buf, size_t len, void *p);
ssize_t coap_arduino_analog_get (coap_pkt_t *pkt, uint8_t *buf, size_t len, void *p);
/* XXX TBD
ssize_t coap_arduino_pwm_put(coap_pkt_t *pkt, uint8_t *buf, size_t len, void *p);
*/
