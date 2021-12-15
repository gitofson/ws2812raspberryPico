# ws2812raspberryPico


Vytvořeno pomocí led pásků s ws2812b a raspberry pi pico.

Dokumentace k Raspberry Pi Pico:

[Hlavní stránka o Raspberry Pi Pico](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html)

[GettingStarted](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) - instalace SDK, blikání LED, ...
[Dokumentace k C/C++ SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
----
Návod k použití tohoto kódu (Linux):
1. instalace [SDK Toolkitu](https://github.com/raspberrypi/pico-sdk)
2. mkdir build
3. cd build
4. cmake .. (případně PICO_SDK_PATH=/cesta_k_sdk/pico-sdk cmake ..)
5. make
6. stisknutí tlačítka na Raspberry Pi Pico a zasunutí konektoru do PC
7. překopírování patřičného .uf2 do nově zobrazeného diskového zařízení


Windows:
[Skript k instalaci SDK toolkitu](https://github.com/ndabas/pico-setup-windows)