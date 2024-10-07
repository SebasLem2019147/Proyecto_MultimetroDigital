#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define OLED_RESET 4

#define RX 2
#define TX 3

// Polo 1
#define Inicio 4
#define menuMultimetro 5
#define medirVoltaje 6
#define medirCorriente 7
#define medirRes 8

// Polo 2
#define medirRes2k 9
#define medirRes20k 10
#define medirRes200k 11
#define medirRes2M 12
#define volverP1 13

Adafruit_SH1106 display(OLED_RESET);

SoftwareSerial moduloBluetooth(RX, TX);

// Símbolo de rayo
const unsigned char rayo [] PROGMEM = {
  0x08, 0x1C, 0x3E, 0x7F, 0x1C, 0x08, 0x00
};

//Función para dibujar el simbolo de Ohms
const unsigned char ohmSymbol [] PROGMEM = {
  0x3C, 0x7E, 0xC3, 0xC3, 0xC3, 0xC3, 0x66, 0x66
};

// Función para dibujar líneas cortadas
void dibujarLineasCortadas(int startX, int startY, int length, int segments) {
  int segmentLength = length / segments; // Longitud de cada segmento
  
  for (int i = 0; i < segments; i++) {
    if (i % 2 == 0) {
      // Dibuja solo segmentos pares (para crear el efecto de líneas cortadas)
      display.drawLine(startX + i * segmentLength, startY, startX + (i + 1) * segmentLength, startY, WHITE);
    }
  }
}

const int voltajeMax = 25000;
int lecturaDigital;
float voltaje;

float sensibilidad=0.185;
float I=0.00;
float ajuste = 0.05;

void setup() {
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.display();

  //Polo 1
  pinMode(medirVoltaje, INPUT);
  pinMode(medirCorriente, INPUT);
  pinMode(menuMultimetro, INPUT);
  pinMode(medirRes, INPUT);
  pinMode(Inicio, INPUT);

  //Polo 2
  pinMode(medirRes2k, INPUT);
  pinMode(medirRes20k, INPUT);
  pinMode(medirRes200k, INPUT);
  pinMode(medirRes2M, INPUT);
  pinMode(volverP1, INPUT);

  delay(1000);
}

void medicion2k_ohms() {
  const int SensorVolt = A2;
  int ValorSensor = 0;

  float Vin = 5;
  float Vout = 0;
  float Rref = 2000;
  float resistencia = 0;

  ValorSensor = analogRead(SensorVolt);
  Vout = (Vin * ValorSensor) / 1023;
  
  resistencia = Rref * (1 / ((Vin / Vout) - 1));

  imprimirResistencia(resistencia);
}

void medicion20k_ohms() {
  const int SensorVolt = A3;
  int ValorSensor = 0;

  float Vin = 5;
  float Vout = 0;
  float Rref = 20000;
  float resistencia = 0;

  ValorSensor = analogRead(SensorVolt);
  Vout = (Vin * ValorSensor) / 1023;
  
  resistencia = Rref * (1 / ((Vin / Vout) - 1));

  imprimirResistencia(resistencia);
}

void medicion200k_ohms() {
  const int SensorVolt = A6;
  int ValorSensor = 0;

  float Vin = 5;
  float Vout = 0;
  float Rref = 200000;
  float resistencia = 0;

  ValorSensor = analogRead(SensorVolt);
  Vout = (Vin * ValorSensor) / 1023;
  
  resistencia = Rref * (1 / ((Vin / Vout) - 1));

  imprimirResistencia(resistencia);
}

void medicion2M_ohms() {
  const int SensorVolt = A7;
  int ValorSensor = 0;

  float Vin = 5;
  float Vout = 0;
  float Rref = 2000000;
  float resistencia = 0;

  ValorSensor = analogRead(SensorVolt);
  Vout = (Vin * ValorSensor) / 1023;
  
  resistencia = Rref * (1 / ((Vin / Vout) - 1));

  imprimirResistencia(resistencia);
}

//imprimir valores de resistencias en pantalla OLED
void imprimirResistencia(float resistencia) {
  if (resistencia >= 1e6) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("Valor de resistencia:");
    display.drawRect(0, 15, 128, 45, WHITE);
    dibujarLineasCortadas(22, 50, 90, 10);
    // Resistencia en Mega ohms (MΩ)
    display.setTextSize(2);
    display.setCursor(20,30);
    display.println(resistencia / 1e6);
    display.setCursor(81,30);
    display.println("M");
    display.drawBitmap(94, 30, ohmSymbol, 8, 8, WHITE);
  } else if (resistencia >= 1e3) {
    display.clearDisplay();
    // Resistencia en Kilo ohms (kΩ)
    display.setTextSize(2);
    display.setCursor(20,30);
    display.print(resistencia / 1e3);
    display.setCursor(81,30);
    display.println("k");
    display.drawBitmap(94, 30, ohmSymbol, 8, 8, WHITE);
  } else {
    display.clearDisplay();
    // Resistencia en ohms (Ω)
    display.setTextSize(2);
    display.setCursor(20,30);
    display.print(resistencia);
    display.drawBitmap(81, 30, ohmSymbol, 8, 8, WHITE);
  }
  display.display();
}

void VoltajeFZ0430(){
  lecturaDigital = analogRead(A0);
  voltaje = map(lecturaDigital, 0, 1023, 0, voltajeMax) / 1000.0;
  imprimirVoltaje(voltaje);
  delay(1000);
}

void imprimirVoltaje(float v){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Valor de Voltaje:");
  display.drawRect(0, 15, 128, 45, WHITE);
  display.setTextSize(2); 
  display.setCursor(30, 30); 
  display.println(v); //imprime el valor del voltaje  
  display.setCursor(92, 30);  
  display.println("V");
  dibujarLineasCortadas(29, 50, 80, 10); 
  display.display();
}

float promedio_I(int muestras_I)
{
  float sensorA1;
  float intencidad=0;
  for(int i=0;i<muestras_I;i++)
  {
    sensorA1 = analogRead(A1) * (5.0 / 1023.0);
    intencidad=intencidad+(sensorA1-2.5)/sensibilidad; //Calculo para obtener el valor de la corriente
  }
  intencidad=intencidad/muestras_I;
  return(intencidad);
}

void CorrienteACS712(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Valor de Corriente:");
  display.drawRect(0, 15, 128, 45, WHITE);
  dibujarLineasCortadas(29, 50, 80, 10); 
  I=promedio_I(500);//Promedio de 500 muestras para mejorar la presición
  if(I>=0.01){//Filtro para eliminar pequeñas oscilasciones(ruido)
  I= ((I)+ajuste),2;
  display.setTextSize(2); 
  display.setCursor(29, 30); 
  display.println(I, 2); //imprime el valor de la corriente   
  display.setCursor(92, 30);  
  display.println("A");
  delay(100); 
  }else{
  display.setTextSize(2); 
  display.setCursor(29, 30);
  display.println("0.00");
  display.setCursor(92, 30);  
  display.println("A");
  I = 0.00; 
  delay(100);   
  }
  display.display();
  delay(1000);
}

void accionPolo2() {
  bool regresar = false;
  display.clearDisplay();
  display.display();
  while (!regresar) {
    if (digitalRead(medirRes2k) == HIGH) {
      medicion2k_ohms();
    } 
    else if (digitalRead(medirRes20k) == HIGH) {
      medicion20k_ohms();
    } 
    else if (digitalRead(medirRes200k) == HIGH) {
      medicion200k_ohms();
    } 
    else if (digitalRead(medirRes2M) == HIGH) {
      medicion2M_ohms();
    } 
    else if (digitalRead(volverP1) == HIGH) {
      regresar = true;  // Romper el bucle cuando se presiona el botón de regresar
    }
    delay(200); 
  }
}

void accionPolo1(){
  display.clearDisplay();
  display.setTextSize(1);
  if(digitalRead(medirVoltaje) == HIGH){
    VoltajeFZ0430();
  }
  else if(digitalRead(medirCorriente) == HIGH){
    CorrienteACS712();
  }
  else if(digitalRead(menuMultimetro) == HIGH){
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println(" -Multimetro Digital-");
    display.setCursor(0,15);
    display.println("1. Voltaje 0-20V");
    display.setCursor(0,25);
    display.println("2. Corriente 0-5A");
    display.setCursor(0,35);
    display.println("3. Resistencia 0-2M");
    display.drawBitmap(115, 35, ohmSymbol, 8, 8, WHITE);
    display.display();
  }
  else if(digitalRead(Inicio) == HIGH){
    display.setTextSize(2);
    display.clearDisplay(); 
    display.setCursor(5, 15); 
    display.println("MULTIMETRO");
    display.setCursor(15, 35); 
    display.println("DIGITAL");
    display.drawBitmap(100, 35, rayo, 8, 8, WHITE);
    display.display();  
    delay(1000);
    display.display();
  }
  else if(digitalRead(medirRes) == HIGH){
    display.clearDisplay(); 
    dibujarLineasCortadas(0, 10, 128, 16);
    dibujarLineasCortadas(0, 60, 128, 16);
    display.setCursor(5, 24);  
    display.println("Elegir el modo de");
    display.setCursor(0, 34);  
    display.println("resistencia en polo 2"); 
    display.display();   
    delay(5000);
    
    accionPolo2();
  }
}

void loop(){
  accionPolo1();
}
