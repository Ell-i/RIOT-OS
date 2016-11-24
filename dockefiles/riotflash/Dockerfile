#############################################################################################
#
# Dockerfile for riot_flash
#
#############################################################################################

##
# Collection of Dockerfiles to get openocd, riot-os development environment
# and avrdude (for arduino :)) up and running
# command lines (run from ...wkdir/RIOT$):
# docker run -it --privileged -v $(pwd):/data/riotbuild -v /dev/bus/usb:/dev/bus/usb riot_tools make -C examples/blink BOARD=nucleo-f401
# docker run -it --privileged -v $(pwd):/data/riotbuild -v /dev/bus/usb:/dev/bus/usb riot_flash make -C examples/blink BOARD=nucleo-f401 flash
#
##
FROM debian:jessie
#
MAINTAINER Eero Hakala <eero.k.hakala@ell-i.org>

# the openocd-part with MAINTAINER Sébastien NEDJAR <sebastien@labaixbidouille.com>
RUN apt-get update -q \
    	&& apt-get install -y \
    	libhidapi-hidraw0 \
    	libusb-0.1-4 \
    	libusb-1.0-0 \
    	libhidapi-dev \
    	libusb-1.0-0-dev \
    	libusb-dev \
    	libtool \
    	make \
    	automake \
    	pkg-config \
    	gcc-arm-none-eabi \
    	git \
    	subversion curl wget python p7zip unzip parallel \
	&& mkdir /usr/src/; cd /usr/src/ \
	&& git clone --depth 1 https://github.com/ntfreak/openocd.git \
	&& cd openocd \
	&& ./bootstrap \
	&& ./configure --enable-stlink --enable-jlink --enable-ftdi --enable-cmsis-dap \
	&& make -j"$(nproc)" \
	&& make install-strip \
	&& cd .. \
	&& rm -rf openocd

RUN cp /usr/local/share/openocd/contrib/99-openocd.rules /etc/udev/rules.d/99-openocd.rules


###
# installing stuff for arduino
###
RUN apt-get -y install avrdude

##
# some environment stuff, like pip and pyserial...
##
RUN wget https://bootstrap.pypa.io/get-pip.py
RUN python get-pip.py
RUN pip install pyserial


RUN mkdir -m 777 -p /data/ccache
ENV CCACHE_DIR /data/ccache

RUN mkdir -p /data/riotbuild
WORKDIR /data/riotbuild
# the last row of Dockerfile for riot_flash!
