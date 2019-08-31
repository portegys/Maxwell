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
 * Body particle.
 * Source: "Physics for Game Developers", Copyright 2000-2001 by David Bourg.
 */

#ifndef __PARTICLE__
#define __PARTICLE__

#include "Physics.h"
#include "Orientation.hpp"

class Cell;
class Body;
class Bond;

class Particle
{
public:

   int         type;            // type
   float       fRadius;         // radius
   float       fMass;           // mass
   float       fCharge;         // charge
   float       coefficientOfRestitution;
   Cell        *cell;           // cell
   Orientation orientation;     // orientation
   Vector3D    vPosition;       // position
   bool        fixed;           // fixed (immobile) particle?
   Body        *body;           // body containing particle
   Bond        *bonds;          // bonds to particles within body
   Particle    *collide;
   Particle    *next;
   int         mark;

   // Constructor.
   Particle(int type, float radius, float mass, float charge);
   Particle(int type);

   // Destructor.
   ~Particle();

   // Set fixed status.
   void setFixed(bool fixed);

   // Get bond to specific particle.
   Bond *getBond(Particle *particle);

   // Duplicate particle.
   Particle *duplicate();

   // Read and write particle.
   static Particle *read(FILE *fp);
   static void write(FILE *fp, Particle *particle);

   // Propel particle.
   void propel(Vector3D& force, double& weight);

   // Weighted propulsion vectors.
   struct Propulsion
   {
      Vector3D          force;
      double            weight;
      int               delay;
      int               duration;
      struct Propulsion *next;
   }
   *propulsions;
};
#endif
