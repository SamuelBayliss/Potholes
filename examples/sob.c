
void  sobel(int Out[94][62], int In[96][64], int Kernel[3][3] );

#include "image_access.h"
#include <cassert>

int main(int argv, char ** argc) {
  int Out[94][62];//5985;                                                                                                                                   
  int In[96][64]; //6144;                                                                                                                                   
  int Kernel[3][3];  

  assert(argv == 3);

  load_image(argc[1]); // load a png file from arg
  display_image();
  sobel(Out, In, Kernel);

  save_image(argc[2]); // save a png file

}

void  sobel(int Out[94][62], int In[96][64], int Kernel[3][3] ) {
  //int Out[94][62];//5985;
  //int In[96][64]; //6144;
  //int Kernel[3][3];
  int x1, x2, x3, x4;
#pragma scop
  for ( x1 = 1 ; x1 < 94 ; x1++) {
   for (x2 = 1 ; x2 < 62 ; x2++)  {
    for (x3 = 0 ; x3 <= 2 ; x3++)  {
     for (x4 = 0 ; x4 <= 2 ; x4++)  {
    Out[x1 -1 ][x2 -1 ] = Out[x1-1][x2-1] + In[x1 + x3 -1][x2 + x4 -1] * Kernel[x3][x4];
     }
    }
   }
  }
#pragma endscop
}

