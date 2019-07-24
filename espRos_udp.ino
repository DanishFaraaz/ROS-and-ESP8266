#include<ESP8266WiFi.h>
#include<WiFiUdp.h>

WiFiUDP Udp;

const char* ssid = "JioFi2_0B7688";
const char* password = "xu7x5n9ucp";
unsigned int localPort = 8888;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.print("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
  

}

void loop() {
  int packetSize = Udp.parsePacket();
  if(packetSize){
    Serial.print("Received packet of size");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i=0; i<4; i++){
      Serial.print(remote[i], DEC);
      if (i < 3) {Serial.print(".");}
    }

     Serial.print(", port ");
     Serial.println(Udp.remotePort());

     //read packet into packetBuffer
     Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
     Serial.println("Contents:");
     String directions ="";
     for (int i=0;i<=UDP_TX_PACKET_MAX_SIZE;i++){
      if(packetBuffer[i] == '*'){
        move_bot(directions);
        break;
      }

      directions += packetBuffer[i];
     }
     
  }

  delay(10);
}

void move_bot(String a){
  int commaIndex = a.indexOf(',');
  int secondCommaIndex = a.indexOf(',', commaIndex+1);
  String firstValue = a.substring(0, commaIndex);
  String secondValue = a.substring(commaIndex+1, secondCommaIndex);
  int linear = firstValue.toInt();
  int angular  = secondValue.toInt();
  Serial.print("LINEAR: ");
  Serial.println(linear);
  Serial.print("Angular: ");
  Serial.println(angular);
  Serial.println("-------------------------------");

  if(linear>0 && angular==0){
    //FORWARD
    Serial.println("FORWARD");
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);

    delay(2000);
    stop();
  }

  else if(linear<0 && angular==0){
    //REVERSE
    Serial.println("REVERSE");
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);

    delay(2000);
    stop();
  }

  else if(linear==0 && angular>0){
    //LEFT
    Serial.println("LEFT");
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);

    delay(2000);
    stop();
  }

  else if(linear==0 && angular<0){
    //RIGHT
    Serial.println("RIGHT");
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);

    delay(2000);
    stop();
  }

  else{
    stop();
  }
}

void stop(){
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
}
