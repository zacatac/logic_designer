#ifndef INIT_H
#define INIT_H


#include "dberror.h"

extern int lglass_cursor;
extern int rot_cursor;

void parse_args(int *argc, char *argv[]);
void setup_defaults(void);
void setup_backptrs(void);

#endif /* INIT_H */
