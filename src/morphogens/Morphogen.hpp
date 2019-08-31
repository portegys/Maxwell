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
 * Morphogen - base class.
 */

#ifndef __MORPHOGEN__
#define __MORPHOGEN__

#include "../util/Compound.hpp"
#include "../util/ScopeFactory.hpp"
#include "../util/Random.hpp"
#include "../base/Body.hpp"
#include "../base/Particle.hpp"
#include "../base/Bond.hpp"
#include "../base/Cell.hpp"
#include "../base/Emission.hpp"

// "Wall" particle.
#define WALL_TYPE    (-1)

// Morphogen.
class Morphogen
{
public:

   // Mechanics.
   Mechanics *mechanics;

   // Constructor.
   Morphogen();

   // Initialize.
   void init(Mechanics *mechanics);

   // Load body configuration.
   void load(Body **bodies, int numBodies);

   // Pre-morph processing.
   void preMorph();

   // Emit signals.
   Emission *signal(Neighborhood *neighbors);

   // Morph.
   void morph(Cell *cell);

   // Post-morph processing.
   void postMorph();
};
#endif
