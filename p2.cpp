#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLfloat bgColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // �ʱⰪ : ��� 

std::random_device rd;  // �õ尪�� ��� ���� random_device ����.
std::mt19937 gen(rd());	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
std::uniform_real_distribution<float> dis(0.0f, 1.0f); // 0.0f ���� 1.0f ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.

class Rectangle {
public:
	float x, y; // �簢���� �߽� ��ǥ
	float width, height; // �簢���� �ʺ�� ����
	float r, g, b; // �簢���� ���� (RGB)

	Rectangle(float x, float y, float w, float h, float red, float green, float blue)
		: x(x), y(y), width(w), height(h), r(red), g(green), b(blue) {}

	void draw() {
		glColor3f(r, g, b);
		glRectf(x - width / 2, y + height / 2, x + width / 2, y - height / 2);
	}

	bool is_contain(float mx, float my) const {
		return mx >= x - width / 2 && mx <= x + width / 2 &&
			my >= y - height / 2 && my <= y + height / 2;
	}

	void randomColor() {
		r = dis(gen);
		g = dis(gen);
		b = dis(gen);
	}
};

std::vector<Rectangle> rectangles;

void initRectangles() {
	rectangles.push_back(Rectangle(0.5f, 0.5f, 1.0f, 1.0f, dis(gen), dis(gen), dis(gen))); // ������ ���� �ڿ� ���� (push_back)
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ���� (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ 
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	std::cout << "GLEW Initialized\n";
	initRectangles();
	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: ��� �ݹ� �Լ� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // �������� ��white���� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	bgColor[0] = dis(gen);
	bgColor[1] = dis(gen);
	bgColor[2] = dis(gen);
	glColor3f(bgColor[0], bgColor[1], bgColor[2]);
	glRectf(0, 1.0, 1.0, 0); // 1��и� �簢�� �׸���

	bgColor[0] = dis(gen);
	bgColor[1] = dis(gen);
	bgColor[2] = dis(gen);
	glColor3f(bgColor[0], bgColor[1], bgColor[2]);
	glRectf(-1.0, 1.0, 0, 0); // 2��и� �簢�� �׸���
	
	bgColor[0] = dis(gen);
	bgColor[1] = dis(gen);
	bgColor[2] = dis(gen);
	glColor3f(bgColor[0], bgColor[1], bgColor[2]);
	glRectf(-1.0, -1.0, 0, 0); // 3��и�簢�� �׸���
	
	bgColor[0] = dis(gen);
	bgColor[1] = dis(gen);
	bgColor[2] = dis(gen);
	glColor3f(bgColor[0], bgColor[1], bgColor[2]);
	glRectf(0, 0, 1.0, -1.0); // 4��и� �簢�� �׸���

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {	// ���� Ŭ��
		if ((x > 0 && x < 1.0) && (y > 0 && y < 1.0)) {	// 1��и�
			bgColor[0] = dis(gen);
			bgColor[1] = dis(gen);
			bgColor[2] = dis(gen);
			glColor3f(bgColor[0], bgColor[1], bgColor[2]);
			glRectf(0, 1.0, 1.0, 0); // 1��и� �簢�� �׸���
		}
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}