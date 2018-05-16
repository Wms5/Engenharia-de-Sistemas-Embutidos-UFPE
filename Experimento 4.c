#include <REG51F.H>

#define FrClk 12000000
#define FreqTimer0_emHz 100

char c;
bit control = 0x0;

void serial_inicializa()
{
	TR1 = 0;
	TMOD = (TMOD & 0x0F) | 0x20;
	SCON = SCON | 0x50;
	PCON = PCON | 0x80;
	TH1 = 0xCC;
	ES = 1;
	ET1 = 0;
	TR1 = 1;
}

void serial_int(void) interrupt 4 using 2
{
	if(RI) {
		RI = 0;
		c = SBUF;
		control = 1;
	}
	if(TI) {
		TI = 0;
	}
}

void main()
{
	serial_inicializa();
	EA = 1;
	while(1) {
		if(control) {
			control = 0;
			c += 0x01;
			SBUF = c;
		}
	}

	return;
}
