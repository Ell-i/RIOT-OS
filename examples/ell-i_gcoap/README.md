# gcoap Ell-i server

## Example Use

TBD

### Query from libcoap example client

gcoap does not provide any output to the CoAP terminal when it handles a request. We recommend use of Wireshark to see the request and response. You also can add some debug output in the endpoint function callback.

    ./coap-client -N -m get -p 5683 coap://[fe80::1843:8eff:fe40:4eaa%tap0]/.well-known/core

Example response:

    v:1 t:NON c:GET i:0daa {} [ ]
    </cli/stats>

The response shows the endpoint registered by the gcoap CLI example.
