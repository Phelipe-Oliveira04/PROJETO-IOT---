# Projeto: Cancela de Acesso Inteligente com ESP32

Sistema de controle de acesso automatizado desenvolvido como solução de IoT, utilizando lógica de máquina de estados para operação estável e não bloqueante.

## 🚀 Funcionalidades
- **Detecção Inteligente:** Sensor ultrassônico com zona de detecção configurada (50cm a 200cm).
- **Operação Assíncrona:** Código sem `delay()`, permitindo monitoramento contínuo via rede Wi-Fi.
- **Segurança Operacional:** Sistema de "debounce" (pausa de 3s) para evitar desgaste do servo motor.
- **Feedback Visual:** Controle de LEDs para indicação de status (Acesso Liberado/Bloqueado).

## 🛠 Tecnologias Utilizadas
- **Hardware:** ESP32, Sensor HC-SR04, Servo Motor MG996R, LEDs.
- **Ambiente de Desenvolvimento:** Wokwi Simulator / Arduino IDE.
- **Linguagem:** C++ (Framework Arduino).

## 📊 Fluxo de Operação
O sistema opera em um loop contínuo onde:
1. Verifica conexão Wi-Fi.
2. Monitora distância.
3. Valida condições de segurança.
4. Aciona o atuador (servo) com temporização de 5 segundos para fechamento automático.

## 📁 Estrutura do Código
- O firmware utiliza `millis()` para gerenciar o tempo, garantindo que o sistema seja multitarefa e altamente resiliente.

---
*Projeto desenvolvido para fins educacionais - Acesso Inteligente Senac 2026.*
