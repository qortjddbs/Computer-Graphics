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

	bool is_contain(float mx, float my) const {
		return mx >= x - width && mx <= x + width && my >= y - height && my <= y + height;
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
}

void addRectangles() {
	rectangles.push_back(RECTANGLE(dis_coord(gen), dis_coord(gen), 0.1f, 0.1f, dis(gen), dis(gen), dis(gen))); // �߾�
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
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
	rectangles[0].draw();
	for (int i = 1; i < rectangles.size(); ++i) {
		rectangles[i].draw();
	}
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		if (rectangles.size() < 10) addRectangles();
		break;

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
		for (int i = rectangles.size() - 1; i > 0; --i) {
			if (rectangles[i].is_contain(mapToGLCoordX(x), mapToGLCoordY(y))) {
				findIndex = i;
				break;
			}
		}
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
		float x1, x2, y1, y2;
		for (int i = rectangles.size() - 1; i > 0; --i) {
			if (i == findIndex) continue;
			if (is_combine(rectangles[i], rectangles[findIndex])) { 
				if (rectangles[i].x - rectangles[i].width < rectangles[findIndex].x - rectangles[findIndex].width) {
					x1 = rectangles[i].x - rectangles[i].width;
				}
				else {
					x1 = rectangles[findIndex].x - rectangles[findIndex].width;
				}

				if (rectangles[i].y + rectangles[i].height > rectangles[findIndex].y + rectangles[findIndex].height) {
					y1 = rectangles[i].y + rectangles[i].height;
				}
				else {
					y1 = rectangles[findIndex].y + rectangles[findIndex].height;
				}

				if (rectangles[i].x + rectangles[i].width > rectangles[findIndex].x + rectangles[findIndex].width) {
					x2 = rectangles[i].x + rectangles[i].width;
				}
				else {
					x2 = rectangles[findIndex].x + rectangles[findIndex].width;
				}

				if (rectangles[i].y - rectangles[i].height < rectangles[findIndex].y - rectangles[findIndex].height) {
					y2 = rectangles[i].y - rectangles[i].height;
				}
				else {
					y2 = rectangles[findIndex].y - rectangles[findIndex].height;
				}

				rectangles.push_back(RECTANGLE((x1 + x2) / 2, (y1 + y2) / 2, (x2 - x1) / 2, (y1 - y2) / 2, dis(gen), dis(gen), dis(gen)));
				if (i < findIndex) {
					rectangles.erase(rectangles.begin() + i);
					rectangles.erase(rectangles.begin() + findIndex - 1);
				}
				else {
					rectangles.erase(rectangles.begin() + i);
					rectangles.erase(rectangles.begin() + findIndex);
				}
				break;
			}
		}
		findIndex = -1;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {	// ��Ŭ��
		// �簢�� �и� ���� ����
		for (int i = 1; i < rectangles.size(); ++i) {
			if (rectangles[i].is_contain(mapToGLCoordX(x), mapToGLCoordY(y))) {
				rectangles.push_back(RECTANGLE(rectangles[i].x + dis_center(gen), rectangles[i].y + dis_center(gen), dis_size(gen), dis_size(gen), dis(gen), dis(gen), dis(gen)));
				rectangles.push_back(RECTANGLE(rectangles[i].x + dis_center(gen), rectangles[i].y + dis_center(gen), dis_size(gen), dis_size(gen), dis(gen), dis(gen), dis(gen)));
				rectangles.erase(rectangles.begin() + i);
				break;
			}
		}
	} 

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Motion(int x, int y) {
	if (left_button == true && findIndex != -1) {
		rectangles[findIndex].x = mapToGLCoordX(x);
		rectangles[findIndex].y = mapToGLCoordY(y);
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}
