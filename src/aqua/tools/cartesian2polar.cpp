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


/*
  Génère deux fichiers « plot » :

  - « file » contient les coordonnées d'une grille régulière (FFT) en
    coordonnées polaires (x),
  - « file_2 » les points « encadrant » les premiers (*).

Exemple avec une grille 3 × 3 :

  *---*---*---*
  |   |   |   |
  | x-|-x-|-x |
  | | | | | | |
  *---*---*---*
  | | | | | | |
  | x-|-x-|-x |
  | | | | | | |
  *---*---*---*
  | | | | | | |
  | x-|-x-|-x |
  |   |   |   |
  *---*---*---*

  Usage : argv[0] <nb. points> <dimension>.

  plot [-3.5:3.5][0:N] "out.plot"
  replot "out_2.plot"

  La grille n'est pas de taille « size × size » mais  « size − 1 × size − 1 »
  afin que le résultat soit symétrique.
 */



#include <fstream>
#include <iomanip>
#include <iostream>

#include <cstddef>
#include <cstdlib>
#include <cmath>

#include "constant.h"
#include "vector.h"


using namespace std;



const char * file = "out.plot";
const char * file_2 = "out_2.plot";


namespace
{
  void compute(float & magnitude,
	       float & angle,
	       float vector_x,
	       float vector_y);
}


int
main(int argc, char ** argv)
{
  if (argc != 3)
    {
      cerr << "Usage :" << argv[0] << " <nb. points> <dimension>." << endl;
      exit(EXIT_FAILURE);
    }

  ofstream of(file);
  ofstream of2(file_2);
  float size, length;
  float dk, kx, ky, omega, theta;


  size = atof(argv[1]);
  length = atof(argv[2]);
  dk = Constant_2_pi / length;


  //  « i = 1 » et « j = 1 » pour que le résultat soit symétrique
  for (int i = 1; i < size; i++)
    {
      kx = (i - size / 2.0) * dk;

      for (int j = 1; j < size; j++)
	{
	  ky = (j - size / 2.0) * dk;

	  compute(omega, theta, kx, ky);
	  of << theta << " " << omega << endl;

	  compute(omega, theta, kx - dk / 2, ky - dk / 2);
	  of2 << theta << " " << omega << endl;

	  compute(omega, theta, kx + dk / 2, ky - dk / 2);
	  of2 << theta << " " << omega << endl;

	  compute(omega, theta, kx - dk / 2, ky + dk / 2);
	  of2 << theta << " " << omega << endl;

	  compute(omega, theta, kx + dk / 2, ky + dk / 2);
	  of2 << theta << " " << omega << endl;
	}
    }
}


namespace
{


void
compute(float & magnitude, float & angle, float vector_x, float vector_y)
{
  magnitude = hypotf(vector_x, vector_y);
  angle = atan2f(vector_y, vector_x);      /*  atan2(Y, X)  */

  magnitude = sqrtf(Constant_g * magnitude);
}


}
