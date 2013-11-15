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
 $Id: movie.c,v 1.2 2003/07/16 14:17:12 jkrueger Exp $

 RCS-Log:
 $Log: movie.c,v $
 Revision 1.2  2003/07/16 14:17:12  jkrueger
 add debug directive

 Revision 1.1  2003/07/11 11:28:43  jkrueger
 Initial revision

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
#include <stdio.h>
#include <strings.h>

#include "viewglobal.h"


void interp_bases_content(int start, int end);
void copy_movie_struc(int from, int to);
void interp_struc_layout(int start, int end);


int timeout_id;
int tflag = FALSE;

struc_t * add_movie_struc(rnastruc *struc) {

  struc_t *the_struc;
  int i;

  to_main.struc_list = (struc_t **) realloc(to_main.struc_list,
				       ++to_main.n_struc * sizeof(struc_t));
  if(to_main.struc_list == NULL) {
      printf("Failed to realloc in add_movie_struc");
  }
  
  to_main.struc_list[to_main.n_struc-1] = the_struc = (struc_t *) malloc(sizeof(struc_t));
  if(the_struc == NULL) {
      printf("Failed to malloc in add_movie_struc");
  }

  the_struc->name = NULL;
  the_struc->free_energy = NO_E;
  the_struc->valid_dl = FALSE;
  the_struc->length = strlen(struc->bases);
  if(the_struc->length == 0) {
      printf("Structure has zero elements");    
    return NULL;
  }
  the_struc->pull = struc->pull ;

  the_struc->bases = (base_t *) malloc(the_struc->length * sizeof(base_t));
  if(the_struc->bases == NULL) {
      printf("Failed to malloc in add_movie_struc");
  }

  for(i=0;i<the_struc->length;i++) {
    the_struc->bases[i].x = NO_VAL;
    the_struc->bases[i].y = NO_VAL;
    the_struc->bases[i].name = struc->bases[i];
    the_struc->bases[i].hist_num = i+1;
    the_struc->bases[i].free_energy = NO_E;
    the_struc->bases[i].type = struc->types[i];
    if(struc->types[i] != SS &&
       struc->types[i] != HL &&
       struc->types[i] != TL &&
       struc->types[i] != BLL &&
       struc->types[i] != BLR &&
       struc->types[i] != SP) {
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
  the_struc->info = strdup((char *)struc->info);


#ifdef DEBUG
  printf("to_main.n_struc: %d", to_main.n_struc);
#endif

  return the_struc;
}


void free_struc_list(void) {
  
  int i;

#ifdef DEBUG  
  printf("->free_struc_list()");
#endif
  
  /*  if(to_main.movie!=NULL) */
  free_movie();
 
  if(to_main.n_struc > 0 && to_main.struc_list != NULL) {
    for(i=0;i<to_main.n_struc;i++) {
      free(to_main.struc_list[i]->bases);
      free(to_main.struc_list[i]);
    }
    free(to_main.struc_list);
  }
  to_main.struc_list = NULL;

  
  to_main.n_struc = 0;  
  to_main.n_base = 0;

#ifdef DEBUG
  printf("<-free_struc_list()");
#endif
  
  return;
}

void new_movie(void) {

  int i;
  int j;

#ifdef DEBUG
  printf("->new_movie()\n");
  printf("new_movie : \tPlease wait while calculating frames...\n");
#endif

  if(to_main.n_struc==0 || to_main.struc_list==NULL) {
      printf("new_movie :\tsomething goes wrong ... \n");
      if (to_main.n_struc==0) { printf("\t\tto_main.n_struc==0\n");}
      if (to_main.struc_list==NULL) {printf("\t\tto_main.struc_list==NULL\n");}

    return;
  }

  to_main.n_frame = (to_main.n_struc - 1) * to_main.n_interp + to_main.n_struc;

#ifdef DEBUG
  printf("new_movie :\tn_frame: %d\n", to_main.n_frame);
#endif

  to_main.movie = (struc_t **) calloc(to_main.n_frame, sizeof(struc_t *));
  if(to_main.movie==NULL) {
      printf("new_movie : \tfailed to calloc in new_movie()\n");
  }

  for(i=0,j=0; i<to_main.n_frame; i+=to_main.n_interp+1,j++) {
      to_main.movie[i] = to_main.struc_list[j];
      to_main.movie[i]->valid_dl = FALSE;
  }

  if(to_main.n_interp==0) {
    return;
  }

  /* very strange continuation criterion!!!*/
  for(i=0;i<to_main.n_frame-to_main.n_interp; i+=(to_main.n_interp+1)) {
#ifdef DEBUG
      printf("new_movie :\t i: %d\n", i);
#endif    
    

      for(j=i+1;j<(i+to_main.n_interp+1);j++) {
#ifdef DEBUG
	  /* calculate percentage of frames done */
	  if ((j*100 / to_main.n_frame) >= percent) {
	      printf("new_movie :\t%d %% of frames done...\n", percent);
	      percent += 10;
	  }
	  printf("new_movie :\tmalloc movie[%d]\n", j);
#endif
	  to_main.movie[j] = (struc_t *) calloc(1, sizeof(struc_t));
	  if(to_main.movie[j]==NULL) {
	      printf("new_movie :\tfailed to malloc in new_movie()\n");
	  }
      }
      interp_bases_content(i, i+to_main.n_interp+1); 
      interp_struc_layout(i, i+to_main.n_interp+1);
  }


  to_main.curr_frame = 0;
 ;
#ifdef DEBUG
  printf("new_movie :\tTotal: %d Structures, %d Frames\n",to_main.n_struc, to_main.n_frame);
  printf("<-new_movie()\n");
#endif

  return;
}

void free_movie(void) {

  int i,j;
  
#ifdef DEBUG
  printf("->free_movie()\n");
#endif

  if(to_main.movie!=NULL) {
    for(i=0;i<to_main.n_frame-to_main.n_interp; i+=to_main.n_interp+1) {
      for(j=i+1;j<i+to_main.n_interp+1;j++) {
	if(to_main.movie[j]!=NULL) {
          free(to_main.movie[j]->info);
	  if(to_main.movie[j]->bases!=NULL)
	    free(to_main.movie[j]->bases);
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

#ifdef DEBUG
  printf("<-free_movie()\n");
#endif
 
  return;
}

void interp_bases_content(int start, int end) {
  
  int i, j;
  int mid = (end - start) / 2 + start;

#ifdef DEBUG  
  printf("->interp_bases_content()\n");
#endif
  
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
    to_main.movie[i]->length = to_main.movie[start]->length;
  }
  
#ifdef DEBUG
  printf("<-interp_bases_content()\n");
#endif

  return;
}

void copy_movie_struc(int from, int to) {
  /* `to' is expected to be malloced and uninitialised */

  memcpy(to_main.movie[to], to_main.movie[from], sizeof(struc_t));
  to_main.movie[to]->valid_dl = FALSE;
  

  to_main.movie[to]->bases = NULL;
  to_main.movie[to]->bases = (base_t *) calloc(to_main.movie[to]->length,
					       sizeof(base_t));
  if(to_main.movie[to]->bases==NULL){
      printf("copy_movie_struc :\tfailed to calloc in interp_bases_content()\n");
  }
  memcpy(to_main.movie[to]->bases, to_main.movie[from]->bases, 
	 sizeof(base_t) * to_main.movie[to]->length);

  return;
}

void interp_struc_layout(int start, int end) {

  int i, j;
  float step;
  float dx, dy;
  float x, y;
  float xinc, yinc;

#ifdef DEBUG
  printf("->interp_struc_layout()\n");
#endif

  step = end - start;

  for(i=0;i<to_main.movie[start]->length;i++) {
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
#ifdef DEBUG
  printf("<-interp_struc_layout()\n");
#endif

  return;
}






