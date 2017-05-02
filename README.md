# RIOT-OS
self contained repo for the RIOT-OS development.

# Getting started

The repository references RIOT-OS as submodule to keep a proper track of commit history and attribution.
To start using RIOT-OS you have to checkout the submodule too.

```
git clone https://github.com/point-h2020/RIOT-OS.git
cd RIOT-OS
git submodule init
git submodule update
```
For compiling you need `arm-none-eabi-gcc` and tools; for flashing `openocd`.

To build and flash the binary
```
cd examples/examples/ell-i_server
make flash
```

The binary prints its IPv6 configuration over the USB terminal.  
If your Python installation has the `serial` module, 
you can simply use `make term` to connect to the USB terminal.
Otherwise, use e.g. `screen` or `minicom`.
