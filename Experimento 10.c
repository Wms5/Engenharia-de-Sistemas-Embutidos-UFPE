#include <REG517A.H>
#include <string.h>

#define Freqtimer0_emHz 100

char opt = 0;
unsigned char duty = 0xFF;

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
		opt = S0BUF;

		switch(opt){
			case '0':
				duty = 254;
				break;
			case '1':
				duty = 224;
				break;
			case '2':
				duty = 196;
				break;
			case '3':
				duty = 168;
				break;
			case '4':
				duty = 140;
				break;
			case '5':
				duty = 112;
				break;
			case '6':
				duty = 84;
				break;
			case '7':
				duty = 56;
				break;
			case '8':
				duty = 28;
				break;
			case '9':
				duty = 0;
				break;
		}
		//Modificar o CML
		CML0 = duty;
		CML1 = duty;
		CML2 = duty;
		CML3 = duty;
		CML4 = duty;
		CML5 = duty;
		CML6 = duty;
		CML7 = duty;

    }
}

void main()
{
	P4 = 0x00;
    CTCON = 0x00; //Seleciona a frequencia do compare timer e a interrupção a ser chamada(nesse caso nenhuma) e frequencia é Fosc/2)

    //Registrador de comparação, quando o valor de contador chegar ao valor armazenado uma porta é setada para high, a mesma ira voltar para low quando o contador der overflow.

    CMH0 = 0xFF;
    CML0 = 0x00;
    CMH1 = 0xFF;
    CML1 = 0x00;
    CMH2 = 0xFF;
    CML2 = 0x00;
    CMH3 = 0xFF;
    CML3 = 0x00;
    CMH4 = 0xFF;
    CML4 = 0x00;
    CMH5 = 0xFF;
    CML5 = 0x00;
    CMH6 = 0xFF;
    CML6 = 0x00;
    CMH7 = 0xFF;
    CML7 = 0x00;


    CMSEL = CMSEL | 0xFF; //Seta o modo comparador 0 para o registrador CM0(16-bit)
    CMEN = CMEN | 0xFF; //Ativa a função de comparação para CM0(16-bit)


    CTRELH = 0xFF; // Seta o valor de reload do timer quando o mesmo da overflow
    CTRELL = 0x00; // Quando recebe o valor inicia o timer

    serial_inicializa();
    EAL = 1;

    while(1) {
    }

    return;
}
