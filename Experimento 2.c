#include <REG51F.H>

sbit cl = P2^0;
sbit ch = P2^1;

#define valor 9500; // quantidade de vezes para completar 1 segundo

void maquinaCH()
{
	static char state = 'C';
	static unsigned long int tempo;
	
	switch(state) {
		case 'A':
			if (ch == 0x01) { // se o 'botao' ch for pressionado
				state = 'B';
				tempo = valor;
			}
			break;
		case 'B':
			P1 = (P1 & 0x0F) | (P0 & 0xF0);
			if(!tempo--) { // caso se tenha passado 1 segundo, vai pro estado 'C'
				state = 'C';
			}
			break;
		case 'C':
			P1 &= 0x0F; // zera os 4 bits mais significativos de P1 e so vai pro estado A se o botao nao estiver sendo pressionado
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
	static unsigned long int tempo;
	
	switch(state) {
		case 'A':
			if (cl == 0x01) { // se o 'botao' cl for pressionado
				state = 'B';
				tempo = valor;
			}
			break;
		case 'B':
			P1 = (P1 & 0xF0) | (P0 & 0x0F);
			if(!tempo--) { // caso se tenha passado 1 segundo, vai pro estado 'C'
				state = 'C';
			}
			break;
		case 'C': // zera os 4 bits menos significativos de P1 e so vai pro estado A se o botao nao estiver sendo pressionado
			P1 &= 0xF0;
			if (cl == 0x00) {
				state = 'A';
			}
			break;			
	}
	return;
}

void main()
{	
	while(1) {		
		maquinaCH();
		maquinaCL();
	}
	
	return;
}