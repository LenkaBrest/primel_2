#include <p30fxxxx.h>
#include <outcompare.h>
#include "timer2.h"
#include "tajmeri.h"

#define IN1 LATBbits.LATB12
#define IN2 LATBbits.LATB9
#define IN3 LATBbits.LATB10
#define IN4 LATBbits.LATB11

_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);


unsigned int broj1,broj2,r;
unsigned char tempRX;

void initUART1(void)
{
    U1BRG=0x0040;//ovim odredjujemo baudrate

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

void WriteUART1(unsigned int data)
{
	  while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}


void __attribute__((__interrupt__)) _T2Interrupt(void)
{

   	TMR2 =0;
    IFS0bits.T2IF = 0;
}


int main(void)
	{
        initUART1();
        for(broj1=0;broj1<10000;broj1++);

        ADPCFGbits.PCFG9=1;
        ADPCFGbits.PCFG10=1;
        ADPCFGbits.PCFG11=1;
        ADPCFGbits.PCFG12=1;
        
        TRISBbits.TRISB9=0;
        TRISBbits.TRISB10=0;
        TRISBbits.TRISB11=0;
        TRISBbits.TRISB12=0;
        
        //diode
        TRISFbits.TRISF4=0;
        TRISFbits.TRISF5=0;
        TRISDbits.TRISD9=0;


        TRISDbits.TRISD0=0;//konfigurisemo kao izlaz za pwm
        TRISDbits.TRISD1=0;

 LATBbits.LATB9=0;//RB9,RB10,RB11,RB12 za upravljanje L298
        LATBbits.LATB10=0;
        LATBbits.LATB11=0;
        LATBbits.LATB12=0;
        
        LATBbits.LATB0=0;
        LATBbits.LATB1=0;
        LATBbits.LATB2=0;
        LATBbits.LATB3=0;
        

        //za PWM koristimo RD0 i RD1
        PR2=2499;//odredjuje frekvenciju po formuli
        
        OC1RS=20;//postavimo pwm
        OC1R=1000;//inicijalni pwm pri paljenju samo
        OC1CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
        
        OC2RS=20;//postavimo pwm
        OC2R=1000;//inicijalni pwm pri paljenju samo
        OC2CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
        
        
        T2CONbits.TON=1;//ukljucujemo timer koji koristi
        
        //r=2500;
        r=2500;
        OC1RS=r;//ovim postavljamo faktor ispune
        OC2RS=r;
        
        for(broj1=0;broj1<700;broj1++)
        for(broj2=0;broj2<3000;broj2++);

	while(1)
	{
        LATFbits.LATF4=1;
        LATFbits.LATF5=1;
        LATDbits.LATD9=1;
        
        if(tempRX == 'u')
            WriteUART1(tempRX+48);
        
        LATBbits.LATB9=1;
           LATBbits.LATB10=1;
           LATBbits.LATB11=0;
           LATBbits.LATB12=0;
            
            OC1RS=1900;//ovim postavljamo pwm
            OC2RS=1900;


            for(broj1=0;broj1<20;broj1++)
                for(broj2=0;broj2<60;broj2++);







}//od whilea
return 0;
}//od maina


//za 100% mora se staviti OC1RS>PR2 ili koji je vec izabran

//za 0% dovoljno je staviti OC1RS na 0 
//rezolucija pwma zavisi od izabrane periode


