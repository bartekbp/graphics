#include "Variables.h"

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
//exit the program.
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		delete g_pConeMesh;
		g_pConeMesh = NULL;
		delete g_pCylinderMesh;
		g_pCylinderMesh = NULL;
		delete g_pCubeColorMesh;
		g_pCubeColorMesh = NULL;
		delete g_pPlaneMesh;
		g_pPlaneMesh = NULL;
		delete car;
		car = NULL;
		glutLeaveMainLoop();
		return;
	case 'w': g_camTarget.z -= 4.0f; break;
	case 's': g_camTarget.z += 4.0f; break;
	case 'd': g_camTarget.x += 4.0f; break;
	case 'a': g_camTarget.x -= 4.0f; break;
	case 'e': g_camTarget.y -= 4.0f; break;
	case 'q': g_camTarget.y += 4.0f; break;
	case 'W': g_camTarget.z -= 0.4f; break;
	case 'S': g_camTarget.z += 0.4f; break;
	case 'D': g_camTarget.x += 0.4f; break;
	case 'A': g_camTarget.x -= 0.4f; break;
	case 'E': g_camTarget.y -= 0.4f; break;
	case 'Q': g_camTarget.y += 0.4f; break;
	case 'i': g_sphereCamRelPos.y -= 11.25f; break;
	case 'k': g_sphereCamRelPos.y += 11.25f; break;
	case 'j': g_sphereCamRelPos.x -= 11.25f; break;
	case 'l': g_sphereCamRelPos.x += 11.25f; break;
	case 'o': g_sphereCamRelPos.z -= 5.0f; break;
	case 'u': g_sphereCamRelPos.z += 5.0f; break;
	case 'I': g_sphereCamRelPos.y -= 1.125f; break;
	case 'K': g_sphereCamRelPos.y += 1.125f; break;
	case 'J': g_sphereCamRelPos.x -= 1.125f; break;
	case 'L': g_sphereCamRelPos.x += 1.125f; break;
	case 'O': g_sphereCamRelPos.z -= 0.5f; break;
	case 'U': g_sphereCamRelPos.z += 0.5f; break;
	case '1': car->turn(0.15f); break;
	case '2': car->turn(-0.15f); break;
	case '3': car->move(0.5f); break;
	case '4': car->move(-0.5f); break;
		
	case 32:
		g_bDrawLookatPoint = !g_bDrawLookatPoint;
		printf("Target: %f, %f, %f\n", g_camTarget.x, g_camTarget.y, g_camTarget.z);
		printf("Position: %f, %f, %f\n", g_sphereCamRelPos.x, g_sphereCamRelPos.y, g_sphereCamRelPos.z);
		break;
	}

	g_sphereCamRelPos.y = glm::clamp(g_sphereCamRelPos.y, -78.75f, -1.0f);
	g_camTarget.y = g_camTarget.y > 0.0f ? g_camTarget.y : 0.0f;
	g_sphereCamRelPos.z = g_sphereCamRelPos.z > 5.0f ? g_sphereCamRelPos.z : 5.0f;

	glutPostRedisplay();
}

