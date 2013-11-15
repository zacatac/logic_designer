#include <stdlib.h>
#include <stdio.h>
#include <forms.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <gd.h>
#include <libgen.h>

#include "viewforms.h"
#include "dberror.h"
#include "viewglobal.h"
#include "help.h"

#ifdef MESA
#include <GL/osmesa.h>
#endif /* MESA */

extern FILE *yyin;

font_t font;

int yyparse (void);
void yyrestart(FILE *);
void save_font_choice(FL_OBJECT *ob, long data);
void accept_font_choice(FL_OBJECT *, long);
void cancel_font_choice(FL_OBJECT *ob, long data);
void accept_options_choice(FL_OBJECT *ob, long data);
void cancel_options_choice(FL_OBJECT *ob, long data);
void accept_interp_choice(FL_OBJECT *ob, long data);
void cancel_interp_choice(FL_OBJECT *ob, long data);
void save_2d_coords(int frame_from, int frame_to);
void save_png_file(int frame_from, int frame_to);
void zoomfac_cb(FL_OBJECT *ob, long data);
void x_displace_cb(FL_OBJECT *ob, long data);
void y_displace_cb(FL_OBJECT *ob, long data);
void rotation_cb(FL_OBJECT *ob, long data);
void print_frames(void);

/* callbacks for form main */
void file_mnu_cb(FL_OBJECT *ob, long data)
{
  /* fill-in code for callback */
  int num;

  num = fl_get_menu(ob);
  switch(num) {
    /* Open */
  case 1:
    open_cb(ob, data);
    break;
    /* Save */
  case 2:
    if(to_main.curr_frame != NO_FR) {
      char *tmp = NULL;
      char *filename;

      filename = strdup(to_main.filename);
      pause_movie();
      fl_deactivate_all_forms();

      fl_set_input(to_main.fd_save_as->frames_from, "0");
      fl_set_input(to_main.fd_save_as->frames_to, int2str(to_main.n_frame-1));
      if( *(fl_get_input(to_main.fd_save_as->prefix)) == '\0')
	fl_set_input(to_main.fd_save_as->prefix, tmp = file_prefix(basename(filename)));
      if( *(fl_get_input(to_main.fd_save_as->pathname)) == '\0')
	fl_set_input(to_main.fd_save_as->pathname, dirname(filename));

      fl_show_form(to_main.fd_save_as->save_as,
		   FL_PLACE_SIZE, FL_TRANSIENT,"Save");
      free(filename);
      if(tmp != NULL)
	free(tmp);
    } else
      set_comment(to_main.comment, FL_RED, "Nothing to save");
    break;
    /* Print Dialog */
  case 3:
    if(to_main.curr_frame != NO_FR) {
      pause_movie();
      fl_deactivate_all_forms();
      fl_set_input(to_main.fd_print->frames_from, "0");
      fl_set_input(to_main.fd_print->frames_to, int2str(to_main.n_frame-1));
      fl_show_form(to_main.fd_print->print,
		   FL_PLACE_SIZE, FL_TRANSIENT,"Print");   
    } else
      set_comment(to_main.comment, FL_RED, "Nothing to print"); 
    break;
    /* Quit */
  case 4:
    exit(EXIT_SUCCESS);
    break;
    /* No selection */
  default:
    break;
  }

  return; 
}

void help_mnu_cb(FL_OBJECT *ob, long data)
{
  /* fill-in code for callback */
  int num;
  
  num = fl_get_menu(ob);
  switch(num) {
    /* About */
  case 1:
    fl_show_form(to_main.fd_about->about,
		 FL_PLACE_CENTER,FL_TRANSIENT,"About");
    break;
    /* Instructions */
  case 2:
    fl_show_form(to_main.fd_help->help,
		 FL_PLACE_SIZE,FL_FULLBORDER,"Help");
    break;
    /* Log Browser */
  case 3:
    fl_show_form(to_main.fd_logbrowser->log,
		 FL_PLACE_SIZE, FL_FULLBORDER, "Log Browser");
    break;

  default:
    break;
  }

  return;
}


const char *na_infile;
char *na_outfile;

void but_cb(FL_OBJECT *ob, long data)
{

  /* fill-in code for callback */
  switch(data) {
    /* Open */
  case 1:
    open_cb(ob, data);
    break;
    /* Reload */
  case 2:
    if(to_main.filename != NULL && to_main.fileptr != stdin) {
      open_file();
    }
    else {
      set_comment(to_main.comment, FL_RED, "Nothing to reload!");
    }
    break;
    /* Font */
  case 3:
    save_font_choice(ob, data);
    fl_show_form(to_main.fd_fontselect->fontselect, FL_PLACE_FREE,FL_FULLBORDER,"Font Select");   
    break;
    /* Undefined */
  case 4:
    break;
    /* Quit */
  case 5:
    exit(EXIT_SUCCESS);
    break;
    /* snapshot */
  case 9:
    if(to_main.curr_frame != NO_FR) {
      char *tmp;
      char *filename = strdup(to_main.filename);

      if( *(fl_get_input(to_main.fd_save_as->prefix)) == '\0')
	fl_set_input(to_main.fd_save_as->prefix, tmp = file_prefix(basename(filename)));
      if( *(fl_get_input(to_main.fd_save_as->pathname)) == '\0')
	fl_set_input(to_main.fd_save_as->pathname, dirname(filename));

      if (fl_get_button(to_main.fd_save_as->format_png)) {
	save_graphics_file(FORMAT_PNG, to_main.curr_frame, to_main.curr_frame);
      }
      else if (fl_get_button(to_main.fd_save_as->format_jpg)) {
	save_graphics_file(FORMAT_JPG, to_main.curr_frame, to_main.curr_frame);
      }
      else if (fl_get_button(to_main.fd_save_as->format_eps)) {
	outputEPS(to_main.curr_frame, to_main.curr_frame,
		  fl_get_button(to_main.fd_save_as->bg));
      }
      else if (fl_get_button(to_main.fd_save_as->format_2dcoords)) {
	save_2d_coords(to_main.curr_frame, to_main.curr_frame);
      }
      free(filename);
    } else
      set_comment(to_main.comment, FL_RED, "Nothing to save");
    break;
    /* Dismiss Help */
  case 10:
    fl_hide_form(to_main.fd_help->help);
    break;
    /* Cancel Font Select */
  case 20:
    fl_hide_form(to_main.fd_fontselect->fontselect);
    cancel_font_choice(ob, data);
    break;
    /* Accept Font Select */
  case 21:
    accept_font_choice(ob, data);
    fl_hide_form(to_main.fd_fontselect->fontselect);
    break;
    /* Dismiss About Form*/
  case 30:
    fl_hide_form(to_main.fd_about->about);
    break;
    /* Dismiss Layout Form */
  case 40:
    fl_hide_form(to_main.fd_brucc_opt->brucc_opt);
    break;
    /* Close Zoom */
  case 50:
    /*    fl_hide_form(to_main.fd_disp_opt->disp_opt); */
    /*cancel_options_choice(ob, data);*/
    break;
    /* Pause */
  case 60:
    pause_movie();
    break;
    /* Play */
  case 61:
    play_movie();
    break;
    /* Forward */
  case 62:
    forward_movie();
    break;
    /* Backward */
  case 63:
    backward_movie();
    break;
    /* Rewind */
  case 64:
    set_movie(0);
    break;
    /* close log browser */
  case 80:
    fl_hide_form(to_main.fd_logbrowser->log);
    break;
    /* clear log browser */
  case 81:
    fl_clear_browser(to_main.fd_logbrowser->browser);
    break;
    /* cancel save dialog */
  case 90:
    fl_hide_form(to_main.fd_save_as->save_as);
    cancel_options_save(ob, data);
    fl_activate_all_forms();
    break;
    /* accept save dialog */
  case 91:
    fl_hide_form(to_main.fd_save_as->save_as);
    save_frames();
    fl_activate_all_forms();
    break;
    /* fit to window size */
  case 92:
    fl_set_counter_value(to_main.fd_save_as->xsize,
			 to_main.fd_main->glcanvas->w);
    fl_set_counter_value(to_main.fd_save_as->ysize,
			 to_main.fd_main->glcanvas->h);
    break;
    /* apply save dialog */
  case 93:
    fl_hide_form(to_main.fd_save_as->save_as);
    fl_activate_all_forms();
    break;
    /* jump to structure number */
  case 101:
    if(to_main.curr_frame != NO_FR) {
      int struc;
      const char *nptr;
      char *eptr = NULL;
      nptr = fl_show_input("Jump to structure number:", "");
      if(nptr) {
	struc = (int)strtol(nptr, &eptr, 10);
	if((eptr != nptr) && (struc > 0))
	  set_movie((struc-1) * (to_main.n_interp+1));
	else
	  set_comment(to_main.fd_main->comment, FL_RED,
		      "Error: frame input: not unsigned integer");
      }
    }
    break;
    /* jump to frame number */
  case 102:
    if(to_main.curr_frame != NO_FR) {
      int frame;
      const char *nptr;
      char *eptr = NULL;
      nptr = fl_show_input("Jump to frame number:", "");
      if(nptr) {
	frame = (int)strtol(nptr, &eptr, 10);
	if((eptr != nptr) && (frame >= 0))
	  set_movie(frame);
	else
	  set_comment(to_main.fd_main->comment, FL_RED,
		      "Error: frame input: not unsigned integer");
      }
    }
    break;
    /* Frame Speed */
  case 103: 
    {
      long speed;
      const char *nptr;
      char *eptr = NULL;
      nptr = fl_show_input("Enter frame rate in milliseconds:", "");
      if(nptr) {
	speed = strtol(nptr, &eptr, 10);
	if((eptr != nptr) && (speed >= 0)) {
	  to_main.framerate = speed;
	  fl_set_object_label(to_main.fd_main->framespeed, nptr);
	}
	else
	  set_comment(to_main.fd_main->comment, FL_RED,
		      "Error: frame rate input: not unsigned long integer");
      }
    }
    break;
    /* Number of Interpolations */
  case 104: 
    {
      long interp;
      const char *nptr;
      char *eptr = NULL;
      nptr = fl_show_input("Enter number of interpolations between structures:", "");
      if(nptr) {
	interp = strtol(nptr, &eptr, 10);
	if((eptr != nptr) && (interp >= 0)) {
          /* success */
          fl_set_input(to_main.fd_interp->interp_freq, nptr);
          accept_interp_choice((FL_OBJECT *)NULL, (long)0);
	}
	else {
          /* wrong input */
	  set_comment(to_main.fd_main->comment, FL_RED,
		      "Error: interpolation input: not unsigned long integer");
        }
      }   

    }
    break;
  case 110:
    fl_hide_form(to_main.fd_color_opt->color_opt);
    break;

    /* Cancel Interpolation */
  case 120:
    fl_hide_form(to_main.fd_interp->interp);
    cancel_interp_choice(ob, data);
    break;
    /* Accept Interpolation */
  case 121:
    accept_interp_choice(ob, data);
    fl_hide_form(to_main.fd_interp->interp);
    break;
    /* Cancel Printing */
  case 130:
    fl_hide_form(to_main.fd_print->print);
    fl_activate_all_forms();
    break;
    /* Accept Printing */
  case 131:
    fl_hide_form(to_main.fd_print->print);
    print_frames();
    fl_activate_all_forms();
    break;

  default:
    set_comment(to_main.fd_main->comment, FL_RED,
		"but_cb: no Callback for Button %ld installed!\n", data);
    break;
  }

  return;
}


void col_cb(FL_OBJECT *ob, long data)
{
  int r,g,b;

  to_main.col_set[data].color_no = fl_show_colormap(ob->col1);
  fl_set_object_color(ob, to_main.col_set[data].color_no, ob->col2);
  fl_get_icm_color(to_main.col_set[data].color_no, &r,&g,&b);
  to_main.col_set[data].rgb[0] = (GLfloat) r / 255;
  to_main.col_set[data].rgb[1] = (GLfloat) g / 255;
  to_main.col_set[data].rgb[2] = (GLfloat) b / 255;

  /*  fprintf(stderr, "id %d: r: %f g: %f b: %f\n", to_main.col_set[data].color_no,
	  to_main.col_set[data].rgb[0],
	  to_main.col_set[data].rgb[1],
	  to_main.col_set[data].rgb[2]); */

  if(ob == to_main.fd_color_opt->background_col) 
    gl_initialize();

  show_frame();

}

void options_mnu_cb(FL_OBJECT *ob, long data)
{
  /* fill-in code for callback */
  int num;
  
  num = fl_get_menu(ob);
  switch(num) {
    /* Interpolations */
  case 1:
    fl_set_input_selected(to_main.fd_interp->interp_freq, TRUE);
    fl_show_form(to_main.fd_interp->interp,
		 FL_PLACE_SIZE, FL_FULLBORDER,"Interpolations");
    break;
    /* Layout */
  case 2:
    fl_set_input_selected(to_main.fd_brucc_opt->labelfreq, TRUE);
    fl_show_form(to_main.fd_brucc_opt->brucc_opt,
		 FL_PLACE_SIZE, FL_FULLBORDER,"Layout");
    break;
    /* Colors */
  case 3:
    fl_show_form(to_main.fd_color_opt->color_opt,
		 FL_PLACE_SIZE, FL_FULLBORDER,"Color Selection");
    break;
    /* Font */
  case 4:
    save_font_choice(ob, data);
    fl_show_form(to_main.fd_fontselect->fontselect,
		 FL_PLACE_SIZE, FL_FULLBORDER,"Font Selection");
    break;

  default:
    break;
  }

  return;
}

void text_timeout_cb(FL_OBJECT *ob, long data)
{
  /* fill-in code for callback */
  /* set comment object to empty string after timeout,
     if comment starts with string from timeout_list (see viewglobal.h) */

#define TIMEOUT_LIST_LEN 3
  static const char *timeout_list[] = {"error", "pause", "welcome"};
  const char * buffer;
  int i;

  buffer = fl_get_object_label(to_main.fd_main->comment);

  for(i=0;i<TIMEOUT_LIST_LEN;i++) {
    if(!strncasecmp(buffer, timeout_list[i], strlen(timeout_list[i]))) {
      fl_set_object_label(to_main.fd_main->comment, "");
      fl_set_object_lcol(to_main.fd_main->comment, FL_BLUE);
      break;
    }
  }

  return;
}



/* callbacks for form help */


/* callbacks for form fontselect */
void
font_choice_cb(FL_OBJECT *ob, long data) {
  /* fill-in code for callback */
  int style;

  style = fl_get_choice(ob);
  fl_set_object_lstyle(to_main.fd_fontselect->text, --style);
  return;
}

void
size_choice_cb(FL_OBJECT *ob, long data) {
  /* fill-in code for callback */

  switch (fl_get_choice(ob))
  {
    case 1: fl_set_object_lsize(to_main.fd_fontselect->text, 8); break;
    case 2: fl_set_object_lsize(to_main.fd_fontselect->text, 10); break;
    case 3: fl_set_object_lsize(to_main.fd_fontselect->text, 12); break;
    case 4: fl_set_object_lsize(to_main.fd_fontselect->text, 14); break;
    case 5: fl_set_object_lsize(to_main.fd_fontselect->text, 18); break;
    case 6: fl_set_object_lsize(to_main.fd_fontselect->text, 24); break;
  }

  return;
}

void
save_font_choice(FL_OBJECT *ob, long data) {

  font.lstyle = to_main.fd_fontselect->text->lstyle;
  font.lsize = to_main.fd_fontselect->text->lsize;

  return;
}

void
accept_font_choice(FL_OBJECT *ob, long data) {

  /** check if any chages were made **/
  if((font.lstyle == to_main.fd_fontselect->text->lstyle) && 
     (font.lsize == to_main.fd_fontselect->text->lsize)) 
  { /* nothing to do */
    return;
  }
  else {
    font.lstyle = to_main.fd_fontselect->text->lstyle;
    font.lsize = to_main.fd_fontselect->text->lsize;

    glc_SetFont(font.lstyle, font.lsize);
    glc_resize(to_main.glcanvas, 0);
  }
  return;
}

void
cancel_font_choice(FL_OBJECT *ob, long data) {

  fl_set_object_lsize(to_main.fd_fontselect->text, font.lsize);
  fl_set_object_lstyle(to_main.fd_fontselect->text, font.lstyle);

  fl_set_choice(to_main.fd_fontselect->font, font.lstyle+1);
  switch(font.lsize) {
  case 8:fl_set_choice(to_main.fd_fontselect->size, 1); break;
  case 10:fl_set_choice(to_main.fd_fontselect->size, 2); break;
  case 12:fl_set_choice(to_main.fd_fontselect->size, 3); break;
  case 14:fl_set_choice(to_main.fd_fontselect->size, 4); break;
  case 18:fl_set_choice(to_main.fd_fontselect->size, 5); break;
  case 24:fl_set_choice(to_main.fd_fontselect->size, 6); break;
  default: break;
  }

  return;
}

void
set_font_choice(lstyle, lsize) {

  font.lstyle = lstyle;
  font.lsize = lsize;

  fl_set_object_lsize(to_main.fd_fontselect->text, font.lsize);
  fl_set_object_lstyle(to_main.fd_fontselect->text, font.lstyle);

  fl_set_choice(to_main.fd_fontselect->font, font.lstyle+1);
  switch(font.lsize) {
  case 8:fl_set_choice(to_main.fd_fontselect->size, 1); break;
  case 10:fl_set_choice(to_main.fd_fontselect->size, 2); break;
  case 12:fl_set_choice(to_main.fd_fontselect->size, 3); break;
  case 14:fl_set_choice(to_main.fd_fontselect->size, 4); break;
  case 18:fl_set_choice(to_main.fd_fontselect->size, 5); break;
  case 24:fl_set_choice(to_main.fd_fontselect->size, 6); break;
  default: break;
  }

  glc_SetFont(font.lstyle, font.lsize);
  glc_resize(to_main.glcanvas, 0);

  return;
}

/* callbacks for form brucc_opt */

void
labelfreq_cb(FL_OBJECT *ob, long data) {

  /* fill-in code for callback */

  return;
}

void
accept_interp_choice(FL_OBJECT *ob, long data) {

  static char str[20];
  
  /* if the interpolation value has really changed */
  if(to_main.n_interp != abs(atoi(fl_get_input(to_main.fd_interp->interp_freq)))) {
    free_movie();
    
    to_main.n_interp = abs(atoi(fl_get_input(to_main.fd_interp->interp_freq)));
    sprintf(str, "%d", to_main.n_interp);
    /* set value in interp form and display button bar */
    fl_set_input(to_main.fd_interp->interp_freq, str);
    fl_set_object_label(to_main.fd_main->interpolations, str);

    new_movie();
  }
}

void
cancel_interp_choice(FL_OBJECT *ob, long data) {

  static char str[20];
  
  sprintf(str, "%d", to_main.n_interp);
  fl_set_input(to_main.fd_interp->interp_freq, str);
}

#ifdef DEFUNCT
void
accept_options_choice(FL_OBJECT *ob, long data) {

  static char str[100];

  /** check if any changes were made **/
  if((fl_get_button(to_main.fd_brucc_opt->but400) == brucc.prime5) &&
     (fl_get_button(to_main.fd_brucc_opt->but401) == brucc.prime3) && 
     (fl_get_button(to_main.fd_brucc_opt->button42) == brucc.bases) &&
     (fl_get_button(to_main.fd_brucc_opt->button43) == brucc.colored) &&
     (fl_get_button(to_main.fd_brucc_opt->button44) == brucc.spline) &&
     (fl_get_button(to_main.fd_brucc_opt->button48) == brucc.quads) &&
     (fl_get_button(to_main.fd_brucc_opt->button403) == brucc.noback) &&
     (fl_get_button(to_main.fd_brucc_opt->button47) == (!brucc.spline && !brucc.quads && !brucc.noback)) &&
     (fl_get_button(to_main.fd_brucc_opt->button45) == !brucc.none) &&
     (fl_get_button(to_main.fd_brucc_opt->button46) == brucc.none) &&
     (fl_get_button(to_main.fd_brucc_opt->button49) == brucc.gu) &&
     (brucc.freq == abs(atoi(fl_get_input(to_main.fd_brucc_opt->labelfreq)))) &&
     (to_main.n_interp == abs(atoi(fl_get_input(to_main.fd_brucc_opt->interp_freq)))))
  {
    /* no changes, nothing to do */
    return;
  }
  else {
    free_movie();
    
    brucc.prime5 = fl_get_button(to_main.fd_brucc_opt->but400);
    brucc.prime3 = fl_get_button(to_main.fd_brucc_opt->but401);

    brucc.bases = fl_get_button(to_main.fd_brucc_opt->button42);
    brucc.colored = fl_get_button(to_main.fd_brucc_opt->button43);

    brucc.spline = fl_get_button(to_main.fd_brucc_opt->button44);
    brucc.quads = fl_get_button(to_main.fd_brucc_opt->button48);
    brucc.noback = fl_get_button(to_main.fd_brucc_opt->button403);

    brucc.none = fl_get_button(to_main.fd_brucc_opt->button46);
    brucc.gu = fl_get_button(to_main.fd_brucc_opt->button49);

    brucc.freq = abs(atoi(fl_get_input(to_main.fd_brucc_opt->labelfreq)));
    to_main.n_interp = abs(atoi(fl_get_input(to_main.fd_brucc_opt->interp_freq)));
    sprintf(str, "%d", brucc.freq);
    fl_set_input(to_main.fd_brucc_opt->labelfreq, str);
    sprintf(str, "%d", to_main.n_interp);
    fl_set_input(to_main.fd_brucc_opt->interp_freq, str);

    new_movie();
  }
  return;
}

void
cancel_options_choice(FL_OBJECT *ob, long data) {

  static char str[100];

  fl_set_button(to_main.fd_brucc_opt->but400, brucc.prime5);
  fl_set_button(to_main.fd_brucc_opt->but401, brucc.prime3);

  fl_set_button(to_main.fd_brucc_opt->button42, brucc.bases);
  fl_set_button(to_main.fd_brucc_opt->button43, brucc.colored);

  fl_set_button(to_main.fd_brucc_opt->button44, brucc.spline);
  fl_set_button(to_main.fd_brucc_opt->button48, brucc.quads);
  fl_set_button(to_main.fd_brucc_opt->button403, brucc.noback);

  fl_set_button(to_main.fd_brucc_opt->button47, !brucc.spline && !brucc.quads && !brucc.noback);

  fl_set_button(to_main.fd_brucc_opt->button45, !brucc.none);
  fl_set_button(to_main.fd_brucc_opt->button46, brucc.none);
  fl_set_button(to_main.fd_brucc_opt->button49, brucc.gu);

  sprintf(str, "%d", brucc.freq);
  fl_set_input(to_main.fd_brucc_opt->labelfreq, str);
  sprintf(str, "%d", to_main.n_interp);
  fl_set_input(to_main.fd_brucc_opt->interp_freq, str);

  return;
}
#endif

void
cancel_options_save(FL_OBJECT *ob, long data) {

  char *tmp;
  char *filename = strdup(to_main.filename);

  fl_set_button(to_main.fd_save_as->frames_current, 1);
  fl_set_button(to_main.fd_save_as->frames_all, 0);
  fl_set_button(to_main.fd_save_as->frames_fr_to, 0);
  fl_set_button(to_main.fd_save_as->format_png, 1);
  fl_set_button(to_main.fd_save_as->format_jpg, 0);
  fl_set_button(to_main.fd_save_as->format_eps, 0);
  fl_set_button(to_main.fd_save_as->format_2dcoords, 0);

  fl_set_input(to_main.fd_save_as->prefix, tmp = file_prefix(basename(filename)));
  fl_set_input(to_main.fd_save_as->pathname, dirname(filename));

  free(tmp);
  free(filename);
}

void
zoomfac_cb(FL_OBJECT *ob, long data) {

  to_main.zoom = fl_get_counter_value(ob);
  glc_resize(to_main.glcanvas, 0);
return;
}

void
movie_slider_cb(FL_OBJECT *ob, long data) {

  int frame;
  
  frame = fl_get_slider_value(to_main.fd_main->movie_slider);
  set_movie(frame);
  
  return;
}

void
x_displace_cb(FL_OBJECT *ob, long data) {
  
  to_main.x_displacement = fl_get_slider_value(ob); 
  glc_resize(to_main.glcanvas, 0);
  
  return;
}

void
y_displace_cb(FL_OBJECT *ob, long data) {
  
  to_main.y_displacement = fl_get_slider_value(ob);
  glc_resize(to_main.glcanvas, 0);  
  return;
}

void
rotation_cb(FL_OBJECT *ob, long data) {


  dbDebug(DB_DEBUG, "in rotation_cb...");
  to_main.rotation = fl_get_dial_value(ob);
  dbDebug(DB_DEBUG, "rotation: %f", to_main.rotation);
  glc_resize(to_main.glcanvas, 0);  
  return;
}

void
interp_freq_cb(FL_OBJECT *ob, long data) {

  return;
}

void
label_dist_cb(FL_OBJECT *ob, long data) {


  dbDebug(DB_DEBUG, "in label_dist_cb...");
  to_main.label_dist = fl_get_slider_value(ob);
  dbDebug(DB_DEBUG, "label distance: %f", to_main.label_dist);
  glc_resize(to_main.glcanvas, 0);  
  return;
}

void
freq_dist_cb(FL_OBJECT *ob, long data) {


  dbDebug(DB_DEBUG, "in freq_dist_cb...");
  to_main.freq_dist = fl_get_slider_value(ob);
  dbDebug(DB_DEBUG, "freq distance: %f", to_main.freq_dist);
  glc_resize(to_main.glcanvas, 0);  

  return;
}

void
Void (FL_OBJECT *ob, long data) {

  return;
}

void
help_nav_cb(FL_OBJECT *ob, long data) {

  int line;
  int i;
  line = fl_get_browser(ob);
  fl_freeze_form(to_main.fd_help->help);
  fl_clear_browser(to_main.fd_help->help_disp);
  for (i = 0; help_arr[line-1][i]; i++) {
    fl_add_browser_line(to_main.fd_help->help_disp, help_arr[line-1][i]);
  }
  fl_unfreeze_form(to_main.fd_help->help);
  return;
}

void
print_frames(void) {

  int frame_from = 0;
  int frame_to = 0;

  /* get frames to save */
  if (fl_get_button(to_main.fd_print->frames_current)) {
    frame_from = to_main.curr_frame;
    frame_to = to_main.curr_frame;
  }
  else if (fl_get_button(to_main.fd_print->frames_all)) {
    frame_from = 0;
    frame_to = to_main.n_frame-1;
    fl_set_button(to_main.fd_print->frames_all, 0);
    fl_set_button(to_main.fd_print->frames_current, 1);
  }
  else if (fl_get_button(to_main.fd_print->frames_fr_to)) {
    frame_from = atoi(fl_get_input(to_main.fd_print->frames_from));
    frame_to = atoi(fl_get_input(to_main.fd_print->frames_to));
    fl_set_button(to_main.fd_print->frames_fr_to, 0);
    fl_set_button(to_main.fd_print->frames_current, 1);
  }
  printPS(frame_from, frame_to);
}

void
save_frames(void) {

  int frame_from = 0;
  int frame_to = 0;
/*=============================== alain modif ==================*/
  /*fprintf(stdout, "%%%% save frame %g %g %g %g\n",0.1, 0.1, 0.3 ,0.4);*/
/*=============================== alain modif ==================*/
  /* get frames to save */
  if (fl_get_button(to_main.fd_save_as->frames_current)) {
    frame_from = to_main.curr_frame;
    frame_to = to_main.curr_frame;
  }
  else if (fl_get_button(to_main.fd_save_as->frames_all)) {
    frame_from = 0;
    frame_to = to_main.n_frame-1;
    fl_set_button(to_main.fd_save_as->frames_all, 0);
    fl_set_button(to_main.fd_save_as->frames_current, 1);
  }
  else if (fl_get_button(to_main.fd_save_as->frames_fr_to)) {
    frame_from = atoi(fl_get_input(to_main.fd_save_as->frames_from));
    frame_to = atoi(fl_get_input(to_main.fd_save_as->frames_to));
    fl_set_button(to_main.fd_save_as->frames_fr_to, 0);
    fl_set_button(to_main.fd_save_as->frames_current, 1);
  }

  if (fl_get_button(to_main.fd_save_as->format_png)) {
    save_graphics_file(FORMAT_PNG, frame_from, frame_to);
  }
  else if (fl_get_button(to_main.fd_save_as->format_jpg)) {
    save_graphics_file(FORMAT_JPG, frame_from, frame_to);
  }
  else if (fl_get_button(to_main.fd_save_as->format_eps)) {
      outputEPS(frame_from, frame_to, fl_get_button(to_main.fd_save_as->bg));
      /*outputEPS(to_main.n_frame-1, to_main.n_frame-1, fl_get_button(to_main.fd_save_as->bg)); */ /* only the n last structures are computed */
  }
  else if (fl_get_button(to_main.fd_save_as->format_2dcoords)) {
    save_2d_coords(frame_from, frame_to);
  }
}

/*=========================Alain modif========================*/
void
save_frames_alain(char *savetype, int last) {
    
    int frame_from = 0;
    int frame_to = 0;

    if (last > to_main.n_frame)
	    last = to_main.n_frame;
    
    /* get frames to save */
    fprintf(stderr,"\t\t=>receive savetype:%s\n",savetype);

    if (strcmp(savetype,"eps")==0){
	fprintf(stderr,"passage par eps interp=%i\n",to_main.n_interp);
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	outputEPS_alain(frame_from, frame_to, 0);

    }else if (strcmp(savetype,"jpeg")==0){
	fprintf(stderr,"passage par jpg interp=%i\n",to_main.n_interp);
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_graphics_file(FORMAT_JPG, frame_from, frame_to);

    }else if(strcmp(savetype,"png")==0){
	fprintf(stderr,"passage par png interp=%i\n",to_main.n_interp);
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_graphics_file(FORMAT_PNG, frame_from, frame_to);
    
    }else if(strcmp(savetype,"coord")==0){
	fprintf(stderr,"passage par coord interp=%i\n",to_main.n_interp);
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_2d_coords(frame_from, frame_to);
    
    }else if(strcmp(savetype,"coord_jpeg")==0){
	fprintf(stderr,"passage par coord_jpeg\n");
	fprintf(stderr,"passage par jpg\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_graphics_file(FORMAT_JPG, frame_from, frame_to);
	fprintf(stderr,"passage par coord\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_2d_coords(frame_from, frame_to);

    }else if(strcmp(savetype,"coord_eps")==0){
	fprintf(stderr,"passage par coord_eps\n");
	fprintf(stderr,"passage par eps\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	outputEPS_alain(frame_from, frame_to, 0);
	fprintf(stderr,"passage par coord\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_2d_coords(frame_from, frame_to);
    
    }else if(strcmp(savetype,"coord_png")==0){
	fprintf(stderr,"passage par coord_png\n");
	fprintf(stderr,"passage par png\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_graphics_file(FORMAT_PNG, frame_from, frame_to);
	fprintf(stderr,"passage par coord\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_2d_coords(frame_from, frame_to);
    
    }else if(strcmp(savetype,"coord_lasteps")==0){
	fprintf(stderr,"passage par coord_lasteps\n");
	fprintf(stderr,"passage par eps\n");
	frame_from = to_main.n_frame-1;
	frame_to = to_main.n_frame-1;
	outputEPS_alain(frame_from, frame_to, 0);
	fprintf(stderr,"passage par coord\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_2d_coords(frame_from, frame_to);
	
    }else if(strcmp(savetype,"coord_lastjpeg")==0){
	fprintf(stderr,"passage par coord_lastjpeg\n");
	fprintf(stderr,"passage par jpg\n");
	frame_from = to_main.n_frame-1;
	frame_to = to_main.n_frame-1;
	save_graphics_file(FORMAT_JPG, frame_from, frame_to);
	fprintf(stderr,"passage par coord\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_2d_coords(frame_from, frame_to);

    }else if(strcmp(savetype,"coord_lastpng")==0){
	fprintf(stderr,"passage par coord_lastpng\n");
	fprintf(stderr,"passage par png\n");
	frame_from = to_main.n_frame-1;
	frame_to = to_main.n_frame-1;
	save_graphics_file(FORMAT_PNG, frame_from, frame_to);
	fprintf(stderr,"passage par coord\n");
	frame_from = to_main.n_frame-last;
	frame_to = to_main.n_frame-1;
	save_2d_coords(frame_from, frame_to);

    }else{
	fprintf(stderr,"problem\n");
    }
}


void
save_frames_alain_old(void) {

  int frame_from = 0;
  int frame_to = 0;
/*=============================== alain modif ==================*/
  /*fprintf(stdout, "%%%% save frame alain %g %g %g %g\n",0.1, 0.1, 0.3 ,0.4);*/
  if (to_main.fd_save_as->format_eps && to_main.fd_save_as->frames_all)
      fprintf(stdout, "(0)%i\n",to_main.n_frame-1);
  fprintf(stdout, "(1)%i\n",to_main.n_frame-1);
/*=============================== alain modif ==================*/
  /* get frames to save */
  if (to_main.fd_save_as->frames_current) {
    frame_from = to_main.curr_frame;
    frame_to = to_main.curr_frame;
    fprintf(stdout, "(faux 1)%i\n",to_main.n_frame-1);
  }
  else if (to_main.fd_save_as->frames_all) {
    frame_from = 0;
    frame_to = to_main.n_frame-1;
    fprintf(stdout, "(2)%i\n",to_main.n_frame-1);
    /*fl_set_button(to_main.fd_save_as->frames_all, 0);
      fl_set_button(to_main.fd_save_as->frames_current, 1);*/
  }
  /*else if (to_main.fd_save_as->frames_fr_to) {
    frame_from = atoi(fl_get_input(to_main.fd_save_as->frames_from));
    frame_to = atoi(fl_get_input(to_main.fd_save_as->frames_to));
    fl_set_button(to_main.fd_save_as->frames_fr_to, 0);
    fl_set_button(to_main.fd_save_as->frames_current, 1);
    }
  */

  if (to_main.fd_save_as->format_png) {
    save_graphics_file(FORMAT_PNG, frame_from, frame_to);
    fprintf(stdout, "(faux 2)%i\n",to_main.n_frame-1);
  }
  else if (to_main.fd_save_as->format_jpg) {
    save_graphics_file(FORMAT_JPG, frame_from, frame_to);
    fprintf(stdout, "(faux 2)%i\n",to_main.n_frame-1);
  }
  else if (to_main.fd_save_as->format_eps) {
      /*-------alain modif ------*/
      if (to_main.fd_save_as->bg)
	  outputEPS_alain(frame_from, frame_to, 1);
      else
	  outputEPS_alain(frame_from, frame_to, 0);
      fprintf(stdout, "(3)%i\n",to_main.n_frame-1);
  }
  else if (to_main.fd_save_as->format_2dcoords) {
    save_2d_coords(frame_from, frame_to);
    fprintf(stdout, "(faux 3)%i\n",to_main.n_frame-1);
  }
}
/*=========================Alain modif========================*/


/* Stuff for Anette Sander */
void
save_2d_coords(int frame_from, int frame_to) {

    int fr_i, b_i, struct_no;
    struc_t *the_struc = NULL;
    char *fn;
    int length;
    int i;
    FILE *fptr = NULL;
    int * temppsd = NULL;
    int tb_i;

    length = strlen(to_main.filename);

    /* malloc 11 made up from '_%05d.', suffix and \0 */
    fn = (char *) malloc((11+length)*sizeof(char));
    if (fn == NULL) {
	fprintf(stderr, "failed to malloc...\n");
	exit(1);
    }

    strcpy(fn, to_main.filename);
    
    i=length;
    while ((i>0) && (fn[i-1]!='.')) {
	fn[i-1]='\0';
	i--;
    }
    fn[i-1]='\0';

    if (fn[0] == '\0') 
	strcpy(fn, to_main.filename);

    strcat(fn, ".coord");

    fptr = fopen(fn, "w");
    if(fptr == NULL) {
	set_comment(to_main.fd_main->comment, FL_RED,
		    "failed to open file: %s",basename(fn));
        if(temppsd != NULL)
          free(temppsd);
	return;
    }

    set_comment(to_main.fd_main->comment, FL_BLUE,
		"writing file: %s ...",basename(fn));

    if(to_main.title != NULL)
	fprintf(fptr, "Title: %s\n", to_main.title);
    else
        fprintf(fptr, "Title: RNAmovie\n");

    fprintf(fptr, "numStructures: %d\n", frame_to - frame_from + 1);
    for (fr_i=frame_from; fr_i<=frame_to; fr_i++) {
	the_struc = to_main.movie[fr_i];
	if(!the_struc) {
	    set_comment(to_main.fd_main->comment, FL_RED,
			"Found invalid structure!!!");
            if(temppsd != NULL)
              free(temppsd);
	    return;
	}
	struct_no = fr_i/(to_main.n_interp+1);
	fprintf(fptr, "Structure: %d # %s\n", the_struc->length,the_struc->info);
	/*fprintf(fptr, "Structure: %d \n", the_struc->length);*/ /*change 20.06.02*/
	/* change 27/05/02 */
	/* Base    Bindung   X             Y         psd
	   G:      121       -1.081757     3.205374      */

        temppsd = realloc(temppsd, sizeof(int) * the_struc->length);
        if(temppsd == NULL) {
          fprintf(stderr, "failed to realloc temppsd\n");
          exit(1);
        }
	for (b_i=0; b_i < the_struc->length; b_i++) {
	    if (the_struc->bases[0].psd !=0)
		temppsd[0]=the_struc->bases[0].psd;
	    else
		temppsd[0]=-1;
	    for (tb_i=1; tb_i < the_struc->length-1; tb_i++){
		if (the_struc->bases[tb_i].psd !=0 && (the_struc->bases[tb_i-1].psd==0 || the_struc->bases[tb_i+1].psd==0))
		    temppsd[tb_i]=the_struc->bases[tb_i].psd;
		else
		    temppsd[tb_i]=-1;
	    }
	    if(the_struc->bases[the_struc->length-1].psd !=0)
		temppsd[the_struc->length-1]=the_struc->bases[the_struc->length-1].psd;
	    else
		temppsd[the_struc->length-1]=-1;


	    fprintf(fptr, "%c: %5d %12f %12f %5d %5d\n",
		    the_struc->bases[b_i].name,
		    the_struc->bases[b_i].mate + 1,
		    the_struc->bases[b_i].x,
		    the_struc->bases[b_i].y,
		    temppsd[b_i] + 1,
		    (the_struc->bases[b_i].type == KNOT));
	    
	}
    }
    fclose(fptr);
    free(fn);
    free(temppsd);

    set_comment(to_main.fd_main->comment, FL_BLUE,
		"writing file: %s ... done",fn);
}


char *
generate_save_filename(int frame, char suffix[3]) {
    /* char *fn;
       int length;
       int i;

       length = strlen(to_main.filename);*/

    /* malloc 11 made up from '_%05d.', suffix and \0 */
    /*  fn = (char *) malloc((11+length)*sizeof(char));
	if (fn == NULL) {
	fprintf(stderr, "failed to malloc...\n");
	exit(1);
	}

	strcpy(fn, to_main.filename);

	i=length;
	while ((i>0) && (fn[i-1]!='.')) {
	fn[i-1]='\0';
	i--;
	}
	fn[i-1]='\0';

	if (fn[0] == '\0') 
	strcpy(fn, to_main.filename);

	sprintf(fn, "%s_%05d.%s", fn, frame, suffix);

	return fn; */


    char *fn;
    int length;
/*-----*/
    char suffix2[4];

    memcpy(suffix2,suffix,3);
    suffix2[3]='\0';  /*added 14.05.2002*/
 
    /* 11 made up from '_%05d.', suffix and \0 */
    length = strlen(fl_get_input(to_main.fd_save_as->pathname)) +
	strlen(fl_get_input(to_main.fd_save_as->prefix)) + 30;
  
    fn = (char *) malloc(length*sizeof(char));
    if (fn == NULL) {
	fprintf(stderr, "failed to malloc...\n");
	exit(1);
    }
    /*sprintf(fn, "%s/%s_%05d.%s",
      fl_get_input(to_main.fd_save_as->pathname),
      fl_get_input(to_main.fd_save_as->prefix),
      frame,
      suffix);*/ /*change 14.05.2002*/

	/* added by Thomas on 02/07/2002 : feature -name : set the base name when generating a sequence of images*/
	if(g_basename == NULL)
	{
	    sprintf(fn, "%s/%s_%05d.%s",
		    fl_get_input(to_main.fd_save_as->pathname),
	    	fl_get_input(to_main.fd_save_as->prefix),
		    frame,
		    suffix2);
	}
	else
	{
		sprintf(fn, "%s/%s_%05d.%s",
		    fl_get_input(to_main.fd_save_as->pathname),
	    	g_basename,
		    frame,
		    suffix2);
	}
    return fn;
}

/*=========================Alain modif========================*/
char *
generate_save_filename_alain(int frame, char suffix[3]) {
  char *fn;
  int length;
  char *filename;
  char suffix2[4];

  memcpy(suffix2,suffix,3);
  suffix2[3]='\0';

  filename = strdup(to_main.filename);
  fl_set_input(to_main.fd_save_as->prefix, file_prefix(basename(filename)));
  fl_set_input(to_main.fd_save_as->pathname, dirname(filename));
  
  /* 11 made up from '_%05d.', suffix and \0 */
  length = strlen(fl_get_input(to_main.fd_save_as->pathname)) +
      strlen(fl_get_input(to_main.fd_save_as->prefix)) + 30;
  
  fn = (char *) malloc(length*sizeof(char));
  if (fn == NULL) {
      fprintf(stderr, "failed to malloc...\n");
      exit(1);
  }
  
  	/* added by Thomas on 02/07/2002 : feature -name : set the base name when generating a sequence of images*/
	if(g_basename == NULL)
	{
	    sprintf(fn, "%s/%s_%05d.%s",
		    fl_get_input(to_main.fd_save_as->pathname),
	    	fl_get_input(to_main.fd_save_as->prefix),
		    frame,
		    suffix2);
	}
	else
	{
		sprintf(fn, "%s/%s_%05d.%s",
		    fl_get_input(to_main.fd_save_as->pathname),
	    	g_basename,
		    frame,
		    suffix2);
	}
    return fn;
}

const char *int2str(int i) {

  static char str[20];

  sprintf(str, "%d", i);
  return str;
}


char *
file_prefix(const char *filename) {

  char *prefix;
  int i;

  prefix = strdup(filename);
  i = strlen(filename);

  while ((i>0) && (prefix[i-1]!='.')) {
    prefix[i-1]='\0';
    i--;
  }
  prefix[i-1]='\0';

  if (prefix[0] == '\0') 
    strcpy(prefix, filename);

  return prefix;
}
