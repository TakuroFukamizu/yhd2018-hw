/**
 * target
 *     * ESP32
 * dependencies
 *     * PubSubClient, ArduinoJson
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <M5Stack.h>
#include <ArduinoJson.h>

char *ssid = "aterm-a55b9a-g";
char *password = "15208da0108bd";

// MQTTの接続先のIP
const char *endpoint = "192.168.179.7";
// MQTTのポート
const int port = 1883;

// デバイスID
char *deviceID = "M5Stack2";
// トピック
char *akabekoTopic1 = "/sub/target/red";
char *akabekoTopic2 = "/sub/target/yellow";

char blowerPin = 2;

////////////////////////////////////////////////////////////////////////////////

WiFiClient httpsClient;
PubSubClient mqttClient(httpsClient);

int servoPin[] = {2, 5};

void setup() {
    Serial.begin(115200);
    pinMode(blowerPin, OUTPUT);
    pinMode(servoPin[0], OUTPUT);
    pinMode(servoPin[1], OUTPUT);

    // Initialize the M5Stack object
    M5.begin();

    // START
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(3);
    M5.Lcd.printf("START");

    // Start WiFi
    Serial.println("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // WiFi Connected
    Serial.println("\nWiFi Connected.");
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("WiFi Connected.\n");

    mqttClient.setServer(endpoint, port);
    mqttClient.setCallback(mqttCallback);

    connectMQTT();
}

void connectMQTT() {
    while (!mqttClient.connected()) {
        if (mqttClient.connect(deviceID)) {
            Serial.println("Connected.");
            int qos = 0;
            mqttClient.subscribe(akabekoTopic1, qos);
            mqttClient.subscribe(akabekoTopic2, qos);
            Serial.println("Subscribed.");
        } else {
            Serial.print("Failed. Error state=");
            Serial.print(mqttClient.state());
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

/**
 * MQTTから信号を受信
 */
void mqttCallback (char* topic, byte* payload, unsigned int length) {
    Serial.println("Recieve Topic");
    // メッセージを読み込み
    String str = "";
    for (int i = 0; i < length; i++) {
        str += (char)payload[i];
    }
    // JSONにパース
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(str);
    // パースが成功したか確認
    if (!root.success()) {
        Serial.println("ParseError");
        return;
    }

    Serial.printf("Topic: %s\n", topic);
    if (strcmp(topic, "/sub/target/red") == 0) {
        bool enable = root["value"];
        int time = root["time"];
        clappy(0, enable, time);
    } else if (strcmp(topic, "/sub/target/yellow") == 0) {
        bool enable = root["value"];
        int time = root["time"];
        clappy(1, enable, time);
    }
}

void mqttLoop() {
    if (!mqttClient.connected()) {
        connectMQTT();
    }
    mqttClient.loop();
}

/**
 * クラッピーを制御
 */
void servoSet(int Pin, int Angle,int Time) {
    int pulseWidth;
    int TimeCount;
  
    TimeCount = Time/20;                                                
  
    for(int i=0; i <= TimeCount; i++){
        pulseWidth = map(Angle, 0, 180, 544, 2400);
        digitalWrite(servoPin[Pin], HIGH);
        delayMicroseconds(pulseWidth);                          
        digitalWrite(servoPin[Pin], LOW);
        delayMicroseconds(20000 - pulseWidth);
    }   
}

void clappy(int id, bool enable, int time) {
    if (enable) {
        Serial.printf("clappy %d drive\n", id);
        servoSet(id,130,1000); //130deg
        // time == 0 の場合は自動OFFしない
        if (time != 0) {
            delay(time);
            servoSet(id,45,1000); //45deg
        }
    } else {
        servoSet(id,45,1000); //45deg
    }
}

void loop() {
    // 接続確認
    mqttLoop();
}
