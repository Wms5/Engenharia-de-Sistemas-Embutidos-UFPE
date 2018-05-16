#include <REG51F.H>

sbit cl = P2^0;
sbit ch = P2^1;

void main()
{
	while(1) {
		if (cl == 0x01) {
			P1 = (P1 & 0xF0) | (P0 & 0x0F);
		}
		else {
			P1 &= 0xF0;
		}

		if (ch == 0x01) {
			P1 = (P1 & 0x0F) | (P0 & 0xF0);
		}
		else {
			P1 &= 0x0F;
		}
	}
}
