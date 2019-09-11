/******************************************************************************
                            Example code for crc8
                        parity, serial transmit and receive 
*******************************************************************************/

#include <stdio.h>
#include <string.h>

# define __GENERATOR__ 0x1D // generator value

typedef unsigned char u8; //typedef u8
typedef unsigned short int u16;  //typedef u16 value
#define __BITS_IN_BYTE__ 8

// function pointer definition of the crc_x where x is 16, 32, etc.
u8 (*CRC)(u8*, int);

u8 compute_c8(u8 *data, int len); // function declaration

u8 compute_c8_one_byte(u8 data);


// union declaration

typedef union PACKET_T{
    u16 data;
    u8 bytes[sizeof(u16)];
}packet_t;




 
// print the polynomial 
void print_poly(u16 value){
    printf("Printing the polynomial = \t");
    for (int i = sizeof(value)*__BITS_IN_BYTE__ - 1; i >= 0; i--){
        int bits = (value & (1 << i)) >> i;

        
        if (i == 0 && bits == 1){
            printf("1");
        }
        else if (i == 0 && bits == 0){
            printf("1");
        }
        else if (bits == 1){
            printf("x^%d + ", i);
        }

    }
   
    printf("\n");
    
}




void
print_bits_reverse (u16 n, int len)
{
  printf("-Bit representation ---\t");   
  // simple reverse loop
  for (int i = len - 1 ; i >= 0; i--)
    {
      printf ("%d", (n & (1 << i)) >> i);
    }
  printf ("\n");
}

// ======================== CRC algorithm Implementation ======================


// computer crc for a frame
u8 compute_c8(u8 *data, int len){
    u8 crc = 0;
    for (int i = 0; i < len; i++){
        crc ^= data[i];
        crc = compute_c8_one_byte(crc);
        printf("data = 0x%x \t CRC = 0x%x\n", data[i], crc);
    }

    return crc;
}

// compute crc for the byte
u8 compute_c8_one_byte(u8 data){
    
    const u8 generator = __GENERATOR__; // constant 
    u8 crc = data;
    
    // from msb
    for (int i = 0; i < sizeof(data)*__BITS_IN_BYTE__; i++){
        // if msb = 1
        if ((crc & 0x80) != 0){
            crc = (u8) (crc << 1) ^ generator;
        
        }
        // if msb == 0
        else{
            crc <<= 1;
        }
    }
    return crc;
    
}
// =============================================================================



void
print_bytes (unsigned char *byte, int len)
{

  for (int i = 0; i < len; i++)
    {
      printf ("byte[%d] = 0x%X\n", i, byte[i]);
    }
}


// main function
int main()
{
   
    u16 value = 0x0201;
    packet_t frame;
    frame.data = value;
    printf("\n\n\n\n\n----------- CRC Implementation ----------- \n\n\n\n");
    printf("data in the frame --> 0x%X \n", frame.data);
    print_bits_reverse(value, sizeof(value)*__BITS_IN_BYTE__);
    print_poly(value);
   
    print_bytes(frame.bytes, sizeof(frame));
    
    printf("Creation of frame complete\n");
    
    printf("CRC calculated by bytes 0x%X\n", compute_c8(frame.bytes, sizeof(frame)));
    
    
    
    printf("\n\n\n\n-------------------------------------------- \n\n\n");
    return 0;
}
