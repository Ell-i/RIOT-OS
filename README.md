# RIOT-OS

This is a self contained repo for the RIOT-OS based development within the POINT project.

# Getting started

This example uses the STM Nucleo F401 board, see
http://www.st.com/resource/en/user_manual/DM00105823.pdf

The repository references RIOT-OS as submodule to keep a proper track
of commit history and attribution.  To start using RIOT-OS you have to
checkout the submodule too.

## Clone the main repsitory

If you don't have your SSH key installed in your github account (and you do remember your password):
```
git clone https://github.com/point-h2020/RIOT-OS.git
```
If you have installed your SSH key at your github account, it is preferable to use it:
```
git clone git@github.com:point-h2020/RIOT-OS.git
```

## Initialise RIOT as a git submodule, from Ell-i RIOT github fork

```
cd RIOT-OS
git submodule init
git submodule update
```

## Compile and flash

For compiling you need `arm-none-eabi-gcc` and tools; for flashing `openocd`.

To build the ACM ICN demo version from Berlin in September 2017:
```
cd examples/ell-i_rdcli_simple
make flash
```

The binary prints its IPv6 configuration over the USB terminal.
If your Python installation has the `serial` module, 
you can simply use `make term` to connect to the USB terminal.
Otherwise, use e.g. `screen` or `minicom`.
