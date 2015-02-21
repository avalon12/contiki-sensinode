# Contiki optimized for CC2530dk

This contiki stack is optimized for the CC2530 MCU.

# What does this fork of contiki offer?
## SimpleRDC protocol 
Simple yet effective RDC protocol that doesn't use significant RAM, making it useful for the CC2530DK platform.
## Optimal state of Contiki OS for cc2530 
The contiki OS project continues to evolve, but this has not much new to offer to the CC2530 platform other than requiring more and more stack size.
The state of Contiki OS as it whas in 2013 is better suited for the CC2530 and is preserved in the Contiki-Sensinode branch wich this project is forked from.

# Goals for near future
- Implement low power mode PM2 (deep sleep timer) working in harmony with the SimpleRDC protocol to provide extremely low power usage for the CC2530. 

# Credits
With thanks to the following people making this fork possible:
- The folks over at the [Contiki OS project](http://www.contiki-os.org/)
- George Oikonomou for implementing the CC2530 platform and his help on my work on it.
- Marcus Linderoth for sharing his RDC protocol [on github.](https://github.com/msloth/contiki-launchpad).
