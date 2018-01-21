#include "stdafx.h"
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
#include "buffer_management.h"
//#include "BlauImage.h"

char * aligned_and_managed_new16b(size_t size)
{
  return aligned_and_managed_new(size, 16);
}

char * aligned_and_managed_new(size_t size, size_t align_size)
{
  char * ptr, * ptr2, * aligned_ptr;
  int align_mask = int(align_size - 1);

  ptr = new char [size + align_size + sizeof(char*) + sizeof(int)];
  if (ptr == 0) return 0;

  ptr2 = ptr + sizeof(char*) + sizeof(int);
  aligned_ptr = ptr2 + (align_size - ((size_t)ptr2 & align_mask));

  ((char**)aligned_ptr)[-1] = ptr;

  return aligned_ptr;
}

void aligned_and_managed_delete(void * aligned_ptr)
{
  if (aligned_ptr != 0) {
    if (MANAGED_MEMORY_DEBUGGING)
      cout << endl
	   << "[MANAGED_MEMORY_DEBUGGING: freeing    "
	   << *(int*)(((char*)aligned_ptr) - sizeof(void*) - sizeof(int)) << " items at "
	   << "\t" << aligned_ptr << "]" << endl;
    delete [] ((char**)aligned_ptr)[-1];
    aligned_ptr = 0;
  }
}

bool manage_image(ByteImage ** image, int width, int height, int nChannels)
{
  bool just_created = false;

  if (*image == 0) {
	  *image = B_CreateImage(B_size(width, height), nChannels);
    just_created = true;
  }

  bool different = (width != (*image)->GetWidth()   ||
		    height != (*image)->GetHeight() ||
		    nChannels != (*image)->GetChannel());

  if (just_created || different) {
    int image_size, width_step;
   
	width_step = ((width*nChannels*sizeof(BYTE) + 3)&~3) / sizeof(BYTE);
    if (width_step % 16 != 0) width_step = 16 * (width_step / 16 + 1);
    image_size = width_step * height * nChannels;
    manage_buffer((*image)->pImageData, image_size, BYTE);

    (*image)->nWStep = width_step;
    (*image)->Width     = width;
    (*image)->Height    = height;
    (*image)->nChannels = nChannels;

    return true;
  } else
    return false;
}

bool load_managed_image(char * filename, ByteImage ** image, int code)
{
	ByteImage * tmp_image=NULL;
	tmp_image->LoadImage(filename);

  if (tmp_image == 0) return false;

  manage_image(image, tmp_image->GetWidth(), tmp_image->GetHeight(), tmp_image->GetChannel());
  tmp_image = *image;
  B_ReleaseImage(&tmp_image);

  return true;
}

void free_managed_image(ByteImage ** image)
{
  delete_managed_buffer((*image)->GetPtr());
  B_ReleaseImage(image);
}

void release_managed_image(ByteImage ** image)
{
  free_managed_image(image);
}


void save_image_in_pakfile(ofstream & f, ByteImage * image)
{
  f << image->GetWidth() << " " << image->GetHeight() << " " << " " << image->GetChannel()
    << " " << image->GetWStep() << " " << endl;

  int image_size = image->GetWStep() *image->GetHeight()* image->GetChannel();
  save_buffer_in_pakfile(f, image->pImageData, image_size, BYTE);
}

void save_managed_image_in_pakfile(ofstream & f, ByteImage * image)
{
  save_image_in_pakfile(f, image);
}

bool load_managed_image_in_pakfile(ifstream & f, ByteImage ** image)
{
  int width, height, nChannels, width_step;
  f >> width >> height >> nChannels
    >> width_step;

  //   cout << "> [buffer_management::load_managed_image_in_pakfile] Loading compressed image "
  //        << " size = " << width << "x" << height
  //        << ", depth = " << depth << ", nChannels = " << nChannels
  //        << "." << endl;

  bool result = manage_image(image, width, height, nChannels);
  int image_size = width_step * height * nChannels;
  load_managed_buffer_in_pakfile(f, (*image)->pImageData, image_size, BYTE);

  (*image)->nWStep = width_step;

  return result;
}
