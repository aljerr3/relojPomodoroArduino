#include <TM1637Display.h>
#include <ESP8266WiFi.h>

#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494

// Definimos los pines para el display TM1637
#define CLK D1
#define DIO D2

// Creamos una instancia de TM1637Display
TM1637Display display(CLK, DIO);

// Definimos los pines para el botón pulsador
#define BUTTON_PIN D3

// Definimos los pines para el buzzer activo
const int buzzerPin = D6;

// Definimos las variables para el temporizador
int workTime = 25; // Duración del tiempo de trabajo en minutos
int breakTime = 5; // Duración del tiempo de descanso en minutos
int flag = 0;
int cont = 0;
int currentTime = 0; // El tiempo actual, en segundos
bool isWorking = true; // Si estamos en modo de trabajo o de descanso

// Creamos una variable para almacenar el tiempo del último botón pulsador
unsigned long lastButtonTime = 0;

void setup() {
  // Configuramos el botón pulsador
  pinMode(D0, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  // Configuramos el display TM1637
  display.setBrightness(0x0f);
}

void startSong() {
  // Sonido de notificaciones de iPhone
  int notes[] = { 1567, 1397, 1319, 1175, 1047 };
  int duration[] = { 50, 50, 50, 50, 150 };

  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, notes[i], duration[i]);
    delay(duration[i]);
    noTone(buzzerPin);
    delay(50);
  }
}

void clickSong() {
  // Sonido de notificaciones de iPhone
  int notes[] = { 1176, 1567, 1389 };
  int duration[] = { 50, 50, 250 };

  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, notes[i], duration[i]);
    delay(duration[i]);
    noTone(buzzerPin);
    delay(50);
  }
}

void alarm() {
  // Melodía alegre
  int melody[] = { 261, 329, 392, 329, 261 };
  int duration[] = { 500, 500, 500, 500, 500 };

  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, melody[i], duration[i]);
    delay(duration[i]);
    noTone(buzzerPin);
    delay(50);
  }
}

void loop() {
  int minutes = currentTime / 60;
  int seconds = currentTime % 60;
  display.showNumberDecEx((minutes * 100 + seconds), 0b01000000, true);

  // Comprobamos si el botón pulsador ha sido presionado
  if (digitalRead(BUTTON_PIN) == LOW) {
  digitalWrite(D0, HIGH);
  cont++;
  Serial.println(cont);

      if (cont > 1 && flag == 0){
      flag = 1;
      startSong();
    }

    if (cont > 2) { // Si han pasado al menos 1 segundo desde el último botón pulsador
      flag = 0;
      isWorking = !isWorking; // Cambiamos entre trabajo y descanso
      currentTime = isWorking ? workTime * 60 : breakTime * 60; // Actualizamos el tiempo actual
      cont = 0;
      clickSong();
    }
  }

   if (digitalRead(BUTTON_PIN) != LOW) {
    cont = 0;
      digitalWrite(D0, LOW);
   }

  // Comprobamos si el tiempo ha llegado a cero
  if (currentTime <= 0) {
    alarm();
    flag = 0;
    isWorking = !isWorking; // Cambiamos entre trabajo y descanso
    currentTime = isWorking ? workTime * 60 : breakTime * 60; // Actualizamos el tiempo actual
  }

    // Reducimos el tiempo actual en un segundo
  if (flag == 1){
  delay(1000);
  currentTime--;
  digitalWrite(D0, LOW);
  }

  if (flag == 0){
    digitalWrite(D0, HIGH);
    delay(100);
    digitalWrite(D0, LOW);
    delay(100);
  }
}