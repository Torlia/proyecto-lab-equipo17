//Proyecto
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Para manejo de archivos
#include <fstream> 
#include <iostream>


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f);
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 Light1 = glm::vec3(0);
//Anim

// --- Variables de Animación de Mesas ---
bool animarMesas = false;
// Empezamos con una escala casi de cero (usar 0 exacto a veces rompe la iluminación)
float escalaNuevas = 0.001f;

float rotEsfera1 = 0;
float rotEsfera1Inc = 0;
float rotEsfera2 = 0;
float rotEsfera2Inc = 0;
float rotEsfera3 = 0;
float rotEsfera3Inc = 0;
float rotEsfera4 = 0;
float rotEsfera4Inc = 0;
float rotEsfera5 = 0;
float rotEsfera5Inc = 0;

// --- Variables de la animación de los estudiantes ---
bool animarEstudiantes = false;

float womanTrayectoria = 0.0f;
int womanSegment = 0;
float womanSpeed = 0.14f;

float manTrayectoria = 1.0f;
int manSegment = 2;
float manSpeed = 0.2f;
//#define MAN_OFFSET 5.0f // Para que el hombre no empiece en el mismo eje que la mujer

glm::vec3 womanPos(0.0f, 0.0f, 0.0f);
float womanRot = 0.0f;
glm::vec3 manPos(0.0f);
float manRot = 0.0f;

float RightLeg = 0.0f;
float LeftLeg = 0.0f;
float RightArm = 0.0f;
float LeftArm = 0.0f;
bool  step = false;
float manRightLeg = 0.0f;
float manLeftLeg = 0.0f;
float manRightArm = 0.0f;
float manLeftArm = 0.0f;
bool  manStep = false;

// --- CURVAS DE BEZIER ---
struct BezierSegment {
	glm::vec3 P0, P1, P2, P3;
};

glm::vec3 CubicBezier(const BezierSegment& s, float t) {
	float u = 1.0f - t;
	return u*u*u*s.P0 + 3*u*u*t*s.P1 + 3*u*t*t*s.P2 + t*t*t* s.P3;
}

glm::vec3 CubicBezierTangent(const BezierSegment& s, float t) {
	float u = 1.0f - t;
	return 3.0f*(u*u*(s.P1-s.P0) + 2*u*t*(s.P2-s.P1) + t*t*(s.P3-s.P2));
}

// Definicion de la trayectoria
#define NUM_SEGMENTS 5

BezierSegment trayectoria[NUM_SEGMENTS] = {
	// Segmento 0: Entrada -> Tren
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(22.0f, 0.0f, 0.0f),
		glm::vec3(44.0f, 0.0f, 0.0f),
		glm::vec3(65.0f, 0.0f, 0.0f)
	},
	// Segmento 1: Tren -> Pasillo
	{
		glm::vec3(65.0f, 0.0f, 0.0f),
		glm::vec3(67.0f, 0.0f, 10.0f),
		glm::vec3(69.0f, 0.0f, 20.0f),
		glm::vec3(69.0f, 0.0f, 30.0f)
	},
	// Segmento 2: Pasillo -> Stands
	{
		glm::vec3(69.0f, 0.0f, 30.0f),
		glm::vec3(43.0f, 0.0f, 32.0f),
		glm::vec3(22.0f, 0.0f, 34.0f),
		glm::vec3(3.0f, 0.0f, 34.0f)
	},
	// Segmento 3: Stands -> Auditorio
	{
		glm::vec3(3.0f, 0.0f, 34.0f),
		glm::vec3(5.0f, 0.0f, -15.0f),
		glm::vec3(7.0f, 0.0f, -45.0f),
		glm::vec3(7.0f, 0.0f, -85.0f)
	},
	// Segmento 4: Regreso al inicio
   {
	   glm::vec3(7.0f, 0.0f, -85.0f),
	   glm::vec3(4.0f, 0.0f, -40.0f),
	   glm::vec3(2.0f, 0.0f, -20.0f),
	   glm::vec3(0.0f, 0.0f,   0.0f)
   },
};

BezierSegment trayectoriaHombre[NUM_SEGMENTS] = {
	// Segmento 0: Pasillo -> Centro
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -10.0f),
		glm::vec3(0.0f, 0.0f, -25.0f),
		glm::vec3(0.0f, 0.0f, -43.0f)
	},
	// Segmento 1: Centro -> Entrada
	{
		glm::vec3(0.0f, 0.0f, -43.0f),
		glm::vec3(-15.0f, 0.0f, -41.0f),
		glm::vec3(-35.0f, 0.0f, -39.0f),
		glm::vec3(-55.0f, 0.0f, -39.0f)
	},
	// Segmento 2: Entrada -> Stands
	{
		glm::vec3(-55.0f, 0.0f, -39.0f),
		glm::vec3(-53.0f, 0.0f, -35.0f),
		glm::vec3(-51.0f, 0.0f, -31.0f),
		glm::vec3(-51.0f, 0.0f, -29.0f)
	},
	// Segmento 3: Stands -> Pasillo
   {
	   glm::vec3(-51.0f, 0.0f, -29.0f),
	   glm::vec3(-15.0f, 0.0f, -27.0f),
	   glm::vec3(-35.0f, 0.0f, -25.0f),
	   glm::vec3(0.0f, 0.0f, -25.0f)
   },
	// Segmento 4: Regreso al inicio
   {
	   glm::vec3(0.0f, 0.0f, -25.0f),
	   glm::vec3(2.0f, 0.0f, -15.0f),
	   glm::vec3(4.0f, 0.0f, -5.0f),
	   glm::vec3(4.0f, 0.0f, 0.0f)
   }
};


//KeyFrames

#define MAX_FRAMES 150
int i_max_steps = 290;
int i_curr_steps = 0;
typedef struct _frame {

	// Rotaciones de las 5 esferas
	float rotEsfera1, rotEsfera1Inc;
	float rotEsfera2, rotEsfera2Inc;
	float rotEsfera3, rotEsfera3Inc;
	float rotEsfera4, rotEsfera4Inc;
	float rotEsfera5, rotEsfera5Inc;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;


void resetElements(void)
{

	rotEsfera1 = KeyFrame[0].rotEsfera1;
	rotEsfera2 = KeyFrame[0].rotEsfera2;
	rotEsfera3 = KeyFrame[0].rotEsfera3;
	rotEsfera4 = KeyFrame[0].rotEsfera4;
	rotEsfera5 = KeyFrame[0].rotEsfera5;

}
void interpolation(void) //Equipo 17
{

	KeyFrame[playIndex].rotEsfera1Inc = (KeyFrame[playIndex + 1].rotEsfera1 - KeyFrame[playIndex].rotEsfera1) / i_max_steps;
	KeyFrame[playIndex].rotEsfera2Inc = (KeyFrame[playIndex + 1].rotEsfera2 - KeyFrame[playIndex].rotEsfera2) / i_max_steps;
	KeyFrame[playIndex].rotEsfera3Inc = (KeyFrame[playIndex + 1].rotEsfera3 - KeyFrame[playIndex].rotEsfera3) / i_max_steps;
	KeyFrame[playIndex].rotEsfera4Inc = (KeyFrame[playIndex + 1].rotEsfera4 - KeyFrame[playIndex].rotEsfera4) / i_max_steps;
	KeyFrame[playIndex].rotEsfera5Inc = (KeyFrame[playIndex + 1].rotEsfera5 - KeyFrame[playIndex].rotEsfera5) / i_max_steps;

}
//Equipo 17

//Equipo 17
void cargarPasos(void) {
	std::ifstream archivo("animacion_pendulo.txt");
	if (archivo.is_open()) {
		archivo >> FrameIndex; // Leemos cuantos cuadros hay que cargar

		for (int i = 0; i < FrameIndex; i++) {
			archivo >> KeyFrame[i].rotEsfera1
				>> KeyFrame[i].rotEsfera2
				>> KeyFrame[i].rotEsfera3
				>> KeyFrame[i].rotEsfera4
				>> KeyFrame[i].rotEsfera5;
		}
		archivo.close();
		std::cout << "SISTEMA: Se ha detectado y cargado la animacion del pendulo (" << FrameIndex << " cuadros)." << std::endl;
	}
	else {
		std::cout << "SISTEMA: No se encontro animacion previa. Iniciando buffer limpio." << std::endl;
		FrameIndex = 0;
	}
}
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	//Mi nombre
	std::cout << "Equipo 17" << std::endl;

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Pasillo Examenes Profesionales - Equipo 17", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//models


	//Escuela
	Model Escuela((char*)"Models/escuela/facultad.obj");

	//Inmuebles
	Model Mesa((char*)"Models/mesa/exteriorTable.obj");
	Model Silla((char*)"Models/silla/silla.obj");

	//Pendulo
	Model Soporte((char*)"Models/pendulo/pendulo_base.obj"); // La estructura fija (se dibuja 1 vez)
	Model Esfera((char*)"Models/pendulo/pendulo_ball.obj");  // La bola (se dibuja 5 veces)

	//Personas
	Model BodyWoman((char*)"Models/mujer/woman_torso.obj");
	Model HeadWoman((char*)"Models/mujer/woman_head.obj");
	Model W_RightLeg((char*)"Models/mujer/woman_right_leg.obj");
	Model W_LeftLeg((char*)"Models/mujer/woman_left_leg.obj");
	Model W_RightArm((char*)"Models/mujer/woman_right_arm.obj");
	Model W_LeftArm((char*)"Models/mujer/woman_left_arm.obj");
	Model BodyMan((char*)"Models/hombre/man_torso.obj");
	Model HeadMan((char*)"Models/hombre/man_head.obj");
	Model M_RightLeg((char*)"Models/hombre/man_right_foot.obj");
	Model M_LeftLeg((char*)"Models/hombre/man_left_foot.obj");
	Model M_RightArm((char*)"Models/hombre/man_right_hand.obj");
	Model M_LeftArm((char*)"Models/hombre/man_left_hand.obj");



	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		// Rotaciones iniciales de las 5 esferas
		KeyFrame[i].rotEsfera1 = 0;
		KeyFrame[i].rotEsfera2 = 0;
		KeyFrame[i].rotEsfera3 = 0;
		KeyFrame[i].rotEsfera4 = 0;
		KeyFrame[i].rotEsfera5 = 0;

		// Incrementos para la interpolación (deben iniciar en 0)
		KeyFrame[i].rotEsfera1Inc = 0;
		KeyFrame[i].rotEsfera2Inc = 0;
		KeyFrame[i].rotEsfera3Inc = 0;
		KeyFrame[i].rotEsfera4Inc = 0;
		KeyFrame[i].rotEsfera5Inc = 0;
	}

	//Cargar una animación
	cargarPasos();

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// --- RENDERIZADO DE LA FACULTAD ---
		// 1. Reiniciamos la matriz de modelo para que parta del origen del mundo
		model = glm::mat4(1);

		// 2. Posicionamiento: Ajusta el valor de Y (segundo parámetro) para que el 
		// piso de la escuela coincida con el nivel de tu cámara o del suelo.
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));

		// 3. Escalado base: Los modelos arquitectónicos suelen venir en escalas muy 
		// distintas. Si no ves nada o se ve gigante, ajusta este valor (ej. 0.1f o 0.01f).
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		// 4. Envío de la matriz resultante al shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// 5. Llamada de dibujo
		Escuela.Draw(lightingShader);

		// --- MESAS Y SILLAS SECUNDARIAS (Las que crecen) ---

		// --- MESA CENTRAL (Siempre visible en su tamaño original) ---
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, -9.5f, 80.0f)); // Posición central
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.Draw(lightingShader);

		// Mesa Izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-40.0f, -9.5f, 80.0f)); // Desplazada a la izquierda
		model = glm::scale(model, glm::vec3(escalaNuevas * 8.0f));          // <-- APLICAMOS LA MAGIA DE LA ESCALA
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.Draw(lightingShader);

		// Mesa Derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, -9.5f, 80.0f)); // Desplazada a la derecha
		model = glm::scale(model, glm::vec3(escalaNuevas * 8.0f));         // <-- ESCALA
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.Draw(lightingShader);

		// Silla Izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-15.0f, -9.5f, 45.0f)); // Cerca de la mesa izquierda
		model = glm::scale(model, glm::vec3(escalaNuevas * 0.6f));          // Crecen juntas
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		// Silla Centro
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -9.5f, 45.0f)); // Cerca de la mesa izquierda
		model = glm::scale(model, glm::vec3(escalaNuevas * 0.6f));          // Crecen juntas
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);


		// Silla Derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(15.0f, -9.5f, 45.0f)); // Cerca de la mesa derecha
		model = glm::scale(model, glm::vec3(escalaNuevas * 0.6f));         // Crecen juntas
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		// --- DIBUJO DEL PÉNDULO DE NEWTON ---

		// 1. Dibujar el Soporte (Estructura fija)

		//Posicionando en el centro de la escena

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, -4.0f, 50.0f)); // Posición general aquí
		model = glm::scale(model, glm::vec3(escalaNuevas * 2.5f));
		modelTemp = model; // Guardar la base del soporte para las esferas
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Soporte.Draw(lightingShader);

		// Altura del hilo (distancia desde el soporte al centro de la esfer
		float largoHilo = 0.5f;
		// Separación entre esferas
		float separacion = 0.15f;

		// --- ESFERA 1 (Izquierda) ---
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-separacion * 2, 0.5f, 0.0f)); // 1. Ir al punto de anclaje (arriba)
		model = glm::rotate(model, glm::radians(rotEsfera1), glm::vec3(0.0f, 0.0f, 1.0f)); // 2. Rotar
		model = glm::translate(model, glm::vec3(0.0f, -largoHilo, 0.0f)); // 3. Bajar hasta la esfera
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Esfera.Draw(lightingShader);

		// --- ESFERA 2 ---
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-separacion, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(rotEsfera2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -largoHilo, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Esfera.Draw(lightingShader);

		// --- ESFERA 3 (Centro) ---
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(rotEsfera3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -largoHilo, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Esfera.Draw(lightingShader);

		// --- ESFERA 4 ---
		model = modelTemp;
		model = glm::translate(model, glm::vec3(separacion, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(rotEsfera4), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -largoHilo, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Esfera.Draw(lightingShader);

		// --- ESFERA 5 (Derecha) ---
		model = modelTemp;
		model = glm::translate(model, glm::vec3(separacion * 2, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(rotEsfera5), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -largoHilo, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Esfera.Draw(lightingShader);

		// --- DIBUJO DE MUJER ESTUDIANTE ---
		// Cuerpo
		glm::mat4 womanBase = glm::mat4(1.0f);
		womanBase = glm::translate(womanBase, womanPos + glm::vec3(-30.5f, -9.0f, 6.0f)); // posícion inicial en la entrada
		womanBase = glm::rotate(womanBase, glm::radians(womanRot + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // gira a su izquierda, para ver al frente
		womanBase = glm::scale(womanBase, glm::vec3(8.0f, 8.0f, 8.0f));
		modelTemp = model = womanBase;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BodyWoman.Draw(lightingShader);

		// Cabeza
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadWoman.Draw(lightingShader);

		// Brazo derecho
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.05f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(RightArm), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		W_RightArm.Draw(lightingShader);

		// Brazo izquierdo
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.05f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(LeftArm), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		W_LeftArm.Draw(lightingShader);

		// Pierna derecha
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.15f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(RightLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		W_RightLeg.Draw(lightingShader);

		// Pierna izquierda
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.15f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(LeftLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		W_LeftLeg.Draw(lightingShader);

		// --- DIBUJO DE HOMBRE ESTUDIANTE ---
		// Cuerpo
		glm::mat4 manBase = glm::mat4(1.0f);
		manBase = glm::translate(manBase, manPos + glm::vec3(40.0f, -9.0f, 59.0f)); // posícion inicial en el pasillo
		manBase = glm::rotate(manBase, glm::radians(manRot - 180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // gira a su izquierda, para ver al frente
		manBase = glm::scale(manBase, glm::vec3(7.0f, 7.0f, 7.0f));
		modelTemp = model = manBase;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BodyMan.Draw(lightingShader);

		// Cabeza
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadMan.Draw(lightingShader);

		// Brazo derecho
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.05f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(manRightArm), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		M_RightArm.Draw(lightingShader);

		// Brazo izquierdo
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.05f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(manLeftArm), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		M_LeftArm.Draw(lightingShader);

		// Pierna derecha
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.25f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(manRightLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		M_RightLeg.Draw(lightingShader);

		// Pierna izquierda
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.25f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(manLeftLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		M_LeftLeg.Draw(lightingShader);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// --- CONTROLES DE ANIMACIÓN Y ESTADOS (Solo se ejecutan una vez al presionar) ---

	if (action == GLFW_PRESS) {
		// 1. Activar animación de mesas (Teclado M)
		if (key == GLFW_KEY_M) {
			animarMesas = !animarMesas;
		}

		// 2. Encender/Apagar Luz (Barra espaciadora)
		if (key == GLFW_KEY_SPACE) {
			active = !active;
			if (active) Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			else Light1 = glm::vec3(0);
		}

		// 3. Reproducir/Pausar Péndulo (Teclado L)
		if (key == GLFW_KEY_L) {
			if (play == false && (FrameIndex > 1)) {
				resetElements();
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
			}
			else {
				play = false;
			}
		}

		// 4. Activar animacion de personas
		if (key == GLFW_KEY_N) {
			animarEstudiantes = !animarEstudiantes;
			if (animarEstudiantes) {
				womanTrayectoria = 0.0f;
				womanSegment = 0.0f;
				manTrayectoria = 0.0f;
				manSegment = 0.0f;
			}
		}
	} 
}


void Animation() {
	if (play)
	{
		if (i_curr_steps >= i_max_steps) // ¿Terminamos la transición entre cuadros?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	// ¿Terminó toda la coreografía?
			{
				printf("SISTEMA: Animacion finalizada\n");
				playIndex = 0;
				play = false;
			}
			else // Configuración para el siguiente tramo de la animación
			{
				i_curr_steps = 0; // Reiniciamos el contador de pasos
				interpolation();  // Calculamos los nuevos incrementos para las esferas
			}
		}
		else // Bloque de REPRODUCCIÓN (aquí es donde se suma paso a paso)
		{
			// Aplicamos los incrementos a las 5 esferas
			rotEsfera1 += KeyFrame[playIndex].rotEsfera1Inc;
			rotEsfera2 += KeyFrame[playIndex].rotEsfera2Inc;
			rotEsfera3 += KeyFrame[playIndex].rotEsfera3Inc;
			rotEsfera4 += KeyFrame[playIndex].rotEsfera4Inc;
			rotEsfera5 += KeyFrame[playIndex].rotEsfera5Inc;

			// Avanzamos un paso en el contador
			i_curr_steps++;
		}
	}

	if (animarMesas) {
		// Si la animación está activa, crecen hasta llegar a 1.0f (escala 100%)
		if (escalaNuevas < 1.0f) {
			escalaNuevas += 0.0005f; // Velocidad con la que "aparecen"
		}
		else {
			escalaNuevas = 1.0f; // Tope para que no crezcan al infinito
		}
	}
	else {
		// Opcional: Si vuelves a presionar 'M', se hacen chiquitas de nuevo
		if (escalaNuevas > 0.001f) {
			escalaNuevas -= 0.0005f;
		}
		else {
			escalaNuevas = 0.001f;
		}
	}

	if (animarEstudiantes) {
		womanTrayectoria += womanSpeed * deltaTime;
		if (womanTrayectoria >= 1.0f) {
			womanTrayectoria -= 1.0f;
			womanSegment++;
			if (womanSegment >= NUM_SEGMENTS) {
				womanSegment = 0;
			}
		}
		womanPos = CubicBezier(trayectoria[womanSegment], womanTrayectoria);
		glm::vec3 tangent = CubicBezierTangent(trayectoria[womanSegment], womanTrayectoria);
		if (glm::length(tangent) > 0.001f) {
			tangent = glm::normalize(tangent);
			womanRot = glm::degrees(atan2(tangent.x, tangent.z))-90.0f;
		}
		// Animación de piernas/brazos al caminar (maquina de estados)
		if (!step) {
			RightLeg += 0.2f;
			LeftLeg -= 0.2f;
			RightArm -= 0.2f;
			LeftArm += 0.2f;
			if (RightLeg > 5.0f) { //angulo maximo, regresa atras
				step = true;
			}
		}
		else {
			RightLeg -= 0.2f;
			LeftLeg += 0.2f;
			RightArm += 0.2f;
			LeftArm -= 0.2f;
			if (RightLeg < -5.0f) { //angulo maximo, regresa en frente
				step = false;
			}
		}

		// Repitiendo para el hombre
		manTrayectoria += manSpeed * deltaTime;
		if (manTrayectoria >= 1.0f) {
			manTrayectoria -= 1.0f;
			manSegment++;
			if (manSegment >= NUM_SEGMENTS) {
				manSegment = 0;
			}
		}
		manPos = CubicBezier(trayectoriaHombre[manSegment], manTrayectoria);
		glm::vec3 manTangent = CubicBezierTangent(trayectoriaHombre[manSegment], manTrayectoria);
		if (glm::length(manTangent) > 0.001f) {
			manTangent = glm::normalize(manTangent);
			manRot = glm::degrees(atan2(manTangent.x, manTangent.z)) - 180.0f;
		}
		// Animación de piernas/brazos al caminar (maquina de estados)
		if (!manStep) {
			manRightLeg += 0.2f;
			manLeftLeg -= 0.2f;
			manRightArm -= 0.2f;
			manLeftArm += 0.2f;
			if (manRightLeg > 5.0f) {
				manStep = true;
			}
		}
		else {
			manRightLeg -= 0.2f;
			manLeftLeg += 0.2f;
			manRightArm += 0.2f;
			manLeftArm -= 0.2f;
			if (manRightLeg < -5.0f) {
				manStep = false;
			}
		}
	}

}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
