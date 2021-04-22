#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2.h>

#include "VertexBuffer.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const glm::mat4 unit_matrix(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void do_movement();

unsigned int loadTexture(char const * path);

const GLuint WIDTH = 1920, HEIGHT = 1080;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat fov = 45.0f;
bool keys[1024];
GLfloat cameraSpeed = 6.0f;

bool flashlightSwitch = false;

GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;  

glm::vec3 lightPos(1.0f, 1.0f, 2.0f);

struct CubeLocator
{
	glm::vec3 translate = glm::vec3(0.0f, 0.0f, -30.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f);
};

CubeLocator cubes[65];

//int cubeIndx = 57;

int countdown = 0;

int main()
{
	glfwInit();

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);
	


	//GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);


	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window ,scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	glfwSetCursorPos(window, lastX, lastY);

	// Smooth animation 
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;

	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);
	// Show version
	std::cout << glGetString(GL_VERSION) << std::endl << std::endl;

	Shader texturedShader("C:\\dev\\2 course\\ÑourseProject\\ÑourseProject\\RES\\shaders\\general_V.shader", "C:\\dev\\2 course\\ÑourseProject\\ÑourseProject\\RES\\shaders\\with_texture_F.shader");
	Shader nontexturedShader("C:\\dev\\2 course\\ÑourseProject\\ÑourseProject\\RES\\shaders\\general_V.shader", "C:\\dev\\2 course\\ÑourseProject\\ÑourseProject\\RES\\shaders\\no_texture_F.shader");
	Shader lampShader("C:\\dev\\2 course\\ÑourseProject\\ÑourseProject\\RES\\shaders\\lightbulb_V.shader", "C:\\dev\\2 course\\ÑourseProject\\ÑourseProject\\RES\\shaders\\lightbulb_F.shader");

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 spotLightPositions[] = {
		glm::vec3(-15.4803 + 0.14, 7.63008 - 0.32, -25.5899 - 0.5),
		glm::vec3(-8.4201 + 0.12, 7.56008 - 0.34, -25.5099 - 0.45),
		glm::vec3(0.04 , 7.70008 - 0.35, -25.5199 - 0.47),
		glm::vec3(8.16009 - 0.07, 7.68008 - 0.32, -25.6399 - 0.5),
		glm::vec3(14.5502 - 0.12, 7.72008 - 0.32, -25.5999 - 0.5)
	};

	glm::vec3 spotLightRotations[] = {
		glm::vec3(-0.61, -0.26, 0),
		glm::vec3(-0.65, -0.17, 0),
		glm::vec3(-0.71, 0, 0),
		glm::vec3(-0.63, 0.16, 0),
		glm::vec3(-0.55, 0.24, 0)
	};
	
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	VertexBuffer* VBO = new VertexBuffer(vertices, sizeof(vertices));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	VBO->Unbind();
	glBindVertexArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	VBO->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	VBO->Unbind();
	glBindVertexArray(0);

	unsigned int stageTxtr = loadTexture("C:\\dev\\2 course\\ÑourseProject\\ÑourseProject\\RES\\images\\Wood_plank_007_COLOR.png");
	unsigned int curtainTxtr = loadTexture("C:\\dev\\2 course\\ÑourseProject\\ÑourseProject\\RES\\images\\Fabric.jpg");

#pragma region MyRegion

	int sx = 11;
	int sz = 31;
	// K
	cubes[0].translate = glm::vec3(-4.5f - sx, 2.5f, - sz);
	cubes[0].scale = glm::vec3(1.0f, 5.0f, 1.0f);

	cubes[1].translate = glm::vec3(-3.14f - sx, 1.64, - sz);
	cubes[1].scale = glm::vec3(1, 3.51998, 0.97);
	cubes[1].rotate = glm::vec3(0, 0, 0.77);

	cubes[2].translate = glm::vec3(-3.14f - sx, 3.49, - sz);
	cubes[2].scale = glm::vec3(1, 3.51998, 0.95);
	cubes[2].rotate = glm::vec3(0, 0, -0.82);
	// Ó
	cubes[3].translate = glm::vec3(1.12005 - sx, 2.5, - sz);
	cubes[3].scale = glm::vec3(1, 5.35001, 1);
	cubes[3].rotate = glm::vec3(0, 0, -0.53);

	cubes[4].translate = glm::vec3(0.08002 - sx, 3.39, - sz);
	cubes[4].scale = glm::vec3(1, 3.41998, 0.96);
	cubes[4].rotate = glm::vec3(0, 0, 0.5);
	// Ð
	cubes[5].translate = glm::vec3(3.69 - sx, 2.5, - sz);
	cubes[5].scale = glm::vec3(1, 5, 1);
	cubes[5].rotate = glm::vec3(0, 0, 0);

	cubes[6].translate = glm::vec3(4.84002 - sx, 2.51, - sz);
	cubes[6].scale = glm::vec3(1, 1.30998, 1);
	cubes[6].rotate = glm::vec3(0, 0, 1.57);

	cubes[7].translate = glm::vec3(5.73004 - sx, 2.73, - sz);
	cubes[7].scale = glm::vec3(1, 1.10998, 0.97);
	cubes[7].rotate = glm::vec3(0, 0, -1.04);

	cubes[8].translate = glm::vec3(5.73004 - sx, 2.73, - sz);
	cubes[8].scale = glm::vec3(1, 1.10998, 0.97);
	cubes[8].rotate = glm::vec3(0, 0, -1.04);

	cubes[9].translate = glm::vec3(4.82002 - sx, 4.48001, - sz);
	cubes[9].scale = glm::vec3(1.01, 1.33998, 0.99);
	cubes[9].rotate = glm::vec3(0, 0, 1.57);

	cubes[10].translate = glm::vec3(5.72004 - sx, 4.24, - sz);
	cubes[10].scale = glm::vec3(1.01, 1.18998, 0.97);
	cubes[10].rotate = glm::vec3(0, 0, 0.999999);

	cubes[11].translate = glm::vec3(6.03004 - sx, 3.45, - sz);
	cubes[11].scale = glm::vec3(0.89, 1.77998, 0.95);
	cubes[11].rotate = glm::vec3(0, 0, 0);
	// Ñ
	cubes[12].translate = glm::vec3(7.30008 - sx, 2.5, - sz);
	cubes[12].scale = glm::vec3(1, 3.07998, 1);
	cubes[12].rotate = glm::vec3(0, 0, 0);

	cubes[13].translate = glm::vec3(7.60009 - sx, 0.79, - sz);
	cubes[13].scale = glm::vec3(1, 1.29998, 0.96);
	cubes[13].rotate = glm::vec3(0, 0, -2.46);

	cubes[14].translate = glm::vec3(7.60009 - sx, 4.21, - sz);
	cubes[14].scale = glm::vec3(1, 1.29998, 0.96);
	cubes[14].rotate = glm::vec3(0, 0, -0.69);

	cubes[15].translate = glm::vec3(8.6 - sx, 4.52, - sz);
	cubes[15].scale = glm::vec3(1, 1.94998, 0.92);
	cubes[15].rotate = glm::vec3(0, 0, -1.58);

	cubes[16].translate = glm::vec3(8.62011 - sx, 0.44, - sz);
	cubes[16].scale = glm::vec3(1, 1.95998, 0.94);
	cubes[16].rotate = glm::vec3(0, 0, -1.6);
	// Î
	cubes[17].translate = glm::vec3(10.80008 - sx, 2.5, - sz);
	cubes[17].scale = glm::vec3(1, 3.07998, 1);
	cubes[17].rotate = glm::vec3(0, 0, 0);

	cubes[18].translate = glm::vec3(11.10009 - sx, 0.79, - sz);
	cubes[18].scale = glm::vec3(1, 1.29998, 0.96);
	cubes[18].rotate = glm::vec3(0, 0, -2.46);

	cubes[19].translate = glm::vec3(11.10009 - sx, 4.21, - sz);
	cubes[19].scale = glm::vec3(1, 1.29998, 0.96);
	cubes[19].rotate = glm::vec3(0, 0, -0.69);

	cubes[20].translate = glm::vec3(12.10 - sx, 4.53, - sz);
	cubes[20].scale = glm::vec3(1, 1.94998, 0.92);
	cubes[20].rotate = glm::vec3(0, 0, -1.57);

	cubes[21].translate = glm::vec3(12.105 - sx, 0.47, - sz);
	cubes[21].scale = glm::vec3(1, 1.95998, 0.94);
	cubes[21].rotate = glm::vec3(0, 0, -1.57);

	cubes[22].translate = glm::vec3(13.10009 - sx, 0.79, - sz);
	cubes[22].scale = glm::vec3(1, 1.29998, 0.96);
	cubes[22].rotate = glm::vec3(0, 0, 2.46);

	cubes[23].translate = glm::vec3(13.10009 - sx, 4.21, - sz);
	cubes[23].scale = glm::vec3(1, 1.29998, 0.96);
	cubes[23].rotate = glm::vec3(0, 0, 0.69);

	cubes[24].translate = glm::vec3(13.40008 - sx, 2.5, - sz);
	cubes[24].scale = glm::vec3(1, 3.07998, 1);
	cubes[24].rotate = glm::vec3(0, 0, 0);
	// Â
	cubes[25].translate = glm::vec3(14.84 - sx, 2.5, - sz);
	cubes[25].scale = glm::vec3(1, 5, 1);
	cubes[25].rotate = glm::vec3(0, 0, 0);

	cubes[26].translate = glm::vec3(15.84002 - sx, 2.51, - sz);
	cubes[26].scale = glm::vec3(1, 1.30998, 0.99);
	cubes[26].rotate = glm::vec3(0, 0, 1.57);

	cubes[27].translate = glm::vec3(16.73004 - sx, 2.73, - sz);
	cubes[27].scale = glm::vec3(1, 1.10998, 0.97);
	cubes[27].rotate = glm::vec3(0, 0, -1.04);

	cubes[28].translate = glm::vec3(15.82002 - sx, 4.48001, - sz);
	cubes[28].scale = glm::vec3(1.01, 1.33998, 0.99);
	cubes[28].rotate = glm::vec3(0, 0, 1.57);

	cubes[29].translate = glm::vec3(16.72004 - sx, 4.24, - sz);
	cubes[29].scale = glm::vec3(1.01, 1.18998, 0.97);
	cubes[29].rotate = glm::vec3(0, 0, 0.999999);

	cubes[30].translate = glm::vec3(17.03004 - sx, 3.45, - sz);
	cubes[30].scale = glm::vec3(0.89, 1.77998, 0.95);
	cubes[30].rotate = glm::vec3(0, 0, 0);

	cubes[31].translate = glm::vec3(15.84002 - sx, 0.51, - sz);
	cubes[31].scale = glm::vec3(1, 1.30998, 0.99);
	cubes[31].rotate = glm::vec3(0, 0, 1.57);

	cubes[32].translate = glm::vec3(16.73004 - sx, 0.73, - sz);
	cubes[32].scale = glm::vec3(1, 1.10998, 0.97);
	cubes[32].rotate = glm::vec3(0, 0, -1.04);

	cubes[33].translate = glm::vec3(16.72004 - sx, 2.24, - sz);
	cubes[33].scale = glm::vec3(1.01, 1.18998, 0.96);
	cubes[33].rotate = glm::vec3(0, 0, 0.999999);

	cubes[34].translate = glm::vec3(17.03004 - sx, 1.45, - sz);
	cubes[34].scale = glm::vec3(0.89, 1.77998, 0.95);
	cubes[34].rotate = glm::vec3(0, 0, 0);
	// A
	cubes[35].translate = glm::vec3(19.4 - sx, 2.5, - sz);
	cubes[35].scale = glm::vec3(1, 4.88, 1);
	cubes[35].rotate = glm::vec3(0, 0, -0.41);

	cubes[36].translate = glm::vec3(20.56 - sx, 2.54, - sz);
	cubes[36].scale = glm::vec3(1, 4.88, 0.99);
	cubes[36].rotate = glm::vec3(0, 0, 0.47);

	cubes[37].translate = glm::vec3(19.95 - sx, 1.48, - sz);
	cubes[37].scale = glm::vec3(1, 2.29998, 0.98);
	cubes[37].rotate = glm::vec3(0, 0, -1.57);
	// ß
	cubes[38].translate = glm::vec3(25.5601 - sx, 2.5, - sz);
	cubes[38].scale = glm::vec3(1, 5, 1);
	cubes[38].rotate = glm::vec3(0, 0, 0);

	cubes[39].translate = glm::vec3(24.41012 - sx, 4.49001, - sz);
	cubes[39].scale = glm::vec3(1, 1.33998, 0.97);
	cubes[39].rotate = glm::vec3(0, 0, -1.57);

	cubes[40].translate = glm::vec3(24.41012 - sx, 2.53, - sz);
	cubes[40].scale = glm::vec3(1, 1.33998, 0.99);
	cubes[40].rotate = glm::vec3(0, 0, -1.58);

	cubes[41].translate = glm::vec3(23.22009 - sx, 3.47, - sz);
	cubes[41].scale = glm::vec3(1, 1.33998, 0.9);
	cubes[41].rotate = glm::vec3(0, 0, 0);

	cubes[42].translate = glm::vec3(23.5601 - sx, 4.18, - sz);
	cubes[42].scale = glm::vec3(1, 1.31998, 0.93);
	cubes[42].rotate = glm::vec3(0, 0, -0.869999);

	cubes[43].translate = glm::vec3(23.5401 - sx, 2.81, - sz);
	cubes[43].scale = glm::vec3(1, 1.28998, 0.91);
	cubes[43].rotate = glm::vec3(0, 1.49012e-08, 0.919999);

	cubes[44].translate = glm::vec3(24.03011 - sx, 1.32, - sz);
	cubes[44].scale = glm::vec3(1, 2.80998, 0.91);
	cubes[44].rotate = glm::vec3(0, 0, -0.78);
	// Scene
	cubes[45].translate = glm::vec3(10.52 - sx, -2.56, 0.31 - sz);
	cubes[45].scale = glm::vec3(39.1288, 5, 12.0202);
	cubes[45].rotate = glm::vec3(0, 0, 0);

	cubes[46].translate = glm::vec3(10.59 - sx, 4.05, -5.1593 - sz);
	cubes[46].scale = glm::vec3(38.8789, 8.72011, 0.400001);
	cubes[46].rotate = glm::vec3(0, 0, 0);

	cubes[47].translate = glm::vec3(10.53 - sx, 8.00009, 5.87 - sz);
	cubes[47].scale = glm::vec3(38.8789, 0.66, 0.68);
	cubes[47].rotate = glm::vec3(0, 0, 0);

	cubes[48].translate = glm::vec3(-19.5804, 3.83, -25.1399);
	cubes[48].scale = glm::vec3(0.55, 7.88009, 0.62);
	cubes[48].rotate = glm::vec3(0, 0, 0);

	cubes[49].translate = glm::vec3(18.6103, 3.83, -25.1599);
	cubes[49].scale = glm::vec3(0.55, 7.83008, 0.66);
	cubes[49].rotate = glm::vec3(0, 0, 0);

	cubes[50].translate = glm::vec3(18.5603, 8.00009, -30.69);
	cubes[50].scale = glm::vec3(0.66, 0.64, 10.5601);
	cubes[50].rotate = glm::vec3(0, 0, 0);

	cubes[51].translate = glm::vec3(-19.5304, 7.98009, -30.9);
	cubes[51].scale = glm::vec3(0.61, 0.64, 10.8702);
	cubes[51].rotate = glm::vec3(0, 0, 0);
	// Spotlights
	cubes[52].translate = glm::vec3(-15.4803, 7.63008, -25.5899);
	cubes[52].scale = glm::vec3(1.16, 1.09, 1.5);
	cubes[52].rotate = glm::vec3(-0.61, -0.26, 0);

	cubes[53].translate = glm::vec3(-8.4201, 7.56008, -25.5099);
	cubes[53].scale = glm::vec3(1.16, 1.09, 1.47);
	cubes[53].rotate = glm::vec3(-0.65, -0.17, 0);

	cubes[54].translate = glm::vec3(0.04, 7.70008, -25.5199);
	cubes[54].scale = glm::vec3(1.16, 1.08, 1.47);
	cubes[54].rotate = glm::vec3(-0.71, 0, 0);

	cubes[55].translate = glm::vec3(8.16009, 7.68008, -25.6399);
	cubes[55].scale = glm::vec3(1.16, 1.09, 1.47);
	cubes[55].rotate = glm::vec3(-0.63, 0.16, 0.0);

	cubes[56].translate = glm::vec3(14.5502, 7.72008, -25.5999);
	cubes[56].scale = glm::vec3(1.16, 1.09, 1.5);
	cubes[56].rotate = glm::vec3(-0.55, 0.24, 0);


#pragma endregion


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		do_movement();
		if(countdown > 300) glClearColor(0.3f, 0.4f, 0.4f, 1.0f);
		else glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
		glm::mat4 model = unit_matrix;


		glm::vec3 spotAmbient(0.0f, 0.0f, 0.0f);
		glm::vec3 spotDiffuse(2.0f, 2.0f, 2.0f);
		glm::vec3 spotSpecular(1.0f, 1.0f, 1.0f);
		float spotConst = 1.0f, spotLinear = 0.045f, spotQuadratic = 0.0075f, spotCutOff = glm::cos(glm::radians(17.5f)), spotOuterCutOff = glm::cos(glm::radians(32.0f));

		glm::vec3 directAmbient = (countdown > 300)? glm::vec3(0.2f, 0.2f, 0.2f) : glm::vec3(0.01f);
		glm::vec3 directDiffuse = (countdown > 300) ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(0.0f);

		texturedShader.UseProgram();
		texturedShader.SetVec3("viewPos", cameraPos);
		texturedShader.SetInt("material.deffuse", 0);
		texturedShader.SetVec3("material.specular", glm::vec3(0.0f));
		texturedShader.SetFloat("material.shininess", 32.0f);
		texturedShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		texturedShader.SetVec3("dirLight.ambient", directAmbient);
		texturedShader.SetVec3("dirLight.diffuse", directDiffuse);
		texturedShader.SetVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);
		texturedShader.SetVec3("flashlight.position", cameraPos);
		texturedShader.SetVec3("flashlight.direction", cameraFront);
		texturedShader.SetVec3("flashlight.ambient", 0.0f, 0.0f, 0.0f);
		texturedShader.SetVec3("flashlight.diffuse", 4.0f, 4.0f, 4.0f);
		texturedShader.SetVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);
		texturedShader.SetFloat("flashlight.constant", 1.0f);
		texturedShader.SetFloat("flashlight.linear", 0.045f);
		texturedShader.SetFloat("flashlight.quadratic", 0.0075f);
		texturedShader.SetFloat("flashlight.cutOff", glm::cos(glm::radians(12.5f)));
		texturedShader.SetFloat("flashlight.outerCutOff", glm::cos(glm::radians(15.0f)));
		texturedShader.SetBool("flashlightSwitch", flashlightSwitch);


		texturedShader.SetVec3("spotLights[0].position", -15.2552, 7.22035, -26.2433);
		texturedShader.SetVec3("spotLights[0].direction", 0.252794, -0.55557f, -0.79211);
		texturedShader.SetVec3("spotLights[0].ambient", spotAmbient);
		texturedShader.SetVec3("spotLights[0].diffuse", spotDiffuse * ((countdown > 200) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetVec3("spotLights[0].specular", spotSpecular * ((countdown > 200) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetFloat("spotLights[0].constant", spotConst);
		texturedShader.SetFloat("spotLights[0].linear", spotLinear);
		texturedShader.SetFloat("spotLights[0].quadratic", spotQuadratic);
		texturedShader.SetFloat("spotLights[0].cutOff", spotCutOff);
		texturedShader.SetFloat("spotLights[0].outerCutOff", spotOuterCutOff);
		texturedShader.SetVec3("spotLights[1].position", -8.31702, 6.8467, -26.3478);
		texturedShader.SetVec3("spotLights[1].direction", 0.16449, -0.607376, -0.777199);
		texturedShader.SetVec3("spotLights[1].ambient", spotAmbient);
		texturedShader.SetVec3("spotLights[1].diffuse", spotDiffuse * ((countdown > 100) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetVec3("spotLights[1].specular", spotSpecular * ((countdown > 100) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetFloat("spotLights[1].constant", spotConst);
		texturedShader.SetFloat("spotLights[1].linear", spotLinear);
		texturedShader.SetFloat("spotLights[1].quadratic", spotQuadratic);
		texturedShader.SetFloat("spotLights[1].cutOff", spotCutOff);
		texturedShader.SetFloat("spotLights[1].outerCutOff", spotOuterCutOff);
		texturedShader.SetVec3("spotLights[2].position", 0.0381766, 7.14894, -26.0308);
		texturedShader.SetVec3("spotLights[2].direction", 0.00464562, -0.649448, -0.760392);
		texturedShader.SetVec3("spotLights[2].ambient", spotAmbient);
		texturedShader.SetVec3("spotLights[2].diffuse", spotDiffuse * ((countdown > 0) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetVec3("spotLights[2].specular", spotSpecular * ((countdown > 0)? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetFloat("spotLights[2].constant", spotConst);
		texturedShader.SetFloat("spotLights[2].linear", spotLinear);
		texturedShader.SetFloat("spotLights[2].quadratic", spotQuadratic);
		texturedShader.SetFloat("spotLights[2].cutOff", spotCutOff);
		texturedShader.SetFloat("spotLights[2].outerCutOff", spotOuterCutOff);
		texturedShader.SetVec3("spotLights[3].position", 8.04403, 7.07597, -26.3904);
		texturedShader.SetVec3("spotLights[3].direction", -0.154568, -0.59131, -0.791493);
		texturedShader.SetVec3("spotLights[3].ambient", spotAmbient);
		texturedShader.SetVec3("spotLights[3].diffuse", spotDiffuse * ((countdown > 100) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetVec3("spotLights[3].specular", spotSpecular * ((countdown > 100) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetFloat("spotLights[3].constant", spotConst);
		texturedShader.SetFloat("spotLights[3].linear", spotLinear);
		texturedShader.SetFloat("spotLights[3].quadratic", spotQuadratic);
		texturedShader.SetFloat("spotLights[3].cutOff", spotCutOff);
		texturedShader.SetFloat("spotLights[3].outerCutOff", spotOuterCutOff);
		texturedShader.SetVec3("spotLights[4].position", 14.3109, 7.36264, -26.1804);
		texturedShader.SetVec3("spotLights[4].direction", -0.246624, -0.509041, -0.824654);
		texturedShader.SetVec3("spotLights[4].ambient", spotAmbient);
		texturedShader.SetVec3("spotLights[4].diffuse", spotDiffuse * ((countdown > 200) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetVec3("spotLights[4].specular", spotSpecular * ((countdown > 200) ? glm::vec3(1) : glm::vec3(0)));
		texturedShader.SetFloat("spotLights[4].constant", spotConst);
		texturedShader.SetFloat("spotLights[4].linear", spotLinear);
		texturedShader.SetFloat("spotLights[4].quadratic", spotQuadratic);
		texturedShader.SetFloat("spotLights[4].cutOff", spotCutOff);
		texturedShader.SetFloat("spotLights[4].outerCutOff", spotOuterCutOff);


		texturedShader.SetMat4("projection", projection);
		texturedShader.SetMat4("view", view);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, stageTxtr);

		glBindVertexArray(cubeVAO);
		model = glm::translate(unit_matrix, cubes[45].translate);
		model = glm::rotate(model, cubes[45].rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cubes[45].rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cubes[45].rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cubes[45].scale);
		texturedShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindTexture(GL_TEXTURE_2D, curtainTxtr);

		model = glm::translate(unit_matrix, cubes[46].translate);
		model = glm::rotate(model, cubes[46].rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cubes[46].rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cubes[46].rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cubes[46].scale);
		texturedShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		nontexturedShader.UseProgram();
		nontexturedShader.SetVec3("viewPos", cameraPos);
		nontexturedShader.SetVec3("material.ambient", 0.1745f, 0.01175f, 0.01175f);
		nontexturedShader.SetVec3("material.diffuse", 0.61424f, 0.04136f, 0.04136f);
		nontexturedShader.SetVec3("material.specular", 0.727811f +1, 0.626959f+1, 0.626959f+1);
		nontexturedShader.SetFloat("material.shininess", 1.2f * 128);
		nontexturedShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		nontexturedShader.SetVec3("dirLight.ambient", directAmbient);
		nontexturedShader.SetVec3("dirLight.diffuse", directDiffuse);
		nontexturedShader.SetVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);
		nontexturedShader.SetVec3("flashlight.position", cameraPos);
		nontexturedShader.SetVec3("flashlight.direction", cameraFront);
		nontexturedShader.SetVec3("flashlight.ambient", 0.1f, 0.1f, 0.1f);
		nontexturedShader.SetVec3("flashlight.diffuse", 1.0f, 1.0f, 1.0f);
		nontexturedShader.SetVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);
		nontexturedShader.SetFloat("flashlight.constant", 1.0f);
		nontexturedShader.SetFloat("flashlight.linear", 0.045f);
		nontexturedShader.SetFloat("flashlight.quadratic", 0.0075f);
		nontexturedShader.SetFloat("flashlight.cutOff", glm::cos(glm::radians(12.5f)));
		nontexturedShader.SetFloat("flashlight.outerCutOff", glm::cos(glm::radians(15.0f)));
		nontexturedShader.SetBool("flashlightSwitch", flashlightSwitch);

		nontexturedShader.SetVec3("spotLights[0].position", -15.2552, 7.22035, -26.2433);
		nontexturedShader.SetVec3("spotLights[0].direction", 0.252794, -0.55557f, -0.79211);
		nontexturedShader.SetVec3("spotLights[0].ambient", spotAmbient);
		nontexturedShader.SetVec3("spotLights[0].diffuse", spotDiffuse * ((countdown > 200) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetVec3("spotLights[0].specular", spotSpecular * ((countdown > 200) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetFloat("spotLights[0].constant", spotConst);
		nontexturedShader.SetFloat("spotLights[0].linear", spotLinear);
		nontexturedShader.SetFloat("spotLights[0].quadratic", spotQuadratic);
		nontexturedShader.SetFloat("spotLights[0].cutOff", spotCutOff);
		nontexturedShader.SetFloat("spotLights[0].outerCutOff", spotOuterCutOff);
		nontexturedShader.SetVec3("spotLights[1].position", -8.31702, 6.8467, -26.3478);
		nontexturedShader.SetVec3("spotLights[1].direction", 0.16449, -0.607376, -0.777199);
		nontexturedShader.SetVec3("spotLights[1].ambient", spotAmbient);
		nontexturedShader.SetVec3("spotLights[1].diffuse", spotDiffuse * ((countdown > 100) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetVec3("spotLights[1].specular", spotSpecular * ((countdown > 100) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetFloat("spotLights[1].constant", spotConst);
		nontexturedShader.SetFloat("spotLights[1].linear", spotLinear);
		nontexturedShader.SetFloat("spotLights[1].quadratic", spotQuadratic);
		nontexturedShader.SetFloat("spotLights[1].cutOff", spotCutOff);
		nontexturedShader.SetFloat("spotLights[1].outerCutOff", spotOuterCutOff);
		nontexturedShader.SetVec3("spotLights[2].position", 0.0381766, 7.14894, -26.0308);
		nontexturedShader.SetVec3("spotLights[2].direction", 0.00464562, -0.649448, -0.760392);
		nontexturedShader.SetVec3("spotLights[2].ambient", spotAmbient);
		nontexturedShader.SetVec3("spotLights[2].diffuse", spotDiffuse * ((countdown > 0) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetVec3("spotLights[2].specular", spotSpecular * ((countdown > 0) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetFloat("spotLights[2].constant", spotConst);
		nontexturedShader.SetFloat("spotLights[2].linear", spotLinear);
		nontexturedShader.SetFloat("spotLights[2].quadratic", spotQuadratic);
		nontexturedShader.SetFloat("spotLights[2].cutOff", spotCutOff);
		nontexturedShader.SetFloat("spotLights[2].outerCutOff", spotOuterCutOff);
		nontexturedShader.SetVec3("spotLights[3].position", 8.04403, 7.07597, -26.3904);
		nontexturedShader.SetVec3("spotLights[3].direction", -0.154568, -0.59131, -0.791493);
		nontexturedShader.SetVec3("spotLights[3].ambient", spotAmbient);
		nontexturedShader.SetVec3("spotLights[3].diffuse", spotDiffuse * ((countdown > 100) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetVec3("spotLights[3].specular", spotSpecular * ((countdown > 100) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetFloat("spotLights[3].constant", spotConst);
		nontexturedShader.SetFloat("spotLights[3].linear", spotLinear);
		nontexturedShader.SetFloat("spotLights[3].quadratic", spotQuadratic);
		nontexturedShader.SetFloat("spotLights[3].cutOff", spotCutOff);
		nontexturedShader.SetFloat("spotLights[3].outerCutOff", spotOuterCutOff);
		nontexturedShader.SetVec3("spotLights[4].position", 14.3109, 7.36264, -26.1804);
		nontexturedShader.SetVec3("spotLights[4].direction", -0.246624, -0.509041, -0.824654);
		nontexturedShader.SetVec3("spotLights[4].ambient", spotAmbient);
		nontexturedShader.SetVec3("spotLights[4].diffuse", spotDiffuse * ((countdown > 200) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetVec3("spotLights[4].specular", spotSpecular * ((countdown > 200) ? glm::vec3(1) : glm::vec3(0)));
		nontexturedShader.SetFloat("spotLights[4].constant", spotConst);
		nontexturedShader.SetFloat("spotLights[4].linear", spotLinear);
		nontexturedShader.SetFloat("spotLights[4].quadratic", spotQuadratic);
		nontexturedShader.SetFloat("spotLights[4].cutOff", spotCutOff);
		nontexturedShader.SetFloat("spotLights[4].outerCutOff", spotOuterCutOff);

		nontexturedShader.SetMat4("projection", projection);
		nontexturedShader.SetMat4("view", view);

		glBindVertexArray(cubeVAO);
		for (int i = 0; i < 45; i++)
		{
			model = glm::translate(unit_matrix, cubes[i].translate);
			model = glm::rotate(model, cubes[i].rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, cubes[i].rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, cubes[i].rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, cubes[i].scale);
			nontexturedShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		nontexturedShader.SetVec3("material.ambient", 0.0f, 0.0f, 0.0f);
		nontexturedShader.SetVec3("material.diffuse", 0.01f, 0.01f, 0.01f);
		nontexturedShader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
		nontexturedShader.SetFloat("material.shininess", 0.25f * 128);
		for (int i = 47; i < 57; i++)
		{
			model = glm::translate(unit_matrix, cubes[i].translate);
			model = glm::rotate(model, cubes[i].rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, cubes[i].rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, cubes[i].rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, cubes[i].scale);
			nontexturedShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		lampShader.UseProgram();
		lampShader.SetMat4("projection", projection);
		lampShader.SetMat4("view", view);

		glBindVertexArray(lightVAO);
		if (countdown > 0)
		{
			model = glm::translate(unit_matrix, spotLightPositions[2]);
			model = glm::rotate(model, spotLightRotations[2].x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[2].y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[2].z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.7f)); // Make it a smaller cube
			lampShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		if (countdown > 100)
		{
			model = glm::translate(unit_matrix, spotLightPositions[1]);
			model = glm::rotate(model, spotLightRotations[1].x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[1].y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[1].z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.7f)); // Make it a smaller cube
			lampShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			model = glm::translate(unit_matrix, spotLightPositions[3]);
			model = glm::rotate(model, spotLightRotations[3].x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[3].y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[3].z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.7f)); // Make it a smaller cube
			lampShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		if (countdown > 200)
		{
			model = glm::translate(unit_matrix, spotLightPositions[0]);
			model = glm::rotate(model, spotLightRotations[0].x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[0].y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[0].z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.7f)); // Make it a smaller cube
			lampShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			model = glm::translate(unit_matrix, spotLightPositions[4]);
			model = glm::rotate(model, spotLightRotations[4].x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[4].y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, spotLightRotations[4].z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.7f)); // Make it a smaller cube
			lampShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		if (countdown != 0 && countdown < 500) countdown++;

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	delete VBO;


	glfwTerminate();

	//while (true);
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
	{
		countdown = 1;
		return;
	}
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_RELEASE)
	{
		countdown = 0;
		return;
	}
	//if (key == GLFW_KEY_INSERT && action == GLFW_RELEASE)
	//{
	//	std::cout << "cubes[" << cubeIndx << "].translate = glm::vec3(" << cubes[cubeIndx].translate.x << ", " << cubes[cubeIndx].translate.y << ", " << cubes[cubeIndx].translate.z << ");" << std::endl;
	//	std::cout << "cubes[" << cubeIndx << "].scale = glm::vec3(" << cubes[cubeIndx].scale.x << ", " << cubes[cubeIndx].scale.y << ", " << -cubes[cubeIndx].scale.z << ");" << std::endl;
	//	std::cout << "cubes[" << cubeIndx << "].rotate = glm::vec3(" << cubes[cubeIndx].rotate.x << ", " << cubes[cubeIndx].rotate.y << ", " << -cubes[cubeIndx].rotate.z << ");" << std::endl;
	//	cubeIndx++;
	//}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	GLfloat currentFrame = (GLfloat)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (keys[GLFW_KEY_LEFT_CONTROL])
		deltaTime *= 4;
	cameraSpeed *= deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z));
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z));
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, worldUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, worldUp)) * cameraSpeed;
	if (keys[GLFW_KEY_SPACE])
		cameraPos += worldUp * cameraSpeed;
    if (keys[GLFW_KEY_LEFT_SHIFT])
		cameraPos -= worldUp * cameraSpeed;

	//if (keys[GLFW_KEY_UP])
	//{
	//	if (keys[GLFW_KEY_1]) cubes[cubeIndx].translate.z -= 0.01f;
	//	if (keys[GLFW_KEY_2]) cubes[cubeIndx].scale.z += 0.01f;
	//	if (keys[GLFW_KEY_3]) cubes[cubeIndx].rotate.z += 0.01f;
	//}
	//if (keys[GLFW_KEY_DOWN])
	//{
	//	if (keys[GLFW_KEY_1]) cubes[cubeIndx].translate.z += 0.01f;
	//	if (keys[GLFW_KEY_2]) cubes[cubeIndx].scale.z -= (cubes[cubeIndx].scale.z < 0.01f) ? 0.0f : 0.01f;
	//	if (keys[GLFW_KEY_3]) cubes[cubeIndx].rotate.z -= 0.01f;
	//}
	//if (keys[GLFW_KEY_RIGHT])
	//{
	//	if (keys[GLFW_KEY_1]) cubes[cubeIndx].translate.x += 0.01f;
	//	if (keys[GLFW_KEY_2]) cubes[cubeIndx].scale.x += 0.01f;
	//	if (keys[GLFW_KEY_3]) cubes[cubeIndx].rotate.x += 0.01f;
	//}
	//if (keys[GLFW_KEY_LEFT])
	//{
	//	if (keys[GLFW_KEY_1]) cubes[cubeIndx].translate.x -= 0.01f;
	//	if (keys[GLFW_KEY_2]) cubes[cubeIndx].scale.x -= (cubes[cubeIndx].scale.x < 0.01f) ? 0.0f : 0.01f;
	//	if (keys[GLFW_KEY_3]) cubes[cubeIndx].rotate.x -= 0.01f;
	//}
	//if (keys[GLFW_KEY_RIGHT_SHIFT])
	//{
	//	if (keys[GLFW_KEY_1]) cubes[cubeIndx].translate.y += 0.01f;
	//	if (keys[GLFW_KEY_2]) cubes[cubeIndx].scale.y += 0.01f;
	//	if (keys[GLFW_KEY_3]) cubes[cubeIndx].rotate.y += 0.01f;
	//}
	//if (keys[GLFW_KEY_SLASH])
	//{
	//	if (keys[GLFW_KEY_1]) cubes[cubeIndx].translate.y -= 0.01f;
	//	if (keys[GLFW_KEY_2]) cubes[cubeIndx].scale.y -= (cubes[cubeIndx].scale.y < 0.01f) ? 0.0f : 0.01f;
	//	if (keys[GLFW_KEY_3]) cubes[cubeIndx].rotate.y -= 0.01f;
	//}

	cameraSpeed /= deltaTime;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos;
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	GLfloat sensitivity = 0.05f;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= (GLfloat)yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		flashlightSwitch = !flashlightSwitch;
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char *data = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SOIL_free_image_data(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		SOIL_free_image_data(data);
	}

	return textureID;
}



