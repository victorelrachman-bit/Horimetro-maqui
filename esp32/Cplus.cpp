#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Preferences.h>
#define BUZ 2
#define BUT_RES 18
#define BUT_STO 19
#define DELAY 200
#define TIME 32000

//NVS
Preferences prefs;
long long cont;
int quartil;
bool paused = false;
bool buzon = false;

bool modo_config = false;
WebServer server(80);
WiFiClientSecure wifi_client;
//senha do cliente web el-rachman: Elrachman123

//MQTT
const char* mqtt_server = "846613f2ff8f4e40be716012473fc849.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_client_id = "espdovitao";
const char* mqtt_username = "Vectorchapa1234";
const char* mqtt_password = "Compacdahora123";
PubSubClient mqtt_client(wifi_client);

void setup() {
  Serial.begin(115200);
  wifi_client.setInsecure();
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(callback);
  if(connect_wifi())
  {
    connect_mqtt();
    mqtt_client.subscribe("hori/m1/pause");
    mqtt_client.subscribe("hori/m1/reiniciar");
    prefs.begin("hori", true);
    cont = prefs.getLong("cont", 0);
    prefs.end();
    quartil = cont / (TIME / 4);
    if (quartil > 4) quartil = 4;
  }
}

void loop() {
  if(modo_config)
  {
    server.handleClient();
    return;
  }

  mqtt_client.loop();

  if(WiFi.status() != WL_CONNECTED)
  {
    connect_wifi();
  }
  if(!mqtt_client.connected())
  {
    connect_mqtt();
  }
  if(digitalRead(BUT_RES))
  {
    reiniciar();
    delay(300);
  }
  
  if(digitalRead(BUT_STO))
  {
    paused = !paused;
    diz_pause();
    delay(300);
  }
  if(paused)
  {
    delay(DELAY);
    return;
  }

  if(cont >= (quartil + 1) * (TIME / 4) && cont < TIME)
  {
    quartil = cont / (TIME / 4);
    escreve_nvs(cont);
  }else if(cont >= TIME && !buzon)
  {
    escreve_nvs(cont);
    tone(BUZ, 1200);
    buzon = true;
  }

  delay(DELAY);
  
  if(cont < TIME)
  {
    cont += DELAY;
    mqtt_client.publish("hori/m1/cont", String(cont).c_str());
  }
  Serial.printf("%lld\n", cont);
}

void modo_config_start()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("HORIMETRO", "12345678");
  server.on("/", [](){
    server.send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
    <html>
      <head>
        <meta charset='utf-8'>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <title>Configurar WiFi Horímetro</title>
      </head>
      <body>
        <h1> Configurar WiFi Horímetro </h1>
        <form action='/save'>
          SSID: <input name='s' type='text'>
          Password: <input name='p' type='text'><br>
          <input type='submit'>
        </form>
      </body>
    </html>
    )rawliteral");
  });
  server.on("/save", [](){
    String ssid = server.arg("s");
    String pass = server.arg("p");
    
    prefs.begin("wifi", false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.end();

    server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset='utf-8'>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <title>Configurar WiFi Horímetro</title>
      </head>
      <body>
        <h1> Configurar WiFi Horímetro </h1>
        <p>Salvo, reiniciando...</p>
      </body>
    </html>
    )rawliteral");
    delay(1000);

    ESP.restart();
  });

  server.begin();
}

void escreve_nvs(long long c)
{
  prefs.begin("hori", false);
  prefs.putLong("cont", c);
  prefs.end();
}

bool connect_wifi()
{
  prefs.begin("wifi", false);
  String Sssid = prefs.getString("ssid", "0");
  String Spass = prefs.getString("pass", "0");
  prefs.end();
  if(Sssid == "0" || Spass == "0")
  {
    modo_config = true;
    modo_config_start();
    return false;
  }
  Serial.print("Connecting WiFi in ");
  Serial.print(Sssid);
  Serial.printf(".");
  WiFi.begin(Sssid.c_str(), Spass.c_str());
  int tent = 0;
  while(WiFi.status() != WL_CONNECTED && tent < 50)
  {
    tent++;
    delay(200);
  }
  if(WiFi.status() != WL_CONNECTED)
  {
    modo_config = true;
    modo_config_start();
    return false;
  }
  return true;
}

void connect_mqtt()
{
  while(!mqtt_client.connected())
  {
    mqtt_client.connect(mqtt_client_id, mqtt_username, mqtt_password);
    delay(200);
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  char msg[64];
  if(length > sizeof(msg)) length = sizeof(msg) - 1;

  memcpy(msg, payload, length);
  msg[length] = '\0';
  String mensagem = String(msg);
  
  if(strcmp(topic, "hori/m1/pause") == 0)
  {
    paused = !paused;
    diz_pause();
  }

  if(strcmp(topic, "hori/m1/reiniciar") == 0 && mensagem.indexOf("reinicie") >= 0)
  {
    reiniciar();
  }
}

void reiniciar(){
  cont = 0;
  quartil = 0;
  escreve_nvs(cont);
  noTone(BUZ);
  buzon = false;
  paused = false;
  diz_pause();
}

void diz_pause()
{
  if(paused)
    mqtt_client.publish("hori/m1/diz_pause", "pausado");
  else
    mqtt_client.publish("hori/m1/diz_pause", "rodando");

}
