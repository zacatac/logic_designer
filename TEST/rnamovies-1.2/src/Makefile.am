# Copyright (C) 1999 Dirk Evers <dirk@TechFak.Uni-Bielefeld.DE>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

EXTRA_DIST = viewforms.fd

SUFFIXES = .fd

.fd.c:
	$(FDESIGN) -convert $<

.fd.h:
	$(FDESIGN) -convert $<


bin_PROGRAMS = rnamovies

rnamovies_SOURCES = dberror.c memory.c viewforms.c                            \
                    movie.c postscript.c viewmain.c save.c init.c             \
                    naview.c simple_parse.c xfonts.c fill_viewforms_cb.c      \
                    config.h dberror.h init.h memory.h viewforms.h            \
                    viewglobal.h help.h                                       \
                    accept.xpm cancel.xpm forward.xpm quit.xpm rewind.xpm     \
                    backward.xpm circle.xpm pause.xpm radial.xpm squiggle.xpm \
                    button1.xpm close.xpm play.xpm reload.xpm fit.xpm         \
                    lglass_m.xbm  lglass_s.xbm  rot_m.xbm  rot_s.xbm



