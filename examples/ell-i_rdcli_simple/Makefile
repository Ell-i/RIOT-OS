# name of your application
APPLICATION = rdcli_simple

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Enable GNRC networking
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default

# Run the simple CoRE resource directory
USEMODULE += rdcli_simple_standalone

# Include the shell for testing purposes
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

FEATURES_REQUIRED += periph_hwrng

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# For debugging and demonstration purposes, we limit the lifetime to the minimal
# allowed value of 60s (see draft-ietf-core-resource-directory-11, Table 2)
CFLAGS += -DRDCLI_LT=60

include $(RIOTBASE)/Makefile.include
