/* Array.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef ARRAY_INCLUDED
#define ARRAY_INCLUDED

#include "error.h"
#include <vector>
#include <algorithm>

/**
 * This class is a wrapper around the <code>vector<code>, with
 * some added functionalities.
 *
 * @author Jean-Sébastien Senécal
 * @version %I% %G%
 */
template <class T>
class Array : public std::vector<T>
{
public:
  //! Inner type definitions.
  typedef typename std::vector<T>::size_type size_type;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::reference reference;
  typedef typename std::vector<T>::const_reference const_reference;
  typedef typename std::vector<T>::pointer pointer;

public:
  // using (as suggested by Norm)
  using std::vector<T>::end;
  using std::vector<T>::begin;
  using std::vector<T>::front;

  /**
   * Default constructor.
   */
  Array() : std::vector<T>() {}

  /**
   * Constructor
   *
   * @param size the size of the vector
   */
  Array(size_type size) : std::vector<T>(size) {}

  //! Destructor.
  virtual ~Array() {}

  /**
   * Fills the array with some value.
   *
   * @param fillValue the value to fill the array with
   */
  void fill(const_reference fillValue)
  {
    std::fill(begin(), end(), fillValue);
  }

  /**
   * Returns a const pointer to the start of memory.
   *
   * @return a const pointer to the first element in memory
   */
  const pointer data() const { return (const pointer)&front(); }

  /**
   * Returns a pointer to the start of memory.
   *
   * @return a pointer to the first element in memory
   */
  pointer data() { return &front(); }

  // DEPRECATED : use std::vector::empty() method instead.
  bool isNull() const { return empty(); }
};

#endif
