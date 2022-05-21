/*
 * Operasi dasar Wifi
 */

//conection
const char* ssid = "hahahaha"; //Ganti dengan SSID wifi anda
const char* password =  "hahahahay"; //Ganti dengan PW wifi anda

#define WIFI_ATTEMP  100
#define WIFI_TIMEOUT 60000

// watchdog timer
Periodic wt_wifi(WIFI_TIMEOUT);

void wifiPrintMacAddress() {
  byte mac[6];
  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  for(int i=4; i>=0; i--) {
    Serial.print(":");
    Serial.print(mac[i], HEX);
  }
  Serial.println();
}

void wifiPrintIP() {  
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}

/*
 * Print all available networks
 * return number of available networks
 */
int wifiPrintNetworks() {
  // scan for nearby networks:
  Serial.println("Scan Networks");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("No wifi available");
    return 0;
  }
  
  // print the list of networks seen:
  Serial.println("No SSID             dBm Encryption");
  char buff[40];
  for (int i = 0; i < numSsid; i++) {
    sprintf(buff, "%3d %-16s %3d %3d", i,
      WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));
    Serial.println(buff);
  }
}

int wifiSetup() {
  int attemp = 0;
  Serial.print("Wifi connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // Connect to WiFi.
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");

      attemp++;
      if (attemp > WIFI_ATTEMP) {
        Serial.println(" failed !!!");
        return E_CONNECT;
      }
  }
  Serial.println(" connected");
  wt_wifi.reset();
  return E_OK;
}

int wifiLoop() {
  if (WiFi.status() != WL_CONNECTED) {
    if (wt_wifi.isTime()) {
      return wifiSetup();
    }
    return E_CONNECT;
  }
  return E_OK;
}
