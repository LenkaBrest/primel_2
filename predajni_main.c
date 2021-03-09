/*
 * File:   predajni_main.c
 * Author: Lenka
 *
 * Created on 06. mart 2021., 11.43
 */


#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include<p30fxxxx.h>
#include "tajmeri.h"

_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);

unsigned char tempRX;
unsigned int broj1,broj2;
int stanje_gore, gore, stanje_dole, dole, stanje_levo, levo, stanje_desno, desno, stanje_sirena, sirena, stanje_svetla, svetla, stanje_turbo, turbo;

//---------------------------------
unsigned int brojac_ms,stoperica,ms,sekund;

/*******************************************************************
//funkcija za kasnjenje u milisekundama
*********************************************************************/
void Delay_ms (int vreme)//funkcija za kasnjenje u milisekundama
	{
		stoperica = 0;
		while(stoperica < vreme);
	}



void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 1ms
{

		TMR2 =0;
     ms=1;//fleg za milisekundu ili prekid;potrebno ga je samo resetovati u funkciji

	brojac_ms++;//brojac milisekundi
    stoperica++;//brojac za funkciju Delay_ms

    if (brojac_ms==1000)//sek
        {
          brojac_ms=0;
          sekund=1;//fleg za sekundu
		 } 
	IFS0bits.T2IF = 0; 
       
}

void initUART1(void)
{
    U1BRG=0x0207;//ovim odredjujemo baudrate

    U1MODEbits.ALTIO=1;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne

    IEC0bits.U1RXIE=1;//omogucavamo rx1 interupt

    U1STA&=0xfffc;

    U1MODEbits.UARTEN=1;//ukljucujemo ovaj modul

    U1STAbits.UTXEN=1;//ukljucujemo predaju
}

void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
    tempRX=U1RXREG;

} 

/*********************************************************************
* Ime funkcije      : WriteUART1                                     *
* Opis              : Funkcija upisuje podatak u registar 			 *
* 					  za slanje podataka U1TXREG,   				 *
* Parametri         : unsigned int data-podatak koji treba poslati   *
* Povratna vrednost : Nema                                           *
*********************************************************************/

void WriteUART1(unsigned int data)
{
	  while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void RS232_putst(register const char *str)
{
  while((*str)!=0)
  {
    WriteUART1(*str);
    if (*str==13) WriteUART1(10);
    if (*str==10) WriteUART1(13);
    str++;
  }
}


int main(void) {
    
    ADPCFGbits.PCFG5=1;
    ADPCFGbits.PCFG9=1;
    ADPCFGbits.PCFG10=1;
    ADPCFGbits.PCFG11=1;
    ADPCFGbits.PCFG12=1;

    
    TRISBbits.TRISB5 = 1; //svetla
    TRISBbits.TRISB9 = 1; //pravo
    TRISBbits.TRISB10 = 1; //levo
    TRISBbits.TRISB11 = 1; //dole
    TRISBbits.TRISB12 = 1; //desno
    TRISDbits.TRISD9 = 1; //sirena
    TRISFbits.TRISF4 = 1; //turbo
    LATBbits.LATB12 = 0;
    LATDbits.LATD9 = 0;
    
    stanje_sirena = 0;
    sirena = 0;
    initUART1();
    Init_T2();
    
    while(1)
    {
        
         
       
        if(PORTBbits.RB12 == 1) { 
            
           RS232_putst("Aa");
           RS232_putst("DESNO");
           
        }
        
        
          if(PORTFbits.RF4 == 1) { 
            
              RS232_putst("Aa");
           RS232_putst("TURBO");
        }

          if(PORTBbits.RB11 == 1) { 
            
              RS232_putst("Aa");
           RS232_putst("DOLE");
        }

          if(PORTBbits.RB10 == 1) { 
            
              RS232_putst("Aa");
           RS232_putst("LEVO");
        }
        if(PORTBbits.RB9 == 1) { 
            
              RS232_putst("Aa");
           RS232_putst("PRAVO");
        
        }

        if(PORTBbits.RB5 == 1) { 
            
              RS232_putst("Aa");
           RS232_putst("SVETLA");
        }

        
      
				/*{   RS232_putst("nesto4");
					if(stanje_sirena<21)//stanje-taster mora da bude pritisnut jedno vreme
      					stanje_sirena++;
                       RS232_putst("hgh");
            }
			else
      				stanje_sirena=0;//taster nije bio dovoljno dugo pritisnut
           RS232_putst("nije");

			if(stanje_sirena==21)//ako je taster zadrzan u pritisnutom stanju jedno vreme 
                //RS232_putst("aaasirena");
     				 sirena = 1;//smatramo ga pritisnutim;ovim otklanjamo mogucnost 'odskakanja tastera'
           RS232_putst("100");

			if(sirena==1)//ukoliko je taster pritisnut menjamo stanje diode
				{
                    RS232_putst("aaasirena");
                    sirena=0;
                    //Delay_ms(500);
				}
        
        /*if(PORTDbits.RD9 == 1) {
        
            sirena = 0;
        
        }
        
        else 
            
        {
        
            sirena = 1;
        
        }*/
        
     /*  switch(sirena) {
        
            case 0:
                 WriteUART1('l');
                 break;
                 
             case 1:
                 WriteUART1('h');
                 break;
                 }*/ 
        
        
       /* RS232_putst("pocetak");
        
            /*if(PORTDbits.RD9==1)
				{
					if(stanje_sirena<21)//stanje-taster mora da bude pritisnut jedno vreme
      					stanje_sirena++;
            }
			else
      				stanje_sirena=0;//taster nije bio dovoljno dugo pritisnut

			if(stanje_sirena==20)//ako je taster zadrzan u pritisnutom stanju jedno vreme 
     				 sirena = 1;//smatramo ga pritisnutim;ovim otklanjamo mogucnost 'odskakanja tastera'
        

			if(sirena==1)//ukoliko je taster pritisnut menjamo stanje diode
				{
                    RS232_putst("aaasirena");
                    sirena=0;
                    stanje_sirena = 0;
				}	
            /*
              if(PORTBbits.RB5==1)
				{
					if(stanje_svetla<21)//stanje-taster mora da bude pritisnut jedno vreme
      					stanje_svetla++;
				}
			else
      				stanje_svetla=0;//taster nije bio dovoljno dugo pritisnut

			if(stanje_svetla==20)//ako je taster zadrzan u pritisnutom stanju jedno vreme 
     				 svetla = 1;//smatramo ga pritisnutim;ovim otklanjamo mogucnost 'odskakanja tastera'
        

			if(svetla==1)//ukoliko je taster pritisnut menjamo stanje diode
				{
				 RS232_putst("aaasvetla");
				svetla=0;
				}	
            
              if(PORTBbits.RB9==1)
				{
					if(stanje_gore<21)//stanje-taster mora da bude pritisnut jedno vreme
      					stanje_gore++;
				}
			else
      				stanje_gore=0;//taster nije bio dovoljno dugo pritisnut

			if(stanje_gore==20)//ako je taster zadrzan u pritisnutom stanju jedno vreme 
     				 gore = 1;//smatramo ga pritisnutim;ovim otklanjamo mogucnost 'odskakanja tastera'
        

			if(gore==1)//ukoliko je taster pritisnut menjamo stanje diode
				{
				 RS232_putst("aaagore");
				gore=0;
				}	
            
              if(PORTBbits.RB10==1)
				{
					if(stanje_levo<21)//stanje-taster mora da bude pritisnut jedno vreme
      					stanje_levo++;
				}
			else
      				stanje_levo=0;//taster nije bio dovoljno dugo pritisnut

			if(stanje_levo==20)//ako je taster zadrzan u pritisnutom stanju jedno vreme 
     				 levo = 1;//smatramo ga pritisnutim;ovim otklanjamo mogucnost 'odskakanja tastera'
        

			if(levo==1)//ukoliko je taster pritisnut menjamo stanje diode
				{
				 RS232_putst("aaalevo");
				levo=0;
				}	
            
              if(PORTBbits.RB11==1)
				{
					if(stanje_dole<21)//stanje-taster mora da bude pritisnut jedno vreme
      					stanje_dole++;
				}
			else
      				stanje_dole=0;//taster nije bio dovoljno dugo pritisnut

			if(stanje_dole==20)//ako je taster zadrzan u pritisnutom stanju jedno vreme 
     				 dole = 1;//smatramo ga pritisnutim;ovim otklanjamo mogucnost 'odskakanja tastera'
        

			if(dole==1)//ukoliko je taster pritisnut menjamo stanje diode
				{
				 RS232_putst("aaadole");
				dole=0;
				}	
 
            
              if(PORTBbits.RB12==1)
				{
					if(stanje_desno<21)//stanje-taster mora da bude pritisnut jedno vreme
      					stanje_desno++;
				}
			else
      				stanje_desno=0;//taster nije bio dovoljno dugo pritisnut

			if(stanje_desno==20)//ako je taster zadrzan u pritisnutom stanju jedno vreme 
     				 desno = 1;//smatramo ga pritisnutim;ovim otklanjamo mogucnost 'odskakanja tastera'
        

			if(desno==1)//ukoliko je taster pritisnut menjamo stanje diode
				{
				 RS232_putst("aaadesno");
				desno=0;
				}	
            
            
              if(PORTFbits.RF4==1)
				{
					if(stanje_turbo<21)//stanje-taster mora da bude pritisnut jedno vreme
      					stanje_turbo++;
				}
			else
      				stanje_turbo=0;//taster nije bio dovoljno dugo pritisnut

			if(stanje_turbo==20)//ako je taster zadrzan u pritisnutom stanju jedno vreme 
     				 turbo = 1;//smatramo ga pritisnutim;ovim otklanjamo mogucnost 'odskakanja tastera'
        

			if(turbo==1)//ukoliko je taster pritisnut menjamo stanje diode
				{
				 RS232_putst("aaadesno");
				turbo=0;
				}	*/

    }
    
    return 0;
}
