

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const int flowPin1 = D7;
const int flowPin2 = D6;
unsigned long flow1count = 0;
unsigned long flowacount = 0;
unsigned long flow2count = 0;
unsigned long flowbcount = 0;
float dt = 0;
float dt2 =0;
float t1 = 0;
float t2 = 0;
float t3 = 0;
float t4 = 0;
int cnt = 1;
float dta =0;
float dt2a =0;
float w =0;
float w2=0;
float fa=0;
float fb=0;
#define countof(a) (sizeof(a) / sizeof(a[0]))

#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 12

char data[80];
StaticJsonBuffer<200> jsonBuffer;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress inputa, outputa, inputb, outputb;

const char* ssid = "IOT";
const char* password = "";
const char* mqtt_server = "192.168.168.150";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
 sensors.begin();
  

pinMode(flowPin1, INPUT);
attachInterrupt(flowPin1, flow1, CHANGE);
pinMode(flowPin2, INPUT);
attachInterrupt(flowPin2, flow2, CHANGE);

  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);

  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  if (!sensors.getAddress(inputa, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(outputa, 1)) Serial.println("Unable to find address for Device 1"); 
   if (!sensors.getAddress(inputb, 2)) Serial.println("Unable to find address for Device 2"); 
  if (!sensors.getAddress(outputb, 3)) Serial.println("Unable to find address for Device 3"); 
  Serial.print("Device 0 Address: ");
  printAddress(inputa);
  Serial.println();
  Serial.print("Device 1 Address: ");
  printAddress(outputa);
  Serial.println();
   Serial.print("Device 2 Address: ");
  printAddress(inputb);
  Serial.println();
  Serial.print("Device 3 Address: ");
  printAddress(outputb);
  Serial.println();
  sensors.setResolution(inputa, TEMPERATURE_PRECISION);
  sensors.setResolution(outputa, TEMPERATURE_PRECISION);
   sensors.setResolution(inputb, TEMPERATURE_PRECISION);
  sensors.setResolution(outputb, TEMPERATURE_PRECISION);
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(inputa), DEC); 
  Serial.println();
  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(outputa), DEC); 
  Serial.println();
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(inputb), DEC); 
  Serial.println();
  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(outputb), DEC); 
  Serial.println();

  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


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

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}






void loop() {
cnt= ++cnt;
Serial.print("Flow 1 in Liters: ");
Serial.print(flow1count/450.0);
Serial.print("\tFlow 2 in Liters: ");
Serial.print(flow2count/450.0);
Serial.print("\tFlow 1 in pulses: ");
//Serial.print(flowacount);
Serial.print(flow1count);
Serial.print("\tFlow 2 in pulse: ");
//Serial.print(flowbcount);
Serial.println(flow2count);
delay(500);
 Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
    printData(inputa);
  printData(outputa);
  
  t1 = (sensors.getTempC(inputa)+0.0);  //add error correction
  t2 = (sensors.getTempC(outputa)+0.15);
dt = (t1 - t2); 
dta= (dta+dt); 
Serial.println(dt);
Serial.println(dta);
Serial.println(cnt);
 printData(inputb);
  printData(outputb);
  t3 = (sensors.getTempC(inputb)+0.00);
  t4 = (sensors.getTempC(outputb)+0.0);
dt2 = (t3 - t4);
dt2a=(dt2a+dt2);
Serial.println(dt2);
Serial.println(dt2a);  

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {   // 5 second sample
    lastMsg = now;
   // ++value;
    
    
    
    fa =(flowacount/450.0);
   fb=(flowbcount/450.0);
    dta=(dta/cnt);
    dt2a=(dt2a/cnt);
    Serial.println(dta);
    Serial.println(dt2a);
    w=(((((flowacount/450.0)*1000)*dta)*4.188)/5);     // adjust
   // strw=t1;
    w2=(((((flowbcount/450.0)*1000)*dt2a)*4.188)/5);
     Serial.println(w);
      Serial.println(w2);
  String value = "\"Input\": " + String(t1) ;
String value1 = ",\"Output\": " + String(t2) ;
String value2 = ",\"Flow\": " + String(fa) ;
String value3 = ",\"Watt\": " + String(w) ;
   value = value + value1 + value2 + value3;
  String payload = "{ \"devices\": \"A\",\"payload\": {" + value + "}}";
   payload.toCharArray(data, (payload.length() + 1));
      
client.publish("FlowAb",data);
  Serial.print("Publish message: ");
    Serial.println(data);    
    delay(50);
    String value8 = String(w) ;
    String payload3 =  value8 ;
   payload3.toCharArray(data, (payload3.length() + 1));
  client.publish("FlowAw",data);
  Serial.print("Publish message: ");
    Serial.println(w);    
    delay(50);  
        String value9 = String(w2) ;
    String payload4 =  value9 ;
   payload4.toCharArray(data, (payload4.length() + 1));
  client.publish("FlowAw2",data);
    Serial.print("Publish message: ");
    Serial.println(w2);    
    delay(50);  
      String value4 = "\"Input\": " + String(t3) ;
String value5 = ",\"Output\": " + String(t4) ;
String value6 = ",\"Flow\": " + String(fb) ;
String value7 = ",\"Watt\": " + String(w2) ;
   value = value4 + value5 + value6 + value7;
  String payload2 = "{ \"devices\": \"B\",\"payload\": {" + value + "}}";
   payload2.toCharArray(data, (payload2.length() + 1));
      
client.publish("FlowBb",data);
  Serial.print("Publish message: ");
    Serial.println(data);    
    flowacount =0;
    flowbcount =0;
    cnt=0;
    dta=0;
    dt2a=0;
  }
}

void flow1()
{
flow1count +=1;
flowacount +=1;
}

void flow2()
{
flow2count +=1;
flowbcount +=1;
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();    
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

