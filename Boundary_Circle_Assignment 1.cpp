#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <windows.h> 

// This is for each constant that we gonna use
const int windowWidth= 700;
const int windowHeight = 700;
const float Pi = 3.14159265359f;

struct Circle {
	float x, y;
	float radiusOfCircle;
	float speedX, speedY;
	bool colourSwap;
	// {Position of circle (in the middle),, Radius/Size of circle, position speed for x vertically, position speed for y horizontally, boolean swap set to false }
} circle = {350.0f, 350.0f, 50.0f, 0.5f * cos(35.0 * Pi / 180.0),  0.5f * sin(35.0 * Pi / 180.0), false };

// Constant for blue line, blue line initial position is in the middle of the windows and the ball should not move 
float blueLineY = windowHeight / 2;
bool isCircleMoving = false;

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_S) {
			isCircleMoving = !isCircleMoving;
		}
		if (key == GLFW_KEY_UP) {
			if (blueLineY + 25 <= windowHeight - 10) {
				blueLineY += 25.0f;
			}
		}
		if (key == GLFW_KEY_DOWN) {
			if (blueLineY - 25 >= 10) {
				blueLineY -= 25.0f;
			}
		}
	}
}


void updateCircle() {
	if (!isCircleMoving) return;

	circle.x += circle.speedX;
	circle.y += circle.speedY;

	if (circle.x - circle.radiusOfCircle <= 0 || circle.x + circle.radiusOfCircle >= windowWidth)
		circle.speedX *= -1;
	if (circle.y - circle.radiusOfCircle <= 0 || circle.y + circle.radiusOfCircle >= windowHeight)
		circle.speedY *= -1;

	if (circle.y - circle.radiusOfCircle <= blueLineY && circle.y + circle.radiusOfCircle >= blueLineY)
		circle.colourSwap = !circle.colourSwap;
}

void renderCircle() {
	float normalX = (circle.x / 350.0f) - 1.0f;
	float normalY = (circle.y / 350.0f) - 1.0f;
	float normalRadius = circle.radiusOfCircle / 350.0f;


	float red, green;
	if (circle.colourSwap) {
		red = 0.0f;
		green = 1.0f;
	}
	else {
		red = 1.0f;
		green = 0.0f;
	}


	glBegin(GL_TRIANGLE_FAN);
	glColor3f(red, green, 0.0f);
	glVertex2f(normalX, normalY);
	for (int i = 0; i <= 100; ++i) {
		float angle = i * 2 * Pi / 100;
		float dx = cos(angle) * normalRadius;
		float dy = sin(angle) * normalRadius;

		if (circle.colourSwap) {
			red = 1.0f;
			green = 0.0f;
		}
		else {
			red = 0.0f;
			green = 1.0f;
		}

		glColor3f(red, green, 0.0f);
		glVertex2f(normalX + dx, normalY + dy);

	}
	glEnd();

}

void renderBlueLine() {
	float normalBlueLineY = (blueLineY / 350.0f) - 1.0f;

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(-0.5f, normalBlueLineY);
	glVertex2f(0.5f, normalBlueLineY);
	glEnd();
}

int main() {
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Bouncing Circle",
		NULL, NULL);
	if (!window) { glfwTerminate(); return -1; }
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetKeyCallback(window, keyCallBack);

	SetConsoleOutputCP(CP_UTF8);
	std::cout << "ESC: To Exit\n";
	std::cout << "S: To Start\n";
	std::cout << "Arrow Key Up: To move the blue line up\n";
	std::cout << "Arrow Key Down: To move the blue line down\n";

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		updateCircle();
		renderCircle();
		renderBlueLine();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
