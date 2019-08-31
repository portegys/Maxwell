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
 * Bond morphogen - Bonds/unbonds particles.
 * BOND/UNBOND signal parameter: particle.
 */

#ifndef __BOND_MORPH__
#define __BOND_MORPH__

#include "Morphogen.hpp"

// Bond/unbond morphogen.
class BondMorph : public Morphogen
{
public:

   // Signal types.
   Compound *BOND;
   Compound *UNBOND;

   // Signal type scoping.
   Scope *scope;

   // Constructor.
   BondMorph();

   // Destructor.
   ~BondMorph();

   // Morph.
   void morph(Cell *cell);

   // Create a bond signal.
   Signal *createBondSignal(Particle *particle, int targetType);

   // Create an unbond signal.
   Signal *createUnbondSignal(Particle *particle, int targetType);
};
#endif
