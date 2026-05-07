#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "TPSN035";    // à modifier
const char* password = "BTSSN2022"; // à modifier aussi

WebServer server(80);

int sortieled = 23;
int buzzer = 26;
int frequence = 2000;

void gestionLED() {
  digitalWrite(sortieled, HIGH);
  
  server.send(200, "text/plain", "LED test");
}
void gestionLEDoff() {
  digitalWrite(sortieled, LOW);
  
  server.send(200, "text/plain", "LED test");
}
void gestionSON() {
  ledcWriteTone(buzzer, 1000);
  delay(500);
  ledcWriteTone(buzzer, 500); 
  delay(500);
  ledcWriteTone(buzzer, 1000); 
  delay(500);
  ledcWriteTone(buzzer, 500); 
  delay(500);

  
  server.send(200, "text/plain", "Son test");
}
void gestionSONoff() {
  ledcWriteTone(buzzer, 0);
 

  
  server.send(200, "text/plain", "Son test");
}

void setup() {

  Serial.begin(115200);

  pinMode(sortieled, OUTPUT);
  ledcAttach(buzzer, frequence, 8);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());

  server.on("/led", gestionLED);
  server.on("/son", gestionSON);
  server.on("/ledoff", gestionLEDoff);
  server.on("/sonoff", gestionSONoff);

  server.begin();
}

void loop() {
  server.handleClient();
}
