/*  Emacs mode: -*- C++ -*-  */
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


#ifndef AQUA_SPECTRUM_TOOL_QUADTREE_NODE_H
#define AQUA_SPECTRUM_TOOL_QUADTREE_NODE_H


/****************  includes  ****************/


/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  classes  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    /****  class Quadtree_Node  ****/

    class Quadtree_Node
    {
    public:

      /*  amplitude = √(E * coeff)  */
      Quadtree_Node(double energy,
		    double theta,
		    double omega,
		    double energy_to_amplitude_coeff,
		    class Quadtree_Node * child_1,
		    class Quadtree_Node * child_2,
		    class Quadtree_Node * child_3,
		    class Quadtree_Node * child_4);

      /****  get  ****/

      class Wave_Polar get_wave(void);
      /*  θ = 2 θp − θ  */
      class Wave_Polar get_wave_opposite(void);

      class Quadtree_Node * get_child(size_t index);

      double get_energy(void);
      double get_amplitude(void);
      double get_theta(void);
      double get_omega(void);

      /****  operators  ****/

      bool operator < (const class Quadtree_Node & cell) const;


    protected:

      class Wave_Polar * wave;
      class Quadtree_Node * children[4];

      double energy;
 

      /*  a = √(E * coeff)  */
      double compute_amplitude(double energy, double coefficient);


    private:

      /*  we allow default copy constructor and assignement operator  */
    };


    /****  class Quadtree_Node_Less_Than  ****/

    class Quadtree_Node_Less_Than
    {
    public:

      bool operator () (const class Quadtree_Node * cell_1,
			const class Quadtree_Node * cell_2) const;
    };
  }
}


#endif  /*  AQUA_SPECTRUM_TOOL_QUADTREE_NODE_H  */
