# name of your application
APPLICATION = ell-i_rdcli_simple

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f401

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

# Use ENC24J600 Ethernet hardware
USEMODULE += encx24j600
CFLAGS += "-DENCX24J600_SPI=SPI_DEV(0)" "-DENCX24J600_CS=GPIO_PIN(PORT_C,10)" "-DENCX24J600_INT=GPIO_PIN(PORT_D,2)"

# Enable GNRC networking
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_icmpv6_echo

# Run the simple CoRE resource directory
USEMODULE += gcoap
USEMODULE += od
USEMODULE += fmt
# USEMODULE += rdcli_simple_standalone

# Include the shell for testing purposes
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

FEATURES_REQUIRED += periph_hwrng

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP  -fplan9-extensions

# For debugging and demonstration purposes, we limit the lifetime to the minimal
# allowed value of 60s (see draft-ietf-core-resource-directory-11, Table 2)
CFLAGS += -DRDCLI_LT=60

# We need ADC and timers
FEATURES_REQUIRED = periph_adc
USEMODULE += xtimer

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
