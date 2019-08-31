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
 * Cell.
 */

#ifndef __CELL__
#define __CELL__

#include "Emission.hpp"
#include "Particle.hpp"
#include <list>

class Cell
{
public:

   // Position.
   int x, y;

   // Cell particles.
   std::list<Particle *> particles;

   // Absorbed signal emissions.
   Emission *absorption;

   // Cell constructor.
   Cell();

   // Cell destructor.
   ~Cell();

   // Absorb signal emission.
   void absorb(Emission *emission);

   // Reset cell.
   void reset();
};

// Cell neighborhood.
// This is a rotatable/mirrorable Moore neighborhood in which the
// diagonal cells of an obliquely oriented neighborhood is compacted
// into the same size neighborhood as a perpendicularly oriented one.
class Neighborhood
{
public:

   // The neighborhood origin (0,0) is mapped to the center cell
   // in the matrix.
   Cell *cells[3][3];

   // Constructor.
   Neighborhood();

   // Transform neighborhood by given orientation.
   void transform(Orientation& orientation);

   // Get real cell location given input location and current transform state.
   void getCellLocation(int& x, int& y);

   // Get offsets to cell clockwise steps from given cell in neighborhood.
   // Given cell location must be in neighborhood: -1 <= x,y <= 1
   static void getDxy(int x, int y, Orientation steps, int& dx, int& dy);

private:

   Orientation orientation;
};
#endif
