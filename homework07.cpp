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

// 태양의 위치에 위치성 광원 설정
void MyLightInit() {
	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 }; // 전역 주변 반사

	// 0번 광원 특성
	GLfloat light0_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat light0_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	// 물체 특성
	GLfloat material_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat material_specular[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 25.0 };

	glShadeModel(GL_SMOOTH); // 구로 셰이딩
	glEnable(GL_DEPTH_TEST); // 깊이 버퍼 활성화
	glEnable(GL_LIGHTING); // 조명 기능 활성화

	// 광원
	glEnable(GL_LIGHT0); // 0번 광원 활성화
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); // 0번 광원 특성
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	// 물체 특성
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); // 전역 주변광
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // 근접 시점
}

void setLightPosition() {
	GLfloat LightPosition0[] = { 0.0, 0.0, 2.0, 1.0 }; // 0번 광원 위치
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0); // 위치성 광원
	// glutLookAt()의 시점 변환과 glTranslatef()의 모델 변환 이후에 광원이 선언...(광원의 위치가 모델 뷰 행렬에 의해 변환)... 을 해석해라..
}

// 원근투상, 평행투상 구현
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

	//MyLightInit();

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


// 1. 태양의 위치에 위치성 광원 설정
// 2. 지구와 달의 위치에 점 광원 설치