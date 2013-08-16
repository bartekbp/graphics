#include "Variables.h"

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		delete g_pConeMesh;
		g_pConeMesh = NULL;
		delete g_pCylinderMesh;
		g_pCylinderMesh = NULL;
		delete g_pPlaneMesh;
		g_pPlaneMesh = NULL;
		delete car;
		car = NULL;
		delete g_pTetrahedronMesh;
		g_pTetrahedronMesh = NULL;
		delete g_pSphereMesh;
		g_pSphereMesh = NULL;
		glutLeaveMainLoop();
		return;
	case '1': car->turn(0.15f); break;
	case '2': car->turn(-0.15f); break;
	case '3': car->move(1.0f); break;
	case '4': car->move(-0.8f); break;
		
	case 32:
		g_bDrawLookatPoint = !g_bDrawLookatPoint;
		break;
	default:
		g_viewPole.CharPress(key);
	}

	glutPostRedisplay();
}

