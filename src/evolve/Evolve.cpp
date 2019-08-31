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

/**
 * Evolve morphogen defined MORPHOGEN in Parameters.h
 *
 * New run usage:
 * Evolve -cycles <evolution cycles> -bodies <test body input file name>
 *    [-input <evolution input file name> (for run continuation)]
 *    -output <evolution output file name>
 *    [-logfile <log file name>]
 *    [-display]
 */

#ifdef WIN32
#include <windows.h>
#endif
#ifdef UNIX
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#endif
#include <time.h>
#include <assert.h>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../base/Parameters.h"
#include "../base/Automaton.hpp"
#include "../util/Log.hpp"

// Use test genome.
#define TEST_GENOME    1
#if (TEST_GENOME == 1)
#include "../util/TestGenome.hpp"
#endif

#ifdef WIN32
#ifdef _DEBUG
// For Windows memory checking, set CHECK_MEMORY = 1
#define CHECK_MEMORY    0
#if (CHECK_MEMORY == 1)
#include <crtdbg.h>
#endif
#endif
#endif

// Evolution parameters.
#define FIT_POPULATION_SIZE    10
#define NUM_MUTANTS            8
#define NUM_OFFSPRING          2
#define POPULATION_SIZE        (FIT_POPULATION_SIZE + NUM_MUTANTS + NUM_OFFSPRING)
#define MUTATION_RATE          0.1
#define MORPH_CYCLES           1000
#define EVOLVE_LOGGING         LOG_TO_BOTH

// Evolve food foraging.
#define FORAGE_EVOLVE          1

// Foraging hill-climbing optimization of selected values.
#define FORAGING_HILL_CLIMB    0

#if (FORAGING_MOVEMENT_SCREEN == 1)
bool MobileFound = false;
#endif

// Usage.
char *Usage = "Evolve -cycles <evolution cycles> -bodies <test body input file name>\n \t[-input <evolution input file name> (for run continuation)]\n \t-output <evolution output file name>\n\t[-logfile <log file name>]\n\t[-display]";

// Automaton.
Automaton *automaton;

// Morphogen.
MORPHOGEN *morphogen;

// Test bodies.
Body **TestBodies;
int  NumTestBodies;

// Evolution cycles.
int Cycles;
int CycleCount;

// Population file names.
char *InputFileName;
char *OutputFileName;

// Population member.
class Member
{
public:

   Genome *genome;
   double fitness;
   int    age;

   // Constructors.
   Member(Genome *genome)
   {
      this->genome = genome;
      fitness      = 0.0;
      age          = 0;
   }


   Member(Genome *genome, double fitness, int age)
   {
      this->genome  = genome;
      this->fitness = fitness;
      this->age     = age;
   }


   // Destructor.
   ~Member()
   {
      delete genome;
   }
};

// Population.
Member *Population[POPULATION_SIZE];

// Start/end functions.
void logParameters();
void loadBodies(char *fileName);
void loadPopulation(char *fileName);
void savePopulation(char *fileName);
void terminate(int);

// Evolve functions.
void evolve(), evaluate(), prune(), mutate(), mate();

// Display mode?
bool Display = false;

// Draw grid?
bool DrawGrid = false;

// Quit requested?
bool Quit = false;

// Window size.
#define WINDOW_WIDTH     700
#define WINDOW_HEIGHT    700
int   WindowWidth  = WINDOW_WIDTH;
int   WindowHeight = WINDOW_HEIGHT;
float CellWidth    = (float)WindowWidth / (float)WIDTH;
float CellHeight   = (float)WindowHeight / (float)HEIGHT;

/*
 *      Available fonts:
 *      GLUT_BITMAP_8_BY_13
 *      GLUT_BITMAP_9_BY_15
 *      GLUT_BITMAP_TIMES_ROMAN_10
 *      GLUT_BITMAP_TIMES_ROMAN_24
 *      GLUT_BITMAP_HELVETICA_10
 *      GLUT_BITMAP_HELVETICA_12
 *      GLUT_BITMAP_HELVETICA_18
 */
#define SMALL_FONT    GLUT_BITMAP_8_BY_13
#define FONT          GLUT_BITMAP_9_BY_15
#define BIG_FONT      GLUT_BITMAP_TIMES_ROMAN_24
#define LINE_SPACE    10

// Display functions.
void display(), drawParticle(Particle *), idle(), reshape(int, int);
void keyInput(unsigned char key, int x, int y);

void renderBitmapString(GLfloat, GLfloat, void *, char *);

int main(int argc, char *argv[])
{
   // Logging (see evolve.log file).
   Log::LOGGING_FLAG = EVOLVE_LOGGING;

   // Parse arguments.
   char *bodiesFileName = NULL;
   InputFileName = OutputFileName = NULL;

   for (int i = 1; i < argc; i++)
   {
      if (strcmp(argv[i], "-cycles") == 0)
      {
         i++;
         Cycles = atoi(argv[i]);
         if (Cycles < 0)
         {
            sprintf(Log::messageBuf, "%s: invalid cycles", argv[0]);
            Log::logError();
            exit(1);
         }
         continue;
      }

      if (strcmp(argv[i], "-bodies") == 0)
      {
         i++;
         bodiesFileName = argv[i];
         continue;
      }

      if (strcmp(argv[i], "-input") == 0)
      {
         i++;
         InputFileName = argv[i];
         continue;
      }

      if (strcmp(argv[i], "-output") == 0)
      {
         i++;
         OutputFileName = argv[i];
         continue;
      }

      if (strcmp(argv[i], "-logfile") == 0)
      {
         i++;
         Log::setLogFileName(argv[i]);
         continue;
      }

      if (strcmp(argv[i], "-display") == 0)
      {
         Display = true;
         continue;
      }

      sprintf(Log::messageBuf, "\nUsage: %s", Usage);
      Log::logError();
      exit(1);
   }

   if ((bodiesFileName == NULL) || (OutputFileName == NULL))
   {
      sprintf(Log::messageBuf, "\nUsage: %s", Usage);
      Log::logError();
      exit(1);
   }

   if (InputFileName == NULL)
   {
      sprintf(Log::messageBuf, "Initializing evolve: cycles=%d bodies=%s output=%s",
              Cycles, bodiesFileName, OutputFileName);
   }
   else
   {
      sprintf(Log::messageBuf, "Initializing evolve: cycles=%d bodies=%s input=%s output=%s",
              Cycles, bodiesFileName, InputFileName, OutputFileName);
   }
   Log::logInformation();

   // Seed random numbers.
   Random::setRand(time(NULL));

   // Create automaton containing morphogen.
   automaton = new Automaton();
   assert(automaton != NULL);
   morphogen = &automaton->morphogen;

   // Log run parameters.
   logParameters();

   // Load test bodies.
   loadBodies(bodiesFileName);

   // Load population.
   if (InputFileName == NULL)
   {
      for (int i = 0; i < POPULATION_SIZE; i++)
      {
#if (TEST_GENOME == 1)
         Population[i] = new Member(new TestGenome());
#else
         Population[i] = new Member(morphogen->createGenome());
         Population[i]->genome->randomize();
#endif
         assert(Population[i] != NULL);
      }
   }
   else
   {
      // Continue run.
      loadPopulation(InputFileName);
   }

   Log::logInformation("Begin evolve:");
   CycleCount = 0;
   if (!Display)
   {
#if (FORAGING_MOVEMENT_SCREEN == 1)
      Log::LOGGING_FLAG = NO_LOG;
#endif

      // Evolution loop.
      for ( ; CycleCount < Cycles; CycleCount++)
      {
#if (FORAGING_MOVEMENT_SCREEN == 1)
         if (!MobileFound)
         {
            printf("Cycle=%d\n", CycleCount);
            fflush(stdout);
         }
#endif
         sprintf(Log::messageBuf, "Cycle=%d", CycleCount);
         Log::logInformation();
         evolve();

         // Save population.
#if (FORAGING_MOVEMENT_SCREEN == 1)
         if (MobileFound || ((CycleCount % 1000) == 0))
#endif
         savePopulation(OutputFileName);
      }
#if (FORAGING_MOVEMENT_SCREEN == 1)
      savePopulation(OutputFileName);
#endif

      terminate(0);
   }
   else                 // Display.

   {            // Initialize display.
      glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
      glutInitWindowSize(WindowWidth, WindowHeight);
      char buf[50];
      sprintf(buf, "%s Evolve", MORPHOGEN_NAME);
      glutCreateWindow(buf);
      glutDisplayFunc(display);
      glutReshapeFunc(reshape);
      glutKeyboardFunc(keyInput);
      glutIdleFunc(idle);
      glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0.0f, (float)WindowWidth, 0.0f, (float)WindowHeight);
      glScalef(1, -1, 1);
      glTranslatef(0, -WindowHeight, 0);

      // Start up.
      glutMainLoop();
   }
   return(0);
}


// Log run parameters.
void logParameters()
{
   Log::logInformation("Evolve Parameters:");
   sprintf(Log::messageBuf, "FIT_POPULATION_SIZE = %d", FIT_POPULATION_SIZE);
   Log::logInformation();
   sprintf(Log::messageBuf, "NUM_MUTANTS = %d", NUM_MUTANTS);
   Log::logInformation();
   sprintf(Log::messageBuf, "NUM_OFFSPRING = %d", NUM_OFFSPRING);
   Log::logInformation();
   sprintf(Log::messageBuf, "MUTATION_RATE = %f", MUTATION_RATE);
   Log::logInformation();
   sprintf(Log::messageBuf, "MORPH_CYCLES = %d", MORPH_CYCLES);
   Log::logInformation();
#if (TEST_GENOME == 1)
   sprintf(Log::messageBuf, "TEST_GENOME = TRUE");
#else
   sprintf(Log::messageBuf, "TEST_GENOME = FALSE");
#endif
   Log::logInformation();

   Log::logInformation("Foraging evolution options:");
#if (FORAGE_EVOLVE == 1)
   sprintf(Log::messageBuf, "FORAGE_EVOLVE = TRUE");
#else
   sprintf(Log::messageBuf, "FORAGE_EVOLVE = FALSE");
#endif
   Log::logInformation();
   switch (FORAGING_GENES_INITIALIZATION)
   {
   case EFFECTIVE_FORAGING_VALUES:
      sprintf(Log::messageBuf, "FORAGING_GENES_INITIALIZATION = EFFECTIVE_FORAGING_VALUES");
      break;

   case BASIC_FORAGING_VALUES:
      sprintf(Log::messageBuf, "FORAGING_GENES_INITIALIZATION = BASIC_FORAGING_VALUES");
      break;

   case RANDOM_FORAGING_VALUES:
      sprintf(Log::messageBuf, "FORAGING_GENES_INITIALIZATION = RANDOM_FORAGING_VALUES");
      break;

   case CLEAR_FORAGING_VALUES:
      sprintf(Log::messageBuf, "FORAGING_GENES_INITIALIZATION = CLEAR_FORAGING_VALUES");
      break;
   }
   Log::logInformation();
#if (FORAGING_MOVEMENT_SCREEN == 1)
   sprintf(Log::messageBuf, "FORAGING_MOVEMENT_SCREEN = TRUE");
#else
   sprintf(Log::messageBuf, "FORAGING_MOVEMENT_SCREEN = FALSE");
#endif
   Log::logInformation();
#if (FORAGING_HILL_CLIMB == 1)
   sprintf(Log::messageBuf, "FORAGING_HILL_CLIMB = TRUE");
#else
   sprintf(Log::messageBuf, "FORAGING_HILL_CLIMB = FALSE");
#endif
   Log::logInformation();

   Log::logInformation("Morphogen Parameters:");
#if (FORAGE_ENV == 1)
   sprintf(Log::messageBuf, "FORAGE_ENV = TRUE");
#else
   sprintf(Log::messageBuf, "FORAGE_ENV = FALSE");
#endif
   Log::logInformation();
#if (POISON_ENV == 1)
   sprintf(Log::messageBuf, "POISON_ENV = TRUE");
#else
   sprintf(Log::messageBuf, "POISON_ENV = FALSE");
#endif
   Log::logInformation();
#if (OBSTACLE_ENV == 1)
   sprintf(Log::messageBuf, "OBSTACLE_ENV = TRUE");
#else
   sprintf(Log::messageBuf, "OBSTACLE_ENV = FALSE");
#endif
   Log::logInformation();
   sprintf(Log::messageBuf, "MORPHOGEN = %s", MORPHOGEN_NAME);
   Log::logInformation();
   sprintf(Log::messageBuf, "NUM_PARTICLE_TYPES = %d", NUM_PARTICLE_TYPES);
   Log::logInformation();
   sprintf(Log::messageBuf, "NUM_GENES = %d", NUM_GENES);
   Log::logInformation();
   sprintf(Log::messageBuf, "NUM_ACTIONS = %d", NUM_ACTIONS);
   Log::logInformation();
}


// Load test bodies.
void loadBodies(char *fileName)
{
   FILE *fp;

   if ((fp = fopen(fileName, "r")) == NULL)
   {
      sprintf(Log::messageBuf, "Cannot load test body file %s", fileName);
      Log::logError();
      exit(1);
   }
   fscanf(fp, "%d", &NumTestBodies);
   if (NumTestBodies < 0)
   {
      sprintf(Log::messageBuf, "Invalid number of test bodies: %d", NumTestBodies);
      Log::logError();
      exit(1);
   }
   TestBodies = new Body *[NumTestBodies];
   assert(TestBodies != NULL);
   for (int i = 0; i < NumTestBodies; i++)
   {
      TestBodies[i] = Body::read(fp, &automaton->mechanics);
   }
   fclose(fp);
}


// Load evolution population.
void loadPopulation(char *fileName)
{
   FILE   *fp;
   char   buf[100];
   double fitness;
   int    age;

   if ((fp = fopen(fileName, "r")) == NULL)
   {
      sprintf(Log::messageBuf, "Cannot load population file %s", fileName);
      Log::logError();
      exit(1);
   }
   for (int i = 0; i < POPULATION_SIZE; i++)
   {
      fscanf(fp, "%s", buf);
      fitness = atof(buf);
      fscanf(fp, "%d", &age);
      Population[i] = new Member(Genome::read(fp), fitness, age);
      assert(Population[i] != NULL);
   }
   fclose(fp);
}


// Save evolution population.
void savePopulation(char *fileName)
{
   FILE *fp;

   if ((fp = fopen(fileName, "w")) == NULL)
   {
      sprintf(Log::messageBuf, "Cannot save to population file %s", fileName);
      Log::logError();
      exit(1);
   }
   for (int i = 0; i < POPULATION_SIZE; i++)
   {
      fprintf(fp, "%f %d\n", Population[i]->fitness, Population[i]->age);
      Genome::write(fp, Population[i]->genome);
   }
   fclose(fp);
}


// Terminate.
void terminate(int code)
{
   int i;

   // Release memory.
   if (automaton != NULL)
   {
      delete automaton;
      automaton = NULL;
   }
   if (TestBodies != NULL)
   {
      for (i = 0; i < NumTestBodies; i++)
      {
         if (TestBodies[i] != NULL)
         {
            delete TestBodies[i];
            TestBodies[i] = NULL;
         }
      }
      delete TestBodies;
      TestBodies = NULL;
   }
   for (i = 0; i < POPULATION_SIZE; i++)
   {
      if (Population[i] != NULL)
      {
         delete Population[i];
         Population[i] = NULL;
      }
   }

#ifdef WIN32
#if (CHECK_MEMORY == 1)
   // Check for memory leaks.
   HANDLE hFile = CreateFile(        // Dump to temp log.
      TEMP_LOG_FILE_NAME,
      GENERIC_WRITE,
      FILE_SHARE_WRITE,
      NULL,
      OPEN_ALWAYS,
      0,
      NULL
      );
   if (hFile == INVALID_HANDLE_VALUE)
   {
      sprintf(Log::messageBuf, "Cannot open memory check temporary file %s",
              TEMP_LOG_FILE_NAME);
      Log::logError();
      exit(1);
   }
   Log::logInformation("\nMemory leak check output:");
   _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_WARN, hFile);
   _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_ERROR, hFile);
   if (!_CrtDumpMemoryLeaks())
   {
      Log::logInformation("No memory leaks");
      CloseHandle(hFile);
   }
   else
   {
      CloseHandle(hFile);
      Log::appendTempLog();
   }
   Log::removeTempLog();
#endif
#endif

   Log::logInformation("End evolve");
   Log::close();
   exit(code);
}


// Evolution cycle.
void evolve()
{
   // Evaluate member fitness.
   evaluate();

   // Prune unfit members.
   prune();

   // Create new members by mutation.
   mutate();

   // Create new members by mating.
   mate();
}


// Evaluate member fitnesses.
void evaluate()
{
   int    i, j;
   Member *member;
   long   seed;

   Log::logInformation("Evaluate:");

   // Get random seed.
   seed = Random::nextInt();
   sprintf(Log::messageBuf, "  Random seed=%d", seed);
   Log::logInformation();

   for (i = 0; i < POPULATION_SIZE; i++)
   {
      Random::setRand(seed);

      // Install member genome.
      member = Population[i];

      // Create automaton containing morphogen.
      delete automaton;
      automaton = new Automaton();
      assert(automaton != NULL);
      morphogen = &automaton->morphogen;

      // Load member genome into morphogen.
      morphogen->setGenome(member->genome->duplicate());

      // Load test bodies into morphogen.
      if (!morphogen->load(TestBodies, NumTestBodies))
      {
         Log::logError("Cannot load morphogen");
         exit(1);
      }

#if (FORAGE_EVOLVE == 1 && FORAGING_MOVEMENT_SCREEN == 1)
      if (morphogen->isMobile(21) ||
          morphogen->isMobile(22) ||
          morphogen->isMobile(23))
      {
         MobileFound       = true;
         Log::LOGGING_FLAG = EVOLVE_LOGGING;
#endif

      // Morph.
      for (j = 0; j < MORPH_CYCLES; j++)
      {
         automaton->morph();

         // Display?
         if (Display)
         {
            display();
         }
      }

#if (FORAGE_EVOLVE == 1 && FORAGING_MOVEMENT_SCREEN == 1)
   }
   else
   {
      if (Display)
      {
         display();
      }
   }
#endif

      // Accumulate fitness.
      member->fitness = (member->fitness * member->age) + morphogen->getFitness();
      member->age++;
      member->fitness /= member->age;
      sprintf(Log::messageBuf, "  Member=%d, Fitness=%f, Age=%d",
              i, member->fitness, member->age);
      Log::logInformation();
   }
}


// Prune unfit members.
void prune()
{
   double max;
   int i, j, m;
   Member *member;
   Member *fitPopulation[FIT_POPULATION_SIZE];

   Log::logInformation("Prune:");
   for (i = 0; i < FIT_POPULATION_SIZE; i++)
   {
      m   = -1;
      max = 0.0;
      for (j = 0; j < POPULATION_SIZE; j++)
      {
         member = Population[j];
         if (member == NULL)
         {
            continue;
         }
         if ((m == -1) || (member->fitness > max))
         {
            m   = j;
            max = member->fitness;
         }
      }
      member           = Population[m];
      Population[m]    = NULL;
      fitPopulation[i] = member;
      sprintf(Log::messageBuf, "  Fitness=%f, Age=%d",
              member->fitness, member->age);
      Log::logInformation();
   }
   for (i = 0; i < POPULATION_SIZE; i++)
   {
      if (Population[i] != NULL)
      {
         delete Population[i];
         Population[i] = NULL;
      }
   }
   for (i = 0; i < FIT_POPULATION_SIZE; i++)
   {
      Population[i] = fitPopulation[i];
   }
}


// Mutate members.
void mutate()
{
   int i, j, k, x, y;
   Member *member, *mutant;
   Genome *genome;
   Gene *gene;

   Log::logInformation("Mutate:");
   for (i = 0; i < NUM_MUTANTS; i++)
   {
      // Select a fit member to mutate.
      j      = Random::nextInt(FIT_POPULATION_SIZE);
      member = Population[j];
      sprintf(Log::messageBuf, "  Member=%d", j);
      Log::logInformation();

      // Create new member.
      genome = member->genome->duplicate();
      mutant = new Member(genome);
      assert(mutant != NULL);
      Population[FIT_POPULATION_SIZE + i] = mutant;

      // Mutate.
      for (j = 0; j < NUM_GENES; j++)
      {
         gene = genome->genes[j];

#if (FORAGE_EVOLVE == 1)
         // Only mutate foraging genes.
         if ((j < 21) || (j > 23))
         {
            continue;
         }
#endif

#if (FORAGE_EVOLVE == 1 && FORAGING_HILL_CLIMB == 1)
         // Random hill-climb of selected values.
         switch (Random::nextInt(4))
         {
         case 0:
#if (FORAGING_GENES_INITIALIZATION != EFFECTIVE_FORAGING_VALUES)
            if (Random::nextBoolean())
            {
               gene->orientation.direction++;
               gene->orientation.direction %= 8;
            }
            else
            {
               gene->orientation.direction--;
               if (gene->orientation.direction < 0)
               {
                  gene->orientation.direction += 8;
               }
            }
#endif
            break;

         case 1:
            if (Random::nextBoolean())
            {
               gene->strength += Gene::STRENGTH_QUANTUM;
            }
            else
            {
               gene->strength -= Gene::STRENGTH_QUANTUM;
            }
            if (gene->strength > Gene::MAX_STRENGTH)
            {
               gene->strength = Gene::MAX_STRENGTH;
            }
            if (gene->strength < 0.0)
            {
               gene->strength = 0.0;
            }
            break;

         case 2:
            if (Random::nextBoolean())
            {
               gene->delay++;
            }
            else
            {
               gene->delay--;
            }
            if (gene->delay > Gene::MAX_DELAY)
            {
               gene->delay = Gene::MAX_DELAY;
            }
            if (gene->delay < 0)
            {
               gene->delay = 0;
            }
            break;

         case 3:
            if (Random::nextBoolean())
            {
               gene->duration++;
            }
            else
            {
               gene->duration--;
            }
            if (gene->duration > Gene::MAX_DURATION)
            {
               gene->duration = Gene::MAX_DURATION;
            }
            if (gene->duration < 0)
            {
               gene->duration = 0;
            }
            break;
         }

#else
         // Mutate action index?
         if (Random::nextDouble() < MUTATION_RATE)
         {
            // Create a viable gene for chosen action.
            morphogen->createViableGene(gene, Random::nextInt(NUM_ACTIONS));
            continue;
         }

         // Mutate matching types.
         for (x = 0; x < 3; x++)
         {
            for (y = 0; y < 3; y++)
            {
               if (Random::nextDouble() < MUTATION_RATE)
               {
                  if (Random::nextBoolean())
                  {
                     switch (Random::nextInt(3))
                     {
                     case 0:
                        gene->types[x][y] = Gene::IGNORE_CELL;
                        break;

                     case 1:
                        gene->types[x][y] = Gene::EMPTY_CELL;
                        break;

                     case 2:
                        gene->types[x][y] = Gene::OCCUPIED_CELL;
                        break;
                     }
                  }
                  else
                  {
                     gene->types[x][y] = Random::nextInt(NUM_PARTICLE_TYPES);
                  }
               }
            }
         }

         // Mutate target location.
         if (Random::nextDouble() < MUTATION_RATE)
         {
            k = Random::nextInt(Gene::MAX_TARGET_DISTANCE + 1);
            if (Random::nextBoolean())
            {
               k = -k;
            }
            gene->dx = k;
            k        = Random::nextInt(Gene::MAX_TARGET_DISTANCE + 1);
            if (Random::nextBoolean())
            {
               k = -k;
            }
            gene->dy = k;
         }

         // Mutate parameters.
         if (Random::nextDouble() < MUTATION_RATE)
         {
            gene->type = Random::nextInt(NUM_PARTICLE_TYPES);
         }
         if (Random::nextDouble() < MUTATION_RATE)
         {
            gene->orientation.direction = Random::nextInt(8);
            gene->orientation.mirrored  = Random::nextBoolean();
         }
         if (Random::nextDouble() < MUTATION_RATE)
         {
            k = (int)(Gene::MAX_STRENGTH / Gene::STRENGTH_QUANTUM);
            gene->strength =
               (double)(Random::nextInt(k + 1)) * Gene::STRENGTH_QUANTUM;
         }
         if (Random::nextDouble() < MUTATION_RATE)
         {
            k = (int)(Gene::MAX_TENDENCY_MAGNITUDE / Gene::TENDENCY_QUANTUM);
            gene->tendency =
               (double)(Random::nextInt(k + 1)) * Gene::TENDENCY_QUANTUM;
            if (Random::nextBoolean())
            {
               gene->tendency = -gene->tendency;
            }
         }
         if (Random::nextDouble() < MUTATION_RATE)
         {
            gene->delay = Random::nextInt(Gene::MAX_DELAY + 1);
         }
         if (Random::nextDouble() < MUTATION_RATE)
         {
            gene->duration = Random::nextInt(Gene::MAX_DURATION) + 1;
         }
#endif
      }
   }
}


// Produce offspring though matings.
void mate()
{
   int i, j, k;
   Member *member1, *member2, *offspring;
   Genome *offspringGenome;

   Log::logInformation("Mate:");
   for (i = 0; i < NUM_OFFSPRING; i++)
   {
      // Select a pair of fit members to mate.
      j       = Random::nextInt(FIT_POPULATION_SIZE);
      member1 = Population[j];
      while ((k = Random::nextInt(FIT_POPULATION_SIZE)) == j)
      {
      }
      member2 = Population[k];
      sprintf(Log::messageBuf, "  Members=%d,%d", j, k);
      Log::logInformation();

      // Create offspring.
      offspringGenome = morphogen->createGenome();
      offspringGenome->clear();
      for (j = 0; j < NUM_GENES; j++)
      {
         if (Random::nextBoolean())
         {
            offspringGenome->genes[j] =
               member1->genome->genes[j]->duplicate();
         }
         else
         {
            offspringGenome->genes[j] =
               member2->genome->genes[j]->duplicate();
         }
      }
      offspring = new Member(offspringGenome);
      assert(offspring != NULL);
      Population[FIT_POPULATION_SIZE + NUM_MUTANTS + i] = offspring;
   }
}


// Display.
void display()
{
   int x, y;
   float x2, y2, x3, y3;
   Body *body;
   Particle *particle;
   Bond *bond;

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0f, 0.0f, 0.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Draw morphogen.
   if (automaton != NULL)
   {
      glBegin(GL_LINES);
      if (DrawGrid)
      {
         y2 = (float)WindowHeight;
         for (x = 1, x2 = CellWidth - 1.0f; x < WIDTH;
              x++, x2 = (CellWidth * (float)x) - 1.0f)
         {
            glVertex2f(x2, 0.0f);
            glVertex2f(x2, y2);
         }
         x2 = (float)WindowWidth;
         for (y = 1, y2 = CellHeight - 1.0f; y < HEIGHT;
              y++, y2 = (CellHeight * (float)y) - 1.0f)
         {
            glVertex2f(0.0f, y2);
            glVertex2f(x2, y2);
         }
      }
      else
      {
         glVertex2f(0.0f, 0.0f);
         glVertex2f((float)WindowWidth, 0.0f);
         glVertex2f(0.0f, (float)WindowHeight - 1.0f);
         glVertex2f((float)WindowWidth, (float)WindowHeight - 1.0f);
      }
      glEnd();

      for (body = automaton->mechanics.bodies; body != NULL;
           body = body->next)
      {
         for (particle = body->particles; particle != NULL;
              particle = particle->next)
         {
            drawParticle(particle);
            glBegin(GL_LINES);
            for (bond = particle->bonds; bond != NULL; )
            {
               x2 = CellWidth * bond->particle1->vPosition.x;
               y2 = CellHeight * bond->particle1->vPosition.y;
               y2 = WindowHeight - y2;
               x3 = CellWidth * bond->particle2->vPosition.x;
               y3 = CellHeight * bond->particle2->vPosition.y;
               y3 = WindowHeight - y3;
               glVertex2f(x2, y2);
               glVertex2f(x3, y3);
               if (bond->particle1 == particle)
               {
                  bond = bond->next1;
               }
               else
               {
                  bond = bond->next2;
               }
            }
            glEnd();
         }
      }
   }

   glutSwapBuffers();
   glFlush();
}


// Draw a particle.
void drawParticle(Particle *particle)
{
   if (particle->type == WALL_TYPE)
   {
      return;
   }

   // Show charge in color.
   glColor3f(0.0f, 0.0f, 0.0f);
   if (particle->fCharge > 0.0)
   {
      glColor3f(1.0f, 0.0f, 0.0f);
   }
   if (particle->fCharge < 0.0)
   {
      glColor3f(0.0f, 0.0f, 1.0f);
   }

   // Draw particle.
   glBegin(GL_LINE_LOOP);
   float x = CellWidth * particle->vPosition.x;
   float y = CellHeight * particle->vPosition.y;
   y = WindowHeight - y;
   float rx = CellWidth * particle->fRadius;
   float ry = CellHeight * particle->fRadius;
   if (particle->fixed)
   {
      // Draw a square.
      glVertex2f(x + rx, y + ry);
      glVertex2f(x + rx, y - ry);
      glVertex2f(x - rx, y - ry);
      glVertex2f(x - rx, y + ry);
   }
   else
   {
      // Draw circle.
      glVertex2f(x + rx, y);
      int sides = 20;
      float ad  = 360.0f / (float)sides;
      float a   = ad;
      for (int i = 1; i < sides; i++, a += ad)
      {
         glVertex2f(x + (rx * cos(DegreesToRadians(a))),
                    y + (ry * sin(DegreesToRadians(a))));
      }
   }
   glEnd();

   // Draw particle type.
   char buf[20];
   switch (particle->type)
   {
   case 0:
      sprintf(buf, "F");
      break;

   case 1:
      sprintf(buf, "P");
      break;

   case 2:
   case 3:
      sprintf(buf, "M");
      break;

   case 4:
      sprintf(buf, "O");
      break;

   case 5:
   case 6:
      sprintf(buf, "D");
      break;

   default:
      sprintf(buf, "%d", particle->type);
   }
#if (SNAPSHOT == 1)
   renderBitmapString(x - (CellWidth / 3.0), y + (CellHeight / 3.0), BIG_FONT, buf);
#else
   renderBitmapString(x - (CellWidth / 3.0), y + (CellHeight / 3.0), FONT, buf);
#endif
}


// Idle.
void idle()
{
   // Run a cycle.
   sprintf(Log::messageBuf, "Cycle=%d", CycleCount);
   Log::logInformation();
   evolve();

   // Save population.
   savePopulation(OutputFileName);

   // Done?
   CycleCount++;
   if ((CycleCount == Cycles) || Quit)
   {
      terminate(0);
   }
}


// Reshape window.
void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   WindowWidth  = w;
   WindowHeight = h;
   CellWidth    = (float)WindowWidth / (float)WIDTH;
   CellHeight   = (float)WindowHeight / (float)HEIGHT;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0f, (float)WindowWidth, 0.0f, (float)WindowHeight);
   glScalef(1, -1, 1);
   glTranslatef(0, -WindowHeight, 0);
}


// Keyboard input.
void keyInput(unsigned char key, int x, int y)
{
   if (key == 'q')
   {
      Quit = true;
   }
}


// Print string on screen at specified location.
void renderBitmapString(GLfloat x, GLfloat y, void *font, char *string)
{
   char *c;

   glRasterPos2f(x, y);
   for (c = string; *c != '\0'; c++)
   {
      glutBitmapCharacter(font, *c);
   }
}
