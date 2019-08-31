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
 * Orient morphogen - orient particles.
 * ORIENT signal parameters: direction and mirrored value.
 */

#ifndef __ORIENT_MORPH__
#define __ORIENT_MORPH__

#include "Morphogen.hpp"

// Orient morphogen.
class OrientMorph : public Morphogen
{
public:

   // Signal type.
   Compound *ORIENT;

   // Signal type scoping.
   Scope *scope;

   // Constructor.
   OrientMorph();

   // Destructor.
   ~OrientMorph();

   // Morph.
   void morph(Cell *cell);

   // Create an orient signal.
   Signal *createOrientSignal(Orientation orientation, int targetType);
};
#endif
