#include <ESP8266WiFi.h>
#include <SimpleDHT.h>
#include <Adafruit_BMP085.h>
//#include <OneWire.h>
//a linha faz a importação da biblioteca do esp8266 para que se possa fazer uso das funcões wifi deste modulo. as linha 2 e 3 fazem a importação das bibliotecas dos sensoresdht11 e bmp085 para que se possa fazer uso das funções pre-definidas pelos desenvolvedores destes modulos.
//int DS18S20_Pin = D4; //DS18S20 Signal pin on digital 2

//Temperature chip i/o
//OneWire ds(DS18S20_Pin);  // on digital pin 2
Adafruit_BMP085 bmp;//cria a variavel que ira ser usada pelo bmp085.

const char* ssid     = "D-Link";//ssid da rede wi-fi, e senha, onde será conectado a placa nodemcu a web.
const char* password = "maria12345678";

const char* host = "192.168.0.8";//aqui é o endereço ip do servidor web, no qual será enviado o resultados das coletas para serem armazenadas em um banco de dados.
/*
   sempre verificar qual o ip da maquina
  para que se possa enviar os dados
  para o endereco correto
*/

int luminosidade = 0, pinDHT11 = D3;
byte temperatura = 0, umidade = 0;
float bmpTemp, bmpPress, bmpAltt, bmpSeaP;

SimpleDHT11 dht11;//cria um objeto da classe que contem todas as funções para a manipulação do sensor dht11.

void setup() {
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Conectado com ");
  Serial.println(ssid);


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("EndereÃƒÂ§o IP : ");
  Serial.println(WiFi.localIP());

  if (!bmp.begin()) {//verifica se o sensor bmp085 está devidamente conectado a placa, se ele não for identificado irá mostrar no monitor serial a linha seguinte dizendo que não foi possivel encontrá-lo
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}

void loop() {


  bmpTemp = bmp.readTemperature(); // temperatura
  bmpPress = bmp.readPressure(); // pressao atmosferica
  bmpAltt = bmp.readAltitude(); // altitude
  bmpSeaP = bmp.readSealevelPressure(); // pressao do nivel do mar

  /*
    Serial.print("Temperature = ");Serial.print(bmp.readTemperature());Serial.println(" *C");
    Serial.print("Pressure = ");Serial.print(bmp.readPressure()); Serial.println(" Pa");
    // Calculate altitude assuming 'standard' barometric // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = "); Serial.print(bmp.readAltitude()); Serial.println(" meters");
    Serial.print("Pressure at sealevel (calculated) = ");Serial.print(bmp.readSealevelPressure());Serial.println(" Pa");
    Serial.print("Real altitude = ");Serial.print(bmp.readAltitude(101500)); Serial.println(" meters");
  */


  //Sensor de temperatura do solo
  //float tsolo = getTemp();

  //Sensor de luminosidade
  luminosidade = analogRead(A0);//faz a leitura do LDR e armazena na variavel, luminosidade.

  //snesor de temperatura e umidade relativa
  temperatura = 0, umidade = 0;
  if (dht11.read(pinDHT11, &temperatura, &umidade, NULL)) {//faz a leitura do dht11 e atribui o valor de cada informação as variaveis precedidas do '&'. e se não for possivel fazer a leitura ele caira na instrução seguinte, avisando que não foi possivel encontrar o sensor.
    Serial.print("falha na leitura do DHT11.\n");
    return;
  }

  Serial.print("conectado a ");//status da rede
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("conexao falhou");
    return;
  }
  // We now create a URI for the request
  // bmpTemp; bmpPress; bmpAltt; bmpSeaP;
  String url = "/nodemcu/salvar.php?luminosidade=";
  url += luminosidade;
  url += "&temperatura=";
  url += temperatura;
  url += "&umidade=";
  url += umidade;
  url += "&tsolo=";
  url += "-";
  url += "&bmpTemp=";
  url += bmpTemp;
  url += "&bmpPress=";
  url += bmpPress;
  url += "&bmpAltt=";
  url += bmpAltt;
  url += "&bmpSeaP=";
  url += bmpSeaP;
  // a variavel url recebera todas as coletas em formato de url para em seguida ser enviada via metodo get para uma pagina web que recebera esses dados.

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");//envia a variavel url pelo metodo get diretamente para a pagina.
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');

    if (line.indexOf("salvo_com_sucesso") != -1) {
      Serial.println("salvo com sucesso");
    } else if (line.indexOf("erro_ao_salvar") != -1) {
      Serial.println("erro ao salvar");
    }
  }

  Serial.println("fechando conexao");
  delay(100000);
}


//codigo do sensor de temperatura do solo

/*
float getTemp() {
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;

}*/
