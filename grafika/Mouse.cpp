#include "Variables.h"
#include "../framework/MousePole.h"

void MouseMotion(int x, int y)
{
	Framework::ForwardMouseMotion(g_viewPole, x, y);
	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y)
{
	Framework::ForwardMouseButton(g_viewPole, button, state, x, y);
	glutPostRedisplay();
}

void MouseWheel(int wheel, int direction, int x, int y)
{
	Framework::ForwardMouseWheel(g_viewPole, wheel, direction, x, y);
	glutPostRedisplay();
}