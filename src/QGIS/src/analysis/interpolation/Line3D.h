/***************************************************************************
                          Line3D.h  -  description
                             -------------------
    copyright            : (C) 2004 by Marco Hugentobler
    email                : mhugent@geo.unizh.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LINE3D_H
#define LINE3D_H

#include "Point3D.h"
#include "Node.h"

/** \ingroup analysis
 * This class represents a line. It is implemented as a single directed linked list of nodes (with related Point3D objects). Attention: the points inserted in a line are not deleted from Line3D*/
class ANALYSIS_EXPORT Line3D
{
  private:
    /** Copy constructor, declared private to not use it*/
    Line3D( const Line3D& );
    /** Assignment operator, declared private to not use it*/
    Line3D& operator=( const Line3D& );
  protected:
    Node* head;
    Node* z;
    Node* currentNode;
    unsigned int size;
    unsigned int current;

  public:
    Line3D();
    ~Line3D();
    /** Returns true, if the Line contains no Point3D, otherwise false*/
    bool empty() const;
    /** Inserts a node behind the current position and sets the current position to this new node*/
    void insertPoint( Point3D* p );
    /** Removes the point behind the current position*/
    void removePoint();
    /** Gets the point at the current position*/
    Point3D* getPoint() const;
    /** Returns the current position*/
    unsigned int getCurrent() const;
    /** Returns the size of the line (the numbero of inserted Nodes without 'head' and 'z'*/
    unsigned int getSize() const;
    /** Sets the current Node to head*/
    void goToBegin();
    /** Goes to the next Node*/
    void goToNext();
};

inline Line3D::Line3D( const Line3D& )
{

}

inline Line3D& Line3D::operator=( const Line3D& )
{
  return ( *this );
}

inline unsigned int Line3D::getCurrent() const
{
  return current;
}

inline Point3D* Line3D::getPoint() const
{
  return ( currentNode->getPoint() );
}

inline unsigned int Line3D::getSize() const
{
  return size;
}

#endif
