/******************************************************************************

                            Example code for crc8
                        parity, serial transmit and receive 

*******************************************************************************/

#include <stdio.h>
#include <string.h>


# define BITS_IN_BYTE 8
typedef unsigned char u8; //typedef u8

// function pointer definition of the crc_x where x is 16, 32, etc.
u8 (*CRC)(u8*, int);

u8 compute_c8(u8 *data, int len); // function declaration

u8 compute_c8_one_byte(u8 data);
// union declaration for for serial frames
// why union? The size of this structure is 8 byte in here
typedef struct PACKET_T{
    u8 start;                   // start is a byte
    u8 *data_bytes;             // 8 bit data
    u8 data_len;                // length of the data
    u8 parity:1;                // parity is a bit
    u8 crc8;                    // crc is a byte here
    u8 stop[2];                 // stop bits 
}packet_t;

 

// ======================== CRC algorithm Implementation ======================


// computer crc for a frame
u8 compute_c8(u8 *data, int len){
    u8 crc = 0;
    for (int i = 0; i < len; i++){
        crc ^= data[i];
        crc = compute_c8_one_byte(crc);
    }
    return crc;
}

// compute crc for the byte
u8 compute_c8_one_byte(u8 data){
    
    const u8 generator = 0x1D; // constant 
    u8 crc = data;
    
    // from msb
    for (int i = 0; i < sizeof(data)*BITS_IN_BYTE; i++){
        // if msb = 1
        if ((crc & 0x80) != 0){
            crc = (crc << 1) ^ generator;
        
        }
        // if msb == 0
        else{
            crc <<= 1;
        }
    }
    return crc;
    
}
// =============================================================================



// create packet or frame 

// I will use malloc here to copy the data_bytes
// Please do not get concerned 
// you can easily do -----------
// data_byte[0] = "A"
// data_byte[1] = "B"
// etc
packet_t* create_frame(u8 *data){
    packet_t *frame;
    frame = (packet_t*) malloc(sizeof(packet_t));
    frame->data_bytes = (u8*)malloc(sizeof(u8)*strlen(data));
    // string copy
    strcpy(frame->data_bytes, data);
    
    frame->start = 0x00;
    frame->stop[0] = 0xff;
    frame->stop[1] = 0xff;
    frame->parity = 0;
    
    // store function pointer 
    CRC = compute_c8; // why a function pointer? because you can have
    // function pointer directly inside struct
    
    frame->crc8 = CRC(frame->data_bytes, strlen(frame->data_bytes));
    return frame;
    
}


// print the frame
void print_frame(packet_t *frame){
    printf("\n\n+++++++++++++++ FRAME +++++++++++++++++\n\n");
    printf("\n frame.start \t\t= 0x%x", frame->start);
    printf("\n frame.data_bytes \t= %s", frame->data_bytes);
    printf("\n frame.parity \t\t= %d", frame->parity);
    printf("\n frame.crc8 \t\t= 0x%x", frame->crc8);
    printf("\n frame.stop[0] \t\t= 0x%x", frame->stop[0]);
    printf("\n frame.stop[1] \t\t= 0x%x", frame->stop[1]);
    printf("\n\n+++++++++++FRAME COMPLETE ++++++++++++++++\n\n");
    
}

// main function
int main()
{
    printf("----------- CRC Implementation ----------- \n");
    u8 *transmit_data_bytes = "Hello World\r\n";
    packet_t *frame;
    printf("Creating frame\n");
    frame = create_frame(transmit_data_bytes);
    printf("Creation of frame complete\n");
    
    printf("Printing frames\n");
    
    print_frame(frame);
    
    
    return 0;
}
