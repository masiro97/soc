/*
  Copyright 2007 Computer Vision Lab,
  Ecole Polytechnique Federale de Lausanne (EPFL), Switzerland.
  All rights reserved.

  Author: Vincent Lepetit (http://cvlab.epfl.ch/~lepetit)

  This file is part of the ferns_demo software.

  ferns_demo is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2 of the License, or (at your option) any later
  version.

  ferns_demo is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
  PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  ferns_demo; if not, write to the Free Software Foundation, Inc., 51 Franklin
  Street, Fifth Floor, Boston, MA 02110-1301, USA
*/
#ifndef template_matching_based_tracker_h
#define template_matching_based_tracker_h

//#include <cv.h>

#include "homography06.h"
#include "homography_estimator.h"
#include "BlauImage.h"
class template_matching_based_tracker
{
 public:
  template_matching_based_tracker(void);

  bool load(const char * filename);
  void save(const char * filename);

 // void learn(ByteImage * image,
//	     int number_of_levels, int max_motion, int nx, int ny,
//	     int xUL, int yUL,
//	     int xBR, int yBR,
//	     int bx, int by,
//	     int Ns);

  void initialize(void);
  void initialize(int u0, int v0,
		  int u1, int v1,
		  int u2, int v2,
		  int u3, int v3);

  bool track(ByteImage * input_frame);

  homography06 f;

  //private:
  void find_2d_points(ByteImage * image, int bx, int by);
  //void compute_As_matrices(ByteImage * image, int max_motion, int Ns);
  void move(int x, int y, float & x2, float & y2, int amp);
  bool normalize(B_Mat * V)
  {
	  float sum = 0.0, sum2 = 0.0;
	  float * v = V->data.fl;

	  for (int i = 0; i < V->rows; i++) {
		  sum += v[i];
		  sum2 += v[i] * v[i];
	  }

	  // Not enough contrast,  better not put this sample into the training set:
	  if (sum < (V->rows * 10))
		  return false;

	  float mean = sum / V->rows;
	  float inv_sigma = 1.0 / sqrt(sum2 / V->rows - mean * mean);

	  // Not enough contrast,  better not put this sample into the training set:
	  //  if (!finite(inv_sigma))
	  if (!_finite(inv_sigma))
		  return false;

	  for (int i = 0; i < V->rows; i++)
		  v[i] = inv_sigma * (v[i] - mean);

	  return true;
  }
  //bool normalize(B_Mat * V);
  void add_noise(B_Mat * V);
  ByteImage * compute_gradient(ByteImage * image);
  void get_local_maximum(ByteImage * G,
			 int xc, int yc, int w, int h,
			 int & xm, int & ym);

  homography_estimator he;

  int * m;
  B_Mat ** As;
  B_Mat * U0, * U, * I0, * DU, * DI, * I1;
  float * u0, * u, * i0, * du, * i1;
  int number_of_levels, nx, ny;
};


#endif
