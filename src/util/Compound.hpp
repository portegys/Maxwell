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
 * Compound number.
 */

#ifndef __COMPOUND__
#define __COMPOUND__

#include <stdio.h>
#include <assert.h>

class Compound
{
public:

   int *value;
   int size;

   // Constructor.
   Compound(int *value, int size)
   {
      this->value = new int[size];
      assert(this->value != NULL);
      for (int i = 0; i < size; i++) { this->value[i] = value[i]; }
      this->size = size;
   }


   // Destructor.
   ~Compound()
   {
      delete value;
   }


   // Clone.
   Compound *clone()
   {
      Compound *compound = new Compound(value, size);

      assert(compound != NULL);
      return(compound);
   }


   // Equality.
   bool equals(Compound *compound)
   {
      if (size != compound->size) { return(false); }
      for (int i = 0; i < size; i++)
      {
         if (value[i] != compound->value[i]) { return(false); }
      }
      return(true);
   }
};
#endif
