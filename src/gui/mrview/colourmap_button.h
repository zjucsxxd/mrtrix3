#ifndef __gui_mrview_colourmap_button_h__
#define __gui_mrview_colourmap_button_h__

#include "mrtrix.h"
#include "gui/opengl/gl.h"
#include "gui/mrview/colourmap.h"

namespace MR
{
namespace GUI
{
namespace MRView
{

class ColourMapButton;
class ColourMapButtonObserver
{
public:
    virtual void selected_colourmap(size_t, const ColourMapButton&) {}
    virtual void selected_custom_colour(const QColor&, const ColourMapButton&) {}
    virtual void toggle_show_colour_bar(bool, const ColourMapButton&) {}
    virtual void toggle_invert_colourmap(bool, const ColourMapButton&) {}
    virtual void reset_colourmap(const ColourMapButton&) {}
};


class ColourMapButton : public QToolButton
{
    Q_OBJECT
public:
    ColourMapButton(QWidget* parent, ColourMapButtonObserver& obs, bool use_special_colourmaps = true);
    void set_colourmap_index(size_t index);
    std::vector<QAction*> colourmap_actions;
private:
    void init_menu(bool create_shortcuts, bool use_special);
    void init_core_menu_items(bool create_shortcuts);
    void init_custom_colour_menu_items();
    void init_special_colour_menu_items(bool create_shortcuts);

    static const std::vector<ColourMap::Entry> core_colourmaps_entries;
    static const std::vector<ColourMap::Entry> special_colourmaps_entries;

    ColourMapButtonObserver& observer;
    QActionGroup *core_colourmaps_actions;
    ColourMap::Renderer colourbar_renderer;

    QMenu* colourmap_menu;
    QAction* custom_colour_action;


private slots:
    void select_colourmap_slot(QAction* action);
    void select_colour_slot();
    void select_random_colour_slot();
    void show_colour_bar_slot(bool visible);
    void invert_colourmap_slot(bool inverted);
    void reset_intensity_slot();
};

}
}
}

#endif // __gui_mrview_colourmap_button_h__
