// Intelligent POWER MANAGEMENT
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <BlynkSimpleEsp32.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS

//char auth[] = "6G-rJ1HYuYTMVF-8XehOA1q-DFP1jvsJ";   //Harsha
char auth[] = "VhtW_rpX16sipQb60iynk-4iLCqrBuXY";   //Sai Teja
//char auth[] = "dW63OGnAWH-acygaksauxFJv051RiTOE";   //Sainadh

const char* ssid = "ANONYMOUS";
const char* password = "smts@Kothakota#0813";

//const char* ssid = "KT";
//const char* password = "password";


const char* PARAM_INPUT_1 = "input1";
String bat_level;
String bat_charge_status;
int state,upper_limit,xy=0,a=0,blynk_access=0;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
  <title>Intelligent Power Management</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
  setInterval(timer,2000);
  function timer()
  {
  var form=document.getElementById("abc");
  navigator.getBattery().then(function(battery)
    {
    var level=(battery.level)*100;
    var check=(battery.charging) ? "true":"false";
    document.getElementById("a1").value=level+","+check;
    form.submit();
    });
  }  
</script>
</head>
<body align="center">
<h1> Intelligent Power Management</h1>
  <form action="/get" id="abc">
    <input type="text" name="input1" id="a1">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
WidgetLCD lcd(V15);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password);
  Blynk.virtualWrite(V3,1);
  Blynk.virtualWrite(V2,"--");
  lcd.clear();
  lcd.print(3, 0, "INTELLIGENT ");
  lcd.print(0, 1, "POWER MANAGEMENT");
  delay(3000);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    digitalWrite(2, LOW);
  }
  digitalWrite(2, HIGH);
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.print(0, 0, "WiFi CONNECTED");
  lcd.print(0, 1, "->");
  lcd.print(3, 1, ssid);
  delay(3000);
  lcd.clear();
  if (WiFi.localIP())
  {
    IPAddress x = WiFi.localIP();
    lcd.print(0, 0, "IP ADDREESS ");
    lcd.print(0, 1, x[0]);
    lcd.print(3, 1, ".");
    lcd.print(4, 1, x[1]);
    lcd.print(7, 1, ".");
    lcd.print(8, 1, x[2]);
    lcd.print(11, 1, ".");
    lcd.print(12, 1, x[3]);
  }
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<bat_level>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/get?input1=<bat_level>
    if (request->hasParam(PARAM_INPUT_1))
    {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      for (int i = 0; i < inputMessage.length(); i++)
      {
        if (inputMessage.substring(i, i + 1) == ",")
        {
          bat_level = inputMessage.substring(0, i);
          bat_charge_status = inputMessage.substring(i + 1);
          break;
        }
      }
    }
    else
    {
      bat_level = "No message sent";
    }

    request->send(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();
}

BLYNK_WRITE(V3)         // Button 
{
  state = param.asInt();
  if (state == 0)
  {
    if( bat_level.toInt()< 80)
    {
      blynk_access = 0;
      digitalWrite(5, LOW);
      Serial.println("Button ON");
      lcd.clear();
      lcd.print(0,0,"Charging");
    }
    else
    {
      blynk_access = 2;
      digitalWrite(5, LOW);
      Serial.println("Button ON");
      lcd.clear();
      lcd.print(0,0,"Charging");
    }
  }
  else
  {
    blynk_access = 1;
    digitalWrite(5, HIGH);
    Serial.println("Button OFF");
    lcd.clear();
    lcd.print(0,0,"not charging");
  }
}

BLYNK_WRITE(V5)         //Threshold Value
{
    blynk_access = 1;
    int x = 0;
    int sr = 0;
    upper_limit = param.asInt();
    if(upper_limit >= bat_level.toInt())
    {
      digitalWrite(5,LOW);          //Relay ON
      Blynk.virtualWrite(V3,0);         //Switch ON
      lcd.clear();
      lcd.print(0, 0, "Threshold Value");
      lcd.print(0, 1, "set to : ");
      lcd.print(11, 1, upper_limit);
      Blynk.virtualWrite(V2, upper_limit);
      delay(2000);
      Serial.print("Threshold Value : ");
      Serial.println(upper_limit);
      while (upper_limit > bat_level.toInt())
      {
        if (bat_charge_status == "true")
        {
          x = 0;
          sr = 0;
          lcd.clear();
          lcd.print(0, 0, "Charger");
          lcd.print(0, 1, "Connected     :)");
          Blynk.virtualWrite(V1, bat_level);      
          digitalWrite(5, LOW);         //Relay ON
          Blynk.virtualWrite(V3,0);         //Switch ON
          Serial.print("Charging..!  Current Battery Level : ");
          Serial.println(bat_level);
          delay(2000);
        }
        else if (bat_charge_status == "false")
        {
          x = x + 1;
          Serial.println("Charger Disconnected..!");
          lcd.clear();
          lcd.print(0, 0, "Charger");
          lcd.print(0, 1, "Not Connected :(");
          delay(2000);
          if (x == 15)
          {
            sr = 1;
            digitalWrite(5, HIGH);
            Blynk.virtualWrite(V3, 1);
            Blynk.virtualWrite(V2,"--");
            Serial.println("Socket Powering OFF");
            lcd.clear();
            lcd.print(0, 0, "Socket");
            lcd.print(0, 1, "Powering OFF");
            delay(5000);
            lcd.clear();
            lcd.print(3, 0, "INTELLIGENT ");
            lcd.print(0, 1, "POWER MANAGEMENT");
            break;
          }
        }
      }
    }
    else
    {
      sr = 1;
      if (bat_charge_status == "true")
      {
        if(bat_level.toInt() == 100)
        {
          Blynk.virtualWrite(V3, 1);
          digitalWrite(5,HIGH);
          lcd.clear();
          lcd.print(0, 0, "Completely");
          lcd.print(0, 1, "Charged  :)");
          delay(5000);
        }
        else
        {
          Blynk.virtualWrite(V3, 1);
          digitalWrite(5,HIGH);
          lcd.clear();
          lcd.print(0, 0, "Enter a value");
          lcd.print(0, 1, "more than ");
          lcd.print(12, 1, bat_level);
          delay(3000);
        }
      }
      else
      {
        lcd.clear();
        lcd.print(0, 0, "Connect to a");
        lcd.print(0, 1, "Power Supply..!");
        delay(3000);
      }
    }
    if (sr == 0)
    {
      digitalWrite(5, HIGH);
      Blynk.virtualWrite(V3, 1);
      Blynk.virtualWrite(V2,"--");
      Serial.println("Successfully charged your device  :D");
      Serial.println("Socket Powering OFF.....");
      lcd.clear();
      lcd.print(0, 0, "Successfully ");
      lcd.print(0, 1, "Charged : ");
      lcd.print(11,1, upper_limit);
      lcd.print(15,1,"%");
      delay(2500);
      lcd.clear();
      lcd.print(3, 0, "INTELLIGENT ");
      lcd.print(0, 1, "POWER MANAGEMENT");
    }
}

void loop() {
  Blynk.run();
  Blynk.virtualWrite(V1, bat_level);
  if( blynk_access == 0 && bat_level.toInt() > 80 )
  {
    digitalWrite(5,HIGH);
    Blynk.virtualWrite(V3, 1); 
    lcd.clear();
    lcd.print(0,0,"Not Charging");        
  }
  else if(( blynk_access == 0 || blynk_access == 2 ) && bat_level.toInt() < 30 )
  {
    digitalWrite(5,LOW);
    Blynk.virtualWrite(V3, 0); 
    lcd.clear();
    lcd.print(0,0,"Charging");     
  }
  else if(blynk_access == 2 && bat_level.toInt() < 100) 
  {
    digitalWrite(5,LOW);
    Blynk.virtualWrite(V3, 0); 
    lcd.clear();
    lcd.print(0,0,"Charging");
  }
  if(bat_level.toInt() == 100)
  {
    lcd.clear();
    lcd.print(0,0,"Completely");
    lcd.print(0,1,"Charged...!");
    digitalWrite(5,HIGH);
    Blynk.virtualWrite(V3, 0);          //Switch OFF
    delay(3000);
    lcd.clear();
    lcd.print(3, 0, "INTELLIGENT ");
    lcd.print(0, 1, "POWER MANAGEMENT");
  }
}
