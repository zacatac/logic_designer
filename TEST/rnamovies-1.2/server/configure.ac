AC_INIT([rnamovies_server], [1.2])
AC_CONFIG_AUX_DIR([.])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE()
AC_PROG_CC

dnl Checks for library functions.
AC_FUNC_VPRINTF
AC_CHECK_FUNCS(strdup strtol)
AC_CHECK_LIB(m,pow)

dnl Generate Output
AC_CONFIG_FILES([Makefile client/Makefile])
AC_OUTPUT