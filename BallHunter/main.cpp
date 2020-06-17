// The main routine makes the window, and then runs an even loop
// until the window is closed
#include <windows.h>
#include <iostream>
#include <ctime>

#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>
#include <Fl/Fl_Value_Slider.H>
#include <Fl/Fl_Button.h>
#include <Fl/Fl_Light_Button.h>
#include <Fl/Fl_Choice.h>
#include <FL/Fl_Box.h>

#include "MyGlWindow.h"


//==================== Windows Constants ====================//


long lastRedraw;
int frameRate = 60;


//==================== UI Constants ====================//


Fl_Group* widgets;


//==================== Buttons ====================//


void changeFrameCB(Fl_Widget * w, void* data)
{
	Fl_Choice * widget = (Fl_Choice *)w;
	int i = widget->value();
	const char * menu = widget->text(i);
	frameRate = atoi(menu);

	MyGlWindow * win = (MyGlWindow *)data;
	win->redraw();
}

void idleCB(void* w)
{
	MyGlWindow * win = (MyGlWindow *)w;
	if (clock() - lastRedraw > CLOCKS_PER_SEC / frameRate) {
		lastRedraw = clock();
		win->update();
	}
	win->redraw();
}


void but_cb_play(Fl_Widget* o, void*data)
{
	Fl_Button* b = (Fl_Button*)o;
	MyGlWindow * win = (MyGlWindow *)data;
	if (b->value())
		win->run = 1;
	else
		win->run = 0;
	win->damage(1);
}

void but_cb_reload(Fl_Widget* o, void* data)
{
	Fl_Button* b = (Fl_Button*)o;
	MyGlWindow* win = (MyGlWindow*)data;
	win->reload();
	win->damage(1);
}

void but_cb_restart(Fl_Widget* o, void*data)
{
	Fl_Button* b = (Fl_Button*)o;
	MyGlWindow * win = (MyGlWindow *)data;
	win->restart();
	win->damage(1);
}


//==================== MAIN ====================//


int main()
{
	Fl::scheme("plastic"); // plastic
	int width = 1280;
	int height = 720;
	
	Fl_Double_Window* wind = new Fl_Double_Window(100, 100, width, height, "Ball Hunter by SACHS_A");

	wind->begin(); // put widgets inside of the window

	widgets = new Fl_Group(0, 0, width, height);
	widgets->begin();

	MyGlWindow* gl = new MyGlWindow(10, 10, width - 20, height - 50);
	Fl::add_idle((void(*)(void*)) idleCB, gl);

	widgets->end();
	Fl_Group::current()->resizable(widgets);

	Fl_Choice* choice;
	choice = new Fl_Choice(100, height - 40, 50, 20, "FrameRate");
	choice->add("15");
	choice->add("30");
	choice->add("60");
	choice->value(2);
	choice->callback((Fl_Callback*)changeFrameCB, gl);

	Fl_Light_Button * play = new Fl_Light_Button(width - 800, height - 40, 100, 20, "Play/Pause");
	play->callback(but_cb_play, gl);

	Fl_Button* restart = new Fl_Button(width - 600, height - 40, 100, 20, "Restart");
	restart->callback(but_cb_restart, gl);

	Fl_Button* reload = new Fl_Button(width - 400, height - 40, 100, 20, "Reload Ball");
	reload->callback(but_cb_reload, gl);

	wind->end();

	wind->show();

	Fl::run();
	delete wind;

	return 1;
}