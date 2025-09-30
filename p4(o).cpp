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
GLvoid TimerFunction(int value);
// GLfloat bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // �ʱⰪ : ��� 
bool button_1 = false;
bool button_2 = false;
bool button_3 = false;
bool button_4 = false;
bool button_5 = false;
bool runningTimer = false;
int train_head = -1;

std::random_device rd;  // �õ尪�� ��� ���� random_device ����.
std::mt19937 gen(rd());	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
std::uniform_real_distribution<float> dis(0.0f, 1.0f); // 0.0f ���� 1.0f ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
std::uniform_real_distribution<float> dis_random(-1.0f, 1.0f);
std::uniform_int_distribution<int> dis_number(1, 5);

class RECTANGLE {
public:
	float x, y; // �簢���� �߽� ��ǥ
	float width, height; // �簢���� �ʺ�� ����
	float r, g, b; // �簢���� ���� (RGB)
	bool growing = dis(gen) > 0.5f ? true : false;
	float dx = dis_random(gen) > 0.0f ? 0.01f : -0.01f;
	float dy = dis_random(gen) > 0.0f ? 0.01f : -0.01f;
	float first_x, first_y;

	RECTANGLE(float x, float y, float w, float h, float red, float green, float blue, float first_x, float first_y)
		: x(x), y(y), width(w), height(h), r(red), g(green), b(blue), first_x(first_x), first_y(first_y) {
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

float mapToGLCoordX(int x) {
	return (static_cast<float>(x) / (WINDOW_WIDTH / 2)) - 1.0f;
}

float mapToGLCoordY(int y) {
	return 1.0f - (static_cast<float>(y) / (WINDOW_HEIGHT / 2));
}

std::vector<RECTANGLE> rectangles;

void MoveToCorner() {
	for (int i = 1; i < rectangles.size(); ++i) {
		if (rectangles[i].x - rectangles[i].width <= -1.0f) {
			rectangles[i].dx = 0.01f;
		} else if (rectangles[i].x + rectangles[i].width >= 1.0f) {
			rectangles[i].dx = -0.01f;
		} 

		if (rectangles[i].y - rectangles[i].height <= -1.0f) {
			rectangles[i].dy = 0.01f;
		} else if (rectangles[i].y + rectangles[i].height >= 1.0f) {
			rectangles[i].dy = -0.01f;
		}
		rectangles[i].x += rectangles[i].dx;
		rectangles[i].y += rectangles[i].dy;
	}
}

void MoveToSide() {
	for (int i = 1; i < rectangles.size(); ++i) {
		if (rectangles[i].x - rectangles[i].width <= -1.0f) {
			rectangles[i].dx = 0.01f;
		}
		else if (rectangles[i].x + rectangles[i].width >= 1.0f) {
			rectangles[i].dx = -0.01f;
		}

		if (rectangles[i].y - rectangles[i].height <= -1.0f) {
			rectangles[i].dy = 0.01f;
		}
		else if (rectangles[i].y + rectangles[i].height >= 1.0f) {
			rectangles[i].dy = -0.01f;
		}
		rectangles[i].x += rectangles[i].dx * 5;
		rectangles[i].y += rectangles[i].dy / 8;
	}
}

void changeSize() {
	for (int i = 1; i < rectangles.size(); ++i) {
		if (rectangles[i].growing) {
			rectangles[i].width += 0.01f;
			rectangles[i].height += 0.01f;
			if (rectangles[i].width >= 0.2f) rectangles[i].growing = false;
		}
		else {
			rectangles[i].width -= 0.01f;
			rectangles[i].height -= 0.01f;
			if (rectangles[i].width <= 0.05f) rectangles[i].growing = true;
		}
	}
}

void changeColor() {
	for (int i = 1; i < rectangles.size(); ++i) {
		rectangles[i].randomColor();
	}
}

void MoveTrain() {
	if (rectangles.size() <= 1 || train_head < 1) return;

	RECTANGLE& head = rectangles[train_head];
	static std::vector<std::pair<float, float>> trail; // �Ӹ��� ������ ����
	const float SPACING = 0.25f; // �簢���� ������ ����

	// �Ӹ� �簢���� ������ (1��, 2�� Ű �Է¿� ����)
	if (button_1) {
		// 1��: �밢������ �����̴� ����
		if (head.x - head.width <= -1.0f) {
			head.dx = 0.01f;
		}
		else if (head.x + head.width >= 1.0f) {
			head.dx = -0.01f;
		}
		if (head.y - head.height <= -1.0f) {
			head.dy = 0.01f;
		}
		else if (head.y + head.height >= 1.0f) {
			head.dy = -0.01f;
		}
		head.x += head.dx;
		head.y += head.dy;
	}
	else if (button_2) {
		// 2��: ���η� ������, ���η� õõ�� �����̴� ����
		if (head.x - head.width <= -1.0f) {
			head.dx = 0.01f;
		}
		else if (head.x + head.width >= 1.0f) {
			head.dx = -0.01f;
		}
		if (head.y - head.height <= -1.0f) {
			head.dy = 0.01f;
		}
		else if (head.y + head.height >= 1.0f) {
			head.dy = -0.01f;
		}
		head.x += head.dx * 5;
		head.y += head.dy / 8;
	}

	// �Ӹ��� ���� ��ġ�� ������ �߰�
	trail.push_back({head.x, head.y});
	
	// ������ �ʹ� ������� ������ ����
	if (trail.size() > 100) {
		trail.erase(trail.begin());
	}

	// ������ �簢������ ������ ���� ��ġ
	int follower_count = 0;
	for (int i = 1; i < rectangles.size(); ++i) {
		if (i != train_head) {
			follower_count++;
			// ���ݿ� ���� ���� �ε��� ���
			int trail_index = trail.size() - 1 - (int)(follower_count * SPACING / 0.02f);
			
			if (trail_index >= 0 && trail_index < trail.size()) {
				rectangles[i].x = trail[trail_index].first;
				rectangles[i].y = trail[trail_index].second;
			}
		}
	}
}

void initBackground() {
	rectangles.push_back(RECTANGLE(0.0f, 0.0f, 1.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.0f, 0.0f)); // ��� background ��׶���
}

void addRectangles(float x, float y) {
	rectangles.push_back(RECTANGLE(x, y, 0.1f, 0.1f, dis(gen), dis(gen), dis(gen), x, y)); // �߾�
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("Practice 4"); // ������ ���� (������ �̸�)

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
	glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid TimerFunction(int value)
{
	if (button_1) MoveToCorner();
	if (button_2) MoveToSide();
	if (button_3) changeSize();
	if (button_4) changeColor();
	if (button_5 && train_head > 0) MoveTrain();
	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(10, TimerFunction, 1); // Ÿ�̸� �Լ� ����
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
	case '1':
		if (!button_1) {
			button_1 = true;
			if (!runningTimer) {
				glutTimerFunc(10, TimerFunction, 1); // Ÿ�̸� �Լ� ����
				runningTimer = true;
			}
		}
		else {
			button_1 = false;
		}
		break;

	case '2':
		if (!button_2) {
			button_2 = true;
			if (!runningTimer) {
				glutTimerFunc(10, TimerFunction, 1); // Ÿ�̸� �Լ� ����
				runningTimer = true;
			}
		}
		else button_2 = false;
		break;

	case '3':
		if (!button_3) {
			button_3 = true;
			for (int i = 1; i < rectangles.size(); ++i) {
				rectangles[i].growing = dis(gen) > 0.5f ? true : false;
			}
			if (!runningTimer) {
				glutTimerFunc(10, TimerFunction, 1); // Ÿ�̸� �Լ� ����
				runningTimer = true;
			}
		}
		else button_3 = false;
		break;

	case '4':
		if (!button_4) {
			button_4 = true;
			if (!runningTimer) {
				glutTimerFunc(10, TimerFunction, 1); // Ÿ�̸� �Լ� ����
				runningTimer = true;
			}
		}
		else button_4 = false;
		break;

	case '5':
		if (!button_5) {
			button_5 = true;
			if (rectangles.size() > 1) {
				// 1���� rectangles.size()-1������ ������ ����
				std::uniform_int_distribution<int> train_dis(1, rectangles.size() - 1);
				train_head = train_dis(gen);
			}
			if (!runningTimer) {
				glutTimerFunc(10, TimerFunction, 1); // Ÿ�̸� �Լ� ����
				runningTimer = true;
			}
		}
		else {
			button_5 = false;
			train_head = -1;
		}
		break;

	case 's':
		for (int i = 1; i < 6; ++i) {
			if (button_1 || button_2 || button_3 || button_4 || button_5) {
				button_1 = false;
				button_2 = false;
				button_3 = false;
				button_4 = false;
				button_5 = false;
			}
		}
		break;

	case 'r':
		rectangles.clear();
		initBackground();
		for (int i = 1; i < 6; ++i) {
			if (button_1 || button_2 || button_3 || button_4 || button_5) {
				button_1 = false;
				button_2 = false;
				button_3 = false;
				button_4 = false;
				button_5 = false;
			}
		}
		break;

	case 'm':
		for (int i = 1; i < rectangles.size(); ++i) {
			rectangles[i].x = rectangles[i].first_x;
			rectangles[i].y = rectangles[i].first_y;
		}
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
		if (rectangles.size() < 6) addRectangles(Mouse_x, Mouse_y);
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}