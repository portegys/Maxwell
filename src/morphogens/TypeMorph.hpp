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
 * Type morphogen - set particle type.
 * TYPE signal parameters: particle type.
 */

#ifndef __TYPE_MORPH__
#define __TYPE_MORPH__

#include "Morphogen.hpp"

// Type morphogen.
class TypeMorph : public Morphogen
{
public:

   // Signal type.
   Compound *TYPE;

   // Signal type scoping.
   Scope *scope;

   // Constructor.
   TypeMorph();

   // Destructor.
   ~TypeMorph();

   // Morph.
   void morph(Cell *cell);

   // Create a type signal.
   Signal *createTypeSignal(int deltaType, int targetType);
};
#endif
