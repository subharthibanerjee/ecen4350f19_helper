# include "your_header.h" // reg51.h

/*
 		Check reference: http://sdcc.sourceforge.net/doc/sdccman.pdf 
 		Page 44 is ISR

 		Okay I just have defined my ISR

 		it still does not work: why?

 		use EA = 1/ES = 1
*/
volatile unsigned char received_byte=0; // should I use static or volatile here?

volatile unsigned char received_flag = 0;
// check .rst file after compilation: what is in IVT
// there can be another trick used
void ISR_receive() __interrupt (4) {

	 

	if (RI == 1){
		received_byte = SBUF;
		RI = 0;
		received_flag= 1;

	}
	


}


void UART_Init(){

    SCON = 0x50;  // Asynchronous mode, 8-bit data and 1-stop bit
    TMOD = 0x20;  // Timer1 in Mode2. in 8 bit auto reload
    TH1 =  0xFD;  // Load timer value for 9600 baudrate
    TR1 = 1;      // Turn ON the timer for Baud rate generation
    ES  = 1;      // Enable Serial INterrupt
    EA  = 1;      // Enable Global Interrupt bit
}


void UART_transmit(unsigned char byte){


    SBUF = byte;
    while(TI == 1);
    TI = 0;
}


void main(){
	if (recieved_flag == 1){
		// do something with received_byte
		recieved_flag = 0;
	}
}
