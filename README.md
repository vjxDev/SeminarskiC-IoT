# Seminarski rad C - IOT uređaju
![img](.github/assets/BLUETOOTHLOWENERGY.jpg)

## Neophodan software

Za pokretanje porojekta je potreban [VS Code](https://code.visualstudio.com/). Nakon instalacije, za rad sa ESP32 potrebno je instalirati sledeće ekstenzije:
- [PlatformIO](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) - kompajlovanje i poveyivanje sa mikrokontrolerima.
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) - Syntax highlighting 

[Node.js](https://nodejs.org/en) (Opcionalno) - Pokretanje web aplikacije


## Neophodne komponente
1. ESP 32
2. 16x2 LCD display sa I2C konekcijom

![img](.github/assets/b.png)

ESP32 I2C konekcija se nalazi na pinovima 21 (SCL) i 22 (SDL) 

## Rad sa ESP32 mikrokontrolerom

Ekstenzija **PlatformIO** je odgovorna za organizaciju i poveziavanje sa ESP32. Ekstenzija nam nudi sve od kompajlovanja koda do kreacije više konfiguracije za više različitih kontolera. 

### Build i Upload

U levom menu se sad nalazi sekcija za PlatformIo. Klikom na `Upload` se kompajlije i uploduje kod na ESP32, ako je povezan.

![img](.github/assets/meni.png)



## Web aplikacija

Web aplikacija se nalazi na [iot.f.jovica.me](https://iot.f.jovica.me)
Nije nephodno lokalno pokretati sajt. Uputsvta se nalaze u [README.MD](web/README.md)