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
 * Automaton.
 * A cellular automaton in which each cell is capable of emitting
 * and absorbing signals to and from its neighbors. In each cell,
 * morphogenic functions translate absorbed signals into changes
 * in orientation, body configuration, and subsequent signal emissions.
 */

#ifndef __AUTOMATON__
#define __AUTOMATON__

#include "Parameters.h"
#include "Cell.hpp"
#include "Mechanics.hpp"
#include MORPHOGEN_INCLUDE

class Automaton
{
public:

   // Cells.
   Cell cells[WIDTH][HEIGHT];

   // Morphogen.
   MORPHOGEN morphogen;

   // Bodies.
   Mechanics mechanics;

   // Constructor.
   Automaton();

   // Morph.
   void morph();

   // Get cell at location.
   Cell *getCell(int x, int y);
};
#endif
