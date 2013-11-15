/*
-------------------------------------------------------------------------------
 $Id: dberror.c,v 1.1 1998/11/04 08:14:25 asczyrba Exp $
 C-code File
 Dirk J. Evers
 Technische Fakultaet
 Universitaet Bielefeld
 email: dirk@techfak.uni-bielefeld.de

 Filename: $RCSfile: dberror.c,v $
 Revision: $Revision: 1.1 $
 Date:     $Date: 1998/11/04 08:14:25 $

 Revision Log:
 $Log: dberror.c,v $
 Revision 1.1  1998/11/04 08:14:25  asczyrba
 Initial revision

 Revision 1.1  1997/05/02 11:15:03  dirk
 Initial revision


-------------------------------------------------------------------------------
*/

/* includes */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>

#include "dberror.h"

/* global variables */

int debuglevel = DB_NON;
char *debugname = "DBDEBUG";
char *errorname = "DBERROR";
int debugsys = DB_FALSE;
int debugsyslevel = LOG_DEBUG;

/* functions */

void   /* Write standardized debug message to Error Channel */
dbDebug(const int level, const char *fmt, ...) {

  static char the_str[DB_ERRLEN];
  va_list args;

  if(level <= debuglevel)
    {
      va_start(args, fmt);
      sprintf(the_str,"%s:[%d]:", debugname, level);
      vsprintf(the_str + strlen(the_str), fmt, args);
      dbPrintError(the_str);
      va_end(args);
    }
  return;
}

void   /* Write standardized debug message to Error Channel */
dbError(const int exitflag, const char *fmt, ...) {

  static char the_str[DB_ERRLEN];
  va_list args;
  
  va_start(args, fmt);
  sprintf(the_str,"%s:[%d]:", errorname, exitflag);
  vsprintf(the_str + strlen(the_str), fmt, args);
  dbPrintError(the_str);
  va_end(args);
  
  if(exitflag >= DB_TRUE)
    exit(exitflag);
  else
    return;
}

void /* Print the error */
dbPrintError(const char *buf) {

  if(debugsys == DB_TRUE) {
    syslog(debugsyslevel, buf);
  }
  else {
    fflush(NULL);
    fputs("\n", stderr);
    fputs(buf, stderr);
    fputs("\n", stderr);
  }

  return;
}
