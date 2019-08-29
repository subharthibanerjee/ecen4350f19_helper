/******************************************************************************

                            Take Home Quiz 1.
                    Solution: please check all the functions
            There are couple good solutions provided by the students
                    I tried to provide easy solution here. Ask questions!
                        

*******************************************************************************/

#include <stdio.h>


#define __BITS_IN_BYTE__ 8

// Try diffent typedefs and check the outputs
typedef unsigned long int si;           // 8 bytes
// typedef unsigned short int si;       // 2 bytes
// typedef unsigned int si;             // 4bytes


int
main ()
{
  si n = 0;
  printf ("Hello World\n");
  int len = 0;

  printf ("scanned number: \t");
  
  // TODO: better scan
  
  scanf ("%d", &n);
  printf ("printing bits  of %d\n", n);
  print_bits (n, sizeof (n) * __BITS_IN_BYTE__);


  printf ("naive implementation\n");
  print_bits_naive (n);
  
  printf("print bits reverse (lsb at the right):  \n");
  print_bits_reverse (n, sizeof (n) * __BITS_IN_BYTE__);
  printf("\nall prints complete");
  return 0;
}


void
print_bits (si n, int len)
{

  for (int i = 0; i < len; i++)
    {
      printf ("%d", (n & (1 << i)) >> i);   // this: n & (1 << i) extracts i_th_bit
    }
  printf ("\n");
}

// actual representation for bits
void
print_bits_reverse (si n, int len)
{
   
  // simple reverse loop
  for (int i = len - 1 ; i >= 0; i--)
    {
      printf ("%d", (n & (1 << i)) >> i);
    }
  printf ("\n");
}


// Only one student proposed this solution and it works. Good job!
// Note: msb on the left

void
print_bits_naive (si n)
{
  int rem = 0;
  while (n != 1)
    {
      rem = n % 2;
      printf ("%d", rem);
      n = n / 2;

    }
  printf ("\n");
}
