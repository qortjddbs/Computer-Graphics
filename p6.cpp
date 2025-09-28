#include <iostream>
#include <vector>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLfloat bgColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // �ʱⰪ : ��� 
bool left_button = false;
bool right_button = false;
int findIndex = -1;

std::random_device rd;  // �õ尪�� ��� ���� random_device ����.
std::mt19937 gen(rd());	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
std::uniform_real_distribution<float> dis(0.0f, 1.0f); // 0.0f ���� 1.0f ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
std::uniform_real_distribution<float> dis_coord(-1.0f, 1.0f);
std::uniform_real_distribution<float> dis_center(-0.1f, 0.1f);
std::uniform_real_distribution<float> dis_size(0.05f, 0.1f);

class RECTANGLE {
public:
	float x, y; // �簢���� �߽� ��ǥ
	float width, height; // �簢���� �ʺ�� ����
	float r, g, b; // �簢���� ���� (RGB)

	RECTANGLE(float x, float y, float w, float h, float red, float green, float blue)
		: x(x), y(y), width(w), height(h), r(red), g(green), b(blue) {
	}

	void draw() {
		glColor3f(r, g, b);
		glRectf(x - width, y + height, x + width, y - height);
	}

	void randomColor() {
		r = dis(gen);
		g = dis(gen);
		b = dis(gen);
	}
};

bool is_combine(RECTANGLE& rect1, RECTANGLE& rect2) {
	// �� �簢���� ��ġ���� Ȯ��
	bool overlapX = (rect1.x - rect1.width < rect2.x + rect2.width) && (rect1.x + rect1.width > rect2.x - rect2.width);
	bool overlapY = (rect1.y - rect1.height < rect2.y + rect2.height) && (rect1.y + rect1.height > rect2.y - rect2.height);
	return overlapX && overlapY;
}

float mapToGLCoordX(int x) {
	return (static_cast<float>(x) / (WINDOW_WIDTH / 2)) - 1.0f;
}

float mapToGLCoordY(int y) {
	return 1.0f - (static_cast<float>(y) / (WINDOW_HEIGHT / 2));
}

std::vector<RECTANGLE> rectangles;

void initBackground() {
	rectangles.push_back(RECTANGLE(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)); // �߾�
	for (int i = 0; i < 5; ++i) {
		rectangles.push_back(RECTANGLE(dis_coord(gen), dis_coord(gen), dis_size(gen), dis_size(gen), dis(gen), dis(gen), dis(gen)));
	}
}

void addRectangles(float x, float y) {
	rectangles.push_back(RECTANGLE(x, y, 0.05f, 0.05f, dis(gen), dis(gen), dis(gen))); // �߾�
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("practice 5"); // ������ ���� (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ 
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	initBackground();
	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: ��� �ݹ� �Լ� 
{
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	rectangles[0].draw();
	for (int i = 1; i < rectangles.size(); ++i) {
		rectangles[i].draw();
	}
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
	float Mouse_x = mapToGLCoordX(x);
	float Mouse_y = mapToGLCoordY(y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {	// ��Ŭ��
		left_button = true;
		rectangles[1].x = mapToGLCoordX(x);
		rectangles[1].y = mapToGLCoordY(y);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
		rectangles[1].r = 0.0f;
		rectangles[1].g = 0.0f;
		rectangles[1].b = 0.0f;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {	// ��Ŭ��
		if (rectangles.size() < 30) {
			addRectangles(Mouse_x, Mouse_y);
			rectangles[1].width -= 0.02f;
			rectangles[1].height -= 0.02f;
		}
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Motion(int x, int y) {
	if (left_button == true) {
		rectangles[1].x = mapToGLCoordX(x);
		rectangles[1].y = mapToGLCoordY(y);
	}

	if (left_button == true) {
		for (int i = rectangles.size() - 1; i > 1; --i) {
			// ���찳(rectangles[1])�� �ٸ� �簢��(rectangles[i])�� ��ġ���� üũ
			if (is_combine(rectangles[1], rectangles[i])) {
				rectangles[1].r = rectangles[i].r;
				rectangles[1].g = rectangles[i].g;
				rectangles[1].b = rectangles[i].b;
				rectangles.erase(rectangles.begin() + i);
				rectangles[1].width += 0.02f;
				rectangles[1].height += 0.02f;
			}
		}
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}