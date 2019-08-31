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

/**
 * Create morphogen - create/destroy bodies/particles.
 * CREATE signal parameter: particle type.
 * DESTROY signal parameter: none.
 */

#ifndef __CREATE_MORPH__
#define __CREATE_MORPH__

#include "Morphogen.hpp"

// Crate/destroy morphogen.
class CreateMorph : public Morphogen
{
public:

   // Signal types.
   Compound *CREATE;
   Compound *DESTROY;

   // Signal type scoping.
   Scope *scope;

   // Constructor.
   CreateMorph();

   // Destructor.
   ~CreateMorph();

   // Morph.
   void morph(Cell *cell);

   // Create a create signal.
   Signal *createCreateSignal(Particle *particle,
                              Orientation orientation, int type);

   // Create a destroy signal.
   Signal *createDestroySignal(int targetType);
};
#endif
