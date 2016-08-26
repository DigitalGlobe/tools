/*
  This file is part of “The Aqua library”.

  Copyright © 2005  Jocelyn Fréchot

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


/*  no constant object  */


/****************  includes  ****************/


#include "aqua_spectrum_tool_quadtree_node.h"

#include "aqua_spectrum_tool_wave_polar.h"

/*  C lib  */
#include <cmath>
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum_tool;


/*******************************************************/
/****************  class Quadtree_Node  ****************/
/*******************************************************/


/****************  public functions  ****************/


/*  amplitude = √(E * coeff)  */
Quadtree_Node::Quadtree_Node(double energy,
			     double theta,
			     double omega,
			     double energy_to_amplitude_coeff,
			     class Quadtree_Node * child_1,
			     class Quadtree_Node * child_2,
			     class Quadtree_Node * child_3,
			     class Quadtree_Node * child_4)
  : wave(new Wave_Polar(this->compute_amplitude(energy,
						energy_to_amplitude_coeff),
			theta,
			omega)),
    energy(energy)
{
  this->children[0] = child_1;
  this->children[1] = child_2;
  this->children[2] = child_3;
  this->children[3] = child_4;
}


/****  get  ****/

class Wave_Polar
Quadtree_Node::get_wave(void)
{
  return *this->wave;
}


/*  θ = −θ  */
class Wave_Polar
Quadtree_Node::get_wave_opposite(void)
{
  return Wave_Polar(this->wave->amplitude,
		    - this->wave->theta,
		    this->wave->omega);
}


class Quadtree_Node *
Quadtree_Node::get_child(size_t index)
{
  return this->children[index];
}


double
Quadtree_Node::get_energy(void)
{
  return this->energy;
}


double
Quadtree_Node::get_amplitude(void)
{
  return this->wave->amplitude;
}


double
Quadtree_Node::get_theta(void)
{
  return this->wave->theta;
}


double
Quadtree_Node::get_omega(void)
{
  return this->wave->omega;
}


/****  operators  ****/

bool
Quadtree_Node::operator < (const class Quadtree_Node & cell) const
{
  return this->energy < cell.energy;
}


/****************  protected functions  ****************/


/*  a = √(E * coeff)  */
double
Quadtree_Node::compute_amplitude(double energy, double coefficient)
{
  return sqrt(energy * coefficient);
}


/*****************************************************************/
/****************  class Quadtree_Node_Less_Than  ****************/
/*****************************************************************/


/****************  public functions  ****************/


bool
Quadtree_Node_Less_Than::
operator () (const class Quadtree_Node * cell_1,
	     const class Quadtree_Node * cell_2) const
{
  return *cell_1 < *cell_2;
}
