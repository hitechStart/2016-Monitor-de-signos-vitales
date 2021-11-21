#include <TimeLib.h>//Biblioteca para las funciones de fecha y hora
#include <NewTone.h>///Biblioteca para las funciones de los tono del pulso 

#include <Wire.h>//biblioteca para la pantalla de cristal liquido,Los pines analógicos 4 y 5 de Arduino son los que usa la librería Wire.
#include <LiquidCrystal_I2C.h>//biblioteca para la pantalla de cristal liquido

#define I2C_ADDR    0x3F
LiquidCrystal_I2C lcd(I2C_ADDR,2, 1, 0, 4, 5, 6, 7);// asignamos estas salidas para usar la LCD
 //***************************definicion de variables**************************************
 
long T0 = 0 ;  // Variable global para tiempo
// Variables volatiles para las rutinas de interrupcion
volatile int BPM;                   // variable que contiene el valor analógica en A0, se actualiza cada 2mS
volatile int Signal;                // contiene los datos de la señal proveniente de analógica A0, (por lo general oscilarria entre 500 y 1000- desde 0 a 1023)
volatile int IBI = 600;             //variable que contiene el intervalo de tiempo entre latidos, se fija en un rango promedio de 600 ms
volatile boolean Pulse = false;     // "True" cuando se detecta los latidos del corazón en directo.
volatile boolean QS = false;        // Verdadero cuando se encuentra un latido.

char command;//Letra recibida desde el celular
int flag=0,r=0;//contador para mostrar el resultado cada 20 pulsos, es para estabilizar la señal al pulso correspondiente del paciente

float temp,valor=995,lectura_Max=995,lectura_Min=1000,lectura_del_oxigeno;//variables para el oxigeno
int pulso;
time_t t;

void setup(){
  Serial.begin(9600);

      lcd.begin (16,2);    // Inicializar el display con 16 caracteres de 2 lineas
       lcd.setBacklightPin(3,POSITIVE);
       lcd.setBacklight(HIGH);//iniciamos el fondo retroiluminado
       lcd.home ();  

      iniciarTarjetaSD();
 
     
      //hh-mm-ss dia-mes-año
     setTime(19,33,00,21,11,2016);
     pinMode(7,OUTPUT);        // pin por donde se visualiza el latido del corazon y los valores de braquicardia y taquicardia asi como nivel febril,hipotermia y Hiperpirexia
                                // tambien los niveles de hipoxia
     pinMode(2,OUTPUT);        // pin por donde se escucha el latido del corazon en el buzzer
     pinMode(8,OUTPUT);        // pin por donde se visualiza el estado del corazon para valores normales, termepratura y spo2
      pinMode(10, OUTPUT); //establecemos el pin 10 como medio de comunicacion de la SD
      
     interruptSetup();      //funcion que activa las interrupciones
     delay(1000);

     
     command='z';//luego del saludo inicial pasamos a mostrar la fecha en señal que el dispositivo esta en stand by
}

void loop(){ 
 t = now();//Declaramos un objeto del tipo time para establecer la fecha

  //forma de comunicacion con el android
  while(Serial.available() > 0) //mientras se ingresen datos entonces,,,,,,
    {
      command = ((byte)Serial.read()); // los tomamos y selecccionamos el caso que queremos utilizar
     }
     
      switch(command){

    case 'a':
    lcd.clear();
   
     //enviamos los datos al celular
   // Serial.print('#');
    Serial.print("Siga las indicaciones");
    Serial.println('~');

    lcd.print("   Siga las ");
    lcd.setCursor(0,1);    // Ponte en la fila 0, posicion 1
    lcd.print("  indicaciones ");
    delay(28000);
    command='g';
    break;

    case 'g':
    lcd.clear();
    
    medirPulso();
    
    if( r==20){
      lcd.clear();
      lcd.print("BPM ");
         if(pulso <60){
    
    lcd.print("Bradicardia");
    digitalWrite(7,HIGH);   
    }     
   
   else if(pulso >100){
   
    lcd.print("Taquicardia"); 
    digitalWrite(7,HIGH);   
      }
      else
      {
    lcd.print("Normal"); 
    digitalWrite(8,HIGH);   
        }
        
        delay(5000);  
        digitalWrite(7,LOW);         
        digitalWrite(8,LOW); 
        r=0; 
       
    break;
    
    case 'b':
     lcd.clear();
       
    //Serial.print('#');
    Serial.print("Siga las indicaciones");
    Serial.println('~');
    
   lcd.print("   Siga las ");
   lcd.setCursor(0,1);    // Ponte en la line 1, posicion 6
   lcd.print("  indicaciones ");
    delay(28000);
     medirTemperatura();
    lcd.clear();
     command ='z'; // hacemos por terminda la actividad
    break;
    
    case 'e':
    lcd.clear();
   
   // Serial.print('#');
    Serial.print("Almacenando datos");
    Serial.println('~');
    
    lcd.setCursor(0,0);
    lcd.print("Almacenando dato");
    lcd.setCursor(0,1);
    lcd.print("en registro");
     imprimirDatos();
     delay(5000);
     lcd.clear();

     //Serial.print('#');
    Serial.print("Guardado");
    Serial.println('~');
    
     lcd.print("    Guardado");
     delay(3000); 
     lcd.clear();
     command ='z';
    break;

    case 'd':
 
  // lcd.print("   dibujando");
  // lcd.setCursor(0,1);
   //lcd.print("  cardiograma");
   
    Serial.print('S');
    Serial.println(analogRead(3));
    
    if (QS == true){   
    
    Serial.print('B');
    Serial.println(BPM/3.25);
    Serial.print('Q');
    Serial.println(IBI);
    }
    QS = false; 
 
    break;

    case 'c':

     lcd.clear();
       
       oximetro();
    
       if(flag==20){
        
      lcd.setCursor(0,1);
      if(lectura_del_oxigeno> 94 && lectura_del_oxigeno < 100){
        lcd.print("Nivel Normal");
         digitalWrite(8,HIGH); 
        }
        else if(lectura_del_oxigeno>90 && lectura_del_oxigeno<95){
          lcd.print("Hipoxia Leve");
           digitalWrite(7,HIGH);
          }
          else if(lectura_del_oxigeno>85 && lectura_del_oxigeno<91){
          lcd.print("Hipoxia Moderada");
           digitalWrite(7,HIGH);
          }
          else
          {
            lcd.print("Hipoxia Severa");
             digitalWrite(7,HIGH);
            }
            lectura_Max=995;
            lectura_Min=1000;
           
          delay(3000);
          digitalWrite(7,LOW);         
          digitalWrite(8,LOW); 
          flag=0;
           command ='z';
      }
    }

   
    break;  
     case 'f':
      lcd.print("    MOVISIGN"); // mostramos la palabra MOVISIGN en la LCD
      delay(5000);
      lcd.clear();
    command='z';
      
     break;
     
     case 'z' :
   lcd.clear();
   lcd.setCursor(3,0);   
   lcd.print(day(t));
   lcd.print(+ "/") ;
  lcd.print(month(t));
  lcd.print(+ "/") ;
  lcd.print(year(t));
  lcd.setCursor(6,1); 
  lcd.print(hour(t));  
  lcd.print(+ ":") ;
  lcd.print(minute(t));
  delay(1000);
        }                 
}
