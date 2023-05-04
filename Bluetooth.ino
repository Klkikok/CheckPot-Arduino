#include <dht11.h>
#include <ArduinoBLE.h>

#define DHT11PIN 0
dht11 DHT11;

int ligntSensorPin = 4; // select the input pin for LDR
unsigned char lightSensorValue = 0;

int soilSensor = 2;
unsigned char soilValue = 0;


BLEService checkPotService("19B10000-E8F2-537E-4F6C-D104768A1200"); // Bluetooth® Low Energy LED Service

BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1201", BLERead | BLEWrite);
BLEByteCharacteristic temperatureCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1202", BLERead | BLENotify);
BLEByteCharacteristic humidityCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1203", BLERead | BLENotify);
BLEByteCharacteristic soilMoistureCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1204", BLERead | BLENotify);
BLEByteCharacteristic lightCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1205", BLERead | BLENotify);

const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);
  pinMode(soilSensor,INPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("CheckPot");
  BLE.setAdvertisedService(checkPotService);

  // add the characteristic to the service
  checkPotService.addCharacteristic(switchCharacteristic);
  checkPotService.addCharacteristic(temperatureCharacteristic);
  checkPotService.addCharacteristic(humidityCharacteristic);
  checkPotService.addCharacteristic(soilMoistureCharacteristic);
  checkPotService.addCharacteristic(lightCharacteristic);
  

  // add service
  BLE.addService(checkPotService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();
  digitalWrite(ledPin, HIGH);
  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();


 
  
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
  while (central.connected()) {
        delay(1000);
        int chk = DHT11.read(DHT11PIN);
        Serial.print("Humidity (%): ");
        Serial.println((float)DHT11.humidity, 2);

        Serial.print("Temperature  (C): ");
        Serial.println((float)DHT11.temperature, 2);

        lightSensorValue = analogRead(ligntSensorPin);
        Serial.print("Light:");
        Serial.println(lightSensorValue); 

        soilValue = analogRead(soilSensor);
        Serial.print("Soil moisture: ");
        Serial.println(soilValue);  


        Serial.println();
        Serial.println();


        humidityCharacteristic.writeValue(DHT11.humidity);
        temperatureCharacteristic.writeValue(DHT11.temperature);
        lightCharacteristic.writeValue(lightSensorValue);
        soilMoistureCharacteristic.writeValue(soilValue)

        if (switchCharacteristic.written()) {
          if (switchCharacteristic.value()) { 
            Serial.println("LED on");
            digitalWrite(ledPin, LOW); // changed from HIGH to LOW       
          } else {                              
            Serial.println(F("LED off"));
            digitalWrite(ledPin, HIGH); // changed from LOW to HIGH     
          }
        }
      }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
