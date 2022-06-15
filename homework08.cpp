#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glaux.h>
#include <GL/glu.h>
#include <GL/gl.h>

#pragma comment (lib, "glaux.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

static int Day = 0, Time = 0;
GLboolean IsWire = true;
GLboolean IsParallel = true;
GLfloat Xrot, Yrot, Zrot;

const int StartWindowWidth = 500;
const int StartWindowHeight = 500;

GLint CurrentWindowWidth = 500;
GLint CurrentWindowHeight = 500;

GLUquadric* solid_sphere = NULL;

GLuint MyTextureObject[2];
AUX_RGBImageRec* pTextureImage[2];

// 광원 상태 변수 설정
void MyLightInit() {
	glShadeModel(GL_SMOOTH); // 구로 셰이딩
	glEnable(GL_DEPTH_TEST); // 깊이 버퍼 활성화
	glEnable(GL_LIGHTING); // 조명 기능 활성화

	// 전역 광원 설정
	GLfloat global_ambient[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// 태양 광원 설정
	GLfloat sun_diffuse[] = { 1, 1, 1, 1 }; // 분산광
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_diffuse);

	// 지구 광원 설정
	GLfloat earth_diffuse[] = { 0, 0.7, 0, 1 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, earth_diffuse);

	// 달 광원 설정
	GLfloat moon_diffuse[] = { 0, 0, 0.7, 1 };
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, moon_diffuse);
}

void setProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (IsParallel) {
		GLfloat WidthFactor = (GLfloat)CurrentWindowWidth / (GLfloat)StartWindowWidth;
		GLfloat HeightFactor = (GLfloat)CurrentWindowHeight / (GLfloat)StartWindowHeight;
		glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor, -1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 10.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);
	}
	else {
		GLdouble aspect = (GLfloat)CurrentWindowWidth / (GLfloat)CurrentWindowHeight;
		gluPerspective(30, aspect, 1, 50);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);
	}
}

void drawXyzLine() {
	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
}

void drawSphere(GLdouble radius, GLint slices, GLint stacks) {
	if (IsWire)
		glutWireSphere(radius, slices, stacks);
	else
		glutSolidSphere(radius, slices, stacks);
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setProjection();

	// 광원 위치
	GLfloat lightPosition[] = { 0,0,0,1 };

	// 태양 출력
	glColor3f(1.0, 0.3, 0.3);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glDisable(GL_LIGHTING);
	drawSphere(0.2, 20, 15);
	glEnable(GL_LIGHTING);

	// 지구 출력
	glPushMatrix();
	glRotatef((GLfloat)Day, 0.0, 1.0, 0.0);
	glTranslatef(0.7, 0.0, 0.0);
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.6, 0.7);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	if (IsWire) {
		glutWireSphere(0.1, 10, 8);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
		gluSphere(solid_sphere, 0.1, 10, 8);
	}

	// 달 출력
	glPushMatrix();
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glTranslatef(0.2, 0.0, 0.0);
	glColor3f(0.9, 0.8, 0.2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition);
	if (IsWire) {
		glutWireSphere(0.04, 10, 8);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
		gluSphere(solid_sphere, 0.04, 10, 8);
	}

	glPopMatrix();
	glPopMatrix();

	drawXyzLine();

	glutSwapBuffers();
}

// 텍스처 매핑
void loadTexture(void)
{
	pTextureImage[0] = auxDIBImageLoad(".\\earth.bmp");
	pTextureImage[1] = auxDIBImageLoad(".\\moon.bmp");

	glGenTextures(2, MyTextureObject);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pTextureImage[i]->sizeX, pTextureImage[i]->sizeY, 0,
			GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[i]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	for (int i = 0; i < 2; i++)
	{
		if (pTextureImage[i])
		{
			if (pTextureImage[i]->data)
				free(pTextureImage[i]->data);
			free(pTextureImage[i]);
		}
	}
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
		//exit(0);
		glutPostRedisplay();
}

void RenderingMenu(int entryID) {
	if (entryID == 1)
		IsWire = false; // 참
	else if (entryID == 2)
		IsWire = true; // 거짓
	glutPostRedisplay();
}

void MyProjectionSubMenu(int projectionID) {
	if (projectionID == 1)
		IsParallel = true; // 참
	else if (projectionID == 2)
		IsParallel = false; // 거짓
	glutPostRedisplay();
}

void MyReshape(int NewWidth, int NewHeight) {
	CurrentWindowWidth = NewWidth;
	CurrentWindowHeight = NewHeight;
	glViewport(0, 0, CurrentWindowWidth, CurrentWindowHeight);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(StartWindowWidth, StartWindowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");

	MyLightInit();
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 메뉴 추가
	GLint MySubMenuID = glutCreateMenu(RenderingMenu); // Rendering Menu 추가
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

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 텍스처 매핑
	glEnable(GL_TEXTURE_2D);
	solid_sphere = gluNewQuadric();

	gluQuadricNormals(solid_sphere, GLU_SMOOTH);
	gluQuadricOrientation(solid_sphere, GLU_OUTSIDE);
	gluQuadricDrawStyle(solid_sphere, GLU_FILL);
	gluQuadricTexture(solid_sphere, GL_TRUE);

	loadTexture();

	// 콜백 함수 등록
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutReshapeFunc(MyReshape);

	glutMainLoop();

	return 0;
}