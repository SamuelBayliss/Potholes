
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <stdlib.h>

#define PNG_DEBUG 3
#include <png.h>

#include "image_access.h"

#include<X11/Xlib.h>

void abort_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
        abort();
}



int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;

int number_of_passes;
png_bytep * row_pointers;

void load_image(char * inputname) { 
  char header[8];

  FILE * fp = fopen(inputname, "rb");
  if (!fp) {
    abort();
  } 
  fread(header, 1, 8, fp);
  if(png_sig_cmp((png_byte *)header, 0, 8))
    abort();

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort();

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort();
  if (setjmp(png_jmpbuf(png_ptr)))
    abort();

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);
  
  width=png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);

  if (color_type == PNG_COLOR_TYPE_RGB || PNG_COLOR_TYPE_RGB_ALPHA) 
    png_set_rgb_to_gray_fixed(png_ptr, 1, -1, -1);
                      

  bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  
  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);



  if (setjmp(png_jmpbuf(png_ptr))) abort();

  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

  for (int y = 0 ; y < height ; y++) { 
    row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
  }

  png_read_image(png_ptr, row_pointers);

  fprintf(stdout, "Example has %d pixels width and %d pixels height with %d depth\n", width, height, bit_depth);

  fclose(fp);


  




}

void display_image() { 
   Display *d;
   int s;
   Window w;
   XEvent e;
 
                        /* open connection with the server */
   d=XOpenDisplay(NULL);
   if(d==NULL) {
     printf("Cannot open display\n");
     exit(1);
   }
   s=DefaultScreen(d);
 





   Visual * vis;
   vis = DefaultVisual(d,s);

   char * dImage = (char *)malloc(sizeof(png_byte) * height);
  
   XImage * image = XCreateImage(d,vis, bit_depth, ZPixmap, 0, dImage, width, height, 16, 0);

  

   //  XCreatePixmap(d, DefaultRootWindow(display),width, height, depth);
   //gc = XCreateGC(display, pixmap, 0, &gcv);

   //XPutImage(display, pixmap, gc, image, 0,0,0,0,width, height);
   
   /* create window */
   w=XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1,
                         BlackPixel(d, s), WhitePixel(d, s));



   // Process Window Close Event through event handler so XNextEvent does Not fail
        Atom delWindow = XInternAtom( d, "WM_DELETE_WINDOW", 0 );
        XSetWMProtocols(d , w, &delWindow, 1);
 
                        /* select kind of events we are interested in */
   XSelectInput(d, w, ExposureMask | KeyPressMask);
 
                        /* map (show) the window */
   XMapWindow(d, w);
 
                        /* event loop */
   while(1) {
     XNextEvent(d, &e);
                        /* draw or redraw the window */
     if(e.type==Expose) {
       XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
     }
                        /* exit on key press */
     if(e.type==KeyPress)
       break;
 
     // Handle Windows Close Event
     if(e.type==ClientMessage)
        break;
   }
 
                        /* destroy our window */
   XDestroyWindow(d, w);
 
                        /* close connection to server */
   XCloseDisplay(d);
 

}

void save_image(char * outputname){

}
