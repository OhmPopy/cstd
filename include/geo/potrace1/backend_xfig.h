/* Copyright (C) 2001-2005 Peter Selinger.
   This file is part of potrace. It is pfree software and it is covered
   by the GNU General Public License. See the file COPYING for details. */

/* $Id: backend_xfig.h,v 1.5 2005/02/22 21:31:23 selinger Exp $ */

#ifndef BACKEND_XFIG_H
#define BACKEND_XFIG_H

#include "potracelib.h"
#include "main.h"

int page_xfig(FILE *fout, potrace_path_t *plist, imginfo_t *imginfo);

#endif /* BACKEND_XFIG_H */

