#include <Windows.h>
#include <GL/glut.h>
#include <math.h>
#include "main.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/******************************************************************
	
	Enumeration Simulation

	Nathan Kocmoud
	This program simulates the navigation of UAV's through
	a mapped buiding so that all persons are counted in the 
	building.

 *****************************************************************/

float framebuffer[WINDOW_WIDTH][WINDOW_WIDTH*2][3];

vector<Vertex> vertices;
vector<Edge> edges;


int main(int argc, char** argv)
{
	// receive command line input
	if (argv[1] != NULL) {
		load_file(string(argv[1]));
	}
	else {
		load_file("floor_plan1.obj");
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Nathan Kocmoud - Homework 4");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

Vertex to_window(Vertex v) {

	// Account for inverted y axis and border
	int x = (v.x - WIDTH_MIN)*((WINDOW_WIDTH-BORDER)) / (WIDTH_MAX - WIDTH_MIN) + BORDER/2;
	int y = (WINDOW_HEIGHT - BORDER)*(1 - ((v.y - HEIGHT_MIN) / (HEIGHT_MAX - HEIGHT_MIN))) + BORDER/2;

	// Adjust for centering in window
	return Vertex(x, y);
}

// Draws the scene
void drawit(void)
{
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, framebuffer);
   glFlush();
}

// Clears framebuffer to black
void clearFramebuffer()
{
	int i,j;

	for (i = 0; i < WINDOW_HEIGHT; ++i ){
		for (j = 0; j<WINDOW_WIDTH; j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

// Sets pixel x,y to the color RGB
// I've made a small change to this function to make the pixels match
// those returned by the glutMouseFunc exactly - Scott Schaefer 
void setFramebuffer(int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;
	if (R<=1.0)
		if (R>=0.0)
			framebuffer[y][x][0]=R;
		else
			framebuffer[y][x][0]=0.0;
	else
		framebuffer[y][x][0]=1.0;
	if (G<=1.0)
		if (G>=0.0)
			framebuffer[y][x][1]=G;
		else
			framebuffer[y][x][1]=0.0;
	else
		framebuffer[y][x][1]=1.0;
	if (B<=1.0)
		if (B>=0.0)
			framebuffer[y][x][2]=B;
		else
			framebuffer[y][x][2]=0.0;
	else
		framebuffer[y][x][2]=1.0;
}

void display(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	// Clear background color to white
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();

	glColor3f(0, 0, 0);

	for (int i = 0; i < edges.size(); ++i) {
		Vertex start = to_window(edges[i].start);
		Vertex end = to_window(edges[i].end);

		glBegin(GL_LINES);
			glVertex2f(start.x, start.y);
			glVertex2f(end.x, end.y);
		glEnd();
	}

	glFlush();
}

void init(void)
{
	clearFramebuffer();

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1, 0, 0);									//Set color to red

	glEnable(GL_BLEND);							// Enable the OpenGL Blending functionality  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void keyboard(unsigned char key, int x, int y) {
	clearFramebuffer();
	
	vector<Vertex> pts;

	switch (key)
	{
	case '1':
				
		glutPostRedisplay();
		break;

	case '2':
		
		glutPostRedisplay();
		break;
	default:
		break;

	}

}

void load_file(string file_name) {
	
	char type = NULL;
	string px, py, qx, qy = "";
	vector<int> face_vertices;	// store vertices for one face

	ifstream file(file_name);
	stringstream line;

	while (!file.eof()) {

		// get line
		string line_str("");
		getline(file, line_str);
		line.str(line_str);

		// get first character
		line >> type;

		// read in edge
		if (type == 'e') {

			line >> px >> py >> qx >> qy;
			float x_max = max((float)atof(px.c_str()), (float)atof(qx.c_str()));
			float x_min = min((float)atof(px.c_str()), (float)atof(qx.c_str()));
			float y_max = max((float)atof(py.c_str()), (float)atof(qy.c_str()));
			float y_min = min((float)atof(py.c_str()), (float)atof(qy.c_str()));

			if (x_max > WIDTH_MAX)
				WIDTH_MAX = x_max;
			if (x_min < WIDTH_MIN)
				WIDTH_MIN = x_min;
			if (y_max > HEIGHT_MAX)
				HEIGHT_MAX = y_max;
			if (y_min < HEIGHT_MIN)
				HEIGHT_MIN = y_min;

			edges.push_back(Edge(Vertex((float)atof(px.c_str()), (float)atof(py.c_str())), Vertex((float)atof(qx.c_str()), (float)atof(qy.c_str()))));
		}

		// throw away line
		line.clear();
	}

	WINDOW_HEIGHT = (HEIGHT_MAX - HEIGHT_MIN)*WINDOW_WIDTH / (WIDTH_MAX - WIDTH_MIN);
}


