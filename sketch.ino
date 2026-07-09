#include <WiFi.h>          // Biblioteca para conexão Wi-Fi do ESP32
#include <WiFiMulti.h>     // Biblioteca para gerenciar múltiplas redes Wi-Fi
#include <HTTPClient.h>    // Biblioteca para realizar requisições HTTP
#include <ESP32Servo.h>    // Biblioteca para controlar o Servo Motor

// --- DEFINIÇÃO DE PINOS ---
const int PIN_TRIG = 12;   // Pino de disparo do sensor ultrassônico
const int PIN_ECHO = 14;   // Pino de recepção do sensor ultrassônico
const int PIN_SERVO = 13;  // Pino de sinal do servo motor
const int LED_VM = 2;      // Pino do LED Vermelho (Bloqueado)
const int LED_VD = 5;      // Pino do LED Verde (Liberado)

// --- CONFIGURAÇÕES DE VARIÁVEIS ---
const float LIMIAR_DISTANCIA = 400.0; // Distância máxima de detecção (em cm)
unsigned long tempoInicioAcao = 0;    // Armazena o tempo em que a cancela abriu
unsigned long tempoUltimoFechamento = 0; // Armazena o tempo em que a cancela fechou
bool cancelaAberta = false;           // Variável de controle do estado da cancela

// Tempo de espera (3 segundos) para evitar detecções seguidas (Debounce)
const unsigned long PAUSA_ENTRE_CICLOS = 3000; 

Servo cancelaServo;       // Cria o objeto para controlar o servo
WiFiMulti wifiMulti;      // Cria o objeto para gerenciar as conexões Wi-Fi

void setup() {
  Serial.begin(115200);   // Inicia a comunicação serial para debug
  pinMode(PIN_TRIG, OUTPUT); // Define o pino TRIG como saída
  pinMode(PIN_ECHO, INPUT);  // Define o pino ECHO como entrada
  pinMode(LED_VM, OUTPUT);   // Define o LED Vermelho como saída
  pinMode(LED_VD, OUTPUT);   // Define o LED Verde como saída
  
  cancelaServo.attach(PIN_SERVO, 500, 2400); // Conecta o servo ao pino 13
  cancelaServo.write(0);  // Coloca o servo na posição inicial (fechado)
  
  digitalWrite(LED_VM, HIGH); // Liga o LED Vermelho no início
  digitalWrite(LED_VD, LOW);  // Garante o LED Verde desligado no início
  
  // Adiciona as redes Wi-Fi disponíveis
  wifiMulti.addAP("Wokwi-GUEST", "");
  wifiMulti.addAP("WiFi-Senac", "Senac2026");
}

void loop() {
  wifiMulti.run(); // Mantém a conexão Wi-Fi ativa
  unsigned long tempoAtual = millis(); // Captura o tempo atual do sistema
  
  // --- ROTINA DE LEITURA DO SENSOR ---
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); // Dispara o pulso do sensor
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  // Calcula a distância baseada no tempo de resposta do eco
  long duracao = pulseIn(PIN_ECHO, HIGH, 20000);
  float distancia = (duracao == 0) ? 999.0 : duracao * 0.034 / 2;

  // --- LÓGICA DE ABERTURA ---
  // Verifica se: cancela está fechada, objeto detectado E tempo de pausa decorrido
  if (!cancelaAberta && distancia > 0 && distancia <= LIMIAR_DISTANCIA && (tempoAtual - tempoUltimoFechamento >= PAUSA_ENTRE_CICLOS)) {
    cancelaAberta = true;        // Define que a cancela agora está aberta
    tempoInicioAcao = tempoAtual; // Grava o momento da abertura

    digitalWrite(LED_VM, LOW);   // Apaga o LED Vermelho
    digitalWrite(LED_VD, HIGH);  // Acende o LED Verde
    cancelaServo.write(90);      // Move o servo para abrir a cancela
    Serial.println("Acesso liberado! Aberta por 5 segundos.");
  }

  // --- LÓGICA DE FECHAMENTO ---
  // Verifica se a cancela está aberta E se passaram 5 segundos (5000ms)
  if (cancelaAberta && (tempoAtual - tempoInicioAcao >= 5000)) {
    cancelaAberta = false;       // Define que a cancela agora está fechada
    tempoUltimoFechamento = tempoAtual; // Grava o momento do fechamento
    
    digitalWrite(LED_VD, LOW);   // Apaga o LED Verde
    digitalWrite(LED_VM, HIGH);  // Acende o LED Vermelho
    cancelaServo.write(0);       // Move o servo para fechar a cancela
    Serial.println("Cancela fechada. Aguardando...");
  }
}