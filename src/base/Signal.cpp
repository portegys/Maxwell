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
 * Signal.
 */

#include "Signal.hpp"

// Constructors.
Signal::Signal(Compound *type)
{
   this->type = type;
   parameters = NULL;
   strength   = 0.0;
}


Signal::Signal(Compound *type, void **parameters)
{
   this->type       = type;
   this->parameters = parameters;
   strength         = 0.0;
}


Signal::Signal(Compound *type, void **parameters, double strength)
{
   this->type       = type;
   this->parameters = parameters;
   this->strength   = strength;
}


// Destructor
Signal::~Signal()
{
   delete type;
   delete parameters;
}
