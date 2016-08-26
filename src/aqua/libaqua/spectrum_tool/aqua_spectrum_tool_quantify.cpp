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


/****************  includes  ****************/


#include "aqua_spectrum_tool_quantify.h"

#include "aqua_spectrum_tool_quadtree.h"
#include "aqua_spectrum_tool_quadtree_node.h"
#include "aqua_spectrum_tool_wave_polar.h"

#include "spectrum/aqua_spectrum_directional.h"

/*  C++ lib  */
#include <algorithm>  /*  sort(), max_element()  */
#include <string>
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum_tool;
using std::vector;


/****************  static function prototypes  ****************/


namespace
{
  /*  waves are sorted by amplitude  */
  void fill_waves(class vector<class vector<class Wave_Polar> > & waves,
		  class Quadtree_Node * root_node,
		  const size_t number_of_waves);
  /*  waves are sorted by amplitude  */
  void fill_waves(class vector<class Wave_Polar > & waves,
		  class Quadtree_Node * root_node,
		  const size_t number_of_waves);
  /*  waves are unsorted  */
  void fill_waves(class vector<class Wave_Polar > & waves,
		  class std::vector<size_t> & wave_indices,
		  class Quadtree_Node * root_node,
		  const size_t number_of_waves);
  class Quadtree_Node * get_child(class Quadtree_Node * node,
				  size_t index) throw(std::string);
}


/****************  functions  ****************/


/*  waves are sorted by amplitude  */
void
aqua::spectrum_tool::
quantify(class std::vector<class std::vector<class Wave_Polar> > & waves,
	 const class spectrum::Directional & spectrum,
	 size_t number_of_waves)
{
  class Quadtree quadtree(spectrum);

  fill_waves(waves, quadtree.get_root(), number_of_waves);
}


/*  waves are sorted by amplitude  */
void
aqua::spectrum_tool::quantify(class std::vector<class Wave_Polar> & waves,
			      const class spectrum::Directional & spectrum,
			      size_t number_of_waves)
{
  class Quadtree quadtree(spectrum);

  fill_waves(waves, quadtree.get_root(), number_of_waves);
}


/*  waves are unsorted  */
void
aqua::spectrum_tool::quantify(class std::vector<class Wave_Polar> & waves,
			      class std::vector<size_t> & wave_indices,
			      const class spectrum::Directional & spectrum,
			      size_t number_of_waves)
{
  class Quadtree quadtree(spectrum);

  fill_waves(waves, wave_indices, quadtree.get_root(), number_of_waves);
}


/****************  static functions  ****************/


namespace
{


/*  waves are sorted by amplitude  */
void
fill_waves(class vector<class vector<class Wave_Polar> > & waves,
	   class Quadtree_Node * root_node,
	   const size_t number_of_waves)
{
  class vector<class Quadtree_Node *> nodes;

  class Quadtree_Node * temp;


  /****  one wave  ****/
  waves.push_back(vector<class Wave_Polar>(1, root_node->get_wave()));

  /****  four waves  ****/
  waves.push_back(vector<class Wave_Polar>());

  nodes.push_back(get_child(root_node, 0));
  waves.back().push_back(nodes.back()->get_wave());
  waves.back().push_back(nodes.back()->get_wave_opposite());

  nodes.push_back(get_child(root_node, 1));
  waves.back().push_back(nodes.back()->get_wave());
  waves.back().push_back(nodes.back()->get_wave_opposite());


  std::sort(waves.back().begin(), waves.back().end());

  /****  next  ****/
  while(waves.back().size() < number_of_waves)
    {
      waves.push_back(vector<class Wave_Polar>(waves.back().begin(),
						waves.back().end() - 2));

      std::sort(nodes.begin(), nodes.end(), Quadtree_Node_Less_Than());
      temp = nodes.back();
      nodes.pop_back();

      for (size_t i = 0; i < 4; i++)
	{
	  nodes.push_back(get_child(temp, i));

	  waves.back().push_back(nodes.back()->get_wave());
	  waves.back().push_back(nodes.back()->get_wave_opposite());
	}

      std::sort(waves.back().begin(), waves.back().end());
    }
}


/*  waves are sorted by amplitude  */
void
fill_waves(class vector<class Wave_Polar > & waves,
	   class Quadtree_Node * root_node,
	   const size_t number_of_waves)
{
  class vector<class Quadtree_Node *> nodes;

  class Quadtree_Node * temp;


  nodes.push_back(get_child(root_node, 0));
  nodes.push_back(get_child(root_node, 1));


  while(nodes.size() * 2 < number_of_waves)
    {
      std::sort(nodes.begin(), nodes.end(), Quadtree_Node_Less_Than());
      temp = nodes.back();
      nodes.pop_back();

      for (size_t i = 0; i < 4; i++)
	{
	  nodes.push_back(get_child(temp, i));
	}
    }


  for (size_t i = 0; i < nodes.size(); i++)
    {
      waves.push_back(nodes[i]->get_wave());
      waves.push_back(nodes[i]->get_wave_opposite());
    }

  std::sort(waves.begin(), waves.end());
}


/*  waves are unsorted  */
void
fill_waves(class vector<class Wave_Polar > & waves,
	   class std::vector<size_t> & wave_indices,
	   class Quadtree_Node * root_node,
	   const size_t number_of_waves)
{
  class vector<class Quadtree_Node *> nodes;

  vector<class Quadtree_Node *>::iterator nodes_iterator;

  class Quadtree_Node * temp_node;
  size_t temp_index;


  /****  one wave  ****/
  waves.push_back(root_node->get_wave());
  wave_indices.push_back(0);

  /****  four waves  ****/
  nodes.push_back(get_child(root_node, 0));
  waves.push_back(nodes.back()->get_wave());
  waves.push_back(nodes.back()->get_wave_opposite());

  nodes.push_back(get_child(root_node, 1));
  waves.push_back(nodes.back()->get_wave());
  waves.push_back(nodes.back()->get_wave_opposite());

  /****  next  ****/
  while(nodes.size() * 2 < number_of_waves)
    {
      nodes_iterator = std::max_element(nodes.begin(),
					nodes.end(),
					Quadtree_Node_Less_Than());

      temp_index = (nodes_iterator - nodes.begin()) * 2;
      wave_indices.push_back(temp_index);
      temp_node = *nodes_iterator;
      nodes.erase(nodes_iterator);

      for (size_t i = 0; i < 4; i++)
	{
	  nodes.push_back(get_child(temp_node, i));
	  waves.push_back(nodes.back()->get_wave());
	  waves.push_back(nodes.back()->get_wave_opposite());
	}
    }
}


class Quadtree_Node *
get_child(class Quadtree_Node * node, size_t index) throw(std::string)
{
  class Quadtree_Node * child;

  child = node->get_child(index);

  if (child == NULL)
    {
      throw std::string("NULL Quadtree_Node pointer");
    }

  return child;
}


}
