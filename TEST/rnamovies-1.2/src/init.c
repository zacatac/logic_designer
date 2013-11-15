#include <unistd.h>
#include <stdlib.h>
#include <forms.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "../config.h"
#include "viewforms.h"
#include "viewglobal.h"
#include "init.h"


/* cursor bitmaps */
#include "lglass_s.xbm"
#include "lglass_m.xbm"
#include "rot_s.xbm"
#include "rot_m.xbm"

extern char *optarg;
extern int optind;

extern char rcs_id[];
extern char rcs_date[];
extern char rcs_rev[];

int lglass_cursor;
int rot_cursor;

void
parse_args(int *argc, char *argv[]) {

  int c;
  
  while ((c = getopt(*argc, argv, "hHD:")) != EOF)
    switch (c) {
    case 'D':
      debuglevel = atoi(optarg);
      fprintf(stderr, "Debuglevel=%d\n", debuglevel);
      break;

    case 'h':
    case 'H':
    case '?':
      dbError(DB_ARGS, "usage: %s [filename]", argv[0]);
      break;
    }

  if(optind < *argc) {
    dbDebug(DB_DEBUG, "filename set to %s", argv[optind]);
    to_main.filename = strdup(argv[optind]);
  }

  return;
}


void
setup_defaults(void) {

  to_main.filename = NULL;
  to_main.fileptr = NULL;
  to_main.zoom = 10.0;
  to_main.x_displacement = 0.0;
  to_main.y_displacement = 0.0;
  to_main.rotation = 0.0;
  to_main.n_interp = 9;
  to_main.n_struc = 0;
  to_main.n_frame = 0;
  to_main.struc_list = NULL;
  to_main.movie = NULL;
  to_main.curr_frame = NO_FR;
  to_main.label_dist = 0.4;
  to_main.label_dist = 0.7;
  to_main.strip_width = 0.15;
  to_main.framerate = 1;

  brucc.gu = TRUE;
  brucc.prime5 = TRUE;
  brucc.prime3 = TRUE;
  brucc.bases = FALSE;
  brucc.colored = TRUE;
  brucc.spline = FALSE;
  brucc.noback = FALSE;
  brucc.quads = TRUE;
  /*  brucc.loops = FALSE;*/
  brucc.dots = FALSE;
  brucc.none = FALSE;
  brucc.freq = 0;

  return;
}

void
setup_backptrs(void) {

  Pixmap pixs, pixm;

  to_main.glcanvas = to_main.fd_main->glcanvas;
  to_main.comment = to_main.fd_main->comment;
  to_main.timeout = to_main.fd_main->timeout;

  fl_set_form_minsize(to_main.fd_main->main, 460, 360);

  fl_set_object_bw(to_main.fd_main->movie_slider, 3);
  
  fl_set_input(to_main.fd_fontselect->text,
	       "The quick brown fox jumps over the lazy dog.");


  fl_set_object_label(to_main.fd_about->package, "Package: " PACKAGE);
  fl_set_object_label(to_main.fd_about->version, "Version: " VERSION);



  /* create cursors */
  lglass_cursor = fl_create_bitmap_cursor((const char *)lglass_s_bits, (const char *)lglass_m_bits, 
				      lglass_s_width, lglass_s_height,
				      lglass_s_width/2, lglass_s_height/2);

  rot_cursor = fl_create_bitmap_cursor((const char *)rot_s_bits, (const char *)rot_m_bits, 
				      rot_s_width, rot_s_height,
				      rot_s_width/2, rot_s_height/2);

  fl_set_cursor_color(lglass_cursor, FL_RED, FL_WHITE);
  fl_set_cursor_color(rot_cursor, FL_RED, FL_WHITE);  

  /* set form icon */

  fl_get_pixmapbutton_pixmap(to_main.fd_main->button9, &pixs, &pixm);
  fl_set_form_icon(to_main.fd_main->main, pixs, pixm);
 

  /* setup colors */
  /* nicht schoen, aber selten! */
  {
    int i, r, g, b;

       to_main.col_set[0].color_no = to_main.fd_color_opt->background_col->col1;
       to_main.col_set[1].color_no = to_main.fd_color_opt->backbone_col->col1;
       to_main.col_set[2].color_no = to_main.fd_color_opt->nucno_col->col1;
       to_main.col_set[3].color_no = to_main.fd_color_opt->wc_col->col1;
       to_main.col_set[4].color_no = to_main.fd_color_opt->gu_col->col1;
       to_main.col_set[5].color_no = to_main.fd_color_opt->other_bond_col->col1;
       to_main.col_set[6].color_no = to_main.fd_color_opt->a_col->col1;
       to_main.col_set[7].color_no = to_main.fd_color_opt->c_col->col1;
       to_main.col_set[8].color_no = to_main.fd_color_opt->g_col->col1;
       to_main.col_set[9].color_no = to_main.fd_color_opt->u_col->col1;
       to_main.col_set[10].color_no = to_main.fd_color_opt->other_nuc_col->col1;
       to_main.col_set[11].color_no = to_main.fd_color_opt->nuc_un_col->col1;

       for(i=0;i<12;i++) {
	 fl_get_icm_color(to_main.col_set[i].color_no, &r,&g,&b);
	 to_main.col_set[i].rgb[0] = (GLfloat) r / 255;
	 to_main.col_set[i].rgb[1] = (GLfloat) g / 255;
	 to_main.col_set[i].rgb[2] = (GLfloat) b / 255;
	 /* fprintf(stderr, "i: %d id %d: r: %f g: %f b: %f\n", i,
		 to_main.col_set[i].color_no,
		 to_main.col_set[i].rgb[0],
		 to_main.col_set[i].rgb[1],
		 to_main.col_set[i].rgb[2]); */
       }
       
  }

  return;
}

void
set_options(void) {
  static char str[20];

  fl_set_counter_value(to_main.fd_save_as->xsize,
		       to_main.fd_main->glcanvas->w);
  fl_set_counter_value(to_main.fd_save_as->ysize,
		       to_main.fd_main->glcanvas->h);
  
  sprintf(str, "%d", to_main.n_interp);
  fl_set_input(to_main.fd_interp->interp_freq, str);
  fl_set_object_label(to_main.fd_main->interpolations, str);

  fl_set_input(to_main.fd_brucc_opt->labelfreq, "0");
  fl_set_input(to_main.fd_print->print_cmd, "lpr %s");

  sprintf(str, "%ld", to_main.framerate);
  fl_set_object_label(to_main.fd_main->framespeed, str);

  return;
}





