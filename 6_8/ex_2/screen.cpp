#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include "screen.hpp"

namespace screen {

	class primitive {
		public:
		virtual void draw(Display *, Window) = 0;
		virtual ~primitive() {}
		virtual primitive *clone() const = 0;
	};

	class line : public primitive {
		public:

		line(int x0, int y0, int x1, int y1)
		{
			this->x0 = x0;
			this->y0 = y0;
			this->x1 = x1;
			this->y1 = y1;
		}

		line(line const &another)
		{
			this->x0 = another.x0;
			this->y0 = another.y0;
			this->x1 = another.x1;
			this->y1 = another.y1;
		}

		virtual ~line() {}

		line *clone() const
		{
			return new line(*this);
		}

		void draw(Display *dsp, Window wnd)
		{
			int nscr = DefaultScreen(dsp);
			XDrawLine(dsp, wnd,
				       	DefaultGC(dsp, nscr), x0, y0, x1, y1);

		}

		private:
		int x0, y0, x1, y1;

	};

	class point : public primitive {
		public:

		point(int x, int y)
		{
			this->x = x;
			this->y = y;
		}

		point(point const & another)
		{
			this->x = another.x;
			this->y = another.y;
		}

		virtual ~point() {}

		point *clone() const
		{
			return new point(*this);
		}

		void draw(Display *dsp, Window wnd)
		{
			int nscr = DefaultScreen(dsp); 
			XDrawPoint(dsp, wnd,
					DefaultGC(dsp, nscr), x, y);
		}

		private:
		int x, y;
	};

	class screen {
		public:

		screen(Display *dsp, Window wnd)
		{
			this->wnd = wnd;
			this->dsp = dsp;
		}

		~screen() { clear(); }

		void clear()
		{
			for (auto &prim : primitives) {
				delete prim;
			}
			primitives.erase(primitives.begin(), primitives.end());
		}

		void draw()
		{
			for (auto &prim : primitives) {
				prim->draw(dsp, wnd);
			}
		}

		void add_primitive(primitive const& pr)
		{
			primitives.push_back(pr.clone());
		}

		private:
			Window wnd;
			Display *dsp;
			std::vector<primitive*> primitives;
	};

}

Display *screen_display;
Window screen_window;
screen::screen *main_screen;

void put_point(int a, int b)
{
	screen::point p(a, b);
	main_screen->add_primitive(p);

}

void put_line(int x0, int y0, int x1, int y1)
{
	screen::line l(x0, y0, x1, y1);
	main_screen->add_primitive(l);
}

static void fix_wnd_size(Window wnd, int xmax, int ymax)
{
	XSizeHints hints;
	long supret;
	XGetWMNormalHints(screen_display, wnd, &hints, &supret);
	hints.min_width = hints.max_width = xmax;
	hints.min_height = hints.max_height = ymax;
	hints.flags |= PMinSize | PMaxSize;
	XSetWMNormalHints(screen_display, wnd, &hints);
}

void screen_init()
{
	if (screen_display != NULL)
		return;

	screen_display = XOpenDisplay(NULL);
	
	// default screen number
	int nscr = DefaultScreen(screen_display);

	// left upeer wnd coords
	int x0_w = 10, y0_w = 10;

   	screen_window = XCreateSimpleWindow(screen_display, RootWindow(screen_display, nscr),
			x0_w, y0_w, XMAX, YMAX, 1, BlackPixel(screen_display, nscr),
			WhitePixel(screen_display, nscr));
	fix_wnd_size(screen_window, XMAX, YMAX);

	XSelectInput(screen_display, screen_window, ExposureMask | KeyPressMask);
   	XMapWindow(screen_display, screen_window);

	main_screen = new screen::screen(screen_display, screen_window);
}

void screen_destroy()
{
	delete main_screen;
	if (screen_display != NULL)
		XCloseDisplay(screen_display);
}

void screen_refresh()
{
	XEvent e;
	for (;;) {
		XNextEvent(screen_display, &e);
		if (e.type == Expose)
			main_screen->draw();
		if (e.type == KeyPress)
			break;
	}
}

void screen_clear()
{
}

