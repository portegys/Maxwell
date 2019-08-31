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

#ifndef __EMISSION__
#define __EMISSION__

#include "Signal.hpp"

class Emission
{
public:

   Signal   *signal;
   int      dx, dy;
   int      delay;
   int      duration;
   Emission *next;

   // Constructors.
   Emission(Signal *signal, int dx, int dy);
   Emission(Signal *signal, int dx, int dy, int delay, int duration);

   // Destructor.
   ~Emission();
};
#endif
