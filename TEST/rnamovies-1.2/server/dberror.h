/*
-------------------------------------------------------------------------------
 $Id: dberror.h,v 1.2 2003/07/16 14:18:16 jkrueger Exp $
 Headerfile
 Multi Agenten Kommunikations Subsystem
 Dirk J. Evers
 Technische Fakultaet
 Universitaet Bielefeld
 email: dirk@techfak.uni-bielefeld.de

 Filename: $RCSfile: dberror.h,v $
 Revision: $Revision: 1.2 $
 Date:     $Date: 2003/07/16 14:18:16 $

 Revision Log:
 $Log: dberror.h,v $
 Revision 1.2  2003/07/16 14:18:16  jkrueger
 .delteete some uneseused code

 Revision 1.1  2003/07/11 11:30:35  jkrueger
 Initial revision

 Revision 1.1  1998/11/04 08:15:21  asczyrba
 Initial revision

 Revision 1.1  1997/05/02 11:20:14  dirk
 Initial revision


-------------------------------------------------------------------------------
*/

#ifndef _DBERROR_H_
#define _DBERROR_H_

/* defines */

/* Debug Levels */

#define DB_ALL 100
#define DB_DEBUG 50
#define DB_HINT 30
#define DB_USER 10
#define DB_NON 0

#define DB_ERRLEN 256

#define DB_ARGS 2
#define DB_FATAL 1
#define DB_NONFATAL 0
#define DB_TRUE 1
#define DB_FALSE 0

/* external variables */
extern int debuglevel;
extern char *debugname;
extern int debugsys;
extern int debugsyslevel;



/* function prototypes */
void dbDebug(const int debuglevel, const char *fmt, ...);
void dbError(const int exitflag, const char *fmt, ...);
void dbPrintError(const char *buf);

#endif /* _DBERROR_H_ */
