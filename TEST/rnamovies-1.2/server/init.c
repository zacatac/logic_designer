#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

#include "viewglobal.h"
#include "init.h"


extern char *optarg;
extern int optind;

extern char rcs_id[];
extern char rcs_date[];
extern char rcs_rev[];

int lglass_cursor;
int rot_cursor;

void parse_args(int *argc, char *argv[]) {

  int c;
  
  while ((c = getopt(*argc, argv, "vhH")) != EOF)
    switch (c) {
      /* version */
    case 'v':
      printf("RNAmovieserver version 1.2\n");
      exit(EXIT_SUCCESS);
      /* help */
    case 'h':
    case 'H':
      printf("usage: %s [filename]\n", argv[0]);
      exit(EXIT_SUCCESS);
    }

  if(optind < *argc) {
      to_main.filename = (char *)strdup(argv[optind]);
  }

  return;
}


void setup_defaults(void) {

  to_main.filename = NULL;
  to_main.fileptr = NULL;
  to_main.n_interp = 0;
  to_main.n_struc = 0;
  to_main.n_frame = 0;
  to_main.struc_list = NULL;
  to_main.movie = NULL;
  to_main.curr_frame = NO_FR;
  return;
}





