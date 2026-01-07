# Sistema embarcado: Horímetro

## Objetivo
  # Sistema embarcado: Horímetro

## Objetivo
  Desenvolver um horímetro baseado em ESP32 para cronometrar o tempo de uso de máquinas e indicar o momento correto para troca de óleo ou manutenção preventiva. O sistema permite pausar, retomar e reiniciar a contagem, além de enviar informações para um site via protocolo MQTT usando o broker HiveMQ.

## Funcionamento
  - O ESP32 realiza a contagem contínua do tempo de funcionamento da máquina.
  - Ao atingir o tempo limite configurado, o sistema aciona um buzzer como alerta sonoro.
  - A contagem pode ser pausada, retomada ou reiniciada tanto por botões físicos quanto por comandos enviados via site.
  - Caso o Wi‑Fi configurado não esteja disponível ou precise ser alterado, o ESP32 entra automaticamente em modo Access Point, permitindo a reconfiguração da rede pelo endereço http://192.168.4.1.
  - Em caso de desligamento do dispositivo, o progresso da contagem é salvo na memória não volátil (NVS), registrando se o tempo já atingiu 0%, 25%, 50% ou 75% do total.

## Tecnologias utilizadas
  - ESP32
  - Buzzer
  - Botões em pull-down
  - Html, css e js
  - HiveMQ

## Aplicações
  O sistema pode ser aplicado em ambientes industriais ou oficinas que necessitam de controle de manutenção periódica de máquinas. O projeto pode ser expandido para incluir automações adicionais, como integração com sistemas de gestão, notificações remotas ou acionamento automático de processos de manutenção.

## Autor
  Victor Gabriel El Rachman Carvalho da Silva, estudante do ensino médio integrado ao curso de informática.




