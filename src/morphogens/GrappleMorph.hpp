/*
 * This software is provided under the terms of the GNU General
 * Public License as published by the Free Software Foundation.
 *
 * Copyright (c) 2002 Tom Portegys, All Rights Reserved.
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
 * Grapple morphogen - particle grappling and movement.
 * GRAPPLE signal parameter: grappling particle and movement direction.
 */

#ifndef __GRAPPLE_MORPH__
#define __GRAPPLE_MORPH__

#include "Morphogen.hpp"

// Grapple morphogen.
class GrappleMorph : public Morphogen
{
public:

   // Signal type.
   Compound *GRAPPLE;

   // Signal type scoping.
   Scope *scope;

   // Constructor.
   GrappleMorph();

   // Destructor.
   ~GrappleMorph();

   // Morph.
   void morph(Cell *cell);

   // Create a grapple signal.
   Signal *createGrappleSignal(Particle *particle,
                               int dx, int dy, int targetType);
};
#endif
