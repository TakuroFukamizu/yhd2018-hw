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
#include "BLEDevice.h"

char *ssid = " aterm-a55b9a-g";
char *password = "15208da0108bd";

// MQTTの接続先のIP
const char *endpoint = "192.168.179.7";
// MQTTのポート
const int port = 1883;

// デバイスID
char *deviceID = "M5Stack";
// トピック
char *akabekoTopic = "/sub/target/red";

char blowerPin = 2;

////////////////////////////////////////////////////////////////////////////////

WiFiClient httpsClient;
PubSubClient mqttClient(httpsClient);

void setup() {
    Serial.begin(115200);
    pinMode(blowerPin, OUTPUT);

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

//    BLEDevice::init("");
}

void connectMQTT() {
    while (!mqttClient.connected()) {
        if (mqttClient.connect(deviceID)) {
            Serial.println("Connected.");
            int qos = 0;
            mqttClient.subscribe(akabekoTopic, qos);
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
    if (strcmp(topic, "/sub/bigakabeko") == 0) {
        bool enable = root["value"];
        int time = root["time"];
        blower(enable, time);
    }
}

/**
 * ブロワーを制御
 */
void blower(bool enable, int time) {
    if (enable) {
        M5.Lcd.println("blower drive");
        digitalWrite(blowerPin, HIGH);
        // time == 0 の場合は自動OFFしない
        if (time != 0) {
            delay(time);
            digitalWrite(blowerPin, LOW);
        }
    } else {
        digitalWrite(blowerPin, LOW);
    }
}

void mqttLoop() {
    if (!mqttClient.connected()) {
        connectMQTT();
    }
    mqttClient.loop();
}

/**
 * のせラジ接続
 */
static BLEUUID serviceUUID("f7fce500-7a0b-4b89-a675-a79137223e2c");
static BLEUUID serviceUUID2("f7fce510-7a0b-4b89-a675-a79137223e2c");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("f7fce515-7a0b-4b89-a675-a79137223e2c");

static BLEAddress *pServerAddress = 0;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;

uint8_t output = 0;
bool coonnecting = false;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
}

bool connectToServer(BLEAddress pAddress) {
    Serial.print("Forming a connection to ");
    Serial.println(pAddress.toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    // Connect to the remove BLE Server.
    pClient->connect(pAddress);
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID2);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID2.toString().c_str());
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Write the value of the characteristic.
    uint8_t writeData[1] = {0};
    pRemoteCharacteristic->writeValue(writeData,1,false);
 
//  pRemoteCharacteristic->registerForNotify(notifyCallback);
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {

      // 
      Serial.print("Found our device!  address: "); 
      advertisedDevice.getScan()->stop();

      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      Serial.print("Set doConnect true."); 
      doConnect = true;
 
    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void NoseRadiLoop() {
    if(M5.BtnC.wasPressed() && coonnecting == false && connected == false) {
    M5.Lcd.println("Start Connectiong.");
    Serial.println("Start connecting.");
    coonnecting = true;
    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 30 seconds.
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
    M5.Lcd.println("End Connectiong.");
    Serial.println("End connecting.");
    // 接続中フラグをクリア
    coonnecting = false;
  }

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      // LCD display
      M5.Lcd.println("NoseRaji connected.");
      M5.Lcd.println("Please A button push to change LED1.");
      uint8_t writeData[1] = {0};
      pRemoteCharacteristic->writeValue(writeData, 1);
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    coonnecting = false;
    doConnect = false;
   }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
//    String newValue = "Time since boot: " + String(millis()/1000);
//    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
//    
//    // Set the characteristic's value to be the array of bytes that is actually a string.
//    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
    if(M5.BtnA.wasPressed()) {
      if(output == 0) {
        output = 1;
      } else {
        output = 0;
      }
      Serial.print("change output = ");
      Serial.println(output);
      uint8_t writeData[1] = {output};
      pRemoteCharacteristic->writeValue(writeData,1,false);
      delay(200); // Delay a second between loops.
    }
  }

  M5.update();
} 

void loop() {
  // 接続確認
  mqttLoop();

  // のせラジ接続確認  
  NoseRadiLoop();  

}