#INCLUDE <16f887.h>
//#USE DELAY(CLOCK=4000000) // Reloj 4MHz
#DEVICE adc=10
#use delay(clock=4000000,crystal)//Crystal Externo 4MHz
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,NOPUT,NOLVP
//Definiciones para colocar el LCD y el teclado en el puerto B
#DEFINE USE_PORTB_LCD TRUE
#DEFINE USE_PORTB_KBD TRUE  
//Librer�as del LCD y Teclado

#INCLUDE <LCD.C>
#INCLUDE <KBD.C> 
//Definiciones
#BYTE PORTC= 7
#BYTE PORTB= 6
#BYTE PORTA= 5
#DEFINE LED1 PORTC,4
#DEFINE LED2 PORTC,7
//Variables Globales
BYTE CONST DISPLAY[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67}; //numeros de display 7 segmentos
INT CONTADOR;
int T;
int CONT;
CHAR K;
INT8 MIL,CEN,DEC,UNI,VAL;
INT16 contra=9999; //Variable entera para almacenar la contrase�a
INT8 sinal; //Variable para saber si tengo contrase�a
VOID TECLADO()
{
k=kbd_getc(); //Llamar la funci�n del teclado y almacenar
//el valor digitado en una variable tipo
//car�cter. Si no se oprime ninguna tecla el
//teclado retornara el car�cter nulo.*/
WHILE(k=='\0') //si no se oprime ninguna tecla sigue llamando al teclado.
{
k=kbd_getc();
}
IF( (k!='\0'))
{
IF(K=='0')//Si K es igual a cero
VAL=0;//Val es igual a cero
IF(K=='1')
VAL=1;
IF(K=='2')
VAL=2;
IF(K=='3')
VAL=3;
IF(K=='4')
VAL=4;
IF(K=='5')
VAL=5;
IF(K=='6')
VAL=6;
IF(K=='7')
VAL=7;
IF(K=='8')
VAL=8;
IF(K=='9')
VAL=9;
}
}
//Rutina para pedir la clave
void pedir_clave(void)
{
TECLADO();
IF((k!='#')&&(k!='*'))
{
lcd_putc('*');
MIL=VAL;
}
if(k=='*' || k=='#')
return;
TECLADO();
IF((k!='#')&&(k!='*'))
{
lcd_putc('*');
CEN=VAL;
}
if(k=='*' || k=='#')
return;
TECLADO();
IF((k!='#')&&(k!='*'))
{
lcd_putc('*');
DEC=VAL;
}
if(k=='*' || k=='#')
return;
TECLADO();
IF((k!='#')&&(k!='*'))
{
lcd_putc('*');
UNI=VAL;
}
if(k=='*' || k=='#')
return;
}
int validar_clave(void)
{
//Variables locales
int16 clave=0,m,c,d,u;
//compilador (_mul)*/
m=MIL*1000; //Convierto miles a numero
//c=_mul(CEN,100); //Otra alternativa para multiplicar int8*int8=int16
c=(int16)CEN*100; //Convierto centemas, haciendo un CAST para obtener un int16
d=DEC*10; //Convierto decenas a numero 
u=UNI; //Convierto unidades a numero 
clave=m+c+d+u;
//Borra lo �ltimo que fue digitado en el teclado
MIL=0;CEN=0;DEC=0;UNI=0; 
if(clave==contra) //Si la clave es igual a la contrase�a
return(1);
else
return(0);
}
void cambio_clave(void)
{
int bandera=0;
int16 clave=0,m,c,d,u;
LCD_PUTC("\f");
LCD_GOTOXY(1,1);
LCD_PUTC("CLAVE ACTUAL");
LCD_GOTOXY(1,2);      
pedir_clave(); //Llama la funcion de pedir la clave
bandera=validar_clave(); //Compruebo si la clave actual es correcta
if(bandera==1)
{
LCD_PUTC("\f");
LCD_GOTOXY(1,1);
LCD_PUTC("CLAVE NUEVA  ");
LCD_GOTOXY(1,2);   
TECLADO();
IF((k!='#')&&(k!='*'))
{
lcd_putc('*');
MIL=VAL;
}
TECLADO();
IF((k!='#')&&(k!='*'))
{
lcd_putc('*');
CEN=VAL;
}
TECLADO();
IF((k!='#')&&(k!='*'))
{
lcd_putc('*');
DEC=VAL;
}
TECLADO();
IF((k!='#')&&(k!='*'))
{
lcd_putc('*');
UNI=VAL;
}
m=MIL*1000; //Convierto miles a numero
c=CEN*10; //Convierto centemas a numero y lo sumo al anterior
c=c*10;
d=DEC*10; //Convierto decenas a numero y lo sumo al anterior
u=UNI; //Convierto unidades a numero y lo sumo al anterior
clave=m+c+d+u;
contra=clave;
WRITE_EEPROM(0,MIL); //Guarda en la eemprom posicion cero la nueva contrase�a
WRITE_EEPROM(1,CEN);
WRITE_EEPROM(2,DEC);
WRITE_EEPROM(3,UNI);
WRITE_EEPROM(4,12); //Guardo un 12 en la posicion 1 de la EEPROM, para decir que tengo
//una contrase�a guardada


}
else
{
LCD_PUTC("\f");
LCD_GOTOXY(1,2);
LCD_PUTC(" CLAVE INVALIDA ");
DELAY_MS(4000);
LCD_PUTC("\f");
}
}
VOID MAIN()
{
//Variables Locales

int16 m,c,d,u;
INT8 m1,c1,d1,u1;
//Puerto C como Salida
SET_TRIS_C(0B00000000);
PORTC=0; //Limpia Puerto C
lcd_init(); //Inicializa el LCD
kbd_init(); //Inicializa el Teclado
//Configura las Resistencias PULL-UP del Puerto B
port_b_pullups(0xFF); //PIC16F887
//port_b_pullups(TRUE); //PIC16F877A
sinal=READ_EEPROM(4); //Averiguo si tengo una contrase�a guardada o no
if(sinal!=12)
contra=9999;
else
{ 
//Lee los datos del EEPROM
m1=READ_EEPROM(0); 
c1=READ_EEPROM(1); 
d1=READ_EEPROM(2); 
u1=READ_EEPROM(3); 
/* Para realizar la multiplicaci�n cuando se tienen numeros del tipo int8, 
como por ejemplo 100 con otro tipo int8 como por ejemplo CEN, el programa
har� un producto int8, sin embargo se desea un resultado int16, por lo 
tanto debe informarce al compilador por medio de un 'cast' (c=(int16)CEN*100)
o utilizando la funci�n de multiplicaci�n de tipos del compilador (_mul)*/
m=m1*1000; //Convierto miles a numero
//c=_mul(c1,100); //Otra alternativa para multiplicar int8*int8=int16
c=(int16)c1*100; //Convierto centemas, haciendo un CAST para obtener un int16
d=d1*10; //Convierto decenas a numero 
u=u1; //Convierto unidades a numero 
contra=m+c+d+u;
}
WHILE(TRUE)
{
LCD_PUTC("\f");
LCD_GOTOXY(1,1);
LCD_PUTC(" DIGITE CLAVE ");
LCD_GOTOXY(1,2);
DELAY_MS(1000);
pedir_clave(); //Llama la funcion de pedir la clave
if(k=='*')
cambio_clave();
else
{
WHILE((k!='#'))
{
TECLADO();
if(k=='*')
cambio_clave();
}

if(validar_clave()==1) //Aqu� se compara si
//los n�meros digitados
//est�n correctos.*/
{

LCD_PUTC("\f"); //Se borra LCD
LCD_GOTOXY(1,1);//Se ubica en la posici�n 1,1
LCD_PUTC(" CLAVE CORRECTA ");
DELAY_MS(1000);
LCD_PUTC("\f"); //Se borra LCD
LCD_PUTC("DIGITE UN NUMERO ");
DELAY_MS(1000);
LCD_PUTC("\f"); //Se borra LCD
port_b_pullups(0xFF); //se mide la resistencia al presionar 
do
{

k=kbd_getc();//se obtiene el dato 
T=k-48; //vuelve el dato ascii a decimal 
if(k!=0){
printf(lcd_putc,"\f ha seleccionado \n elnumero %c",k); // mensaje que muestra el numero 
delay_ms(5);

//inicio led VERDE
if(T==3){   
output_low(pin_e1); //apaga led
output_high(pin_e1); //enciende led
   delay_ms(1000);       
}else{
output_low(pin_e1); //apaga led
}
//fin de led VERDE

//inicio led azul
if(T==4){   
output_low(pin_e2);
output_high(pin_e2);  
   delay_ms(500);       
}else{
output_low(pin_e2);
}
//fin led azul


//Motor Tiempor recomendado inico
if(T==1){

byte const HORARIO[16] = { 0b1000, //se crea un vector con 16 posiciones
                           0b1100,
                           0b0100,
                           0b0110,
                           0b0110,
                           0b0100,
                           0b1100,
                           0b1000,
                           0b1000,
                           0b1100,
                           0b0100,
                           0b0110,
                           0b0110,
                           0b0100,
                           0b1100,
                           0b1000,};
                           

              
   SET_TRIS_A(0B00000000);                //Configura el puerto A como salida
   WHILE(T==1)
   {
              CONT=0;                
         WHILE((CONT<16)&&(T==1))  //Mientras que cont sea menor a 16
                                          //y T==1
         {
            PORTA=(HORARIO[CONT]);        //Env�e al puerto A la informaci�n //de
                                          //la tabla de horario
            DELAY_MS(500);                //Retardo de 500 milisegundos
            CONT++; //Incremente la variable cont
            if(CONT==16){
            T=0;
            }
         }
      
   }
}

//MOTOR recomendado

//siete segmentos y motor

IF(T==2){
  
   byte const HORARIO[10] = { 0b1000,
                              0b1100,
                              0b0100,
                              0b0110,
                              0b0010,
                              0b0011,
                              0b0001,
                              0b1001,
                              0b1100,
                              0b0110,};
   
   
   
   
      CONTADOR = 0;                     
      WHILE((CONTADOR<10)&&(T==2))              // Mientras contador < 10 y t==2
      {
      //MOTOR
      SET_TRIS_A(0B00000000); 
      PORTA=(HORARIO[CONTADOR]);        //Env�e al puerto A la informaci�n //de
                                          //la tabla de horario
            DELAY_MS(500);                //Retardo de 500 milisegundos
            if(CONTADOR==10){
            T=0;
            }
      //MOTOR
      
      //siete segmentos
         SET_TRIS_C(0B00000000); //Configura el puerto c
         PORTC= DISPLAY[CONTADOR];      // Muestre el valor en el display
         CONTADOR++;                   //Incrementa TEMPORIZADOR
         DELAY_MS(500); // Retardo de 500 
         if(CONTADOR==10){
         T==0;
         output_low(pin_c0);    //apagar segmentos
         output_low(pin_c1);     
         output_low(pin_c2);     
         output_low(pin_c3);     
         output_low(pin_c4);     
         output_low(pin_c5);     
         output_low(pin_c6);     
         }
      } 
      //siete segmentos
      
  //fin motor y siete segmentos  
}
if(T==5){
long bits;     //Variable almacena los bits
float tem;     //Almacena la temperatura
tem=0;
bits=0;
printf(lcd_putc,"\f ha seleccionado \n elnumero %c",k); // mensaje que muestra el numero 
       delay_ms(1000);
   set_tris_e(1);          //Pongo el Re0 como entrada        
   setup_adc_ports(sAN5);     //Pongo todo el puerto a analogo
   setup_adc(adc_clock_internal);   //Selecciono reloj interno para conversion
   lcd_init();                      //Inicializo el LCD
   lcd_putc("\f");                  //Borro el LCD
   CONTADOR=0;
  
   while(CONTADOR<10)
   {
       set_adc_channel(5);          //Selecciono el canal 0 (RA0)
       delay_ms(1);                 //llamo retardo de 1 ms
       bits=read_adc();             //Guarde el dato del LM en temp

       tem=bits*0.4882;              //Conversion de bits a temperatura
       lcd_gotoxy(1,1);             //Ubiquese en la posicion 1,1
       lcd_putc("LA TEMPERATURA");
       lcd_gotoxy(2,2);             //Ubiquese en la posicion 2,2
       printf(lcd_putc,"ES C= %f    ",tem);  //Muestra el valor numerico de la conversionconversion
       delay_ms(500);  
      
       byte const HORARIO[10] = { 0b1000,
                              0b1100,
                              0b0100,
                              0b0110,
                              0b0010,
                              0b0011,
                              0b0001,
                              0b1001,
                              0b1100,
                              0b0110,};
        CONTADOR=0;
       WHILE((CONTADOR<10)&&(tem>50))              // Mientras contador < 10 y t==2
      {
      
      
       lcd_putc("LA TEMPERATURA");
       lcd_gotoxy(2,2);             //Ubiquese en la posicion 2,2
       printf(lcd_putc,"ES C= %f    ",tem);  //Muestra el valor numerico de la conversionconversion
       delay_ms(500);  
      //MOTOR
      SET_TRIS_A(0B00000000); 
      PORTA=(HORARIO[CONTADOR]);        //Env�e al puerto A la informaci�n //de
      CONTADOR++;                                  //la tabla de horario
            DELAY_MS(500);                //Retardo de 500 milisegundos
            
  
      }
      
   T=0;

}


}

}

}

WHILE (true);

}
if(validar_clave()==0);
{
LCD_PUTC("\f");
LCD_GOTOXY(1,1);
LCD_PUTC(" CLAVE INVALIDA ");
DELAY_MS(1000);
LCD_PUTC("\f");

}


}

}

}
