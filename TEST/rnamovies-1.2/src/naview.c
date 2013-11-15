/*
 *   NAVIEW -- A program to make a modified radial drawing of an RNA
 *   secondary structure. 
 *   
 *   Copyright (c) 1988 Robert E. Bruccoleri
 *   Copying of this software, in whole or in part, is permitted
 *   provided that the copies are not made for commercial purposes,
 *   appropriate credit for the use of the software is given, this
 *   copyright notice appears, and notice is given that the copying
 *   is by permission of Robert E. Bruccoleri. Any other copying
 *   requires specific permission. 
 *
 *   Usage: RADIAL ct-file PLT2-file
 *
 *          ct-file is produced by Zuker's RNA folding program.
 *          
 *   See R. Bruccoleri and G. Heinrich, Computer Applications in the
 *   Biosciences, 4, 167-173 (1988) for a full description.
 *   
 *   Michael Zuker has made the following changes :
 *
 *   bases[i].hist_num : This is the historical numbering of the ith
 *                       base in the original sequence. This number is
 *                       picked up as the last item of each record in
 *                       the .ct file.
 *
 *   istart : This is the first position to label on the plot. The
 *            historical number for this base is the smallest multiple
 *            of label_rate that is >= the historical number of the
 *            first base.
 */

/*#include "viewparse.tab.h"*/

#include <stdlib.h>
#include <math.h>
#include <forms.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "viewforms.h"
#include "dberror.h"
#include "viewglobal.h"

typedef int logical;

/*#define null 0*/
/*#define TRUE 1
  #define FALSE 0*/
#ifdef vms
#   define FATAL_ERROR 4
#   define SUCCESS 1
#else
#   define FATAL_ERROR 1
#   define SUCCESS 0
#endif

#define type_alloc(type) (type *) malloc(sizeof(type))

#define struct_alloc(structure_name) type_alloc(struct structure_name)

#define add_double_list(head,tail,newp) {\
  (newp)->next = (newp)->prev = NULL; \
  if ((head) == NULL) (head) = (tail) = (newp); \
  else { \
    (tail)->next = (newp); \
    (newp)->prev = (tail); \
    (tail) = (newp); \
  } \
}


static float pi = 3.141592653589793;
/* static double pid = 3.141592653589793; */
/* static float rad120 = (120.0/180.0*3.141592653589793); */
/* static float dtor = (3.141592653589793/180.0); */
static float bignum = 1.7e38;
static float anum = 9999.0;
/* static float eps1 = 1.0e-6; */
/* static int bigint = 2147483647; */

brucc_t brucc;
bases_t *bases;


/*
 *   Function data type definitions
 */

/*FILE *inf,*outf;*/

/*
  struct base {
  char name;
  int mate,hist_num;
  float x,y;
  logical extracted;
  struct region *region;
  } *bases;
  
  struct region {
  int start1,end1,start2,end2;
  };
  */	
struct loop {
    int nconnection;
    struct connection **connections;
    int number;
    int depth;
    logical mark;
    float x,y,radius;
};

struct connection {
    struct loop *loop;
    struct region *region;
    int start,end;	    /* Start and end form the 1st base pair of the region. */
    float xrad,yrad,angle;
    logical extruded;	    /* True if segment between this connection and
			       the next must be extruded out of the circle */
    logical broken;	    /* True if the extruded segment must be drawn long. */
};

int nbase,nregion,loop_count;

struct loop *anchor,*root,*loops;

struct region *regions;

/* extern struct loop *construct_loop(); */

struct radloop
{
    float radius;
    int loopnumber;
    struct radloop *next,*prev;
};

struct radloop *rlphead,*rlptail;

char *dev;

float xsize,ysize;

float lencut;

logical mark_loops,draw_bases;

float csz;

int label_rate;

logical dot_pairs;

int mosaicx,mosaicy;

float glob_rot;

logical ss;

#define LINEMX 500
char line[LINEMX],title[LINEMX];

logical debug = FALSE;

/* function prototypes */

void find_regions(void);
void find_central_loop(void);
void dump_loops(void);
void traverse_loop(struct loop *lp, struct connection *anchor_connection);
void write_plt2_output(void);
void read_in_bases(void);
void determine_depths(void);
int depth(struct loop *lp);
void determine_radius(struct loop *lp, float lencut);
struct loop *construct_loop(int ibase);
logical connected_connection(struct connection *cp, struct connection *cpnext);
int find_ic_middle(int icstart, int icend,
		   struct connection *anchor_connection,
		   struct connection *acp, struct loop *lp);
void generate_region(struct connection *cp);
void construct_circle_segment(int start,int end);
void construct_extruded_segment(struct connection *cp,
				struct connection *cpnext);
void find_center_for_arc(int n, float b, float *hp,float *thetap);
void na_scanf(char *st, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
	      int a8, int a9, int a10, int a11, int a12, int a13, int a14,
	      int a15, int a16, int a17, int a18, int a19, int a20);
float minf2(float x1, float x2);
float maxf2(float x1, float x2);
int ask(char *question);
void trim(char *st, int *stlenp);
void chcnbl(char *st, int stlen);
void free_memory(void);
void load_naview_struc(struc_t * the_struc);
struc_t *save_naview(struc_t *the_struc);
void passthrough_char(const char c);
void anglexaya(struc_t *the_struc);
/*void anglexaya(void);*/

/* function definitions */

int
naview(void) {
  
    /*  char *cp; */
    /*    int fd; */
    /*    logical change_rad; */
    /*    int ilp,linel; */
    /*    float r; */
    /*    struct radloop *rlp; */
  
    /*
      if (argc != 3)
      dbError(DB_TRUE, ""Usage is RADIAL ct-file PLT2-file, please try again.\n");
    */
  
    /*    inf = fopen(infile,"r");
	  if (inf == NULL) {
	  fl_show_alert("Unable to open file:", infile, "", TRUE);
	  return(FATAL_ERROR);
	  }
    */
  
    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  
    /*
      inf = to_main.fileptr;
      if(inf==NULL)
      return(FATAL_ERROR);
    
      if ((cp = fgets(line,LINEMX,inf)) == NULL) {
      fl_show_alert("Unable to read header record:", to_main.filename, "", TRUE);
      return(FATAL_ERROR);
      }
    
      #   ifdef vms
      if (strlen(line) <= 1) {
      if ((cp = fgets(line,LINEMX,inf)) == NULL)
      dbError(DB_TRUE, "Unable to read header record.\n");

      }
      #   endif
    
      line[76] = '\0';
      linel = strlen(line);
      chcnbl(line,linel);
      trim(line,&linel);
      line[linel] = '\0';
      sscanf(line,"%5d",&nbase);
    */
  
  
  
    /*  set_comment(to_main.comment, FL_BLUE, "Length: %d",nbase); */
  
    /*    printf("%d bases specified in file. Title is \n%s\n",nbase,&line[5]);*/
/*    strcpy(title,&line[5]); */
  
    /*    outf = fopen("naview.plt2", "w");
	
	  if (outf == NULL) {
	  fl_show_alert("Unable to open file:", "naview.plt2", "", TRUE);
	  return(FATAL_ERROR);
	  } */
  
  
  
    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  
    /*
      bases = (struct base *) malloc(sizeof(struct base)*(nbase+1));
      regions = (struct region *) malloc(sizeof(struct region)*(nbase+1));
      read_in_bases();
    */
  
    /*  dbDebug(DB_DEBUG, "output...\n");
	{
	int i;
	for(i=0;i<=nbase;i++)
	dbDebug(DB_DEBUG, "[%d] %c %d %d %f %f\n",
	bases[i].hist_num, bases[i].name, bases[i].mate,
	bases[i].extracted, bases[i].x, bases[i].y);
	}
    */
  
  
    /*    printf("Please specify a PLT2 device string:\n");
	  fgets(line,LINEMX,stdin);*/
    /*    linel = strlen(line);
	  if (line[linel-1] == '\n') {
	  line[--linel] = '\0';
	  }
	  dev = (char *) malloc(linel+1);
	  strcpy(dev,line);*/
    /*    printf("Please specify the plotter device dimensions in cm.\n");
	  printf("Horizontal then vertical sizes separated by a space:\n");
	  na_scanf("%f%f",&xsize,&ysize); */
  
    xsize = to_main.glcanvas->w;
      ysize = to_main.glcanvas->h; /*change 14.05.2002*/
    /*xsize = 200;
      ysize = 200;*/
  
    /*    Draw_bases = ask("Should bases be displayed?");
	  mark_loops = ask("Should loops numbers be displayed?"); */
  
    lencut = 0.5;
  
    /*    printf("Please specify the minimum permissible separation between bases.\n");
	  printf("A return will use the default of %f.\n",lencut);
	  fgets(line,LINEMX,stdin);
	  linel = strlen(line);
	  if (line[linel-1] == '\n') {
	  line[--linel] = '\0';
	  }
	  trim(line,&linel);
	  if (linel > 0) {
	  line[linel] = '\0';
	  sscanf(line,"%f",&lencut);
	  lencut = max2(0.0,min2(0.5,lencut));
	  } */
    rlphead = rlptail = NULL;
    /*    change_rad = ask("Do you want to change loop radii?");
	  if (change_rad) {
	  do {
	  printf("Please specify a loop number and radius. Type zeroes to quit:\n");
	  r = 0.0;
	  na_scanf("%d%f",&ilp,&r);
	  if (ilp > 0) {
	  if (r <= 0.0)
	  printf("Bad radius specified. It must be positive.\n");
	  else {
	  rlp = struct_alloc(radloop);
	  rlp->radius = r;
	  rlp->loopnumber = ilp;
	  add_double_list(rlphead,rlptail,rlp);
	  }
	  }
	  }
	  while    (ilp > 0);
	  }
    */
    /*    printf("Please specify the character size:\n");
	  na_scanf("%f",&csz);*/
    csz = 0.4;
    /*    dot_pairs = ask("Do you want base pairs drawn as dots?");
	  printf("What frequency for labeling bases (specify 0 for none)?\n");
	  na_scanf("%d",&label_rate);
	  if (label_rate < 0) label_rate = 0;
	  if (ask("Do you want a mosaic?")) {
	  printf("How many display pages/screens for the horizontal dimension?\n");
	  na_scanf("%d",&mosaicx);
	  printf("How many display pages/screens for the vertical dimension?\n");
	  na_scanf("%d",&mosaicy);
	  if (mosaicx < 1) mosaicx = 1;
	  if (mosaicy < 1) mosaicy = 1;
	  }
	  else*/ {
	mosaicx = 1;
	mosaicy = 1;
    }
	
    /*    printf("Please specify a rotation for the drawing (positive is clockwise):\n"); */
    glob_rot = 0.0;
    /*    na_scanf("%f",&glob_rot); */
    dbDebug(DB_DEBUG, "Now constructing tree of loops...\n");
    find_regions();
    loop_count = 0;
    loops = (struct loop *) calloc(nbase+1, sizeof(struct loop));
    anchor = construct_loop(0);
    find_central_loop();
    if (debug) dump_loops();
    dbDebug(DB_DEBUG, "Now constructing the drawing...\n");
    traverse_loop(root,NULL);

    /* dbDebug(DB_DEBUG, "Now writing PLT2 commands...\n");
       write_plt2_output(); */
    dbDebug(DB_DEBUG, "All done.\n");
	
    /*free_memory();*/

    return(SUCCESS);
}

/*
  void
  read_in_bases(void) */
/*
 *   Read in the bases from the CT-file.
 */
/*
  {   char *cp;
  int i;
  */
/*
 *   Set up an origin.
 */
/*
  bases[0].name = 'o';
  bases[0].mate = 0;
  bases[0].hist_num = 0;
  bases[0].extracted = FALSE;
  bases[0].x = anum;
  bases[0].y = anum;
  i = 1;
  for (;;) {
  cp = fgets(line,LINEMX,inf);
  if (cp == NULL) break;
  sscanf(line,"%*d %1s %*d %*d %d %d",&bases[i].name,&bases[i].mate
  ,&bases[i].hist_num); */
/*
 *	The code in this program depends on mate being false (namely zero)
 *	where there is no mate.
 */
/*	bases[i].extracted = FALSE;
	bases[i].x = anum;
	bases[i].y = anum;
	i++;
	}
	if (--i != nbase) {
	dbDebug(DB_DEBUG, "Number of bases read from file, %d, doesn't match header record.\n",i);
	nbase = i;
	}
	return;
	}
	*/

void
find_regions(void) {
    /*
     *   Identifies the regions in the structure.
   */
  
  
  /*  int j; */
    int i,mate,nb1;
    logical *mark;
    /*  logical found; */

    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    ss = TRUE;
    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  
    nb1 = nbase + 1;
    mark = (int *) malloc(sizeof(int)*nb1);
    for (i = 0; i < nb1; i++) mark[i] = FALSE;
    nregion = 0;

  /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    for (i=0; i<=nbase; i++)
	if(bases[i].mate)
	    ss = FALSE;

    if(ss) {
	dbDebug(DB_DEBUG, "Found Complete Single Strand 2D-Structure\n");
	bases[1].mate = nbase;
	bases[nbase].mate = 1;
    }
    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    for (i=0; i<=nbase; i++) {
	if ( (mate = bases[i].mate) && !mark[i]) {
	    regions[nregion].start1 = i;
	    regions[nregion].end2 = mate;
	    mark[i] = TRUE;
	    mark[mate] = TRUE;
	    bases[i].region = bases[mate].region = &regions[nregion];
	    for (i++,mate--; 
		 i<mate && bases[i].mate == mate; 
		 i++,mate--) {
		mark[i] = mark[mate] = TRUE;
		bases[i].region = bases[mate].region = &regions[nregion];
	    }
	    regions[nregion].end1 = --i;
	    regions[nregion].start2 = mate+1;
	    if (debug) {
		if (nregion == 0)
		    dbDebug(DB_DEBUG, "\nRegions are:\n");
		/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
		dbDebug(DB_DEBUG, "No. of Regions: %d\n", nregion);
		/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
		dbDebug(DB_DEBUG, "Region %d is %d-%d and %d-%d with gap of %d.\n",
			nregion+1,regions[nregion].start1,regions[nregion].end1,
			regions[nregion].start2,regions[nregion].end2,
			regions[nregion].start2-regions[nregion].end1+1);
	    }
	    nregion++;
	}
    }
    free(mark);
  
    return;
}

struct loop *
construct_loop(int ibase) {
    /*
     *   Starting at residue ibase, recursively constructs the loop containing
     *   said base and all deeper bases. 
     */
  
    int i,mate;
    struct loop *retloop;
    struct loop *lp = NULL;
    struct connection *cp;
    /*    struct connection **cpp; */
    struct region *rp;
    struct radloop *rlp;

    retloop = &loops[loop_count++];
    retloop->nconnection = 0;
    retloop->connections = (struct connection **) malloc(sizeof(struct connection *));
    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    retloop->connections[0] = NULL;
    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    retloop->depth = 0;
    retloop->number = loop_count;
    retloop->radius = 0.0;
    for (rlp = rlphead;  rlp;  rlp = rlp->next)
	if (rlp->loopnumber == loop_count) retloop->radius = rlp->radius;
    i = ibase;
    do {
	if ((mate = bases[i].mate) != 0) {
	    rp = bases[i].region;
	    if (!bases[rp->start1].extracted) {
		if (i == rp->start1) {
		    bases[rp->start1].extracted = TRUE;
		    bases[rp->end1].extracted = TRUE;
		    bases[rp->start2].extracted = TRUE;
		    bases[rp->end2].extracted = TRUE;
		    lp = construct_loop(rp->end1 < nbase ? rp->end1+1 : 0);	
		}
		else if (i == rp->start2){
		    bases[rp->start2].extracted = TRUE;
		    bases[rp->end2].extracted = TRUE;
		    bases[rp->start1].extracted = TRUE;
		    bases[rp->end1].extracted = TRUE;
		    lp = construct_loop(rp->end2 < nbase ? rp->end2+1 : 0);
		}
		else {
		    dbError(DB_TRUE, "\n\nError detected in construct_loop. i = %d not found in region table.\n",i);
		}
		retloop->connections = (struct connection **) 
		    realloc(retloop->connections,
			    (++retloop->nconnection+1) * 
			    sizeof(struct connection *));
		retloop->connections[retloop->nconnection-1] = cp = 
		    struct_alloc(connection);
		retloop->connections[retloop->nconnection] = NULL;
		cp->loop = lp;
		cp->region = rp;
		if (i == rp->start1) {
		    cp->start = rp->start1;
		    cp->end = rp->end2;
		}
		else {
		    cp->start = rp->start2;
		    cp->end = rp->end1;
		}
		cp->extruded = FALSE;
		cp->broken = FALSE;
		lp->connections = (struct connection **) 
		    realloc(lp->connections,
			    (++lp->nconnection+1) * 
			    sizeof(struct connection *));
		lp->connections[lp->nconnection-1] = cp = 
		    struct_alloc(connection);
		lp->connections[lp->nconnection] = NULL;
		cp->loop = retloop;
		cp->region = rp;
		if (i == rp->start1) {
		    cp->start = rp->start2;
		    cp->end = rp->end1;
		}
		else {
		    cp->start = rp->start1;
		    cp->end = rp->end2;
		}
		cp->extruded = FALSE;
		cp->broken = FALSE;
	    }
	    i = mate;
	}
	if (++i > nbase) i = 0;
    }
    while (i != ibase);

    return retloop;
}

void
dump_loops(void) {
    /*
     *   Displays all the loops.
   */
  
    int il,ilp,irp;
    struct loop *lp;
    struct connection *cp,**cpp;
  
    dbDebug(DB_DEBUG, "\nRoot loop is #%d\n",(root-loops)+1);
    for (il=0; il < loop_count; il++) {
	lp = &loops[il];
	dbDebug(DB_DEBUG, "Loop %d has %d connections:\n",il+1,lp->nconnection);
	if(lp->nconnection!=0) {
	    for (cpp = lp->connections;(cp = *cpp)!=NULL; cpp++) {
		ilp = (cp->loop - loops) + 1;
		irp = (cp->region - regions) + 1;
		dbDebug(DB_DEBUG, "  Loop %d Region %d (%d-%d)\n",
			ilp,irp,cp->start,cp->end);
	    }
	}
    }
  
    return;
}

void
find_central_loop(void) {
    /*
     *   Find node of greatest branching that is deepest.
   */
  
    struct loop *lp;
    int maxconn,maxdepth,i;
  
    determine_depths();
    maxconn = 0;
    maxdepth = -1;
  
    for (i=0; i<loop_count; i++) {
	lp = &loops[i];
	if (lp->nconnection > maxconn) {
	    maxdepth = lp->depth;
	    maxconn = lp->nconnection;
	    root = lp;
	}
	else if (lp->depth > maxdepth && lp->nconnection == maxconn) {
	    maxdepth = lp->depth;
	    root = lp;
	}
    }
  
    return;
}

void
determine_depths(void) {
    /*
     *   Determine the depth of all loops.
   */
  
    struct loop *lp;
    int i,j;
  
    for (i=0; i<loop_count; i++) {
	lp = &loops[i];
	for (j=0; j<loop_count; j++) loops[j].mark = FALSE;
	lp->depth = depth(lp);
    }
  
    return;
}

int
depth(struct loop *lp) {
    /*
     *   Determines the depth of loop, lp. Depth is defined as the minimum
     *   distance to a leaf loop where a leaf loop is one that has only one
     *   or no connections.
   */
  
    struct connection *cp,**cpp;
    int count,ret,d;
  
    if (lp->nconnection <= 1) return 0;
    if (lp->mark) return -1;
    lp->mark = TRUE;
    count = 0;
    ret = 0;
    for (cpp=lp->connections; (cp = *cpp)!=NULL; cpp++) {
	d = depth(cp->loop);
	if (d >= 0) {
	    if (++count == 1) ret = d;
	    else if (ret > d) ret = d;
	}
    }
    lp->mark = FALSE;

    return ret+1;
}

void
traverse_loop(struct loop *lp, struct connection *anchor_connection) {
    /*
     *   This is the workhorse of the display program. The algorithm is
     *   recursive based on processing individual loops. Each base pairing
     *   region is displayed using the direction given by the circle diagram,
     *   and the connections between the regions is drawn by equally spaced
     *   points. The radius of the loop is set to minimize the square error
     *   for lengths between sequential bases in the loops. The "correct"
     *   length for base links is 1. If the least squares fitting of the
     *   radius results in loops being less than 1/2 unit apart, then that
     *   segment is extruded. 
   *   
   *   The variable, anchor_connection, gives the connection to the loop
   *   processed in an previous level of recursion.
   */
  
    float xs,ys,xe,ye,xn,yn,angleinc,r;
    /*   float sumn,sumd,angle,dt; */
    float radius,xc,yc,xo,yo,astart,aend,a;
    /*   float mindit,dit; */
    struct connection *cp,*cpnext,**cpp,*acp,*cpprev;
    /*    struct region *rp; */
    int i,j,n,ic;
    /* int end,start,l,mate,iprev,i2,nl,ns2,nstem,imindit; */
    float da,maxang;
    /*   float astartnext,xlr,ylr,xla,yla,angle_lp,radius1,ci; */
    int count,icstart,icend,icmiddle,icroot;
    logical done,done_all_connections,rooted;
    /*   logical found; */
    int sign;
    /*   int iend; */
    float midx,midy,nrx,nry,mx,my,vx,vy,dotmv,nmidx,nmidy;
    /*   float dotcn; */
    int icstart1,icup,icdown,icnext,direction;
    /*    int il1,il2,il3,il4; */
    float dan,dx,dy,rr;
    /*    float xlp,ylp,rlp,da1,rr1,rr2,xcross,ycross,rcross,xout,yout; */
    float cpx,cpy,cpnextx,cpnexty,cnx,cny,rcn,rc,lnx,lny,rl,ac,acn,sx,sy,dcp;
    /*    int maxloop; */
    int imaxloop = 0;
  
    angleinc = 2 * pi / (nbase+1);
    acp = NULL;
    icroot = -1;
    for (cpp=lp->connections, ic = 0; 
	 (cp = *cpp)!=NULL; 
	 cpp++, ic++) {
	/*	xs = cos(angleinc*cp->start);
	ys = sin(angleinc*cp->start);
	xe = cos(angleinc*cp->end);
	ye = sin(angleinc*cp->end); */
	xs = -sin(angleinc*cp->start);
	ys = cos(angleinc*cp->start);
	xe = -sin(angleinc*cp->end);
	ye = cos(angleinc*cp->end);
	xn = ye-ys;
	yn = xs-xe;
	r = sqrt(xn*xn + yn*yn);
	cp->xrad = xn/r;
	cp->yrad = yn/r;
	cp->angle = atan2(yn,xn);
	if (cp->angle < 0.0) cp->angle += 2*pi;
	if (anchor_connection != NULL &&
	    anchor_connection->region == cp->region) {
	    acp = cp;
	    icroot = ic;
	}
    }
  
 set_radius:
    determine_radius(lp,lencut);
    radius = lp->radius;
    if (anchor_connection == NULL) xc = yc = 0.0;
    else {
	xo = (bases[acp->start].x+bases[acp->end].x) / 2.0;
	yo = (bases[acp->start].y+bases[acp->end].y) / 2.0;
	xc = xo - radius * acp->xrad;
	yc = yo - radius * acp->yrad;
    }
  
    /*
   *   The construction of the connectors will proceed in blocks of
   *   connected connectors, where a connected connector pairs means
   *   two connectors that are forced out of the drawn circle because they
   *   are too close together in angle.
   */
  
  /*
   *   First, find the start of a block of connected connectors
   */
  
    if (icroot == -1) 
	icstart = 0;
    else icstart = icroot;
    cp = lp->connections[icstart];
    count = 0;
    if (debug)
	dbDebug(DB_DEBUG, "Now processing loop %d\n",lp->number);
    done = FALSE;
    do {
	j = icstart - 1;
	if (j < 0) j = lp->nconnection - 1;
	cpprev = lp->connections[j];
	if (!connected_connection(cpprev,cp)) {
	    done = TRUE;
	}
	else {
	    icstart = j;
	    cp = cpprev;
	}
	if (++count > lp->nconnection) {
	    /*
	     *   Here everything is connected. Break on maximum angular separation
	     *   between connections. 
	     */
	    maxang = -1.0;
	    for (ic = 0;  ic < lp->nconnection;  ic++) {
		j = ic + 1;
		if (j >= lp->nconnection) j = 0;
		cp = lp->connections[ic];
		cpnext = lp->connections[j];
		ac = cpnext->angle - cp->angle;
		if (ac < 0.0) ac += 2*pi;
		if (ac > maxang) {
		    maxang = ac;
		    imaxloop = ic;
		}
	    }
	    icend = imaxloop;
	    icstart = imaxloop + 1;
	    if (icstart >= lp->nconnection) icstart = 0;
	    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
	    if(lp->nconnection!=0) {
		/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
		cp = lp->connections[icend];
		cp->broken = TRUE;
	    }
	    done = TRUE;
	}
    } while    (!done);
    done_all_connections = FALSE;
    icstart1 = icstart;
    if (debug) dbDebug(DB_DEBUG, "Icstart1 = %d\n",icstart1);
    while (!done_all_connections) {
	count = 0;
	done = FALSE;
	icend = icstart;
	rooted = FALSE;
	while (!done) {
	    cp = lp->connections[icend];
	    if (icend == icroot) rooted = TRUE;
	    j = icend + 1;
	    if (j >= lp->nconnection) {
		j = 0;
	    }
	    cpnext = lp->connections[j];
	    if (connected_connection(cp,cpnext)) {
		if (++count >= lp->nconnection) break;
		icend = j;
	    }
	    else {
		done = TRUE;
	    }
	}
	icmiddle = find_ic_middle(icstart,icend,anchor_connection,acp,lp);
	ic = icup = icdown = icmiddle;
	if (debug)
	    dbDebug(DB_DEBUG, "IC start = %d  middle = %d  end = %d\n",
		    icstart,icmiddle,icend);
	done = FALSE;
	direction = 0;
	while (!done) {
	    if (direction < 0) {
		ic = icup;
	    }
	    else if (direction == 0) {
		ic = icmiddle;
	    }
	    else {
		ic = icdown;
	    }
	    if (ic >= 0) {
		cp = lp->connections[ic];
		if (anchor_connection == NULL || acp != cp) {
		    if (direction == 0) {
			astart = cp->angle - asin(1.0/2.0/radius);
			aend = cp->angle + asin(1.0/2.0/radius);
			bases[cp->start].x = xc + radius*cos(astart);
			bases[cp->start].y = yc + radius*sin(astart);
			bases[cp->end].x = xc + radius*cos(aend);
			bases[cp->end].y = yc + radius*sin(aend);
		    }
		    else if (direction < 0) {
			j = ic + 1;
			if (j >= lp->nconnection) j = 0;
			cp = lp->connections[ic];
			cpnext = lp->connections[j];
			cpx = cp->xrad;
			cpy = cp->yrad;
			ac = (cp->angle + cpnext->angle) / 2.0;
			if (cp->angle > cpnext->angle) ac -= pi;
			cnx = cos(ac);
			cny = sin(ac);
			lnx = cny;
			lny = -cnx;
			da = cpnext->angle - cp->angle;
			if (da < 0.0) da += 2*pi;
			if (cp->extruded) {
			    if (da <= pi/2) rl = 2.0;
			    else rl = 1.5;
			}
			else {
			    rl = 1.0;
			}
			bases[cp->end].x = bases[cpnext->start].x + rl*lnx;
			bases[cp->end].y = bases[cpnext->start].y + rl*lny;
			bases[cp->start].x = bases[cp->end].x + cpy;
			bases[cp->start].y = bases[cp->end].y - cpx;
		    }
		    else {
			j = ic - 1;
			if (j < 0) j = lp->nconnection - 1;
			cp = lp->connections[j];
			cpnext = lp->connections[ic];
			cpnextx = cpnext->xrad;
			cpnexty = cpnext->yrad;
			ac = (cp->angle + cpnext->angle) / 2.0;
			if (cp->angle > cpnext->angle) ac -= pi;
			cnx = cos(ac);
			cny = sin(ac);
			lnx = -cny;
			lny = cnx;
			da = cpnext->angle - cp->angle;
			if (da < 0.0) da += 2*pi;
			if (cp->extruded) {
			    if (da <= pi/2) rl = 2.0;
			    else rl = 1.5;
			}
			else {
			    rl = 1.0;
			}
			bases[cpnext->start].x = bases[cp->end].x + rl*lnx;
			bases[cpnext->start].y = bases[cp->end].y + rl*lny;
			bases[cpnext->end].x = bases[cpnext->start].x - cpnexty;
			bases[cpnext->end].y = bases[cpnext->start].y + cpnextx;
		    }
		}
	    }
	    if (direction < 0) {
		if (icdown == icend) {
		    icdown = -1;
		}
		else if (icdown >= 0) {
		    if (++icdown >= lp->nconnection) {
			icdown = 0;
		    }
		}
		direction = 1;
	    }
	    else {
		if (icup == icstart) icup = -1;
		else if (icup >= 0) {
		    if (--icup < 0) {
			icup = lp->nconnection - 1;
		    }
		}
		direction = -1;
	    }
	    done = icup == -1 && icdown == -1;
	}
	icnext = icend + 1;
	if (icnext >= lp->nconnection) icnext = 0;
	if (icend != icstart && (! (icstart == icstart1 && icnext == icstart1))) {
	    /*
	     *   Move the bases just constructed (or the radius) so
	     *   that the bisector of the end points is radius distance
	     *   away from the loop center.
	     */
	    cp = lp->connections[icstart];
	    cpnext = lp->connections[icend];
	    dx = bases[cpnext->end].x - bases[cp->start].x;
	    dy = bases[cpnext->end].y - bases[cp->start].y;
	    midx = bases[cp->start].x + dx/2.0;
	    midy = bases[cp->start].y + dy/2.0;
	    rr = sqrt(dx*dx + dy*dy);
	    mx = dx / rr;
	    my = dy / rr;
	    vx = xc - midx;
	    vy = yc - midy;
	    rr = sqrt(dx*dx + dy*dy);
	    vx /= rr;
	    vy /= rr;
	    dotmv = vx*mx + vy*my;
	    nrx = dotmv*mx - vx;
	    nry = dotmv*my - vy;
	    rr = sqrt(nrx*nrx + nry*nry);
	    nrx /= rr;
	    nry /= rr;
	    /*
	     *   Determine which side of the bisector the center should be.
	     */
	    dx = bases[cp->start].x - xc;
	    dy = bases[cp->start].y - yc;
	    ac = atan2(dy,dx);
	    if (ac < 0.0) ac += 2*pi;
	    dx = bases[cpnext->end].x - xc;
	    dy = bases[cpnext->end].y - yc;
	    acn = atan2(dy,dx);
	    if (acn < 0.0) acn += 2*pi;
	    if (acn < ac) acn += 2*pi;
	    if (acn - ac > pi) sign = -1;
	    else sign = 1;
	    nmidx = xc + sign*radius*nrx;
	    nmidy = yc + sign*radius*nry;
	    if (rooted) {
		xc -= nmidx - midx;
		yc -= nmidy - midy;
	    }
	    else {
		for (ic=icstart; ; ++ic >= lp->nconnection ? (ic = 0) : 0) {
		    cp = lp->connections[ic];
		    i = cp->start;
		    bases[i].x += nmidx - midx;
		    bases[i].y += nmidy - midy;
		    i = cp->end;
		    bases[i].x += nmidx - midx;
		    bases[i].y += nmidy - midy;
		    if (ic == icend) break;
		}
	    }
	}
	icstart = icnext;
	done_all_connections = icstart == icstart1;
    }
    for (ic=0; ic < lp->nconnection; ic++) {
	cp = lp->connections[ic];
	j = ic + 1;
	if (j >= lp->nconnection) j = 0;
	cpnext = lp->connections[j];
	dx = bases[cp->end].x - xc;
	dy = bases[cp->end].y - yc;
	rc = sqrt(dx*dx + dy*dy);
	ac = atan2(dy,dx);
	if (ac < 0.0) ac += 2*pi;
	dx = bases[cpnext->start].x - xc;
	dy = bases[cpnext->start].y - yc;
	rcn = sqrt(dx*dx + dy*dy);
	acn = atan2(dy,dx);
	if (acn < 0.0) acn += 2*pi;
	if (acn < ac) acn += 2*pi;
	dan = acn - ac;
	dcp = cpnext->angle - cp->angle;
	if (dcp <= 0.0) dcp += 2*pi;
	if (fabs(dan-dcp) > pi) {
	    if (cp->extruded) {
		dbDebug(DB_DEBUG, "Warning from traverse_loop. Loop %d has crossed regions\n",
			lp->number);
	    }
	    else {
		cp->extruded = TRUE;
		goto set_radius;	    /* Forever shamed */
	    }
	}
	if (cp->extruded) {
	    construct_extruded_segment(cp,cpnext);
	}
	else {
	    n = cpnext->start - cp->end;
	    if (n < 0) n += nbase + 1;
	    angleinc = dan / n;
	    for (j = 1;  j < n;  j++) {
		i = cp->end + j;
		if (i > nbase) i -= nbase + 1;
		a = ac + j*angleinc;
		rr = rc + (rcn-rc)*(a-ac)/dan;
		bases[i].x = xc + rr*cos(a);
		bases[i].y = yc + rr*sin(a);
	    }
	}
    }
    for (ic=0; ic < lp->nconnection; ic++) {
	if (icroot != ic) {
	    cp = lp->connections[ic];
	    generate_region(cp);
	    traverse_loop(cp->loop,cp);
	}
    }
    n = 0;
    sx = 0.0;
    sy = 0.0;
    for (ic = 0;  ic < lp->nconnection;  ic++) {
	j = ic + 1;
	if (j >= lp->nconnection) j = 0;
	cp = lp->connections[ic];
	cpnext = lp->connections[j];
	n += 2;
	sx += bases[cp->start].x + bases[cp->end].x;
	sy += bases[cp->start].y + bases[cp->end].y;
	if (!cp->extruded) {
	    for (j = cp->end + 1;  j != cpnext->start;  j++) {
		if (j > nbase) j -= nbase + 1;
		n++;
		sx += bases[j].x;
		sy += bases[j].y;
	    }
	}
    }
    lp->x = sx / n;
    lp->y = sy / n;
  
    return;
}

void
determine_radius(struct loop *lp,float lencut) {
    /*
     *   For the loop pointed to by lp, determine the radius of
     *   the loop that will ensure that each base around the loop will
     *   have a separation of at least lencut around the circle.
     *   If a segment joining two connectors will not support this separation,
     *   then the flag, extruded, will be set in the first of these
     *   two indicators. The radius is set in lp.
   *   
   *   The radius is selected by a least squares procedure where the sum of the
   *   squares of the deviations of length from the ideal value of 1 is used
   *   as the error function.
   */
  
    int count,i,j,end,start,imindit = 0;
    float mindit,ci,dt,sumn,sumd,radius,dit;
    /*   float da; */
    struct connection *cp,*cpnext;
    static float rt2_2 = 0.7071068;
  
    count = 0;
    do {
	mindit = 1.0e10;
	for (sumd=0.0, sumn=0.0, i=0;
	     i < lp->nconnection;
	     i++) {
	    cp = lp->connections[i];
	    j = i + 1;
	    if (j >= lp->nconnection) j = 0;
	    cpnext = lp->connections[j];
	    end =  cp->end;
	    start = cpnext->start;
	    if (start < end) start += nbase + 1;
	    dt = cpnext->angle - cp->angle;
	    if (dt <= 0.0) dt += 2*pi;
	    if (!cp->extruded) 
		ci = start - end;
	    else {
		if (dt <= pi/2) ci = 2.0;
		else ci = 1.5;
	    }
	    sumn += dt * (1.0/ci + 1.0);
	    sumd += dt * dt / ci;
	    dit = dt/ci;
	    if (dit < mindit && !cp->extruded && ci > 1.0) {
		mindit = dit;
		imindit = i;
	    }
	}
	radius = sumn/sumd;
	if (radius < rt2_2) radius = rt2_2;
	if (mindit*radius < lencut) {
	    lp->connections[imindit]->extruded = TRUE;
	}
    } while (mindit*radius < lencut);
    if (lp->radius > 0.0)
	radius = lp->radius;
    else lp->radius = radius;
  
    return;
}

logical
connected_connection(struct connection *cp, struct connection *cpnext) {
    /*
     *   Determines if the connections cp and cpnext are connected
     */
  
  /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    if(cp==NULL || cpnext==NULL)
	return FALSE;
    /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  
    if (cp->extruded) {
	return TRUE;
    }
    else if (cp->end+1 == cpnext->start) {
	return TRUE;
    }
    else {
	return FALSE;
    }
}

int
find_ic_middle(int icstart, int icend,
	       struct connection *anchor_connection,
	       struct connection *acp, struct loop *lp) {
    /*
   *   Finds the middle of a set of connected connectors. This is normally
   *   the middle connection in the sequence except if one of the connections
   *   is the anchor, in which case that connection will be used.
   */
  
    int count,ret,ic,i;
    logical done;
  
    count = 0;
    ret = -1;
    ic = icstart;
    done = FALSE;
    while (!done) {
	if (count++ > lp->nconnection * 2) {
	    dbError(DB_TRUE, "Infinite loop detected in find_ic_middle\n");
	}
	if (anchor_connection != NULL && lp->connections[ic] == acp) {
	    ret = ic;
	}
	done = ic == icend;
	if (++ic >= lp->nconnection) {
	    ic = 0;
	}
    }
    if (ret == -1) {
	for (i=1, ic=icstart; i<(count+1)/2; i++) {
	    if (++ic >= lp->nconnection) ic = 0;
	}
	ret = ic;
    }

    return ret;
}

void
generate_region(struct connection *cp) {
    /*
     *   Generates the coordinates for the base pairing region of a connection
     *   given the position of the starting base pair.
   */
  
    int l,start,end,i,mate;
    struct region *rp;
  
    rp = cp->region;
    l = 0;
    if (cp->start == rp->start1) {
	start = rp->start1;
	end = rp->end1;
    }
    else {
	start = rp->start2;
	end = rp->end2;
    }
    if (bases[cp->start].x > anum - 100.0 ||
	bases[cp->end].x > anum - 100.0) {
	dbError(DB_TRUE, "Bad region passed to generate_region. Coordinates not defined.\n");
    }
    for (i=start+1; i<=end; i++) {
	l++;
	bases[i].x = bases[cp->start].x + l*cp->xrad;
	bases[i].y = bases[cp->start].y + l*cp->yrad;
	mate = bases[i].mate;
	bases[mate].x = bases[cp->end].x + l*cp->xrad;
	bases[mate].y = bases[cp->end].y + l*cp->yrad;
    }
  
    return;
}

void
construct_circle_segment(int start,int end) {
    /*
     *   Draws the segment of residue between the bases numbered start
     *   through end, where start and end are presumed to be part of a base
     *   pairing region. They are drawn as a circle which has a chord given
     *   by the ends of two base pairing regions defined by the connections.
   */
  
    float dx,dy,rr,h,angleinc,midx,midy,xn,yn,nrx,nry,mx,my,a;
    int l,j,i;
  
    dx = bases[end].x - bases[start].x;
    dy = bases[end].y - bases[start].y;
    rr = sqrt(dx*dx + dy*dy);
    l = end - start;
    if (l < 0) l += nbase + 1;
    if (rr >= l) {
	dx /= rr;
	dy /= rr;
	for (j = 1;  j < l;  j++) {
	    i = start + j;
	    if (i > nbase) i -= nbase + 1;
	    bases[i].x = bases[start].x + dx*(float)j/(float)l;
	    bases[i].y = bases[start].y + dy*(float)j/(float)l;
	}
    }
    else {
	find_center_for_arc(l-1,rr,&h,&angleinc);
	dx /= rr;
	dy /= rr;
	midx = bases[start].x + dx*rr/2.0;
	midy = bases[start].y + dy*rr/2.0;
	xn = dy;
	yn = -dx;
	nrx = midx + h*xn;
	nry = midy + h*yn;
	mx = bases[start].x - nrx;
	my = bases[start].y - nry;
	rr = sqrt(mx*mx + my*my);
	a = atan2(my,mx);
	for (j = 1;  j < l;  j++) {
	    i = start + j;
	    if (i > nbase) i -= nbase + 1;
	    bases[i].x = nrx + rr*cos(a+j*angleinc);
	    bases[i].y = nry + rr*sin(a+j*angleinc);
	}
    }
    return;
}

void
construct_extruded_segment(struct connection *cp, struct connection *cpnext) {
    /*
     *   Constructs the segment between cp and cpnext as a circle if possible.
     *   However, if the segment is too large, the lines are drawn between
     *   the two connecting regions, and bases are placed there until the
     *   connecting circle will fit.
   */
  
    float astart,aend1,aend2,aave,dx,dy,a1,a2,ac,rr,da,dac;
    int start,end,n,nstart,nend;
    logical collision;
  
    astart = cp->angle;
    aend2 = aend1 = cpnext->angle;
    if (aend2 < astart) aend2 += 2*pi;
    aave = (astart + aend2) / 2.0;
    start = cp->end;
    end = cpnext->start;
    n = end - start;
    if (n < 0) n += nbase + 1;
    da = cpnext->angle - cp->angle;
    if (da < 0.0) {
	da += 2*pi;
    }
    if (n == 2) construct_circle_segment(start,end);
    else {
	dx = bases[end].x - bases[start].x;
	dy = bases[end].y - bases[start].y;
	rr = sqrt(dx*dx + dy*dy);
	dx /= rr;
	dy /= rr;
	if (rr >= 1.5 && da <= pi/2) {
	    nstart = start + 1;
	    if (nstart > nbase) nstart -= nbase + 1;
	    nend = end - 1;
	    if (nend < 0) nend += nbase + 1;
	    bases[nstart].x = bases[start].x + 0.5*dx;
	    bases[nstart].y = bases[start].y + 0.5*dy;
	    bases[nend].x = bases[end].x - 0.5*dx;
	    bases[nend].y = bases[end].y - 0.5*dy;
	    start = nstart;
	    end = nend;
	}
	do {
	    collision = FALSE;
	    construct_circle_segment(start,end);
	    nstart = start + 1;
	    if (nstart > nbase) nstart -= nbase + 1;
	    dx = bases[nstart].x - bases[start].x;
	    dy = bases[nstart].y - bases[start].y;
	    a1 = atan2(dy,dx);
	    if (a1 < 0.0) a1 += 2*pi;
	    dac = a1 - astart;
	    if (dac < 0.0) dac += 2*pi;
	    if (dac > pi) collision = TRUE;
	    nend = end - 1;
	    if (nend < 0) nend += nbase + 1;
	    dx = bases[nend].x - bases[end].x;
	    dy = bases[nend].y - bases[end].y;
	    a2 = atan2(dy,dx);
	    if (a2 < 0.0) a2 += 2*pi;
	    dac = aend1 - a2;
	    if (dac < 0.0) dac += 2*pi;
	    if (dac > pi) collision = TRUE;
	    if (collision) {
		ac = min2(aave,astart + 0.5);
		bases[nstart].x = bases[start].x + cos(ac);
		bases[nstart].y = bases[start].y + sin(ac);
		start = nstart;
		ac = max2(aave,aend2 - 0.5);
		bases[nend].x = bases[end].x + cos(ac);
		bases[nend].y = bases[end].y + sin(ac);
		end = nend;
		n -= 2;
	    }
	} while    (collision && n > 1);
    }
    return;
}

void
find_center_for_arc(int n, float b, float *hp,float *thetap) {
    /*
     *   Given n points to be placed equidistantly and equiangularly on a
     *   polygon which has a chord of length, b, find the distance, h, from the
     *   midpoint of the chord for the center of polygon. Positive values
     *   mean the center is within the polygon and the chord, whereas
     *   negative values mean the center is outside the chord. Also, the
     *   radial angle for each polygon side is returned in theta.
   *    
   *   The procedure uses a bisection algorithm to find the correct
   *   value for the center. Two equations are solved, the angles
   *   around the center must add to 2*pi, and the sides of the polygon
   *   excluding the chord must have a length of 1.
   */
  
    float h,hhi,hlow,r,disc,theta,e,phi;
    int iter;
#define maxiter 500
  
    hhi = (n+1) / pi;
    hlow = - hhi - b/(n+1-b);
    iter = 0;
    do {
	h = (hhi + hlow) / 2.0;
	r = sqrt(h*h + b*b/4.0);
	disc = 1.0 - 1.0/2.0/(r*r);
	/* ajout */
	if (disc <-1.0)
	    disc = -0.99;
	if (disc >1.0)
	    disc = 0.99;
/*fin */
	if (fabs(disc) > 1.0) { /* initial value 1.0)*/
	    dbError(DB_TRUE, "Unexpected large magnitude discriminant = %g\n",disc);
	}
	theta = acos(disc);
	phi = acos(h/r);
	e = theta * (n+1) + 2*phi - 2*pi;
	if (e > 0.0) {
	    hlow = h;
	}
	else {
	    hhi = h;
	}
    } while    (fabs(e) > 0.0001 && ++iter < maxiter);
    if (iter >= maxiter) {
	dbDebug(DB_DEBUG, "Iteration failed in find_center_for_arc\n");
	h = 0.0;
	theta = 0.0;
    }
    *hp = h;
    *thetap = theta;
  
    return;
}
#undef maxiter

void
write_plt2_output(void) {
    /*
     *   Writes the coordinates as a PLT2 command stream.
   */
  
    int i,mate,imx,imy,istart;
    /* int pagecnt; */
    float xmin,xmax,ymin,ymax,scalex,scaley,scale,scalecsz;
    float xmn,xmx,ymn,ymx;
    float xn,yn,r,x1,y1;
    float x2 = 0.0F;
    float y2 = 0.0F;
    float xs,ys,xoff,yoff;
    /*   float ct,st,xr,yr; */
    struct loop *lp;
  
    GLfloat *ctlp, *ctlpoints;
    float the_max;

#define okx(x) (xmn-scale <= (x) && (x) <= xmx+scale)
#define oky(y) (ymn-scale <= (y) && (y) <= ymx+scale)
  
    /*    fprintf(outf,"DEV %s\n",dev);
	fprintf(outf,"CSZ %10.4f\n",csz); */
  
  /* globale Rotation */
  /*    ct = cos(glob_rot*pi/180.0);
	st = sin(glob_rot*pi/180.0);
	for (i=0; i<=nbase; i++) {
	xr = bases[i].x*ct + bases[i].y*st;
	yr = bases[i].y*ct - bases[i].x*st;
	bases[i].x = xr;
	bases[i].y = yr;
	} 
	for (i=0;  i<loop_count;  i++)
	{
	lp = loops + i;
	xr = lp->x*ct + lp->y*st;
	yr = lp->y*ct - lp->x*st;
	lp->x = xr;
	lp->y = yr;
	} */
  
    xmax = ymax = -bignum;
    xmin = ymin = bignum;
    for (i=0; i<=nbase; i++) {
	if (bases[i].x > anum - 100.0) {
	    dbDebug(DB_DEBUG, "\nError in write_plt2_output -- base %d position is undefined.\n",i);
	}
	else {
	    xmax = max2(xmax,bases[i].x);
	    ymax = max2(ymax,bases[i].y);
	    xmin = min2(xmin,bases[i].x);
	    ymin = min2(ymin,bases[i].y);
	}
    }
  
    scalex = (xmax-xmin) / xsize /* / (float) mosaicx */ ;
    scaley = (ymax-ymin) / ysize /* / (float) mosaicy */ ;
    scale = max2(scalex,scaley) * 1.02; 
    xs = xsize * scale;
    ys = ysize * scale;
    xoff = (xs /* *mosaicx */ - xmax + xmin) / 2.0;
    yoff = (ys /* *mosaicy */ - ymax + ymin) / 2.0;
  
    the_max = max2(max2(fabs(xmax), fabs(xmin)), max2(fabs(ymax), fabs(ymin)));

    /*    fprintf(outf,"SA %g\n",scale);
	fprintf(outf,"ORI 0.0 0.0\n"); */
  /*  pagecnt = 0; */
  
    imx = imy = 1;
    /*    for (imx=1; imx <= mosaicx; imx++) {
	for (imy=1; imy <= mosaicy; imy++) { */
  /*	    if (++pagecnt > 1) { */
  /*		fprintf(outf,"DUMP\n"); */
  /*		} */
  /*	    if (title[0] != '\0') {
	    if (imx == (mosaicx+1)/2 && imy == 1) { */
  /*		    fprintf(outf,"CSZ 0.4\n");
		    fprintf(outf,"CTA %10.3f 1.0 \"%s\"\n",xsize/2.0,title);
		    fprintf(outf,"CSZ %10.4f\n",csz); */
  /*		    }
		    }*/
  
    xmn = xmin /* + (imx-1)*xs */ - xoff; /* weil imx = imy = 1 */
    ymn = ymin /* + (imy-1)*ys */ - yoff;
    xmx = xmin + /* imx* */ xs - xoff;
    ymx = ymin + /* imy* */ ys - yoff;
  
  /*	    fprintf(outf,"OD %10.3f %10.3f\n",xmn,ymn);
	    fprintf(outf,"COLOR WHITE\n"); */
  /*	    if (bases[0].x != anum) { */
  /*		fprintf(outf,"BRI 2\nMOV %10.3f %10.3f\nCIA 0.05\nCIA 0.1\nCIA 0.15\n",
		bases[0].x,bases[0].y); */
  /*	      } */
  /*	    fprintf(outf,"BRI 3\n"); */
  /*xxxxxxxxxxxxxxxxxx*/
    dbDebug(DB_DEBUG, "resizing...\n");
    {    
	/*  dbDebug(DB_DEBUG, "the_max: %f\n", the_max);
	    glc_resize(to_main.glcanvas, the_max*1.2); */
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    /*xxxxxxxxxxxxxxxxxx*/
    if (brucc.bases) {
	/*		fprintf(outf,"CM BASES\n"); */
	for (i=1; i <= nbase; i++) {
	    if (bases[i].x != anum) {
		/* if (okx(bases[i].x) && oky(bases[i].y)) { */
		/*			    fprintf(outf,"CTX %10.3f %10.3f \"%c\"\n",bases[i].x,bases[i].y,bases[i].name); */
		/*xxxxxxxxxxxxxxxxxx*/
		glRasterPos2d(bases[i].x, bases[i].y);
		glCallList(dlstart + bases[i].name);
		/*xxxxxxxxxxxxxxxxxx*/
		/*  } */
	    }
	}
    }
  
    /*	    fprintf(outf,"CM SEQUENCE LINES\n"); */
    if (brucc.bases) scalecsz = scale * csz * 1.8;
    else scalecsz = 0.0;
  
  /*xxxxxxxxxxxxxxxxxx*/
    ctlpoints = NULL;
    ctlpoints = malloc(nbase * 3 * sizeof(GLfloat));
    if(ctlpoints==NULL)
	dbError(DB_TRUE, "failed to malloc in write_plt2_output()");
    ctlp = ctlpoints;
    /*  glBegin(GL_LINES);*/
    /*xxxxxxxxxxxxxxxxxx*/
    for (i=1; i < nbase; i++) {
	if (bases[i].x != anum && bases[i+1].x != anum) {
	    /*  if (okx(bases[i].x) && oky(bases[i].y) ||
		okx(bases[i+1].x) && oky(bases[i+1].y)) { */
	    xn = bases[i+1].x - bases[i].x;
	    yn = bases[i+1].y - bases[i].y;
	    r = sqrt(xn*xn + yn*yn);
	    if (r > scalecsz) {
		xn /= r;
		yn /= r;
		x1 = bases[i].x + xn*scalecsz/2.0;
		y1 = bases[i].y + yn*scalecsz/2.0;
		x2 = bases[i+1].x - xn*scalecsz/2.0;
		y2 = bases[i+1].y - yn*scalecsz/2.0;
		/*			    fprintf(outf,"LI %10.3f %10.3f 0.0 %10.3f %10.3f 0.0\n",
					    x1,y1,x2,y2); */
		/*xxxxxxxxxxxxxxxxxx*/
	
		*ctlp = (GLfloat) x1;
		ctlp++;
		*ctlp = (GLfloat) y1;
		ctlp++;
		*ctlp = (GLfloat) 0.0;
		ctlp++;
	
		/*    glVertex2f(x1, y1);
		      glVertex2f(x2, y2); */
		/*xxxxxxxxxxxxxxxxxx*/
	    }
	    /* } */
	}
    }
    *ctlp = (GLfloat) x2;
    ctlp++;
    *ctlp = (GLfloat) y2;
    ctlp++;
    *ctlp = (GLfloat) 0.0;
  
  /*xxxxxxxxxxxxxxxxxx*/
  /*  glEnd();*/
  
  /*    for(i=0;i<nbase;i++)
	dbDebug(DB_DEBUG, "base[%d]: %f %f %f\n",
	i, ctlpoints[i*3], ctlpoints[i*3+1], ctlpoints[i*3+2]); */
  
    {
#define ORDER 3
    
	int knum;
	GLfloat *knots;
	GLfloat j = 0.0;
	GLUnurbsObj *theNurb;
    
	/* glShadeModel (GL_FLAT);*/
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, (GLenum)GLU_SAMPLING_TOLERANCE, 5.0);      
      
	knum = nbase + ORDER;
	knots = NULL;
	knots = malloc(knum * sizeof(GLfloat));
	if(knots==NULL)
	    dbError(DB_TRUE, "failed to malloc in write_plt2_output");
    
	for(i=0;i<ORDER;i++)
	    knots[i] = j;
	for(;i<nbase;i++)
	    knots[i] = ++j;
	j++;
	for(;i<knum;i++)
	    knots[i] = j;
    
/*      for(i=0;i<knum;i++)
	dbDebug(DB_DEBUG, "knots[%d] = %f\n", i, knots[i]); */

	gluBeginCurve(theNurb);
	gluNurbsCurve(theNurb, 
		      knum, knots,
		      3,
		      ctlpoints, 
		      ORDER,
		      GL_MAP1_VERTEX_3);
	gluEndCurve(theNurb);
    
	gluDeleteNurbsRenderer(theNurb);
	free(knots);
    }
  
    free(ctlpoints);
  
/*xxxxxxxxxxxxxxxxxx*/
/*	   fprintf(outf,"CM BASE PAIRING LINES\n");
	   fprintf(outf,"COLOR RED\nBRI 5\n"); */
  
#define draw_1bp \
  if (brucc.dots)\
    {\
       x1 = (bases[i].x + bases[mate].x) / 2.0;\
       y1 = (bases[i].y + bases[mate].y) / 2.0;\
/*		    if (okx(x1) && oky(y1)) { \ */ \
/*		      fprintf(outf,"MOV %10.3f %10.3f\nCIA 0.0\n",x1,y1);\ */ \
/*xxxxxxxxxxxxxxxxxx*/ \
		      glPointSize((GLfloat)5.0); \
			    glBegin(GL_POINTS); \
			    glVertex2f(x1, y1); \
			    glEnd(); \
		      glPointSize((GLfloat)1.0); \
/*xxxxxxxxxxxxxxxxxx*/ \
		  /*  } \ */ \
		}\
	    else\
		{\
		    xn = bases[mate].x - bases[i].x;\
		    yn = bases[mate].y - bases[i].y;\
		    r = sqrt(xn*xn + yn*yn);\
		    if (r > scalecsz) {\
			xn /= r;\
			yn /= r;\
			x1 = bases[i].x + xn*scalecsz/2.0;\
			y1 = bases[i].y + yn*scalecsz/2.0;\
			x2 = bases[mate].x - xn*scalecsz/2.0;\
			y2 = bases[mate].y - yn*scalecsz/2.0;\
/*			if (okx(x1) && oky(y1) || \
			okx(x2) && oky(y2)) { \ */ \
/*			    fprintf(outf,"LI %10.3f %10.3f 0.0 %10.3f %10.3f 0.0\n",\
			    x1,y1,x2,y2);\ */ \
/*xxxxxxxxxxxxxxxxxx*/ \
			    glBegin(GL_LINES); \
			    glVertex2f(x1, y1); \
			    glVertex2f(x2, y2); \
			    glEnd(); \
/*xxxxxxxxxxxxxxxxxx*/ \
			  /*  }\ */ \
			}\
		}

    glColor3f(0.0, 1.0, 0.0);

    if(!brucc.none && !ss) {
	for (i=0; i <= nbase; i++) {
	    if ((mate = bases[i].mate) && i < mate ) {
		if (bases[i].x != anum && bases[mate].x != anum) {
		    if ((bases[i].name == 'G' && bases[mate].name == 'C') ||
			(bases[i].name == 'C' && bases[mate].name == 'G'))
		    {
			draw_1bp;
		    }
		}
	    }
	}
    
	/*	   fprintf(outf,"COLOR MAGENTA\nBRI 1\n"); */
	for (i=0; i <= nbase; i++) {
	    if ((mate = bases[i].mate) && i < mate ) {
		if (bases[i].x != anum && bases[mate].x != anum) {
		    if (!((bases[i].name == 'G' && bases[mate].name == 'C') ||
			  (bases[i].name == 'C' && bases[mate].name == 'G')))
		    {
			draw_1bp;
		    }
		}
	    }
	}
    }
  
    /*	    fprintf(outf,"COLOR WHITE\nBRI 3\n"); */
    if (brucc.freq > 0) {
	glColor3f(1.0, 1.0, 1.0);
	/*		fprintf(outf,"CM LABELS\nCSZ %10.4f\n",csz); */

	/* haeaeaeaeah ??????????, was soll das????? */
	if(brucc.freq != 1)
	    istart = brucc.freq*(1 + bases[1].hist_num/brucc.freq) 
		- bases[1].hist_num + 1;
	else
	    istart = 1;
	for (i=istart; i <= nbase; i += brucc.freq) {
	    if (bases[i].x != anum) {
		/*if (okx(bases[i].x) && oky(bases[i].y)) {
		  float dx,dy,angle;
		  int ia;
		  if (i == nbase) {
		  dx = bases[i].x - bases[i-1].x;
		  dy = bases[i].y - bases[i-1].y;
		  }
		  else {
		  dx = bases[i+1].x - bases[i].x;
		  dy = bases[i+1].y - bases[i].y;
		  }
		  angle = atan2(dy,dx)/dtor - 90.0;
		  ia = angle;*/
		/*			    fprintf(outf,"TEX %d %10.3f %10.3f \"  %d\"\n"
					    ,ia,bases[i].x,bases[i].y,bases[i].hist_num); */
		/*xxxxxxxxxxxxxxx*/
		glc_Print(bases[i].x, bases[i].y, "   %d", bases[i].hist_num);
		/*xxxxxxxxxxxxxxx*/			 /*   } */
	    }
	}
    }

    if (TRUE/*brucc.loops*/) {
	glColor3f(1.0, 1.0, 1.0);
	/*		fprintf(outf,"CM LOOP LAELS\n"); */
	for (i=0;  i<loop_count;  i++)
	{
	    lp = loops + i;
	    /* if (okx(lp->x) && oky(lp->y)) { */
	    /*			    fprintf(outf,"CTX %10.3f %10.3f \"%d\"\n",lp->x,lp->y,lp->number); */
	    /*xxxxxxxxxxxxxxx*/
	    glc_Print(lp->x, lp->y, "%d", lp->number);
	    /*xxxxxxxxxxxxxxx*/
	    /*  } */
	}
    }
    /*  }
      } */
  /*xxxxxxxxxxxxxxx*/
    glXSwapBuffers(fl_display, FL_ObjWin(to_main.glcanvas));
    dbDebug(DB_DEBUG, "->naview\n");
    /*xxxxxxxxxxxxxxx*/
  
    return;
}

/*
void
na_scanf(char *st, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
	 int a8, int a9, int a10, int a11, int a12, int a13, int a14,
	 int a15, int a16, int a17, int a18, int a19, int a20)
*/
/*  A variant of scanf which reads standard input and then runs sscanf on it. */
/*
#define BUFFERSIZE 2000
{   char buffer[BUFFERSIZE];
    int bufsiz = BUFFERSIZE;
    char *cp;
#undef BUFFERSIZE
    
    cp = fgets(buffer,bufsiz,stdin);
    if (cp == NULL) {
	dbError(DB_TRUE, "Error in na_scanf -- End of input detected.\n");
	}
    sscanf(buffer,st,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,
		     a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);
    return;
    }
*/

float
minf2(float x1, float x2) {
    /* 
     *   Computes minimum of two floating point numbers.
   */
  
    return x1 < x2 ? x1 : x2;
}

/****************************************************/

float
maxf2(float x1, float x2) {
    /* 
     *   Computes maximum of two floating point numbers.
   */
  
    return x1 > x2 ? x1 : x2;
}

/*
int
ask(char *question) */
/*
*   Ask a yes/no question and return the answer as Boolean variable.
*/
/*
{
    char answer[2];
    char help[10];
    int done,ret;

    strcpy(help,"");
    done = FALSE;
    while (!done) {
	printf("%s %s",question,help);
	na_scanf("%1s",answer);
	if (answer[0] == 'Y' || answer[0] == 'y') {
	    ret = TRUE;
	    done = TRUE;
	    }
	else if (answer[0] == 'N' || answer[0] == 'n') {
	    ret = FALSE;
	    done = TRUE;
	    }
	else {
	    strcpy(help,"(Y or N) ");
	    }
    }
    return ret;
}
*/

void
trim(char *st, int *stlenp) {
    /*
     *   Trims blanks off the end of st. *stlenp gives the current length
     *   of st.
   */
  
    int stlen = *stlenp;
  
    while (stlen > 0) {
	if (st[stlen-1] != ' ') break;
	stlen -= 1;
    }
    *stlenp = stlen;
  
    return;
}

void
chcnbl(char *st, int stlen) {
    /*
     *   Converts all non-printable control characters into blanks.
   */
  
    int i;
  
    for (i = 1; i <= stlen; i++) {
	if (*st < 32 || *st > 126) *st = ' ';
	st += 1;
    }
  
    return;
}

void
convert_struc(rnastruc * struc) {

    int i;
  
    dbDebug(DB_DEBUG, "->convert_struc(...)\n");
  
    /*
     *   Set up an origin.
     */

    nbase = strlen(struc->bases);
    bases = (struct base *) malloc(sizeof(struct base)*(nbase+1));
    regions = (struct region *) malloc(sizeof(struct region)*(nbase+1));

    bases[0].name = 'o';
    bases[0].mate = 0;
    bases[0].hist_num = 0;
    bases[0].extracted = FALSE;
    bases[0].x = anum;
    bases[0].y = anum;

    for (i=0;i<nbase;i++) {
	bases[i+1].name = struc->bases[i];
	if(struc->types[i] != SS &&
	   struc->types[i] != HL &&
	   struc->types[i] != TL &&
	   struc->types[i] != BLL &&
	   struc->types[i] != BLR )
	    bases[i+1].mate = struc->index[i] + 1;
	else
	    bases[i+1].mate = 0;
	bases[i+1].hist_num = i + 1;
	/*
	 *	The code in this program depends on mate being false (namely zero)
	 *	where there is no mate.
	 */
	bases[i+1].extracted = FALSE;
	bases[i+1].x = anum;
	bases[i+1].y = anum;
    }
    dbDebug(DB_DEBUG, "<-convert_struc(...)\n");

    return;
}

void
free_memory(void) {

    int i, j;

    for(i=0;i<=nbase;i++) {
	if(loops[i].connections != NULL && loops[i].nconnection != 0) {
	    for(j=0;j<=loops[i].nconnection;j++) {
		if(loops[i].connections[j] != NULL)
		    free(loops[i].connections[j]);
	    }
	    free(loops[i].connections);
	}
    }
    free(loops);
    loops =NULL;
    free(bases);
    bases = NULL;
    free(regions);
    regions = NULL;

    return;
}

void
load_naview_struc(struc_t * the_struc) {

    int i;
  
    dbDebug(DB_DEBUG, "->load_naview_struc()");
  
    /*
     *   Set up an origin.
     */

    nbase = the_struc->length;
    bases = (struct base *) malloc(sizeof(struct base)*(nbase+1));
    regions = (struct region *) malloc(sizeof(struct region)*(nbase+1));

    bases[0].name = 'o';
    bases[0].mate = 0;
    bases[0].hist_num = 0;
    bases[0].extracted = FALSE;
    bases[0].x = anum;
    bases[0].y = anum;

    for (i=0;i<nbase;i++) {
	bases[i+1].name = the_struc->bases[i].name;
	bases[i+1].mate = the_struc->bases[i].mate >= 0 ? the_struc->bases[i].mate+1:0;
	bases[i+1].hist_num = the_struc->bases[i].hist_num + 1;
	/*
	 *	The code in this program depends on mate being false (namely zero)
	 *	where there is no mate.
	 */
	bases[i+1].extracted = FALSE;
	bases[i+1].x = anum;
	bases[i+1].y = anum;
	dbDebug(DB_DEBUG, "[%d] name: %c mate: %d hist: %d\n", i+1, bases[i+1].name, bases[i+1].mate, bases[i+1].hist_num);
    }
    dbDebug(DB_DEBUG, "<-convert_struc()");

    return;
}

struc_t *
save_naview(struc_t *the_struc) {

    int i;
    float xmin, ymin, xmax, ymax, xoff, yoff;

    dbDebug(DB_DEBUG, "->save_naview()");

    if(the_struc->length!=nbase)
	dbError(TRUE, "Lengths differ in nbase and struc_t");

    xmin = bases[1].x;
    xmax = bases[1].x;
    ymin = bases[1].y;
    ymax = bases[1].y;

    for(i=0;i<the_struc->length;i++) {
	the_struc->bases[i].x = bases[i+1].x;
	the_struc->bases[i].y = bases[i+1].y;

	/* get width and height of struc to center it */
	xmin = min2(the_struc->bases[i].x, xmin);
	xmax = max2(the_struc->bases[i].x, xmax);
	ymin = min2(the_struc->bases[i].y, ymin);
	ymax = max2(the_struc->bases[i].y, ymax);

	if(ss)
	    the_struc->ss = TRUE;
	else
	    the_struc->ss = FALSE;
    }

    /* offset */
    xoff = xmax-(xmax-xmin)/2;
    yoff = ymax-(ymax-ymin)/2;

    /* center struc (move all points) */
    to_main.zoom_old = to_main.zoom; /* Alain added 27/06/02 */
    for(i=0;i<the_struc->length;i++) {
	the_struc->bases[i].x -= xoff;
	the_struc->bases[i].y -= yoff;

	to_main.zoom = max2(the_struc->bases[i].x, to_main.zoom);
	to_main.zoom = max2(the_struc->bases[i].y, to_main.zoom);
	to_main.zoom = max2(fabs(the_struc->bases[i].x), to_main.zoom);
	to_main.zoom = max2(fabs(the_struc->bases[i].y), to_main.zoom);
    }
    if (to_main.zoom_old != to_main.zoom)/* Alain added 27/06/02 */
	to_main.zoom = to_main.zoom * 1.1;

    dbDebug(DB_DEBUG, "<-save_naview()");

    return the_struc;
}

struc_t *
layout_naview(struc_t *the_struc) {
    dbDebug(DB_DEBUG, "->layout_naview()");
    load_naview_struc(the_struc);
    naview();
    save_naview(the_struc);
    if (the_struc->pull)
        /* here we change the position of the base pull */
	anglexaya(the_struc);
    free_memory();
    dbDebug(DB_DEBUG, "<-layout_naview()");
    return the_struc;
}


static void
base_color (const base_t *b)
{
    switch(b->name) {
	case 'a':
	case 'A':
	    glColor3fv(to_main.col_set[6].rgb);
	    break;
	case 'c':
	case 'C':
	    glColor3fv(to_main.col_set[7].rgb);
	    break;
/* desktop */
	    /*case 'g':
	case 'G':
	    glColor3fv(to_main.col_set[8].rgb);
	    break;*/
/* Web */
	case 'g':
	case 'G':
	    glColor3fv(to_main.col_set[8].rgb);
	    break;
/* laptop
	case 'g':
	case 'G':
	    glColor3fv(to_main.col_set[2].rgb);
	    break;
*/
	case 'u':
	case 'U':
	    glColor3fv(to_main.col_set[9].rgb);
	    break;
	default:
	    glColor3fv(to_main.col_set[10].rgb);
	    break;
    }
}

static void
normal (const base_t *b1, const base_t *b2, GLfloat *nx, GLfloat *ny, GLfloat width)
{
    GLfloat	dx = b1->x - b2->x,
	dy = b1->y - b2->y,
	r = sqrt (dx * dx + dy * dy);
    *nx =  width * dy / r;
    *ny = -width * dx / r;
}

static void
ortho_step (const base_t *b1, const base_t *b2, float where, float width)
{
    GLfloat	px = (1 - where) * b1->x + where * b2->x,
	py = (1 - where) * b1->y + where * b2->y,
	nx, ny;

    normal (b1, b2, &nx, &ny, width);
    glVertex2f (px - nx, py - ny);
    glVertex2f (px + nx, py + ny);
}


static void
angle_step (const base_t *before, const base_t *center, const base_t *after, float width)
{
    static GLfloat
	dir[2] = { -1, 1 };
    int		i,
	valid;
    GLfloat	n1x, n1y,
	n2x, n2y,
	a1, b1, c1, /* a1 x + b1 y + c1 = 0  line eq. for border 1 */
	a2, b2, c2, /* a2 x + b2 y + c2 = 0  line eq. for border 2 */
	d1, d2,
	px, py,
	dx, dy,
	r;

    GLfloat x = 0.0F;
    GLfloat y = 0.0F;

    normal (before, center, &n1x, &n1y, width);
    normal (center, after,  &n2x, &n2y, width);

    a1 = n1x; b1 = n1y;
    a2 = n2x; b2 = n2y;

    for (i = 0; i < 2; i ++)
    {
	px = center->x + dir[i] * n1x;
	py = center->y + dir[i] * n1y;
	c1 = - (n1x * px + n1y * py);

	px = center->x + dir[i] * n2x;
	py = center->y + dir[i] * n2y;
	c2 = - (n2x * px + n2y * py);

	/* find intersection of lines (a1, b1, c1) and (a2, b2, c2) */
	d1 = a2 * b1 - a1 * b2;
	d2 = a2 * c1 - a1 * c2;
	valid = 0;
	if (fabs (d1) > 1e-5)
	{
	    y = - d2 / d1;
	    if (fabs (a1) > 1e-10)
	    {   x = (-c1 - b1 * y) / a1;
	    valid = 1;
	    }
	    else if (fabs (a2) > 1e-10)
	    {	x = (-c2 - b2 * y) / a2;
	    valid = 1;
	    }
	}
	if (!valid)
	{   x = px;
	y = py;
	}
	else
	{
	    dx = x - center->x;
	    dy = y - center->y;
	    r = sqrt (dx * dx + dy * dy);
	    if (r > 3 * width)
	    {
	        x = center->x;
	        y = center->y;
	    }
	}

	glVertex2f (x, y);
    }
}


struc_t *
compile_struc(struc_t *the_struc, GLenum mode) {

#define ORDER 3

#define NOBONDS (! fl_get_button(to_main.fd_brucc_opt->button46))
#define GUBONDS (fl_get_button(to_main.fd_brucc_opt->button49))
#define SPLINES (fl_get_button(to_main.fd_brucc_opt->button44))
#define QUADS   (fl_get_button(to_main.fd_brucc_opt->button48))
#define NOBACK  (fl_get_button(to_main.fd_brucc_opt->button403))
#define PRIME5  (fl_get_button(to_main.fd_brucc_opt->but400))
#define PRIME3  (fl_get_button(to_main.fd_brucc_opt->but401))
#define BASES   (fl_get_button(to_main.fd_brucc_opt->button42))
#define FREQ    (abs(atoi(fl_get_input(to_main.fd_brucc_opt->labelfreq))))
#define COLORED (fl_get_button(to_main.fd_brucc_opt->button43))

    int i;
	int j;
	int curframe;
	char *comments;

    int mate;
    int istart;
    int thefreq;

    dbDebug(DB_DEBUG, "->compile_struc()");
  
    if(the_struc == NULL)
	return NULL;

    /* Setup Display List */	

    /*    if(the_struc->disp_list == NO_DL) {
	  the_struc->disp_list = glGenLists(1);
	  if(glIsList(the_struc->disp_list)==GL_FALSE)
	  dbError(DB_TRUE,
	  "Failed to generate display list in display_a_struc()");
	  else
	  dbDebug(DB_DEBUG, "generated DL[%d]", the_struc->disp_list);
	  }
	  glNewList(the_struc->disp_list, mode);
    */

    /* define size of letter for the structure */
    if(g_xsize > 300){
	if(the_struc->length >150)
	    font.lsize =6; /**/
	else if(the_struc->length >100)
	    font.lsize =8; /*tiny*/
	else if(the_struc->length >70)
	    font.lsize =10; /*small*/
	else if(the_struc->length >50)
	    font.lsize =12; /*normal*/
	else if(the_struc->length <50)
	    font.lsize =14; /*medium*/
	glc_SetFont(font.lstyle, font.lsize);
    }else if (g_xsize > 199){
	    font.lsize =6; /*tiny*/    
	glc_SetFont(font.lstyle, font.lsize);
    }


/*================= The Bonds ====================*/
    if(NOBONDS && !the_struc->ss) {
    /*--- default value : 1-----*/
	glLineWidth(2);
	for (i=0; i < the_struc->length; i++) {
	    if ((mate = the_struc->bases[i].mate) && i < mate ) {    
		if (the_struc->bases[i].x != anum
		    && the_struc->bases[mate].x != anum) {
		    /*---- Remarq : if pseudoknot we prefer to not show this appariment-----*/
		    /*---- modif : we add the condition the_struc->bases[i].psd (==0)---*/
		    if(the_struc->bases[i].psd && GUBONDS && ((the_struc->bases[i].name == 'G'
				    && the_struc->bases[mate].name == 'U')
				   || (the_struc->bases[i].name == 'U'
				       && the_struc->bases[mate].name == 'G'))) {
			glPointSize((GLfloat)5.0);
			glColor3fv(to_main.col_set[4].rgb);
			glBegin(GL_POINTS);
			glVertex2f((the_struc->bases[i].x+the_struc->bases[mate].x)/2.0,
				   (the_struc->bases[i].y+the_struc->bases[mate].y)/2.0);
			glEnd();
			glPointSize((GLfloat)2.0);
		    }
		    else{
			float dx = the_struc->bases[i].x - the_struc->bases[mate].x;
			float dy = the_struc->bases[i].y - the_struc->bases[mate].y;
			if(dx*dx +dy*dy < 100*100) {
			    if (the_struc->bases[i].type==KNOT)
				glColor3fv(to_main.col_set[1].rgb);
			    else
				glColor3fv(to_main.col_set[3].rgb);
			    glBegin(GL_LINES);
			    glVertex2f(the_struc->bases[i].x, the_struc->bases[i].y);
			    glVertex2f(the_struc->bases[mate].x, the_struc->bases[mate].y);
			    glEnd();		
			}
		    }
		}
	    }
	}
    }
 
/*================ Backbone Color ===================*/
    glColor3fv(to_main.col_set[1].rgb);
    

    if(SPLINES) {
	int knum;
	GLfloat *ctlp, *ctlpoints, *knots;
	GLfloat j = 0.0;
	GLUnurbsObj *theNurb;

	ctlpoints = NULL;
	ctlpoints = malloc(the_struc->length * 3 * sizeof(GLfloat));
	if(ctlpoints==NULL)
	    dbError(DB_TRUE, "failed to malloc in display_a_struc()");
	ctlp = ctlpoints;
      
	for(i=0;i<the_struc->length;i++) {
	    *ctlp = (GLfloat) the_struc->bases[i].x;
	    ctlp++;
	    *ctlp = (GLfloat) the_struc->bases[i].y;
	    ctlp++;
	    *ctlp = (GLfloat) 0.0;
	    ctlp++;
	}
      
	knum = the_struc->length + ORDER;
	knots = NULL;
	knots = malloc(knum * sizeof(GLfloat));
	if(knots==NULL)
	    dbError(TRUE, "failed to malloc in display_a_struc()");
      
	for(i=0;i<ORDER;i++)
	    knots[i] = j;
	for(;i<the_struc->length;i++)
	    knots[i] = ++j;
	j++;
	for(;i<knum;i++)
	    knots[i] = j;
      
	/* Spline Interpolated Backbone */
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, (GLenum)GLU_SAMPLING_TOLERANCE, 1.0);      
      
      
	gluBeginCurve(theNurb);
	gluNurbsCurve(theNurb, 
		      knum, knots,
		      3,
		      ctlpoints, 
		      ORDER,
		      GL_MAP1_VERTEX_3);
	gluEndCurve(theNurb);
	gluDeleteNurbsRenderer(theNurb);
      
	free(knots);
	free(ctlpoints);
    }
    else if(QUADS) { 
	/* The Backbone as colored strip of quads */
	base_t *b = the_struc->bases;
	float strip_width = fl_get_slider_value(to_main.fd_brucc_opt->strip_width);

	glShadeModel (GL_FLAT);
	glBegin(GL_QUAD_STRIP);
	base_color (&b[0]);
	ortho_step (&b[0], &b[1], 0, strip_width);
	ortho_step (&b[0], &b[1], 0.5, strip_width);
	for(i=1;i<the_struc->length-1;i++) {
	    /* change color if pseudoknots */
	    if (the_struc->bases[i].psd !=0){
/* destop */
		glColor3fv(to_main.col_set[4].rgb);
/* laptop
                glColor3fv(to_main.col_set[2].rgb);*/
		angle_step (&b[i-1], &b[i], &b[i+1], strip_width);
		ortho_step (&b[i], &b[i+1], 0.5, strip_width);
	    }
	    else{
		/* color of the other are now the same : green, instead of color of the base */
		/*------------  old version  ----------------*/
		/* base_color (&b[i]); */
	   	glColor3fv(to_main.col_set[1].rgb);
		angle_step (&b[i-1], &b[i], &b[i+1], strip_width);
		ortho_step (&b[i], &b[i+1], 0.5, strip_width);
	    }
	} 
	/* i is the_struc->length-1 */
	base_color (&b[i]);
	ortho_step (&b[i-1], &b[i], 1, strip_width);
	glEnd();
    }
    else if(NOBACK) {

      /* Do not draw the backbone! */

    }
    else {
	/* Linear Interpolated Backbone */
	glBegin(GL_LINE_STRIP);
	for(i=0;i<the_struc->length;i++)
	    glVertex2f(the_struc->bases[i].x, the_struc->bases[i].y);
	glEnd();
    }
    
/*======================== pseudoknots link ====================*/
    for (i=0; i < the_struc->length; i++) {
	int colormod = 0 ;
/* desktop */
	int color = 0; /*init for pseudo link color*/
/*laptop 
	int color = 7;
*/
	if ((mate=the_struc->bases[i].psd) != 0  && i < mate)
	    if (the_struc->bases[i-1].psd == 0 || the_struc->bases[i+1].psd == 0 ||the_struc->bases[i+1].psd != mate-1 || the_struc->bases[i-1].psd != mate+1){
		if (++colormod % 2) { /* desktop */
                    int x;
                    x = color % 4;
                    color = x;
		}
		glColor3fv(to_main.col_set[color+2].rgb);
		/* laptop */
		/*
		  color = (4 + color) % 12 ;
		  glColor3fv(to_main.col_set[color].rgb);
		*/
		glBegin(GL_LINES);
		glVertex2f(the_struc->bases[i].x, the_struc->bases[i].y);
		glVertex2f(the_struc->bases[mate].x, the_struc->bases[mate].y);
		glEnd();
	    }
    }


    if(PRIME5) {
	glColor3fv(to_main.col_set[12].rgb);
	glRasterPos2d(the_struc->bases[0].x, the_struc->bases[0].y+0.5);
	passthrough_char('5');
	glCallList(dlstart + '5');
	passthrough_char('\'');
	glCallList(dlstart + '\'');
    }

    if(PRIME3) {
	glColor3fv(to_main.col_set[12].rgb);
	glRasterPos2d(the_struc->bases[the_struc->length-1].x,
		      the_struc->bases[the_struc->length-1].y+0.5);
	passthrough_char('3');
	glCallList(dlstart + '3');
	passthrough_char('\'');
	glCallList(dlstart + '\'');
    }
	
	
	/* added by Thomas on 04/07/2002 */
	/* insertion of comments into the picture => automatic comments for the KineFold server*/
	glColor3fv(to_main.col_set[12].rgb);
	
	
	if(g_comments)
	{
		
		curframe = to_main.curr_frame;
		
		comments = to_main.movie[curframe]->info;
		
	/*	fprintf(stderr, "n°%i = %s\n",curframe, comments );*/
		
		
		glRasterPos2d(1.1*to_main.zoom,-0.95*to_main.zoom);
	/*	printf("Comment position : %.2f x %.2f\n", 1.2*to_main.zoom,-0.95*to_main.zoom);*/
		
		for(j=0; j<strlen(comments); j++)
		{
			passthrough_char(comments[j]);
			glCallList(dlstart + comments[j]);
		}
		
		
		glRasterPos2d(1.1*to_main.zoom,0.95*to_main.zoom);
		for(j=0; j<strlen(comments+2); j++)
		{
			passthrough_char(comments[j+2]);
			glCallList(dlstart + comments[j+2]);
		}
	
	}	
 
    /*#define POINTS*/
    /* The Bases as big points or letters */
    if(BASES) {
	label_pos(the_struc, to_main.label_dist);
#ifdef POINTS
	glPointSize(20);
	glBegin(GL_POINTS);
#endif /* POINTS */
	glColor3fv(to_main.col_set[11].rgb);
	for(i=0;i<the_struc->length;i++) {

	    if(COLORED) {
		base_color (&the_struc->bases[i]);
	    }
#ifdef POINTS
	    glVertex2f(the_struc->bases[i].lx, the_struc->bases[i].ly);
#else
	    passthrough_char(the_struc->bases[i].name);

	    glRasterPos2d(the_struc->bases[i].lx, the_struc->bases[i].ly);
	    glCallList(dlstart + the_struc->bases[i].name);
#endif /* POINTS */
	}
#ifdef POINTS
	glEnd();
#endif /* POINTS */
    }


    if ((thefreq = FREQ) > 0) {
	label_pos(the_struc, to_main.freq_dist);
	glColor3fv(to_main.col_set[2].rgb);
	if(thefreq != 1)
	    istart = thefreq*(1 + the_struc->bases[0].hist_num/thefreq) 
		- the_struc->bases[1].hist_num + 1;
	else
	    istart = 0;
	for (i=istart; i < the_struc->length; i += thefreq) {
	    if (the_struc->bases[i].x != anum) {
		glc_Print(the_struc->bases[i].lx, the_struc->bases[i].ly,
			  "%d", the_struc->bases[i].hist_num);
	    }
	}
    }
    
    /*glEndList();
      the_struc->valid_dl = TRUE; */

    dbDebug(DB_DEBUG, "<-compile_struc()");

    return the_struc;
}

static void
label_normal (const base_t *b1, const base_t *b2,
	      GLfloat *nx, GLfloat *ny,
	      GLfloat width)
{
    GLfloat	dx = b1->x - b2->x,
	dy = b1->y - b2->y,
	r = sqrt (dx * dx + dy * dy);
    *nx =  width * dy / r;
    *ny = -width * dx / r;
}

static void
label_ortho_step (base_t *b1, base_t *b2, int where, float dist)
{
    GLfloat	px = (1 - where) * b1->x + where * b2->x,
	py = (1 - where) * b1->y + where * b2->y,
	nx, ny;

    label_normal (b1, b2, &nx, &ny, dist);

    if(where == 0) {
	b1->lx = px - nx;
	b1->ly = py - ny;
    }
    if(where == 1) {
	b2->lx = px - nx;
	b2->ly = py - ny;
    } 
}

static void
label_angle_step (const base_t *before, base_t *center, const base_t *after,
		  float width) {

    int         valid;
    GLfloat	n1x, n1y,
	n2x, n2y,
	a1, b1, c1, /* a1 x + b1 y + c1 = 0  line eq. for border 1 */
	a2, b2, c2, /* a2 x + b2 y + c2 = 0  line eq. for border 2 */
	d1, d2,
	px, py,
	dx, dy,
	r;

    GLfloat x = 0.0F;
    GLfloat y = 0.0F;

    label_normal (before, center, &n1x, &n1y, width);
    label_normal (center, after,  &n2x, &n2y, width);

    a1 = n1x; b1 = n1y;
    a2 = n2x; b2 = n2y;

    px = center->x - n1x;
    py = center->y - n1y;
    c1 = - (n1x * px + n1y * py);

    px = center->x - n2x;
    py = center->y - n2y;
    c2 = - (n2x * px + n2y * py);

	/* find intersection of lines (a1, b1, c1) and (a2, b2, c2) */
    d1 = a2 * b1 - a1 * b2;
    d2 = a2 * c1 - a1 * c2;
    valid = 0;
    if (fabs (d1) > 1e-5)
    {
	y = - d2 / d1;
	if (fabs (a1) > 1e-10)
	{   x = (-c1 - b1 * y) / a1;
	valid = 1;
	}
	else if (fabs (a2) > 1e-10)
	{	x = (-c2 - b2 * y) / a2;
	valid = 1;
	}
    }
    if (!valid)
    {   x = px;
    y = py;
    }
    else
    {
	dx = x - center->x;
	dy = y - center->y;
	r = sqrt (dx * dx + dy * dy);
	if (r > 3 * width)
	{
	    x = center->x;
	    y = center->y;
	}
    }
	
    center->lx = x;
    center->ly = y;
}

void
label_pos(struc_t *the_struc, GLfloat dist) {

    base_t *b = the_struc->bases;
    int i;

    label_ortho_step (&b[0], &b[1], 0, dist);
    for(i=1;i<the_struc->length-1;i++) {
	label_angle_step (&b[i-1], &b[i], &b[i+1], dist);
    } 
    label_ortho_step (&b[i-1], &b[i], 1, dist);
  
 
}

void
passthrough_char(const char c) {
    glPassThrough((GLfloat) c); /* The base name */
    glPassThrough(fdimlist[(int)c].width); /* The xoffset   */
    glPassThrough(fdimlist[(int)c].height); /* The yoffset   */
    /*
      fprintf(stderr, "  base: %c\n", c);
      fprintf(stderr, " width: %f\n", fdimlist[c].width);
      fprintf(stderr, "heigth: %f\n", fdimlist[c].height);
    */
}

void
anglexaya (struc_t *the_struc)
/*anglexaya()*/{
    int i, ibegin, iend, xbegin, xend, ybegin, yend ;
    int d = 1.2 ; /*inter-base length*/
/* Determine the first and last base not pair */
    i=0;
    while (the_struc->bases[i].mate==NO_MATE){
	i++;}
    ibegin=i;

    i=nbase-1;
    while (the_struc->bases[i].mate==NO_MATE){
	i--;}
    iend=i;
    
/* New position of not pairing bases */
    xbegin = the_struc->bases[ibegin-1].x;
    xend = the_struc->bases[iend+1].x;
    ybegin = the_struc->bases[ibegin-1].y;
    yend = the_struc->bases[iend+1].y;

    for(i=0 ; i<=ibegin ; i++){
	/*printf("%d - ",i);*/
	the_struc->bases[i].x = xbegin +(i-ibegin)*d ;
	the_struc->bases[i].y = ybegin ;}
    for(i=iend ; i<=nbase-1 ; i++){
	/*printf("%d - ",i);*/
	the_struc->bases[i].x = xend +(i-iend)*d;
	the_struc->bases[i].y = yend ;}
}
