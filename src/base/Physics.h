/*
 * Physics parameters.
 * Source: "Physics for Game Developers", Copyright 2000-2001 by David Bourg.
 */

#ifndef __PHYSICS__
#define __PHYSICS__

#include "../util/Math_etc.h"

// Parameters.
#define DEFAULT_RADIUS              0.5f
#define FIXED_RADIUS                5.0f    // Radius of fixed body.
#define DEFAULT_MASS                1.0f
#define FIXED_MASS                  1000.0f // Mass of fixed body.
#define DEFAULT_CHARGE              0.0f
#define CHARGECONSTANT              1.0f
#define COEFFICIENTOFRESTITUTION    1.0f
#define MAX_VELOCITY                0.5f
#define VISCOSITY_FRICTION          0.1f
#define MAX_PARTICLES               5000

// Quantized positioning.
#define POSITION(x)    ((float)((int)(x)) + 0.5f)

#endif
