/*
 * This software is provided under the terms of the GNU General
 * Public License as published by the Free Software Foundation.
 *
 * Copyright (c) 2003 Tom Portegys, All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for NON-COMMERCIAL purposes and without
 * fee is hereby granted provided that this copyright notice
 * appears in all copies.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.
 */

/*
 * Signal emission.
 */

#include "Emission.hpp"

// Constructors.
Emission::Emission(Signal *signal, int dx, int dy)
{
   this->signal = signal;
   this->dx     = dx;
   this->dy     = dy;
   delay        = 0;
   duration     = 1;
   next         = NULL;
}


Emission::Emission(Signal *signal, int dx, int dy, int delay, int duration)
{
   this->signal   = signal;
   this->dx       = dx;
   this->dy       = dy;
   this->delay    = delay;
   this->duration = duration;
   next           = NULL;
}


// Destructor.
Emission::~Emission()
{
   if (signal != NULL)
   {
      delete signal;
   }
}
