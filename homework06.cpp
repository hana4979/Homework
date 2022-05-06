#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
static int Day = 0, Time = 0;
GLboolean IsWire = true; // 참
GLboolean IsParallel = false; // 참

const int StartWindowWidth = 500;
const int StartWindowHeight = 500;

GLint CurrentWindowWidth = 500;
GLint CurrentWindowHeight = 500;

void setProjectionState() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (IsParallel) {
		GLfloat WidthFactor = (GLfloat)CurrentWindowWidth / (GLfloat)StartWindowWidth;
		GLfloat HeightFactor = (GLfloat)CurrentWindowHeight / (GLfloat)StartWindowHeight;
		glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor, -1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 10.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 1, 2, 0, 0, 0, 0, 1, 0);
	}
	else {
		gluPerspective(30, (GLfloat)CurrentWindowWidth / (GLfloat)CurrentWindowHeight, 1, 50);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(2, 2, 4, 0, 0, 0, 0, 1, 0);
	}
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.3, 0.3);
	if (IsWire)
		glutWireSphere(0.2, 20, 15);
	else
		glutSolidSphere(0.2, 20, 15);

	glPushMatrix();
	glRotatef((GLfloat)Day, 0.0, 1.0, 0.0);
	glTranslatef(0.7, 0.0, 0.0);
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.6, 0.7);
	if (IsWire)
		glutWireSphere(0.1, 10, 8);
	else
		glutSolidSphere(0.1, 10, 8);
	glPushMatrix();
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glTranslatef(0.2, 0.0, 0.0);
	glColor3f(0.9, 0.8, 0.2);
	if (IsWire)
		glutWireSphere(0.04, 10, 8);
	else
		glutSolidSphere(0.04, 10, 8);
	glPopMatrix();
	glPopMatrix();


	// 3차원 객체 확인용 좌표축
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();

	glutSwapBuffers();
}

// 'D' 'T' 입력 시 반대방향 회전
void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'd':
		Day = (Day + 10) % 360;
		glutPostRedisplay();
		break;
	case 't':
		Time = (Time + 5) % 300;
		glutPostRedisplay();
		break;
	case 'D':
		Day = (Day - 10) % 360;
		glutPostRedisplay();
		break;
	case 'T':
		Time = (Time - 5) % 300;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void MyMainMenu(int entryID) {
	if (entryID == 3)
		exit(0);
	glutPostRedisplay();
}

void MySubMenu(int entryID) {
	if (entryID == 1)
		IsWire = true; // 참
	else if (entryID == 2)
		IsWire = false; // 거짓
	glutPostRedisplay();
}

void MyProjectionSubMenu(int projectionID) {
	if (projectionID == 1)
		IsParallel = true; // 참
	else if (projectionID == 2)
		IsParallel = false; // 거짓
	setProjectionState();
	glutPostRedisplay();
}

// 창크기 변경되어도 왜곡 발생 X
void MyReshape(int NewWidth, int NewHeight) {
	CurrentWindowWidth = NewWidth;
	CurrentWindowHeight = NewHeight;
	glViewport(0, 0, CurrentWindowWidth, CurrentWindowHeight);
	setProjectionState();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(StartWindowWidth, StartWindowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(1.0, 1.0, 1.0, 1.0);

	GLint MySubMenuID = glutCreateMenu(MySubMenu); // Rendering Menu 추가
	glutAddMenuEntry("Solid", 1);
	glutAddMenuEntry("Wire", 2);
	GLint MyProjectionSubMenuID = glutCreateMenu(MyProjectionSubMenu); // Projection Menu 추가
	glutAddMenuEntry("Parallel", 1);
	glutAddMenuEntry("Perspective", 2);
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("Change Rendering", MySubMenuID);
	glutAddSubMenu("Change Projection", MyProjectionSubMenuID);
	glutAddMenuEntry("Exit", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutReshapeFunc(MyReshape); // Reshpae 이벤트
	glutMainLoop();
	return 0;
}