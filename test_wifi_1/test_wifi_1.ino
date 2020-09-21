#include <HTTP_Method.h>
#include <WebServer.h>


#include <ArduinoJson.h>

#include <HTTPClient.h>

#include <string.h>

#include <WiFi.h>
#include <WiFiClient.h>

#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <strings_en.h>
#include <WiFiManager.h>


const char* ssid = "ESP32AP";
const char* password = "esp32test";
const uint16_t port = 8080;
boolean full;
boolean empty;  
String MAC;
String serverName;
IPAddress IP;

int filling;
WiFiServer wifiServer(6666);
WebServer webServer(80);
WiFiManager wm;

//WebSocketsServer webSocketServer;


IPAddress serverAdress;


void ConnectToWifi(){
  WiFi.mode(WIFI_STA);
  if (!wm.autoConnect(ssid, password))
    Serial.println("Erreur de connexion.");
  else
    Serial.println("Connexion etablie!");
    MAC= WiFi.macAddress();
   IP= WiFi.localIP();
}

void getIpAdress(){
  wifiServer.begin();
  WiFiClient client = wifiServer.available();
  if (client) {
    

    Serial.println("Client connected with IP:");
    Serial.println(client.remoteIP());
    serverAdress = client.remoteIP();
    serverName = "http://" +serverAdress.toString()+":8080/bennes";
    uint8_t ReceiveBuffer[30];
    
    
    int c = client.read(ReceiveBuffer, 30);
    String coucou = (char*)ReceiveBuffer;
    //coucou.toCharArray
    Serial.println("The first message is: ");
    Serial.println(coucou);
    client.stop();
    wifiServer.close();
    webServer.begin();
    Serial.println("Server start");
  }
}

void firstConnectionApi(){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      String serverPath = serverName+"/"+MAC;
      http.begin(serverPath.c_str());
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        if (payload == ""){
          full = false;
          empty = false;
          HTTPClient http1;
          http1.begin(serverName.c_str());
          String first = "{\n        \"name\": \"Dumpster\",\n        \"connected\": true,\n        \"ip\": \""+ IP.toString()+"\",\n        \"id\": \""+MAC+"\",\n        \"emptyInitialised\": false,\n        \"fullInitialised\": false\n    }\n    ";
          http1.addHeader("Content-Type", "application/json");
          int httpResponseCode1 = http1.POST(first);
          Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode1);
        String payload1 = http1.getString();

        
        
    
          }
          else{
            Serial.print("payload in database ");
            Serial.println(payload);
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            JsonObject obj = doc.as<JsonObject>();
            empty = obj[String("emptyInitialised")].as<boolean>();
            full = obj[String("fullInitialised")].as<boolean>();
            }
        
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
      
  }
  else {
      Serial.println("WiFi Disconnected");
    }
      
}

/*char waitingMessage(){
  wifiServer.begin();
  WiFiClient client = wifiServer.available();
  




  if (client.connected() && webSocketServer.handshake(client)) {
    Serial.print("Client connected with IP:");
    Serial.println(client.remoteIP());
    String data;
    while(client.connected()){
      
        data = webSocketServer.getData();
        if (data.length()>0){
          Serial.println(data);
          webSocketServer.sendData(data);
        }
    if(data =="0"){
      Serial.println("data == 0");     
      empty = true;
      return(data);
    }
    else if(data =="1"){
      Serial.println("ch == 1");
      
      full = true;
      return data;
    }
    return data;
      
    }

  

    client.stop();
    wifiServer.close();
    
  }
  return '9';
  
}*/


void handle_empty(){
  //send 0 to arduino to emptyinitialisation
  Serial.println("handle_empty");
  webServer.send(200,"text/html", "empty");
  empty = true;
  
}

void handle_full(){

  //send 1 to arduino
  Serial.println("handle_full");
  webServer.send(200,"text/html", "full");
  full = true;
  
}


void setup() {
  Serial.begin(9600);
  webServer.on("/empty",handle_empty);
  webServer.on("/full",handle_full);
  webServer.onNotFound(handle_full);
  
  delay(1000);
  Serial.println("\n");
  ConnectToWifi();  
}

void loop() {
  if(!serverAdress){
    getIpAdress();
    firstConnectionApi();
    
  }else{
    while(!(empty||full)){
    webServer.handleClient();
    }
  
  }
  
  
  
  
  Serial.print("Empty :");
  Serial.println(empty);
  Serial.print("full :");
  Serial.println(full);
  delay(4000);
  

}
