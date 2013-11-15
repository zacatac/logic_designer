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
  GLfloat bb_color[3];
  GLfloat mate_color[3];
/*-----pseudoknots---*/
    int psd ;
} base_t;

typedef struct {
  char *name;
  char *info;
  float free_energy;
  int pause;
  base_t *bases;
  int length;
  int ss;
  int valid_dl;
  GLuint disp_list;
/*------pull---------*/
    int pull ;
} struc_t;

typedef struct color {
  int color_no;
  GLfloat rgb[3];
} color_t;

typedef struct {
  FD_main *fd_main;
  FD_help *fd_help;
  FD_fontselect *fd_fontselect;
  FD_about *fd_about;
  FD_brucc_opt *fd_brucc_opt;
  FD_color_opt *fd_color_opt;
  FD_interp *fd_interp;
  color_t col_set[12];
  FD_log *fd_logbrowser;
  FD_save_as *fd_save_as;
  FD_print *fd_print;
  FL_OBJECT *glcanvas;
  GLXContext glxcontext;
  char *filename;
  FILE *fileptr;
  FL_OBJECT *comment;
  FL_OBJECT *timeout;
  GLfloat zoom;
  GLfloat zoom_old;
  GLdouble x_displacement;
  GLdouble y_displacement;
  GLdouble rotation;
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
  GLfloat label_dist;
  GLfloat freq_dist;
  GLfloat strip_width ;
  long framerate;
} BP_main;

typedef struct {
  char *bases;
  int  *index;
  int  *types;
  int pull;
  int pause;
  char info[200];
} rnastruc;

typedef struct {
  int lsize;
  int lstyle;
} font_t;

typedef struct {
  int bases;
  int colored;
    int spline ;
  int quads;
  int noback;
  int dots;
  int none;
  int gu;
  int prime5;
  int prime3;
  int freq;
} brucc_t;

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

typedef struct dim {
  GLfloat width;
  GLfloat height;
} fontdim_t;

/* viewmain.c */
extern BP_main to_main;

/* added by Thomas on 02/07/2002 */
int g_quality;		/* the quality for JPG file */
int g_xsize;
int g_ysize;
int g_comments;
char* g_basename;

/* fill_viewforms_cb.c */
extern GLuint dlstart;
extern XFontStruct *xfs;
extern font_t font;
extern fontdim_t fdimlist[256];
extern brucc_t brucc;

/* viewparse.y */
/*extern int rnalen;
extern int rnalen2;
extern double bstep;
extern double gstep;
extern double crad;
extern GLdouble * circle;*/

/* viewscan.lex */
extern int basecount;

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

/* graphics formats */
#define FORMAT_PNG 0
#define FORMAT_JPG 1

/* allocation increment */
#define ALLOC_INC 4095

/* automatic save */
#define AUTOMATIC_SAVE FALSE

int glc_Expose(FL_OBJECT *, Window, int, int, XEvent *, void *);
int glc_MotionNotify(FL_OBJECT *, Window, int, int, XEvent *, void *);
int glc_EnterNotify(FL_OBJECT *, Window, int, int, XEvent *, void *);
int glc_LeaveNotify(FL_OBJECT *, Window, int, int, XEvent *, void *);
int glc_ButtonPress(FL_OBJECT *, Window, int, int, XEvent *, void *);
int glc_ButtonRelease(FL_OBJECT *, Window, int, int, XEvent *, void *);
int glc_ResizeRequest(FL_OBJECT *, Window, int, int, XEvent *, void *);
GLuint glc_SetFontDL(GLuint dl);
void glc_SetFont(int, int);
void glc_Print(GLfloat x, GLfloat y, const char *fmt, ...);
void open_cb(FL_OBJECT *, long);
void open_file(void);
void set_comment(FL_OBJECT *, FL_COLOR, const char *, ...);
void update_glc(FL_OBJECT *, long);
void display_struc(FL_OBJECT *, rnastruc *);
void glc_resize(FL_OBJECT *ob, long data);
void glc_expose(FL_OBJECT *ob, long data);
void disp_circle_graph(FL_OBJECT * ob, rnastruc * struc);
double * gen_circle_lut(size_t points, double radius, double * lut);
void disp_circle(size_t points, double * lut);
void disp_radial_graph(FL_OBJECT * ob, rnastruc * struc);
void disp_squiggle_plot(FL_OBJECT * ob, rnastruc * struc);
int gl_initialize(void);
void gl_resize(GLint width, GLint height);
void update_cb(FL_OBJECT *ob, long data);
void empty_cb(FL_OBJECT *ob, long data);
int naview(void);
void set_font_choice(int lstyle, int lsize);
void set_options_choice(void);
void set_options(void);
void cancel_options_save(FL_OBJECT *ob, long data);
char *file_prefix(const char *filename);
/*------alain modif------*/
char * initOptions(int argc, char **argv, int *);
/*----------------------*/


void convert_struc(rnastruc * struc);
struc_t *add_movie_struc(rnastruc *struc);
void free_struc_list(void);
void new_movie(void);
void free_movie(void);
void show_frame(void);
void pause_movie(void);
void play_movie(void);
void forward_frame(void);
void forward_movie(void);
void backward_frame(void);
void backward_movie(void);
void set_movie(int frame);
int parse_strucs(int strucs);
void save_graphics_file(int format, int frame_from, int frame_to);
/*------alain modif------*/
void save_jpeg_file(int frame_from, int frame_to);
/*-----------------------*/
void parse_err(int s_i, int pos);
char * generate_save_filename(int frame, char suffix[3]);
/*------alain modif------*/
char * generate_save_filename_alain(int frame, char suffix[3]);
/*----------------------*/
const char *int2str(int i);

struc_t *layout_naview(struc_t *the_struc);
struc_t *compile_struc(struc_t *the_struc, GLenum mode);
void label_pos(struc_t *the_struc, GLfloat dist);

void save_frames(void);
/*------alain modif------*/
void save_frames_alain(char *,int);
void outputEPS_alain(int frame_from, int frame_to, int bg);
/*----------------------*/
void outputEPS(int frame_from, int frame_to, int bg);
void printPS(int frame_from, int frame_to);

/* void spewWireFrameEPS(FILE * file, int doSort, GLint size, GLfloat * buffer,
		      char *creator, int bg);
*/
int count_strucs(FILE *fileptr);

void generate_font(FL_OBJECT *ob, fontdim_t *flist, Font font, int first, int count, int listbase);

















































































