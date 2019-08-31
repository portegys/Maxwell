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

#include "Morphogen.hpp"

// Constructor.
Morphogen::Morphogen()
{
}

// Initialize.
void Morphogen::init(Mechanics *mechanics)
{
   this->mechanics = mechanics;
}


// Load body configuration.
void Morphogen::load(Body **bodies, int numBodies)
{
}

// Pre-morph processing.
void Morphogen::preMorph()
{
}

// Emit signals.
Emission *Morphogen::signal(Neighborhood *neighbors)
{
   return(NULL);
}


// Morph.
void Morphogen::morph(Cell *cell)
{
}

// Post-morph processing.
void Morphogen::postMorph()
{
}
