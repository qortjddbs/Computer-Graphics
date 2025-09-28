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
GLfloat bgColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // �ʱⰪ : ��� 
bool left_button = false;
int findIndex = -1;
bool animationRunning = false;

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
	float dx, dy; // �̵� ����
	bool isMoving; // �����̴� ��������
	float shrinkRate; // ��� ����

	RECTANGLE(float x, float y, float w, float h, float red, float green, float blue)
		: x(x), y(y), width(w), height(h), r(red), g(green), b(blue), dx(0), dy(0), isMoving(false), shrinkRate(0.001f) {
	}

	RECTANGLE(float x, float y, float w, float h, float red, float green, float blue, float dx, float dy)
		: x(x), y(y), width(w), height(h), r(red), g(green), b(blue), dx(dx), dy(dy), isMoving(true), shrinkRate(0.001f) {
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

	void update() {
		if (isMoving) {
			x += dx;
			y += dy;
			width -= shrinkRate;
			height -= shrinkRate;
			
			// ȭ�� ��迡�� �ݻ�
			// if (x - width <= -1.0f || x + width >= 1.0f) {
			// 	dx = -dx;
			// }
			// if (y - height <= -1.0f || y + height >= 1.0f) {
			// 	dy = -dy;
			// }
		}
	}

	bool shouldRemove() const {
		return isMoving && (width <= 0.0001f || height <= 0.0001f);
	}
};

float mapToGLCoordX(int x) {
	return (static_cast<float>(x) / (WINDOW_WIDTH / 2)) - 1.0f;
}

float mapToGLCoordY(int y) {
	return 1.0f - (static_cast<float>(y) / (WINDOW_HEIGHT / 2));
}

std::vector<RECTANGLE> rectangles;

void initBackground() {
	rectangles.push_back(RECTANGLE(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)); // ��� background ��׶���
	for (int i = 0; i < 10; ++i) {
		rectangles.push_back(RECTANGLE(dis_coord(gen), dis_coord(gen), dis_size(gen), dis_size(gen), dis(gen), dis(gen), dis(gen)));
	}
}

void addRectangles(float x, float y, float width, float height, float r, float g, float b) {
	rectangles.push_back(RECTANGLE(x, y, width, height, r, g, b)); // �߾�
}

void addMovingRectangles(float centerX, float centerY, float width, float height, float r, float g, float b) {
	float speed = 0.01f;
	float newWidth = width * 0.3f;
	float newHeight = height * 0.3f;
	
	// 8�������� ���� �簢���� ���� (�����¿� + �밢��)
	rectangles.push_back(RECTANGLE(centerX, centerY + height * 0.3f, newWidth, newHeight, r, g, b, 0, speed));        // ��
	rectangles.push_back(RECTANGLE(centerX, centerY - height * 0.3f, newWidth, newHeight, r, g, b, 0, -speed));       // �Ʒ�
	rectangles.push_back(RECTANGLE(centerX - width * 0.3f, centerY, newWidth, newHeight, r, g, b, -speed, 0));        // ����
	rectangles.push_back(RECTANGLE(centerX + width * 0.3f, centerY, newWidth, newHeight, r, g, b, speed, 0));         // ������
	rectangles.push_back(RECTANGLE(centerX - width * 0.2f, centerY + height * 0.2f, newWidth, newHeight, r, g, b, -speed * 0.7f, speed * 0.7f)); // ���� ��
	rectangles.push_back(RECTANGLE(centerX + width * 0.2f, centerY + height * 0.2f, newWidth, newHeight, r, g, b, speed * 0.7f, speed * 0.7f));  // ������ ��
	rectangles.push_back(RECTANGLE(centerX - width * 0.2f, centerY - height * 0.2f, newWidth, newHeight, r, g, b, -speed * 0.7f, -speed * 0.7f)); // ���� �Ʒ�
	rectangles.push_back(RECTANGLE(centerX + width * 0.2f, centerY - height * 0.2f, newWidth, newHeight, r, g, b, speed * 0.7f, -speed * 0.7f));  // ������ �Ʒ�
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("practice 6"); // ������ ���� (������ �̸�)

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
	glutTimerFunc(16, TimerFunction, 1); // Ÿ�̸� �Լ� ���� (�� 60FPS)
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
		for (int i = 1; i < rectangles.size(); ++i) {
			if (rectangles[i].is_contain(Mouse_x, Mouse_y) && !rectangles[i].isMoving) {
				// Ŭ���� �簢���� 8���� ���� �簢������ �����ϰ� �۶߸���
				addMovingRectangles(rectangles[i].x, rectangles[i].y, rectangles[i].width, rectangles[i].height, rectangles[i].r, rectangles[i].g, rectangles[i].b);
				rectangles.erase(rectangles.begin() + i);
				animationRunning = true;
				break;
			}
		}
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Motion(int x, int y) {

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid TimerFunction(int value) {
	if (animationRunning) {
		// ��� �簢�� ������Ʈ
		for (auto& rect : rectangles) {
			rect.update();
		}
		
		// �ʹ� �۾��� �簢���� ����
		rectangles.erase(
			std::remove_if(rectangles.begin() + 1, rectangles.end(), 
				[](const RECTANGLE& rect) { return rect.shouldRemove(); }),
			rectangles.end()
		);
		
		// �����̴� �簢���� ������ �ִϸ��̼� ����
		bool hasMovingRect = false;
		for (const auto& rect : rectangles) {
			if (rect.isMoving) {
				hasMovingRect = true;
				break;
			}
		}
		animationRunning = hasMovingRect;
	}
	
	glutPostRedisplay();
	glutTimerFunc(16, TimerFunction, 1); // ���� �������� ���� Ÿ�̸� ����
}