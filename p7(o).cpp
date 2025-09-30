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
int findIndex = -1;
float initial_mouse_x, initial_mouse_y;
float initial_rect_x, initial_rect_y;

std::random_device rd;  // �õ尪�� ��� ���� random_device ����.
std::mt19937 gen(rd());	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
std::uniform_real_distribution<float> dis(0.0f, 1.0f); // 0.0f ���� 1.0f ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
std::uniform_real_distribution<float> dis_coord(-1.0f, 1.0f);
std::uniform_real_distribution<float> dis_center(-0.1f, 0.1f);
std::uniform_real_distribution<float> dis_size(0.05f, 0.1f);
std::uniform_real_distribution<float> dis_right_x(0.0f, 1.0f); // ������ ������ x ��ǥ ����
std::uniform_real_distribution<float> dis_y_pos(-1.0f, 1.0f); // y ��ǥ ����

class RECTANGLE {
public:
	float x, y; // �簢���� �߽� ��ǥ
	float width, height; // �簢���� �ʺ�� ����
	float r, g, b; // �簢���� ���� (RGB)
	bool fillMode;
	bool isCombined; // ���յ� �簢������ ��Ÿ���� �÷���

	RECTANGLE(float x, float y, float w, float h, float red, float green, float blue, bool fill = true)
		: x(x), y(y), width(w), height(h), r(red), g(green), b(blue), fillMode(fill), isCombined(false) {
	}

	void draw() {
		glColor3f(r, g, b);

		if (fillMode) {
			glRectf(x - width, y + height, x + width, y - height);
		}
		else {
			glBegin(GL_LINE_LOOP);
			glVertex2f(x - width, y + height);
			glVertex2f(x + width, y + height);
			glVertex2f(x + width, y - height);
			glVertex2f(x - width, y - height);
			glEnd();
		}
		
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
	// ���� ��� ����
	rectangles.push_back(RECTANGLE(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f)); // ���� ���

	// �̹����� ���� ������ �������� ���� ����
	// ��� �簢������ �� �簢��(�׵θ���)���� ����
	
	// === ��� ���� �簢���� (�� �簢��) ===
	rectangles.push_back(RECTANGLE(-0.7f, 0.75f, 0.1f, 0.1f, 0.7f, 0.3f, 1.0f, false));    // ���� �Ķ�
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.1f, dis_y_pos(gen), 0.1f, 0.1f, 0.7f, 0.3f, 1.0f, true));    // ���� �Ķ� - ä����

	rectangles.push_back(RECTANGLE(-0.5f, 0.8f, 0.1f, 0.15f, 0.0f, 1.0f, 0.0f, false)); 
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.1f, dis_y_pos(gen), 0.1f, 0.15f, 0.0f, 1.0f, 0.0f, true));   // �ʷ� - ä����

	rectangles.push_back(RECTANGLE(-0.3f, 0.75f, 0.1f, 0.1f, 0.1f, 0.5f, 0.0f, false)); 
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.1f, dis_y_pos(gen), 0.1f, 0.1f, 0.1f, 0.5f, 0.0f, true));    // ��Ȳ - ä����

	rectangles.push_back(RECTANGLE(-0.1f, 0.8f, 0.1f, 0.15f, 0.4f, 0.1f, 1.0f, false));
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.1f, dis_y_pos(gen), 0.1f, 0.15f, 0.4f, 0.1f, 1.0f, true));   // �Ķ� - ä����

	// === �߾��� ū ���� �簢�� (�� �簢��) ===
	rectangles.push_back(RECTANGLE(-0.4f, 0.6f, 0.35f, 0.05f, 0.7f, 0.7f, 1.0f, false));  //�� ���� �簢��
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.35f, dis_y_pos(gen), 0.35f, 0.05f, 0.7f, 0.7f, 1.0f, true)); // ���� �Ķ� - �� ���� ä����

	// === �߾� ���� �簢���� (�� �簢��) ===
	rectangles.push_back(RECTANGLE(-0.6f, 0.2f, 0.03f, 0.35f, 1.0f, 0.5f, 1.0f, false));  //���� ����
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.03f, dis_y_pos(gen), 0.03f, 0.35f, 1.0f, 0.5f, 1.0f, true)); // �Ķ� - ���� ���� ä����

	rectangles.push_back(RECTANGLE(-0.5f, 0.2f, 0.03f, 0.35f, 0.2f, 0.5f, 0.2f, false));  // ���� ����
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.03f, dis_y_pos(gen), 0.03f, 0.35f, 0.2f, 0.5f, 0.2f, true)); // �Ķ� - ���� ���� ä����

	rectangles.push_back(RECTANGLE(-0.4f, 0.2f, 0.03f, 0.35f, 0.7f, 0.5f, 0.8f, false));  // ���� ����
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.03f, dis_y_pos(gen), 0.03f, 0.35f, 0.7f, 0.5f, 0.8f, true)); // �Ķ� - ���� ���� ä����

	rectangles.push_back(RECTANGLE(-0.3f, 0.2f, 0.03f, 0.35f, 0.5f, 0.5f, 0.8f, false));  // ���� ����
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.03f, dis_y_pos(gen), 0.03f, 0.35f, 0.5f, 0.5f, 0.8f, true)); // �Ķ� - ���� ���� ä����

	// === ���� ū �簢�� (�� �簢��) ===
	rectangles.push_back(RECTANGLE(-0.78f, 0.0f, 0.15f, 0.25f, 1.0f, 0.5f, 0.0f, false));  // ��Ȳ - ū �簢��
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.15f, dis_y_pos(gen), 0.15f, 0.25f, 1.0f, 0.5f, 0.0f, true)); // ��Ȳ - ū �簢�� ä����

	// === �߰����� ��Ŀ� ���� �簢���� (�� �簢��) ===
	rectangles.push_back(RECTANGLE(-0.1f, 0.47f, 0.06f, 0.08f, 1.0f, 1.0f, 0.0f, false));  // ��� - ���� ���
	rectangles.push_back(RECTANGLE(dis_right_x(gen) + 0.06f, dis_y_pos(gen), 0.06f, 0.08f, 1.0f, 1.0f, 0.0f, true)); // ��� - ���� ��� ä����

}

void addRectangles(float x, float y, float width, float height, float r, float g, float b) {
	RECTANGLE combinedRect(x, y, width, height, r, g, b);
	combinedRect.isCombined = true; // ���յ� �簢������ ǥ��
	rectangles.push_back(combinedRect);
	std::cout << "���� ! " << std::endl;
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("Practice 7"); // ������ ���� (������ �̸�)

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
	case 'r':
		rectangles.clear();
		initBackground();
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
		findIndex = -1; // �ʱ�ȭ
		for (int i = rectangles.size() - 1; i > 0; --i) {
			if (rectangles[i].is_contain(mapToGLCoordX(x), mapToGLCoordY(y)) && rectangles[i].fillMode && !rectangles[i].isCombined) {
				findIndex = i;
				// �ʱ� ��ġ�� ����
				initial_mouse_x = Mouse_x;
				initial_mouse_y = Mouse_y;
				initial_rect_x = rectangles[i].x;
				initial_rect_y = rectangles[i].y;
				break;
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
		if (findIndex > 0) {
			if (is_combine(rectangles[findIndex], rectangles[findIndex - 1])) {
				addRectangles(rectangles[findIndex - 1].x, rectangles[findIndex - 1].y, rectangles[findIndex - 1].width, rectangles[findIndex - 1].height, rectangles[findIndex - 1].r, rectangles[findIndex - 1].g, rectangles[findIndex - 1].b);
				rectangles.erase(rectangles.begin() + findIndex);
				rectangles.erase(rectangles.begin() + findIndex - 1);
			}
		}
		findIndex = -1;
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Motion(int x, int y) {
	if (left_button == true && findIndex != -1) {
		// ���콺 �̵� �Ÿ� ���
		float mouse_x = mapToGLCoordX(x);
		float mouse_y = mapToGLCoordY(y);
		float delta_x = mouse_x - initial_mouse_x;
		float delta_y = mouse_y - initial_mouse_y;
		
		// �ʱ� �簢�� ��ġ���� ���콺 �̵� �Ÿ���ŭ �̵�
		rectangles[findIndex].x = initial_rect_x + delta_x;
		rectangles[findIndex].y = initial_rect_y + delta_y;
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}
