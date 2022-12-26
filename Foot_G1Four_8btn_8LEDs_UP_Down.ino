 /*
 * Código criado para ZOOM G1Four/B1Four.
 * 8 pedais para patches com 8 LEDs.
 * Pedal 4 também funciona para descer bancos ao ser pressionado por 1 segundo.
 * Pedal 8 também funciona para subir bancos ao ser pressionado por 1 segundo.
 * Criado por Alcimar Ramos
 * https://youtube.com/c/alcimarramos
 * https://www.instagram.com/alcimar_ramos
 */

/* Bibliotecas utilizadas */
#define ATMEGA328
#include <MIDI.h>
#include <Usb.h>
#include <usbhub.h>
#include <usbh_midi.h>

/* Definindo nome para os pinos de entrada, onde ficam os botões */
#define pino2 2  /*  pedal patch1 */
#define pino3 3  /*  pedal patch2 */
#define pino4 4  /*  pedal patch3 */
#define pino5 5  /*  pedal patch4 & Bank Down */
#define pino6 6  /*  pedal patch5 */
#define pino7 7  /*  pedal patch6 */
#define pino8 8  /*  pedal patch7 */
#define pino9 9  /*  pedal patch8 & Bank Up */

/* Definindo nome para os pinos de saída onde ficam os LEDs */
#define led1  A0 /*  LED pedal 1 */
#define led2  A1 /*  LED pedal 2 */
#define led3  A2 /*  LED pedal 3 */
#define led4  A3 /*  LED pedal 4 & Bank Down*/
#define led5  A4 /*  LED pedal 5 */
#define led6  A5 /*  LED pedal 6 */
#define led7  10 /*  LED pedal 7 */
#define led8  11 /*  LED pedal 8 & Bank Up*/

/* Definindo comunicação Midi USB */
USB Usb; 
USBH_MIDI Midi(&Usb);

/* Variáveis recebendo patches e bancos */
byte patchA0 = 0x00;
byte patchA1 = 0x01;
byte patchA2 = 0x02;
byte patchA3 = 0x03;
byte patchA4 = 0x04;
byte patchA5 = 0x05;
byte patchA6 = 0x06;
byte patchA7 = 0x07;
byte patchA8 = 0x08;
byte patchA9 = 0x09;
byte ccc[]= {0x00, 0x01, 0x02, 0x03, 0x04};

/* Variáveis que receberão  o acionamento dos botões nos pinos de entrada */
int pedal1;
int pedal2;
int pedal3;
int pedal4;
int pedal5;
int pedal6;
int pedal7;
int bank_Down;
int bank_Up;
int i = 0;
int bank = 0;
int outPinErr = 13; /* Se Algo der errado LED 13 ficará piscando */
int espera = 333; /* Tempo em ms até poder enviar o comando novamente */

unsigned long tempoInicio = 0;
unsigned long tempoBotao = 0;

bool estadoBotao;
bool estadoBotaoAnt;

bool estadoBotao2;
bool estadoBotaoAnt2;

void setup() {

/* Definindo pinos de entrada */
   pinMode(pino2, INPUT_PULLUP);
   pinMode(pino3, INPUT_PULLUP);
   pinMode(pino4, INPUT_PULLUP);
   pinMode(pino5, INPUT_PULLUP);
   pinMode(pino6, INPUT_PULLUP);
   pinMode(pino7, INPUT_PULLUP);
   pinMode(pino8, INPUT_PULLUP);
   pinMode(pino9, INPUT_PULLUP);

/* Definindo pinos de saída */
   pinMode(led1, OUTPUT);
   pinMode(led2, OUTPUT);
   pinMode(led3, OUTPUT);
   pinMode(led4, OUTPUT);
   pinMode(led5, OUTPUT);
   pinMode(led6, OUTPUT);
   pinMode(led7, OUTPUT);
   pinMode(led8, OUTPUT);
   
   Serial.begin(115200); /* inicia comunicação serial e define a velocidade */
   iniciar();            /* Função que inicia comunicação MIDI USB */
}

void loop() {
/* Variáveis que recebem  o acionamento dos botões nos pinos de entrada */
   pedal1 = digitalRead(pino2);
   pedal2 = digitalRead(pino3);
   pedal3 = digitalRead(pino4);
   bank_Down = !digitalRead(pino5);
   pedal5 = digitalRead(pino6);
   pedal6 = digitalRead(pino7);
   pedal7 = digitalRead(pino8);
   bank_Up = !digitalRead(pino9);

  /* PEDAL 1 */
  if ( pedal1 == LOW ) {             /* Se o Pedal 1 for acionado */
      SendMIDI(patchA1 , ccc[bank]); /* Envia mensagem midi para ir pro Patch 1 */
      led1on();
      //Serial.println("Patch 1");
      delay(espera);                 /* Tempo em milesegundos oara debounce */
    }

  /* PEDAL 2 */
  if ( pedal2 == LOW ) {
      SendMIDI(patchA2 , ccc[bank]);
      led2on();
      //Serial.println("Patch 2");
      delay(espera);
    }

  /* PEDAL 3 */
  if ( pedal3 == LOW ) {
      SendMIDI(patchA3 , ccc[bank]);
      led3on();
      //Serial.println("Patch 3");
      delay(espera);
    }

  /* PEDAL 5 */
  if ( pedal5 == LOW ) {
      SendMIDI(patchA5 , ccc[bank]);
      led5on();
      //Serial.println("Patch 5");
      delay(espera);
    }

  /* PEDAL 6 */
  if ( pedal6 == LOW ) {
      SendMIDI(patchA6 , ccc[bank]);
      led6on();
      //Serial.println("Patch 6");
      delay(espera);
    }

   /* PEDAL 7 */
   if ( pedal7 == LOW ) {
      SendMIDI(patchA7 , ccc[bank]);
      led7on();
      //Serial.println("Patch 7");
      delay(espera);
    }
    

////////////// PEDAL 4 & BANK DOWN //////////////

   //Ao apertar o botão 4
  if ( bank_Down && !estadoBotaoAnt ) {
    if (tempoInicio == 0) {
      tempoInicio = millis();
    }
  }  

  //Ao soltar o botão 4
  if (tempoInicio > 200) {  //Filtro Debounce
    if (!bank_Down && estadoBotaoAnt) {
      tempoBotao = millis() - tempoInicio;
      tempoInicio = 0;
  }

 //Comando 1
  if ((tempoBotao > 0) && (tempoBotao <= 500)) {
     tempoBotao = 0;
     SendMIDI(patchA4 , ccc[bank]);
     led4on();
     //Serial.println("Patch 7");
 }
 
  //Comando 2
  if ((tempoBotao > 500) && (tempoBotao <= 3000)) {
     tempoBotao = 0;
     bankDown();
     ledsoff();
     led4on();
     ledsoff();
     led4on();
     ledsoff();
     led4on();
     ledsoff();
     //Serial.println("Bank Down acionado");
    }
  
  estadoBotaoAnt = bank_Down;
  delay(20);
}

////////////// PEDAL 8 & BANK UP //////////////

   //Ao apertar o botão 8
  if ( bank_Up && !estadoBotaoAnt2 ) {
    if (tempoInicio == 0) {
      tempoInicio = millis();
    }
  }  

  //Ao soltar o botão 8
  if (tempoInicio > 200) {  //Filtro Debounce
    if (!bank_Up && estadoBotaoAnt2) {
      tempoBotao = millis() - tempoInicio;
      tempoInicio = 0;
  }

 //Comando 1
  if ((tempoBotao > 0) && (tempoBotao <= 500)) {
     tempoBotao = 0;
     SendMIDI(patchA8 , ccc[bank]);
     led8on();
     //Serial.println("Patch 8");
 }
 
  //Comando 2
  if ( tempoBotao > 500 ) {
     tempoBotao = 0;
     bankUp();
     ledsoff();
     led8on();
     ledsoff();
     led8on();
     ledsoff();
     led8on();
     ledsoff();
     //Serial.println("Bank Up acionado");
    }
  
  estadoBotaoAnt2 = bank_Up;
  delay(20);
  }

}



/* --------------------------------------------------------------------- */
/* ****************************** FUNÇÔES ****************************** */
/* --------------------------------------------------------------------- */



/* Acende o Led 1 e apaga os demais */
void led1on() {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      digitalWrite(led7, LOW);
      digitalWrite(led8, LOW);
      }

/* Acende o Led 2 e apaga os demais */
void led2on() {
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      digitalWrite(led7, LOW);
      digitalWrite(led8, LOW);
      }

/* Acende o Led 3 e apaga os demais */
void led3on() {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      digitalWrite(led7, LOW);
      digitalWrite(led8, LOW);
      }

/* Acende o Led 4 e apaga os demais */
void led4on() {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, HIGH);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      digitalWrite(led7, LOW);
      digitalWrite(led8, LOW);
      }

/* Acende o Led 5 e apaga os demais */
void led5on() {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, HIGH);
      digitalWrite(led6, LOW);
      digitalWrite(led7, LOW);
      digitalWrite(led8, LOW);
      }
      
/* Acende o Led 6 e apaga os demais */
void led6on() {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, HIGH);
      digitalWrite(led7, LOW);
      digitalWrite(led8, LOW);
      }

/* Acende o Led 7 e apaga os demais */
void led7on() {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      digitalWrite(led7, HIGH);
      digitalWrite(led8, LOW);
      }
      
/* Acende o Led 8 e apaga os demais */
void led8on() {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      digitalWrite(led7, LOW);
      digitalWrite(led8, HIGH);
      }

void ledsoff(){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      digitalWrite(led7, LOW);
      digitalWrite(led8, LOW);
}


/* Função que envia as mensagens midi */
void SendMIDI(byte number, byte bankcc){
  Usb.Task();
  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING ){ 
    byte Cc_1[] = {0xb0, 0x00,  0x00}; 
    byte Cc_2[] = {0xb0, 0x20, bankcc};
    byte Message[2];                 /* Construindo mensagem midi (2 bytes) */
    Message[0] = 0xC0;               /* 0xC0 para Program Change */
    Message[1] = number;             /* Number é o program/patch */
    Midi.SendData(Cc_1);
    Midi.SendData(Cc_2);      
    Midi.SendData(Message);          /* Envia a mensagem */
    delay(50); 
    }else{
    digitalWrite(outPinErr, HIGH);
    delay (5);
    digitalWrite(outPinErr, LOW);
    delay (5);
    digitalWrite(outPinErr, HIGH);
    delay (5);
    digitalWrite(outPinErr, LOW);
    delay (5);
    digitalWrite(outPinErr, HIGH);
    delay (5);
    digitalWrite(outPinErr, LOW);
      }
    }

/* Função que inicia a comunicação MIDI USB e envia o primeiro patch por padrão */
void iniciar() {
  Serial.println("USB NAO INICIOU");
  if (Usb.Init() == -1) { 
      while (1);
  }
  Serial.println("USB INICIOU");
  while (Usb.getUsbTaskState() != USB_STATE_RUNNING) {Usb.Task();} /* se USB iniciou, continue */
  Serial.println("USB CONECTOU");
  for (int i = 0; i < 4; i++) { /* forçando os 4 primeiros comandos enviados pra pedaleira que são ignorados */
       SendMIDI(patchA1 , ccc[bank]);
       Serial.println("Iniciou Patch 1");
       delay(200);
  }
       led1on(); //Acende o Led 1  
}    



/* Função para Descer os Bancos */
void bankDown() {
  if( i > 0 ){
      i--;
      bank = i;
      delay(50);
   }
    if( i == 0 ) { 
       i = 5;
       bank = i;
       delay(50);
      }
}

/* Função para Subir os Bancos */
void bankUp() {
  if( i >= 0 && i <= 5){
      i++;
      bank = i;
      delay(50);
   }
    if( i > 5 ) { 
       i = 0;
       bank = i;
       delay(50);
      }
}
