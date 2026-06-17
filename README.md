## ESP 32 playground

- 01_blink_blocking.c: Ejemplo blink con delay bloqueantes y log.
- 02_blink_timers.c: Ejemplo blink con timer, posibilidad de enceder y apagar 2 LEDS con distinto tiempo.
- 03_spi_matrix.c



## Anotaciones SPI

En ESP32 SPI1 y SPI2 son general purpose.

- MOSI: Master Out Slave In.
- MISO: Master In Slave Out.
- SCLK: Serial Clock.
- CS: Chip Select.

Transaction: Atomica no se puede interrumpir.
    - Por interrupciones.
    - Por polling.


Ver docs/spi


Videos YouTube:

https://www.youtube.com/playlist?list=PL-Hb9zZP9qC65SpXHnTAO0-qV6x5JxCMJ