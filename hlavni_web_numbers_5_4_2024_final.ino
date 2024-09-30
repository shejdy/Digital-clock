#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>
#include <Wire.h>






#define I2C_SDA 22
#define I2C_SCL 21
#define ONE_WIRE_BUS 32
#define DATA_PIN 4
#define NUM_LEDS 86
#define BRIGHTNESS 255
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
int A;
int minjed;
int mindes;
int hodjed;
int hoddes;
int i = 21;
int i2 = 44;
int i3 = 23;
int color1 = 0;
int color2 = 255;
int color3 = 0;
int temp;
int jed;
int des;
float jasf = 0.5;
unsigned long previousMillis = 0;
int interval_1 = 10000;
int interval_2 = 5000;
bool ledState = false;
bool function1Active = true;
const unsigned long interval = 1000;
unsigned long previousMillis2 = 0;
int countdown;
int seccountdown;
int mincountdown;
int secodp;
int minodp;
int hodod;
int rtctime = 0;
int hodiny2 = 0;
int minuty2 = 0;
int sekundy2 = 0;
int akttime = 0;
int akthodiny2 = 0;
int aktminuty2 = 0;
int aktsekundy2 = 0;

String color;
String jas;
String hod;
String display_time;
String display_temperature;
String odpocet;
String cas;
String pocet_blik;
String aktualnicas;
String secodp2;
String minodp2;
String hododp2;


WebServer server(80);
RTC_DS3231 rtc;

CRGB leds[NUM_LEDS];
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define UPDATES_PER_SECOND 100


const char* ssid = "HODINY";
const char* password = "DilanD7";

IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
char daysOfWeek[7][12] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};
const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ovládání hodin</title>
    <style>
        
        #time5 {
    font-size: 24px;
  }
  *{
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    color: rgb(30, 30, 30);
  }
        body {
            font-family: Arial, sans-serif;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            background-color: whitesmoke;
            height: 100vh;
            margin: 0;
        }
        body h1{
          text-align: center;
          font-size: 50px;
        }
        form{
          display: flex;
          flex-direction: column;
          background-color: white;
          gap: 15px;
          padding: 15px;
          border-radius: 15px;
          box-shadow: 0px 0px 5px 0px rgb(30, 30, 30);
        }
        .settings-top{
          display: flex;
          justify-content: center;
          flex-wrap: wrap;
          border-bottom: 2px solid rgba(30, 30, 30, 0.5);
          border-top: 2px solid rgba(30, 30, 30, 0.5);
        }
        .settings-top-left{
          padding: 15px;
          display: flex;
          flex-direction: column;
          gap: 15px;

          width: 500px;
        }
        .settings-top-right{
          padding: 15px;
          display: flex;
          flex-direction: column;
          gap: 15px;

          width: 500px;
        }

    
      #showTimeCheckbox{
        align-self: flex-start;
        height: 20px;
        width: 20px;
        accent-color: #118AB2;
      }
        form div div div{
          padding: 5px;
          display: flex;
          flex-direction: column;
          justify-content: center;
        }
        .real-time-settings{
          gap: 5px;
        }
        label {
            display: block;
            margin-bottom: 5px;
            font-size: 20px;
        }
        input, select{
          border: 2px solid #118AB2;
          background-color: transparent;
          border-radius: 15px;
          padding: 5px;
        }
       
        input[type="range"] {
            accent-color: #118AB2;
        }
        input[type="submit"]{
          height: 40px;
          width: 200px;
          align-self: center;
          font-size: 25px;
          background-color: #118AB2;
          color: white;
          transition: 0.2s ease-out;
          cursor: pointer;
        }
        input[type="submit"]:hover{
          background-color: transparent;
          color: rgb(30, 30, 30);
          transition: 0.2s ease-out;
        }


        
        @media only screen and (max-width: 530px){
          .settings-top-left{
            width: 300px;
          }
          .settings-top-right{
            width: 300px;
          }
          body{
            height: fit-content;
            align-items: flex-start;
          }
        }
    </style>
</head>
<body>
    <form method='POST' action='/message'>
        <h1>Ovládání hodin</h1>
          <div class="settings-top">
          <div class="settings-top-left">
            <div class="color-settings">
              <label>Barva číslic:</label>
                <select name="color" id="color">
                  
                  <option value="Red">Červená</option>
                  <option value="Green">Zelená</option>
                  <option value="Blue">Modrá</option>


              </select>
            </div>

            

            <div class="jas-settings">
              <label for="brightness">Nastavení jasu:</label>
              <input type="range" id="posuvnik1" name="posuvnik1" min="0" max="100" value="50" oninput="updateValue(this)">
              <input type="text" id="hodnota1" name="hodnota1" value="50" oninput="updateRange(this)">
            </div>
            
            <div class="time-settings">
              <label for="time-format">Nastavení aktuálního času (HH:MM:SS):</label>
              <input id="settime1" name="settime" type="time" step="1" value="00:00:00">
            </div>
            <div class="real-time-settings">
              <label for="showTimeCheckbox">Synchronizovat čas s časem v zařízení</label>

              
              <input type="checkbox" id="showTimeCheckbox" onchange="toggleTimeDisplay()">

              <input id="settime" name="time5" type="time" step="1" value="00:00:00" style="display:none;">
            </div>
              
            
          </div>
          <div class="settings-top-right">
            <div class="display-time-settings">
              <label for="display-time">Délka zobrazení času (sekundy):</label>
              <input type="number" id="display-time" name="display-time" value="10">
            </div>
            <div class="display-temp-settings">
              <label for="display-temperature">Délka zobrazení teploty (sekundy):</label>
              <input type="number" id="display-temperature" name="display-temperature" value="5">
            </div>
              
            
            <div class="time-settings">
            <label for="timeInput">Nastavení časovače (mm:ss):</label>
            <input type="text" id="timeInput" name="timeInput" placeholder="mm:ss" maxlength="5" oninput="formatTimeInput()">
            </div>
            
            
            
            <div class="blik-settings">
              <label for="display-pocetblik">Počet bliknutí při konci odpočtu:</label>
              <input type="number" id="display-pocetblik" name="display-pocetblik" value="5">
            </div>
          </div>
      </div>
        
        
        
        
        

        <script>

function formatTimeInput() {
    var timeInput = document.getElementById("timeInput").value;
    var formattedInput = timeInput.replace(/[^0-9]/g, ''); // Odstranit všechny znaky kromě čísel
    var length = formattedInput.length;

    if (length > 4) {
        formattedInput = formattedInput.substr(0, 4); // Případně zkrátit na 4 číslice
    }

    if (length > 2) {
        formattedInput = formattedInput.substr(0, 2) + ':' + formattedInput.substr(2); // Přidat dvojtečku mezi první a druhou číslicí
    }

    document.getElementById("timeInput").value = formattedInput;
}

function toggleTimeDisplay() {
  var checkbox = document.getElementById("showTimeCheckbox");
  var timeDisplay = document.getElementById("settime");
  
  if (checkbox.checked) {
    timeDisplay.style.display = "inline-block";
    timeDisplay.value = getCurrentTime();
    
    setInterval(function() {
      timeDisplay.value = getCurrentTime();
    }, 1000);
  } else {
    timeDisplay.style.display = "none";
    timeDisplay.value = "00:00:00";
  }
}

function getCurrentTime() {
  var now = new Date();
  var hours = now.getHours().toString().padStart(2, '0');
  var minutes = now.getMinutes().toString().padStart(2, '0');
  var seconds = now.getSeconds().toString().padStart(2, '0');
  return hours + ":" + minutes + ":" + seconds;
}


            function updateValue(input) {
                const value = input.value;
                const id = input.id.replace('posuvnik', 'hodnota');
                const textInput = document.getElementById(id);
                textInput.value = value;
            }
        </script>
        <input type="submit" value="Odeslat">
    </form>
</body>
</html>




)rawliteral";

void handleRoot() {

  server.send(200, "text/html", html);
}

void handleMessage() {


  jas = server.arg("hodnota1");
  color = server.arg("color");
  hod = server.arg("settime");
  display_time = server.arg("display-time");
  display_temperature = server.arg("display-temperature");
  odpocet = server.arg("timeInput");
  cas = server.arg("settime");
  pocet_blik = server.arg("display-pocetblik");
  aktualnicas = server.arg("time5");

  String secodp2 = odpocet.substring(3, 5);
  int sekundy = secodp2.toInt();
  String minodp2 = odpocet.substring(0, 2);
  int minuty = minodp2.toInt();
  countdown = sekundy + minuty * 60;


  String rtcsek2 = cas.substring(6, 8);
  sekundy2 = rtcsek2.toInt();
  String rtcmin2 = cas.substring(3, 5);
  minuty2 = rtcmin2.toInt();
  String rtchod2 = cas.substring(0, 2);
  hodiny2 = rtchod2.toInt();
  rtctime = sekundy2 + minuty2 * 60 + hodiny2 * 3600;


  String aktsek2 = aktualnicas.substring(6, 8);
  aktsekundy2 = aktsek2.toInt();
  String aktmin2 = aktualnicas.substring(3, 5);
  aktminuty2 = aktmin2.toInt();
  String akthod2 = aktualnicas.substring(0, 2);
  akthodiny2 = akthod2.toInt();
  akttime = aktsekundy2 + aktminuty2 * 60 + akthodiny2 * 3600;


  interval_1 = display_time.toInt() * 1000;
  if(interval_1 == 0){
    interval_1 = 0;
  }
  else if (interval_1 < 500 ) {
    interval_1 = 10000;
  }

  interval_2 = display_temperature.toInt() * 1000;
  if(interval_2 == 0){
    interval_2 = 0;
  }
  else if (interval_2 < 500) {
    interval_2 = 5000;
  }
  Serial.println(interval_1);
  Serial.println(interval_2);





  Serial.println("barva: " + color);
  jasf = jas.toFloat();
  jasf = jasf / 100;
  Serial.println(jasf);
  server.send(200, "text/html", html);
  Serial.println(jas);
  Serial.println(color);
  Serial.println(hod);
  Serial.println(display_time);
  Serial.println(display_temperature);
  Serial.println(odpocet);
  Serial.println(countdown);

  Serial.println(aktualnicas);

  Serial.println(hodiny2);
  Serial.println(minuty2);
  Serial.println(sekundy2);
}

void setup() {
  Serial.begin(115200);

  sensors.begin();


  Wire.begin(I2C_SDA, I2C_SCL);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);


  WiFi.softAP(ssid, password);


  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Nepodařilo se nastavit pevnou IP adresu pro přístupový bod.");
  }
  if (!rtc.begin()) {
    Serial.println("RTC module is NOT found");
    Serial.flush();
    while (1)
      ;
  }

  server.on("/", handleRoot);


  server.on("/message", HTTP_POST, handleMessage);

  server.begin();

  Serial.println("Přístupový bod spuštěn");
  Serial.println(WiFi.softAPIP());
}

void wcolor() {
  if (color == "Red") {
    color1 = 255;
    color2 = 0;
    color3 = 0;
  } else if (color == "Green") {
    color1 = 0;
    color2 = 255;
    color3 = 0;
  } else if (color == "Blue") {
    color1 = 0;
    color2 = 0;
    color3 = 255;
  }
}
void tecka() {
  leds[42] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
}
void teckaoff() {
  leds[42] = CRGB(0, 0, 0);
  leds[43] = CRGB(0, 0, 0);
}
void off() {
  leds[0] = CRGB(0, 0, 0);
  leds[1] = CRGB(0, 0, 0);
  leds[2] = CRGB(0, 0, 0);
  leds[3] = CRGB(0, 0, 0);
  leds[4] = CRGB(0, 0, 0);
  leds[5] = CRGB(0, 0, 0);
  leds[6] = CRGB(0, 0, 0);
  leds[7] = CRGB(0, 0, 0);
  leds[8] = CRGB(0, 0, 0);
  leds[9] = CRGB(0, 0, 0);
  leds[10] = CRGB(0, 0, 0);
  leds[11] = CRGB(0, 0, 0);
  leds[12] = CRGB(0, 0, 0);
  leds[13] = CRGB(0, 0, 0);
  leds[14] = CRGB(0, 0, 0);
  leds[15] = CRGB(0, 0, 0);
  leds[16] = CRGB(0, 0, 0);
  leds[17] = CRGB(0, 0, 0);
  leds[18] = CRGB(0, 0, 0);
  leds[19] = CRGB(0, 0, 0);
  leds[20] = CRGB(0, 0, 0);

  FastLED.show();
}
void one() {
  leds[0] = CRGB(0, 0, 0);
  leds[1] = CRGB(0, 0, 0);
  leds[2] = CRGB(0, 0, 0);
  leds[3] = CRGB(0, 0, 0);
  leds[4] = CRGB(0, 0, 0);
  leds[5] = CRGB(0, 0, 0);
  leds[6] = CRGB(0, 0, 0);
  leds[7] = CRGB(0, 0, 0);
  leds[8] = CRGB(0, 0, 0);
  leds[9] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(0, 0, 0);
  leds[16] = CRGB(0, 0, 0);
  leds[17] = CRGB(0, 0, 0);
  leds[18] = CRGB(0, 0, 0);
  leds[19] = CRGB(0, 0, 0);
  leds[20] = CRGB(0, 0, 0);

  FastLED.show();
}

void two() {
  leds[0] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3] = CRGB(0, 0, 0);
  leds[4] = CRGB(0, 0, 0);
  leds[5] = CRGB(0, 0, 0);
  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12] = CRGB(0, 0, 0);
  leds[13] = CRGB(0, 0, 0);
  leds[14] = CRGB(0, 0, 0);
  leds[15] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);

  FastLED.show();
}

void three() {
  leds[0] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3] = CRGB(0, 0, 0);
  leds[4] = CRGB(0, 0, 0);
  leds[5] = CRGB(0, 0, 0);
  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18] = CRGB(0, 0, 0);
  leds[19] = CRGB(0, 0, 0);
  leds[20] = CRGB(0, 0, 0);
  FastLED.show();
}
void four() {
  leds[0] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6] = CRGB(0, 0, 0);
  leds[7] = CRGB(0, 0, 0);
  leds[8] = CRGB(0, 0, 0);
  leds[9] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(0, 0, 0);
  leds[16] = CRGB(0, 0, 0);
  leds[17] = CRGB(0, 0, 0);
  leds[18] = CRGB(0, 0, 0);
  leds[19] = CRGB(0, 0, 0);
  leds[20] = CRGB(0, 0, 0);
  FastLED.show();
}

void five() {
  leds[0] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9] = CRGB(0, 0, 0);
  leds[10] = CRGB(0, 0, 0);
  leds[11] = CRGB(0, 0, 0);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18] = CRGB(0, 0, 0);
  leds[19] = CRGB(0, 0, 0);
  leds[20] = CRGB(0, 0, 0);
  FastLED.show();
}

void six() {
  leds[0] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9] = CRGB(0, 0, 0);
  leds[10] = CRGB(0, 0, 0);
  leds[11] = CRGB(0, 0, 0);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}

void seven() {
  leds[0] = CRGB(0, 0, 0);
  leds[1] = CRGB(0, 0, 0);
  leds[2] = CRGB(0, 0, 0);
  leds[3] = CRGB(0, 0, 0);
  leds[4] = CRGB(0, 0, 0);
  leds[5] = CRGB(0, 0, 0);
  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(0, 0, 0);
  leds[16] = CRGB(0, 0, 0);
  leds[17] = CRGB(0, 0, 0);
  leds[18] = CRGB(0, 0, 0);
  leds[19] = CRGB(0, 0, 0);
  leds[20] = CRGB(0, 0, 0);
  FastLED.show();
}

void eight() {
  leds[0] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);

  FastLED.show();
}

void nine() {
  leds[0] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18] = CRGB(0, 0, 0);
  leds[19] = CRGB(0, 0, 0);
  leds[20] = CRGB(0, 0, 0);

  FastLED.show();
}

void zero() {
  leds[0] = CRGB(0, 0, 0);
  leds[1] = CRGB(0, 0, 0);
  leds[2] = CRGB(0, 0, 0);
  leds[3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}

void off2() {
  int i = 21;
  leds[0 + i] = CRGB(0, 0, 0);
  leds[1 + i] = CRGB(0, 0, 0);
  leds[2 + i] = CRGB(0, 0, 0);
  leds[3 + i] = CRGB(0, 0, 0);
  leds[4 + i] = CRGB(0, 0, 0);
  leds[5 + i] = CRGB(0, 0, 0);
  leds[6 + i] = CRGB(0, 0, 0);
  leds[7 + i] = CRGB(0, 0, 0);
  leds[8 + i] = CRGB(0, 0, 0);
  leds[9 + i] = CRGB(0, 0, 0);
  leds[10 + i] = CRGB(0, 0, 0);
  leds[11 + i] = CRGB(0, 0, 0);
  leds[12 + i] = CRGB(0, 0, 0);
  leds[13 + i] = CRGB(0, 0, 0);
  leds[14 + i] = CRGB(0, 0, 0);
  leds[15 + i] = CRGB(0, 0, 0);
  leds[16 + i] = CRGB(0, 0, 0);
  leds[17 + i] = CRGB(0, 0, 0);
  leds[18 + i] = CRGB(0, 0, 0);
  leds[19 + i] = CRGB(0, 0, 0);
  leds[20 + i] = CRGB(0, 0, 0);
  FastLED.show();
}
void one2() {
  int i = 21;
  leds[0 + i] = CRGB(0, 0, 0);
  leds[1 + i] = CRGB(0, 0, 0);
  leds[2 + i] = CRGB(0, 0, 0);
  leds[3 + i] = CRGB(0, 0, 0);
  leds[4 + i] = CRGB(0, 0, 0);
  leds[5 + i] = CRGB(0, 0, 0);
  leds[6 + i] = CRGB(0, 0, 0);
  leds[7 + i] = CRGB(0, 0, 0);
  leds[8 + i] = CRGB(0, 0, 0);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(0, 0, 0);
  leds[16 + i] = CRGB(0, 0, 0);
  leds[17 + i] = CRGB(0, 0, 0);
  leds[18 + i] = CRGB(0, 0, 0);
  leds[19 + i] = CRGB(0, 0, 0);
  leds[20 + i] = CRGB(0, 0, 0);
  FastLED.show();
}
void two2() {
  int i = 21;
  leds[0 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i] = CRGB(0, 0, 0);
  leds[4 + i] = CRGB(0, 0, 0);
  leds[5 + i] = CRGB(0, 0, 0);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(0, 0, 0);
  leds[13 + i] = CRGB(0, 0, 0);
  leds[14 + i] = CRGB(0, 0, 0);
  leds[15 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}

void three2() {
  int i = 21;
  leds[0 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i] = CRGB(0, 0, 0);
  leds[4 + i] = CRGB(0, 0, 0);
  leds[5 + i] = CRGB(0, 0, 0);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i] = CRGB(0, 0, 0);
  leds[19 + i] = CRGB(0, 0, 0);
  leds[20 + i] = CRGB(0, 0, 0);
  FastLED.show();
}
void four2() {
  int i = 21;
  leds[0 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i] = CRGB(0, 0, 0);
  leds[7 + i] = CRGB(0, 0, 0);
  leds[8 + i] = CRGB(0, 0, 0);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(0, 0, 0);
  leds[16 + i] = CRGB(0, 0, 0);
  leds[17 + i] = CRGB(0, 0, 0);
  leds[18 + i] = CRGB(0, 0, 0);
  leds[19 + i] = CRGB(0, 0, 0);
  leds[20 + i] = CRGB(0, 0, 0);
  FastLED.show();
}
void five2() {
  int i = 21;
  leds[0 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(0, 0, 0);
  leds[10 + i] = CRGB(0, 0, 0);
  leds[11 + i] = CRGB(0, 0, 0);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i] = CRGB(0, 0, 0);
  leds[19 + i] = CRGB(0, 0, 0);
  leds[20 + i] = CRGB(0, 0, 0);
  FastLED.show();
}
void six2() {

  leds[0 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(0, 0, 0);
  leds[10 + i] = CRGB(0, 0, 0);
  leds[11 + i] = CRGB(0, 0, 0);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}
void seven2() {
  leds[0 + i] = CRGB(0, 0, 0);
  leds[1 + i] = CRGB(0, 0, 0);
  leds[2 + i] = CRGB(0, 0, 0);
  leds[3 + i] = CRGB(0, 0, 0);
  leds[4 + i] = CRGB(0, 0, 0);
  leds[5 + i] = CRGB(0, 0, 0);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(0, 0, 0);
  leds[16 + i] = CRGB(0, 0, 0);
  leds[17 + i] = CRGB(0, 0, 0);
  leds[18 + i] = CRGB(0, 0, 0);
  leds[19 + i] = CRGB(0, 0, 0);
  leds[20 + i] = CRGB(0, 0, 0);
  FastLED.show();
}
void eight2() {
  leds[0 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);

  FastLED.show();
}
void nine2() {
  leds[0 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i] = CRGB(0, 0, 0);
  leds[19 + i] = CRGB(0, 0, 0);
  leds[20 + i] = CRGB(0, 0, 0);

  FastLED.show();
}
void zero2() {
  leds[0 + i] = CRGB(0, 0, 0);
  leds[1 + i] = CRGB(0, 0, 0);
  leds[2 + i] = CRGB(0, 0, 0);
  leds[3 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}
void off3() {
  leds[0 + i2] = CRGB(0, 0, 0);
  leds[1 + i2] = CRGB(0, 0, 0);
  leds[2 + i2] = CRGB(0, 0, 0);
  leds[3 + i2] = CRGB(0, 0, 0);
  leds[4 + i2] = CRGB(0, 0, 0);
  leds[5 + i2] = CRGB(0, 0, 0);
  leds[6 + i2] = CRGB(0, 0, 0);
  leds[7 + i2] = CRGB(0, 0, 0);
  leds[8 + i2] = CRGB(0, 0, 0);
  leds[9 + i2] = CRGB(0, 0, 0);
  leds[10 + i2] = CRGB(0, 0, 0);
  leds[11 + i2] = CRGB(0, 0, 0);
  leds[12 + i2] = CRGB(0, 0, 0);
  leds[13 + i2] = CRGB(0, 0, 0);
  leds[14 + i2] = CRGB(0, 0, 0);
  leds[15 + i2] = CRGB(0, 0, 0);
  leds[16 + i2] = CRGB(0, 0, 0);
  leds[17 + i2] = CRGB(0, 0, 0);
  leds[18 + i2] = CRGB(0, 0, 0);
  leds[19 + i2] = CRGB(0, 0, 0);
  leds[20 + i2] = CRGB(0, 0, 0);
  FastLED.show();
}
void one3() {

  leds[0 + i2] = CRGB(0, 0, 0);
  leds[1 + i2] = CRGB(0, 0, 0);
  leds[2 + i2] = CRGB(0, 0, 0);
  leds[3 + i2] = CRGB(0, 0, 0);
  leds[4 + i2] = CRGB(0, 0, 0);
  leds[5 + i2] = CRGB(0, 0, 0);
  leds[6 + i2] = CRGB(0, 0, 0);
  leds[7 + i2] = CRGB(0, 0, 0);
  leds[8 + i2] = CRGB(0, 0, 0);
  leds[9 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(0, 0, 0);
  leds[16 + i2] = CRGB(0, 0, 0);
  leds[17 + i2] = CRGB(0, 0, 0);
  leds[18 + i2] = CRGB(0, 0, 0);
  leds[19 + i2] = CRGB(0, 0, 0);
  leds[20 + i2] = CRGB(0, 0, 0);
  FastLED.show();
}
void two3() {

  leds[0 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i2] = CRGB(0, 0, 0);
  leds[4 + i2] = CRGB(0, 0, 0);
  leds[5 + i2] = CRGB(0, 0, 0);
  leds[6 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i2] = CRGB(0, 0, 0);
  leds[13 + i2] = CRGB(0, 0, 0);
  leds[14 + i2] = CRGB(0, 0, 0);
  leds[15 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}

void three3() {

  leds[0 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i2] = CRGB(0, 0, 0);
  leds[4 + i2] = CRGB(0, 0, 0);
  leds[5 + i2] = CRGB(0, 0, 0);
  leds[6 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i2] = CRGB(0, 0, 0);
  leds[19 + i2] = CRGB(0, 0, 0);
  leds[20 + i2] = CRGB(0, 0, 0);
  FastLED.show();
}
void four3() {

  leds[0 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i2] = CRGB(0, 0, 0);
  leds[7 + i2] = CRGB(0, 0, 0);
  leds[8 + i2] = CRGB(0, 0, 0);
  leds[9 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(0, 0, 0);
  leds[16 + i2] = CRGB(0, 0, 0);
  leds[17 + i2] = CRGB(0, 0, 0);
  leds[18 + i2] = CRGB(0, 0, 0);
  leds[19 + i2] = CRGB(0, 0, 0);
  leds[20 + i2] = CRGB(0, 0, 0);
  FastLED.show();
}
void five3() {

  leds[0 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i2] = CRGB(0, 0, 0);
  leds[10 + i2] = CRGB(0, 0, 0);
  leds[11 + i2] = CRGB(0, 0, 0);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i2] = CRGB(0, 0, 0);
  leds[19 + i2] = CRGB(0, 0, 0);
  leds[20 + i2] = CRGB(0, 0, 0);
  FastLED.show();
}
void six3() {
  leds[0 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i2] = CRGB(0, 0, 0);
  leds[10 + i2] = CRGB(0, 0, 0);
  leds[11 + i2] = CRGB(0, 0, 0);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}
void seven3() {
  leds[0 + i2] = CRGB(0, 0, 0);
  leds[1 + i2] = CRGB(0, 0, 0);
  leds[2 + i2] = CRGB(0, 0, 0);
  leds[3 + i2] = CRGB(0, 0, 0);
  leds[4 + i2] = CRGB(0, 0, 0);
  leds[5 + i2] = CRGB(0, 0, 0);
  leds[6 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(0, 0, 0);
  leds[16 + i2] = CRGB(0, 0, 0);
  leds[17 + i2] = CRGB(0, 0, 0);
  leds[18 + i2] = CRGB(0, 0, 0);
  leds[19 + i2] = CRGB(0, 0, 0);
  leds[20 + i2] = CRGB(0, 0, 0);
  FastLED.show();
}
void eight3() {
  leds[0 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);

  FastLED.show();
}
void nine3() {
  leds[0 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i2] = CRGB(0, 0, 0);
  leds[19 + i2] = CRGB(0, 0, 0);
  leds[20 + i2] = CRGB(0, 0, 0);

  FastLED.show();
}
void zero3() {
  leds[0 + i2] = CRGB(0, 0, 0);
  leds[1 + i2] = CRGB(0, 0, 0);
  leds[2 + i2] = CRGB(0, 0, 0);
  leds[3 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[13 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[14 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[15 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20 + i2] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}

void off4() {

  leds[42 + i3] = CRGB(0, 0, 0);
  leds[43 + i3] = CRGB(0, 0, 0);
  leds[44 + i3] = CRGB(0, 0, 0);
  leds[45 + i3] = CRGB(0, 0, 0);
  leds[46 + i3] = CRGB(0, 0, 0);
  leds[47 + i3] = CRGB(0, 0, 0);
  leds[48 + i3] = CRGB(0, 0, 0);
  leds[49 + i3] = CRGB(0, 0, 0);
  leds[50 + i3] = CRGB(0, 0, 0);
  leds[51 + i3] = CRGB(0, 0, 0);
  leds[52 + i3] = CRGB(0, 0, 0);
  leds[53 + i3] = CRGB(0, 0, 0);
  leds[54 + i3] = CRGB(0, 0, 0);
  leds[55 + i3] = CRGB(0, 0, 0);
  leds[56 + i3] = CRGB(0, 0, 0);
  leds[57 + i3] = CRGB(0, 0, 0);
  leds[58 + i3] = CRGB(0, 0, 0);
  leds[59 + i3] = CRGB(0, 0, 0);
  leds[60 + i3] = CRGB(0, 0, 0);
  leds[61 + i3] = CRGB(0, 0, 0);
  leds[62 + i3] = CRGB(0, 0, 0);
  FastLED.show();
}


void one4() {
  leds[42 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[44 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[45 + i3] = CRGB(0, 0, 0);
  leds[46 + i3] = CRGB(0, 0, 0);
  leds[47 + i3] = CRGB(0, 0, 0);
  leds[48 + i3] = CRGB(0, 0, 0);
  leds[49 + i3] = CRGB(0, 0, 0);
  leds[50 + i3] = CRGB(0, 0, 0);
  leds[51 + i3] = CRGB(0, 0, 0);
  leds[52 + i3] = CRGB(0, 0, 0);
  leds[53 + i3] = CRGB(0, 0, 0);
  leds[54 + i3] = CRGB(0, 0, 0);
  leds[55 + i3] = CRGB(0, 0, 0);
  leds[56 + i3] = CRGB(0, 0, 0);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(0, 0, 0);
  leds[61 + i3] = CRGB(0, 0, 0);
  leds[62 + i3] = CRGB(0, 0, 0);
  FastLED.show();
}
void two4() {
  leds[42 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[44 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[45 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[46 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[47 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[48 + i3] = CRGB(0, 0, 0);
  leds[49 + i3] = CRGB(0, 0, 0);
  leds[50 + i3] = CRGB(0, 0, 0);
  leds[51 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[52 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[53 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[54 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[55 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[56 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[57 + i3] = CRGB(0, 0, 0);
  leds[58 + i3] = CRGB(0, 0, 0);
  leds[59 + i3] = CRGB(0, 0, 0);
  leds[60 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[61 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[62 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}

void three4() {
  leds[42 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[44 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[45 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[46 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[47 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[48 + i3] = CRGB(0, 0, 0);
  leds[49 + i3] = CRGB(0, 0, 0);
  leds[50 + i3] = CRGB(0, 0, 0);
  leds[51 + i3] = CRGB(0, 0, 0);
  leds[52 + i3] = CRGB(0, 0, 0);
  leds[53 + i3] = CRGB(0, 0, 0);
  leds[54 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[55 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[56 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[61 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[62 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}
void four4() {
  leds[42 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[44 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[45 + i3] = CRGB(0, 0, 0);
  leds[46 + i3] = CRGB(0, 0, 0);
  leds[47 + i3] = CRGB(0, 0, 0);
  leds[48 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[49 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[50 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[51 + i3] = CRGB(0, 0, 0);
  leds[52 + i3] = CRGB(0, 0, 0);
  leds[53 + i3] = CRGB(0, 0, 0);
  leds[54 + i3] = CRGB(0, 0, 0);
  leds[55 + i3] = CRGB(0, 0, 0);
  leds[56 + i3] = CRGB(0, 0, 0);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[61 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[62 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}
void five4() {
  leds[42 + i3] = CRGB(0, 0, 0);
  leds[43 + i3] = CRGB(0, 0, 0);
  leds[44 + i3] = CRGB(0, 0, 0);
  leds[45 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[46 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[47 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[48 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[49 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[50 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[51 + i3] = CRGB(0, 0, 0);
  leds[52 + i3] = CRGB(0, 0, 0);
  leds[53 + i3] = CRGB(0, 0, 0);
  leds[54 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[55 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[56 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[61 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[62 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}
void six4() {
  leds[42 + i3] = CRGB(0, 0, 0);
  leds[43 + i3] = CRGB(0, 0, 0);
  leds[44 + i3] = CRGB(0, 0, 0);
  leds[45 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[46 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[47 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[48 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[49 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[50 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[51 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[52 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[53 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[54 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[55 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[56 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[61 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[62 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}
void seven4() {
  leds[42 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[44 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[45 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[46 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[47 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[48 + i3] = CRGB(0, 0, 0);
  leds[49 + i3] = CRGB(0, 0, 0);
  leds[50 + i3] = CRGB(0, 0, 0);
  leds[51 + i3] = CRGB(0, 0, 0);
  leds[52 + i3] = CRGB(0, 0, 0);
  leds[53 + i3] = CRGB(0, 0, 0);
  leds[54 + i3] = CRGB(0, 0, 0);
  leds[55 + i3] = CRGB(0, 0, 0);
  leds[56 + i3] = CRGB(0, 0, 0);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(0, 0, 0);
  leds[61 + i3] = CRGB(0, 0, 0);
  leds[62 + i3] = CRGB(0, 0, 0);
  FastLED.show();
}
void eight4() {
  leds[42 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[44 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[45 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[46 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[47 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[48 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[49 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[50 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[51 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[52 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[53 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[54 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[55 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[56 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[61 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[62 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}
void nine4() {
  leds[42 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[44 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[45 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[46 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[47 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[48 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[49 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[50 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[51 + i3] = CRGB(0, 0, 0);
  leds[52 + i3] = CRGB(0, 0, 0);
  leds[53 + i3] = CRGB(0, 0, 0);

  leds[54 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[55 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[56 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[61 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[62 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  FastLED.show();
}

void zero4() {

  leds[42 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[43 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[44 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[45 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[46 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[47 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[48 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[49 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[50 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[51 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[52 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[53 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[54 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[55 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[56 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[57 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[58 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[59 + i3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[60 + i3] = CRGB(0, 0, 0);
  leds[61 + i3] = CRGB(0, 0, 0);
  leds[62 + i3] = CRGB(0, 0, 0);
  FastLED.show();
}
void Celsia() {
  leds[0] = CRGB(0, 0, 0);
  leds[1] = CRGB(0, 0, 0);
  leds[2] = CRGB(0, 0, 0);
  leds[3] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);

  leds[6] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);

  leds[9] = CRGB(0, 0, 0);
  leds[10] = CRGB(0, 0, 0);
  leds[11] = CRGB(0, 0, 0);

  leds[12] = CRGB(0, 0, 0);
  leds[13] = CRGB(0, 0, 0);
  leds[14] = CRGB(0, 0, 0);

  leds[15] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[16] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[17] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[18] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[19] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[20] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);

  leds[0 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[1 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[2 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[3 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[4 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[5 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[6 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[7 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[8 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[9 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[10 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[11 + i] = CRGB(color1 * jasf, color2 * jasf, color3 * jasf);
  leds[12 + i] = CRGB(0, 0, 0);
  leds[13 + i] = CRGB(0, 0, 0);
  leds[14 + i] = CRGB(0, 0, 0);
  leds[15 + i] = CRGB(0, 0, 0);
  leds[16 + i] = CRGB(0, 0, 0);
  leds[17 + i] = CRGB(0, 0, 0);
  leds[18 + i] = CRGB(0, 0, 0);
  leds[19 + i] = CRGB(0, 0, 0);
  leds[20 + i] = CRGB(0, 0, 0);

  FastLED.show();
}

void vypiscislo(int cislojed, int cislodes, int cislosto, int cislotis) {


  switch (cislojed) {
    case 1:
      one();
      break;
    case 2:
      two();
      break;
    case 3:
      three();
      break;
    case 4:
      four();
      break;
    case 5:
      five();
      break;
    case 6:
      six();
      break;
    case 7:
      seven();
      break;
    case 8:
      eight();
      break;
    case 9:
      nine();
      break;
    case 0:
      zero();
      break;
  }
  switch (cislodes) {
    case 1:
      one2();
      break;
    case 2:
      two2();
      break;
    case 3:
      three2();
      break;
    case 4:
      four2();
      break;
    case 5:
      five2();
      break;
    case 6:
      six2();
      break;
    case 7:
      seven2();
      break;
    case 8:
      eight2();
      break;
    case 9:
      nine2();
      break;
    case 0:
      zero2();
      break;
  }
  switch (cislosto) {
    case 1:
      one3();
      break;
    case 2:
      two3();
      break;
    case 3:
      three3();
      break;
    case 4:
      four3();
      break;
    case 5:
      five3();
      break;
    case 6:
      six3();
      break;
    case 7:
      seven3();
      break;
    case 8:
      eight3();
      break;
    case 9:
      nine3();
      break;
    case 0:
      zero3();
      break;
  }
  switch (cislotis) {
    case 1:
      one4();
      break;
    case 2:
      two4();
      break;
    case 3:
      three4();
      break;
    case 4:
      four4();
      break;
    case 5:
      five4();
      break;
    case 6:
      six4();
      break;
    case 7:
      seven4();
      break;
    case 8:
      eight4();
      break;
    case 9:
      nine4();
      break;
    case 0:
      zero4();
      break;
  }
}
void gettemp() {
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  int jed = temp % 10;
  int des = (temp - jed) / 10;

  int cislotis = des;
  int cislosto = jed;
  int cislojed = 11;
  int cislodes = 11;
  vypiscislo(cislojed, cislodes, cislosto, cislotis);
  teckaoff();
  Celsia();
}

void gettime() {
  DateTime now = rtc.now();

  int minjed = now.minute() % 10;
  int mindes = (now.minute() - minjed) / 10;
  int hodjed = now.hour() % 10;
  int hoddes = (now.hour() - hodjed) / 10;
  int cislotis = hoddes;
  int cislosto = hodjed;
  int cislodes = mindes;
  int cislojed = minjed;
  tecka();
  vypiscislo(cislojed, cislodes, cislosto, cislotis);
}
void rtcclock() {
  if (rtctime > 0) {
    rtc.adjust(DateTime(2024, 3, 14, hodiny2, minuty2, sekundy2 + 1));
    hodiny2 = 0;
    minuty2 = 0;
    sekundy2 = 0;
    rtctime = 0;
  } else if (akttime > 0) {
    rtc.adjust(DateTime(2024, 3, 14, akthodiny2, aktminuty2, aktsekundy2 + 1));
    akthodiny2 = 0;
    aktminuty2 = 0;
    aktsekundy2 = 0;
    akttime = 0;
  }
}
void svap() {
  unsigned long currentMillis = millis();
  unsigned long currentMillis2 = millis();
  if (countdown != 0) {
    if (currentMillis2 - previousMillis >= interval) {
      previousMillis = currentMillis2;

      if (countdown > 0) {
        countdown--;
        int seccountdown = countdown % 60;
        int mincountdown = (countdown - seccountdown) / 60;
        int secjedcout = seccountdown % 10;
        int secdescout = (seccountdown - secjedcout) / 10;
        int minjedcount = mincountdown % 10;
        int mindescout = (mincountdown - minjedcount) / 10;
        if (countdown == 1) {
          one();
          delay(1000);
          int pocetb = pocet_blik.toInt();
          for (A; A <= pocetb; A++) {
            off();
            off2();
            off2();
            off3();
            off4();
            teckaoff();
            delay(200);
            eight();
            eight2();
            eight3();
            eight4();
            tecka();
            delay(200);
          }
        }
        int cislotis = mindescout;
        int cislosto = minjedcount;
        int cislodes = secdescout;
        int cislojed = secjedcout;
        A = 0;
        tecka();
        vypiscislo(cislojed, cislodes, cislosto, cislotis);
      }
    }
  }

  else if (function1Active) {
    if (currentMillis - previousMillis >= interval_1) {
      previousMillis = currentMillis;
      function1Active = false;
    }
    if(interval_1 != 0){
     gettime(); 
    }
    
  }

  else {
    if (currentMillis - previousMillis >= interval_2) {
      previousMillis = currentMillis;
      function1Active = true;
    }
    if(interval_2 != 0){
      gettemp();
    }
  }
}
void loop() {
  server.handleClient();
  rtcclock();
  wcolor();
  svap();
}
