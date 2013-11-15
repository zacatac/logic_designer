/*
-------------------------------------------------------------------------------

 RNA secondary structure parser
 OpenGL movie C file

 Author: Dirk Evers
         Technische Fakultaet
         Arbeitsgruppe Praktische Informatik
         Universitaet Bielefeld
         dirk@TechFak.Uni-Bielefeld.DE
                                      
 Date: 05.06.96

 RCS-Id:
 $Id: movie.c,v 1.1 1999/11/02 14:29:38 dirk Exp dirk $

 RCS-Log:
 $Log: movie.c,v $
 Revision 1.1  1999/11/02 14:29:38  dirk
 Initial revision

 Revision 1.4  1999/08/27 16:49:59  dirk
 first configure version basepair bug fixed

 Revision 1.2  1999/08/12 13:20:37  dirk
 fixd the notorious incremental length bug!

 Revision 1.1  1999/08/12 09:43:29  dirk
 Initial revision

 Revision 1.3  1999/01/11 17:19:19  asczyrba
 *** empty log message ***

 Revision 1.2  1998/11/05 15:15:12  asczyrba
 progress display included

 Revision 1.1  1998/11/04 08:14:25  asczyrba
 Initial revision

 Revision 1.4  1998/11/03 13:23:02  dirk
 can't remember

 Revision 1.3  1998/06/18 16:03:53  dirk
 pserockas version GK Begehung

 Revision 1.2  1997/09/12 11:00:49  dirk
 split off parse_marc in simple_parse.c

 Revision 1.1  1997/05/02 11:11:41  dirk
 Initial revision


-------------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include <forms.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "viewforms.h"
#include "dberror.h"
#include "viewglobal.h"


void interp_bases_content(int start, int end);
void interp_bases_content_xaya(int start, int end);
void copy_movie_struc(int from, int to);
void interp_struc_layout(int start, int end);
void forward_cb(int id, void *user_data);
void play_cb(int id, void *user_data);
void backward_cb(int id, void *user_data);

int timeout_id;
int tflag = FALSE;

struc_t *
add_movie_struc(rnastruc *struc) {

  struc_t *the_struc;
  int i;
  to_main.struc_list = (struc_t **) realloc(to_main.struc_list,
				       ++to_main.n_struc * sizeof(struc_t));
  if(to_main.struc_list == NULL)
    dbError(DB_TRUE, "Failed to realloc in add_movie_struc");

  to_main.struc_list[to_main.n_struc-1] = the_struc = (struc_t *) malloc(sizeof(struc_t));
  if(the_struc == NULL)
    dbError(DB_TRUE,"Failed to malloc in add_movie_struc");


  the_struc->name = NULL;
  the_struc->free_energy = NO_E;
  the_struc->pause = struc->pause;
  the_struc->valid_dl = FALSE;
  the_struc->disp_list = NO_DL;
  the_struc->length = strlen(struc->bases);

#if(VERBOSE > -7)
  printf("longueur de la structure lue : %d\n",the_struc->length);

#endif
  the_struc->pull = struc->pull ;

  if(the_struc->length == 0) {
    fl_show_alert("Structure", " ", "has zero elements", TRUE);
    return NULL;
  }

  the_struc->bases = (base_t *) malloc(the_struc->length * sizeof(base_t));
  if(the_struc->bases == NULL)
    dbError(DB_TRUE,"Failed to malloc in add_movie_struc");

  for(i=0;i<the_struc->length;i++) {
    the_struc->bases[i].x = NO_VAL;
    the_struc->bases[i].y = NO_VAL;
    the_struc->bases[i].name = struc->bases[i];
    the_struc->bases[i].hist_num = i+1;
    the_struc->bases[i].free_energy = NO_E;
    the_struc->bases[i].bb_color[0] = NO_COL;
    the_struc->bases[i].bb_color[1] = NO_COL;
    the_struc->bases[i].bb_color[2] = NO_COL;
    the_struc->bases[i].mate_color[0] = NO_COL;
    the_struc->bases[i].mate_color[1] = NO_COL;
    the_struc->bases[i].mate_color[2] = NO_COL;
    the_struc->bases[i].type = struc->types[i];
    if(struc->types[i] != SS &&
       struc->types[i] != HL &&
       struc->types[i] != TL &&
       struc->types[i] != BLL &&
       struc->types[i] != BLR &&
       struc->types[i] != SP){
      the_struc->bases[i].mate = struc->index[i];
      the_struc->bases[i].psd = 0;
    }
    else {
	if(struc->types[i] == SP) {
	    the_struc->bases[i].psd = struc->index[i];
	}
	else
	     the_struc->bases[i].psd = 0;
	the_struc->bases[i].mate = NO_MATE;
    }
  }

  /* copy comments */
  the_struc->info = strdup(struc->info);

#if (VERBOSE > -5)
  printf("fin de movies.c   => ");
#endif

  dbDebug(DB_DEBUG, "to_main.n_struc: %d", to_main.n_struc);
  return the_struc;
}


void
free_struc_list(void) {
  
  int i;
  
  dbDebug(DB_DEBUG, "->free_struc_list()");
  
  /*  if(to_main.movie!=NULL) */
  free_movie();
 
  if(to_main.n_struc > 0 && to_main.struc_list != NULL) {
    for(i=0;i<to_main.n_struc;i++) {
      free(to_main.struc_list[i]->bases);
      if(glIsList(to_main.struc_list[i]->disp_list)==GL_TRUE)
	glDeleteLists(to_main.struc_list[i]->disp_list, 1);
      free(to_main.struc_list[i]);
    }
    free(to_main.struc_list);
  }
  to_main.struc_list = NULL;

  to_main.zoom = 0.0;
  to_main.x_displacement = 0.0;
  to_main.y_displacement = 0.0;
  to_main.rotation = 0.0;

  to_main.n_struc = 0;  
  to_main.n_base = 0;

  dbDebug(DB_DEBUG, "<-free_struc_list()");
  
  return;
}

void
new_movie(void) {

  int i;
  int j;
  int percent = 10;
  int xayastruc=0;

  dbDebug(DB_DEBUG, "->new_movie()");

  set_comment(to_main.comment, FL_BLUE, "Please wait while calculating frames...");

  if(to_main.n_struc==0 || to_main.struc_list==NULL) {
    set_movie(0);
    return;
  }

  to_main.n_frame = (to_main.n_struc - 1) * to_main.n_interp + to_main.n_struc;
  fl_set_slider_bounds(to_main.fd_main->movie_slider, 0.0, to_main.n_frame-1);
  dbDebug(DB_DEBUG, "n_frame: %d", to_main.n_frame);

  to_main.movie = (struc_t **) calloc(to_main.n_frame, sizeof(struc_t *));
  if(to_main.movie==NULL)
    dbError(DB_TRUE, "failed to calloc in new_movie()");

  for(i=0,j=0; i<to_main.n_frame; i+=to_main.n_interp+1,j++) {
    dbDebug(DB_DEBUG, "i: %d j: %d", i, j);
    to_main.movie[i] = to_main.struc_list[j];
    to_main.movie[i]->valid_dl = FALSE;
  }

  if(to_main.n_interp==0) {
    glc_resize(to_main.glcanvas, 0);
    set_movie(0);
    return;
  }

  /* very strange continuation criterion!!!*/
  for(i=0;i<to_main.n_frame-to_main.n_interp; i+=(to_main.n_interp+1)) {
    dbDebug(DB_DEBUG, "i: %d", i);
    xayastruc++;
    if(to_main.movie[i]->length <= to_main.movie[i+to_main.n_interp+1]->length) { /*new synthesis ? */
    	for(j=i+1;j<(i+to_main.n_interp+1);j++) {
	    /* calculate percentage of frames done */
	    if ((j*100 / to_main.n_frame) >= percent) {
		set_comment(to_main.comment, FL_BLUE, 
			    "%d %% of frames done...", percent);
		percent += 10;
	    }
	    
	    dbDebug(DB_DEBUG, "malloc movie[%d]", j);
	    to_main.movie[j] = (struc_t *) calloc(1, sizeof(struc_t));
	    if(to_main.movie[j]==NULL)
		dbError(DB_TRUE, "failed to malloc in new_movie()");
	}
	interp_bases_content(i, i+to_main.n_interp+1); 
	interp_struc_layout(i, i+to_main.n_interp+1);
    }
    else{ /*new synthesis => replace intermidiate by the same structure */
	for(j=i+1;j<(i+to_main.n_interp+1);j++){
            /* calculate percentage of frames done */
	    if ((j*100 / to_main.n_frame) >= percent) {
		set_comment(to_main.comment, FL_BLUE, 
			    "%d %% of frames done...", percent);
		percent += 10;
	    }
	    to_main.movie[j] = (struc_t *) calloc(1, sizeof(struc_t));
/*	    to_main.movie[j] = to_main.struc_list[xayastruc];*/
#if (VERBOSE > -5)
    printf("\n *******************\n result %d \n", xayastruc);
#endif
	}
	/*interp_bases_content_xaya(i, i+to_main.n_interp+1);*/
	interp_bases_content(i, i+to_main.n_interp+1);
	interp_struc_layout(i, i+to_main.n_interp+1);
    }
  }
  glc_resize(to_main.glcanvas, 0);
  to_main.curr_frame = 0;
  set_movie(0);
  
  set_comment(to_main.comment, FL_BLUE, "Total: %d Structures, %d Frames",
	      to_main.n_struc, to_main.n_frame);
  
  dbDebug(DB_DEBUG, "<-new_movie()");
  
  return;
}

void
free_movie(void) {

  int i,j;

  dbDebug(DB_DEBUG, "->free_movie()");

  if(to_main.movie!=NULL) {
    for(i=0;i<to_main.n_frame-to_main.n_interp; i+=to_main.n_interp+1) {
      for(j=i+1;j<i+to_main.n_interp+1;j++) {
	if(to_main.movie[j]!=NULL) {
          free(to_main.movie[j]->info);
	  if(to_main.movie[j]->bases!=NULL)
	    free(to_main.movie[j]->bases);
	  if(glIsList(to_main.movie[j]->disp_list)==GL_TRUE)
	    glDeleteLists(to_main.movie[j]->disp_list, 1);
	  free(to_main.movie[j]);
	}
      }
    }
    free(to_main.movie);
  }

  to_main.movie = NULL;
  to_main.n_frame = 0;
  to_main.movie_end = FALSE;
  to_main.movie_start = FALSE;

  to_main.curr_frame = NO_FR;

  dbDebug(DB_DEBUG, "<-free_movie()");
  
  return;
}

void
interp_bases_content(int start, int end) {
  
  int i, j,k;
  int mid = (end - start) / 2 + start;
  
  dbDebug(DB_DEBUG, "->interp_bases_content()");
  
  /* anything to interpolate? */
  if((end - start) < 2)
    return;
  
  /* start to mid (inclusive) */
  for(i=start+1;i<=mid;i++) {
    copy_movie_struc(start, i);
  }
  /* mid + 1 to end */
  for(;i<end;i++) {
    copy_movie_struc(end, i);
    /* delete mates in intermediate structures that pair to new bases */
    for(j=to_main.movie[start]->length;j<to_main.movie[i]->length;j++) {
      int mate = to_main.movie[i]->bases[j].mate;
      if(mate!=NO_MATE) {
	to_main.movie[i]->bases[mate].mate = NO_MATE;
	to_main.movie[i]->bases[j].mate = NO_MATE;
      }
    }
    /* set length to length of first structure so that backbone will not
       be drawn */
    k=to_main.movie[start]->length;
    if(k>to_main.movie[end]->length) k=to_main.movie[end]->length;
    to_main.movie[i]->length = k;
    /*to_main.movie[i]->length = to_main.movie[start]->length;*/
  }
  

  /* old working code start */
  /*  for(i=start+1;i<end;i++) {
    memcpy(to_main.movie[i], to_main.movie[start], sizeof(struc_t));
    to_main.movie[i]->valid_dl = FALSE;
    to_main.movie[i]->disp_list = NO_DL;
    to_main.movie[i]->bases = NULL;
    to_main.movie[i]->bases = (base_t *) malloc(to_main.movie[i]->length * sizeof(base_t));
    if(to_main.movie[i]->bases==NULL)
	dbError(DB_TRUE, "failed to malloc in interp_bases_content()");
    if(((end-start)/2+start)>i)
	memcpy(to_main.movie[i]->bases, to_main.movie[start]->bases, 
	       sizeof(base_t) * to_main.movie[i]->length);
    else
	memcpy(to_main.movie[i]->bases, to_main.movie[end]->bases, 
	       sizeof(base_t) * to_main.movie[i]->length);
    
        if(to_main.movie[i]->bases->mate > to_main.movie[i]->length) {
	to_main.movie[i]->bases->mate=0;
	} 
    for(j=to_main.movie[start]->length;j<to_main.movie[i]->length;j++) {
	int mate = to_main.movie[i]->bases[j].mate;
	to_main.movie[i]->bases[mate].mate = NO_MATE;
	to_main.movie[i]->bases[j].mate = NO_MATE;
    }
  } */
 /* end */

  dbDebug(DB_DEBUG, "<-interp_bases_content()");

  return;
}


void
interp_bases_content_xaya(int start, int end) {
  
  int i;
  
  /* anything to interpolate? */
  if((end - start) < 2)
    return;
  
  /* start to mid (inclusive) */
  for(i=start+1;i<end;i++) {
    /*for(i=start+1;i<=end;i++) { bug!*/
    copy_movie_struc(end, i);
  }
  return;
}

void copy_movie_struc(int from, int to) {
  /* `to' is expected to be malloced and uninitialised */

  memcpy(to_main.movie[to], to_main.movie[from], sizeof(struc_t));
  to_main.movie[to]->valid_dl = FALSE;
  to_main.movie[to]->disp_list = NO_DL;
  to_main.movie[to]->pause = FALSE;
  to_main.movie[to]->bases = NULL;
  to_main.movie[to]->bases = (base_t *) calloc(to_main.movie[to]->length,
					       sizeof(base_t));
  if(to_main.movie[to]->bases==NULL)
    dbError(DB_TRUE, "failed to calloc in interp_bases_content()");

  memcpy(to_main.movie[to]->bases, to_main.movie[from]->bases, 
	 sizeof(base_t) * to_main.movie[to]->length);

  to_main.movie[to]->info = strdup(to_main.movie[from]->info);

  return;
}

void
interp_struc_layout(int start, int end) {

  int i, j,k;
  float step;
  float dx, dy;
  float x, y;
  float xinc, yinc;

  dbDebug(DB_DEBUG, "->interp_struc_layout()");

  step = end - start;

  k=to_main.movie[start]->length;
  if(k>to_main.movie[end]->length) k=to_main.movie[end]->length;
  for(i=0;i<k;i++) {
  /*for(i=0;i<to_main.movie[start]->length;i++) {*/
    dx = to_main.movie[end]->bases[i].x - to_main.movie[start]->bases[i].x;
    dy = to_main.movie[end]->bases[i].y - to_main.movie[start]->bases[i].y;
    if(dx==0.0 && dy==0.0)
      continue; /* nothing todo */
    xinc = dx/step;
    yinc = dy/step;
    x = to_main.movie[start]->bases[i].x + xinc;
    y = to_main.movie[start]->bases[i].y + yinc;
    for(j=start+1;j<end;j++) {
      to_main.movie[j]->bases[i].x = x;
      to_main.movie[j]->bases[i].y = y;
      x= x + xinc;
      y= y + yinc;
   }
  }

  dbDebug(DB_DEBUG, "<-interp_struc_layout()");

  return;
}

void
play_cb(int id, void *user_data) {

  static int pause_flag = FALSE;

  dbDebug(DB_DEBUG, "->play_cb()");

  /* check for pause */
  if(to_main.movie[to_main.curr_frame]->pause && (!pause_flag)) {
    tflag = FALSE;
    pause_flag = TRUE;
    /* if there is a comment, we do not display "Pause" */
    if(!strlen(to_main.movie[to_main.curr_frame]->info)) {
      set_comment(to_main.comment, FL_BLUE, "Pause");
    }
    return;
  }
  else
    pause_flag = FALSE;

  if(!to_main.movie_end) {
    forward_frame();
    timeout_id = fl_add_timeout(to_main.framerate, play_cb, NULL);
    tflag = TRUE;
  }
  else tflag = FALSE;

  dbDebug(DB_DEBUG, "<-play_cb()");

  return;
}

void
forward_cb(int id, void *user_data) {

  dbDebug(DB_DEBUG, "->forward_cb()");

  if(!to_main.movie_end) {
    forward_frame();
    if((to_main.curr_frame % (to_main.n_interp+1))!=0) {
      timeout_id = fl_add_timeout(to_main.framerate, forward_cb, NULL);
      tflag = TRUE;
    }
    else tflag = FALSE;
  }

  dbDebug(DB_DEBUG, "<-forward_cb()");

  return;
}

void
backward_cb(int id, void *user_data) {

  dbDebug(DB_DEBUG, "->backward_cb()");

  if(!to_main.movie_start) {
    backward_frame();
    if((to_main.curr_frame % (to_main.n_interp+1))!=0) {
      timeout_id = fl_add_timeout(to_main.framerate, backward_cb, NULL);
      tflag = TRUE;
    }
    else tflag = FALSE;
  }
  
  dbDebug(DB_DEBUG, "<-backward_cb()");

  return;
}

void
show_frame(void) {

  static char framebuf[20];
  static char strucbuf[20];
  int struc_no;

  /* clear the canvas */
  glClear(GL_COLOR_BUFFER_BIT);

  /* if there is a frame */
  if(to_main.curr_frame != NO_FR) {

    /* if there is a valid display list */
    if(to_main.movie[to_main.curr_frame]->valid_dl)
      /* display it */
      glCallList(to_main.movie[to_main.curr_frame]->disp_list);
    else 
      /* compile it and draw it */
	  compile_struc(to_main.movie[to_main.curr_frame], GL_COMPILE_AND_EXECUTE);
	
    
    /* compute the structure number */
    struc_no = to_main.curr_frame / (to_main.n_interp+1);

    /* are we in between structures? */
    if((to_main.curr_frame % (to_main.n_interp+1)) == 0) 
      sprintf(strucbuf, "%d", struc_no+1);
    else
      sprintf(strucbuf, "%d-%d", struc_no+1, struc_no+2);

    /* set comment */
    set_comment(to_main.comment, FL_BLUE, "%s",to_main.movie[to_main.curr_frame]->info);

    /* compute the frame number */
    sprintf(framebuf, "%d", to_main.curr_frame);

  }
  else {
    sprintf(framebuf, "None");
    sprintf(strucbuf, "None");

  }

  /* display the structure & frame number */
  fl_set_object_label(to_main.fd_main->struccount, strucbuf);
  fl_set_object_label(to_main.fd_main->framecount, framebuf);


  /* show the new frame */
  glXSwapBuffers(fl_display, FL_ObjWin(to_main.glcanvas));

  return;
}

void
pause_movie(void) {
  
  if(tflag == TRUE) {
    fl_remove_timeout(timeout_id);
    tflag = FALSE;
  }

  return;
}

void
play_movie(void) {

  pause_movie();
  if((to_main.movie != NULL) && (to_main.movie_end == FALSE)) {
    timeout_id = fl_add_timeout(to_main.framerate, play_cb, NULL);
    tflag = TRUE;
  }
  return;
}

void
forward_frame(void) {

  if(!to_main.movie_end) {
    to_main.curr_frame++;
    fl_set_slider_value(to_main.fd_main->movie_slider, to_main.curr_frame);
    show_frame();
    if(to_main.curr_frame == to_main.n_frame-1)
      to_main.movie_end = TRUE;
    if(to_main.curr_frame > 0)
      to_main.movie_start = FALSE;
  }

  return;
}

void
backward_frame(void) {

  if(!to_main.movie_start) {
    to_main.curr_frame--;
    fl_set_slider_value(to_main.fd_main->movie_slider, to_main.curr_frame);
    show_frame();
    if(to_main.curr_frame == 0)
      to_main.movie_start = TRUE;
    if(to_main.curr_frame < (to_main.n_frame-1))
      to_main.movie_end = FALSE;
  }

  return;
}
 
void
backward_movie(void) {

  pause_movie();
  if(to_main.movie!=NULL)
    timeout_id = fl_add_timeout(to_main.framerate, backward_cb, NULL);

  return;
}

void
forward_movie(void) {

  pause_movie();
  if(to_main.movie!=NULL)
    timeout_id = fl_add_timeout(to_main.framerate, forward_cb, NULL);

  return;
}

void
set_movie(int frame) {

  pause_movie();
  if(to_main.movie!=NULL) {

    if((frame >= 0) && (frame < to_main.n_frame)) {
      to_main.curr_frame = frame;
      fl_set_slider_value(to_main.fd_main->movie_slider, frame);
      show_frame();
      
      if(to_main.curr_frame == to_main.n_frame-1)
	to_main.movie_end = TRUE;
      if(to_main.curr_frame > 0)
	to_main.movie_start = FALSE;
      
      if(to_main.curr_frame == 0)
	to_main.movie_start = TRUE;
      if(to_main.curr_frame < (to_main.n_frame-1))
	to_main.movie_end = FALSE;
    }

  }  else {
    to_main.curr_frame = NO_FR;
    to_main.movie_end = FALSE;
    to_main.movie_start = FALSE;
    fl_set_slider_value(to_main.fd_main->movie_slider, 0);
  }
  
  return;
}
