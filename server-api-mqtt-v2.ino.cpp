#include <PubSubClient.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include "DHT.h"
#define DHTTYPE DHT11 // DHT 11

const char *mqtt_server = "ioticos.org";
const int mqtt_port = 1883;
const char *mqtt_user = "7smjvxoNrqpsizT";
const char *mqtt_pass = "L8deH0Ef7BzQQVe";
const char *root_topic_subscribe = "t3GQncnL0ObphtZ/input";
const char *root_topic_publish_general = "t3GQncnL0ObphtZ/general";
const char *root_topic_subscribe_temperatura = "t3GQncnL0ObphtZ/modificar-temperatura";
const char *root_topic_publish_temperatura = "t3GQncnL0ObphtZ/temperatura";
const char *root_topic_subscribe_humedad = "t3GQncnL0ObphtZ/modificar-humedad";
const char *root_topic_publish_humedad = "t3GQncnL0ObphtZ/humedad";
const char *root_topic_subscribe_puerta = "t3GQncnL0ObphtZ/puerta";


//**************
//**** WIFICONFIG ******
//**************
const char* ssid = "Varelas grajales";
const char* password = "F4m1l14V4r3l4";
const char* host = "maker.ifttt.com";

//*************
//*****CONFIG EMAIL****
//*************
String apiKey = "cIHL21wmXS7-MsB2fXc-t-"; //"YOUR_IFTTT_API_KEY";
String eventName = "send_email";
String value1, value2, value3;
String request;

//**************
//**** GLOBALES   ******
//**************
WiFiClient espClient;
PubSubClient client(espClient);
const int DHTPin = 5;
const int cooler = 4;
const int sv = 0;
DHT dht(DHTPin, DHTTYPE);
Servo servo1;


//**************
//**** VARIABLES  ******
//**************
float t, h = 0;
float lastT, lastH = 0;
float tempLimit = 30.0;
bool stateServo = false;
String strpublicar, strtemp, strhum;
char msg[85], msgtemp[30], msghum[30];
String top;
String mens = "";

//timers
const unsigned long publeshTemp = 25000;
unsigned long lastPubleshTemp;

const unsigned long publeshHum = 25000;
unsigned long lastPubleshHum;


void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();


bool event = false;

void myTestCall() {
  event = true;
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  //SENSORES
  pinMode(DHTPin, INPUT);
  pinMode(cooler, OUTPUT);
  servo1.attach(0);
  servo1.write(0);
  dht.begin();
  myTestCall();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  if (client.connected()) {
    lastT = dht.readTemperature();
    h = dht.readHumidity();
    if (!isnan(lastT)) {
      t = lastT;
    }
    
    strpublicar = "Temperatura: " + String(t) + "°C  " + "Humedad: " + String(h) + "%" + "Temperatura limite: " + tempLimit + " Estado de puerta: " + stateServo;
    strpublicar.toCharArray(msg, 85);
    client.publish(root_topic_publish_general, msg);
    delay(200);

    unsigned long topLoop1 = millis();
    if (topLoop1 - lastPubleshTemp >= publeshTemp) {
      lastPubleshTemp = topLoop1;
      strtemp = "Temperatura: " + String(t) + "°C";
      strtemp.toCharArray(msgtemp, 30);
      client.publish(root_topic_publish_temperatura, msgtemp);
      delay(200);
    }
    unsigned long topLoop2 = millis();
    if (topLoop2 - lastPubleshHum >= publeshHum) {
      lastPubleshHum = topLoop2;
      strhum = "Humedad: " + String(h) + "%";
      strhum.toCharArray(msghum, 30);
      client.publish(root_topic_publish_humedad, msghum);
      delay(200);
    }
    delay(3000);
  }
  client.loop();
 //refri();
  leerSubscripcion();
  
}

void setup_wifi() {
  delay(10);
  // Nos conectamos a nuestra red Wifi
  Serial.println();
  Serial.print("Conectando a ssid: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a red WiFi!");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

//***********
//*    CONEXION MQTT      *
//***********

void reconnect() {

  while (!client.connected()) {
    Serial.print("Intentando conexión Mqtt...");
    // Creamos un cliente ID
    String clientId = "IOTICOS_H_W_";
    clientId += String(random(0xffff), HEX);
    // Intentamos conectar
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Conectado!");
      // Nos suscribimos
      if (client.subscribe(root_topic_subscribe)) {
        Serial.println("Suscripcion raíz");
      } else {
        Serial.println("falló Suscripción raíz");
      }
      if (client.subscribe(root_topic_subscribe_temperatura)) {
        Serial.println("Suscripcion temperatura");
      } else {
        Serial.println("falló Suscripción temperatura");
      }
      if (client.subscribe(root_topic_subscribe_humedad)) {
        Serial.println("Suscripcion humedad");
      } else {
        Serial.println("falló Suscripción humedad");
      }
      if (client.subscribe(root_topic_subscribe_puerta)) {
        Serial.println("Suscripcion puerta");
      } else {
        Serial.println("falló Suscripción puerta");
      }

    } else {
      Serial.print("falló :( con error -> ");
      Serial.print(client.state());
      Serial.println(" Intentamos de nuevo en 5 segundos");
      delay(5000);
    }
  }
}





