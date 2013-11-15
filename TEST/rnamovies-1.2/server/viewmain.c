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
 $Id: viewmain.c,v 1.2 2003/07/16 14:17:12 jkrueger Exp $

-------------------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <strings.h>

#include "dberror.h"
#include "viewglobal.h"
#include "init.h"

int Button = FALSE;

BP_main to_main;

double x_prev, y_prev;

int main (int argc, char *argv[]) {

    setup_defaults();
    parse_args(&argc, argv);
    
    /* filename on commandline? */
    if(to_main.filename!=NULL) {
	open_file();
	save_2d_coords(0,to_main.n_frame-1);
    }
    else { /* no filename */
      printf("usage: %s [filename]\n", argv[0]);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
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
	printf("Unable to open file : %s\n",to_main.filename);
	free(to_main.filename);
	to_main.filename = NULL;
	to_main.fileptr = NULL;
	free_struc_list();
      return;
    }
    else {
#ifdef DEBUG
      printf("\nOpened file : %s\n",to_main.filename);
#endif
      to_main.fileptr = fileptr;
      free_struc_list();

      strucs = count_strucs(fileptr);
#ifdef DEBUG
      printf("\nFound %i structures\n",  strucs);
#endif
      if (parse_strucs(strucs)==0) {
	new_movie();
      }
    }
  }
  return;
}

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
      if(temppsd != NULL)
        free(temppsd);
      return;
    }

    if(to_main.title != NULL)
        fprintf(fptr, "Title: %s\n", to_main.title);
    else
        fprintf(fptr, "Title: RNAmovie\n");

    fprintf(fptr, "numStructures: %d\n", frame_to - frame_from + 1);
    for (fr_i=frame_from; fr_i<=frame_to; fr_i++) {
        the_struc = to_main.movie[fr_i];
        if(!the_struc) {
          if(temppsd != NULL)
            free(temppsd);
          return;
        }
        struct_no = fr_i/(to_main.n_interp+1);
        if(the_struc->info)
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
}

