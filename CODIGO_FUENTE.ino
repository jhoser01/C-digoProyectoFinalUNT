#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
// Colocar datos de la red WiFi
const char* nombre_wifi= "Tu_red_wifi";
const char* contraseña = "728575841922";
//Desde Botfather en telegram se obtiene el token de Telegram BOT
#define token_Bot "Tu_token"
// En IDBot en telegram, se obtiene el ID
#define ID_Chat "Tu_chat_ID"
WiFiClientSecure client;
UniversalTelegramBot bot(token_Bot, client);
const int pin_sensorPIR = 23; //La entrada del sensor PIR está conectada a este PIN
const int pin_luz = 13;// Pin para la luz de 220V(Con módulo relay)
int contar = 0;
String aviso = "";
String contar_string = "";
int PIR = 0;
////Setup////
void setup() {
  Serial.begin(115200);
  //Para la entrada sensor PIR activamos el resistor de PULLUP 
  pinMode(pin_sensorPIR, INPUT_PULLUP);
  pinMode(pin_luz, OUTPUT);
  // Intento de conexión a la red WiFi:
  Serial.print("Intentando conectarse a la red wifi... ");
  Serial.println(nombre_wifi);
  //Arreglo de la red WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(nombre_wifi, contraseña);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Conectado a la red wifi!!!");
  Serial.print("Dirección ip: ");
  Serial.println(WiFi.localIP());//Imprimimos la direción ip local
  bot.sendMessage(ID_Chat, "Sistema listo para el arranque!!!!", "");//Enviamos un mensaje a Telegram para informar que el sistema está preparado
}
/////loop//////
void loop() {
   PIR = digitalRead(pin_sensorPIR);//Se lee el etado del sensor PIR
   //Si es igual a "1" la condición se cumple
  if(PIR == 1){
    digitalWrite(pin_luz, HIGH);//Encendemos la luz
    Serial.println("Luz Prendida!!!");
      contar = contar + 1;//Incremento en el contador de movimientos detectados 
    contar_string = String(contar);//Lo convertimos a una cadena
    aviso = "Movimientos detectados: " + contar_string;//Concatenamos ambas cadenas
    bot.sendMessage(ID_Chat, aviso, "");//Enviamos el mensaje a Telegram
    delay(30000);//Mantenemos la luz encendida durante ese tiempo
    digitalWrite(pin_luz, LOW);//Luego la apagamos
    Serial.println("Luz apagada!!!");
    PIR = 0;//Asignamos el valor "0" a la variable PIR para que deje de cumplirse la condición
  }
}
