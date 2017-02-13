/**
 *
 * $Id: ArrowBG.h,v 1.3 2000/09/28 08:20:58 amai Exp $
 *
 * Copyright (C) 1995 Free Software Foundation, Inc.
 * Copyright (C) 1995-2000 LessTif Development Team
 *
 * This file is part of the GNU LessTif Library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 **/

#ifndef _XM_ARROWBG_H
#define _XM_ARROWBG_H

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

extern WidgetClass xmArrowButtonGadgetClass;

typedef struct _XmArrowButtonGadgetRec *XmArrowButtonGadget;
typedef struct _XmArrowButtonGadgetClassRec *XmArrowButtonGadgetClass;

#ifndef XmIsArrowButtonGadget
#define XmIsArrowButtonGadget(w) XtIsSubclass((w), xmArrowButtonGadgetClass)
#endif

extern Widget XmCreateArrowButtonGadget(Widget parent,
				  char *name,
				  Arg *arglist,
				  Cardinal argCount);

extern Widget XmCreateArrowButtonGadget(Widget parent, char *name, Arg *arglist, Cardinal argcount);

#ifdef __cplusplus
}
#endif

#endif /* _XM_ARROWBG_H */
