#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
static int Day = 0, Time = 0;
GLboolean IsWire = true;
GLboolean IsParallel = true;

const int StartWindowWidth = 500;
const int StartWindowHeight = 500;

GLint CurrentWindowWidth = 500;
GLint CurrentWindowHeight = 500;

// �¾��� ��ġ�� ��ġ�� ���� ����
void MyLightInit() {
	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 }; // ���� �ֺ� �ݻ�

	// 0�� ���� Ư��
	GLfloat light0_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat light0_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	// ��ü Ư��
	GLfloat material_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat material_specular[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 25.0 };

	glShadeModel(GL_SMOOTH); // ���� ���̵�
	glEnable(GL_DEPTH_TEST); // ���� ���� Ȱ��ȭ
	glEnable(GL_LIGHTING); // ���� ��� Ȱ��ȭ

	// ����
	glEnable(GL_LIGHT0); // 0�� ���� Ȱ��ȭ
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); // 0�� ���� Ư��
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	// ��ü Ư��
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); // ���� �ֺ���
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // ���� ����
}

void setLightPosition() {
	GLfloat LightPosition0[] = { 0.0, 0.0, 2.0, 1.0 }; // 0�� ���� ��ġ
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0); // ��ġ�� ����
	// glutLookAt()�� ���� ��ȯ�� glTranslatef()�� �� ��ȯ ���Ŀ� ������ ����...(������ ��ġ�� �� �� ��Ŀ� ���� ��ȯ)... �� �ؼ��ض�..
}

// ��������, �������� ����
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


	// 3���� ��ü Ȯ�ο� ��ǥ��
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

// 'D' 'T' �Է� �� �ݴ���� ȸ��
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
		IsWire = true; // ��
	else if (entryID == 2)
		IsWire = false; // ����
	glutPostRedisplay();
}

void MyProjectionSubMenu(int projectionID) {
	if (projectionID == 1)
		IsParallel = true; // ��
	else if (projectionID == 2)
		IsParallel = false; // ����
	setProjectionState();
	glutPostRedisplay();
}

// âũ�� ����Ǿ �ְ� �߻� X
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

	//MyLightInit();

	GLint MySubMenuID = glutCreateMenu(MySubMenu); // Rendering Menu �߰�
	glutAddMenuEntry("Solid", 1);
	glutAddMenuEntry("Wire", 2);
	GLint MyProjectionSubMenuID = glutCreateMenu(MyProjectionSubMenu); // Projection Menu �߰�
	glutAddMenuEntry("Parallel", 1);
	glutAddMenuEntry("Perspective", 2);
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("Change Rendering", MySubMenuID);
	glutAddSubMenu("Change Projection", MyProjectionSubMenuID);
	glutAddMenuEntry("Exit", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutReshapeFunc(MyReshape); // Reshpae �̺�Ʈ
	glutMainLoop();
	return 0;
}


// 1. �¾��� ��ġ�� ��ġ�� ���� ����
// 2. ������ ���� ��ġ�� �� ���� ��ġ