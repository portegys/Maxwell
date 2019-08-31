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
 * Propulsion morphogen - body self-propulsion.
 * PROPEL signal parameters: propulsion values.
 */

#ifndef __PROPEL_MORPH__
#define __PROPEL_MORPH__

#include "Morphogen.hpp"

// Propel morphogen.
class PropelMorph : public Morphogen
{
public:

   // Signal type.
   Compound *PROPEL;

   // Signal type scoping.
   Scope *scope;

   // Constructor.
   PropelMorph();

   // Destructor.
   ~PropelMorph();

   // Morph.
   void morph(Cell *cell);

   // Post-morph processing.
   void postMorph();

   // Create a propel signal.
   Signal *createPropelSignal(Particle *particle, int direction,
                              double force, double weight, int targetType);
};
#endif
