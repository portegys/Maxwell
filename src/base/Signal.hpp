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
 * Signal.
 */

#ifndef __SIGNAL__
#define __SIGNAL__

#include "../util/Compound.hpp"

class Signal
{
public:

   Compound *type;
   void     **parameters;
   double   strength;

   // Constructors.
   Signal(Compound *type);
   Signal(Compound *type, void **parameters);
   Signal(Compound *type, void **parameters, double strength);

   // Destructor
   ~Signal();
};
#endif
