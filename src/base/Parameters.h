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
 * Parameters.
 */

#ifndef __PARAMETERS__
#define __PARAMETERS__

// Morphogen.
#define MORPHOGEN            Maxwell
#define MORPHOGEN_INCLUDE    "../morphogens/Maxwell.hpp"
#define MORPHOGEN_NAME       "Maxwell"

// Step delta time.
#define DTIME                1.0f

// Body energy:
// To use energy to create particles, set USE_ENERGY = 1
#define USE_ENERGY                1
#if (USE_ENERGY == 1)
#define PARTICLE_CREATE_ENERGY    1
#endif
// To store energy, set STORE_ENERGY = 1
#define STORE_ENERGY              1
#if (STORE_ENERGY == 1)
#define FOOD_PARTICLE_ENERGY      10
#endif
#define INFINITE_ENERGY           (-1)
#define INITIAL_ENERGY            10

// To take a screen snapshot, set SNAPSHOT = 1
#define SNAPSHOT                  0

// Cellular automaton dimensions.
#if (SNAPSHOT == 1)
#define WIDTH     10
#define HEIGHT    10
#else
#define WIDTH     50
#define HEIGHT    50
#endif

// Directions.
#define NORTH        0
#define NORTHEAST    1
#define EAST         2
#define SOUTHEAST    3
#define SOUTH        4
#define SOUTHWEST    5
#define WEST         6
#define NORTHWEST    7
#define CENTER       8

#endif
