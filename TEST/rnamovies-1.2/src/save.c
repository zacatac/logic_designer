#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <forms.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <libgen.h>
#include <gd.h>

#include "dberror.h"
#include "viewforms.h"
#include "viewglobal.h"

/* prototypes */
void grabPixels(GLvoid *buffer, unsigned int width, unsigned int height);
void writeGraphics(int format, gdImagePtr im, int frame, unsigned char *buffer, int xsize, int ysize);
void writeJPG(gdImagePtr im, int frame, unsigned char *buffer, int xsize, int ysize);

/* external prototypes */
/* no need for glcanvas.h */
XVisualInfo *fl_get_glcanvas_xvisualinfo(FL_OBJECT * ob);

void
save_graphics_file(int format, int frame_from, int frame_to) {

    gdImagePtr im;
    XVisualInfo *vi;
    Pixmap pmap;
    GLXPixmap glxpmap;
    Display *dpy;
    GLXContext glxctx;
    GLXDrawable glxdrw;
    int xsize, ysize, i;
    unsigned char *buffer;
    static int flag = FALSE;
    GLuint dl = 0;
    GLuint olddl = 0;

    /* if there are no structures stop */
    if(to_main.curr_frame == NO_FR) {
	set_comment(to_main.fd_main->comment, FL_RED, "no frame");
	return;
    } 

    /* collect some info needed */
    xsize = fl_get_counter_value(to_main.fd_save_as->xsize);
    ysize = fl_get_counter_value(to_main.fd_save_as->ysize);

    glxctx = glXGetCurrentContext();
    glxdrw = glXGetCurrentDrawable();
    dpy = fl_get_display();
    vi = (XVisualInfo *)fl_get_glcanvas_xvisualinfo(to_main.glcanvas);

    /* create glxpixmap */
    pmap = XCreatePixmap(dpy, RootWindow(dpy, vi->screen),
			 xsize, ysize, vi->depth);
    glxpmap = glXCreateGLXPixmap(dpy, vi, pmap);
    /* redirect gl to pixmap */
    glXMakeCurrent(dpy, glxpmap, glxctx);

    /* reset gl */
    gl_resize(xsize,ysize);
    gl_initialize();

    if(flag==FALSE) {
	dl = glGenLists(256);
	flag = TRUE;
    }
    olddl = glc_SetFontDL(dl);
	
	if(xsize>200)
	    glc_SetFont(font.lstyle, font.lsize);

    /** allocate gd buffer **/
    im = gdImageCreate(xsize, ysize);
    /** allocate bytebuffer **/
    buffer = (unsigned char *)malloc(xsize * ysize * 3 * sizeof(unsigned char));
    if(buffer == NULL) {
	fprintf(stderr, "failed to malloc...\n");
	exit(EXIT_FAILURE);
    }

    /* write frames */
    for (i=frame_from; i<=frame_to; i++) {
	/* draw structure */
	glClear(GL_COLOR_BUFFER_BIT);
	to_main.curr_frame=i; 	/*added by Thomas on 04/07/2002 for generating the comments in pictures */
	compile_struc(to_main.movie[i], GL_COMPILE_AND_EXECUTE);

	grabPixels(buffer, xsize, ysize);
#if AUTOMATIC_SAVE
	writeGraphics(format, im, frame_to-i, buffer, xsize, ysize);
#else
	writeGraphics(format, im, i, buffer, xsize, ysize);
#endif
    }

    /* redirect gl to window */
    glXMakeCurrent(dpy, glxdrw, glxctx);
    glc_SetFontDL(olddl);

    /* free buffers */
    free(buffer);
    gdImageDestroy(im);
    glXDestroyGLXPixmap(dpy, glxpmap);
    XFreePixmap(dpy, pmap);
}


void
grabPixels(GLvoid *buffer, unsigned int width, unsigned int height)
{
    GLint swapbytes, lsbfirst, rowlength;
    GLint skiprows, skippixels, alignment;

    /* Save current modes. */
    glGetIntegerv(GL_PACK_SWAP_BYTES, &swapbytes);
    glGetIntegerv(GL_PACK_LSB_FIRST, &lsbfirst);
    glGetIntegerv(GL_PACK_ROW_LENGTH, &rowlength);
    glGetIntegerv(GL_PACK_SKIP_ROWS, &skiprows);
    glGetIntegerv(GL_PACK_SKIP_PIXELS, &skippixels);
    glGetIntegerv(GL_PACK_ALIGNMENT, &alignment);
    /* Little endian machines (DEC Alpha for example) could
       benefit from setting GL_PACK_LSB_FIRST to GL_TRUE
       instead of GL_FALSE, but this would require changing the
       generated bitmaps too. */
    glPixelStorei(GL_PACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei(GL_PACK_LSB_FIRST, GL_FALSE);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    /* Actually read the pixels. */
    glReadPixels(0, 0, width, height, GL_RGB,
		 GL_UNSIGNED_BYTE, (GLvoid *) buffer);

    /* Restore saved modes. */
    glPixelStorei(GL_PACK_SWAP_BYTES, swapbytes);
    glPixelStorei(GL_PACK_LSB_FIRST, lsbfirst);
    glPixelStorei(GL_PACK_ROW_LENGTH, rowlength);
    glPixelStorei(GL_PACK_SKIP_ROWS, skiprows);
    glPixelStorei(GL_PACK_SKIP_PIXELS, skippixels);
    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
}

void
writeGraphics(int format, gdImagePtr image, int frame,
         unsigned char *buffer, int xsize, int ysize) {

    int i, x, y, col;
    char *filename;
    FILE *fptr;

#if AUTOMATIC_SAVE
    if(format == FORMAT_JPG) {
	filename = generate_save_filename_alain(frame+1, "jpg");
    }
    else {
	filename = generate_save_filename_alain(frame+1, "png");
    }
#else
    if(format == FORMAT_JPG) {
      filename = generate_save_filename(frame, "jpg");
    }
    else {
      filename = generate_save_filename(frame, "png");
    }
#endif

    fprintf(stderr,"%s",filename);

    fptr = fopen( filename, "w" );
    if(!fptr) {
      set_comment(to_main.fd_main->comment, FL_RED,
                  "failed to open file: %s",basename(filename));
      fprintf(stderr,"failed to open file: %s",basename(filename));
	return;
    }

    set_comment(to_main.fd_main->comment, FL_BLUE, "writing file: %s ...",basename(filename));
      
    for (y=ysize-1; y>=0; y--) {
	for (x=0; x<xsize; x++) {
	    i = (y*xsize + x) * 3;
	    /* Try to allocate color directly */
	    col = gdImageColorExact(image, buffer[i], buffer[i+1], buffer[i+2]);
	    /* If color isn't already present... */
	    if (col == (-1)) {
		/* Second best: try to allocate it directly. */
		col = gdImageColorAllocate(image, buffer[i], buffer[i+1], buffer[i+2]);
		/* Out of colors, so find the closest color instead. */
		col = gdImageColorClosest(image, buffer[i], buffer[i+1], buffer[i+2]);
	    }	
	    gdImageSetPixel(image, x, ysize-y, col);
	}
    }

    if(format == FORMAT_PNG) {
      gdImagePng(image, fptr);
    }
    else if(format == FORMAT_JPG) {
      gdImageJpeg(image, fptr, -1);
    }

    fclose(fptr);
    set_comment(to_main.fd_main->comment, FL_BLUE, "writing file: %s ... done",basename(filename));
}

/*************************************************/
/*                 JPEG                          */
/*************************************************/

void
save_jpeg_file(int frame_from, int frame_to) {

    gdImagePtr im;
    XVisualInfo *vi;
    Pixmap pmap;
    GLXPixmap glxpmap;
    Display *dpy;
    GLXContext glxctx;
    GLXDrawable glxdrw;
    int xsize, ysize, i;
    unsigned char *buffer;
    static int flag = FALSE;
    GLuint dl = 0;
    GLuint olddl = 0;

    /* if there are no structures stop */
    if(to_main.curr_frame == NO_FR) {
	set_comment(to_main.fd_main->comment, FL_RED, "no frame");
	return;
    } 

    /* collect some info needed */
    xsize = fl_get_counter_value(to_main.fd_save_as->xsize);
    ysize = fl_get_counter_value(to_main.fd_save_as->ysize);

    glxctx = glXGetCurrentContext();
    glxdrw = glXGetCurrentDrawable();
    dpy = fl_get_display();
    vi = (XVisualInfo *)fl_get_glcanvas_xvisualinfo(to_main.glcanvas);

    /* create glxpixmap */
    pmap = XCreatePixmap(dpy, RootWindow(dpy, vi->screen),
			 xsize, ysize, vi->depth);
    glxpmap = glXCreateGLXPixmap(dpy, vi, pmap);
    /* redirect gl to pixmap */
    glXMakeCurrent(dpy, glxpmap, glxctx);

    /* reset gl */	

    /* added by Thomas on 02/07/2002 */
    if( g_xsize != -1)	/* the value are initialized */
      {
        xsize=g_xsize;
        ysize=g_ysize;
      }
    
    gl_resize(xsize,ysize);
    gl_initialize();

    if(flag==FALSE) {
	dl = glGenLists(256);
	flag = TRUE;
    }
    olddl = glc_SetFontDL(dl);
	
	if(xsize>200)
	    glc_SetFont(font.lstyle, font.lsize);

    /** allocate png buffer **/
    im = gdImageCreate(xsize, ysize);
    /** allocate bytebuffer **/
    buffer = (unsigned char *)malloc(xsize * ysize * 3 * sizeof(unsigned char));
    if(buffer == NULL) {
	fprintf(stderr, "failed to malloc...\n");
	exit(EXIT_FAILURE);
    }

    /* write frames */
    for (i=frame_from; i<=frame_to; i++) {
	/* draw structure */
	glClear(GL_COLOR_BUFFER_BIT);
	to_main.curr_frame=i;	/*added by Thomas on 04/07/2002 for generating the comments in pictures */
	compile_struc(to_main.movie[i], GL_COMPILE_AND_EXECUTE);

	grabPixels(buffer, xsize, ysize);
	/*writeJPG(im, i, buffer, xsize, ysize);*/
	writeJPG(im,frame_to-i, buffer, xsize, ysize); /*alain modif*/
    }

    /* redirect gl to window */
    glXMakeCurrent(dpy, glxdrw, glxctx);
    glc_SetFontDL(olddl);

    /* free buffers */
    free(buffer);
    gdImageDestroy(im);
    glXDestroyGLXPixmap(dpy, glxpmap);
    XFreePixmap(dpy, pmap);
}


void
writeJPG(gdImagePtr image, int frame, unsigned char *buffer, int xsize, int ysize) {

    int i, x, y, col;
    char *filename;
    FILE *fptr;
    filename = generate_save_filename_alain(frame+1, "jpg");
	
    fptr = fopen( filename, "w" );
    if(!fptr) {
	/*set_comment(to_main.fd_main->comment, FL_RED,
	  "failed to open file: %s",basename(filename));*/
	printf("problem %i\n",frame);
	return;
    }
      
    for (y=ysize-1; y>=0; y--) {
	for (x=0; x<xsize; x++) {
	    i = (y*xsize + x) * 3;
	    /* Try to allocate color directly */
	    col = gdImageColorExact(image, buffer[i], buffer[i+1], buffer[i+2]);
	    /* If color isn't already present... */
	    if (col == (-1)) {
		/* Second best: try to allocate it directly. */
		col = gdImageColorAllocate(image, buffer[i], buffer[i+1], buffer[i+2]);
		/* Out of colors, so find the closest color instead. */
		col = gdImageColorClosest(image, buffer[i], buffer[i+1], buffer[i+2]);
	    }	
	    gdImageSetPixel(image, x, ysize-y, col);
	}
    }

    gdImageJpeg(image, fptr, g_quality);

    fclose(fptr);

}

/*
  void writePPM() {
  if (f) {
      
  int i, x, y;
  GLubyte *ptr;
  ptr = (GLubyte *) buffer;
  fprintf(f,"P6\n");
  fprintf(f,"# ppm-file created by RNAmovies\n");
  fprintf(f,"%i %i\n", xsize,ysize);
  fprintf(f,"255\n");
  fclose(f);
  f = fopen( filename, "ab" );
  for (y=ysize-1; y>=0; y--) {
  for (x=0; x<xsize; x++) {
  i = (y*xsize + x) * 4;
  fputc(ptr[i], f);  
  fputc(ptr[i+1], f);
  fputc(ptr[i+2], f);
      
  }
  }
  fclose(f);
  }
  }
*/
