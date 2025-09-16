#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
bool timer = false;

std::random_device rd;  // �õ尪�� ��� ���� random_device ����.
std::mt19937 gen(rd());	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
std::uniform_real_distribution<float> dis(0.0f, 1.0f); // 0.0f ���� 1.0f ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
		//--- ������ �����ϱ�
		glutInit(&argc, argv); //--- glut �ʱ�ȭ
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //--- ���÷��� ��� ����
		glutInitWindowPosition(0, 0); //--- �������� ��ġ ����
		glutInitWindowSize(800, 600); //--- �������� ũ�� ����
		glutCreateWindow("Practice 1"); //--- ������ ���� (������ �̸�)
		//--- GLEW �ʱ�ȭ�ϱ�
		glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) //--- glew �ʱ�ȭ 
	{
	std::cerr << "Unable to initialize GLEW" << std::endl;
	exit(EXIT_FAILURE);
	}
else
	std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); //--- ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); //--- �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
	glutMainLoop(); //--- �̺�Ʈ ó�� ����
}

// ���� ������ ���� ����
GLfloat bgColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // �ʱⰪ : ��� 

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid TimerFunction(int value) // Ÿ�̸� �ݹ� �Լ�
{
	if (!timer) return; // Ÿ�̸Ӱ� ��Ȱ��ȭ �����̸� �Լ� ����
	bgColor[0] = dis(gen);
	bgColor[1] = dis(gen);
	bgColor[2] = dis(gen);
	bgColor[3] = 1.0f;
	glutPostRedisplay(); // ȭ�� ���� ��û
	glutTimerFunc(100, TimerFunction, 1); // 100�и��� �Ŀ� �ٽ� Ÿ�̸� �Լ� ȣ��
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c': //--- ������ û�ϻ����� ����
		bgColor[0] = 0.0f;
		bgColor[1] = 1.0f;
		bgColor[2] = 1.0f;
		bgColor[3] = 1.0f;
		break; 

	case 'm':
		bgColor[0] = 1.0f;
		bgColor[1] = 0.0f;
		bgColor[2] = 1.0f;
		bgColor[3] = 1.0f;
		break; //--- ������ ��ȫ������ ����

	case 'y':
		bgColor[0] = 1.0f;
		bgColor[1] = 1.0f;
		bgColor[2] = 0.0f;
		bgColor[3] = 1.0f;
		break; //--- ������ ��������� ����

	case 'a':
		bgColor[0] = dis(gen);
		bgColor[1] = dis(gen);
		bgColor[2] = dis(gen);
		bgColor[3] = 1.0f;
		break;

	case 'w':
		bgColor[0] = 1.0f;
		bgColor[1] = 1.0f;
		bgColor[2] = 1.0f;
		bgColor[3] = 1.0f;
		break;

	case 'k':
		bgColor[0] = 0.0f;
		bgColor[1] = 0.0f;
		bgColor[2] = 0.0f;
		bgColor[3] = 1.0f;
		break;
		
	case 't':		// �ٵ� ���������� Ÿ�̸Ӱ� ������.
		timer = true;
		glutTimerFunc(100, TimerFunction, 1); // Ÿ�̸� �Լ� ����
		break;

	case 's':
		timer = false;
		break;

	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}