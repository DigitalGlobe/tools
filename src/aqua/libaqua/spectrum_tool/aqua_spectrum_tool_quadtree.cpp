/*
  This file is part of “The Aqua library”.

  Copyright © 2005 2006  Jocelyn Fréchot

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


/****************  includes  ****************/


#include "aqua_spectrum_tool_quadtree.h"

#include "aqua_spectrum_tool_quadtree_node.h"
#include "aqua_spectrum_tool_sum_four_neighbours.h"

#include "spectrum/aqua_spectrum_directional.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>


/****************  namespaces  ****************/


using namespace aqua::spectrum_tool;
using namespace aqua::spectrum;
using std::vector;


/****************  public functions  ****************/


const size_t samples_omega = 1024;               /*  must be a power of two */
const size_t samples_theta = samples_omega / 2;


Quadtree::Quadtree(const class spectrum::Directional & spectrum)
{
  /*  quantification values  */
  class vector<double> integral(samples_theta * samples_omega);

  //  should be constants
  double top, right, bottom;
  double step_theta, step_omega;

  /*  1 ∕ coeff = 2 ∕ (4 ∆θ ∆ω)  */
  double coefficient;


  /*  sets “top”, “right” and “bottom”  */
  spectrum.find_boundaries(top, right, bottom);

  /*  fills “integral”  */
  step_theta = (right - 0.0) / samples_theta;
  step_omega = (top - bottom) / samples_omega;

  this->compute_integral(integral,
			 spectrum,
			 samples_theta,
			 samples_omega,
			 step_theta,
			 step_omega,
			 bottom);


  /*  fills leaf array of “node_arrays”  */
  coefficient = step_omega * step_theta / 2.0;
  this->node_arrays.push_back(vector<class Quadtree_Node *>());

  this->fill_leaf_array(this->node_arrays.back(),
			integral,
			samples_theta,
			samples_omega,
			step_theta,
			step_omega,
			0.0 + step_theta / 2.0,
			bottom + step_omega / 2.0,
			coefficient);
  /*  adds and fills “node_arrays”  */
  this->fill_node_array(this->node_arrays,
			samples_theta / 2,
			samples_omega / 2,
			coefficient);

  /*  adds last array of “node_arrays”  */
  this->add_root(this->node_arrays, coefficient);
}


Quadtree::~Quadtree(void)
{
  for (size_t i = 0; i < this->node_arrays.size(); i++)
    {
      for (size_t j = 0; j < this->node_arrays[i].size(); j++)
	{
	  delete this->node_arrays[i][j];
	}
    }
}


/****  get  ****/

class Quadtree_Node *
Quadtree::get_root(void)
{
  return this->node_arrays.back().back();
}


/****************  protected functions  ****************/


void
Quadtree::compute_integral(class std::vector<double> & integral,
			   const class spectrum::Directional & spectrum,
			   const size_t size_theta,
			   const size_t size_omega,
			   const double step_theta,
			   const double step_omega,
			   const double origin_omega) const
{
  const size_t temp_size_theta = size_theta + 1;
  const size_t temp_size_omega = size_omega + 1;

  class vector<double> temp(temp_size_theta * temp_size_omega);


  /*  fills “temp” with spectrum values  */
  for (size_t i = 0; i < temp_size_theta; i++)
    {
      for (size_t j = 0; j < temp_size_omega; j++)
	{
	  temp[i * temp_size_omega + j] =
	    spectrum.compute(0.0          + i * step_theta,
			     origin_omega + j * step_omega);
	}
    }

  /*  fills “integral” with integral values  */
  /*
    Does not really compute the mean, just the sum of the four values, which
    will be normalized later. See “class Quadtree_Node”.
  */
  for (size_t i = 0; i < size_theta; i++)
    {
      for (size_t j = 0; j < size_omega; j++)
	{
	  integral[i * size_omega + j]
	    = sum_four_neighbours(temp, temp_size_omega, i, j);
	}
    }
}


/*  fills the leaf array of “node_arrays” using “integral”  */
void
Quadtree::
fill_leaf_array(class std::vector<class Quadtree_Node *> & node_array,
		const class std::vector<double> & integral,
		size_t integral_size_theta,
		size_t integral_size_omega,
		double step_theta,
		double step_omega,
		double origin_theta,
		double origin_omega,
		double coefficient)
{
  size_t index_integral_0_0, index_integral_1_0;
  size_t index_integral_0_1, index_integral_1_1;
  double energy, theta, omega;

  for (size_t i = 0; i < integral_size_theta; i += 2)
    {
      theta = origin_theta + (i + 0.5) * step_theta;

      for (size_t j = 0; j < integral_size_omega; j += 2)
	{
	  omega = origin_omega + (j + 0.5) * step_omega;

	  index_integral_0_0 = (i    ) * integral_size_omega + (j    );
	  index_integral_1_0 = (i + 1) * integral_size_omega + (j    );
	  index_integral_0_1 = (i    ) * integral_size_omega + (j + 1);
	  index_integral_1_1 = (i + 1) * integral_size_omega + (j + 1);
	  energy =
	      integral[index_integral_0_0]
	    + integral[index_integral_1_0]
	    + integral[index_integral_0_1]
	    + integral[index_integral_1_1];

	  node_array.push_back(new class Quadtree_Node(energy,
						       theta,
						       omega,
						       coefficient,
						       NULL,
						       NULL,
						       NULL,
						       NULL));
	}
    }
}


/*
  adds and fills the last array of “node_arrays” using the previous one,
  until…
*/
void
Quadtree::
fill_node_array(std::vector<std::vector<Quadtree_Node *> > & node_arrays,
		size_t size_theta,
		size_t size_omega,
		double coefficient)
{
  /*  index_target_theta_omega  */
  size_t index_target_0_0, index_target_1_0;
  size_t index_target_0_1, index_target_1_1;

  double energy, theta, omega;

  while (size_theta >= 2)  /*  size_omega >= 4  */
    {
      size_theta /= 2;
      size_omega /= 2;

      //  segfault
      //class vector<class Quadtree_Node> & next_to_last = node_arrays.back();
      //node_arrays.push_back(vector<class Quadtree_Node>());
      //class vector<class Quadtree_Node> & last = node_arrays.back();

      node_arrays.push_back(vector<class Quadtree_Node *>());
      class vector<class Quadtree_Node *> & last = node_arrays.back();
      class vector<class Quadtree_Node *> & next_to_last =
	node_arrays[node_arrays.size() - 2];

      for (size_t i = 0; i < size_theta; i++)
	{
	  for (size_t j = 0; j < size_omega; j++)
	    {
	      index_target_0_0 = (i * 2    ) * (size_omega * 2) + (j * 2);
	      index_target_1_0 = (i * 2 + 1) * (size_omega * 2) + (j * 2);
	      index_target_0_1 = (i * 2    ) * (size_omega * 2) + (j * 2 + 1);
	      index_target_1_1 = (i * 2 + 1) * (size_omega * 2) + (j * 2 + 1);

	      energy =
	          next_to_last[index_target_0_0]->get_energy()
		+ next_to_last[index_target_1_0]->get_energy()
		+ next_to_last[index_target_0_1]->get_energy()
		+ next_to_last[index_target_1_1]->get_energy();
	      theta =
		(  next_to_last[index_target_0_0]->get_theta()
		 + next_to_last[index_target_1_0]->get_theta())
		/ 2.0;
	      omega = 
		(  next_to_last[index_target_0_0]->get_omega()
		 + next_to_last[index_target_0_1]->get_omega())
		/ 2.0;

	      last
		.push_back(new Quadtree_Node(energy,
					     theta,
					     omega,
					     coefficient,
					     next_to_last[index_target_0_0],
					     next_to_last[index_target_1_0],
					     next_to_last[index_target_0_1],
					     next_to_last[index_target_1_1]));
	    }
	}
    }
}


/*  adds the last array of “node_arrays” using the previous one  */
void
Quadtree::add_root(std::vector<std::vector<Quadtree_Node *> > & node_arrays,
		   double coefficient)
{
  class Quadtree_Node * child_1, * child_2;
  double energy, theta, omega;

  child_1 = node_arrays.back()[0];
  child_2 = node_arrays.back()[1];
  energy =
    (  node_arrays.back()[0]->get_energy()
     + node_arrays.back()[1]->get_energy())
    * 2.0;  /*  we was working on [0, π], not [−π, π]  */
  theta = 0.0;
  omega =
    (node_arrays.back()[0]->get_omega() + node_arrays.back()[1]->get_omega())
    / 2.0;

  node_arrays
    .push_back(vector<class Quadtree_Node *>(1, new Quadtree_Node(energy,
								  theta,
								  omega,
								  coefficient,
								  child_1,
								  child_2,
								  NULL,
								  NULL)));
}
