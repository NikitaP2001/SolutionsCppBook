const int XMAX=640;
const int YMAX=480;

struct point {
	int x, y;
	point () { }
	point (int a, int b) { x = a; y = b; }
};

extern void put_point(int a, int b);
inline void put_point(point p) { put_point(p.x, p.y); }
extern void put_line(int, int, int, int);
inline void put_line(point a, point b)
{
	put_line(a.x, a.y, b.x, b.y);
}
extern void screen_init();
extern void screen_destroy();
extern void screen_refresh();
extern void screen_clear();

extern void put_circle(int x0, int y0, int radius);

inline void put_triangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	put_line(x0, y0, x1, y1);
	put_line(x1, y1, x2, y2);
	put_line(x2, y2, x0, y0);
}
