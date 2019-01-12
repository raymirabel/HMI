/**************************************************************************
 @FILE:         Ejemplo_Basico.ino
 @AUTHOR:       Santiago Pérez Tello
 @COMPANY:      Ray Ingeniería Electronica, S.L.
 @DESCRIPTION:  Ejemplo básico de uso del HMI CNMB5_I2C para ArduPLCs

 @LICENCE DETAILS:
  Este sketch está basada en software libre. Tu puedes redistribuir
  y/o modificar esta sketch bajo los términos de licencia GNU.

  Esta programa se distribuye con la esperanza de que sea útil,
  pero SIN NINGUNA GARANTÍA, incluso sin la garantía implícita de
  COMERCIALIZACIÓN O PARA UN PROPÓSITO PARTICULAR.
  Consulte los términos de licencia GNU para más detalles:

  http://www.gnu.org/licenses/gpl-3.0.txt

  This sketch is based on free software. You can redistribute
  and/or modify this library under the terms of the GNU license.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY, even without the implied warranty of
  MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU license terms for more details:

  http://www.gnu.org/licenses/gpl-3.0.txt

 @VERSIONS:
  08-01-2019 - v1.00 : Primera versión. Incluir comentarios acerca de la nueva versión

**************************************************************************/

// Librerias necesarias
#include <Wire.h>  		  // Control del bus I2C
#include "PCF8574.h" 	  // Control del expansor I/O de 8 puertos por I2C
#include "U8glib.h"		  // Control de pantalla

// Asignamos una palabra clave al pin al que corresponde cada boton
#define BT_UP     	0   // Boton up conectado al pin 0 del PCF8574T
#define BT_DOWN   	1   // Boton down conectado al pin 1 del PCF8574T
#define BT_LEFT   	2   // Boton left conectado al pin 2 del PCF8574T
#define BT_RIGHT  	3   // Boton RIGHT conectado al pin 3 del PCF8574T
#define BT_INTRO  	4   // Boton intro conectado al pin 4 del PCF8574T

// Asignamos una palabra clave al pin al que corresponde cada led
#define LED_RED   	5	  // Led rojo conectado al P5 del PCF8574T
#define LED_GRE 	  6   // Led verde conectado al P6 del PCF8574T
#define LED_ORA 	  7   // Led naranja conectado al P7 del PCF8574T

// Definimos el valor del radio (en pixel) de los circulos mostrados en pantalla simulando los botones
#define RADIO       10

// Instancia del expansor I2C
PCF8574 HMI_IO;

// Instancia de la pantalla OLED
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_FAST);

// Variables globales
uint8_t height;

/**************************************************************************
	SETUP  (solo se ejecuta una vez)
**************************************************************************/
void setup() {
  // Inicializamos el expansor I2C con la dirección 0x20
  // Podemos modificar físicamente esta dirección si tenemos conflicto con otro expanson en el mismo bus.
  // Para ello debes modificar el estado de los jumpers S7, S8 y S9 (A0, A1 y A2) que hay en la parte posterior de la PCB.
  // Consulta el manual de la placa CNMB5_I2C para mas detalles.
  HMI_IO.begin(0x20);

  // Indicamos qué pines del expansor I2C son de entrada (BOTONES) LOW = Pulsado,  HIGH = NO Pulsado
  HMI_IO.pinMode(BT_UP, INPUT_PULLUP);
  HMI_IO.pinMode(BT_DOWN, INPUT_PULLUP);
  HMI_IO.pinMode(BT_LEFT, INPUT_PULLUP);
  HMI_IO.pinMode(BT_RIGHT, INPUT_PULLUP);
  HMI_IO.pinMode(BT_INTRO, INPUT_PULLUP);

  // Indicamos qué pines del expansor I2C son de salida (LEDs)
  HMI_IO.pinMode(LED_RED, OUTPUT);
  HMI_IO.pinMode(LED_GRE, OUTPUT);
  HMI_IO.pinMode(LED_ORA, OUTPUT);

  // Indicamos que los leds estén apagados inicialmente
  // (LOW = Encendido y HIGH = APAGADO)
  HMI_IO.digitalWrite(LED_RED, HIGH);
  HMI_IO.digitalWrite(LED_GRE, HIGH);
  HMI_IO.digitalWrite(LED_ORA, HIGH);

  // Calculamos el ancho y el alto de la pantalla
  height = u8g.getWidth() - 1;
}

/**************************************************************************
	LOOP  (se ejecuta continuamente)
**************************************************************************/
void loop() {
  static unsigned long timeAnterior = millis();
  static uint8_t state = 0;

  // Enciende y alterna los leds cada segundo...
  if (millis() - timeAnterior >= 1000) {
    timeAnterior = millis();
    switch (state) {
      case 0:
        HMI_IO.digitalWrite (LED_RED, HIGH);
        HMI_IO.digitalWrite (LED_GRE, LOW);
        HMI_IO.digitalWrite (LED_ORA, HIGH);
        break;
      case 1:
        HMI_IO.digitalWrite (LED_RED, LOW);
        HMI_IO.digitalWrite (LED_GRE, HIGH);
        HMI_IO.digitalWrite (LED_ORA, HIGH);
        break;
      case 2:
        HMI_IO.digitalWrite (LED_RED, HIGH);
        HMI_IO.digitalWrite (LED_GRE, HIGH);
        HMI_IO.digitalWrite (LED_ORA, LOW);
        break;
    }
    state++;

    if (state > 2) {
      state = 0;
    }
  }


  // Estructura utilizada para la representación por la pantalla LCD
  u8g.firstPage();
  do {
    // Pintamos los circulos vacios
    DrawCircles();

    // Cambiamos el estado de los circulos en funcion del boton que presionamos (modo PULSADOR)
    if (HMI_IO.digitalRead(BT_UP) == 0)     u8g.drawDisc( height / 2, RADIO, RADIO);
    if (HMI_IO.digitalRead(BT_DOWN) == 0)   u8g.drawDisc( height / 2, RADIO + 4 * RADIO + 2, RADIO);
    if (HMI_IO.digitalRead(BT_INTRO) == 0)  u8g.drawDisc( height / 2, RADIO + 2 * RADIO + 1, RADIO - 2);
    if (HMI_IO.digitalRead(BT_LEFT) == 0)   u8g.drawDisc( height / 2 - height / 5, RADIO + 2 * RADIO + 1, RADIO);
    if (HMI_IO.digitalRead(BT_RIGHT) == 0)  u8g.drawDisc( height / 2 + height / 5, RADIO + 2 * RADIO + 1, RADIO);   

  } while (u8g.nextPage());
}


/**************************************************************************
	FUNCIONES IMPLEMENTADAS
**************************************************************************/

// Función que dibuja los circulos vacios en la pantalla
void DrawCircles() {
  u8g.drawCircle( height / 2, RADIO, RADIO);
  u8g.drawCircle( height / 2, RADIO + 2 * RADIO + 1, RADIO - 2);
  u8g.drawCircle( height / 2, RADIO + 4 * RADIO + 2, RADIO);
  u8g.drawCircle( height / 2 - height / 5, RADIO + 2 * RADIO + 1, RADIO);
  u8g.drawCircle( height / 2 + height / 5, RADIO + 2 * RADIO + 1, RADIO);
}
