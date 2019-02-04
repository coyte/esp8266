/*
 ESP8266 MQTT bridge for RG-11

 It connects to an MQTT server and reports any rain as triggered by the RG-11 
 rain sensor. It sends the raw "bucket" counters allowing selection of sensitivity
 on the server side.

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. This also applies for wifi.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "yourssid";
const char* password = "yourpassword";

const char* mqtt_server = "your.mqtt.broker.com";
const char* rain_topic = "house/outside/rain";

WiFiClient espClient;
PubSubClient client(espClient);

// The follow is needed for interrupt handling
const byte rainsensorPin = 13;
volatile byte bucketCounter = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Turn off LED since we're offline
  digitalWrite(BUILTIN_LED, HIGH);
  while (!client.connected()) {
    // Make sure wifi is still ok, otherwise it's dumb to connect to mqtt
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Network isn't connected.");
      setup_wifi();
    }
    
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  // Turn on LED since we're back online
  digitalWrite(BUILTIN_LED, LOW);
}

void handleInterrupt() {
  bucketCounter++;
}

void setup() {
  // Initialize interrupt
  pinMode(rainsensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rainsensorPin), handleInterrupt, FALLING);
  
  // Setup wifi and mqtt (also make sure LED is off)
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.begin(115200);
  //setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  // Always make sure we're connected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Service all that rain that happened
  if(bucketCounter > 0) {
    byte buckets = bucketCounter;
    bucketCounter -= buckets;
    Serial.print("We got ");
    Serial.print((int)buckets);
    Serial.println(" buckets of rain, let server know.");

    // Share this with the MQTT server
    client.publish(rain_topic, String((int)buckets).c_str());
  }
}
