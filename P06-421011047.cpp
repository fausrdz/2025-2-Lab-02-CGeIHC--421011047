/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Model Delorean_Chasis; // Chasis
Model Delorean_Cofre; //Cofre
Model Llanta_derecha;// LL1
Model Llanta_izquierda;// LL2
Model Derecha_trasera;// LL3
Model Izquierda_trasera;// LL4

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;

Model Dado_M;
Model Dado_M1;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};


	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
	
		// front
		//// x      y      z      S         T         NX    NY    NZ
		-0.5f,  -0.5f,  0.5f,  0.104432f,   0.354307f, 0.0f,  0.0f,  -1.0f,  // 0
		0.5f,   -0.5f,  0.5f,  0.294100f,   0.354307f,  0.0f,  0.0f,  -1.0f,  // 1
		0.5f,    0.5f,  0.5f,  0.294100f,   0.624413f,  0.0f,  0.0f,  -1.0f,  // 2
		-0.5f,   0.5f,  0.5f,  0.104432f,   0.624413f,  0.0f,  0.0f,  -1.0f,  // 3

		//// right
		0.5f,   -0.5f,  0.5f,  0.2910f, 0.3556f,   -1.0f,  0.0f,  0.0f,  // 4
		0.5f,   -0.5f, -0.5f,  0.4824f, 0.3556f,   -1.0f,  0.0f,  0.0f,  // 5
		0.5f,    0.5f, -0.5f,  0.4824f, 0.6278f,   -1.0f,  0.0f,  0.0f,  // 6
		0.5f,    0.5f,  0.5f,  0.2910f, 0.6278f,   -1.0f,  0.0f,  0.0f,  // 7

		//// back
		0.5f,   -0.5f, -0.5f,  0.4805f, 0.3556f,     0.0f,  0.0f,   1.0f,  // 8
		-0.5f,  -0.5f, -0.5f,  0.6719f, 0.3556f,     0.0f,  0.0f,   1.0f,  // 9
		-0.5f,   0.5f, -0.5f,  0.6719f, 0.6278f,     0.0f,  0.0f,   1.0f,  // 10
		0.5f,    0.5f, -0.5f,  0.4805f, 0.6278f,     0.0f,  0.0f,   1.0f,  // 11

		//// left
		-0.5f,  -0.5f, -0.5f,  0.6754777f,  0.354307f,   1.0f,  0.0f,  0.0f,  // 12
		-0.5f,  -0.5f,  0.5f,  0.864139f,   0.354307f,   1.0f,  0.0f,  0.0f,  // 13
		-0.5f,   0.5f,  0.5f,  0.864139f,   0.624413f,   1.0f,  0.0f,  0.0f,  // 14
		-0.5f,   0.5f, -0.5f,  0.6754777f,  0.624413f,   1.0f,  0.0f,  0.0f,  // 15

		//// bottom
		-0.5f,  -0.5f,  0.5f,  0.6754777f,  0.085033f,    0.0f,  1.0f,  0.0f,  // 16
		0.5f,   -0.5f,  0.5f,  0.4834139f,  0.085033f,    0.0f,  1.0f,  0.0f,  // 17
		0.5f,   -0.5f, -0.5f,  0.4834139f,  0.354307f,    0.0f,  1.0f,  0.0f,  // 18
		-0.5f,  -0.5f, -0.5f,  0.6754777f,  0.354307f,    0.0f,  1.0f,  0.0f,  // 19

		//// top
		-0.5f,   0.5f,  0.5f,  0.6754777f,  0.896730f,  -1.0f,  0.0f,  0.0f,  // 20
		0.5f,    0.5f,  0.5f,  0.483413f,   0.896730f,  -1.0f,  0.0f,  0.0f,  // 21
		0.5f,    0.5f, -0.5f,  0.483413f,   0.624413f,  -1.0f,  0.0f,  0.0f,  // 22
		-0.5f,   0.5f, -0.5f,  0.6754777f,  0.624413f,  -1.0f,  0.0f,  0.0f,  // 23
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);


}

void CrearOctaedro()
{
	unsigned int octaedro_indices[] = {
		// Cara frontal --- Arriba
	   0, 1, 2,
	   // Cara derecha --- Arriba
	   3, 4, 5,
	   // Cara trasera --- Arriba
	   6, 7, 8,
	   // Cara izquierda --- Arriba
	   9, 10, 11,
	   // Cara frontal --- Abajo
	   12, 13, 14,
	   // Cara derecha --- Abajo
	   15, 16, 17,
	   // Cara trasera --- Abajo
	   18, 19, 20,
	   // Cara izquierda --- Abajo
	   21, 22, 23
	};
	GLfloat octaedro_vertices[] = {
		// Cara frontal --- Arriba
		-0.5f,  0.0f,  0.5f,   0.535f,  0.746f,   0.0f,  0.0f,  1.0f,
		 0.5f,  0.0f,  0.5f,   0.080f,  0.746f,   0.0f,  0.0f,  1.0f,
		 0.0f,  0.5f,  0.0f,   0.307f,  0.500f,   0.0f,  1.0f,  0.0f,

		 // Cara derecha --- Arriba
		 0.5f,  0.0f, -0.5f,   0.980f,  0.256f,   1.0f,  0.0f,  0.0f,
		-0.5f,  0.0f, -0.5f,   0.533f,  0.256f,   1.0f,  0.0f,  0.0f,
		 0.0f, -0.5f,  0.0f,   0.757f,  0.500f,   0.0f,  1.0f,  0.0f,


		 // Cara trasera --- Arriba
		 0.5f,  0.0f,  0.5f,   0.766f,  0.502f,   0.0f,  0.0f, -1.0f,
		 0.5f,  0.0f, -0.5f,   0.313f,  0.502f,   0.0f,  0.0f, -1.0f,
		 0.0f, -0.5f,  0.0f,   0.540f,  0.256f,   0.0f,  1.0f,  0.0f,

		 // Cara izquierda --- Arriba
		-0.5f,  0.0f, -0.5f,   0.082f,  0.748f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.0f,  0.5f,   0.535f,  0.748f,  -1.0f,  0.0f,  0.0f,
		 0.0f,  0.5f,  0.0f,   0.308f,  0.992f,   0.0f,  1.0f,  0.0f,


		 // Cara frontal --- Abajo
		 0.5f,  0.0f,  -0.5f,   0.980f,  0.748f,   0.0f,  -1.0f, 0.0f,
		-0.5f,  0.0f,  -0.5f,   0.533f,  0.748f,   0.0f,  -1.0f, 0.0f,
		 0.0f,  0.5f,  0.0f,   0.756f,  0.503f,   0.0f,  -1.0f, 0.0f,

		 // Cara derecha --- Abajo
		 0.5f,  0.0f,  0.5f,   0.082f,  0.256f,   1.0f,  0.0f,  0.0f,
		-0.5f,  0.0f,  0.5f,   0.529f,  0.256f,   1.0f,  0.0f,  0.0f,
		 0.0f, -0.5f,  0.0f,   0.306f,  0.500f,   0.0f, -1.0f,  0.0f,


		 // Cara trasera --- Abajo 
		-0.5f,  0.0f, -0.5f,   0.082f,  0.256f,   0.0f, -1.0f,  0.0f,
		-0.5f,  0.0f,  0.5f,   0.535f,  0.256f,   0.0f, -1.0f,  0.0f,
		 0.0f, -0.5f,  0.0f,   0.307f,  0.004f,   0.0f, -1.0f,  0.0f,

		 // Cara izquierda --- Abajo
		 0.5f,  0.0f,  0.5f,   0.311f,  0.504f,  -1.0f,  0.0f,  0.0f,
		 0.5f,  0.0f, -0.5f,   0.758f,  0.504f,  -1.0f,  0.0f,  0.0f,
		 0.0f,  0.5f,  0.0f,   0.534f,  0.748f,   0.0f, -1.0f,  0.0f
	};
	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 192, 36);
	meshList.push_back(octaedro);
}

int main()
{
	mainWindow = Window(1366, 768); 
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();
	CrearOctaedro();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	//Delorean 
	Delorean_Chasis = Model();
	Delorean_Chasis.LoadModel("Models/Chasis1.obj");

	Delorean_Cofre = Model(); //Cofre
	Delorean_Cofre.LoadModel("Models/Cofre.obj");

	Llanta_izquierda = Model();//Llanta_Izquierda_delantera
	Llanta_izquierda.LoadModel("Models/Llanta1.obj");

	Llanta_derecha = Model();//Llanta_Derecha_Delantera
	Llanta_derecha.LoadModel("Models/Llanta2.obj");

	Derecha_trasera = Model();//Llanta_Derecha_Trasera
	Derecha_trasera.LoadModel("Models/Llanta3.obj");

	Izquierda_trasera = Model();//Llanta_Izquierda_Trasera
	Izquierda_trasera.LoadModel("Models/Llanta4.obj");


	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_8_caras_2.png");
	dadoTexture.LoadTextureA();

	

	Model Dado_M1;
	Dado_M1.LoadModel("Models/Dadoanimalesop.obj");

	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();


		//Dado de Opengl
		//Ejercicio 1: Texturizar su cubo con la imagen del dado octaedro ya optimizada 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, 40.0f, -2.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[5]->RenderMesh();
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen dado_animales ya optimizada por ustedes
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(-3.5f, 40.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Dado_M1.RenderModel(); 

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Chasis (Carroceria - Nodo principal )

		color = glm::vec3(0.5529f, 0.5490f, 0.5529f); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(mainWindow.getPosX(), 12.0f, mainWindow.getPosZ())); 
		model = glm::rotate(model, glm::radians(mainWindow.getAutoRotY()), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::translate(model, glm::vec3(0.0f, 12.0f, -1.5f)); coordenadas anteriorers al movimiento 
		modelaux = model; 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Delorean_Chasis.RenderModel(); 
		modelaux = model; 

		//Jerarquia de las partes del carro

		// Cofre (hij0 de la chasis)
		model = modelaux; 
		model = glm::translate(model, glm::vec3(-1.5f, 3.5f, 13.5f)); 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Delorean_Cofre.RenderModel(); 

		//// Llanta delantera izquierda (jerarquia hija del chasis)
		model = modelaux; 
		model = glm::translate(model, glm::vec3(15.0f, -8.0f, 24.0f)); 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Llanta_izquierda.RenderModel(); 

		////// Llanta delantera derecha (jerarquia hija del chasis)
		model = modelaux; 
		model = glm::translate(model, glm::vec3(-17.0f, -8.0f, 24.0f)); 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Llanta_derecha.RenderModel(); 

		////// Llanta trasera izquierda (jerarquia hija del chasis)
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.0f, -7.0f, -26.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Izquierda_trasera.RenderModel();

		////// Llanta trasera derecha (jerarquia hija del chasis)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-17.0f, -7.0f, -26.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Derecha_trasera.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}