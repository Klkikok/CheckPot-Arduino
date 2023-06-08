# CheckPot-Arduino
This application is for the Seeed nRF52840 module. It advertises its service for the phone to discover and connect to it. 
After the device is connecrted to the service, it will subscribe to 5 different characteristics.

Those five characteristics are:
- Temperature Characteristic ("19B10001-E8F2-537E-4F6C-D104768A1202") [BLERead | BLENotify]
- Humidity Characteristic ("19B10001-E8F2-537E-4F6C-D104768A1203") [BLERead | BLENotify]
- Soil Moisture Characteristic ("19B10001-E8F2-537E-4F6C-D104768A1204") [BLERead | BLENotify]
- Light Characteristic ("19B10001-E8F2-537E-4F6C-D104768A1205") [BLERead | BLENotify]
- Danger Characteristic ("19B10001-E8F2-537E-4F6C-D104768A1300") [BLERead | BLENotify]

Each characteristic is used to send the respected data over BLE to all subscribed devices.
After a device is connected, the module will read the data from sensors and write them to the characteristics.
The danger characteristic is used when there is a faulty sensor, which will the be presented to the user as an error.
