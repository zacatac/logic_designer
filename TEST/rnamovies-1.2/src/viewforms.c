/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "viewforms.h"

static FL_PUP_ENTRY fdmenu_file_0[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "Open",	0,	"",	 FL_PUP_NONE},
    { "Save",	0,	"",	 FL_PUP_NONE},
    { "Print %l",	0,	"",	 FL_PUP_NONE},
    { "Quit",	0,	"",	 FL_PUP_NONE},
    {0}
};

static FL_PUP_ENTRY fdmenu_options_1[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "Interpolations",	0,	"",	 FL_PUP_NONE},
    { "Layout",	0,	"",	 FL_PUP_NONE},
    { "Colors",	0,	"",	 FL_PUP_NONE},
    { "Font",	0,	"",	 FL_PUP_NONE},
    {0}
};

#include "quit.xpm"
static FL_PUP_ENTRY fdmenu_help_2[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "About",	0,	"",	 FL_PUP_NONE},
    { "Instructions",	0,	"",	 FL_PUP_NONE},
    { "Log Browser",	0,	"",	 FL_PUP_NONE},
    {0}
};

#include "reload.xpm"
#include "button1.xpm"
#include "pause.xpm"
#include "play.xpm"
#include "forward.xpm"
#include "backward.xpm"
#include "rewind.xpm"
#include "squiggle.xpm"
FD_main *create_form_main(void)
{
  FL_OBJECT *obj;
  FD_main *fdui = (FD_main *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->main = fl_bgn_form(FL_NO_BOX, 622, 651);
  obj = fl_add_box(FL_FLAT_BOX,0,0,622,651,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,124,614,58,31,"Rate");
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,246,614,371,31,"Comment");
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthEast);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,66,614,55,31,"Frame");
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,5,614,58,31,"Structure");
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
  fdui->timeout = obj = fl_add_timer(FL_HIDDEN_TIMER,560,10,30,20,"Timer");
    fl_set_object_callback(obj,text_timeout_cb,0);
  fdui->file = obj = fl_add_menu(FL_PULLDOWN_MENU,6,3,30,20,"File");
    fl_set_object_shortcut(obj,"#F",1);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,file_mnu_cb,0);
    fl_set_menu_entries(obj, fdmenu_file_0);
  obj = fl_add_text(FL_NORMAL_TEXT,128,0,121,24,"RNA Movies");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE+FL_SHADOW_STYLE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
  fdui->options = obj = fl_add_menu(FL_PULLDOWN_MENU,46,3,41,20,"Options");
    fl_set_object_shortcut(obj,"#O",1);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,options_mnu_cb,0);
    fl_set_menu_entries(obj, fdmenu_options_1);
  fdui->button5 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,581,26,36,36,"");
    fl_set_button_shortcut(obj,"^Q",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
    fl_set_object_callback(obj,but_cb,5);
    fl_set_object_helper(obj, "Quit (Ctrl Q)\nquit RNA Movies");
    fl_set_pixmapbutton_data(obj,quit_xpm);
  fdui->glcanvas = obj = fl_add_glcanvas(FL_NORMAL_CANVAS,6,94,610,510,"");
    fl_set_object_color(obj,FL_COL1,FL_COL1);
    fl_set_object_gravity(obj, FL_NorthWest, FL_SouthEast);
  fdui->help = obj = fl_add_menu(FL_PULLDOWN_MENU,94,3,30,20,"Help");
    fl_set_object_shortcut(obj,"#h",1);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,help_mnu_cb,0);
    fl_set_menu_entries(obj, fdmenu_help_2);
  fdui->movie_slider = obj = fl_add_slider(FL_HOR_BROWSER_SLIDER,5,67,612,20,"");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthEast);
    fl_set_object_callback(obj,movie_slider_cb,0);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_bounds(obj, 0, 0);
    fl_set_slider_size(obj, 0.10);

  fdui->file_grp = fl_bgn_group();
  fdui->button2 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,45,26,36,36,"");
    fl_set_button_shortcut(obj,"^L",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,2);
    fl_set_object_helper(obj, "Reload (Ctrl L)\nreload the active RNA Movies file");
    fl_set_pixmapbutton_data(obj,reload_xpm);
  fdui->button1 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,6,26,36,36,"");
    fl_set_button_shortcut(obj,"^O",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,1);
    fl_set_object_helper(obj, "Open (Ctrl O)\nopen a RNA Movies file");
    fl_set_pixmapbutton_data(obj,button1_xpm);
  fl_end_group();


  fdui->player_grp = fl_bgn_group();
  fdui->button60 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,90,26,36,36,"");
    fl_set_button_shortcut(obj,"^A",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,60);
    fl_set_object_helper(obj, "Pause (Ctrl A)\n pause the movie");
    fl_set_pixmapbutton_data(obj,pause_xpm);
  fdui->button61 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,130,26,36,36,"");
    fl_set_button_shortcut(obj,"^P",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,61);
    fl_set_object_helper(obj, "Play (Ctrl P)\nplay the movie");
    fl_set_pixmapbutton_data(obj,play_xpm);
  fdui->button62 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,170,26,36,36,"");
    fl_set_button_shortcut(obj,"^F",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,62);
    fl_set_object_helper(obj, "Forward (Ctrl F)\nmove forward to the next structure");
    fl_set_pixmapbutton_data(obj,forward_xpm);
  fdui->button63 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,211,26,36,36,"");
    fl_set_button_shortcut(obj,"^B",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,63);
    fl_set_object_helper(obj, "Backward (Ctrl B)\nmove backward to the previous structure");
    fl_set_pixmapbutton_data(obj,backward_xpm);
  fdui->button64 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,251,26,36,36,"");
    fl_set_button_shortcut(obj,"^R",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,64);
    fl_set_object_helper(obj, "Rewind (Ctrl R)\nrewind to start position");
    fl_set_pixmapbutton_data(obj,rewind_xpm);
  fl_end_group();

  fdui->button9 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,297,26,36,36,"");
    fl_set_button_shortcut(obj,"^S",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,9);
    fl_set_object_helper(obj, "Snapshot (Ctrl S)\nsave the current frame to file");
    fl_set_pixmapbutton_data(obj,squiggle_xpm);
  fdui->struccount = obj = fl_add_button(FL_NORMAL_BUTTON,8,620,52,21,"");
    fl_set_object_boxtype(obj,FL_FLAT_BOX);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
    fl_set_object_callback(obj,but_cb,101);
  fdui->framecount = obj = fl_add_button(FL_NORMAL_BUTTON,69,620,50,21,"");
    fl_set_object_boxtype(obj,FL_FLAT_BOX);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
    fl_set_object_callback(obj,but_cb,102);
  fdui->comment = obj = fl_add_text(FL_NORMAL_TEXT,250,620,358,20,"");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthEast);
  fdui->framespeed = obj = fl_add_button(FL_NORMAL_BUTTON,128,620,50,21,"");
    fl_set_object_boxtype(obj,FL_FLAT_BOX);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
    fl_set_object_callback(obj,but_cb,103);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,185,614,58,31,"Interpolation");
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
  fdui->interpolations = obj = fl_add_button(FL_NORMAL_BUTTON,188,620,53,21,"");
    fl_set_object_boxtype(obj,FL_FLAT_BOX);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
    fl_set_object_callback(obj,but_cb,104);
  fl_end_form();

  fdui->main->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

#include "close.xpm"
FD_help *create_form_help(void)
{
  FL_OBJECT *obj;
  FD_help *fdui = (FD_help *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->help = fl_bgn_form(FL_NO_BOX, 619, 491);
  obj = fl_add_box(FL_UP_BOX,0,0,619,491,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,5,60,125,425,"Index");
    fl_set_object_gravity(obj, FL_NorthWest, FL_SouthWest);
    fl_set_object_resize(obj, FL_RESIZE_Y);
  obj = fl_add_text(FL_NORMAL_TEXT,515,10,100,35,"Help Browser");
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
  fdui->help_index = obj = fl_add_browser(FL_HOLD_BROWSER,10,70,110,410,"");
    fl_set_object_boxtype(obj,FL_FLAT_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_gravity(obj, FL_NorthWest, FL_SouthWest);
    fl_set_object_resize(obj, FL_RESIZE_Y);
    fl_set_object_callback(obj,help_nav_cb,0);
    fl_set_browser_hscrollbar(obj, FL_OFF);
    fl_add_browser_line(obj, "Introduction");
    fl_add_browser_line(obj, "Movie Navigation");
    fl_add_browser_line(obj, "Movie Speed");
    fl_add_browser_line(obj, "Zoom, Pan, Rotate");
    fl_add_browser_line(obj, "Saving Frames");
    fl_add_browser_line(obj, "Snapshots");
    fl_add_browser_line(obj, "Printing");
    fl_add_browser_line(obj, "Interpolations");
    fl_add_browser_line(obj, "Layout Options");
    fl_add_browser_line(obj, "Colors");
    fl_add_browser_line(obj, "Font Choice");
    fl_add_browser_line(obj, "References");
    fl_add_browser_line(obj, "Acknowledgements");
    fl_add_browser_line(obj, "Copyright Issues");
  fdui->help_disp = obj = fl_add_browser(FL_NORMAL_BROWSER,135,60,480,425,"");
    fl_set_object_color(obj,FL_WHITE,FL_YELLOW);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_gravity(obj, FL_NorthWest, FL_SouthEast);
    fl_set_browser_vscrollbar(obj, FL_ON);
    fl_add_browser_line(obj, "   ");
    fl_add_browser_line(obj, "@C1@l@c Help Instructions");
    fl_add_browser_line(obj, "  ");
    fl_add_browser_line(obj, "@C4@c Select one of the topics on the left.");
  fdui->timeout = obj = fl_add_timer(FL_HIDDEN_TIMER,565,5,30,25,"");
    fl_set_object_callback(obj,text_timeout_cb,0);
  fdui->button10 = obj = fl_add_pixmapbutton(FL_RETURN_BUTTON,7,5,36,36,"Close");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,10);
    fl_set_object_helper(obj, "Close this window");
    fl_set_pixmapbutton_data(obj,close_xpm);
  fl_end_form();

  fdui->help->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

#include "cancel.xpm"
static FL_PUP_ENTRY fdchoice_font_3[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "helvetica-medium-r",	0,	"",	 FL_PUP_NONE},
    { "helvetica-bold-r",	0,	"",	 FL_PUP_NONE},
    { "helvetica-medium-o",	0,	"",	 FL_PUP_NONE},
    { "helvetica-bold-o",	0,	"",	 FL_PUP_NONE},
    { "courier-medium-r",	0,	"",	 FL_PUP_NONE},
    { "courier-bold-r",	0,	"",	 FL_PUP_NONE},
    { "courier-medium-o",	0,	"",	 FL_PUP_NONE},
    { "courier-bold-o",	0,	"",	 FL_PUP_NONE},
    { "times-medium-r",	0,	"",	 FL_PUP_NONE},
    { "times-bold-r",	0,	"",	 FL_PUP_NONE},
    { "times-medium-o",	0,	"",	 FL_PUP_NONE},
    { "times-bold-o",	0,	"",	 FL_PUP_NONE},
    { "charter-medium-r",	0,	"",	 FL_PUP_NONE},
    { "charter-bold-r",	0,	"",	 FL_PUP_NONE},
    { "charter-medium-i",	0,	"",	 FL_PUP_NONE},
    { "symbol",	0,	"",	 FL_PUP_NONE},
    {0}
};

#include "accept.xpm"
static FL_PUP_ENTRY fdchoice_size_4[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "8 Tiny",	0,	"",	 FL_PUP_NONE},
    { "10 Small",	0,	"",	 FL_PUP_NONE},
    { "12 Normal",	0,	"",	 FL_PUP_NONE},
    { "14 Medium",	0,	"",	 FL_PUP_NONE},
    { "18 Large",	0,	"",	 FL_PUP_NONE},
    { "24 Huge",	0,	"",	 FL_PUP_NONE},
    {0}
};

FD_fontselect *create_form_fontselect(void)
{
  FL_OBJECT *obj;
  FD_fontselect *fdui = (FD_fontselect *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->fontselect = fl_bgn_form(FL_NO_BOX, 368, 145);
  obj = fl_add_box(FL_UP_BOX,0,0,368,145,"");
  obj = fl_add_text(FL_NORMAL_TEXT,243,10,120,35,"Font Selection");
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
  fdui->button20 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,200,5,36,36,"Cancel");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
    fl_set_object_callback(obj,but_cb,20);
    fl_set_pixmapbutton_data(obj,cancel_xpm);
  fdui->font = obj = fl_add_choice(FL_NORMAL_CHOICE,36,62,135,26,"Font");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,font_choice_cb,0);
    fl_set_choice_entries(obj, fdchoice_font_3);
    fl_set_choice(obj,1);
  fdui->button21 = obj = fl_add_pixmapbutton(FL_RETURN_BUTTON,5,5,36,36,"Accept");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,21);
    fl_set_pixmapbutton_data(obj,accept_xpm);
  fdui->size = obj = fl_add_choice(FL_NORMAL_CHOICE,228,62,135,26,"Size");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,size_choice_cb,0);
    fl_set_choice_entries(obj, fdchoice_size_4);
    fl_set_choice(obj,1);
  fdui->text = obj = fl_add_input(FL_NORMAL_INPUT,5,95,358,44,"");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_COL1);
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_SouthEast);
  fl_end_form();

  fdui->fontselect->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

FD_about *create_form_about(void)
{
  FL_OBJECT *obj;
  FD_about *fdui = (FD_about *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->about = fl_bgn_form(FL_NO_BOX, 321, 210);
  obj = fl_add_box(FL_UP_BOX,0,0,321,210,"");
  fdui->version = obj = fl_add_text(FL_NORMAL_TEXT,80,155,225,20,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->package = obj = fl_add_text(FL_NORMAL_TEXT,80,135,230,20,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,80,75,225,20,"Written by Dirk Evers");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->button30 = obj = fl_add_pixmapbutton(FL_RETURN_BUTTON,5,5,36,36,"Close");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_callback(obj,but_cb,30);
    fl_set_pixmapbutton_data(obj,close_xpm);
  obj = fl_add_text(FL_NORMAL_TEXT,80,37,234,35,"The RNA 2D Structure Animator");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_SHADOW_STYLE);
  obj = fl_add_text(FL_NORMAL_TEXT,80,5,224,35,"RNA Movies");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE+FL_SHADOW_STYLE);
  obj = fl_add_text(FL_NORMAL_TEXT,80,95,225,20,"dirk@TechFak.Uni-Bielefeld.DE");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,80,185,225,20,"http://BiBiServ.TechFak.Uni-Bielefeld.DE/rnamovies/");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,80,117,225,20,"Adapted to pseudoknots by Alain Xayaphoummine");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fl_end_form();

  fdui->about->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

FD_brucc_opt *create_form_brucc_opt(void)
{
  FL_OBJECT *obj;
  FD_brucc_opt *fdui = (FD_brucc_opt *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->brucc_opt = fl_bgn_form(FL_NO_BOX, 463, 314);
  obj = fl_add_box(FL_UP_BOX,0,0,463,314,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,8,268,445,39,"Numbering");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,8,167,445,40,"Bonds");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,8,116,445,40,"Nucleotides");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
  fdui->labelfreq = obj = fl_add_input(FL_INT_INPUT,12,277,65,24,"Frequency");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,0);
  fdui->button43 = obj = fl_add_checkbutton(FL_PUSH_BUTTON,76,118,96,36,"Colored");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,43);
    fl_set_button(obj, 1);
  fdui->button42 = obj = fl_add_checkbutton(FL_PUSH_BUTTON,12,119,84,34,"Labeled");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,42);
    fl_set_button(obj, 1);
  obj = fl_add_text(FL_NORMAL_TEXT,261,5,126,40,"Structure Layout");
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
  fdui->button40 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,7,5,36,36,"Dismiss");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
    fl_set_object_callback(obj,but_cb,40);
    fl_set_pixmapbutton_data(obj,cancel_xpm);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,8,64,445,40,"Backbone");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,8,217,445,39,"Ends");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
  fdui->but400 = obj = fl_add_checkbutton(FL_PUSH_BUTTON,12,219,90,36,"5'");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,400);
    fl_set_button(obj, 1);

  fdui->dln_brucc = fl_bgn_group();
  fdui->button46 = obj = fl_add_checkbutton(FL_RADIO_BUTTON,145,169,77,36,"None");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,46);
  fdui->button45 = obj = fl_add_checkbutton(FL_RADIO_BUTTON,76,169,78,36,"Lines");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,45);
  fdui->button49 = obj = fl_add_checkbutton(FL_RADIO_BUTTON,12,169,78,36,"WC/GU");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,49);
    fl_set_button(obj, 1);
  fl_end_group();

  fdui->label_dist = obj = fl_add_valslider(FL_HOR_SLIDER,314,125,133,22,"Distance");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_DEFAULT_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,label_dist_cb,0);
    fl_set_slider_precision(obj, 3);
    fl_set_slider_bounds(obj, 0, 2);
    fl_set_slider_value(obj, 0.4);
    fl_set_slider_size(obj, 0.15);
    fl_set_slider_increment(obj, 0.1, 0.5);
  fdui->freq_dist = obj = fl_add_valslider(FL_HOR_SLIDER,313,276,133,22,"Distance");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_DEFAULT_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,freq_dist_cb,0);
    fl_set_slider_precision(obj, 3);
    fl_set_slider_bounds(obj, 0, 3);
    fl_set_slider_value(obj, 0.7);
    fl_set_slider_size(obj, 0.15);
  fdui->strip_width = obj = fl_add_valslider(FL_HOR_SLIDER,314,73,133,22,"Width");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_DEFAULT_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,0);
    fl_set_slider_precision(obj, 3);
    fl_set_slider_bounds(obj, 0.01, 0.5);
    fl_set_slider_value(obj, 0.15);
    fl_set_slider_size(obj, 0.15);
    fl_set_slider_increment(obj, 0.1, 0);
  fdui->but401 = obj = fl_add_checkbutton(FL_PUSH_BUTTON,76,219,77,36,"3'");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,401);
    fl_set_button(obj, 1);

  fdui->spl_brucc = fl_bgn_group();
  fdui->button47 = obj = fl_add_checkbutton(FL_RADIO_BUTTON,76,66,62,37,"Linear");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,47);
  fdui->button44 = obj = fl_add_checkbutton(FL_RADIO_BUTTON,12,66,58,37,"Spline");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,44);
  fdui->button48 = obj = fl_add_checkbutton(FL_RADIO_BUTTON,145,66,58,37,"Strips");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,48);
    fl_set_button(obj, 1);
  fdui->button403 = obj = fl_add_checkbutton(FL_RADIO_BUTTON,210,66,62,37,"None");
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,update_cb,403);
  fl_end_group();

  fl_end_form();

  fdui->brucc_opt->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

FD_log *create_form_log(void)
{
  FL_OBJECT *obj;
  FD_log *fdui = (FD_log *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->log = fl_bgn_form(FL_NO_BOX, 530, 360);
  obj = fl_add_box(FL_UP_BOX,0,0,530,360,"");
  fdui->browser = obj = fl_add_browser(FL_NORMAL_BROWSER,10,65,510,285,"");
    fl_set_object_gravity(obj, FL_NorthWest, FL_SouthEast);
  fdui->button80 = obj = fl_add_pixmapbutton(FL_RETURN_BUTTON,10,8,36,36,"Close");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,80);
    fl_set_pixmapbutton_data(obj,close_xpm);
  obj = fl_add_text(FL_NORMAL_TEXT,410,10,110,35,"Log Browser");
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
  fdui->button81 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,58,8,36,36,"Clear Log");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,81);
    fl_set_pixmapbutton_data(obj,cancel_xpm);
  fl_end_form();

  fdui->log->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

#include "fit.xpm"
FD_save_as *create_form_save_as(void)
{
  FL_OBJECT *obj;
  FD_save_as *fdui = (FD_save_as *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->save_as = fl_bgn_form(FL_NO_BOX, 329, 341);
  obj = fl_add_box(FL_UP_BOX,0,0,329,341,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,195,208,125,32,"EPS Options");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,6,115,180,125,"Frames");
  obj = fl_add_text(FL_NORMAL_TEXT,273,10,50,35,"Save");
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
  fdui->button90 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,205,5,36,36,"Cancel");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
    fl_set_object_callback(obj,but_cb,90);
    fl_set_pixmapbutton_data(obj,cancel_xpm);
  fdui->button91 = obj = fl_add_pixmapbutton(FL_RETURN_BUTTON,5,5,36,36,"Save");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,91);
    fl_set_pixmapbutton_data(obj,accept_xpm);
  fdui->xsize = obj = fl_add_counter(FL_NORMAL_COUNTER,5,59,130,34,"X-Size");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_callback(obj,Void,0);
    fl_set_counter_precision(obj, 0);
    fl_set_counter_bounds(obj, 10, 1024);
    fl_set_counter_value(obj, 512);
    fl_set_counter_step(obj, 1, 10);
  fdui->frames_from = obj = fl_add_input(FL_NORMAL_INPUT,65,172,40,24,"from:");
    fl_set_object_callback(obj,Void,0);
  fdui->frames_to = obj = fl_add_input(FL_NORMAL_INPUT,128,172,40,24,"to:");
    fl_set_object_callback(obj,Void,0);
  fdui->pathname = obj = fl_add_input(FL_NORMAL_INPUT,6,261,315,27,"Path:");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_TOP_LEFT);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
    fl_set_object_callback(obj,Void,0);
  fdui->prefix = obj = fl_add_input(FL_NORMAL_INPUT,6,308,315,27,"Prefix:");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_TOP_LEFT);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
    fl_set_object_callback(obj,Void,0);
  fdui->ysize = obj = fl_add_counter(FL_NORMAL_COUNTER,191,59,130,35,"Y-Size");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_callback(obj,Void,0);
    fl_set_counter_precision(obj, 0);
    fl_set_counter_bounds(obj, 10, 1024);
    fl_set_counter_value(obj, 512);
    fl_set_counter_step(obj, 1, 10);
  fdui->button92 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,145,58,36,36,"Window Size");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,92);
    fl_set_pixmapbutton_data(obj,fit1_xpm);

  fdui->frames = fl_bgn_group();
  fdui->frames_fr_to = obj = fl_add_checkbutton(FL_RADIO_BUTTON,11,172,25,25,"");
    fl_set_object_callback(obj,Void,0);
  fdui->frames_current = obj = fl_add_checkbutton(FL_RADIO_BUTTON,11,120,25,25,"current");
    fl_set_object_callback(obj,Void,0);
    fl_set_button(obj, 1);
  fdui->frames_all = obj = fl_add_checkbutton(FL_RADIO_BUTTON,11,145,25,25,"all");
    fl_set_object_callback(obj,Void,0);
  fl_end_group();

  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,195,115,125,80,"File Format");
  fdui->bg = obj = fl_add_checkbutton(FL_PUSH_BUTTON,198,212,25,25,"Use Background");
    fl_set_object_callback(obj,Void,0);

  fdui->format = fl_bgn_group();
  fdui->format_png = obj = fl_add_checkbutton(FL_RADIO_BUTTON,200,122,25,25,"PNG");
    fl_set_object_callback(obj,Void,0);
    fl_set_button(obj, 1);
  fdui->format_eps = obj = fl_add_checkbutton(FL_RADIO_BUTTON,200,145,25,25,"EPS");
    fl_set_object_callback(obj,Void,0);
  fdui->format_2dcoords = obj = fl_add_checkbutton(FL_RADIO_BUTTON,200,169,25,25,"2D Coords");
    fl_set_object_callback(obj,Void,0);
  fdui->format_jpg = obj = fl_add_checkbutton(FL_RADIO_BUTTON,261,123,25,25,"JPG");
    fl_set_object_callback(obj,Void,0);
  fl_end_group();

  fdui->button93 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,52,5,36,36,"Apply");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
    fl_set_object_callback(obj,but_cb,93);
    fl_set_object_helper(obj, "Set  Options for Snapshot \n(Don'tSave)");
    fl_set_pixmapbutton_data(obj,accept_xpm);
  fl_end_form();

  fdui->save_as->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

FD_color_opt *create_form_color_opt(void)
{
  FL_OBJECT *obj;
  FD_color_opt *fdui = (FD_color_opt *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->color_opt = fl_bgn_form(FL_NO_BOX, 154, 492);
  obj = fl_add_box(FL_UP_BOX,0,0,154,492,"");
  fdui->backbone_col = obj = fl_add_button(FL_NORMAL_BUTTON,6,94,29,28,"Backbone");
    fl_set_object_color(obj,FL_GREEN,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,1);
  fdui->background_col = obj = fl_add_button(FL_NORMAL_BUTTON,6,59,29,28,"Background");
    fl_set_object_color(obj,FL_WHITE,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,0);
  fdui->nucno_col = obj = fl_add_button(FL_NORMAL_BUTTON,6,129,29,28,"Nucleotide Number");
    fl_set_object_color(obj,FL_BLACK,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,2);
  fdui->wc_col = obj = fl_add_button(FL_NORMAL_BUTTON,6,165,29,28,"WC Bond");
    fl_set_object_color(obj,FL_RED,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,3);
  fdui->gu_col = obj = fl_add_button(FL_NORMAL_BUTTON,6,200,29,28,"GU Bond");
    fl_set_object_color(obj,FL_DARKORANGE,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,4);
  fdui->other_bond_col = obj = fl_add_button(FL_NORMAL_BUTTON,6,237,29,28,"Other Bonds");
    fl_set_object_color(obj,FL_BLUE,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,5);
  fdui->a_col = obj = fl_add_button(FL_NORMAL_BUTTON,5,274,29,28,"A");
    fl_set_object_color(obj,FL_RED,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,6);
  fdui->c_col = obj = fl_add_button(FL_NORMAL_BUTTON,5,311,29,28,"C");
    fl_set_object_color(obj,FL_BLUE,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,7);
  fdui->g_col = obj = fl_add_button(FL_NORMAL_BUTTON,5,349,29,28,"G");
    fl_set_object_color(obj,FL_BLACK,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,8);
  fdui->u_col = obj = fl_add_button(FL_NORMAL_BUTTON,5,384,29,28,"U");
    fl_set_object_color(obj,FL_GREEN,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,9);
  fdui->other_nuc_col = obj = fl_add_button(FL_NORMAL_BUTTON,5,421,29,28,"Other Nucleotides");
    fl_set_object_color(obj,FL_MAGENTA,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,10);
  obj = fl_add_text(FL_NORMAL_TEXT,76,12,66,34,"Colors");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,6,8,36,36,"Dismiss");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
    fl_set_object_callback(obj,but_cb,110);
    fl_set_pixmapbutton_data(obj,cancel_xpm);
  fdui->nuc_un_col = obj = fl_add_button(FL_NORMAL_BUTTON,5,457,29,28,"Single Color");
    fl_set_object_color(obj,FL_BLACK,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,col_cb,11);
  fl_end_form();

  fdui->color_opt->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

FD_interp *create_form_interp(void)
{
  FL_OBJECT *obj;
  FD_interp *fdui = (FD_interp *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->interp = fl_bgn_form(FL_NO_BOX, 284, 117);
  obj = fl_add_box(FL_UP_BOX,0,0,284,117,"");
  obj = fl_add_frame(FL_ENGRAVED_FRAME,7,73,270,37,"");
    fl_set_object_callback(obj,but_cb,41);
  obj = fl_add_pixmapbutton(FL_RETURN_BUTTON,7,8,36,36,"Accept");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,121);
    fl_set_pixmapbutton_data(obj,accept_xpm);
  obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,124,8,36,36,"Cancel");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
    fl_set_object_callback(obj,but_cb,120);
    fl_set_pixmapbutton_data(obj,cancel_xpm);
  obj = fl_add_text(FL_NORMAL_TEXT,169,6,106,40,"Interpolations");
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
  fdui->interp_freq = obj = fl_add_input(FL_INT_INPUT,17,79,53,25,"Interpolations");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,interp_freq_cb,0);
  fl_end_form();

  fdui->interp->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

static FL_PUP_ENTRY fdchoice_papersize_5[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "A4",	0,	"",	 FL_PUP_NONE},
    { "A3",	0,	"",	 FL_PUP_NONE},
    { "Letter",	0,	"",	 FL_PUP_NONE},
    { "Legal",	0,	"",	 FL_PUP_NONE},
    {0}
};

static FL_PUP_ENTRY fdchoice_paperorient_6[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "Portrait",	0,	"",	 FL_PUP_NONE},
    { "Landscape",	0,	"",	 FL_PUP_NONE},
    {0}
};

FD_print *create_form_print(void)
{
  FL_OBJECT *obj;
  FD_print *fdui = (FD_print *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(1);
  fdui->print = fl_bgn_form(FL_NO_BOX, 316, 233);
  obj = fl_add_box(FL_UP_BOX,0,0,316,233,"");
  fdui->button131 = obj = fl_add_pixmapbutton(FL_RETURN_BUTTON,6,7,36,36,"Accept");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthWest, FL_NorthWest);
    fl_set_object_callback(obj,but_cb,131);
    fl_set_pixmapbutton_data(obj,accept_xpm);
  fdui->button130 = obj = fl_add_pixmapbutton(FL_NORMAL_BUTTON,210,9,36,36,"Cancel");
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
    fl_set_object_callback(obj,but_cb,130);
    fl_set_pixmapbutton_data(obj,cancel_xpm);
  obj = fl_add_text(FL_NORMAL_TEXT,261,10,45,35,"Print");
    fl_set_object_color(obj,FL_COL1,FL_LEFT_BCOL);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_NorthEast, FL_NorthEast);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,6,66,180,110,"Frames");
  fdui->frames_from = obj = fl_add_input(FL_NORMAL_INPUT,65,123,40,24,"from:");
    fl_set_object_callback(obj,Void,0);
  fdui->frames_to = obj = fl_add_input(FL_NORMAL_INPUT,128,123,40,24,"to:");
    fl_set_object_callback(obj,Void,0);
  fdui->print_cmd = obj = fl_add_input(FL_NORMAL_INPUT,5,200,305,27,"Print Command:");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP_LEFT);
    fl_set_object_gravity(obj, FL_SouthWest, FL_SouthWest);
    fl_set_object_callback(obj,Void,0);

  fdui->frames = fl_bgn_group();
  fdui->frames_fr_to = obj = fl_add_checkbutton(FL_RADIO_BUTTON,11,123,25,25,"");
    fl_set_object_callback(obj,Void,0);
  fdui->frames_current = obj = fl_add_checkbutton(FL_RADIO_BUTTON,11,71,25,25,"current");
    fl_set_object_callback(obj,Void,0);
    fl_set_button(obj, 1);
  fdui->frames_all = obj = fl_add_checkbutton(FL_RADIO_BUTTON,11,96,25,25,"all");
    fl_set_object_callback(obj,Void,0);
  fl_end_group();

  fdui->papersize = obj = fl_add_choice(FL_NORMAL_CHOICE2,205,79,92,24,"Paper Size");
    fl_set_object_lalign(obj,FL_ALIGN_TOP_LEFT);
    fl_set_object_callback(obj,Void,0);
    fl_set_choice_entries(obj, fdchoice_papersize_5);
    fl_set_choice(obj,1);
  fdui->paperorient = obj = fl_add_choice(FL_NORMAL_CHOICE2,205,124,92,25,"Orientation");
    fl_set_object_lalign(obj,FL_ALIGN_TOP_LEFT);
    fl_set_object_callback(obj,Void,0);
    fl_set_choice_entries(obj, fdchoice_paperorient_6);
    fl_set_choice(obj,1);
  fdui->background = obj = fl_add_checkbutton(FL_PUSH_BUTTON,199,161,25,25,"Use Background");
    fl_set_object_callback(obj,Void,0);
  fl_end_form();

  fdui->print->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

