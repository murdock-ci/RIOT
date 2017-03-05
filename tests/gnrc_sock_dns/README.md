# Overview

This folder contains a test application for RIOT's sock-based DNS client.

# How to test with native

Setup up a tap interface:

    # sudo ip tuntap add dev riot0 mode tap user $(id -u -n)
    # sudo ip a a 2001:db8::1/64 dev riot0

Start dnsmasq (in another console):

    # sudo dnsmasq -d -2 -z -S 8.8.8.8 -i riot0 -q \
        --dhcp-range=::1,constructor:riot0,ra-only \
        --listen-address 2001:db8::1

Then run the test application

    # bin/native/tests_gnrc_sock_dns.elf riot0

The application will take a little while to auto-configure it's IP address.
Then you should see something like

    riot-os.org resolves to 2a01:4f8:151:64::11
