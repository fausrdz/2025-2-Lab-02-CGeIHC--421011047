//práctica 3: Modelado Geométrico y Cámara Sintética con casa integrada
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>

//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z

using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert"; 

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
		0,1,2,    // Base triangular
		1,3,2,    // Cara lateral 1
		3,0,2,    // Cara lateral 2
		0,1,3     // Cara lateral 3
	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f, -0.5f,    // 0: Vértice base izquierdo
		0.5f, -0.5f, -0.5f,     // 1: Vértice base derecho
		0.0f, -0.5f, 0.5f,    // 2: Vértice base posterior
		0.0f, 0.5f, 0.0f       // 3: Vértice superior (ápice)
	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4
	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders()
{
	Shader* shader = new Shader(); 
	shader->CreateFromFiles(vShader, fShader); 
	shaderList.push_back(*shader); 

	Shader* shader1 = new Shader(); 
	shader1->CreateFromFiles(vShaderColor, fShader);  
	shaderList.push_back(*shader1);  
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	// Crear todas las figuras geométricas necesarias
	CrearPiramideTriangular();// índice 1 en MeshList - Para el techo
	CrearPiramideCuadrangular(); // índice 4 en MeshList - No se usará para la casa

	CreateShaders();

	// Configuración de cámara
	camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 globalRotation(1.0);  
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Fondo gris claro
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);

		/*
		globalRotation = glm::mat4(1.0f); 
		globalRotation = glm::translate(globalRotation, glm::vec3(0.0f, 0.0f, -1.5f)); 
		globalRotation = glm::rotate(globalRotation, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(globalRotation)); 
		*/

		// Configuración del shader principal
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		// Configurar proyección y vista (común para todos los objetos)
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// Contorno Negro (pirámide negra )
		color = glm::vec3(0.0f, 0.0f, 0.0f); // Color Negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		globalRotation = glm::mat4(1.0f); 
		globalRotation = glm::translate(globalRotation, glm::vec3(0.0f, 0.0f, -1.5f)); 
		globalRotation = glm::scale(globalRotation, glm::vec3(1.0f, 1.0f, 1.0f)); 
		globalRotation = glm::rotate(globalRotation, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f)); 
		globalRotation = glm::rotate(globalRotation, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f)); 
		globalRotation = glm::rotate(globalRotation, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(globalRotation)); 
		meshList[0]->RenderMesh(); // Usar pirámide triangular

		// ------------------------------------- Caras de Colores ----------------------------------------------------
		
		// ------------------ piramides pequeñas (cara verde )-------------------------------------
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Punta de la cara verde -- 1 piramide

		model = glm::mat4(1.0f); //Piramide la punta
		model = glm::translate(model, glm::vec3(0.025f, 0.3f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMesh(); 

		//Segunda Fila ---  3 piramides

		model = glm::mat4(1.0f); //Cara izquierda 
		model = glm::translate(model, glm::vec3(0.025f, 0.0f, 0.15f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMesh(); 
	
		model = glm::mat4(1.0f); //Cara central invertida 
		model = glm::translate(model, glm::vec3(0.125f, 0.0f, -0.03f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.225f, 0.5f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMesh(); 

		model = glm::mat4(1.0f); //Cara Derecha
		model = glm::translate(model, glm::vec3(0.175f, 0.0f, -0.15f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = globalRotation * model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		
		//Tercera Fila ---  5 piramides
		
		model = glm::mat4(1.0f);//Cara izquierda 
		model = glm::translate(model, glm::vec3(0.025f, -0.3f, 0.3f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMesh(); 
		
		model = glm::mat4(1.0f);//Cara central invertida izquierda 
		model = glm::translate(model, glm::vec3(0.125f, -0.3f, 0.12f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.225f, 0.5f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMesh(); 

		model = glm::mat4(1.0f); //Cara central sin invertir 
		model = glm::translate(model, glm::vec3(0.175f, -0.3f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMesh(); 
	
		model = glm::mat4(1.0f);//Cara central invertida derecha 
		model = glm::translate(model, glm::vec3(0.275f, -0.3f, -0.17f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.225f, 0.5f)); 
		model = globalRotation * model;  
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMesh(); 

		model = glm::mat4(1.0f); // Cara derecha  
		model = glm::translate(model, glm::vec3(0.325f, -0.3f, -0.3f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMesh(); 
		
		// ----------------- piramides pequeñas (cara Roja)-----------------------------
		color = glm::vec3(1.0f, 0.0f, 0.0f); //Cara Roja
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Punta de la cara Roja -- 1 piramide

		model = glm::mat4(1.0f); //Piramide de la punta
		model = glm::translate(model, glm::vec3(-0.025f, 0.3f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

	    //Segunda Fila ---  3 piramides
		
		model = glm::mat4(1.0f);//Cara Izquierda  
		model = glm::translate(model, glm::vec3(-0.18f, 0.0f, -0.15f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 
		
		model = glm::mat4(1.0f);// Cara Central invertida
		model = glm::translate(model, glm::vec3(-0.125f, 0.0f, -0.03f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, -0.225f, -0.5f));
		model = globalRotation * model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry();

		model = glm::mat4(1.0f);//Cara Derecha 
		model = glm::translate(model, glm::vec3(-0.025f, 0.0f, 0.15f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 


		//Tercera Fila ---  5 piramides 

		model = glm::mat4(1.0f); //Cara Izquierda 
		model = glm::translate(model, glm::vec3(-0.325f, -0.3f, -0.3f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		model = glm::mat4(1.0f);//Cara invertida izquerda 
		model = glm::translate(model, glm::vec3(-0.125f, -0.3f, 0.12f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, -0.225f, -0.5f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 
		
		model = glm::mat4(1.0f); //Cara central sin invertir  
		model = glm::translate(model, glm::vec3(-0.175f, -0.3f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		model = glm::mat4(1.0f);//Cara Invertida Derecha 
		model = glm::translate(model, glm::vec3(-0.275f, -0.3f, -0.175f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, -0.225f, -0.5f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		model = glm::mat4(1.0f); //Cara Derecha 
		model = glm::translate(model, glm::vec3(-0.025f, -0.3f, 0.3f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 
		
		// ----------------- piramides pequeñas (cara blanca)-----------------------------
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Punta de la cara Blanca -- 1 piramide

		model = glm::mat4(1.0f); //Piramide de la punta 
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, -0.05f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		//Segunda Fila ---  3 piramides

		model = glm::mat4(1.0f); //Cara Izquierda  
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, -0.2f));  
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));  
		model = globalRotation * model;  
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));  
		meshList[0]->RenderMeshGeometry();  

		model = glm::mat4(1.0f);//Cara central invertida
		model = glm::translate(model, glm::vec3(0.0f, -0.05f, -0.225f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -0.25f, 0.5f));
		model = globalRotation * model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry();

		model = glm::mat4(1.0f); //Cara derecha
		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, -0.2f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = globalRotation * model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry();
		
		//Tercera Fila ---  5 piramides

		model = glm::mat4(1.0f); //Cara izquierda  
		model = glm::translate(model, glm::vec3(0.3f, -0.3f, -0.35f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		model = glm::mat4(1.0f); //Cara invertida Izquierda 
		model = glm::translate(model, glm::vec3(0.15f, -0.35f, -0.375f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -0.25f, 0.5f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		model = glm::mat4(1.0f); //Cara central sin invertir 
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -0.35f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		model = glm::mat4(1.0f); //Cara invertida Derecha
		model = glm::translate(model, glm::vec3(-0.15f, -0.35f, -0.375f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -0.25f, 0.5f));
		model = globalRotation * model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry();
		
		model = glm::mat4(1.0f);// Cara Derecha 
		model = glm::translate(model, glm::vec3(-0.3f, -0.3f, -0.35f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 
		

		// ----------------- piramides pequeñas (cara Azul) -----------------------------
		color = glm::vec3(0.0f, 0.0f, 1.0f);// Cara Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//La identificacion de cada cara es basando en la perspectiva del movimiento con la tecla E desde el inicio

		// Punta de la cara Azul -- 1 piramide

		model = glm::mat4(1.0f);//Punta de la piramide 
		model = glm::translate(model, glm::vec3(0.0f, -0.39f, 0.25f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		//Segunda Fila ---  3 piramides

		model = glm::mat4(1.0f); //Cara izquierda
		model = glm::translate(model, glm::vec3(-0.15f, -0.39f, -0.05f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = globalRotation * model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry();

		model = glm::mat4(1.0f); //Cara invertida cenral 
		model = glm::translate(model, glm::vec3(0.0f, -0.39f, -0.05f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = globalRotation * model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry();

		model = glm::mat4(1.0f);// Cara derecha 
		model = glm::translate(model, glm::vec3(0.15f, -0.39f, -0.05f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		//Tercera Fila ---  5 piramides
		
		model = glm::mat4(1.0f); //Cara izquierda 
		model = glm::translate(model, glm::vec3(-0.3f, -0.39f, -0.35f));  
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));  
		model = globalRotation * model;  
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));  
		meshList[0]->RenderMeshGeometry();   

		model = glm::mat4(1.0f); //Cara izquierda invertida  
		model = glm::translate(model, glm::vec3(-0.15f, -0.39f, -0.35f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 
		
		model = glm::mat4(1.0f); //Cara central sin invertir 
		model = glm::translate(model, glm::vec3(0.0f, -0.39f, -0.35f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 

		model = glm::mat4(1.0f); //Cara Derecha invertida 
		model = glm::translate(model, glm::vec3(0.15f, -0.39f, -0.35f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f)); 
		model = globalRotation * model;  
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 
		
		model = glm::mat4(1.0f); //Cara derecha  
		model = glm::translate(model, glm::vec3(0.3f, -0.39f, -0.35f)); 
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); 
		model = globalRotation * model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		meshList[0]->RenderMeshGeometry(); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}