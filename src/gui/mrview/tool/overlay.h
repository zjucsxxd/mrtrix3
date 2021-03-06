/*
   Copyright 2009 Brain Research Institute, Melbourne, Australia

   Written by J-Donald Tournier, 13/11/09.

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

#ifndef __gui_mrview_tool_overlay_h__
#define __gui_mrview_tool_overlay_h__

#include "gui/mrview/mode/base.h"
#include "gui/mrview/tool/base.h"
#include "gui/mrview/adjust_button.h"
#include "gui/mrview/colourmap_button.h"

namespace MR
{
  namespace GUI
  {
    namespace MRView
    {
      namespace Tool
      {

        class Overlay : public Base, public ColourMapButtonObserver, public DisplayableVisitor
        {
            Q_OBJECT

          public:

            Overlay (Window& main_window, Dock* parent);

            void draw (const Projection& projection, bool is_3D, int axis, int slice);
            void drawOverlays (const Projection& transform) override;
            bool process_batch_command (const std::string& cmd, const std::string& args);

            void selected_colourmap(size_t index, const ColourMapButton&) override;
            void selected_custom_colour(const QColor& colour, const ColourMapButton&) override;
            void toggle_show_colour_bar(bool visible, const ColourMapButton&) override;
            void toggle_invert_colourmap(bool, const ColourMapButton&) override;
            void reset_colourmap(const ColourMapButton&) override;

            void render_image_colourbar(const Image& image, const Projection& transform) override;

          private slots:
            void image_open_slot ();
            void image_close_slot ();
            void hide_all_slot ();
            void toggle_shown_slot (const QModelIndex&, const QModelIndex&);
            void selection_changed_slot (const QItemSelection &, const QItemSelection &);
            void update_slot (int unused);
            void values_changed ();
            void upper_threshold_changed (int unused);
            void lower_threshold_changed (int unused);
            void upper_threshold_value_changed ();
            void lower_threshold_value_changed ();
            void opacity_changed (int unused);
            void interpolate_changed ();

          protected:
             class Item;
             class Model;
             class InterpolateCheckBox : public QCheckBox
             {
               public:
                 InterpolateCheckBox(const QString& text, QWidget *parent = nullptr)
                   : QCheckBox(text, parent) {}
               protected:
                 // We don't want a click to cycle to a partially checked state
                 // So explicitly specify the allowed clickable states
                 void nextCheckState () override { checkState() == Qt::Unchecked ?
                         setCheckState(Qt::Checked) : setCheckState(Qt::Unchecked);
                 }
             };

             QPushButton* hide_all_button;
             Model* image_list_model;
             QListView* image_list_view;
             ColourMapButton* colourmap_button;
             AdjustButton *min_value, *max_value, *lower_threshold, *upper_threshold;
             QCheckBox *lower_threshold_check_box, *upper_threshold_check_box;
             InterpolateCheckBox* interpolate_check_box;
             QSlider *opacity_slider;

             void update_selection ();
             void updateGL() { 
               window.get_current_mode()->update_overlays = true;
               window.updateGL();
             }
             
             void add_images (VecPtr<MR::Image::Header>& list);

          private:
             ColourMap::Renderer colourbar_renderer;
        };

      }
    }
  }
}

#endif




