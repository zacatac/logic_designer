/*
-------------------------------------------------------------------------------

 RNA secondary structure parser
 Simple Parse C file

 Author: Dirk Evers
         Technische Fakultaet
         Arbeitsgruppe Praktische Informatik
         Universitaet Bielefeld
         dirk@TechFak.Uni-Bielefeld.DE
                                      
 Date: 12.09.97

 RCS-Id:
 $Id: simple_parse.c,v 1.6 1999/08/27 16:49:59 dirk Exp $

 RCS-Log:
 $Log: simple_parse.c,v $
 Revision 1.6  1999/08/27 16:49:59  dirk
 first configure version basepair bug fixed

 Revision 1.4  1999/01/11 17:25:49  asczyrba
 check if input file is ct format,
 parser for ct format is missing...

 Revision 1.3  1998/11/09 17:12:24  asczyrba
 report parse error, if brackets don't match

 Revision 1.2  1998/11/05 15:15:27  asczyrba
 progress display included

 Revision 1.1  1998/11/04 08:14:25  asczyrba
 Initial revision

 Revision 1.4  1998/11/03 13:27:56  dirk
 can't remember

 Revision 1.3  1998/06/22 10:18:44  dirk
 fixed parse_marc initialization bug
 still a big hack though

 Revision 1.2  1998/06/18 16:03:53  dirk
 pserockas version GK Begehung

 Revision 1.1  1997/09/12 11:01:06  dirk
 Initial revision


-------------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <strings.h>

#include "dberror.h"
#include "viewglobal.h"

/* local prototypes */

int is_ct(FILE *fileptr);
int parse_ct(int strucs);
int parse_marc(int strucs);
int parse_alain(int strucs); /*for isambert format*/
int count_strucs(FILE *fileptr);
int count_strucs_rnm(FILE *fileptr);
int count_strucs_ct(FILE *fileptr);
char *fgetline(FILE *stream, size_t n);
char *fgetline_r(char *buffer, size_t *size, FILE *stream, size_t n) ;

/* functions */

int
parse_strucs(int strucs) {
    if (is_ct(to_main.fileptr)==1) {
	return parse_ct(strucs);
    }
    else {
	if (is_ct(to_main.fileptr)==0)
	    return parse_marc(strucs);
	else 
	    return parse_alain(strucs);
    }
}

int
parse_marc(int strucs) {

    static rnastruc marcstruc;

    char *the_title = NULL;
    static char *the_bases = NULL;
    static char *the_brack = NULL;
    size_t buf_size = 0;
    static char *comment = NULL;
    static int  *the_index = NULL;
    static int  *the_types = NULL;
    static int  *the_stack = NULL;
    static int  *the_stack2 = NULL;
    static int  *the_stack3 = NULL;

    static int pause = FALSE;
    int count;
    int stkptr = 0;
    int length;
    int bracklen;
    int i;
    int s_i = 0;
    int percent = 10;
    int old_bracklen = 0;


    /* pseudoknot specific variables */
    int psdptr2 = 0;
    int psdptr3 = 0; 

    dbDebug(DB_DEBUG, "->parse_marc()");

    /* been there, done that */
    if(the_bases != NULL) {
      free(the_bases);
      the_bases = NULL;
    }
    /* been there, done that */
    if(the_brack != NULL) {
      free(the_brack);
      the_brack = NULL;
    }

    /* clear title */
    if(to_main.title != NULL) {
      free(to_main.title);
      to_main.title = NULL;
    }

    the_title = fgetline(to_main.fileptr, ALLOC_INC);
    if(feof(to_main.fileptr)) {
	dbError(DB_FALSE, "premature EOF encountered in File: %s", to_main.filename);
	free_struc_list();
	return 1;
    }

    if(the_title[0] == '>') { /* it's a comment */
	length = strlen(the_title);
	if(the_title[length-1] == '\n')
          the_title[length-1] = '\0';
	dbDebug(DB_DEBUG, "the_title: <%s>", the_title);
	
	to_main.title = the_title;
	/*rem : debut du titre a : to_main.the_name[1]*/
	if (to_main.title[0] == '!')
	    marcstruc.pull = 1;
	else
	    marcstruc.pull = 0;

        /* processed the title, now get the bases */
        the_bases = fgetline(to_main.fileptr, ALLOC_INC);
        if(feof(to_main.fileptr)) {
          dbError(DB_FALSE, "premature EOF encountered in File: %s", to_main.filename);
          free_struc_list();
          return 1;
	}
    }
    else {
      /* no title: then it must be the bases, so copy them */
      the_bases = strdup(the_title);
      the_title[0] = '\0';
    }

    /* truncate the possible newline */
    length = strlen(the_bases);
    if(the_bases[length-1]=='\n') {
	the_bases[length-1]='\0';
	length--;
    }
    /* sanity check, again */
    if(length == 0) {
	dbError(DB_FALSE, "No bases found in input file: %s", to_main.filename);
	free_struc_list();
	return 1;
    }


    marcstruc.bases = the_bases;
    /* finished processing the bases (they may be truncated later on) */
    dbDebug(DB_DEBUG, "the_bases: <%s>", the_bases);
    the_brack = strdup("");

    while(!feof(to_main.fileptr)) {
	the_brack = fgetline_r(the_brack, &buf_size, to_main.fileptr, ALLOC_INC);
	
	bracklen = strlen(the_brack);
	if(the_brack[bracklen-1]=='\n') {
	    the_brack[bracklen-1]='\0';
	    bracklen--;
	}
	dbDebug(DB_DEBUG, "the_brack: <%s>", the_brack);
	
        /* could add support for multiple comment lines */
        /* for now last  line wins */
        if(the_brack[0]=='#'){
	    /* save for next round */
	    comment = strdup(&the_brack[1]);
	    continue;
        }
        /* next time round */
        if(comment) {
	    /* copy pointer */
	    /* need to change this when rnastruc info is (char *). */
	    strcpy((char *)marcstruc.info, comment);
	    /* set for next use */
	    comment = NULL;
        }
	
        count = 0;
        sscanf(the_brack, "pause%n", &count);
        if(count==5) {
	    /* check for pause succeeded */
	    pause = TRUE;
	    continue;
        }
	
        /* next time round */
        if(pause == TRUE) {
	    marcstruc.pause = TRUE;
	    pause = FALSE;
        }
        else {
	    marcstruc.pause = FALSE;
        }
        /* no more continues, start counting structures */
        s_i++; /* structure i */
	
	if(bracklen < old_bracklen) {
	    break;
	}
	old_bracklen = bracklen;
	
	/* calculate percentage of parsed structures */
	if ((s_i*100 / strucs) >= percent) {
	    percent += 10;
	}
	
	
        if(bracklen > length) {
	    dbError(DB_FALSE, "nucleotide length smaller than structure length in File: %s", to_main.filename);
	    free_struc_list();
	    return 1;
        }
	
        /* if bracklen smaller than nucleotide length then truncate the base string */
        the_bases[bracklen] = '\0';
	
	dbDebug(DB_DEBUG, "bracklen: %d", bracklen);
	dbDebug(DB_DEBUG, "truncated bases: <%s>", the_bases);
	
        /* now we now these sizes too, so get some mem */
        the_index = (int *)realloc(the_index, sizeof(int) * (bracklen + 1));
        the_types = (int *)realloc(the_types, sizeof(int) * (bracklen + 1));
        the_stack = (int *)realloc(the_stack, sizeof(int) * (bracklen + 1));
        the_stack2 = (int *)realloc(the_stack2, sizeof(int) * (bracklen + 1));
        the_stack3 = (int *)realloc(the_stack3, sizeof(int) * (bracklen + 1));
	
        /* repointer into rnastruc */
        marcstruc.index = the_index;
        marcstruc.types = the_types;

        

	for(i=0;i<bracklen;i++) {
	    switch(the_brack[i]) {
		case '.':
		    the_index[i] = NO_MATE;
		    the_types[i] = SS;
		    break;
		case '(':
		    the_stack[stkptr++] = i;
		    the_types[i] = SR;
		    break;
                    /* test pseudoknots pairing*/
		case '[' :
		    the_stack2[psdptr2++] = i ;
		    the_types[i] = SP;
		    break;
		case ']':
		    if (psdptr2 == 0) {
			parse_err(s_i, i);
			return 1;	
		    }
		    else {
			the_index[i] = the_stack2[--psdptr2];
			the_index[the_index[i]] = i;
			the_types[i] = SP;
			break;
		    } 
		case '{' :
		    the_stack3[psdptr3++]=i ;
		    the_types[i]=SP ;
		    break;
		case '}':
		    if (psdptr3 == 0) {
			parse_err(s_i, i);
			return 1;	
		    }
		    else {
			the_index[i] = the_stack3[--psdptr3];
			the_index[the_index[i]] = i;
			the_types[i] = SP;
			break;
		    }
                    /* end of test */
		case ')':
		    if (stkptr == 0) {
			parse_err(s_i, i);
			return 1;	
		    }
		    else {
			the_index[i] = the_stack[--stkptr];
			the_index[the_index[i]] = i;
			the_types[i] = SR;
			break;
		    }
		default:
		    dbError(DB_FALSE, "unknown token: %c in File: %s", &(the_brack[i]), to_main.filename);
		    free_struc_list();
		    return 1;	
	    }
	}
	/*    for(i=0;i<bracklen;i++) {
	      dbDebug(DB_DEBUG, "%d ", the_index[i]);
	      } */
	if (!(stkptr == 0) || !(psdptr2 == 0) || !(psdptr3 == 0)) {
          /* some opening brackets do not match up */
	    parse_err(s_i, -1);
	    return 1;	
	}
	else {

          layout_naview(add_movie_struc(&marcstruc));
          marcstruc.info[0] = '\0';
	}
    }
    dbDebug(DB_DEBUG, "<-parse_marc()");

    return 0;
}


/*-------------------------- specific function for rnaviz format --------------*/
int
parse_alain(int strucs) {

    static rnastruc alainstruc;

    char *the_title = NULL;

    static char * the_bases = NULL;
    static char * the_brack = NULL;
    static char * the_trunc_bases = NULL;

    size_t buf_size_bases = 0;
    size_t buf_size_brackets = 0;

    static int * the_index = NULL;
    static int * the_types = NULL;
    static char * the_bases2 = NULL;
    static char * the_brack2 = NULL;
    static int pause = FALSE;

    int count;
    int length;
    int bracklen;
    short s_i = 0;
    short percent = 10;

    int i;
    int old_bracklen = 0 ;

    char *s;
    char psd[1000];
    char knt[1000];
    char test[6];
    int pos = 0 ;
    int table_helix[1000];
    int table_psd[100];
    int table_knot[100];
    int pair_type = UNDEF;
    int maxpsd, maxknt;
    int j = 0;
    int k = 0;
    short brackopen;
    short newsynth;
    int the_ket[1100];

    int olds_i = 0;

#if (VERBOSE > -9)
    printf("\n---------------------------------------------------\n");
    printf(" Bonjour et bienvenu sur notre partie de programme");
    printf("\n---------------------------------------------------\n");
#endif

    /* been there, done that */
    if(the_bases != NULL) {
      free(the_bases);
      the_bases = NULL;
    }
    /* been there, done that */
    if(the_brack != NULL) {
      free(the_brack);
      the_brack = NULL;
    }
    /* clear title */
    if(to_main.title != NULL) {
      free(to_main.title);
      to_main.title = NULL;
    }

    the_title = fgetline(to_main.fileptr, ALLOC_INC);
    if(feof(to_main.fileptr)) {
	dbError(DB_FALSE, "premature EOF encountered in File: %s", to_main.filename);
	free_struc_list();
	return 1;
    }

    if(the_title[0] == '<') { /* it's a comment */
	length = strlen(the_title);
	if(the_title[length-1] == '\n')
          the_title[length-1] = '\0';
	dbDebug(DB_DEBUG, "the_title: <%s>", the_title);
	
	to_main.title = the_title;
	/*rem : debut du titre a : to_main.the_name[1]*/
	if (to_main.title[0] == '!')
	    alainstruc.pull = 1;
	else
	    alainstruc.pull = 0;

        /* processed the title, now get the bases */
        the_bases = fgetline_r(the_bases, &buf_size_bases, to_main.fileptr, ALLOC_INC);

        if(feof(to_main.fileptr)) {
          dbError(DB_FALSE, "premature EOF encountered in File: %s", to_main.filename);
          free_struc_list();
          return 1;
	}
    }
    else {
      /* no title: then it must be the bases, so copy them */
      the_bases = strdup(the_title);
      the_title[0] = '\0';
    }

    the_bases2 = (char *)realloc(the_bases2, sizeof(char) * strlen(the_bases)); 

    strcpy(the_bases2,the_bases);

    length = strlen(the_bases);
    if(the_bases[length-1]=='\n') {
	the_bases[length-1]='\0';
	length--;
    }
    if(length == 0) {
	dbError(DB_FALSE, "No bases found in input file: %s", to_main.filename);
	free_struc_list();
	return 1;
    }

    /* finished processing the bases (they may be truncated later on) */
    
    dbDebug(DB_DEBUG, "the_bases: <%s>", the_bases);
    /* need an empty string? */
    the_brack = strdup("");

    the_index = (int *)realloc(the_index, sizeof(int) * (2*length+2));
    the_types = (int *)realloc(the_types, sizeof(int) * (2*length+2));

    alainstruc.index = the_index;
    alainstruc.types = the_types;

    while(!feof(to_main.fileptr)) {
      the_bases = fgetline_r(the_bases, &buf_size_bases, to_main.fileptr, ALLOC_INC);
      if(feof(to_main.fileptr)) {
        /* uneven number of lines encountered, stop parsing */
        continue;
      }
      
      count = 0;
      sscanf(the_bases, "pause%n", &count);
      if(count==5) {
	  /* check for pause succeeded */
	  pause = TRUE;
#if (VERBOSE > -9)
	    printf(" * PAUSE DETECTED *",pause);
#endif
	  continue;
      }
      /* next time round */
      if(pause == TRUE) {
#if (VERBOSE > -9)
	    printf(" * on passe alors !!! *",pause);
#endif
	  alainstruc.pause = TRUE;
	  pause = FALSE;
      }
      else {
	  alainstruc.pause = FALSE;
      }   

      the_brack = fgetline_r(the_brack, &buf_size_brackets, to_main.fileptr, ALLOC_INC);   
      
      s_i++; /* structure i */

#if (VERBOSE > -7)
	printf("     Num de la structure lue : %d\n", s_i);
#endif

	if(the_bases[0]=='<'){/*use with new definition of to_main.the_name*/
#if (VERBOSE > -7)
	    printf("\n\n **********************\n Nouvelle sequence \n\n");
#endif

            the_bases2 = (char *)realloc(the_bases2, sizeof(char) * buf_size_brackets);
	    strcpy(the_bases2,the_brack);
            the_bases = fgetline_r(the_bases, &buf_size_bases, to_main.fileptr, ALLOC_INC);
            if(feof(to_main.fileptr)) {
              /* uneven number of lines encountered, stop parsing */
              continue;
            }
            the_brack = fgetline_r(the_brack, &buf_size_brackets, to_main.fileptr, ALLOC_INC);
            /* new structure, then pause at first frame, set title */
            alainstruc.pause = TRUE;

	}else{
	    olds_i=s_i;
	}

	if (strchr(the_bases,'#') == 0 )
	    bracklen =0;
	else {
	    s = strpbrk(the_bases, "|");
	    bracklen = (s - the_bases)-7;
	    if(the_brack[bracklen-1]=='\n') {
		the_brack[bracklen-1]='\0';
		bracklen--;
	    }
	}
	/*dbDebug(DB_DEBUG, "the_brack: <%s>", the_brack);*/

	if(strchr(the_brack,'H') != 0){ /* complete structure ? */
	    if (bracklen < old_bracklen){ /* test to know if it's a new structure*/

#if (VERBOSE > -9)
		printf("****************** New synthesis *********************\n");
#endif

		newsynth = TRUE;
	    }
	    else newsynth = FALSE;
	  
	    /* calculate percentage of parsed structures */
	    if ((s_i * 100 * 2 / strucs) >= percent) {
              /*we use the same function to rnm to count nb of sructure*/
		percent += 10;
	    }

	    /* recuperation du commentaire */
            strncpy((char *)alainstruc.info,strchr(the_bases,'|')+2,strpbrk(the_bases, "#")-strpbrk(the_bases, "|")-9);
            

	    /* recuperation des pseudoneouds */
	    for (i=0 ; i<100 ; i++)
		table_psd[i] = -1 ;
	    /*strcpy (psd,strchr(the_bases,'#')+2); change 14/02/04 */
	    if (strchr(the_bases,'&') != NULL ){
		strncpy(psd,strchr(the_bases,'#')+2,strpbrk(the_bases, "&")-strpbrk(the_bases, "#")-1);
	    }
	    else
		strcpy (psd,strchr(the_bases,'#')+2);
	    pos=0;
	    for (s=psd ; (s=strtok(s," "))!=NULL ; s = NULL){
		sscanf (s,"%d",&table_psd[pos++]);
	    }
	    maxpsd = pos ;
#if VERBOSE>-5
	    printf("pos %i\n",pos);
#endif
	    
	    /* recuperation des noeuds change 14/02/04 new */
	    for (i=0 ; i<100 ; i++)
		table_knot[i] = -1 ; 
	    if (strchr(the_bases,'&') != NULL ){
		strcpy (knt,strchr(the_bases,'&')+2);
		pos=0;
		for (s=knt ; (s=strtok(s," "))!=NULL ; s = NULL){
		    sscanf (s,"%d",&table_knot[pos++]);
		}
		maxknt = pos ;
	    }
	    else
		maxknt =0;
	    
	    /* recherche des helices */
	    pos =0;
            the_brack2 = (char *)realloc(the_brack2, sizeof(char) * buf_size_brackets);
	    strcpy(the_brack2,the_brack);
	    for (s=the_brack2 ; (s=strtok(s," '-"))!=NULL && (s-the_brack2)<bracklen ; s = NULL)
		sscanf(s,"%d",&table_helix[pos++]);
	  
	    /* traitement des appariements */
	    pos =0;
	    brackopen = FALSE ;
	    for (i=0 ; i<bracklen ; i++){
		switch (the_bases[i]){
		    case '[' :
			pair_type = SR ;
			for (j=0 ; j<maxpsd ; j++)
			    if (table_helix[pos] == table_psd[j])
				pair_type = SP ;
			for (j=0 ; j<maxknt ; j++) /* change 14/02/04 new */
			    if (table_helix[pos] == table_knot[j])
				pair_type = KNOT ;  
			strncpy(test,&the_brack[i+2],5);
			if (strchr(test,'\'') != NULL){
			    brackopen =TRUE; /* seconde partie de la parenthese */
			    k=0;
			}
			break ;
		      
		    case ']' :
			brackopen = FALSE;
			the_ket[pos] = i;
			pos++;
			break ;
		      
		    case '^' :
			brackopen = FALSE;
			the_ket[pos] = i;
			pos++;
			pair_type = SR ;  
			for (j=0 ; j<maxpsd ; j++)
			    if (table_helix[pos] == table_psd[j])
				pair_type = SP ; 
			for (j=0 ; j<maxknt ; j++) /* change 14/02/04 new */
			    if (table_helix[pos] == table_knot[j])
				pair_type = KNOT ;
			strncpy(test,&the_brack[i+2],5);
			if (strchr(test,'\'') != NULL){
			    brackopen =TRUE; /* second part of the bracket */
			    k=0;
			}
			break ;
		    default :
			if (brackopen == TRUE){  /* second part of the helix */
			    for (j=0; j <pos ; j++)
				if (table_helix[j] == table_helix[pos]){
				    k++;
				    the_index[i]= the_ket[j]-k ;
				    the_index[the_index[i]] = i ;
				    the_types[i] = pair_type ;
				    the_types[the_index[i]] = pair_type ;
				}
			}
			else{
			    the_index[i]=NO_MATE;
			    the_types[i]=SS;
			}
		}
	    }
	    /* il faut enlever tout ce qui n'est pas du codage */
	    for (i = 0 ; i < length ; i++){ 
		if (the_index[2*i+1] !=NO_MATE){
		    the_index[i] = the_index[2*i+1]/2; /* take caution that indice is 2 more than real index */
		    the_types[i] = the_types[2*i+1] ;
		}
		else{
		    the_index[i]= NO_MATE;
		    the_types[i] = SS ;
		}
	    }
	    for (i = 0 ; i < length ; i++){
		the_index[i+length]= NO_MATE;
		the_types[i+length] = SS;
	    }
            the_trunc_bases = (char *)realloc(the_trunc_bases, (1+(bracklen-1)/2) * sizeof(char));
            if(the_trunc_bases == NULL) {
              perror("malloc failed");
              exit(1);
            }
	    strncpy(the_trunc_bases,the_bases2,(bracklen-1)/2);
	    the_trunc_bases[(bracklen-1)/2] = '\0';
            alainstruc.bases = the_trunc_bases;
#if (VERBOSE > -9)
	    printf("PAUSE %d ->",alainstruc.pause);
#endif
	    layout_naview(add_movie_struc(&alainstruc));
#if (VERBOSE > -9)
	    printf("<- PAUSE %d \n",alainstruc.pause);
#endif
	    old_bracklen = bracklen;
	}
	else {
	    /*printf("****************** structure file already incomplete *********************\n");*/
	}
    }
#if (VERBOSE > -9)
    printf("------------------------------------------------------------\n"); 
    printf("            ce message indique que tout c'est bien passe\n");
    printf("------------------------------------------------------------\n");
#endif
    
    dbDebug(DB_DEBUG, "<-parse_alain()");
    
    return 0;
}
/* -------------------------------------------------------------------------*/

int
parse_ct(int strucs) {

    return 1;
}


void
parse_err(int s_i, int pos) {
  
  free_struc_list();

  return;
}


int
count_strucs(FILE *fileptr) {
    if (is_ct(fileptr)==1) {
	int strucs;
	strucs = count_strucs_ct(fileptr);
	return strucs;
    }
    else {
	return count_strucs_rnm(fileptr);
    }
}


int
count_strucs_ct(FILE *fileptr) {
    char inp[100000];
    char name[20];
    int bases;
    int n;
    float energy;
    int strucs = 0;

    fgets(inp, 99999, fileptr);
    do {
	/* get description line */
	n = sscanf(inp, "%d ENERGY = %f >%20s\n", &bases, &energy, name);
	if(n>=2)
	    strucs++;
	fgets(inp, 99999, fileptr);
    } while (!feof(fileptr));

    rewind(fileptr);
    return strucs;
}


int
count_strucs_rnm(FILE *fileptr) {

    char *inp = strdup("");
    size_t inp_size = 0;

    int strucs = 0;

    rewind(fileptr);

    do {
        inp = fgetline_r(inp, &inp_size, fileptr, ALLOC_INC);
	if(inp[0] == '>')
	    continue;
	strucs++;
    } while (!feof(fileptr));

    strucs--;
    free(inp);
    rewind(fileptr);

    return strucs;
}


int
is_ct(FILE *fileptr) {
    char inp[100000];
    char name[20];
    int bases;
    int n;
    float energy;

    fgets(inp, 99999, fileptr);

    n = sscanf(inp, "%d ENERGY = %f >%20s\n", &bases, &energy, name);
  
    rewind(fileptr);

    if (n>=2) { /* seems to be ct format */
	return 1;
    }
    else {
	if(inp[0] == '>')
	    return 0; /* seems to be rnm format */
	else
	    return 2 ;
    }
}


char *
fgetline(FILE *stream, size_t n) {
  /* get a newline terminated string from file */
  /* do the memory allocation as needed in steps of size n */

  char *buffer = NULL;
  size_t offset = 0;

  buffer = strdup("");

  while((buffer[strlen(buffer)-1] != '\n') && !feof(stream)) {
    buffer = realloc(buffer, offset + n);
    if(buffer == NULL) {
      perror("can't malloc\n");
      exit(1);
    }
    fgets(buffer + offset, n, stream);
    offset += n - 1;
  }

  return buffer;
}


char *
fgetline_r(char *buffer, size_t *size, FILE *stream, size_t n) {
  /* get a newline terminated string from file */
  /* do the memory allocation as needed in steps of size n */

  size_t offset = 0;

  if(buffer == NULL)
    buffer = strdup("");
  else
    buffer[0] = '\0';

  while((buffer[strlen(buffer)-1] != '\n') && !feof(stream)) {
    /* We start to realloc once the buffer has to be larger than the one given */
    if(offset + n > (*size))
      buffer = realloc(buffer, offset + n);
    if(buffer == NULL) {
      perror("can't malloc\n");
      exit(1);
    }
    fgets(buffer + offset, n, stream);
    offset += n - 1;
  }

  *size = offset + 1;

  return buffer;
}
