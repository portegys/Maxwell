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
 * Test body construction.
 * Usage: TestBody <test body output file name>
 */

#include <stdio.h>
#include <assert.h>
#include "../base/Automaton.hpp"
#include "../base/Body.hpp"
#include "../base/Particle.hpp"
#include "../base/Bond.hpp"

// Test body list.
Body *bodies;

// Functions.
void create(), save(char *);
void addParticle(Body *, Particle *);
void bondParticles(Particle *, Particle *);

int main(int argc, char *argv[])
{
   char *fileName;

   if (argc == 2)
   {
      fileName = argv[1];
   }
   else
   {
      fprintf(stderr, "Usage: TestBody <test body output file name>\n");
      return(1);
   }

   // Create test bodies.
   printf("Creating bodies...\n");
   create();

   // Save bodies.
   printf("Saving bodies to %s...\n", fileName);
   save(fileName);

   printf("Done.\n");
   return(0);
}


// Construct test bodies.
// Body particles should be centered about XY origin.
void create()
{
   Body     *body;
   Particle *particle, *ne, *se, *sw, *nw, *n, *s, *e, *w;

   bodies = NULL;
   double width  = (double)WIDTH;
   double height = (double)HEIGHT;

   // Create food particle.
   body = new Body();
   assert(body != NULL);
   particle = new Particle(FOOD_TYPE, DEFAULT_RADIUS,
                           DEFAULT_MASS, DEFAULT_CHARGE);
   assert(particle != NULL);
   addParticle(body, particle);
   body->next = bodies;
   bodies     = body;

   // Create poison particle.
   body = new Body();
   assert(body != NULL);
   particle = new Particle(POISON_TYPE, DEFAULT_RADIUS,
                           DEFAULT_MASS, DEFAULT_CHARGE);
   assert(particle != NULL);
   addParticle(body, particle);
   body->next = bodies;
   bodies     = body;

   // Create obstacle particle.
   body = new Body();
   assert(body != NULL);
   particle = new Particle(OBSTACLE_TYPE, DEFAULT_RADIUS,
                           DEFAULT_MASS, DEFAULT_CHARGE);
   assert(particle != NULL);
   addParticle(body, particle);
   body->next = bodies;
   bodies     = body;

   // Create Maxwell.
   body = new Body();
   assert(body != NULL);
   ne = new Particle(BODY_CORNER_TYPE, DEFAULT_RADIUS,
                     DEFAULT_MASS, DEFAULT_CHARGE);
   assert(ne != NULL);
   ne->vPosition.x           = POSITION(1.0);
   ne->vPosition.y           = POSITION(1.0);
   ne->orientation.direction = NORTHEAST;
   addParticle(body, ne);
   e = new Particle(BODY_SIDE_TYPE, DEFAULT_RADIUS,
                    DEFAULT_MASS, DEFAULT_CHARGE);
   assert(e != NULL);
   e->vPosition.x           = POSITION(1.0);
   e->vPosition.y           = POSITION(0.0);
   e->orientation.direction = EAST;
   addParticle(body, e);
   bondParticles(ne, e);
   se = new Particle(BODY_CORNER_TYPE, DEFAULT_RADIUS,
                     DEFAULT_MASS, DEFAULT_CHARGE);
   assert(se != NULL);
   se->vPosition.x           = POSITION(1.0);
   se->vPosition.y           = POSITION(-1.0);
   se->orientation.direction = SOUTHEAST;
   addParticle(body, se);
   bondParticles(e, se);
   s = new Particle(BODY_SIDE_TYPE, DEFAULT_RADIUS,
                    DEFAULT_MASS, DEFAULT_CHARGE);
   assert(s != NULL);
   s->vPosition.x           = POSITION(0.0);
   s->vPosition.y           = POSITION(-1.0);
   s->orientation.direction = SOUTH;
   addParticle(body, s);
   bondParticles(se, s);
   sw = new Particle(BODY_CORNER_TYPE, DEFAULT_RADIUS,
                     DEFAULT_MASS, DEFAULT_CHARGE);
   assert(sw != NULL);
   sw->vPosition.x           = POSITION(-1.0);
   sw->vPosition.y           = POSITION(-1.0);
   sw->orientation.direction = SOUTHWEST;
   addParticle(body, sw);
   bondParticles(s, sw);
   w = new Particle(BODY_SIDE_TYPE, DEFAULT_RADIUS,
                    DEFAULT_MASS, DEFAULT_CHARGE);
   assert(w != NULL);
   w->vPosition.x           = POSITION(-1.0);
   w->vPosition.y           = POSITION(0.0);
   w->orientation.direction = WEST;
   addParticle(body, w);
   bondParticles(sw, w);
   nw = new Particle(BODY_CORNER_TYPE, DEFAULT_RADIUS,
                     DEFAULT_MASS, DEFAULT_CHARGE);
   assert(nw != NULL);
   nw->vPosition.x           = POSITION(-1.0);
   nw->vPosition.y           = POSITION(1.0);
   nw->orientation.direction = NORTHWEST;
   addParticle(body, nw);
   bondParticles(w, nw);
   n = new Particle(BODY_SIDE_TYPE, DEFAULT_RADIUS,
                    DEFAULT_MASS, DEFAULT_CHARGE);
   assert(n != NULL);
   n->vPosition.x           = POSITION(0.0);
   n->vPosition.y           = POSITION(1.0);
   n->orientation.direction = NORTH;
   addParticle(body, n);
   bondParticles(nw, n);
   bondParticles(n, ne);
   body->next = bodies;
   bodies     = body;
}


// Add particle to body.
void addParticle(Body *body, Particle *particle)
{
   particle->body  = body;
   particle->next  = body->particles;
   body->particles = particle;
   body->fMass    += particle->fMass;
   body->calcInertia();
   if (particle->fixed)
   {
      body->vVelocity.Zero();
      body->fixedCount++;
   }
}


// Bond particles.
void bondParticles(Particle *particle1, Particle *particle2)
{
   Bond *bond = new Bond(particle1, particle2);

   assert(bond != NULL);
   bond->next1      = particle1->bonds;
   particle1->bonds = bond;
   bond->next2      = particle2->bonds;
   particle2->bonds = bond;
}


// Save test bodies to file.
void save(char *fileName)
{
   FILE *fp;
   Body *body;
   int  count;

   if ((fp = fopen(fileName, "w")) == NULL)
   {
      fprintf(stderr, "Cannot open test body file %s", fileName);
      return;
   }
   for (body = bodies, count = 0; body != NULL; body = body->next, count++)
   {
   }
   fprintf(fp, "%d\n", count);
   for (body = bodies; body != NULL; body = body->next)
   {
      Body::write(fp, body);
   }
   fclose(fp);
}
