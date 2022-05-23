/*
 * Fungsi dasar MQTT
 * Konvensi nama topik
 * TOPIC_BASE / NODE(Regu/NIM) / IN / TAG; Publish-> labtf2/k4/13319037/in/tag
 * TOPIC_BASE / NODE(Regu/NIM)/ OUT / TAG; Subscribe-> labtf2/k4/13319037/out/tag
 */

//Variabel untuk Publish
enum I_TAGS {I, V, P, RPM, V1, V2}; //tanpa angka = data dari INA219
char* input_tags[] = {"I", "V", "P", "RPM", "V1", "V2"};
float inputs[I, V, P, RPM, V1, V2];

//Variabel untuk Subscribe
enum O_TAGS {L0, L1, O_MAX};
char* output_tags[] = {"L0", "L1"};
int outputs[L0, L1] = {0, 0};
 
//MQTT
const char* mqttServer = "iot.tf.itb.ac.id";
const int mqttPort = 1883;
const char* clientID = NODE;

#define MQTT_TIMEOUT 10000

//Define Topic
#define TOPIC_BASE "LABTF2"
#define TOPIC_NODE TOPIC_BASE "/" NODE

const char* chSubscribe = TOPIC_NODE "/OUT";
const char* chPublish = TOPIC_NODE "/IN";

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

// watchdog timer
Periodic wt_mqtt(WIFI_TIMEOUT);

#define MQTT_STR_MAX  256

char sPayload[MQTT_STR_MAX];
char sTopic[32];

void mqttPublish(char tag[], int value) {
  sprintf(sPayload, "%d", value);
  sprintf(sTopic, "%s/%s",chPublish, tag);   
  client.publish(sTopic, sPayload);
}

// Akan jalan kalau MQTT terima data
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  int i;
  payload[length] = '\0';
  Serial.print(topic);
  Serial.print(":");
  Serial.println((char*)payload);

  // Subscribe
  for(int i=0; i<O_MAX; i++) {
    sprintf(sTopic, "%s/%s",chSubscribe, output_tags[i]);
    if (String(topic) == sTopic){
      int out = atoi((char *)payload); //change char* to int
      outputs[i]= out;
      if (i==0) digitalWrite(ledPin, outputs[i]);
      else ledcWrite(ledChannel, outputs[i]);
    }
  }  
}

int mqttSetup() {
  // PENTING
  client.setServer(mqttServer, mqttPort); // Connect to MQTT
  client.setCallback(mqttCallback);
  
  Serial.print("MQTT connecting to ");
  Serial.print(mqttServer);
  Serial.print(":");
  Serial.println(mqttPort); 
         
  if (!client.connect(clientID)) {
    Serial.println("failed !!!");
    return E_CONNECT;
  }
  // setelah berhasil
  sprintf(sTopic,"%s/#",chSubscribe);
  client.subscribe(sTopic);
  Serial.print("Subscribe to ");
  Serial.println(sTopic);
  return E_OK;
}

int mqttLoop() {
  if (!client.connected()) {
    if (wt_mqtt.isTime()) {
      return mqttSetup();
    }
    return E_CONNECT;
  }
  
  // HARUS selalu dipanggil
  client.loop();
  
  // publish semua outputs
  for(int i=0; i<I_MAX; i++) {
    mqttPublish(input_tags[i], inputs[i]);
  }  
  
  return E_OK;
}
