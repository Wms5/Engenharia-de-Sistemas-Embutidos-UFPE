#include <REG517A.H>
#include <string.h>
#define Freqtimer0_emHz 100

char c;
bit control = 0x0,txTransmitindo=0;
unsigned char inT = 0, outT = 0;
unsigned char inR = 0, outR = 0;
char bufferT[16];
char bufferR[16];
char s[21];

void sendString(char *s);

bit RxBufferVazio(){
    return (inT == outT);
}

char receiveChar(){
    char c = bufferR[outR];

    outR++;
    outR%=16;
    return c;
}
void receiveString(char *s){
    char c;
    unsigned char it = 0;
    bit a=1;

    inR = outR = 0;

    while(a){
      if(inR > outR){
        c = receiveChar();
        if(c == 0x0D){
            s[it++] = '\n';
            s[it] = '\0';
            a = 0;
        }else{
            s[it++] = c;
        }
      }
    }
}
void sendChar(char c){
    if((inT+1)%16 != outT){
        bufferT[inT] = c;
        inT++;
        inT%=16;
    }

    if(!txTransmitindo){
        txTransmitindo = 1;
        TI0=1;
    }
}

void sendString(char *s){
    unsigned char len = strlen(s), i=0;

    for(i=0; i<len;i++){
        sendChar(s[i]);
    }

}

void serial_inicializa()
{
        PCON = PCON | 0x80;
        ADCON0 = ADCON0 | 0x80;
        S0CON = 0x50;
	ES0 = 1;
}

void serial_int(void) interrupt 4 using 2
{
    if(RI0) {
        RI0 = 0;
        if(((inR+1)%16) != outR){
            bufferR[inR] = S0BUF;
            inR++;
            inR%=16;
        }
    }
    if(TI0) {
        TI0 = 0;
        if(RxBufferVazio()) txTransmitindo=0;
        else{
            S0BUF = bufferT[outT];
            outT++;
            outT%=16;
        }
    }
}

void main()
{
    serial_inicializa();
    EAL = 1;

    while(1) {
        receiveString(s);
        sendString(s);
    }

    return;
}
