/*
    Copyright 2008 Brain Research Institute, Melbourne, Australia

    Written by J-Donald Tournier, 27/06/08, David Raffelt, 08/06/2012

    This file is part of MRtrix.

    MRtrix is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MRtrix is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MRtrix.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef __image_filter_erode_h__
#define __image_filter_erode_h__

#include "progressbar.h"
#include "ptr.h"
#include "image/buffer_scratch.h"
#include "image/copy.h"
#include "image/loop.h"
#include "image/filter/base.h"

namespace MR
{
  namespace Image
  {
    namespace Filter
    {

      /** \addtogroup Filters
        @{ */

      //! a filter to erode a mask
      /*!
       * Typical usage:
       * \code
       * Buffer<bool> input_data (argument[0]);
       * auto input_voxel = input_data.voxel();
       *
       * Filter::Erode erode (input_data);
       * Header header (input_data);
       * header.info() = erode.info();
       *
       * Buffer<bool> output_data (header, argument[1]);
       * auto output_voxel = output_data.voxel();
       * erode (input_voxel, output_voxel);
       *
       * \endcode
       */
      class Erode : public Base
      {

        public:
          template <class InfoType>
          Erode (const InfoType& in) :
              Base (in),
              npass_ (1)
          {
            datatype_ = DataType::Bit;
          }


          template <class InputVoxelType, class OutputVoxelType>
          void operator() (InputVoxelType& input, OutputVoxelType& output) {

            RefPtr <BufferScratch<bool> > in_data (new BufferScratch<bool> (input));
            RefPtr <BufferScratch<bool>::voxel_type> in (new BufferScratch<bool>::voxel_type (*in_data));
            Image::copy (input, *in);

            RefPtr <BufferScratch<bool> > out_data;
            RefPtr <BufferScratch<bool>::voxel_type> out;

            Ptr<ProgressBar> progress;
            if (message.size())
              progress = new ProgressBar (message, npass_ + 1);

            for (unsigned int pass = 0; pass < npass_; pass++) {
              out_data = new BufferScratch<bool> (input);
              out = new BufferScratch<bool>::voxel_type (*out_data);
              for (auto l = LoopInOrder(*in) (*in, *out); l; ++l) 
               out->value() = erode (*in);
              
              if (pass < npass_ - 1) {
                in_data = out_data;
                in = out;
              }
              if (progress)
                ++(*progress);
            }
            Image::copy(*out, output);
          }


          void set_npass (unsigned int npass) {
            npass_ = npass;
          }


        protected:

          bool erode (BufferScratch<bool>::voxel_type& in)
          {
            if (!in.value()) return false;
            if (   (in[0] == 0) || (in[0] == in.dim(0)-1)
                || (in[1] == 0) || (in[1] == in.dim(1)-1)
                || (in[2] == 0) || (in[2] == in.dim(2)-1))
              return false;
            bool val;
            if (in[0] > 0) { in[0]--; val = in.value(); in[0]++; if (!val) return false; }
            if (in[1] > 0) { in[1]--; val = in.value(); in[1]++; if (!val) return false; }
            if (in[2] > 0) { in[2]--; val = in.value(); in[2]++; if (!val) return false; }
            if (in[0] < in.dim(0)-1) { in[0]++; val = in.value(); in[0]--; if (!val) return false; }
            if (in[1] < in.dim(1)-1) { in[1]++; val = in.value(); in[1]--; if (!val) return false; }
            if (in[2] < in.dim(2)-1) { in[2]++; val = in.value(); in[2]--; if (!val) return false; }
            return true;
          }

          unsigned int npass_;
      };
      //! @}
    }
  }
}




#endif
