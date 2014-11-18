#ifndef MAIN_H
#define MAIN_H

#include <vector>
#define WINDOW_WIDTH 600
int WINDOW_HEIGHT = 1;

float HEIGHT_MAX = 0;
float WIDTH_MAX = 0;
float HEIGHT_MIN = 10000;
float WIDTH_MIN = 10000;
float HEIGHT_RATIO = 1;
float WIDTH_RATIO = 1;
int BORDER = 20;


using namespace std;

struct color {
	float r, g, b;		// Color (R,G,B values)
};

class Vertex
{
public:
	int x, y;

	Vertex(void)
	{
		x = y = 0;
	}

	Vertex(int nX, int nY)
	{
		x = nX;
		y = nY;
	}
};

class Edge
{
public:
	Vertex start, end;

	Edge()
	{
		start = Vertex();
		end = Vertex();
	}

	Edge( Vertex v1, Vertex v2)
	{
		start = Vertex(v1.x, v1.y);
		end = Vertex(v2.x, v2.y);
	}

};

// Clears framebuffer to black
void clearFramebuffer();

// Sets pixel x,y to the color RGB
void setFramebuffer(int x, int y, float R, float G, float B);

void load_file( string file);
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
Vertex to_window(Vertex v);

#endif