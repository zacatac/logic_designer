/*
-------------------------------------------------------------------------------

 RNA secondary structure parser
 main C file

 Author: Dirk Evers
         Technische Fakultaet
         Arbeitsgruppe Praktische Informatik
         Universitaet Bielefeld
         dirk@TechFak.Uni-Bielefeld.DE
                                      
 Date: 20.02.96

 RCS-Id:
 $Id: viewmain.c,v 1.7 1999/08/27 16:49:59 dirk Exp dirk $

-------------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <forms.h>

#include "viewforms.h"
#include "dberror.h"
#include "viewglobal.h"
#include "init.h"

int Button = FALSE;

BP_main to_main;
GLuint dlstart;
XFontStruct *xfs = NULL;

double x_prev, y_prev;

fontdim_t fdimlist[256];

int
main (int argc, char *argv[])
{
/*=============================== automatic save ==================*/ 
#if AUTOMATIC_SAVE
    char *savetype;
    int last;
    int argc2=2;
#endif

  FL_IOPT flopt;
    
  flopt.borderWidth = 0;  
  fl_set_defaults(FL_PDBorderWidth, &flopt);
  fl_initialize(&argc, argv, 0, 0, 0);
  atexit(fl_finish);
  setup_defaults();

/*=============================== automatic save ==================*/
#if AUTOMATIC_SAVE 
  /*fprintf(stderr,"avant initOptions\n");*/
  savetype = initOptions(argc, argv, &last);
  fprintf(stderr,"savetype:%s\tsend=>\n",savetype);
  parse_args(&argc2, argv);
#else
  parse_args(&argc, argv); /*modif alain 21/05/2002*/ /*remove if automatic save is in use*/
#endif

  to_main.fd_main = create_form_main();
  to_main.fd_help = create_form_help();
  to_main.fd_fontselect = create_form_fontselect();
  to_main.fd_about = create_form_about();
  to_main.fd_brucc_opt = create_form_brucc_opt();
  to_main.fd_color_opt = create_form_color_opt();
  to_main.fd_interp = create_form_interp();

  /* to_main.fd_disp_opt = create_form_disp_opt();*/
  to_main.fd_logbrowser = create_form_log();
  to_main.fd_save_as = create_form_save_as();
  to_main.fd_print = create_form_print();

  fl_setpup_fontsize(FL_NORMAL_SIZE);
  fl_setpup_fontstyle(FL_NORMAL_STYLE);


  setup_backptrs();
  set_options();

  fl_add_canvas_handler(to_main.fd_main->glcanvas, Expose, glc_Expose, NULL);
  fl_add_canvas_handler(to_main.fd_main->glcanvas, ButtonPress, glc_ButtonPress, NULL);

  fl_add_canvas_handler(to_main.fd_main->glcanvas, EnterNotify, glc_EnterNotify, NULL);
  fl_add_canvas_handler(to_main.fd_main->glcanvas, LeaveNotify, glc_LeaveNotify, NULL);

  fl_add_canvas_handler(to_main.fd_main->glcanvas, ButtonRelease, glc_ButtonRelease, NULL);
  fl_add_canvas_handler(to_main.fd_main->glcanvas, MotionNotify, glc_MotionNotify, NULL); 
 

  dbDebug(DB_DEBUG, "\n->fl_show_form(...)\n");
#if AUTOMATIC_SAVE
  fl_show_form(to_main.fd_main->main, FL_PLACE_ICONIC,FL_FULLBORDER, argv[0]);
#else
  fl_show_form(to_main.fd_main->main, FL_PLACE_FREE,FL_FULLBORDER, argv[0]);
#endif
  
    dbDebug(DB_DEBUG, "\n<-fl_show_form(...)\n");
    gl_initialize();

     set_font_choice(FL_NORMAL_STYLE, FL_NORMAL_SIZE);
     set_comment(to_main.fd_main->comment, FL_BLUE, "Welcome to RNA Movies!");

  if(to_main.filename!=NULL) {
      dbDebug(DB_DEBUG, "filename");
      open_file();
/*=============================== automatic save ==================*/
#if AUTOMATIC_SAVE 
       	 save_frames_alain(savetype,last);
#endif
      /*open_cb(to_main.fd_main->button1, 0);*/
  }
#if !AUTOMATIC_SAVE
   fl_do_forms(); /*remove if automatic save is in use*/
#endif

  return EXIT_SUCCESS;
}

int
glc_EnterNotify(FL_OBJECT *ob, Window win, int w, int h, XEvent *xev, void *ud)
{
  dbDebug(DB_DEBUG, "->glc_EnterNotify\n");
  return 0;
}

int
glc_LeaveNotify(FL_OBJECT *ob, Window win, int w, int h, XEvent *xev, void *ud)
{
  dbDebug(DB_DEBUG, "->glc_LeaveNotify\n");
  return 0;
}



int
glc_ButtonPress(FL_OBJECT *ob, Window win, int w, int h, XEvent *xev, void *ud)
{
  dbDebug(DB_DEBUG, "->glc_ButtonPress: %d\n", xev->xbutton.button);
  
  x_prev = xev->xmotion.x*2.0/w-1.0;
  y_prev = xev->xmotion.y*2.0/h-1.0;

  Button = TRUE;

/*
  fprintf(stderr, "button: %d\n", xev->xbutton.button);
  fprintf(stderr, "state : %d\n", xev->xbutton.state);
  fprintf(stderr, "shiftmask : %d\n", ShiftMask);
 */ 

  switch(xev->xbutton.button) {

  case Button1:
    fl_set_cursor(win, XC_hand2);
    set_comment(to_main.fd_main->comment, FL_BLUE, "Move the structure by dragging the mouse");
    if(xev->xbutton.state == ShiftMask) {
      to_main.x_displacement = x_prev = 0.0;
      to_main.y_displacement = y_prev = 0.0;    
      glc_resize(to_main.glcanvas, 0);
    }
    if(xev->xbutton.state == ControlMask) {
      to_main.x_displacement = x_prev = 0.0;
      to_main.y_displacement = y_prev = 0.0;
      to_main.zoom = 10.0;
      to_main.rotation = 0.0;
      glc_resize(to_main.glcanvas, 0);
    }
    break; 

  case Button2:
    fl_set_cursor(win, lglass_cursor);
    set_comment(to_main.fd_main->comment, FL_BLUE, "Zoom the structure by dragging the mouse up or down");
    if(xev->xbutton.state == ShiftMask) {
      to_main.zoom = 10.0;
      glc_resize(to_main.glcanvas, 0);
    }
    break;

  case Button3:
    fl_set_cursor(win, rot_cursor);
    set_comment(to_main.fd_main->comment, FL_BLUE, "Rotate the structure by dragging the mouse up or down");
    if(xev->xbutton.state == ShiftMask) {
      to_main.rotation = 0.0;
      glc_resize(to_main.glcanvas, 0);
    }
    break;

  }

  XFlush(fl_get_display());

  return 0;
}

int
glc_ButtonRelease(FL_OBJECT *ob, Window win, int w, int h, XEvent *xev, void *ud)
{
  dbDebug(DB_DEBUG, "->glc_ButtonRelease\n");
  Button = FALSE;
  fl_reset_cursor(win);
  return 0;
}

int
glc_MotionNotify(FL_OBJECT *ob, Window win, int w, int h, XEvent *xev, void *ud)
{
  double x, y;
  unsigned int state;

  dbDebug(DB_DEBUG, "->glc_MotionNotify: State: %d\n", xev->xmotion.state);

  if(Button) {
    state = xev->xmotion.state & (Button1Mask|Button2Mask|Button3Mask);
    
    if(!state) return 0;
    
    x = xev->xmotion.x*2.0/w-1.0;
    y = xev->xmotion.y*2.0/h-1.0;
    
    switch(state) {
      
    case Button1Mask:
      to_main.x_displacement -= x-x_prev;
      to_main.y_displacement -= y-y_prev;
      break;
      
    case Button2Mask:
      to_main.zoom -=10.0*(y-y_prev);
      break;
      
    case Button3Mask:
      to_main.rotation +=180.0*(y-y_prev);
      break;
    }
    x_prev = x;
    y_prev = y;
    
    glc_resize(to_main.glcanvas, 0);
  }
  return 0;
}

int
glc_Expose(FL_OBJECT *ob, Window win, int w, int h, XEvent *xev, void *ud)
{

  static int w_old = 0, h_old = 0;

  dbDebug(DB_DEBUG, "->glc_Expose\n");

  if(w_old != w || h_old != h) {
    w_old = w;
    h_old = h;
    glc_ResizeRequest(ob, win, w, h, xev, ud);
  }
  else {
    show_frame();
  }

  return 0;
}

int
glc_ResizeRequest(FL_OBJECT *ob, Window win, int w, int h, XEvent *xev, void *ud)
{

  dbDebug(DB_DEBUG, "->glc_ResizeRequest\nw: %d h: %d z:%f\n", ob->w, ob->h, to_main.zoom);

  gl_resize(w,h);
  gl_initialize();

  show_frame();

  return 0;

}

void
open_cb(FL_OBJECT *ob, long data) {

  const char *filename;

    filename = fl_show_fselector("Select a RNA Movies File:",NULL,"*.rnm", NULL);
    if(filename != NULL) {
      if(to_main.filename != NULL) {
	free(to_main.filename);
      }
      to_main.filename = strdup(filename);
      open_file();
    }
  
  return;
}

void open_file(void) {

  FILE *fileptr;
  int strucs;

  if(to_main.filename != NULL && to_main.filename[0] != '\0') {
    if (to_main.fileptr != NULL) {
      fclose(to_main.fileptr);
      to_main.fileptr = NULL;      
    }
    fileptr = fopen(to_main.filename, "r");
    if(fileptr == NULL) {
      fl_show_alert("Unable to open file:", to_main.filename, "", TRUE);
      free(to_main.filename);
      to_main.filename = NULL;
      to_main.fileptr = NULL;
      free_struc_list();
      return;
    }
    else {
      set_comment(NULL, FL_BLUE, "Opened file: %s",  to_main.filename);
      
      to_main.fileptr = fileptr;
      free_struc_list();

      strucs = count_strucs(fileptr);
      set_comment(NULL, FL_BLUE, "Found %i structures",  strucs);

      if (parse_strucs(strucs)==0)
	new_movie();
    }
  }
  
  return;
}

void
set_comment(FL_OBJECT *ob, FL_COLOR lcol, const char *fmt, ...) {

  va_list ap;
  static char buf[100];

  va_start(ap,fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  fl_set_object_lcol(to_main.comment, lcol);
  fl_set_object_label(to_main.comment, buf);
  fl_addto_browser(to_main.fd_logbrowser->browser, buf);

  if(lcol == FL_RED) {
    /* error message stays and rings */
    fl_ringbell(0);
  }
  else {
    fl_set_timer(to_main.timeout, 5.0);
  }

  XFlush(fl_get_display());

  return;
}


void empty_cb(FL_OBJECT *ob, long data) {
  return;
}

void update_cb(FL_OBJECT *ob, long data) {
    glc_resize(to_main.glcanvas, 0);
}

void
update_glc(FL_OBJECT *ob, long data)
{
  static char buf[200];


  dbDebug(DB_DEBUG, "\n->update_glc\n%ld\n", data);
  sprintf(buf, "Structure No: %ld", data);
  fl_set_object_label(to_main.comment, buf);

  glColor3f(1.0, 0.0, 0.0); 
 

  glXSwapBuffers(fl_display, FL_ObjWin(ob)); 
  glClear(GL_COLOR_BUFFER_BIT);

  return;
}

void
glc_resize(FL_OBJECT *ob, long data) {

  glc_ResizeRequest(ob, FL_ObjWin(ob), ob->w, ob->h, NULL, NULL);

  return;
}

void
glc_expose(FL_OBJECT *ob, long data) {

  glc_Expose(ob, FL_ObjWin(ob), ob->w, ob->h, NULL, NULL);

  return;
}


GLuint
glc_SetFontDL(GLuint dl) {
  
  GLuint temp;
  temp = dlstart;
  dlstart = dl;
  
  return temp;
}

void
glc_SetFont(int style, int size) {
  
  int height, width;
  int ascend, descend;
  
  xfs = fl_get_fntstruct(style, size);
  height = fl_get_char_height(style, size, &ascend, &descend);
  width = fl_get_char_width(style, size);

  /*  dbDebug(DB_DEBUG, "height: %d\twidth: %d\n",height, width);
  dbDebug(DB_DEBUG, "ascend: %d\tdescend: %d\n",ascend, descend); */

  generate_font(to_main.glcanvas, fdimlist, xfs->fid, 0 , 256, dlstart);

  return;
}

void
glc_Print(GLfloat x, GLfloat y, const char *fmt, ...) {

  static char the_str[GLC_STRLEN];
  va_list args;
  int i;
  
  va_start(args, fmt);
  vsprintf(the_str, fmt, args);
  va_end(args);

  glRasterPos2f(x, y);

  /* glPushAttrib(GL_LIST_BIT); */

  for(i=0;i<strlen(the_str);i++) {
    glPassThrough((GLfloat) the_str[i]);
    glCallList(dlstart + the_str[i]);
  }
    /*  glCallLists(strlen(the_str), GL_UNSIGNED_BYTE, (GLubyte *) the_str); */

  /*  glPopAttrib(); */

  return;
}

int
gl_initialize(void) {

  static int flag = FALSE;
  
  glClearColor(to_main.col_set[0].rgb[0],
	       to_main.col_set[0].rgb[1],
	       to_main.col_set[0].rgb[2],
	       0.0);
  glColor3f(1.0, 1.0, 1.0);
  glShadeModel( GL_FLAT );
  
  if(flag==FALSE) {
    dlstart = glGenLists(256);
    flag = TRUE;
  }

  return 0;
}

void
gl_resize(GLint width, GLint height) {
  
  glShadeModel( GL_FLAT );
  glViewport(0,0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  to_main.zoom = to_main.zoom < 0.1 ? 0.1 : to_main.zoom;

  if(width<=height)
    glOrtho(to_main.zoom, -to_main.zoom, -to_main.zoom*(GLfloat)height/(GLfloat)width,
	    to_main.zoom*(GLfloat)height/(GLfloat)width, -1.0, 1.0);
  else
    glOrtho(to_main.zoom*(GLfloat)width/(GLfloat)height,
	    -to_main.zoom*(GLfloat)width/(GLfloat)height, -to_main.zoom, to_main.zoom, -1.0, 1.0);
  
  glMatrixMode(GL_MODELVIEW);
  
  glLoadIdentity();
  glTranslated(to_main.zoom*to_main.x_displacement, to_main.zoom*to_main.y_displacement, 0);
  glRotated(to_main.rotation, 0.0, 0.0, 1.0);
}


char *initOptions(int argc, char **argv, int *last)
{
    int i,framesave=0;
	int temp, x, y;		
    int inoprint = 0;
    /* FILE* fnoprint = NULL; */

	/* added by Thomas on 02/07/2002 */
	g_quality = -1;			/* the default quality for JPG files is the optimum quality */
	g_xsize=-1;				/* -1 is the default value ... when generating the picture, if g_xsize==-1, then the default size (cf to_main) is taken */
	g_ysize=-1;
	g_basename=NULL;		/* the base name of the files, for ex, with 'myname' as base, you get myname_00000.eps, myname_00001.eps etc files 
								if(g_basename==NULL), the default base name is used (default = the root of the source file name )
							*/
	g_comments=0;			/* insertion of comments into the picture */
	
	
    *last=100000000;
    to_main.n_interp = 2 ;
    /* analysis of the command line options */
    for(i=1; i<argc; i++)
    {
	if(strncmp(argv[i], "-eps", 4)==0 || strncmp(argv[i], "-EPS", 4)==0)
	{ framesave +=1;
	}
	else if(strncmp(argv[i], "-jpg", 4)==0 || strncmp(argv[i], "-JPG", 4)==0)
	{ framesave +=5;
	}
	else if(strncmp(argv[i], "-png", 3)==0 || strncmp(argv[i], "-png", 3)==0)
	{ framesave +=10;
	}
	else if(strncmp(argv[i], "-coord", 6)==0 || strncmp(argv[i], "-COORD", 6)==0)
	{ framesave +=25;
	}
	else if(strncmp(argv[i], "-lasteps_coord", 13)==0 || strncmp(argv[i], "-LASTEPS_COORD", 13)==0)
	{ framesave +=125;
	}
	else if(strncmp(argv[i], "-lastjpeg_coord", 14)==0 || strncmp(argv[i], "-LASTJPEG_COORD", 14)==0)
	{ framesave +=625;
	}
	else if(strncmp(argv[i], "-lastpng_coord", 13)==0 || strncmp(argv[i], "-LASTPNG_COORD", 13)==0)
	{ framesave +=1225;
	}
	else if(strncmp(argv[i], "-i=", 3)==0 || strncmp(argv[i], "-I=", 3)==0)/* interpolation */
	{
	    to_main.n_interp = atoi(argv[i]+3);
	    printf("to_main.n_interp %i\n",to_main.n_interp);
	    if (to_main.n_interp < 0 || to_main.n_interp > 10)
		to_main.n_interp = 2 ;
	}
	else if(strncmp(argv[i], "-last=", 6)==0 || strncmp(argv[i], "-LAST=", 6)==0)/* the n last structures */
	{ 
	    *last = atoi(argv[i]+6);
	    if (*last <= 0)
		*last=1;
	}
	else if(strncmp(argv[i], "-noprint", 8)==0) /* noprint */
	{
	    fprintf(stderr, "Noprint asked\n");
		
		fclose(stderr);
	    fclose(stdout);
	    close(1);
	    close(2);

	    /*if((fnoprint=fopen("/dev/null", "r+"))==NULL)
		perror("Error: cannot fopen /dev/null");
	    if((inoprint=open("/dev/null", O_RDWR))==-1)
	    perror("Error: cannot open /dev/null");*/

	    /* XXX hack: gives invalid lvalue in assignment in sol2 gcc-3.2
              stderr=fnoprint;
              stdout=fnoprint;
            */
	    dup(inoprint);/* std output */
	    dup(inoprint);/* err output */
	}
	else if(strncmp(argv[i], "-quality=", 9) == 0 || strncmp(argv[i], "-QUALITY=", 9) == 0)	/* the quality of jpg generated files */
	{
		temp = atoi(argv[i]+9);
		fprintf(stderr, "Quality =%d\n", temp);
		
		if(temp != 0 && temp >=-1 && temp <=100 ) /* !=0 when the parameter is bad, so the quality is still -1 (optimum) */
			g_quality = temp;	
	}
	else if(strncmp(argv[i], "-size=", 6) == 0 || strncmp(argv[i], "-SIZE=", 6) == 0)	/* the size of the generated pictures (only for JPGs yet) */
	{
		temp=6;
		while(argv[i][temp++] != 'x');
		y=atoi(argv[i]+temp);		fprintf(stderr,"Y=%d\n", y);
		*(argv[i]+temp-1)='\0';
		x=atoi(argv[i]+6);			fprintf(stderr,"X=%d\n", x);
		
		if( x>0 && y>0 )
			g_xsize=x;
			g_ysize=y;
    }
	else if(strncmp(argv[i], "-name=", 6) == 0 || strncmp(argv[i], "-NAME=", 6) ==0)		/* the base name for the generated file ... used because of the two rnm files generated by Kine */
	{
		temp = strlen(argv[i]+6);
		
		if(temp != 0)
		{
			if((g_basename=malloc( (temp +1)*sizeof(char))) ==NULL)
			{
				fprintf(stderr,"Error: cannot allocate memory for basename.\n");
				exit(EXIT_FAILURE);
			}
			
			strcpy(g_basename, argv[i]+6);
			fprintf(stderr,"Basename=%s\n", g_basename);
		}
	}
	else if(strncmp(argv[i], "-comments", 9) == 0 || strncmp(argv[i], "-COMMENTS", 9) == 0)		/* insertion of comments in the picture */
	{
		g_comments = 1;
	}
	}		

	

    switch(framesave){
	case 1 : return "eps" ; break;
	case 5 : return "jpeg" ; break;
	case 10 : return "png" ; break;
	case 25 : return "coord" ; break;
	case 26 : return "coord_eps"; break;
	case 30 : return "coord_jpeg"; break;
	case 35 : return "coord_png" ; break;
	case  125 : return "coord_lasteps"; break;
	case  625 : return "coord_lastjpeg"; break;
	case 1225 : return "coord_lastpng" ; break;
	default: return "coord_lasteps"; break;
    }
}





