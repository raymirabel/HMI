# CNMB5_I2C
## HMI I2C con pantalla OLED 1.3", 5 pulsadores y 3 leds

----------
Con el módulo opcional HMI (Human-Machine Interface) **CNMB5_I2C** podrás interactuar fácilmente con nuestros ArduPLCs. Puede mostrar o modificar fácilmente parámetros, tiempos, menús, calendario, mensajes de alarma, gráficos, etc.

![CNMB5_I2C](https://github.com/raymirabel/HMI/blob/master/CNMB5_I2C/Doc/HMI_CNMB_I2C_1.jpg)


Lo más destacado:

- Pantalla OLED 1.3" 128x64 pixel.
- 5 pulsadores.
- 3 leds.
- Control completo de pantalla, leds y pulsadores mediante I2C.
- Conector auxiliar RJ12 para BUS de expansión I2C.
- RTC opcional.
- Optimizado para encajar en nuestras cajas de carril DIN.
- Ideal para incorporar a ArduPLC MICRO y ArduPLC V2.


Para el uso de la pantalla OLED recomendamos la librería [u8glib](https://github.com/olikraus/u8glib "u8glib").

Para el uso del expansor E/S recomendamos la librería [pcf8574](https://github.com/skywodd/pcf8574_arduino_library "PCF8574").