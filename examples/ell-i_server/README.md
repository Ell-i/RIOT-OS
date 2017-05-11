# Ell-i nanocoap server

This application is meant to get you started with implementing a CoAP server on RIOT.
It uses the GNRC network stack through RIOT's conn socket API.

## Compilation

If you are using the dockers files from this repo, the following command creates a new docker instance for compiling and then removes it once done:
```
$ docker run -it --rm --privileged -v $(pwd)/../..:/data/riotbuild riotdeve make -C examples/ell-i_server QUIET=1
```
Alternatively, if you have local compilation environment, you can just use, in this directory:
```
$ make
```

## Flashing

Using the docker containers:
```
$ docker run -it --rm --privileged -v $(pwd)/../..:/data/riotbuild riotflash make -C examples/ell-i_server QUIET=1 flash
```
Using native environment:
```
$ make flash
```

Using Serial-ver-USB as a monitoring interface with e.g. minicom (use sudo!) - you might need to reset the embedded device first -
you'll see a screen like the one below:.

```
main(): This is RIOT! (Version: 2016.07-devel-279-g29f0ee-485ec5859977-2016.04)
Successfully initialized ADC_LINE(0)
RIOT Ell-i nanocoap application
Configured network interfaces:
Iface  5   HWaddr: d8:80:39:02:c0:e7
                                                                                
           MTU:1500  HL:64  RTR  RTR_ADV
           Source address length: 6
           Link type: wired
           inet6 addr: ff02::1/128  scope: local [multicast]
           inet6 addr: fe80::da80:39ff:fe02:c0e7/64  scope: local
           inet6 addr: ff02::1:ff02:c0e7/128  scope: local [multicast]
           inet6 addr: ff02::2/128  scope: local [multicast]

Waiting for incoming UDP packet...

```

# Testing

There are multiple external CoAP clients you can use to test the server on native.

## libcoap CLI

NOTE! Do replace "fe80::f8bf:2bff:fe01:9ea3" with your link-layer address

```
# coap-client "coap://[fe80::f8bf:2bff:fe01:9ea3%tap0]/riot/board"
```

## Copper (Firefox Plugin)

The Copper plugin for Firefox provides you with a nice graphical interface, but
getting it to work with RIOT requires a little setup.

Make sure you've installed

- The [Firefox Copper plugin](https://addons.mozilla.org/en-US/firefox/addon/copper-270430/)
- The Router Advertisement Daemon (radvd)

And build the application again using `make`.

Enter the following into your `/etc/radvd.conf` (if it doesn't exist yet, create one):

```
    interface tap0
    {
        AdvSendAdvert on;

        MinRtrAdvInterval 3;
        MaxRtrAdvInterval 10;

        AdvDefaultPreference low;

       prefix 2001:db8:1:0::/64
       {
            AdvOnLink on;
            AdvAutonomous on;
            AdvRouterAddr off;
       };
    };
```

(you can use `radvd -c` to check for syntax errors)

and run

```
sudo radvd
```

Then, run the RIOT binary as usual:

```
make term
```

Note that the output listing all configured interfaces contains a globally scoped
address, which you can now use to reach the RIOT instance via Copper. To do so, enter this:

```
coap://[2001:db8:1:0:e42a:1aff:feca:10ec]/riot/board
```

into your Firefox address bar, where you should replace `2001:db8:1:0:e42a:1aff:feca:10ec`
with your RIOT instance's address marked as "scope: **global**".
If you click the big green `GET` button, the word `native` should appear in the
**Payload** text box at the center of the GUI.

**If this doesn't work,** try manually adding a Global address to the tap0 interface:

```
sudo service radvd start
sudo ip address add 2001:db8:1::a/64 dev tap0
make term
```
