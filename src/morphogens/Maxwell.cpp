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

#include "Maxwell.hpp"
#include "../base/Mechanics.hpp"
#include "../util/Random.hpp"
#include "../util/Log.hpp"

// Set genome
void Maxwell::setGenome(Genome *genome)
{
   if (this->genome != NULL)
   {
      delete this->genome;
   }
   this->genome = genome;
}


// Create genome.
Genome *Maxwell::createGenome()
{
   Genome *genome = new Genome();

   assert(genome != NULL);
   return(genome);
}


// Constructor.
Maxwell::Maxwell()
{
   // Create associated morphogens.
   bondMorph = new BondMorph();
   assert(bondMorph != NULL);
   createMorph = new CreateMorph();
   assert(createMorph != NULL);
   grappleMorph = new GrappleMorph();
   assert(grappleMorph != NULL);
   propelMorph = new PropelMorph();
   assert(propelMorph != NULL);
   orientMorph = new OrientMorph();
   assert(orientMorph != NULL);
   typeMorph = new TypeMorph();
   assert(typeMorph != NULL);

   // Create genome.
   genome = new Genome();
   assert(genome != NULL);

#if (FORAGING_MOVEMENT_SCREEN == 1)
   // Create movement test neighborhood.
   int      x, y;
   Particle *particle;

   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         testNeighbors.cells[x][y] = new Cell();
         assert(testNeighbors.cells[x][y] != NULL);
      }
   }
   particle = new Particle(BODY_SIDE_TYPE);
   assert(particle != NULL);
   testNeighbors.cells[0][1]->particles.push_front(particle);
   particle = new Particle(BODY_CORNER_TYPE);
   assert(particle != NULL);
   particle->orientation.direction = NORTHEAST;
   testNeighbors.cells[1][1]->particles.push_front(particle);
   particle = new Particle(BODY_SIDE_TYPE);
   assert(particle != NULL);
   particle->orientation.direction = EAST;
   testNeighbors.cells[1][0]->particles.push_front(particle);
#endif
}


// Destructor.
Maxwell::~Maxwell()
{
   delete bondMorph;
   delete createMorph;
   delete grappleMorph;
   delete propelMorph;
   delete orientMorph;
   delete typeMorph;
   delete genome;

#if (FORAGING_MOVEMENT_SCREEN == 1)
   // Delete movement test neighborhood.
   int      x, y;
   Particle *particle;
   std::list<Particle *>::const_iterator listItr;

   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         for (listItr = testNeighbors.cells[x][y]->particles.begin();
              listItr != testNeighbors.cells[x][y]->particles.end(); listItr++)
         {
            particle = *listItr;
            delete particle;
         }
         delete testNeighbors.cells[x][y];
      }
   }
#endif
}


// Initialize.
void Maxwell::init(Mechanics *mechanics)
{
   // Base init.
   ((Morphogen *)this)->init(mechanics);

   // Initialize components.
   bondMorph->init(mechanics);
   createMorph->init(mechanics);
   grappleMorph->init(mechanics);
   propelMorph->init(mechanics);
   orientMorph->init(mechanics);
   typeMorph->init(mechanics);
}


// Load body configuration.
bool Maxwell::load(Body **bodies, int numBodies)
{
   int i, n, r, l, x, y;

   n = r = l = 0;
   Body     *body, *maxwell, *food, *poison, *obstacle;
   Particle *particle;

   // Identify bodies.
   maxwell = food = poison = obstacle = NULL;
   for (i = 0; i < numBodies; i++)
   {
      body = bodies[i];
      if ((particle = body->particles) == NULL)
      {
         continue;
      }
      switch (particle->type)
      {
      case FOOD_TYPE:
         food = body;
         break;

      case POISON_TYPE:
         poison = body;
         break;

      case OBSTACLE_TYPE:
         obstacle = body;
         break;

      case BODY_SIDE_TYPE:
      case BODY_CORNER_TYPE:
         maxwell = body;
         break;
      }
   }
   if (maxwell == NULL)
   {
      Log::logError("Cannot load Maxwell");
      return(false);
   }

   // Clear placement map.
   for (x = 0; x < WIDTH; x++)
   {
      for (y = 0; y < HEIGHT; y++)
      {
         map[x][y] = -1;
      }
   }

   // Place Maxwell.
   body = maxwell->duplicate(mechanics);
   if (!placeBody(body, 0.0f))
   {
      Log::logError("Cannot place Maxwell");
      return(false);
   }

   // Add body to automaton.
   mechanics->addBody(body);

#if (FORAGE_ENV == 1)
   // Add food to forage.
   if (food == NULL)
   {
      Log::logError("Cannot load food");
      return(false);
   }

   // Determine number of patches.
   n  = Random::nextInt(MAX_FOOD_PATCHES - MIN_FOOD_PATCHES + 1);
   n += MIN_FOOD_PATCHES;
   for (i = 0; i < n; i++)
   {
      // Determine radius.
      r  = Random::nextInt(MAX_PATCH_RADIUS - MIN_PATCH_RADIUS + 1);
      r += MIN_PATCH_RADIUS;

      // Map food patch.
#if (SNAPSHOT == 1)
      x = WIDTH / 2;
      y = HEIGHT / 2;
#else
      x = Random::nextInt(WIDTH);
      y = Random::nextInt(HEIGHT);
#endif
      mapPatch(FOOD_TYPE, x, y, r);
   }

   // Create food particles.
   for (x = 0; x < WIDTH; x++)
   {
      for (y = 0; y < HEIGHT; y++)
      {
         if (map[x][y] == FOOD_TYPE)
         {
            body     = food->duplicate(mechanics);
            particle = body->particles;
            particle->vPosition.x = POSITION(x);
            particle->vPosition.y = POSITION(y);
            particle->coefficientOfRestitution = 0.1f;
            mechanics->addBody(body);
         }
      }
   }
#endif

#if (POISON_ENV == 1)
   // Add poison to avoid and repair damage from.
   if (poison == NULL)
   {
      Log::logError("Cannot load poison");
      return(false);
   }

   // Determine number of poison particles.
   n  = Random::nextInt(MAX_POISON_PARTICLES - MIN_POISON_PARTICLES + 1);
   n += MIN_POISON_PARTICLES;
   for (i = 0; i < n; i++)
   {
      body = poison->duplicate(mechanics);
      if (!placeBody(body, MAX_VELOCITY))
      {
         Log::logError("Cannot place poison");
         return(false);
      }
      mechanics->addBody(body);
   }
#endif

#if (OBSTACLE_ENV == 1)
   // Add obstacles to avoid.
   if (obstacle == NULL)
   {
      Log::logError("Cannot load obstacle");
      return(false);
   }

   // Determine number of walls.
   n  = Random::nextInt(MAX_OBSTACLE_WALLS - MIN_OBSTACLE_WALLS + 1);
   n += MIN_OBSTACLE_WALLS;
   for (i = 0; i < n; i++)
   {
      // Determine wall length.
      l  = Random::nextInt(MAX_WALL_LENGTH - MIN_WALL_LENGTH + 1);
      l += MIN_WALL_LENGTH;

      // Map wall.
      x = Random::nextInt(WIDTH);
      y = Random::nextInt(HEIGHT);
      switch (Random::nextInt(4))
      {
      case 0:
         for (int j = 0; j < l; j++)
         {
            if (y >= HEIGHT)
            {
               break;
            }
            if ((map[x][y] != -1) && (map[x][y] != OBSTACLE_TYPE))
            {
               break;
            }
            map[x][y] = OBSTACLE_TYPE;
            y++;
         }
         break;

      case 1:
         for (int j = 0; j < l; j++)
         {
            if (y < 0)
            {
               break;
            }
            if ((map[x][y] != -1) && (map[x][y] != OBSTACLE_TYPE))
            {
               break;
            }
            map[x][y] = OBSTACLE_TYPE;
            y--;
         }
         break;

      case 2:
         for (int j = 0; j < l; j++)
         {
            if (x >= WIDTH)
            {
               break;
            }
            if ((map[x][y] != -1) && (map[x][y] != OBSTACLE_TYPE))
            {
               break;
            }
            map[x][y] = OBSTACLE_TYPE;
            x++;
         }
         break;

      case 3:
         for (int j = 0; j < l; j++)
         {
            if (x < 0)
            {
               break;
            }
            if ((map[x][y] != -1) && (map[x][y] != OBSTACLE_TYPE))
            {
               break;
            }
            map[x][y] = OBSTACLE_TYPE;
            x--;
         }
         break;
      }
   }

   // Create wall particles.
   for (x = 0; x < WIDTH; x++)
   {
      for (y = 0; y < HEIGHT; y++)
      {
         if (map[x][y] == OBSTACLE_TYPE)
         {
            body     = obstacle->duplicate(mechanics);
            particle = body->particles;
            particle->vPosition.x = POSITION(x);
            particle->vPosition.y = POSITION(y);
            particle->setFixed(true);
            mechanics->addBody(body);
         }
      }
   }
#endif

   return(true);
}


// Place body.
bool Maxwell::placeBody(Body *body, float maxVelocity)
{
   int      i;
   Particle *particle;
   double   dx, dy, x, y;

   dx = dy = 0.0;
   for (i = 0; i < MAX_PLACEMENT_TRIES; i++)
   {
      dx = WIDTH * Random::nextDouble();
      dy = HEIGHT * Random::nextDouble();
      for (particle = body->particles; particle != NULL;
           particle = particle->next)
      {
         x = particle->vPosition.x + dx;
         y = particle->vPosition.y + dy;
         if ((x < 0.0) || (x >= WIDTH) || (y < 0.0) || (y >= HEIGHT))
         {
            break;
         }
      }
      if (particle == NULL)
      {
         break;
      }
   }
   if (i == MAX_PLACEMENT_TRIES)
   {
      return(false);
   }
   for (particle = body->particles; particle != NULL;
        particle = particle->next)
   {
      particle->vPosition.x = POSITION(particle->vPosition.x + dx);
      particle->vPosition.y = POSITION(particle->vPosition.y + dy);
   }

   // Randomize body velocity.
   if (maxVelocity > 0.0f)
   {
      body->vVelocity.x = Random::nextDouble() * maxVelocity;
      if (Random::nextBoolean())
      {
         body->vVelocity.x = -body->vVelocity.x;
      }
      body->vVelocity.y = Random::nextDouble() * -maxVelocity;
      if (Random::nextBoolean())
      {
         body->vVelocity.y = -body->vVelocity.y;
      }
   }

   // Map particle placements.
   for (particle = body->particles; particle != NULL;
        particle = particle->next)
   {
      map[(int)particle->vPosition.x][(int)particle->vPosition.y] = particle->type;
   }

   return(true);
}


// Map patch.
void Maxwell::mapPatch(int type, int x, int y, int radius)
{
   if (radius == 0)
   {
      return;
   }
   if (map[x][y] == -1)
   {
      map[x][y] = type;
   }
   if (x > 0)
   {
      mapPatch(type, x - 1, y, radius - 1);
   }
   if (x < WIDTH - 1)
   {
      mapPatch(type, x + 1, y, radius - 1);
   }
   if (y > 0)
   {
      mapPatch(type, x, y - 1, radius - 1);
   }
   if (y < HEIGHT - 1)
   {
      mapPatch(type, x, y + 1, radius - 1);
   }
}


// Pre-morph processing.
void Maxwell::preMorph()
{
}

// Emit signals.
Emission *Maxwell::signal(Neighborhood *neighbors)
{
   Cell     *cell;
   int      i, x, y, dx, dy, action, type;
   Particle *particle;
   Gene     *gene;
   Signal   *signal;
   Emission *emissionList, *emission;

   std::list<Particle *>::const_iterator listItr;
   Orientation orientation;

   // Process particles in neighborhood origin.
   emissionList = NULL;
   cell         = neighbors->cells[1][1];
   for (listItr = cell->particles.begin();
        listItr != cell->particles.end(); listItr++)
   {
      particle = *listItr;

      // Transform neighborhood to particle orientation.
      neighbors->transform(particle->orientation);

      // Perform actions determined by genes.
      for (i = 0; i < NUM_GENES; i++)
      {
         gene = genome->genes[i];

         // Get action.
         if (!gene->matchNeighborhood(neighbors))
         {
            continue;
         }
         if (gene->types[1][1] != particle->type)
         {
            continue;
         }
         if ((action = gene->action) == -1)
         {
            continue;
         }
         x = gene->dx;
         y = gene->dy;
         neighbors->getCellLocation(x, y);

         // Execute action.
         signal = NULL;
         switch (action)
         {
         case CREATE_ACTION:
            // Cannot create food.
            if (gene->type == FOOD_TYPE)
            {
               continue;
            }
            orientation.direction =
               particle->orientation.aim(gene->orientation.direction);
            orientation.mirrored =
               particle->orientation.getMirrorX2(gene->orientation.mirrored);
            signal = createMorph->createCreateSignal(particle,
                                                     orientation, gene->type);
            break;

         case BOND_ACTION:
            signal = bondMorph->createBondSignal(particle, gene->type);
            break;

         case SET_TYPE_ACTION:
            // Cannot change to food.
            if (gene->type == FOOD_TYPE)
            {
               continue;
            }

            // Target must be in neighborhood.
            if ((x < -1) || (x > 1) || (y < -1) || (y > 1))
            {
               continue;
            }
            type = gene->types[x + 1][y + 1];
            if (type < 0)
            {
               continue;
            }
            signal = typeMorph->createTypeSignal(gene->type, type);
            break;

         case ORIENT_ACTION:
            orientation.direction =
               particle->orientation.aim(gene->orientation.direction);
            orientation.mirrored =
               particle->orientation.getMirrorX2(gene->orientation.mirrored);
            signal = orientMorph->createOrientSignal(orientation, gene->type);
            break;

         case UNBOND_ACTION:
            signal = bondMorph->createUnbondSignal(particle, gene->type);
            break;

         case DESTROY_ACTION:
            // Cannot destroy obstacle.
            if (gene->type == OBSTACLE_TYPE)
            {
               continue;
            }
            signal = createMorph->createDestroySignal(gene->type);
            break;

         case GRAPPLE_ACTION:
            if ((x < -1) || (x > 1) || (y < -1) || (y > 1))
            {
               continue;
            }
            orientation.direction = gene->orientation.direction;
            orientation.mirrored  = particle->orientation.mirrored;
            Neighborhood::getDxy(x, y, orientation, dx, dy);
            signal = grappleMorph->createGrappleSignal(particle, dx, dy, gene->type);
            break;

         case PROPEL_ACTION:
            signal = propelMorph->createPropelSignal(particle,
                                                     particle->orientation.aim(gene->orientation.direction),
                                                     gene->strength, gene->tendency, gene->type);
            break;
         }

         // Queue signal emission to target cell.
         if (signal != NULL)
         {
            emission = new Emission(signal, x, y, gene->delay, gene->duration);
            assert(emission != NULL);
            emission->next = emissionList;
            emissionList   = emission;
         }
      }
   }

   // Return signal emissions.
   return(emissionList);
}


// Morph.
void Maxwell::morph(Cell *cell)
{
   // Associated signal processing.
   bondMorph->morph(cell);
   createMorph->morph(cell);
   grappleMorph->morph(cell);
   propelMorph->morph(cell);
   orientMorph->morph(cell);
   typeMorph->morph(cell);
}


// Post-morph processing.
void Maxwell::postMorph()
{
   Body     *body;
   Particle *particle, *removeParticle;
   bool     done;

   // Propel bodies.
   propelMorph->postMorph();

   // Kill particles hit by poison particles.
   done = false;
   while (!done)
   {
      done = true;
      for (body = mechanics->bodies; body != NULL; body = body->next)
      {
         for (particle = body->particles; particle != NULL;
              particle = particle->next)
         {
            if ((particle->type == BODY_SIDE_TYPE) ||
                (particle->type == BODY_CORNER_TYPE))
            {
               if ((particle->collide != NULL) &&
                   mechanics->isValidParticle(particle->collide) &&
                   (particle->collide->type == POISON_TYPE))
               {
                  removeParticle = particle;
                  done           = false;
                  break;
               }
            }
         }
         if (!done)
         {
            break;
         }
      }
      if (!done)
      {
         mechanics->removeParticle(body, removeParticle);
      }
   }
}


// Get morphogen fitness.
double Maxwell::getFitness()
{
   Body   *body;
   int    variance;
   double fitness;

   // Maxwell is "alive"?
   body = findMaxwell(variance);
   if ((body == NULL) || (variance > MIN_VARIANCE))
   {
      return(0.0);
   }
   fitness = (double)(body->energy - variance);
   if (fitness < 0.0)
   {
      fitness = 0.0;
   }
   return(fitness);
}


// Find "best" Maxwell: if found, return body (NULL if not found)
// and measure of its "variance" from ideal.
// Note: Maxwell's sides can be temporarily removed to admit food.
Body *Maxwell::findMaxwell(int& bestVariance)
{
   int      corners, sides, i, j;
   int      variance;
   Body     *body, *bestBody;
   Particle *particle;
   Particle *p[4], *px, *py;
   Particle *ne, *se, *sw, *nw; // corners
   Particle *n, *s, *e, *w;     // sides

   bestBody     = NULL;
   bestVariance = -1;
   for (body = mechanics->bodies; body != NULL; body = body->next)
   {
      variance = 0;

      // Count "foreign" particles.
      for (particle = body->particles; particle != NULL;
           particle = particle->next)
      {
         if ((particle->type != BODY_CORNER_TYPE) &&
             (particle->type != BODY_SIDE_TYPE))
         {
            variance++;
         }
      }

      // Get corners.
      p[0]    = p[1] = p[2] = p[3] = NULL;
      corners = 0;
      for (particle = body->particles; particle != NULL;
           particle = particle->next)
      {
         if (particle->type == BODY_CORNER_TYPE)
         {
            corners++;
            if (p[0] == NULL)
            {
               p[0] = particle;
            }
            else if (p[1] == NULL)
            {
               p[1] = particle;
            }
            else if (p[2] == NULL)
            {
               p[2] = particle;
            }
            else if (p[3] == NULL)
            {
               p[3] = particle;
            }
         }
      }
      if (corners < 4)
      {
         variance += (4 - corners);
      }
      if (corners > 4)
      {
         variance += (corners - 4);
      }

      // Got corners: determine ordering.
      ne = se = sw = nw = NULL;
      for (i = 0; i < 4; i++)
      {
         if (p[i] == NULL)
         {
            continue;
         }
         px = py = NULL;
         for (j = 0; j < 4; j++)
         {
            if (i == j)
            {
               continue;
            }
            if (p[j] == NULL)
            {
               continue;
            }
            if (p[i]->vPosition.x == p[j]->vPosition.x)
            {
               px = p[j];
            }
            if (p[i]->vPosition.y == p[j]->vPosition.y)
            {
               py = p[j];
            }
         }
         if ((px == NULL) || (py == NULL))
         {
            break;
         }
         if (px->vPosition.y > p[i]->vPosition.y)
         {
            if (py->vPosition.x > p[i]->vPosition.x)
            {
               sw = p[i];
            }
            else
            {
               se = p[i];
            }
         }
         else
         {
            if (py->vPosition.x > p[i]->vPosition.x)
            {
               nw = p[i];
            }
            else
            {
               ne = p[i];
            }
         }
      }

      // Get sides.
      p[0]  = p[1] = p[2] = p[3] = NULL;
      sides = 0;
      for (particle = body->particles; particle != NULL;
           particle = particle->next)
      {
         if (particle->type == BODY_SIDE_TYPE)
         {
            sides++;
            if (p[0] == NULL)
            {
               p[0] = particle;
            }
            else if (p[1] == NULL)
            {
               p[1] = particle;
            }
            else if (p[2] == NULL)
            {
               p[2] = particle;
            }
            else if (p[3] == NULL)
            {
               p[3] = particle;
            }
         }
      }
      if (sides < 4)
      {
         variance += (4 - sides);
      }
      if (sides > 4)
      {
         variance += (sides - 4);
      }

      // Got sides: determine ordering.
      n = s = e = w = NULL;
      for (i = 0; i < 4; i++)
      {
         if (p[i] == NULL)
         {
            continue;
         }
         for (j = 0; j < 4; j++)
         {
            if (i == j)
            {
               continue;
            }
            if (p[j] == NULL)
            {
               continue;
            }
            if (p[i]->vPosition.x == p[j]->vPosition.x)
            {
               if (p[i]->vPosition.y > p[j]->vPosition.y)
               {
                  n = p[i];
               }
               else
               {
                  s = p[i];
               }
            }
            if (p[i]->vPosition.y == p[j]->vPosition.y)
            {
               if (p[i]->vPosition.x > p[j]->vPosition.x)
               {
                  e = p[i];
               }
               else
               {
                  w = p[i];
               }
            }
         }
      }

      // Check connectivity.
      if (n != NULL)
      {
         if ((ne != NULL) && (n->getBond(ne) == NULL))
         {
            variance++;
         }
         if ((nw != NULL) && (n->getBond(nw) == NULL))
         {
            variance++;
         }
      }
      if (s != NULL)
      {
         if ((se != NULL) && (s->getBond(se) == NULL))
         {
            variance++;
         }
         if ((sw != NULL) && (s->getBond(sw) == NULL))
         {
            variance++;
         }
      }
      if (e != NULL)
      {
         if ((se != NULL) && (e->getBond(se) == NULL))
         {
            variance++;
         }
         if ((ne != NULL) && (e->getBond(ne) == NULL))
         {
            variance++;
         }
      }
      if (w != NULL)
      {
         if ((sw != NULL) && (w->getBond(sw) == NULL))
         {
            variance++;
         }
         if ((nw != NULL) && (w->getBond(nw) == NULL))
         {
            variance++;
         }
      }

      if ((bestVariance == -1) || (variance < bestVariance))
      {
         bestVariance = variance;
         bestBody     = body;
      }
   }
   return(bestBody);
}


// Create a viable gene.
void Maxwell::createViableGene(Gene *gene, int action)
{
   int x, y;

   // Clear gene.
   gene->clear();

   // Choose a target cell/particle.
   x = Random::nextInt(3);
   y = Random::nextInt(3);
   gene->types[x][y] = Random::nextInt(NUM_PARTICLE_TYPES);
   gene->strength    = Gene::STRENGTH_QUANTUM;
   gene->dx          = x - 1;
   gene->dy          = y - 1;

   // Choose default parameters.
   gene->type = gene->types[x][y];
   gene->orientation.direction = Random::nextInt(8);
   gene->tendency = Gene::TENDENCY_QUANTUM;

   switch (gene->action = action)
   {
   case CREATE_ACTION:
      gene->types[x][y] = Gene::EMPTY_CELL;
      gene->type        = Random::nextInt(NUM_PARTICLE_TYPES);
      break;

   case BOND_ACTION:
      break;

   case SET_TYPE_ACTION:

      // Prevent illegal/ineffective type change.
      if (NUM_PARTICLE_TYPES > 2)
      {
         while (true)
         {
            gene->type = Random::nextInt(NUM_PARTICLE_TYPES);
            if ((gene->type != FOOD_TYPE) && (gene->type !=
                                              gene->types[x][y]))
            {
               break;
            }
         }
      }
      break;

   case ORIENT_ACTION:
      gene->orientation.mirrored = Random::nextBoolean();
      break;

   case UNBOND_ACTION:
      break;

   case DESTROY_ACTION:
      break;

   case GRAPPLE_ACTION:

      gene->orientation.mirrored = Random::nextBoolean();
      break;

   case PROPEL_ACTION:
      break;
   }
}


#if (FORAGING_MOVEMENT_SCREEN == 1)

// Gene produces mobility (self-propulsion)?
bool Maxwell::isMobile(int geneNum)
{
   Particle *particle;
   Gene     *gene;

   // Check center particle for propulsion.
   particle = *testNeighbors.cells[1][1]->particles.begin();
   assert(particle != NULL);

   // Transform neighborhood to particle orientation.
   testNeighbors.transform(particle->orientation);

   // Test gene for propulsion.
   gene = genome->genes[geneNum];
   if ((gene->action == PROPEL_ACTION) &&
       (gene->types[1][1] == particle->type) &&
       (gene->dx == 0) && (gene->dy == 0) &&
       (gene->orientation.direction != CENTER) &&
       gene->matchNeighborhood(&testNeighbors))
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


#endif
