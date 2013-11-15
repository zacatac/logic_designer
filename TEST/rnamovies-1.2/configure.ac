# Copyright (C) 1999 Dirk Evers <dirk@TechFak.Uni-Bielefeld.DE>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

dnl Initialization
AC_INIT([rnamovies], [1.2], [dirk@CeBiTec.Uni-Bielefeld.DE])
AC_CONFIG_AUX_DIR([.])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([rnamovies],[1.2])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PATH_PROGS([FDESIGN],[fdesign])

dnl Checks for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([libgen.h X11/Xlib.h GL/gl.h GL/glx.h forms.h glcanvas.h gd.h])
AC_CHECK_HEADERS([X11/Xutil.h], [], [],
[#if HAVE_X11_XLIB_H
# include <X11/Xlib.h>
# endif
])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for standard library functions.
AC_CHECK_LIB([m],[pow])
AC_CHECK_FUNCS([strdup strtol])
AC_FUNC_VPRINTF

dnl check for basename
dnl AC_CHECK_LIB([gen], [basename])

dnl Checks for X11
AC_PATH_XTRA
LDFLAGS="$LDFLAGS $X_LIBS"
LIBS="$X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
CPPFLAGS="$CPPFLAGS $X_CFLAGS"
dnl Needed if shared mem used
AC_CHECK_LIB([Xext], [XShmCreateImage])

dnl Xpm library check
AC_CHECK_LIB([Xpm], [XpmCreateBufferFromImage])

dnl Check for OpenGL
AC_CHECK_LIB([GL], [glOrtho])
AC_CHECK_LIB([GLU], [gluPerspective])

dnl check for cforms library
AC_CHECK_LIB([forms], [fl_initialize])
AC_CHECK_LIB([formsGL], [fl_add_glcanvas])

dnl check for gd image library
AC_CHECK_LIB([gd], [gdImagePng])

dnl the config files
AC_CONFIG_FILES([Makefile doc/Makefile m4/Makefile src/Makefile examples/Makefile])

dnl configure server too
AC_CONFIG_SUBDIRS([server])

dnl Generate Output
AC_OUTPUT()

dnl print out essential unfound libs

if test x$ac_cv_lib_forms_fl_initialize = xno; then
cat <<EOF
*** configure could not find the forms library!
*** RNA Movies will not compile without this library.
EOF
fi

if test x$ac_cv_lib_formsGL_fl_add_glcanvas = xno; then
cat <<EOF
*** configure could not find the formsGL library!
*** RNA Movies will not compile without this library.
EOF
fi

if test x$ac_cv_lib_GL_glOrtho = xno; then
cat <<EOF
*** configure could not find the GL library!
*** RNA Movies will not compile without this library.
EOF
fi

if test x$ac_cv_lib_GLU_gluPerspective = xno; then
cat <<EOF
*** configure could not find the GLU library!
*** RNA Movies will not compile without this library.
EOF
fi

if test x$ac_cv_lib_gd_gdImagePng = xno; then
cat <<EOF
*** configure could not find the gd library!
*** RNA Movies will not compile without this library.
EOF
fi

if test x$ac_cv_lib_Xpm_XpmCreateBufferFromImage = xno; then
cat <<EOF
*** configure could not find the Xpm library!
*** RNA Movies will not compile without this library.
EOF
fi


