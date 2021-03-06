/***********************************************************************
 * FXRuby -- the Ruby language bindings for the FOX GUI toolkit.
 * Copyright (c) 2001-2009 by Lyle Johnson. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact the author by e-mail
 * at "lyle@lylejohnson.name".
 ***********************************************************************/

/***********************************************************************
 * $Id: FXRbGLContext.h 2873 2008-05-30 21:38:58Z lyle $
 ***********************************************************************/

#ifndef FXRBGLCONTEXT_H
#define FXRBGLCONTEXT_H

class FXRbGLContext : public FXGLContext {
  FXDECLARE(FXRbGLContext)
protected:
  FXRbGLContext(){}
#include "FXRbObjectVirtuals.h"
#include "FXRbIdVirtuals.h"
public:
  /**
   * Construct an OpenGL context with default configuration properties;
   * it shares a display list with another context shr.
   */
  FXRbGLContext(FXApp* a,FXGLVisual* vis,FXGLContext *shr=NULL) : FXGLContext(a,vis,shr){}

  // Mark dependencies for the GC
  static void markfunc(FXGLContext* self);

  // Destructor
  virtual ~FXRbGLContext(){
    FXRbUnregisterRubyObj(this);
    }
  };

#endif
