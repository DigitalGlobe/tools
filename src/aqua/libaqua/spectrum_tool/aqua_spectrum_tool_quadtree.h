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


#ifndef AQUA_SPECTRUM_TOOL_QUADTREE_H
#define AQUA_SPECTRUM_TOOL_QUADTREE_H


/****************  includes  ****************/


/*  C++ lib  */
#include <vector>


/****************  external functions  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Directional;
  }
}


/****************  classes  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    class Quadtree
    {
    public:

      Quadtree(const class spectrum::Directional & spectrum);
      ~Quadtree(void);

      /****  get  ****/

      class Quadtree_Node * get_root(void);


    protected:

      class std::vector<std::vector<class Quadtree_Node *> > node_arrays;


      void compute_integral(class std::vector<double> & integral,
			    const class spectrum::Directional & spectrum,
			    const size_t size_theta,
			    const size_t size_omega,
			    const double step_theta,
			    const double step_omega,
			    const double origin_omega) const;
      /*  fills the leaf array of “node_arrays” using “integral”  */
      void
      fill_leaf_array(class std::vector<class Quadtree_Node *> & node_array,
		      const class std::vector<double> & integral,
		      size_t integral_size_theta,
		      size_t integral_size_omega,
		      double step_theta,
		      double step_omega,
		      double origin_theta,
		      double origin_omega,
		      double coefficient);
      /*
	adds and fills the last array of “node_arrays” using the previous one,
	until…
      */
      void
      fill_node_array(std::vector<std::vector<Quadtree_Node *> > &node_arrays,
		      size_t size_theta,
		      size_t size_omega,
		      double coefficient);
      /*  adds the last array of “node_arrays” using the previous one  */
      void
      add_root(std::vector<std::vector<Quadtree_Node *> > &node_arrays,
	       double coefficient);


    private:

      /****  not defined  ****/
      Quadtree(const class Quadtree &);
      void operator =(const class Quadtree &);
    };
  }
}


#endif  /*  AQUA_SPECTRUM_TOOL_QUADTREE_H  */
