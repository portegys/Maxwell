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
 * Maxwell morphogen.
 *
 * The purpose of this experiment is to investigate the properties of a
 * chemical-like organism. The goal is to develop an organism that can:
 * 1. Find and consume food.
 * 2. Navigate obstacles.
 * 3. Avoid poison entities and self-repair.
 *
 * The name Maxwell originates from "Maxwell's daemon", to which it is
 * only nominally related.
 *
 * Maxwell is based on the Morphone system, in which cells communicate
 * via signals and changes are caused by morphogenic functions.
 * Reference: "An Abstraction of Intercellular Communication" by
 * T. E. Portegys in Alife8 Proceedings, December 2002.
 *
 * Implementation:
 *
 * Mobile bodies residing within the cells of a cellular automaton sense
 * the configuration of bodies in their neighborhood. Genes matching these
 * configurations produce weighted actions within the neighborhood.
 * These actions include: create/destroy body, set body type,
 * grapple/bond/unbond body, orient body, and propel body.
 */

#ifndef __MAXWELL__
#define __MAXWELL__

#include <stdio.h>
#include <math.h>
#include "Morphogen.hpp"
#include "BondMorph.hpp"
#include "CreateMorph.hpp"
#include "GrappleMorph.hpp"
#include "PropelMorph.hpp"
#include "OrientMorph.hpp"
#include "TypeMorph.hpp"
#include "Genome.hpp"

// Actions:
// Create/destroy, set type, bond/unbond,
// orient body, grapple and propel.
#define CREATE_ACTION          0
#define BOND_ACTION            1
#define SET_TYPE_ACTION        2
#define ORIENT_ACTION          3
#define UNBOND_ACTION          4
#define DESTROY_ACTION         5
#define GRAPPLE_ACTION         6
#define PROPEL_ACTION          7
#define NUM_ACTIONS            8

// Particle types.
#define NUM_PARTICLE_TYPES     8
#define FOOD_TYPE              0
#define POISON_TYPE            1
#define BODY_CORNER_TYPE       2
#define BODY_SIDE_TYPE         3
#define OBSTACLE_TYPE          4
#define DIGESTING_FOOD_TYPE    5
#define DIGESTED_FOOD_TYPE     6

// Environment types.
#define FORAGE_ENV             1 // Food foraging environment.
#if (FORAGE_ENV == 1)
#if (SNAPSHOT == 1)
#define MIN_FOOD_PATCHES       1
#define MAX_FOOD_PATCHES       1
#define MIN_PATCH_RADIUS       1
#define MAX_PATCH_RADIUS       1
#else
#define MIN_FOOD_PATCHES       10
#define MAX_FOOD_PATCHES       15
#define MIN_PATCH_RADIUS       2
#define MAX_PATCH_RADIUS       5
#endif
#endif

#define POISON_ENV              0 // Poison particles in environment.
#if (POISON_ENV == 1)
#define MIN_POISON_PARTICLES    1
#define MAX_POISON_PARTICLES    10
#endif

#define OBSTACLE_ENV            0 // Obstacles in environment.
#if (OBSTACLE_ENV == 1)
#define MIN_OBSTACLE_WALLS      5
#define MAX_OBSTACLE_WALLS      10
#define MIN_WALL_LENGTH         5
#define MAX_WALL_LENGTH         25
#endif

// Foraging genes pre-initialization types.
#define EFFECTIVE_FORAGING_VALUES        0
#define BASIC_FORAGING_VALUES            1
#define RANDOM_FORAGING_VALUES           2
#define CLEAR_FORAGING_VALUES            3
#define FORAGING_GENES_INITIALIZATION    EFFECTIVE_FORAGING_VALUES

// Test genome for foraging movement.
#define FORAGING_MOVEMENT_SCREEN         0

// Maximum body placement tries.
#define MAX_PLACEMENT_TRIES              1000

// Minimum acceptable Maxwell identification variance
// (see findMaxwell()).
#define MIN_VARIANCE    4

// Maxwell morphogen.
class Maxwell : public Morphogen
{
public:

   // Associated morphogens.
   BondMorph    *bondMorph;
   CreateMorph  *createMorph;
   GrappleMorph *grappleMorph;
   PropelMorph  *propelMorph;
   OrientMorph  *orientMorph;
   TypeMorph    *typeMorph;

   // Genome.
   Genome *genome;

   // Set genome
   void setGenome(Genome *genome);

   // Create genome.
   Genome *createGenome();

   // Constructor.
   Maxwell();

   // Destructor.
   ~Maxwell();

   // Initialize.
   void init(Mechanics *mechanics);

   // Load body configuration.
   bool load(Body **bodies, int numBodies);

   // Pre-morph processing.
   void preMorph();

   // Emit signals.
   Emission *signal(Neighborhood *neighbors);

   // Morph.
   void morph(Cell *cell);

   // Post-morph processing.
   void postMorph();

   // Get fitness.
   double getFitness();

   // Create a viable gene.
   void createViableGene(Gene *, int action);

#if (FORAGING_MOVEMENT_SCREEN == 1)
   // Gene produces mobility (self-propulsion)?
   bool isMobile(int geneNum);
#endif

private:

   // Find "best" Maxwell: if found, return body
   // and measure of its "variance" from ideal.
   Body *findMaxwell(int& variance);

   // Place body.
   bool placeBody(Body *, float maxVelocity);

   // Body placement tools.
   int map[WIDTH][HEIGHT];
   void mapPatch(int type, int x, int y, int radius);

#if (FORAGING_MOVEMENT_SCREEN == 1)
   // Movement test neighborhood.
   Neighborhood testNeighbors;
#endif
};
#endif
