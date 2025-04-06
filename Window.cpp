#include "Window.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <cmath>

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;

	// Inicializa las variables del auto
	posX = 0.0f;
	posZ = 0.0f;
	autoRotY = 0.0f;

	// Inicializa las variables del helicóptero
	helicopterPosX = 0.0f;
	helicopterPosY = 50.0f;  // Para mantener la altura inicial en y=50
	helicopterPosZ = 6.0f;   // Para mantener la posición inicial en z=6
	helicopterRotY = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 7: Iluminacion 1", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}

	//Llanta delantera izquierda
	if (key == GLFW_KEY_F)
	{
		theWindow->articulacion1 += 10.0;
	}
	if (key == GLFW_KEY_V)
	{
		theWindow->articulacion1 -= 10.0;
	}

	//Llanta delantera derecha
	if (key == GLFW_KEY_G)
	{
		theWindow->articulacion2 += 10.0;
	}
	if (key == GLFW_KEY_B)
	{
		theWindow->articulacion2 -= 10.0;
	}

	// Llanta trasera  izquierda
	if (key == GLFW_KEY_H)
	{
		theWindow->articulacion3 += 10.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->articulacion3 -= 10.0;
	}

	//Llanta Trasera derecha
	if (key == GLFW_KEY_J)
	{
		theWindow->articulacion4 += 10.0;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->articulacion4 -= 10.0;
	}

	//Cofre
	if (key == GLFW_KEY_K)
	{
		if (theWindow->articulacion5 > -5.0)
		{
		}
		else
		{
			theWindow->articulacion5 += 10.0;
		}
	}
	if (key == GLFW_KEY_L)
	{
		if (theWindow->articulacion5 < -10.0)
		{
		}
		else
		{
			theWindow->articulacion5 -= 10.0;
		}

	}
	//////////////////////////////////////////////////////////////////////////////////////////// Desplazamiento del carro
	// Movimiento hacia adelante (Z)
	if (key == GLFW_KEY_Z)
	{
		// Calcular el movimiento basado en la rotación actual
		theWindow->posX += sin(glm::radians(theWindow->autoRotY)) * 1.0f;
		theWindow->posZ += cos(glm::radians(theWindow->autoRotY)) * 1.0f;
	}

	// Movimiento hacia atrás (X)
	if (key == GLFW_KEY_X)
	{
		// Calcular el movimiento basado en la rotación actual
		theWindow->posX -= sin(glm::radians(theWindow->autoRotY)) * 1.0f;
		theWindow->posZ -= cos(glm::radians(theWindow->autoRotY)) * 1.0f;
	}

	// Rotación del auto (C) 
	if (key == GLFW_KEY_C)
	{
		theWindow->autoRotY += 5.0f;
		// Mantener el ángulo entre 0 y 360 grados
		if (theWindow->autoRotY >= 360.0f)
			theWindow->autoRotY -= 360.0f;
	}

	//////////////////////////////////////////////////////////////////////////////////////////// Desplazamiento del helicóptero
	// Movimiento hacia adelante del helicóptero (UP)
	if (key == GLFW_KEY_UP)
	{
		// Calcular el movimiento basado en la rotación actual
		theWindow->helicopterPosX += sin(glm::radians(theWindow->helicopterRotY)) * 1.0f;
		theWindow->helicopterPosZ += cos(glm::radians(theWindow->helicopterRotY)) * 1.0f;
	}

	// Movimiento hacia atrás del helicóptero (DOWN)
	if (key == GLFW_KEY_DOWN)
	{
		// Calcular el movimiento basado en la rotación actual
		theWindow->helicopterPosX -= sin(glm::radians(theWindow->helicopterRotY)) * 1.0f;
		theWindow->helicopterPosZ -= cos(glm::radians(theWindow->helicopterRotY)) * 1.0f;
	}

	// Rotación del helicóptero (RIGHT)
	if (key == GLFW_KEY_RIGHT)
	{
		theWindow->helicopterRotY += 5.0f;
		// Mantener el ángulo entre 0 y 360 grados
		if (theWindow->helicopterRotY >= 360.0f)
			theWindow->helicopterRotY -= 360.0f;
	}

	// Rotación del helicóptero (LEFT)
	if (key == GLFW_KEY_LEFT)
	{
		theWindow->helicopterRotY -= 5.0f;
		// Mantener el ángulo entre 0 y 360 grados
		if (theWindow->helicopterRotY < 0.0f)
			theWindow->helicopterRotY += 360.0f;
	}
	////////////////////////////////////////////////////////////////////////////////////////// 

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}