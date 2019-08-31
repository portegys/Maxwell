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

#include "../base/Parameters.h"
#include "TestGenome.hpp"
#include MORPHOGEN_INCLUDE
#include <assert.h>

// Constructor.
TestGenome::TestGenome()
{
   int  n = 0;
   Gene *gene;

   assert(NUM_GENES == 27);

   // FOOD INGESTION GENES:

   // Corner particle grapples and moves food to gut (center).

   // Food north: move to nw.
   gene = genes[n];
   n++;
   gene->types[1][2]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[0][2]           = Gene::EMPTY_CELL;
   gene->action                = GRAPPLE_ACTION;
   gene->dx                    = 0;
   gene->dy                    = 1;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 7;
   gene->orientation.mirrored  = false;

   // Food nw: move to west.
   gene = genes[n];
   n++;
   gene->types[0][2]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[0][1]           = Gene::EMPTY_CELL;
   gene->action                = GRAPPLE_ACTION;
   gene->dx                    = -1;
   gene->dy                    = 1;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 7;
   gene->orientation.mirrored  = false;

   // Food west and side blocked: grapple it.
   gene = genes[n];
   n++;
   gene->types[0][1]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[0][0]           = Gene::OCCUPIED_CELL;
   gene->action                = GRAPPLE_ACTION;
   gene->dx                    = -1;
   gene->dy                    = 0;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 0;
   gene->orientation.mirrored  = false;

   // Food west and gut blocked: grapple it.
   gene = genes[n];
   n++;
   gene->types[0][1]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[1][0]           = Gene::OCCUPIED_CELL;
   gene->action                = GRAPPLE_ACTION;
   gene->dx                    = -1;
   gene->dy                    = 0;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 0;
   gene->orientation.mirrored  = false;

   // Food west and path to gut clear: move to sw.
   gene = genes[n];
   n++;
   gene->types[0][1]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[0][0]           = Gene::EMPTY_CELL;
   gene->types[1][0]           = Gene::EMPTY_CELL;
   gene->action                = GRAPPLE_ACTION;
   gene->dx                    = -1;
   gene->dy                    = 0;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 7;
   gene->orientation.mirrored  = false;

   // Food sw - move to gut.
   gene = genes[n];
   n++;
   gene->types[0][0]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[1][0]           = Gene::EMPTY_CELL;
   gene->action                = GRAPPLE_ACTION;
   gene->dx                    = -1;
   gene->dy                    = -1;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 7;
   gene->orientation.mirrored  = false;

   // Food ne: move to east.
   gene = genes[n];
   n++;
   gene->types[2][2]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[2][1]           = Gene::EMPTY_CELL;
   gene->action                = GRAPPLE_ACTION;
   gene->dx                    = 1;
   gene->dy                    = 1;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 1;
   gene->orientation.mirrored  = false;

   // Food east and side blocked: grapple it.
   gene = genes[n];
   n++;
   gene->types[2][1]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[2][0]           = Gene::OCCUPIED_CELL;
   gene->action                = BOND_ACTION;
   gene->dx                    = 1;
   gene->dy                    = 0;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 0;
   gene->orientation.mirrored  = false;

   // Food east and gut blocked: grapple it.
   gene = genes[n];
   n++;
   gene->types[2][1]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[1][0]           = Gene::OCCUPIED_CELL;
   gene->action                = BOND_ACTION;
   gene->dx                    = 1;
   gene->dy                    = 0;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 0;
   gene->orientation.mirrored  = false;

   // Food east and path to gut clear: bond to it.
   gene = genes[n];
   n++;
   gene->types[2][1] = FOOD_TYPE;
   gene->types[1][1] = BODY_CORNER_TYPE;
   gene->types[2][0] = Gene::EMPTY_CELL;
   gene->types[1][0] = Gene::EMPTY_CELL;
   gene->action      = BOND_ACTION;
   gene->dx          = 1;
   gene->dy          = 0;
   gene->type        = FOOD_TYPE;

   // Food in gut: grapple it.
   gene = genes[n];
   n++;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[1][0]           = FOOD_TYPE;
   gene->action                = GRAPPLE_ACTION;
   gene->dx                    = 0;
   gene->dy                    = -1;
   gene->type                  = FOOD_TYPE;
   gene->orientation.direction = 0;
   gene->orientation.mirrored  = false;

   // Side self-destructs to allow food to enter gut.
   gene = genes[n];
   n++;
   gene->types[1][2] = FOOD_TYPE;
   gene->types[1][1] = BODY_SIDE_TYPE;
   gene->types[1][0] = Gene::EMPTY_CELL;
   gene->action      = DESTROY_ACTION;
   gene->dx          = 0;
   gene->dy          = 0;
   gene->type        = BODY_SIDE_TYPE;

   // Food restores sides when it reaches gut.
   gene = genes[n];
   n++;
   gene->types[0][0]           = BODY_CORNER_TYPE;
   gene->types[2][0]           = BODY_CORNER_TYPE;
   gene->types[0][2]           = BODY_CORNER_TYPE;
   gene->types[2][2]           = BODY_CORNER_TYPE;
   gene->types[1][1]           = FOOD_TYPE;
   gene->types[1][2]           = Gene::EMPTY_CELL;
   gene->action                = CREATE_ACTION;
   gene->dx                    = 0;
   gene->dy                    = 1;
   gene->type                  = BODY_SIDE_TYPE;
   gene->orientation.direction = NORTH;
   gene->orientation.mirrored  = false;

   gene = genes[n];
   n++;
   gene->types[0][0]           = BODY_CORNER_TYPE;
   gene->types[2][0]           = BODY_CORNER_TYPE;
   gene->types[0][2]           = BODY_CORNER_TYPE;
   gene->types[2][2]           = BODY_CORNER_TYPE;
   gene->types[1][1]           = FOOD_TYPE;
   gene->types[0][1]           = Gene::EMPTY_CELL;
   gene->action                = CREATE_ACTION;
   gene->dx                    = -1;
   gene->dy                    = 0;
   gene->type                  = BODY_SIDE_TYPE;
   gene->orientation.direction = WEST;
   gene->orientation.mirrored  = false;

   gene = genes[n];
   n++;
   gene->types[0][0]           = BODY_CORNER_TYPE;
   gene->types[2][0]           = BODY_CORNER_TYPE;
   gene->types[0][2]           = BODY_CORNER_TYPE;
   gene->types[2][2]           = BODY_CORNER_TYPE;
   gene->types[1][1]           = FOOD_TYPE;
   gene->types[2][1]           = Gene::EMPTY_CELL;
   gene->action                = CREATE_ACTION;
   gene->dx                    = 1;
   gene->dy                    = 0;
   gene->type                  = BODY_SIDE_TYPE;
   gene->orientation.direction = EAST;
   gene->orientation.mirrored  = false;

   gene = genes[n];
   n++;
   gene->types[0][0]           = BODY_CORNER_TYPE;
   gene->types[2][0]           = BODY_CORNER_TYPE;
   gene->types[0][2]           = BODY_CORNER_TYPE;
   gene->types[2][2]           = BODY_CORNER_TYPE;
   gene->types[1][1]           = FOOD_TYPE;
   gene->types[1][0]           = Gene::EMPTY_CELL;
   gene->action                = CREATE_ACTION;
   gene->dx                    = 0;
   gene->dy                    = -1;
   gene->type                  = BODY_SIDE_TYPE;
   gene->orientation.direction = SOUTH;
   gene->orientation.mirrored  = false;

   // Food digesting in gut.
   gene = genes[n];
   n++;
   gene->types[0][0] = BODY_CORNER_TYPE;
   gene->types[2][0] = BODY_CORNER_TYPE;
   gene->types[0][2] = BODY_CORNER_TYPE;
   gene->types[2][2] = BODY_CORNER_TYPE;
   gene->types[1][1] = FOOD_TYPE;
   gene->action      = SET_TYPE_ACTION;
   gene->dx          = 0;
   gene->dy          = 0;
   gene->type        = DIGESTING_FOOD_TYPE;

   // Food digesting in gut.
   gene = genes[n];
   n++;
   gene->types[1][1] = DIGESTING_FOOD_TYPE;
   gene->action      = SET_TYPE_ACTION;
   gene->dx          = 0;
   gene->dy          = 0;
   gene->type        = DIGESTED_FOOD_TYPE;

   // Destroy digested food.
   gene = genes[n];
   n++;
   gene->types[1][1] = DIGESTED_FOOD_TYPE;
   gene->action      = DESTROY_ACTION;
   gene->dx          = 0;
   gene->dy          = 0;
   gene->type        = DIGESTED_FOOD_TYPE;

   // Side bonds to left corner.
   gene = genes[n];
   n++;
   gene->types[0][1] = BODY_CORNER_TYPE;
   gene->types[1][1] = BODY_SIDE_TYPE;
   gene->action      = BOND_ACTION;
   gene->dx          = -1;
   gene->dy          = 0;
   gene->type        = BODY_CORNER_TYPE;

   // Side bonds to right corner.
   gene = genes[n];
   n++;
   gene->types[2][1] = BODY_CORNER_TYPE;
   gene->types[1][1] = BODY_SIDE_TYPE;
   gene->action      = BOND_ACTION;
   gene->dx          = 1;
   gene->dy          = 0;
   gene->type        = BODY_CORNER_TYPE;

   // FORAGING GENES:

#if (FORAGING_GENES_INITIALIZATION == EFFECTIVE_FORAGING_VALUES)
   // Random movement if no food visible.
   gene = genes[n];
   n++;
   gene->types[1][2]           = Gene::EMPTY_CELL;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->action                = PROPEL_ACTION;
   gene->dx                    = 0;
   gene->dy                    = 0;
   gene->type                  = BODY_CORNER_TYPE;
   gene->orientation.direction = NORTH;
   gene->orientation.mirrored  = false;
   gene->strength              = 2.0;
   gene->tendency              = 0.1;

   // Move counter-clockwise around food patch.
   gene = genes[n];
   n++;
   gene->types[1][2]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->action                = PROPEL_ACTION;
   gene->dx                    = 0;
   gene->dy                    = 0;
   gene->type                  = BODY_CORNER_TYPE;
   gene->orientation.direction = EAST;
   gene->orientation.mirrored  = false;
   gene->strength              = 0.1;
   gene->tendency              = 5.0;
   gene->duration              = 20;

   gene = genes[n];
   n++;
   gene->types[1][2]           = FOOD_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->action                = PROPEL_ACTION;
   gene->dx                    = 0;
   gene->dy                    = 0;
   gene->type                  = BODY_CORNER_TYPE;
   gene->orientation.direction = NORTH;
   gene->orientation.mirrored  = false;
   gene->strength              = 0.1;
   gene->tendency              = 3.0;
   gene->delay                 = 20;
   gene->duration              = 50;
#endif

#if (FORAGING_GENES_INITIALIZATION == BASIC_FORAGING_VALUES)
   // No food visible.
   gene = genes[n];
   n++;
   gene->types[1][2] = Gene::EMPTY_CELL;
   gene->types[1][1] = BODY_CORNER_TYPE;
   gene->action      = PROPEL_ACTION;
   gene->dx          = 0;
   gene->dy          = 0;
   gene->type        = BODY_CORNER_TYPE;

   // Food visible.
   gene = genes[n];
   n++;
   gene->types[1][2] = FOOD_TYPE;
   gene->types[1][1] = BODY_CORNER_TYPE;
   gene->action      = PROPEL_ACTION;
   gene->dx          = 0;
   gene->dy          = 0;
   gene->type        = BODY_CORNER_TYPE;

   gene = genes[n];
   n++;
   gene->types[1][2] = FOOD_TYPE;
   gene->types[1][1] = BODY_CORNER_TYPE;
   gene->action      = PROPEL_ACTION;
   gene->dx          = 0;
   gene->dy          = 0;
   gene->type        = BODY_CORNER_TYPE;
#endif

#if (FORAGING_GENES_INITIALIZATION == RANDOM_FORAGING_VALUES)
   gene = genes[n];
   n++;
   gene->randomize();
   gene = genes[n];
   n++;
   gene->randomize();
   gene = genes[n];
   n++;
   gene->randomize();
#endif

#if (FORAGING_GENES_INITIALIZATION == CLEAR_FORAGING_VALUES)
   n += 3;
#endif

   // REPAIR GENES:

   // Corner creates missing side to sw.
   gene = genes[n];
   n++;
   gene->types[0][1]           = Gene::EMPTY_CELL;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->types[0][0]           = Gene::EMPTY_CELL;
   gene->types[1][0]           = Gene::EMPTY_CELL;
   gene->action                = CREATE_ACTION;
   gene->dx                    = -1;
   gene->dy                    = -1;
   gene->type                  = BODY_SIDE_TYPE;
   gene->orientation.direction = NORTHWEST;
   gene->orientation.mirrored  = false;

   // Side creates missing corner to west.
   gene = genes[n];
   n++;
   gene->types[0][1]           = Gene::EMPTY_CELL;
   gene->types[1][1]           = BODY_SIDE_TYPE;
   gene->action                = CREATE_ACTION;
   gene->dx                    = -1;
   gene->dy                    = 0;
   gene->type                  = BODY_CORNER_TYPE;
   gene->orientation.direction = NORTHWEST;
   gene->orientation.mirrored  = false;

   // OBSTACLE NAVIGATION GENES:

   // Move counter-clockwise around obstacles.
   gene = genes[n];
   n++;
   gene->types[1][2]           = OBSTACLE_TYPE;
   gene->types[1][1]           = BODY_CORNER_TYPE;
   gene->action                = PROPEL_ACTION;
   gene->dx                    = 0;
   gene->dy                    = 0;
   gene->duration              = 20;
   gene->type                  = BODY_CORNER_TYPE;
   gene->orientation.direction = EAST;
   gene->orientation.mirrored  = false;
   gene->strength              = 0.1;
}


// Destructor.
TestGenome::~TestGenome()
{
   clear();
}
