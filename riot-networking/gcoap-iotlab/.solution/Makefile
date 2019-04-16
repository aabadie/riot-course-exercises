# Default Makefile, for host native GNRC-based networking

# name of your application
APPLICATION = gcoap_example

# use iotlab-m3:
BOARD = iotlab-m3

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../RIOT

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

# Specify the mandatory networking modules
USEMODULE += gnrc_ipv6_default

# Add gcoap module dependency here
USEMODULE += gcoap

# Required by coap server value handler
USEMODULE += lps331ap

USEMODULE += fmt

# include this for printing IP addresses
USEMODULE += shell_commands

DEVELHELP ?= 1

include $(RIOTBASE)/Makefile.include
