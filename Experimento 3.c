#include <REG51F.H>

sbit cl = P2^0;
sbit ch = P2^1;

#define FrClk 12000000
#define CORRECAO 7 // esta correcao eh necessaria para evitar o acumulo de erro quando se executa as instrucoes da funcao de interrupcao
#define FreqTimer0_emHz 100
#define VALOR_TH0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) >> 8)
#define VALOR_TL0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)

unsigned int counter1_10ms = 0; // contador de 10ms para a maquina de estados 1
unsigned int counter2_10ms = 0; // contador de 10ms para a maquina de estados 2

void maquinaCH()
{
	static char state = 'C';
	
	switch(state) {
		case 'A':
			if (ch == 0x01) {
				state = 'B';
				counter1_10ms = 0;
			}
			break;
		case 'B':
			P1 = (P1 & 0x0F) | (P0 & 0xF0);
			if(counter1_10ms == 100) { // quando o contador eh igual a 100, ou seja 100 * 10ms = 1s, vai pro estado 'C'
				state = 'C';
			}
			break;
		case 'C':
			P1 &= 0x0F;
			if (ch == 0x00) {
				state = 'A';
			}
			break;			
	}
	return;
}

void maquinaCL()
{
	static char state = 'C';
	
	switch(state) {
		case 'A':
			if (cl == 0x01) {
				state = 'B';
				counter2_10ms = 0;
			}
			break;
		case 'B':
			P1 = (P1 & 0xF0) | (P0 & 0x0F);
			if(counter1_10ms == 100) { // quando o contador eh igual a 100, ou seja 100 * 10ms = 1s, vai pro estado 'C'
				state = 'C';
			}
			break;
		case 'C':
			P1 &= 0xF0;
			if (cl == 0x00) {
				state = 'A';
			}
			break;			
	}
	return;
}

void timer0_inicializa()
{
	TR0 = 0;
	TMOD = (TMOD & 0xF0) | 0x01;
	TH0 = VALOR_TH0;
	TL0 = VALOR_TL0;
	ET0 = 1;
	TR0 = 1;
}

void timer0_int(void) interrupt 1 using 2 // a cada 10 milisegundos acontece overflow no T0 e essa rotina eh chamada
{
	TR0 = 0;
	TH0 += VALOR_TH0;
	TL0 += VALOR_TL0;
	TR0 = 1;
	counter1_10ms++;
	counter2_10ms++;
}

void main()
{
	timer0_inicializa();
	EA = 1;
	
	while(1) {	
		maquinaCH();
		maquinaCL();	
	}
	
	return;
}