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
 * Body.
 * A body consists of a connected set of particles.
 * Source: "Physics for Game Developers", Copyright 2000-2001 by David Bourg.
 */

#ifndef __BODY__
#define __BODY__

#include <stdio.h>
#include "Parameters.h"
#include "Physics.h"

class Particle;
class Mechanics;

class Body
{
public:

#if (USE_ENERGY == 1 || STORE_ENERGY == 1)
   int energy;                          // energy
#endif
   float fMass;                         // mass
   Matrix3x3 mInertia;                  // mass moment of inertia
   Matrix3x3 mInertiaInverse;           // inverse of mass moment of inertia
   Vector3D  vVelocity;                 // velocity
   Vector3D  vForces;                   // force on body
   Particle  *particles;                // particles comprising body
   int       fixedCount;                // number of fixed particles in body
   bool      collide;                   // collision flag
   Body      *next;                     // next body

   // Constructor.
   Body();

   // Destructor.
   ~Body();

   // Calculate inertia.
   void calcInertia();

   // Propel body.
   void propel();

   // Duplicate body.
   Body *duplicate(Mechanics *);

   // Read and write body.
   static Body *read(FILE *fp, Mechanics *);
   static void write(FILE *fp, Body *body);
};
#endif
