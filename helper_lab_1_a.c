/******************************************************************************
                                     HELPER CODE
                            code compilation for endianness
                endianness can be defined atomically: bit, byte, 2 byte
                In programming languages bits are not readily accessible, 
            they are manipulated. Please check the comments/questions I have 
        received today: 09/27/2019. Please check differnt functions in this code
        where I have addressed the students' questions. Please ask questions in
                                    the class
        
copy paste this in onlinegdbcompiler: https://www.onlinegdb.com/online_c_compiler
*******************************************************************************/

#include <stdio.h>

#define __BITS_IN_BYTE__ 8
// # define unsigned int uint16;  // define it either way, which one is faster?
typedef unsigned short int uint16;	// for reference 2 byte unsigned integer

void print_bytes (unsigned char *byte, int len);
uint16 change_endian (uint16 i);





// please understand union and struct
// TODO: make a 32 bit implementation
typedef union uint16_u
{
  uint16 word;			// this arch 16 bit for short
  unsigned char bytes[2];

} UT;


int main ()
{
  UT            ut;
  uint16        scanned_int;


  printf ("\n===== HELPER CODE FOR LECTURE 1.a ======= \n");
  printf ("Scanning the integer: \t");
  scanf ("%hx", &scanned_int);
  ut.word = scanned_int;
  printf ("The size of the scanned integer: %ld bytes and %ld bits\n",
	  sizeof (scanned_int), sizeof (scanned_int) * __BITS_IN_BYTE__);

  printf ("\nByte endian (Good question Daniel! )--- \n");
  print_bytes(ut.bytes, sizeof(ut));
  printf ("\nNow to answer Reid's solution (Thank you Reid!)\n");
  ut.word = change_endian(scanned_int);
  printf("\n Return reverse endianness according to Reid Rise: \n");
  print_bytes(ut.bytes, sizeof(scanned_int));
  return 0;
}

/* 
    Daniel Faronbi:  He talked endianness about byte. He is correct too. It is 
    hardware dependent. Lets check here by using a union.
    
*/

void
print_bytes (unsigned char *byte, int len)
{

  for (int i = 0; i < len; i++)
    {
      printf ("byte[%d] = 0x%X\n", i, byte[i]);
    }
}

/*  
    Reid Rise:  He talked about this implementation to change endianness
    in this work. He is completey right.
    For the understanding I have printed the memory addresses also. 
    
    Please implement it for 4 bytes to understand how to change byte positions
    
*/
uint16
change_endian (uint16 integer)
{

  uint16 result;

  // mask LSB and left shift
  result |= (integer & (0x00FF)) << (sizeof (integer) * __BITS_IN_BYTE__ - __BITS_IN_BYTE__);	// left shift LSB (B when byte)
  result |= (integer & (0xFF00)) >> (__BITS_IN_BYTE__);	// right shift MSB


  return result;
}
