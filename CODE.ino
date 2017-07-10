// HP-MIDI By Moseley Miranda
// -------------------DIAGRAMA MIDI-------------------
//   ARDUINO----CODE----CONVESSOR----AUDIO COMPOSER.
// ---------------------------------------------------
//  ARDUINO MINI
//  BOTÃO LED
//  LED
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
int controlChange = 176; // Controle de mudaça MIDI Canal 1
int potiWert[16];      
int controllerWert[16];
int controllerWertAlt[16];
static const unsigned ledPin = 13;// {LEDPIN} 13 LED pino do Arduino
int i = 0;

int bit1 = 0;
int bit2 = 0;
int bit3 = 0;


void setup() {
 //Seleção de pinos 4051s
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);
 pinMode(ledPin, OUTPUT);
    MIDI.begin(4); // Iniciar MIDI canal 4
 
    Serial.begin(9600); // Porta de comunicação serial
}


void loop() {
 for (i = 0; i <= 7; i++) {
 bit1 = bitRead(i,0);
 bit2 = bitRead(i,1);
 bit3 = bitRead(i,2);
 
 digitalWrite(2, bit1);
 digitalWrite(3, bit2);
 digitalWrite(4, bit3);
  
 potisAbfragen(i,A0);
 potisAbfragen(i+8,A1);
   if (MIDI.read())                    // If we have received a message
    {
        digitalWrite(ledPin, HIGH);     //Ligado
        MIDI.sendNoteOn(42, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
        delay(1000);                    // Tempo em segundos
        MIDI.sendNoteOff(42, 0, 1);     // Stop the note
        digitalWrite(ledPin, LOW);      //Desligado
    }
 }
}


void potisAbfragen(int zaehler, int analogPin) {
 potiWert[zaehler] = 0.2 * potiWert[zaehler] + 0.8 * analogRead(analogPin);
 controllerWert[zaehler] = map(potiWert[zaehler],0,1023,0,127);
 if (controllerWert[zaehler] != controllerWertAlt[zaehler]) {
 sendeMIDI(controlChange, (20+zaehler), controllerWert[zaehler]);
 controllerWertAlt[zaehler] = controllerWert[zaehler];
 }
}


void sendeMIDI(int statusByte, int dataByte1, int dataByte2) {
 Serial.write(statusByte);
 Serial.write(dataByte1);
 Serial.write(dataByte2);
}
 
