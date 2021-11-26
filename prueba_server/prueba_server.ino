/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server con acceso a SPIFFS
  Basándose en los ejemplos de:
  https://github.com/khoih-prog/WiFiWebServer
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://github.com/luisllamasbinaburo/ESP32-Examples
  https://www.luisllamas.es/esp8266-esp32/
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <LiquidCrystal.h>
//************************************************************************************************
// Variables
//************************************************************************************************
char park1;
char park2;
//************************************************************************************************
// Pines de conexion para pantalla LCD
//************************************************************************************************
const int rs = 22, en = 23, d4 = 5, d5 = 18, d6 = 19, d7 = 21;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);

  server.on("/", handle_home); // página de inicio
  server.on("/sotanos", handle_sotanos); // handler de visualizacion de estado de parqueos de sotano 1 y 2
  server.onNotFound([]() {                  // si el cliente solicita una uri desconocida
    if (!HandleFileRead(server.uri()))      // enviar archivo desde SPIFF, si existe
      handleNotFound();             // sino responder con un error 404 (no existe)
  });
  //server.onNotFound(handle_NotFound); //si no se encuentra el uri, responder con error 404 (no existe)

  server.begin(); // iniciar servidor
  Serial.println("HTTP server started");
  delay(100);

  /////////// inicializacion de pantalla LCD /////////////////////////////////////////
  lcd.begin(16, 2);  // definicion de uso de LCD 16x2
  lcd.print("Sotano1  Sotano2"); ///////// mensaje de bienvenida
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {

  server.handleClient(); // escuchar solicitudes de clientes

  if (Serial.available()) {
    park1 = Serial.read(); 
  }
  if (Serial2.available()) {
    park2 = Serial2.read(); 
  }
   server.send(200, "text/html", SendHTML(park1, park2)); //responde con un OK (200) y envía HTML

}

//************************************************************************************************
// Handler de página de inicio
//************************************************************************************************
void handle_home() {
  server.send(200, "text/html", HTMLhome()); //responde con un OK (200) y envía HTML
}
//************************************************************************************************
// Handler del sotano1
//************************************************************************************************
void handle_sotanos() {
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(park1, park2)); //responde con un OK (200) y envía HTML
}
//************************************************************************************************
// Procesador de HTML para paguina de incio
//************************************************************************************************
String HTMLhome() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Home</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 160px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-s {background-color: #3498db;}\n";
  ptr += ".button-s:active {background-color: #2980b9;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Miraflores System Parking &#128664 </h1>\n";
  ptr += "<h3>De click en boton para visualizar el estado de los parqueos</h1>\n";
  ptr += "<a class=\"button button-s\" href=\"/sotanos\">Visualizar</a>\n";

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Procesador de HTML para los sotanos
//************************************************************************************************
String SendHTML(char parqueo1, char parqueo2) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<title>Sotanos</title>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 160px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Estado de los parqueos &#128664</h1>\n";
  ptr += "<h3> Sotano # 1 </h3>\n";

  switch (parqueo1) {
    case ('a'):
      ptr += "<p>Hay 4 parqueos disponibles</p>\n";
      ptr += "<p>Hay 0 parqueos ocupados</p>\n";
      lcd.setCursor(3, 1);
      lcd.print("4");
      break;
    case ('b'):
      ptr += "<p>Hay 3 parqueos disponibles</p>\n";
      ptr += "<p>Hay 1 parqueo ocupado</p>\n";
      lcd.setCursor(3, 1);
      lcd.print("3");
      break;
    case ('c'):
      ptr += "<p>Hay 2 parqueos disponibles</p>\n";
      ptr += "<p>Hay 2 parqueos ocupados</p>\n";
      lcd.setCursor(3, 1);
      lcd.print("2");
      break;
    case ('d'):
      ptr += "<p>Hay 1 parqueos disponibles</p>\n";
      ptr += "<p>Hay 3 parqueos ocupados</p>\n";
      lcd.setCursor(3, 1);
      lcd.print("1");
      break;
    case ('e'):
      ptr += "<p>Hay 0 parqueos disponibles</p>\n";
      ptr += "<p>Hay 4 parqueos ocupados</p>\n";
      lcd.setCursor(3, 1);
      lcd.print("0");
      break;
  }

  ptr += "<h3> Sotano # 2 </h3>\n";

  switch (parqueo2) {
    case ('a'):
      ptr += "<p>Hay 4 parqueos disponibles</p>\n";
      ptr += "<p>Hay 0 parqueos ocupados</p>\n";
      lcd.setCursor(12, 1);
      lcd.print("4");
      break;
    case ('b'):
      ptr += "<p>Hay 3 parqueos disponibles</p>\n";
      ptr += "<p>Hay 1 parqueo ocupado</p>\n";
      lcd.setCursor(12, 1);
      lcd.print("3");
      break;
    case ('c'):
      ptr += "<p>Hay 2 parqueos disponibles</p>\n";
      ptr += "<p>Hay 2 parqueos ocupados</p>\n";
      lcd.setCursor(12, 1);
      lcd.print("2");
      break;
    case ('d'):
      ptr += "<p>Hay 1 parqueos disponibles</p>\n";
      ptr += "<p>Hay 3 parqueos ocupados</p>\n";
      lcd.setCursor(12, 1);
      lcd.print("1");
      break;
    case ('e'):
      ptr += "<p>Hay 0 parqueos disponibles</p>\n";
      ptr += "<p>Hay 4 parqueos ocupados</p>\n";
      lcd.setCursor(12, 1);
      lcd.print("0");
      break;
  }

  //ptr += "<META HTTP-EQUIV ='Refresh' CONTENT ='1'>";       // auto refresco 
  ptr += "<a class=\"button button-on\" href=\"/\">Refrescar</a>\n";   // refresco por boton 

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}
//************************************************************************************************
// Obtener el tipo de contenido del archivo
//************************************************************************************************
String GetContentType(String filename)
{
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, GetContentType(path));
  file.close();
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path, String contentType)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, contentType);
  file.close();
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
bool HandleFileRead(String path)
{
  if (path.endsWith("/")) path += "index.html";
  Serial.println("handleFileRead: " + path);

  if (SPIFFS.exists(path))
  {
    ServeFile(path);
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}
