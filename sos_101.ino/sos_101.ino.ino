/* Emergency SOS B01
 * Upload SOS_clear_eeprom.inp for clear EEPROM
 * 
Edit For new SET
1.gennerrate LINE_TOKEN   name:     ⚠️เกิดเหตุการณ์⚠️:        forGroup: SOS.B    M5NEV45ijGIbL1C8cvVy5VJfTwLy0AizBYunSmG8CYE
             LINE_TOKENX  name:     ⚠️SOS:             forGroup: SOS.B    KGOamNgeqBtBpNt1oRGi6aJeJSZiJm8gETOJH9HJ06d
2.Edit #define LINE_TOKEN , #define LINE_TOKENX
3.Replace SOS.B with spacific id

Edit for subset
1.Replace B01 with spacific id**
  
PIN
1.ledOnline 2 //D4
2.SW D3
3.Relay D2  //cylen

*/

#define LINE_TOKEN "M5NEV45ijGIbL1C8cvVy5VJfTwLy0AizBYunSmG8CYE"  //line: SOS-B01   ⚠️เกิดเหตุการณ์⚠️:
#define LINE_TOKENX "KGOamNgeqBtBpNt1oRGi6aJeJSZiJm8gETOJH9HJ06d"        // for setup and update   line: SOS-B01      ⚠️SOS:
#define ledOnline 2
#define SW D3
#define Relay D2

void Line_Notify(String message) ;
void Line_Notify1(String message) ;
void Line_Notify2(String message) ;
void Line_Notify3(String message) ;

#include <WiFiClientSecureAxTLS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <Ticker.h>

WiFiClient client;
ESP8266WebServer server(80);
Ticker ticker; 

String Argument_Name;
unsigned long previousMillis = 0;
unsigned long previousMillisAleart = 0;
String unit,location,line1,line2,line3;
int len,len1,len2,len3,len4;
int pressed = 0,updateData = 0;
char htmlResponse[3000];

String EEPROM_read(int index, int length) {
  String text = "";
  char ch = 1;

  for (int i = index; (i < (index + length)) && ch; ++i) {
    if (ch = EEPROM.read(i)) {
      text.concat(ch);
    }
  }
  return text;
}

int EEPROM_write(int index, String text) {
  for (int i = index; i < text.length() + index; ++i) {
    EEPROM.write(i, text[i - index]);
  }
  EEPROM.write(index + text.length(), 0);
  EEPROM.commit();

  return text.length() + 1;
}

void tick()
{
  //toggle state
  int state = digitalRead(ledOnline);  // get the current state of GPIO1 pin
  digitalWrite(ledOnline, !state);     // set pin to the opposite state
}
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup() {

  Serial.begin(115200);

  pinMode(ledOnline, OUTPUT);

  pinMode(Relay, OUTPUT); 
  digitalWrite(Relay, LOW);
  
  ticker.attach(0.6, tick);

  pinMode(SW, INPUT);
  attachInterrupt(SW, Press, CHANGE);
 


  EEPROM.begin(512);
//  EEPROM_write(0,unit);
//      EEPROM_write(50,location);
//      EEPROM_write(100,line);
//      EEPROM_write(150,line2);
//      EEPROM_write(200,line3);
      
//  EEPROM_write(0,"0");
//  EEPROM_write(50,"0");
//  EEPROM_write(100,"0");
//  EEPROM_write(150,"0");
//  EEPROM_write(200,"0");

  unit = EEPROM_read(0, 50);    
  location = EEPROM_read(50, 50);
  line1 = EEPROM_read(100, 50);
  line2 = EEPROM_read(150, 50);
  line3 = EEPROM_read(200, 50);
  
  Serial.print("unit: "); Serial.println(unit);
  Serial.print("location: "); Serial.println(location);
  Serial.print("line1: ");    Serial.println(line1);
  Serial.print("line2: ");    Serial.println(line2);
  Serial.print("line3: ");    Serial.println(line3);
  
  
  WiFiManager wifiManager; 
//  wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("SOS-B01");
  
  if (!wifiManager.autoConnect("SOS-B01")) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }
  Serial.println("connected...yeey :)");
//  ESP.reset();   //*****************
  Line_NotifyX("SOS-B01 : Online\n");
  String msg_setup = "SETUP!!B01⚠️\n1.Connect wifi to : "+WiFi.SSID()+"\n2.Click http://"+WiFi.localIP().toString()+"\n3.Fill Unit,Location,Line-token and password then click SAVE";
  Line_NotifyX(msg_setup);

  
  delay(1000);
  ticker.detach();
  digitalWrite(ledOnline, LOW);
  
 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("WiFi connected..");
  server.begin(); Serial.println("Webserver started..."); 
  Serial.print("Use this URL to connect: http://");
  Serial.print(WiFi.localIP());Serial.println("/");
//  while (WiFi.status() != WL_CONNECTED)
  
  server.on("/", HandleClient); 
  server.on("/result", ShowClientResponse);
  Serial.println ( "HTTP server started" );

  pressed = 0;

}

void HandleClient() {
  String webpage;
  webpage =  "<html>";
//  webpage += "<form method='POST' action='/result' enctype='multipart/form-data'>";
//  webpage += "<head><title>Emergency SOS</title>";
//  webpage += "<style>";
//  webpage += "body { background-color: #E6E6FA; font-family: Arial, Helvetica, Sans-Serif; Color: #CC3300;}";
//  webpage += "</style>";
//  webpage += "</head>";

  webpage += "<head>";
  webpage += "<meta charset=\"utf-8\">";
  webpage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  webpage += "</head>";

  
  webpage += "<body>";
  webpage += "<h1><br>Emergency SOS</h1>";  
  String IPaddress = WiFi.localIP().toString();
  webpage += "<form action='http://"+IPaddress+"' method='POST'>";

  Serial.println("read......");
  String xunit = EEPROM_read(0, 50);
  String xlocation = EEPROM_read(50,50);
  webpage += "Unit &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; : <input type='text' value="+xunit+" name='unit_input'><BR>";
  webpage += "Location : <input type='text' value="+xlocation+" name='location_input'>";
//  webpage += "Password : <input type='text' name='password_input'>";
  
  String xline1 = EEPROM_read(100, 50);
  String xline2 = EEPROM_read(150, 50);
  String xline3 = EEPROM_read(200, 50);
  

  webpage += "<br>Line1 &nbsp;&nbsp;&nbsp;&nbsp; : <input type='text'value="+xline1+" name='line_input'>"; 
  webpage += "<br>Line2 &nbsp;&nbsp;&nbsp;&nbsp; : <input type='text'value="+xline2+" name='line2_input'>"; 
  webpage += "<br>Line3 &nbsp;&nbsp;&nbsp;&nbsp; : <input type='text'value="+xline3+" name='line3_input'>"; 
  webpage += "<br>Password: <input type='password' name='password_input'>";
  

//  String sta_pass = "wwwwwwww";
//  webpage += "<br>"+sta_pass;
//  webpage += "<br>&nbsp;<input type='submit' value='Enter'>"; // omit <input type='submit' value='Enter'> for just 'enter'

  webpage +=  "<br>&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"result\"><button>SAVE</button></a>";
  webpage += "</form>";
  webpage += "</body>";
  webpage += "</html>";
  server.send(200, "text/html", webpage); // Send a response to the client asking for input

  String unit,location,password,line,line2,line3;
  
  if (server.args() > 0 ) { // Arguments were received
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      Serial.println(server.argName(i)); // Display the argument
      Argument_Name = server.argName(i);
      if (server.argName(i) == "unit_input") {
        unit = server.arg(i);   //server.arg(i).toInt();   server.arg(i).toFloat();
      }
      if (server.argName(i) == "location_input") {
        location = server.arg(i);   //server.arg(i).toInt();   server.arg(i).toFloat();
      }
      if (server.argName(i) == "password_input") {
        password = server.arg(i);
      }
      if (server.argName(i) == "line_input") {
        line = server.arg(i);     
      }
      
      if (server.argName(i) == "line2_input") {
        line2 = server.arg(i);

      }
      if (server.argName(i) == "line3_input") {
        line3 = server.arg(i);

        
      }
      
    }
  }

  if((server.args()!=0)){
    if(String(password)=="signal"){
      Serial.print("Write Data:...");
      Serial.print("unit: ");  Serial.println(unit);
      Serial.print("location: ");  Serial.println(location);
      Serial.print("line: ");  Serial.println(line);
      Serial.print("line2: ");  Serial.println(line2);
      Serial.print("line3: ");  Serial.println(line3);
      
      EEPROM_write(0,unit);
      EEPROM_write(50,location);
      EEPROM_write(100,line);
      EEPROM_write(150,line2);
      EEPROM_write(200,line3);
      
      
      
      
//      EEPROM_write(25,"aaaa");
  
      Serial.println("read......");
      unit = EEPROM_read(0, 50);
      location = EEPROM_read(50, 50);
      line1 = EEPROM_read(100, 50);
      line2 = EEPROM_read(150, 50);
      line3 = EEPROM_read(200, 50);
      
      
      Serial.print("unit: ");  Serial.println(unit);
      Serial.print("location: ");  Serial.println(location);
      Serial.print("line: ");  Serial.println(line);
      Serial.print("line2: ");  Serial.println(line2);
      Serial.print("line3: ");  Serial.println(line3);
      
  
      Line_NotifyX("UPDATE!!B01⚠️\nUnit : "+unit+"\nLocation : " + location + "\nLine1 : " + line + "\nLine2 : " + line2 +"\nLine3 : " + line3 );
    }else{
      Serial.print("Wrong password");
      Line_NotifyX("UPDATE!!B01⚠️\nWrong password!!");
    }
  }
  
}

void ShowClientResponse() {
  String webpage;
  webpage =  "<html>";
  webpage += "<head><title>aaaaaaESP8266 Input Example</title>";
  webpage += "<style>";
  webpage += "body { background-color: #E6E6FA; font-family: Arial, Helvetica, Sans-Serif; Color: blue;}";
  webpage += "</style>";
  webpage += "</head>";
  webpage += "<body>";
  webpage += "<h1><br>ESP8266 Server - This was what the client sent</h1>";
//  webpage += "<p>Name received was: " + Clients_Response1 + "</p>";
//  webpage += "<p>Address received was: " + Clients_Response2 + "</p>";
  webpage += "</body>";
  webpage += "</html>";
  server.send(200, "text/html", webpage); // Send a response to the client asking for input
}

void loop() {

  while(WiFi.status() != WL_CONNECTED){
    Serial.println("off line");
//    Line_NotifyX("SOS-B01 : OFFline");
    while(WiFi.status() != WL_CONNECTED){
      Serial.println("off line inside");
      delay(600);
      ticker.attach(0.6, tick);
    }
    ticker.detach();
    digitalWrite(ledOnline, LOW);
    Line_NotifyX("SOS-B01 : ONline");
    
//    WiFiManager wifiManager;
//    if (!wifiManager.autoConnect("SOS-B01")) {
//      Serial.println("failed to connect and hit timeout");
//      ESP.reset();
//      delay(1000);
//    }
  }

//  digitalWrite(ledOnline, LOW);


  
  server.handleClient();
  Serial.println(pressed);
  delay(50);

  Serial.println("read......");
  unit = EEPROM_read(0, 50);
  location = EEPROM_read(50, 50);
  line1 = EEPROM_read(100, 50);
  line2 = EEPROM_read(150, 50);
  line3 = EEPROM_read(200, 50);
  
  
  
  if(pressed==1){
    Serial.println("Down..............");
    digitalWrite(Relay, HIGH); // on
//    digitalWrite(ledOnline, 1);
    ticker.attach(0.1, tick);
    digitalWrite(ledOnline, LOW);
    pressed = 2;

    String msg_setup = "SETUP!!B01⚠️\n1.Connect wifi to : "+WiFi.SSID()+"\n2.Click http://"+WiFi.localIP().toString()+"\n3.Fill Unit,Location,Line-token and password then click SAVE";
    Line_NotifyX(msg_setup);
    
    String msg = ""; 
    msg += "\nสถานที่ : " + unit + "\n";
    msg += "บริเวณ : " + location + "\n";
    msg += "‼️ต้องการความช่วยเหลือ‼️";

    while(pressed == 2){
      Serial.println("Aleart");
      Line_Notify(msg);
      delay(25);   
      Line_Notify1(msg);
      delay(25);
      Line_Notify2(msg);
      delay(25);
      Line_Notify3(msg);
      delay(25);
      
    }
  

  }

  if(pressed==3){
    Serial.println("UP..............");
    digitalWrite(Relay, LOW); // off
    digitalWrite(ledOnline, HIGH);
    ticker.detach();
    digitalWrite(ledOnline, 0);
    pressed = 0;
  }

//  if((millis()-previousMillisAleart > 1000*15)&&((pressed==7)||(pressed==9)||(pressed==10))){
//    Serial.println("OUT Aleart............");
//    ticker.detach();   
//    digitalWrite(ledOnline, 0);
//    pressed = 0;
//  }

  
//  if((pressed==2)&&(millis()-previousMillis > 3000)){
//    pressed = 5;
//    Serial.println("Sent..Setting .............");
//    digitalWrite(ledOnline, 0);
//    String msg_setup = "SETUP!!B01⚠️\n1.Connect wifi to : "+WiFi.SSID()+"\n2.Click http://"+WiFi.localIP().toString()+"\n3.Fill Unit,Location,Line-token and password then click SAVE";
//    Line_NotifyX(msg_setup);  
//    
//  }
//  if(pressed==3){
//    Serial.println("Up..and..Aleart..........");
//    digitalWrite(ledOnline, 0);
//    ticker.attach(0.1, tick);
//    String msg = ""; 
//    msg += "\nสถานที่ : " + unit + "\n";
//    msg += "บริเวณ : " + location + "\n";
//    msg += "‼️ต้องการความช่วยเหลือ‼️";
//        
//    Line_Notify(msg);
//    Line_Notify1(msg);
//    Line_Notify2(msg);
//    Line_Notify3(msg);
//    
//    
//    previousMillisAleart = millis();
//    pressed = 7;
//    
//  }
//
//  if(pressed==9){
//    pressed = 10;
//    String msg2 = "";
//    msg2 += "\nสถานที่ : " + unit + "\n";
//    msg2 += "บริเวณ : " + location + "\n";
//    msg2 += "‼️ต้องการความช่วยเหลือ‼️";
//    Line_Notify(msg2);
//    Line_Notify1(msg2);
//    Line_Notify2(msg2);
//    Line_Notify3(msg2);
//    
//    
//  }

    
}

 
void Press(){
  
  if(pressed == 0){
    previousMillis = millis();
    Serial.println("downnn ");
    pressed = 1;

  }
  if(pressed == 2){
    Serial.println("uppppp ");
    pressed = 3;
  }
  if(pressed == 5){
    pressed = 0;
  }
  if(pressed == 7){
    pressed = 9;
  }
  if(pressed == 10){
    pressed = 7;
  }
   

}

void Line_Notify(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}

void Line_Notify1(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  
  String LINE_TOKEN1 = EEPROM_read(100, 50);
  
  req += "Authorization: Bearer " + String(LINE_TOKEN1) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}

void Line_Notify2(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  
  String LINE_TOKEN2 = EEPROM_read(150, 50);

 
  
  req += "Authorization: Bearer " + String(LINE_TOKEN2) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}

void Line_Notify3(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  
  String LINE_TOKEN3 = EEPROM_read(200, 50);
  
  req += "Authorization: Bearer " + String(LINE_TOKEN3) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}

void Line_NotifyX(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  
//  String LINE_TOKENX = EEPROM_read(200, 50);
  
  req += "Authorization: Bearer " + String(LINE_TOKENX) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}


