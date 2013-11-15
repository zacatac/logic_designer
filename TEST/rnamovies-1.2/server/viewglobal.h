#include <stdio.h>
#include <stdlib.h> 

/* animation structures */

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE (! FALSE)
#endif

#define VERBOSE -9 /*default -5 : information on length and structure number*/
                   /*-3 all indications use for debugging, -7 indicate only new synthesis, -9 nothing*/

typedef struct {
  char name;
  int mate;
  int type;
  int hist_num;
  float free_energy;
  float x;
  float y;
  float lx,ly;
  int psd;
} base_t;

typedef struct {
  char *name;
  char * info;
  float free_energy;
  int pause;
  base_t *bases;
  int length;
  int ss;
  int valid_dl;
  int pull;
} struc_t;

typedef struct color {
  int color_no;
} color_t;

typedef struct {
  color_t col_set[12];
  char *filename;
  FILE *fileptr;
  int n_interp;
  int n_struc;
  int n_base;
  int n_frame;
  struc_t **struc_list;
  struc_t **movie;
  int curr_frame;
  int movie_end;
  int movie_start;
  char *title;
  long framerate;
} BP_main;

typedef struct {
  char *bases;
  int  *index;
  int  *types;
  int pull;
  int pause;
  char *info[200];
} rnastruc;

struct region {
  int start1,end1,start2,end2;
};

typedef struct base {
  char name;
  int mate,hist_num;
  float x,y;
  int extracted;
  struct region *region;
} bases_t;


/* viewmain.c */
extern BP_main to_main;



/* naview.c */
extern bases_t *bases;

/* movie.c */
extern int n_interp;
extern int n_struc;
extern struc_t ** structurelist;
extern struc_t ** movie;

/* defines */
#define even(a) ( !((a)%2) )
#define odd(a)  ( (a)%2 )
#define min2(a,b) ( (a) < (b) ? (a) : (b) )
#define max2(a,b) ( (a) > (b) ? (a) : (b) )

#define GLC_STRLEN 500

#define NO_E -99999.0
#define NO_DL 0
#define NO_VAL -99999
#define NO_COL -1.0
#define NO_MATE -1
#define NO_FR -1

#define	NUM	258
#define	BASE	259
#define	SS	260
#define	SR	261
#define	ISR	262
#define	HL	263
#define	TL	264
#define	BLL	265
#define	BLR	266
#define	IL	267
#define	ML	268

/* for pseudoknots */
#define SP      257
#define UNDEF   256
/* for knots change 14/02/04 new */
#define KNOT    255

/* allocation increment */
#define ALLOC_INC 4095


void open_file(void);
void save_2d_coords(int frame_from, int frame_to);
double * gen_circle_lut(size_t points, double radius, double * lut);
void disp_circle(size_t points, double * lut);
int naview(void);
void set_font_choice(int lstyle, int lsize);
void set_options_choice(void);
void set_options(void);
char *file_prefix(const char *filename);

void convert_struc(rnastruc * struc);
struc_t *add_movie_struc(rnastruc *struc);
void free_struc_list(void);
void new_movie(void);
void free_movie(void);
void show_frame(void);

void set_movie(int frame);
int parse_strucs(int strucs);
void save_gif_file(int frame_from, int frame_to);
void parse_err(int s_i, int pos);
char * generate_save_filename(int frame, char suffix[3]);
const char *int2str(int i);

struc_t *layout_naview(struc_t *the_struc);

void save_frames(void);

int count_strucs(FILE *fileptr);
