#include <TimeLib.h>//Biblioteca para las funciones de fecha y hora

#include <SPI.h>//biblioteca para la targeta SD
#include <SD.h> // biblioteca para la targeta SD
//***************************definicion de variables**************************************
 
int i,j;
String s="";
String cadena="";

// definicion de caracteres especial corazon (pantalla LCD)
byte corazon[8] =
 {
    0b00000,     // Los definimos como binarios 0bxxxxxxx
    0b00000,
    0b01010,
    0b01110,
    0b01110,
    0b00100,
    0b00000,
    0b00000
 };
 
byte borrado[8] =
 {
    0b00000,     // Los definimos como binarios 0bxxxxxxx
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
 };

// definicion de caracteres especial Grados centigrados º (pantalla LCD)
byte grados[8] =
 {
    0b01100,     // Los definimos como binarios 0bxxxxxxx
    0b10010,
    0b10010,
    0b01100,
    0b00000,
    0b00000,
    0b00000,
    0b00000
 };
 //******************************************************************************************************

void iniciarTarjetaSD(){
  
 if (!SD.begin(10)) {  //Si no llega a estar la SD en el modulo entonces notifica error por el monitor serial
    lcd.print("SD fail"); 
    delay(3000); 
    return;   
  }
}
 
void medirPulso(){ 
      
  if (QS == true){        //estamos ascendiendo en la señal, por lo tanto se detecta un pulso
    lcd.setCursor(0,0);    // se posiciona en la fila 0, coumna cero                   
    pulso=((IBI/3.5)+(BPM/3.25))/2; // obtengo el maximo y el minimo, luego hacemos el promedio
    
    lcd.setCursor(0,0);    // se posiciona en la fila uno, coumna cero                   
    lcd.print("BPM= ");
    
    digitalWrite(7,HIGH); 
    lcd.createChar(1,corazon); // y de grados centigrados
    lcd.setCursor(15,0); //Situamos el cursor la fila 0 columna 15
    lcd.write(byte(1));
    NewTone(2, 700, 1000/7); // la duracion de la nota es de 1000/7 en una frecuencia alta de 700 sobre la salida digital 2 del arduino
    delay (IBI);  
 
    lcd.setCursor(5,0);
    lcd.print(pulso); //el BPM se promedio en 3,25 veces tomado de la variable analógica en A0,
    
    //enviamos los datos al celular
    //Serial.print('#');
    Serial.print("BPM: ");
    Serial.print(pulso);
    Serial.println(' ');
      
     r++; // contamos un latido
        }
     QS = false;  //aca ya dejo de latir el pulso, paso a la zona baja de la señal 

     digitalWrite(7,LOW);  
     
     lcd.createChar(2,borrado); // emoticon vacio para generar el efecto invisible del corazon
     lcd.setCursor(15,0); //Situamos el cursor la fila 0 columna 15
     lcd.write(byte(2));  
     noNewTone(2); // silencia el sonido que se obtiene por la salida digital 2 del arduino     
     delay(520);  //esperamos hasta el siguiete pulso, es dato
     if((BPM/3.25)>99){// si el BMP es mayor que 100 entonces refresco para que no deje xxA siendo A un numero y xx menor a 100 pisandose
      
      lcd.clear();
      }       
}
void medirTemperatura(){
     
     temp = (5.0*analogRead(A1)*100.0)/1024.0 + 2.0; // realizamos el calculo de la temperatura
                                                      //constante de calibracion o de conuctividad del calor;
     //si el sensor mide mas de 30 grados lo tiene en la axila, por lo tanto esta midiendo
   if(temp > 32){
         
       lcd.clear();
   
       for(i=60;i>=1;i--){
        
       Serial.println(temp);
        lcd.setCursor(0,1);    // Ponte en la line 1, posicion 6
      lcd.print("Duracion: ");
      s=String(i); // muestra en pantalla LCD la cuenta regresiva
       lcd.setCursor(10,1); 
       lcd.print(s) ;
      delay(1000);
      lcd.clear();
       }
 
   NewTone(2, 700, 1000/7);
   delay(1200);
   noNewTone(2); // silencia el sonido que se obtiene por la salida digital 2 del arduino  
         
 lcd.setCursor(0,0);  
 lcd.print("TEMP: ");
 lcd.setCursor(6,0);
 lcd.print(temp);
 lcd.createChar(2,grados); // y de grados centigrados
 lcd.setCursor(12,0);
 lcd.write(byte(2)); 
 lcd.setCursor(13,0); 
 lcd.print("C");
     
 //enviamos los datos al celular
   // Serial.print('#');
    Serial.print("TEMP: ");
    Serial.print(temp);
    Serial.print(" C");
    Serial.print('~');
     
   if(temp < 34.5){
  cadena="Hipotermia";
  digitalWrite(7,HIGH);  
  }else
  {
    if( temp >34.5){
      
      if( temp <37.5){
        
     cadena="Nivel Normal"; 
      digitalWrite(8,HIGH);
      } 
        
      }else
      {
        if(temp >37.5)
        { 
          if(temp <38.5){
          cadena="Nivel Febril"; 
          digitalWrite(7,HIGH); 
          }  
        }else
        {
          if(temp > 40.0)
          {
            cadena="Hiperpirexia";
            digitalWrite(7,HIGH);
          }         
        }        
       }
    }   
  lcd.setCursor(0,1);    // Ponte en la line 1, posicion 6
  lcd.print(cadena);
 delay(5000);
 digitalWrite(7,LOW);  
 digitalWrite(8,LOW);   
 
}
else{ // si no tiene el sensor puesto en la axila entonces se produce un error
     lcd.clear();
     lcd.print("  Sin medicion"); 
     delay(3000);
  }
}
void imprimirDatos(){
 File dataFile = SD.open("REGISTRO.TXT",FILE_WRITE);

  if (dataFile) { // si se creo/abrio el archivo entonces sigo
  dataFile.print(day(t));
  dataFile.print(+ "/") ;
  dataFile.print(month(t));
  dataFile.print(+ "/") ;
  dataFile.print(year(t)); 
  dataFile.print( " ") ;
  dataFile.print(hour(t));  
  dataFile.print(+ ":") ;
  dataFile.println(minute(t));

  //variables globales
    dataFile.print("BMP: ");
    dataFile.println(pulso);
    dataFile.print("SpO2: ");
    dataFile.println(lectura_del_oxigeno);
    dataFile.print("Valor de Temperatura: ");
    dataFile.println(temp);
    dataFile.println("---------------------------------------------------------------------");
    dataFile.close();
 
  }
 
  else {
    lcd.print("     Error");
  }
 }
 void oximetro(){
   valor=analogRead(3);
     if( valor > 400){
      if( valor< 650){
      if( valor < lectura_Min){  
      lectura_Min=valor;
       }}}  

         if(valor < 1000){
       if(valor> lectura_Max){  
      lectura_Max=valor;
        } 
     }

      if(valor > 1000){
        lectura_Max=995; 
        }
     
     lectura_del_oxigeno=((lectura_Max-lectura_Min/lectura_Max)/10.00);
     
      Serial.print("SpO2: ");
      Serial.print(lectura_del_oxigeno);
      Serial.print('~');
                      
      lcd.print("SpO2= ");
      lcd.setCursor(6,0);
      lcd.print(lectura_del_oxigeno);

       delay(1000);

       flag++;
       
  
  }
