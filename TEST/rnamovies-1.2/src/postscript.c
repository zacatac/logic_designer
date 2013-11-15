#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <forms.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include "dberror.h"
#include "viewforms.h"
#include "viewglobal.h"

/* OpenGL's GL_3D_COLOR feedback vertex format. */
typedef struct _Feedback3Dcolor {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat red;
  GLfloat green;
  GLfloat blue;
  GLfloat alpha;
} Feedback3Dcolor;


static const char *fontstyle[] = {
  "/Helvetica",
  "/Helvetica-Bold",
  "/Helvetica-Oblique",
  "/Helvetica-BoldOblique",
  "/Courier",
  "/Courier-Bold",
  "/Courier-Oblique",
  "/Courier-BoldOblique",
  "/Times-Roman",
  "/Times-Bold",
  "/Times-Italic",
  "/Times-BoldItalic",
  "/Charter",
  "/Charter-Bold",
  "/Charter-Italic",
  "/Symbol"
};

static const int papers[][2] = {
  {0, 0},      /* Nuttin' */
  {595, 842},  /* A4      */
  {842, 1191}, /* A3      */
  {612, 792},  /* Letter  */
  {612, 1008}  /* Legal   */
};

static const char *papernames[] = {
  NULL,
  "A4",
  "A3",
  "Letter",
  "Legal"
};
static const char *paperorientation[] = {
  NULL,
  "Portrait",
  "Landscape",
};

void print3DcolorVertex(GLint size, GLint * count, GLfloat * buffer);
void printBuffer(GLint size, GLfloat * buffer);
void spewUnsortedFeedback(FILE * file, GLint size, GLfloat * buffer);
void spewSortedFeedback(FILE * file, GLint size, GLfloat * buffer);
GLfloat *spewPrimitiveEPS(FILE * file, GLfloat * loc);
void spewHeaderPS(FILE * file, int pages, int pchoice, int ochoice);
void spewWireFrameEPS(FILE * file, int doSort, GLint size,
		      GLfloat * buffer, char *creator, int bg);

void
printPS(int frame_from, int frame_to) {

  static GLfloat *feedbackBuffer = NULL;
  static GLint returned = -1;
  static int size = 500;
  int fr_i, ordinal;
  char *sys_cmd = NULL;
  FILE *file;
  char *tmpname;
  GLint vp[4];
  GLfloat clearColor[4];
  int pchoice, ochoice, xtrans, ytrans;
  int ret;

  if(to_main.curr_frame == NO_FR) {
    set_comment(to_main.fd_main->comment, FL_RED, "no frame");
    return;
  } 

  glLoadIdentity();
  glTranslated(to_main.zoom*to_main.x_displacement,
	       to_main.zoom*to_main.y_displacement, 0);
  glRotated(to_main.rotation, 0.0, 0.0, 1.0);

  /* Get the Clear Color */
  glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);

  /* compute translation to center of paper */
  glGetIntegerv(GL_VIEWPORT, vp);
  pchoice = fl_get_choice(to_main.fd_print->papersize);
  ochoice = fl_get_choice(to_main.fd_print->paperorient);
  switch(ochoice) {
  case 1: /* Portrait */
    xtrans = (papers[pchoice][0] - vp[2] - vp[0])/2;
    ytrans = (papers[pchoice][1] - vp[3] - vp[1])/2;
    break;
  case 2: /* Landscape */
    xtrans = (papers[pchoice][0] - vp[3] - vp[1])/2 + (vp[3] - vp[1]);
    ytrans = (papers[pchoice][1] - vp[2] - vp[0])/2;
    break;
  default:
    fprintf(stderr, "The impossible happend!: Paper Orientation Choice Illegal\n");
    exit(EXIT_FAILURE);
  }

  /* open a temporary file */
  file = fopen(tmpname = tempnam(NULL, "rnm_"), "w");
  if (!file) {
    set_comment(to_main.fd_main->comment, FL_RED,
		"Could not open: %s",tmpname);
    return;
  }

  set_comment(to_main.fd_main->comment, FL_BLUE,
	      "generating PS header");

  spewHeaderPS(file, frame_to - frame_from + 1, pchoice, ochoice);


  /* generate all frames */
  for (fr_i=frame_from, ordinal=1; fr_i<=frame_to; fr_i++, ordinal++) {
    set_comment(to_main.fd_main->comment, FL_BLUE,
		"printing frame: %d",fr_i);
    /* allocate feedback buffer */
    /* this is somewhat involved */
    do {
      /* failed last time? */
      if(returned < 0) {
	size *= 2;
	feedbackBuffer = realloc(feedbackBuffer, size * sizeof(GLfloat));
	if(feedbackBuffer == NULL)
	  dbError(DB_TRUE, "failed to realloc in outputEPS");
      }
      glFeedbackBuffer(size, GL_3D_COLOR, feedbackBuffer);
      glRenderMode(GL_FEEDBACK);
      glClear(GL_COLOR_BUFFER_BIT);
      /* pretend to draw */
      compile_struc(to_main.movie[fr_i], GL_COMPILE_AND_EXECUTE);

      returned = glRenderMode(GL_RENDER);
    } while (returned < 0);

    fprintf(file, "%%%%Page: %d %d\n",fr_i, ordinal);
    fprintf(file, "%d %d translate\n", xtrans, ytrans);
    switch(ochoice) {
    case 1:
      break;
    case 2:
      fputs("90 rotate\n", file);
      break;
    default:
      fprintf(stderr, "The impossible happend!: Paper Orientation Choice Illegal\n");
      exit(EXIT_FAILURE);
    }

    fputs("gsave\n",file);

    /* if we want the background cleared */
    if(fl_get_button(to_main.fd_print->background)) {
      /* Clear the background like OpenGL had it. */
      fprintf(file, "%g %g %g setrgbcolor\n",
	      clearColor[0], clearColor[1], clearColor[2]);
      fprintf(file, "%d %d %d %d rectfill\n\n",
	      vp[0], vp[1], vp[2], vp[3]);
    }

    /* output PS for frame */
    spewUnsortedFeedback(file, returned, feedbackBuffer);

    fputs("grestore\n",file);
    /* print a page */
    fputs("\nshowpage\n", file);

    set_comment(to_main.fd_main->comment, FL_BLUE,
		"printing frame: %d ... done", fr_i);
  }
  /* finished printing to file*/
  fclose(file);

  /* generate the print command */
      sys_cmd = (char *)malloc((strlen(fl_get_input(to_main.fd_print->print_cmd)) + strlen(tmpname))
			       * sizeof(char));
      if(sys_cmd == NULL) {
	fprintf(stderr, "failed to malloc\n");
	exit(EXIT_FAILURE);
      }
      sprintf(sys_cmd, fl_get_input(to_main.fd_print->print_cmd), tmpname);

      /* do the actual printing */
      ret = system(sys_cmd);
      if(ret != 0)
	fl_show_alert("Command failed!", sys_cmd, "", TRUE);
      /* remove the tmpfile */
      ret = unlink(tmpname);
      if(ret == -1)
	fl_show_alert("Unable to remove tmp file!", tmpname, "", TRUE);
      free(sys_cmd);
}

void
outputEPS(int frame_from, int frame_to, int bg) {

  static GLfloat *feedbackBuffer = NULL;
  static GLint returned = -1;
  static int size = ALLOC_INC;
  int fr_i;
  char *filename;
  FILE *file;

  if(to_main.curr_frame == NO_FR) {
      set_comment(to_main.fd_main->comment, FL_RED, "no frame");
      return;
  } 
  
  glLoadIdentity();
  glTranslated(to_main.zoom*to_main.x_displacement,
	       to_main.zoom*to_main.y_displacement, 0);
  glRotated(to_main.rotation, 0.0, 0.0, 1.0);
  
  for (fr_i=frame_from; fr_i<=frame_to; fr_i++) {
      
      do {
	  /* failed last time? */
	  if(returned < 0) {
	      size *= 2;
	      feedbackBuffer = realloc(feedbackBuffer, size * sizeof(GLfloat));
	      if(feedbackBuffer == NULL)
		  dbError(DB_TRUE, "failed to realloc in outputEPS");
	  }
	  glFeedbackBuffer(size, GL_3D_COLOR, feedbackBuffer);
	  glRenderMode(GL_FEEDBACK);
	  glClear(GL_COLOR_BUFFER_BIT);
	  
	  compile_struc(to_main.movie[fr_i], GL_COMPILE_AND_EXECUTE);
 
	  returned = glRenderMode(GL_RENDER);
      } while (returned < 0);
  
      filename = generate_save_filename(fr_i, "eps");
      

      printf ("%s",filename);
      file = fopen(filename, "w");
      if (file) {
	  set_comment(to_main.fd_main->comment, FL_BLUE,
		      "writing file: %s",basename(filename));
	  spewWireFrameEPS(file, 0, returned, feedbackBuffer,
			   "RNA Movies", bg);
	  set_comment(to_main.fd_main->comment, FL_BLUE,
		      "writing file: %s ... done",basename(filename));
      } 
      else {
	  set_comment(to_main.fd_main->comment, FL_RED,
		      "Could not open: %s",basename(filename));
	  return;
	  
      }
  }
}
/*============ Alain modif ==================*/
void
outputEPS_alain(int frame_from, int frame_to, int bg) {
    static GLfloat *feedbackBuffer = NULL;
    static GLint returned = -1;
    static int size = 500;
    int fr_i;
    char *filename;
    FILE *file;
    
/* 21/05/02 added for debug */
    char *filename2;
    int lg, xsize, ysize;
    static int flag = FALSE;
    GLuint dl = 0;
    GLuint olddl = 0;

    /* make in remark for rnamoviesweb */
    /*
      if(to_main.curr_frame == NO_FR) {
      set_comment(to_main.fd_main->comment, FL_RED, "no frame");
      return;
      }
    */

  glLoadIdentity();  
  glTranslated(to_main.zoom*to_main.x_displacement,
	       to_main.zoom*to_main.y_displacement, 0);
  glRotated(to_main.rotation, 0.0, 0.0, 1.0);
   /* reset gl */
		/* added by alain on 19/07/2002 */
  if( g_xsize != -1)	/* the value are initialized */
  {
      xsize=g_xsize;
      ysize=g_ysize;
  }
  else{
      xsize = fl_get_counter_value(to_main.fd_save_as->xsize);
      ysize = fl_get_counter_value(to_main.fd_save_as->ysize);
  }
  
  
  gl_resize(xsize,ysize);
  gl_initialize();
    
    /* setup fonts */
    if(flag==FALSE) {
	dl = glGenLists(256);
	flag = TRUE;
    }
    
    olddl = glc_SetFontDL(dl);
    
    
   if(xsize>200){
	glc_SetFont(font.lstyle, font.lsize);
   }
    

  for (fr_i=frame_from; fr_i<=frame_to; fr_i++) {
      
      do {
	  /* failed last time? */
	  if(returned < 0) {
	      size = xsize*ysize;
	      feedbackBuffer = realloc(feedbackBuffer, size * sizeof(GLfloat));
	      if(feedbackBuffer == NULL)
		  dbError(DB_TRUE, "failed to realloc in outputEPS");
	  }
	  glFeedbackBuffer(size, GL_3D_COLOR, feedbackBuffer);
	  glRenderMode(GL_FEEDBACK);
	  glClear(GL_COLOR_BUFFER_BIT);

	  compile_struc(to_main.movie[fr_i], GL_COMPILE_AND_EXECUTE);
	  
	  returned = glRenderMode(GL_RENDER);
      } while (returned < 0);
      
      /*filename = generate_save_filename_alain(fr_i, "eps");*/
      filename = generate_save_filename_alain(frame_to-fr_i+1, "eps"); /*change 30.04.2002*/

      /* 05/21/02 added for debug */
	filename2=strdup(filename);
	

	lg=strlen(filename2);
	if( filename2[lg-1] != 's'){
		filename2[lg-1] = '\0';
	}
      file = fopen(filename2, "w");
      if (file) {   /* make in remark for rnamoviesweb */
	  /*set_comment(to_main.fd_main->comment, FL_BLUE,
	    "writing file: %s",basename(filename));*/
	  spewWireFrameEPS(file, 0, returned, feedbackBuffer,
			   "RNA Movies", bg);
	  fclose(file);
	  /*set_comment(to_main.fd_main->comment, FL_BLUE,
	    "writing file: %s ... done",basename(filename));*/
      } 
      else {
	  /*set_comment(to_main.fd_main->comment, FL_RED,
	    "Could not open: %s",basename(filename));*/
	  return;
	  
      }
  }
}
/*=======================================*/


/* Write contents of one vertex to stdout. */
void
print3DcolorVertex(GLint size, GLint * count,
  GLfloat * buffer)
{
  int i;

  printf("  ");
  for (i = 0; i < 7; i++) {
    printf("%4.2f ", buffer[size - (*count)]);
    *count = *count - 1;
  }
  printf("\n");
}

void
printBuffer(GLint size, GLfloat * buffer)
{
  GLint count;
  int token, nvertices;

  count = size;
  while (count) {
    token = buffer[size - count];
    count--;
    switch (token) {
    case GL_PASS_THROUGH_TOKEN:
      printf("GL_PASS_THROUGH_TOKEN\n");
      printf("  %4.2f\n", buffer[size - count]);
      count--;
      break;
    case GL_POINT_TOKEN:
      printf("GL_POINT_TOKEN\n");
      print3DcolorVertex(size, &count, buffer);
      break;
    case GL_LINE_TOKEN:
      printf("GL_LINE_TOKEN\n");
      print3DcolorVertex(size, &count, buffer);
      print3DcolorVertex(size, &count, buffer);
      break;
    case GL_LINE_RESET_TOKEN:
      printf("GL_LINE_RESET_TOKEN\n");
      print3DcolorVertex(size, &count, buffer);
      print3DcolorVertex(size, &count, buffer);
      break;
    case GL_POLYGON_TOKEN:
      printf("GL_POLYGON_TOKEN\n");
      nvertices = buffer[size - count];
      count--;
      for (; nvertices > 0; nvertices--) {
        print3DcolorVertex(size, &count, buffer);
      }
    }
  }
}


static char *gouraudtriangleEPS[] =
{
  "/bd{bind def}bind def /triangle { aload pop   setrgbcolor  aload pop 5 3",
  "roll 4 2 roll 3 2 roll exch moveto lineto lineto closepath fill } bd",
  "/computediff1 { 2 copy sub abs threshold ge {pop pop pop true} { exch 2",
  "index sub abs threshold ge { pop pop true} { sub abs threshold ge } ifelse",
  "} ifelse } bd /computediff3 { 3 copy 0 get 3 1 roll 0 get 3 1 roll 0 get",
  "computediff1 {true} { 3 copy 1 get 3 1 roll 1 get 3 1 roll 1 get",
  "computediff1 {true} { 3 copy 2 get 3 1 roll  2 get 3 1 roll 2 get",
  "computediff1 } ifelse } ifelse } bd /middlecolor { aload pop 4 -1 roll",
  "aload pop 4 -1 roll add 2 div 5 1 roll 3 -1 roll add 2 div 3 1 roll add 2",
  "div 3 1 roll exch 3 array astore } bd /gouraudtriangle { computediff3 { 4",
  "-1 roll aload 7 1 roll 6 -1 roll pop 3 -1 roll pop add 2 div 3 1 roll add",
  "2 div exch 3 -1 roll aload 7 1 roll exch pop 4 -1 roll pop add 2 div 3 1",
  "roll add 2 div exch 3 -1 roll aload 7 1 roll pop 3 -1 roll pop add 2 div 3",
  "1 roll add 2 div exch 7 3 roll 10 -3 roll dup 3 index middlecolor 4 1 roll",
  "2 copy middlecolor 4 1 roll 3 copy pop middlecolor 4 1 roll 13 -1 roll",
  "aload pop 17 index 6 index 15 index 19 index 6 index 17 index 6 array",
  "astore 10 index 10 index 14 index gouraudtriangle 17 index 5 index 17",
  "index 19 index 5 index 19 index 6 array astore 10 index 9 index 13 index",
  "gouraudtriangle 13 index 16 index 5 index 15 index 18 index 5 index 6",
  "array astore 12 index 12 index 9 index gouraudtriangle 17 index 16 index",
  "15 index 19 index 18 index 17 index 6 array astore 10 index 12 index 14",
  "index gouraudtriangle 18 {pop} repeat } { aload pop 5 3 roll aload pop 7 3",
  "roll aload pop 9 3 roll 4 index 6 index 4 index add add 3 div 10 1 roll 7",
  "index 5 index 3 index add add 3 div 10 1 roll 6 index 4 index 2 index add",
  "add 3 div 10 1 roll 9 {pop} repeat 3 array astore triangle } ifelse } bd",
  NULL
};

void
spewUnsortedFeedback(FILE * file, GLint size, GLfloat * buffer)
{
  GLfloat *loc, *end;

  loc = buffer;
  end = buffer + size;
  while (loc < end) {
    loc = spewPrimitiveEPS(file, loc);
  }
}

typedef struct _DepthIndex {
  GLfloat *ptr;
  GLfloat depth;
} DepthIndex;

static int
compare(const void *a, const void *b)
{
  const DepthIndex *p1 = (const DepthIndex *) a;
  const DepthIndex *p2 = (const DepthIndex *) b;
  GLfloat diff = p2->depth - p1->depth;

  if (diff > 0.0) {
    return 1;
  } else if (diff < 0.0) {
    return -1;
  } else {
    return 0;
  }
}


void
spewSortedFeedback(FILE * file, GLint size, GLfloat * buffer)
{
  int token = 0;
  GLfloat *loc, *end;
  Feedback3Dcolor *vertex;
  GLfloat depthSum;
  int nprimitives, item;
  DepthIndex *prims;
  int nvertices, i;

  end = buffer + size;

  /* Count how many primitives there are. */
  nprimitives = 0;
  loc = buffer;
  while (loc < end) {
    token = *loc;
    loc++;
    switch (token) {
    case GL_LINE_TOKEN:
    case GL_LINE_RESET_TOKEN:
      loc += 14;
      nprimitives++;
      break;
    case GL_POLYGON_TOKEN:
      nvertices = *loc;
      loc++;
      loc += (7 * nvertices);
      nprimitives++;
      break;
    case GL_POINT_TOKEN:
      loc += 7;
      nprimitives++;
      break;
    case GL_BITMAP_TOKEN:
      loc += 7;
      nprimitives++;
      break;
    case GL_PASS_THROUGH_TOKEN:
      loc += 5;
      nprimitives++;
      break;
    default:
      /* XXX Left as an excersie to the reader. */
      printf("Incomplete implementation.  Unexpected token (%d).\n",
	     token);
      exit(1);
    }
  }

  /* Allocate an array of pointers that will point back at
     primitives in the feedback buffer.  There will be one
     entry per primitive.  This array is also where we keep the
     primitive's average depth.  There is one entry per
     primitive  in the feedback buffer. */
  prims = (DepthIndex *) malloc(sizeof(DepthIndex) * nprimitives);

  item = 0;
  loc = buffer;
  while (loc < end) {
    prims[item].ptr = loc;  /* Save this primitive's location. */
    token = *loc;
    loc++;
    switch (token) {
    case GL_LINE_TOKEN:
    case GL_LINE_RESET_TOKEN:
      vertex = (Feedback3Dcolor *) loc;
      depthSum = vertex[0].z + vertex[1].z;
      prims[item].depth = depthSum / 2.0;
      loc += 14;
      break;
    case GL_POLYGON_TOKEN:
      nvertices = *loc;
      loc++;
      vertex = (Feedback3Dcolor *) loc;
      depthSum = vertex[0].z;
      for (i = 1; i < nvertices; i++) {
        depthSum += vertex[i].z;
      }
      prims[item].depth = depthSum / nvertices;
      loc += (7 * nvertices);
      break;
    case GL_POINT_TOKEN:
      vertex = (Feedback3Dcolor *) loc;
      prims[item].depth = vertex[0].z;
      loc += 7;
      break;
    case GL_PASS_THROUGH_TOKEN:
      loc += 5;
      break;
    case GL_BITMAP_TOKEN:
      vertex = (Feedback3Dcolor *) loc;
      prims[item].depth = vertex[0].z;
      loc += 7;
      break;
    default:
      /* XXX Left as an excerise to the reader. */
      assert(1);
    }
    item++;
  }
  assert(item == nprimitives);

  /* Sort the primitives back to front. */
  qsort(prims, nprimitives, sizeof(DepthIndex), compare);

  /* Understand that sorting by a primitives average depth
     doesn't allow us to disambiguate some cases like self
     intersecting polygons.  Handling these cases would require
     breaking up the primitives.  That's too involved for this
     example.  Sorting by depth is good enough for lots of
     applications. */

  /* Emit the Encapsulated PostScript for the primitives in
     back to front order. */
  for (item = 0; item < nprimitives; item++) {
    (void) spewPrimitiveEPS(file, prims[item].ptr);
  }

  free(prims);
}

#define EPS_GOURAUD_THRESHOLD 0.1  /* Lower for better (slower) 

                                      smooth shading. */

GLfloat *
spewPrimitiveEPS(FILE * file, GLfloat * loc)
{
  int token;
  int nvertices, i;
  GLfloat red, green, blue;
  int smooth;
  GLfloat dx, dy, dr, dg, db, absR, absG, absB, colormax;
  int steps;
  Feedback3Dcolor *vertex;
  GLfloat xstep , ystep, rstep, gstep, bstep;
  GLfloat xnext, ynext, rnext, gnext, bnext, distance;
  static GLfloat xoffset, yoffset;
  static char letter;
  GLfloat pointSize;

  xstep = ystep = rstep = gstep = bstep = 0.0F;
  xnext = ynext = rnext = gnext = bnext = distance = 0.0F;

  glGetFloatv(GL_POINT_SIZE, &pointSize);

  token = *loc;
  loc++;
  switch (token) {
  case GL_LINE_RESET_TOKEN:
  case GL_LINE_TOKEN:
    vertex = (Feedback3Dcolor *) loc;

    dr = vertex[1].red - vertex[0].red;
    dg = vertex[1].green - vertex[0].green;
    db = vertex[1].blue - vertex[0].blue;

    if (dr != 0 || dg != 0 || db != 0) {
      /* Smooth shaded line. */
      dx = vertex[1].x - vertex[0].x;
      dy = vertex[1].y - vertex[0].y;
      distance = sqrt(dx * dx + dy * dy);

      absR = fabs(dr);
      absG = fabs(dg);
      absB = fabs(db);

#define Max(a,b) (((a)>(b))?(a):(b))

#define EPS_SMOOTH_LINE_FACTOR 0.06  /* Lower for better smooth 

                                        lines. */

      colormax = Max(absR, Max(absG, absB));
      steps = Max(1.0, colormax * distance * EPS_SMOOTH_LINE_FACTOR);

      xstep = dx / steps;
      ystep = dy / steps;

      rstep = dr / steps;
      gstep = dg / steps;
      bstep = db / steps;

      xnext = vertex[0].x;
      ynext = vertex[0].y;
      rnext = vertex[0].red;
      gnext = vertex[0].green;
      bnext = vertex[0].blue;

      /* Back up half a step; we want the end points to be
         exactly the their endpoint colors. */
      xnext -= xstep / 2.0;
      ynext -= ystep / 2.0;
      rnext -= rstep / 2.0;
      gnext -= gstep / 2.0;
      bnext -= bstep / 2.0;
    } else {
      /* Single color line. */
      steps = 0;
    }

    fprintf(file, "%g %g %g setrgbcolor\n",
      vertex[0].red, vertex[0].green, vertex[0].blue);
    fprintf(file, "%g %g moveto\n", vertex[0].x, vertex[0].y);

    for (i = 0; i < steps; i++) {
      xnext += xstep;
      ynext += ystep;
      rnext += rstep;
      gnext += gstep;
      bnext += bstep;
      fprintf(file, "%g %g lineto stroke\n", xnext, ynext);
      fprintf(file, "%g %g %g setrgbcolor\n", rnext, gnext, bnext);
      fprintf(file, "%g %g moveto\n", xnext, ynext);
    }
    fprintf(file, "%g %g lineto stroke\n", vertex[1].x, vertex[1].y);

    loc += 14;          /* Each vertex element in the feedback
                           buffer is 7 GLfloats. */

    break;
  case GL_POLYGON_TOKEN:
    nvertices = *loc;
    loc++;

    vertex = (Feedback3Dcolor *) loc;

    if (nvertices > 0) {
      red = vertex[0].red;
      green = vertex[0].green;
      blue = vertex[0].blue;
      smooth = 0;
      for (i = 1; i < nvertices; i++) {
        if (red != vertex[i].red || green != vertex[i].green || blue != vertex[i].blue) {
          smooth = 1;
          break;
        }
      }
      if (smooth) {
        /* Smooth shaded polygon; varying colors at vetices. */
        int triOffset;

        /* Break polygon into "nvertices-2" triangle fans. */
        for (i = 0; i < nvertices - 2; i++) {
          triOffset = i * 7;
          fprintf(file, "[%g %g %g %g %g %g]",
            vertex[0].x, vertex[i + 1].x, vertex[i + 2].x,
            vertex[0].y, vertex[i + 1].y, vertex[i + 2].y);
          fprintf(file, " [%g %g %g] [%g %g %g] [%g %g %g] gouraudtriangle\n",
            vertex[0].red, vertex[0].green, vertex[0].blue,
            vertex[i + 1].red, vertex[i + 1].green, vertex[i + 1].blue,
            vertex[i + 2].red, vertex[i + 2].green, vertex[i + 2].blue);
        }
      } else {
        /* Flat shaded polygon; all vertex colors the same. */
        fprintf(file, "newpath\n");
        fprintf(file, "%g %g %g setrgbcolor\n", red, green, blue);

        /* Draw a filled triangle. */
        fprintf(file, "%g %g moveto\n", vertex[0].x, vertex[0].y);
        for (i = 1; i < nvertices; i++) {
          fprintf(file, "%g %g lineto\n", vertex[i].x, vertex[i].y);
        }
        fprintf(file, "closepath fill\n\n");
      }
    }
    loc += nvertices * 7;  /* Each vertex element in the
                              feedback buffer is 7 GLfloats. */
    break;
  case GL_POINT_TOKEN:
    vertex = (Feedback3Dcolor *) loc;
    fprintf(file, "%g %g %g setrgbcolor\n", vertex[0].red, vertex[0].green, vertex[0].blue);
    fprintf(file, "%g %g %g 0 360 arc fill\n\n", vertex[0].x, vertex[0].y, pointSize);
    loc += 7;           /* Each vertex element in the feedback
                           buffer is 7 GLfloats. */
    break;
  case GL_PASS_THROUGH_TOKEN:
    letter = (char) (*loc);
    xoffset = *(loc + 2);
    yoffset =  *(loc + 4);
    loc += 5;
    break;
  case GL_BITMAP_TOKEN:
    vertex = (Feedback3Dcolor *) loc;
    fprintf(file, "%g %g %g setrgbcolor\n", vertex[0].red, vertex[0].green, vertex[0].blue);
    fprintf(file, "%g %g moveto\n", vertex[0].x - xoffset, vertex[0].y - yoffset);
    fprintf(file, "(%c) show\n", letter);
    loc += 7;
    break;
  default:
    /* XXX Left as an experience to the reader. */
    printf("Incomplete implementation.  Unexpected token (%d).\n", token);
    exit(1);
  }
  return loc;
}

void
spewWireFrameEPS(FILE * file, int doSort, GLint size, GLfloat * buffer, char *creator, int bg)
{
  GLfloat clearColor[4], viewport[4];
  GLfloat lineWidth;
  int i;

  /* Read back a bunch of OpenGL state to help make the EPS
     consistent with the OpenGL clear color, line width, point
     size, and viewport. */
  glGetFloatv(GL_VIEWPORT, viewport);
  glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
  glGetFloatv(GL_LINE_WIDTH, &lineWidth);

  /* Emit EPS header. */
  fputs("%!PS-Adobe-2.0 EPSF-2.0\n", file);

  /* Notice %% for a single % in the fprintf calls. */
  fprintf(file, "%%%%Creator: %s (using OpenGL feedback)\n", creator);
  fprintf(file, "%%%%BoundingBox: %g %g %g %g\n",
    viewport[0], viewport[1], viewport[2], viewport[3]);
  fputs("%%EndComments\n", file);
  fputs("\n", file);
  fputs("gsave\n", file);
  fputs("\n", file);

  /* Output Frederic Delhoume's "gouraudtriangle" PostScript
     fragment. */
  fputs("% the gouraudtriangle PostScript fragement below is free\n", file);
  fputs("% written by Frederic Delhoume (delhoume@ilog.fr)\n", file);
  fprintf(file, "/threshold %g def\n", EPS_GOURAUD_THRESHOLD);
  for (i = 0; gouraudtriangleEPS[i]; i++) {
    fprintf(file, "%s\n", gouraudtriangleEPS[i]);
  }

  /* Set font */
  fprintf(file, "\n%s findfont\n", fontstyle[font.lstyle]);
  fprintf(file, "%i scalefont\n", font.lsize);
  fprintf(file, "setfont\n");

  fprintf(file, "\n%g setlinewidth\n", lineWidth);

  if(bg) {
  /* Clear the background like OpenGL had it. */
  fprintf(file, "%g %g %g setrgbcolor\n",
	  clearColor[0], clearColor[1], clearColor[2]);
  fprintf(file, "%g %g %g %g rectfill\n\n",
	  viewport[0], viewport[1], viewport[2], viewport[3]);
  }

  if (doSort) {
    spewSortedFeedback(file, size, buffer);
  } else {
    spewUnsortedFeedback(file, size, buffer);
  }

  /* Emit EPS trailer. */
  fputs("grestore\n\n", file);
  /*fclose(file);*/
}



void
spewHeaderPS(FILE * file, int pages, int pchoice, int ochoice) {

  GLfloat lineWidth;
  int i;
  time_t clock;

  /* Read back a bunch of OpenGL state to help make the EPS
     consistent with the OpenGL clear color, line width, point
     size, and viewport. */
  glGetFloatv(GL_LINE_WIDTH, &lineWidth);

  /* Emit PS header. */
  fputs("%!PS-Adobe-2.0\n", file);

  /* Emit PS Document Structuring Header Comments */
  /* Notice %% for a single % in the fprintf calls. */
  fprintf(file, "%%%%Creator: RNA Movies\n");
  time(&clock);
  fprintf(file, "%%%%CreationDate: %s", ctime(&clock));
  fprintf(file, "%%%%Title: %s\n", to_main.title);
  fprintf(file, "%%%%Pages: %d\n", pages);
  fputs("%%PageOrder: Ascend\n", file);
  fprintf(file, "%%%%DocumentFonts: %s\n", &(fontstyle[font.lstyle][1]));
  fprintf(file, "%%%%DocumentPaperSizes: %s\n", papernames[pchoice]);
  fprintf(file, "%%%%Orientation: %s\n", paperorientation[ochoice]);

  fputs("%%EndComments\n", file);
  fputs("\n", file);

  /* Emit PS Prolog */
  /* Output Frederic Delhoume's "gouraudtriangle" PostScript
     fragment. */
  fputs("% the gouraudtriangle PostScript fragement below is free\n", file);
  fputs("% written by Frederic Delhoume (delhoume@ilog.fr)\n", file);
  fprintf(file, "/threshold %g def\n", EPS_GOURAUD_THRESHOLD);
  for (i = 0; gouraudtriangleEPS[i]; i++) {
    fprintf(file, "%s\n", gouraudtriangleEPS[i]);
  }
  fputs("%%EndProlog\n", file);

  /* start PS code */
  /* Set font */
  fprintf(file, "\n%s findfont\n", fontstyle[font.lstyle]);
  fprintf(file, "%i scalefont\n", font.lsize);
  fprintf(file, "setfont\n");

  fprintf(file, "\n%g setlinewidth\n", lineWidth);


}

