#include <stdio.h>
#include <string.h>
#include <glew.h>     
//#include <random>
#include <glfw3.h>   

const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

// Modificacion a este para reciba posicones y colores
static const char* vShader = "                        \n\
#version 330                                        \n\
layout (location = 0) in vec3 pos;                 \n\
layout (location = 1) in vec3 color;               \n\
out vec3 vertexColor;                              \n\
void main()                                        \n\
{                                                  \n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);  \n\
    vertexColor = color;                           \n\
}";

// Modificacion a este para que ya no sea un color fijo sino que reciba los colores del vertex
static const char* fShader = "                        \n\
#version 330                                        \n\
in vec3 vertexColor;                               \n\
out vec4 color;                                    \n\
void main()                                        \n\
{                                                  \n\
    color = vec4(vertexColor, 1.0);                \n\
}";

// Omiti la funciion de mostrar fondos con colores aleatroios para que se puedan apreciar los colores de cada letra

////////////////////////////////////////////////////////////////////////////////////////////////
//La funcion para valores aleatorios entre 0 y 1
/*
float randomfloat() {
	static std::random_device rd;
	static std::mt19937 gen(rd()); //el generador de numero aleatoriosque se suele usar en funciones de numeros aleatorios
	static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	return dis(gen);
}

void colorAleatorio(float& r, float& g, float& b) {
	static double UltimocambioT = 0.0;
	static const double cambio_colores = 2.0; // perido de cmabi cada 2 segundos

	double Tiempoactual = glfwGetTime();

	// Verifica si es tiempo de cambiar el color por los valores rojo, verde y azul, contemplando cualquier posible combinacion 
	if (Tiempoactual - UltimocambioT >= cambio_colores) {
		// Genera valores aleatorios para que se comporte como la iluminaci[on rgb
		r = randomfloat(); //(1,0,0)
		g = randomfloat(); //(0,1,0)
		b = randomfloat(); //(0,0,1)

		UltimocambioT = Tiempoactual;
	} 
}*/
/////////////////////////////////////////////////////////////////////////////////////////////


void CrearLetraF()
{
	// Vertices que forman la sletras ahora con los colores (x, y, z, r, g, b)
	GLfloat vertices[] = {
		// F / Color        Rojo (1.0, 0.0, 0.0)
		-0.9f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.9f,  0.45f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.7f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f,  0.45f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.9f,  0.45f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.9f,  0.39f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.7f,  0.39f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.7f,  0.34f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.9f,  0.39f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.9f,  0.34f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.7f,  0.34f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.9f,  0.45f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.9f,  0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.84f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.9f,  0.45f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.84f, 0.45f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.84f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,

		// A / Color        Verde (0.0, 1.0, 0.0)
		-0.6f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.4f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.6f,  0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.4f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.4f,  0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.6f,  0.45f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.6f,  0.39f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.4f,  0.39f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.4f,  0.34f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.6f,  0.39f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.6f,  0.34f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.4f,  0.34f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.6f,  0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.6f,  0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.54f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.6f,  0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.54f, 0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.54f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.46f, 0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.46f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.40f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.46f, 0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.40f, 0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.40f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,

		// R / Color       Azul (0.0, 0.0, 1.0)
		-0.3f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.1f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.3f,  0.45f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.1f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.1f,  0.45f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.3f,  0.45f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.3f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.3f,  0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.3f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.25f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.3f,  0.4f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.1f,  0.4f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.3f,  0.35f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.1f,  0.4f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.1f,  0.35f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.3f,  0.35f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.16f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.16f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.10f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.17f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.10f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.10f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.25f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.15f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.1f,  0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.25f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.19f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.1f,  0.25f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// modifcado pa que ahora ya reciba  6 atributos por los vectores de color extra 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// modifcado pa que ahora ya reciba 6 atributos por los vectores de color extra 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Funcion para agregar los shaders a la tarjeta grafica
//the Program recibe los datos de theShader


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el c digo
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevenci n de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el c digo a la tarjeta gr fica
}

void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gr fica
	//verificaciones y prevenci n de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicializaci n de GLFW
	if (!glfwInit())
	{
		printf("Fall  inicializar GLFW");
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
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 02: Proyecciones y puertos de vista. Transformaciones Geometricas ", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama o de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall  inicializaci n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Crear tr angulo
	CrearLetraF();
	CompileShaders();


	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		
		/////
		//Recibir eventos del usuario
		glfwPollEvents();

		//float r, g, b;
		//colorAleatorio(r, g, b);

		//Limpiar la ventana
		//glClearColor(0.0f,0.0f,0.0f,1.0f);  comentamos el vector para hacer pruebas
		//glClearColor(r, g, b, 1.0f); //Aqui se recibe en lugar del vector, las variables casteadas en flotante 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}


	return 0;
}