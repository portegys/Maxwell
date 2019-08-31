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
 * Bond connecting two particles.
 */

#ifndef __BOND__
#define __BOND__

#include "Physics.h"

class Particle;

class Bond
{
public:

   // Maximum bond length.
   static const double MAX_BOND_LENGTH;

   Particle *particle1; // particle #1
   Particle *particle2; // particle #2
   double   length;     // length of bond
   Bond     *next1;
   Bond     *next2;

   // Constructor.
   Bond(Particle *particle1, Particle *particle2);

   // Destructor.
   ~Bond();

   // Disconnect bond from given particle.
   void disconnect(Particle *particle);
};
#endif
