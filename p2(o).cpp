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
GLfloat bgColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // �ʱⰪ : ��� 

std::random_device rd;  // �õ尪�� ��� ���� random_device ����.
std::mt19937 gen(rd());	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
std::uniform_real_distribution<float> dis(0.0f, 1.0f); // 0.0f ���� 1.0f ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.

class RECTANGLE {
public:
	float x, y; // �簢���� �߽� ��ǥ
	float width, height; // �簢���� �ʺ�� ����
	float r, g, b; // �簢���� ���� (RGB)

	RECTANGLE(float x, float y, float w, float h, float red, float green, float blue)
		: x(x), y(y), width(w), height(h), r(red), g(green), b(blue) {}

	void draw() {
		glColor3f(r, g, b);
		glRectf(x - width, y + height, x + width, y - height);
	}

	bool is_contain(float mx, float my) const {
		return mx >= x - width && mx <= x + width &&
			my >= y - height && my <= y + height;
	}

	void randomColor() {
		r = dis(gen);
		g = dis(gen);
		b = dis(gen);
	}
};

std::vector<RECTANGLE> rectangles;

float mapToGLCoordX(int x) {
	return (static_cast<float>(x) / (WINDOW_WIDTH / 2)) - 1.0f;
}

float mapToGLCoordY(int y) {
	return 1.0f - (static_cast<float>(y) / (WINDOW_HEIGHT / 2));
}

void initRectangles() {
	rectangles.push_back(RECTANGLE(0.5f, 0.5f, 0.5f, 0.5f, dis(gen), dis(gen), dis(gen))); // ������ ���� �ڿ� ���� (push_back) - 1��и�
	rectangles.push_back(RECTANGLE(-0.5f, 0.5f, 0.5f, 0.5f, dis(gen), dis(gen), dis(gen))); // 2��и�
	rectangles.push_back(RECTANGLE(-0.5f, -0.5f, 0.5f, 0.5f, dis(gen), dis(gen), dis(gen))); // 3��и�
	rectangles.push_back(RECTANGLE(0.5f, -0.5f, 0.5f, 0.5f, dis(gen), dis(gen), dis(gen))); // 4��и�
	rectangles.push_back(RECTANGLE(0.0f, 0.0f, 1.0f, 1.0f, dis(gen), dis(gen), dis(gen))); // �߾�
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
	initRectangles();
	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: ��� �ݹ� �Լ� 
{
	rectangles[4].draw(); // �߾� �簢���� ���� �Ʒ��� �׷�������
	rectangles[0].draw();
	rectangles[1].draw();
	rectangles[2].draw();
	rectangles[3].draw();
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {	// ���� Ŭ��
		if ((Mouse_x > (rectangles[0].x-rectangles[0].width) && Mouse_x < (rectangles[0].x + rectangles[0].width)) && (Mouse_y > (rectangles[0].y - rectangles[0].height) && Mouse_y < rectangles[0].y + rectangles[0].height)) {	// 1��и�
			rectangles[0].randomColor();
		} 
		else if ((Mouse_x > (rectangles[1].x - rectangles[1].width) && Mouse_x < (rectangles[1].x + rectangles[1].width)) && (Mouse_y > (rectangles[1].y - rectangles[1].height) && Mouse_y < rectangles[1].y + rectangles[1].height)) {	// 2��и�
			rectangles[1].randomColor();
		}
		else if ((Mouse_x > (rectangles[2].x - rectangles[2].width) && Mouse_x < (rectangles[2].x + rectangles[2].width)) && (Mouse_y > (rectangles[2].y - rectangles[2].height) && Mouse_y < rectangles[2].y + rectangles[2].height)) {	// 3��и�
			rectangles[2].randomColor();
		}
		else if ((Mouse_x > (rectangles[3].x - rectangles[3].width) && Mouse_x < (rectangles[3].x + rectangles[3].width)) && (Mouse_y > (rectangles[3].y - rectangles[3].height) && Mouse_y < rectangles[3].y + rectangles[3].height)) {	// 4��и�
			rectangles[3].randomColor();
		}
		else {
			rectangles[4].randomColor();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {	// ������ Ŭ��
		if ((Mouse_x > (rectangles[0].x - rectangles[0].width) && Mouse_x < (rectangles[0].x + rectangles[0].width)) && (Mouse_y > (rectangles[0].y - rectangles[0].height) && Mouse_y < rectangles[0].y + rectangles[0].height)) {	// 1��и�
			if (rectangles[0].width > 0.1f && rectangles[0].height > 0.1f) {
				rectangles[0].width -= 0.05f;
				rectangles[0].height -= 0.05f;
			}
		}
		else if ((Mouse_x > (rectangles[1].x - rectangles[1].width) && Mouse_x < (rectangles[1].x + rectangles[1].width)) && (Mouse_y > (rectangles[1].y - rectangles[1].height) && Mouse_y < rectangles[1].y + rectangles[1].height)) {	// 2��и�
			if (rectangles[1].width > 0.1f && rectangles[1].height > 0.1f) {
				rectangles[1].width -= 0.05f;
				rectangles[1].height -= 0.05f;
			}
		}
		else if ((Mouse_x > (rectangles[2].x - rectangles[2].width) && Mouse_x < (rectangles[2].x + rectangles[2].width)) && (Mouse_y > (rectangles[2].y - rectangles[2].height) && Mouse_y < rectangles[2].y + rectangles[2].height)) {	// 3��и�
				if (rectangles[2].width > 0.1f && rectangles[2].height > 0.1f) {
					rectangles[2].width -= 0.05f;
					rectangles[2].height -= 0.05f;
				} 
		}
		else if ((Mouse_x > (rectangles[3].x - rectangles[3].width) && Mouse_x < (rectangles[3].x + rectangles[3].width)) && (Mouse_y > (rectangles[3].y - rectangles[3].height) && Mouse_y < rectangles[3].y + rectangles[3].height)) {	// 4��и�
			if (rectangles[3].width > 0.1f && rectangles[3].height > 0.1f) {
				rectangles[3].width -= 0.05f;
				rectangles[3].height -= 0.05f;
			}
		}
		else if ((Mouse_x > 0 && Mouse_x < 1.0f) && (Mouse_y > 0 && Mouse_y < 1.0f)) {
			if (rectangles[0].width < 1.0f && rectangles[0].height < 1.0f) {
				rectangles[0].width += 0.05f;
				rectangles[0].height += 0.05f;
			}
		}
		else if ((Mouse_x > -1.0f && Mouse_x < 0) && (Mouse_y > 0 && Mouse_y < 1.0f)) {
			if (rectangles[1].width < 1.0f && rectangles[1].height < 1.0f) {
				rectangles[1].width += 0.05f;
				rectangles[1].height += 0.05f;
			}
		}
		else if ((Mouse_x > -1.0f && Mouse_x < 0) && (Mouse_y > -1.0f && Mouse_y < 0)) {
			if (rectangles[2].width < 1.0f && rectangles[2].height < 1.0f) {
				rectangles[2].width += 0.05f;
				rectangles[2].height += 0.05f;
			}
		}
		else if ((Mouse_x > 0 && Mouse_x < 1.0f) && (Mouse_y > -1.0f && Mouse_y < 0)) {
			if (rectangles[3].width < 1.0f && rectangles[3].height < 1.0f) {
				rectangles[3].width += 0.05f;
				rectangles[3].height += 0.05f;
			}
		}
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}