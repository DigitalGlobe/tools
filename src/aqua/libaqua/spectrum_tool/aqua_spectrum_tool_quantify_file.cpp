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


#include "aqua_spectrum_tool_quantify_file.h"

#include "aqua_spectrum_tool_quantify.h"
#include "aqua_spectrum_tool_wave_polar.h"

/*  C++ lib  */
#include <fstream>
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum_tool;
using namespace std;


/****************  functions  ****************/


void
aqua::spectrum_tool::
quantify_file_write(const char * file_name,
		    const class spectrum::Directional & spectrum,
		    const size_t number_of_waves)
{
  class vector<class Wave_Polar> waves;
  class vector<size_t> indices;
  ofstream ofstream(file_name);

  size_t i;


  if (!ofstream)
    {
      throw(ios::failure("can not open file"));
    }


  quantify(waves, indices, spectrum, number_of_waves);

  /*  writes indices  */
  for (i = 0; i < indices.size() - 1; i++)
    {
      ofstream << indices[i] << " ";
    }
  ofstream << indices[i];
  ofstream << endl;

  /*  writes waves  */
  for (i = 0; i < waves.size() - 1; i++)
    {
      ofstream << waves[i] <<" ";
    }
  ofstream << waves[i];
  ofstream << endl;
}


void
aqua::spectrum_tool::
quantify_file_read(class std::vector<size_t> & indices,
		   class std::vector<class Wave_Polar> & waves,
		   const char * file_name)
{
  class Wave_Polar temp_wave(0.0, 0.0, 0.0);
  ifstream ifstream(file_name);

  size_t temp;


  if (!ifstream)
    {
      throw(ios::failure("can not open file"));
    }

  //  tester si le fichier est bien formé

  /*  reads indices  */
  while (ifstream.peek() != '\n')
    {
      ifstream >> temp;
      indices.push_back(temp);
    }

  ifstream.ignore();  /*  \n  */

  /*  reads waves  */
  while (ifstream.peek() != '\n')
    {
      ifstream >> temp_wave;
      waves.push_back(temp_wave);
    }
}


void
aqua::spectrum_tool::
quantify_file_read(class std::vector<class Wave_Polar> & waves,
		   const size_t size_max,
		   const std::vector<size_t> & indices,
		   const std::vector<class Wave_Polar> & read_waves,
		   const double theta_peak)
{
  vector<class Wave_Polar>::iterator iterator;

  size_t size_current;


  //  algo de création est dans quantify()

  waves = read_waves;

  /*  fills “waves”  */
  waves.erase(waves.begin());

  /*  indices.[0] == 0  */
  size_current = 4;
  for (size_t i = 1; size_current < size_max; i++)
    {
      if (i >= indices.size())
	{
	  throw "too few waves";
	}

      size_current += 6;

      iterator = waves.begin() + indices[i];
      waves.erase(iterator);
      waves.erase(iterator);
    }
  waves.erase(waves.begin() + size_current, waves.end());

  /*  sets wave angles according to “theta_peak”  */
  // on stoque en polaire → pas besoin d'avoir les vagues en double dans
  // fichier
  for (size_t i = 0; i < waves.size(); i ++)
    {
      waves[i].theta += theta_peak;
    }
}


// void
// aqua::spectrum::
// quantify_file_read(std::vector<std::vector<class Wave_Polar> > & waves,
// 		   const char * file_name)
// {
//   class vector<size_t> indices;
//   class vector<class Wave_Polar> temp_waves;
//   vector<class Wave_Polar>::iterator iterator;

//   size_t temp;


//   quantify_file_read(indices, temp_waves, file_name);


//   //  algo de création est dans quantify()

//   /*  fills “waves”  */
//   waves.push_back(vector<class Wave_Polar>(1, temp_waves[0]));
//   temp_waves.erase(temp_waves.begin());
//   waves.push_back(vector<class Wave_Polar>(temp_waves.begin(),
// 					   temp_waves.begin() + 4));
//   /*  indices.[0] == 0  */
//   for (size_t i = 1; i < indices.size(); i++)
//     {
//       temp = 4 + i * 6;

//       iterator = temp_waves.begin() + indices[i];
//       temp_waves.erase(iterator);
//       temp_waves.erase(iterator);

//       waves
// 	.push_back(vector<class Wave_Polar>(temp_waves.begin(),
// 					    temp_waves.begin() + temp));
//     }

// }
