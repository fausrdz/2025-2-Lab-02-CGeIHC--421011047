/*
Práctica 7: Iluminación 1
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
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
Texture AgaveTexture;

Model Blackhawk_M;

Model Mundo_Luz;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;     // Super clase: color de luz, intensidad ambiental, intendisad difusa.//abuelo
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS]; //superclase: Posicion, atenuación (coeficientes de una ecuacion de 2do grado), distancia //padre
SpotLight spotLights[MAX_SPOT_LIGHTS]; //Direccion, Angulo //hijo

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
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

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	// Mundo
	Mundo_Luz = Model();
	Mundo_Luz.LoadModel("Models/mluz.obj");


	// helicoptero
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

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

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, // Ahora luz blanca
		30.0f, 1.0f,
		-40.0f, 15.0f, 2.0f, //Modificados para que ahora sean los valores en la misma posición del modelo de lampara 
		20.0f, 0.2f, 0.1f);  //nuemros de la ecuacion que deberemos mover para el previo
	pointLightCount++;
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	//Luz fija
	//modificación para que el azul se mueva con el carro
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,  // Cambiamos el verde por el azul
		1.0f, 2.0f,                              
		0.0f, 0.0f, 0.0f,                        
		0.0f, -1.0f, 0.0f,                       
		1.0f, 0.0f, 0.0f,                        
		15.0f);                                  
	spotLightCount++;
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,  // Nueva luz amarilla 
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); /// La que liga la luz a la camara 

		// ------------ - movimiento del spot y seguimiento con el carro-------------------------------------- -

		// Actualiza la posición del spotlight azul (spotLights[1]) para que se mueva con el carro
		glm::mat4 lightTransform = glm::mat4(1.0f);
		lightTransform = glm::translate(lightTransform, glm::vec3(mainWindow.getPosX(), 12.0f, mainWindow.getPosZ()));
		lightTransform = glm::rotate(lightTransform, glm::radians(mainWindow.getAutoRotY()), glm::vec3(0.0f, 1.0f, 0.0f));

		// Posición de la luz ubicada al frente del carro
		glm::vec4 lightPosition = lightTransform * glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);

		// Dirección de la luz: apuntando horizontalmente hacia la derecha 
		glm::vec4 lightDirection = lightTransform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		glm::vec3 normalizedDirection = glm::normalize(glm::vec3(lightDirection));

		// Actualiza la posición y dirección del spotlight azul
		spotLights[1].SetFlash(glm::vec3(lightPosition), normalizedDirection);

		// ------------- movimiento del spot y seguimiento con el carro ---------------------------------------


		// ------------ - movimiento del spot y seguimiento con el helicoptero--------------------------------------

		// Actualiza la posición del spotlight amarillo (spotLights[2]) para que se mueva con el helicoperto 
		glm::mat4 lightTransform1 = glm::mat4(1.0f);
		lightTransform1 = glm::translate(lightTransform1, glm::vec3(mainWindow.getHelicopterPosX(), mainWindow.getHelicopterPosY(), mainWindow.getHelicopterPosZ()));
		lightTransform1 = glm::rotate(lightTransform1, glm::radians(mainWindow.getHelicopterRotY()), glm::vec3(0.0f, 1.0f, 0.0f));

		// Posición de la luz ubicada debajo del helicoptero
		glm::vec4 lightPosition1 = lightTransform1 * glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);

		// Dirección de la luz: apuntando horizontalmente hacia abajo
		glm::vec4 lightDirection1 = lightTransform1 * glm::vec4(0.0f, -10.0f, 0.0f, 0.0f);
		glm::vec3 normalizedDirection1 = glm::normalize(glm::vec3(lightDirection1));

		// Actualiza la posición y dirección del spotlight amarillo
		spotLights[2].SetFlash(glm::vec3(lightPosition1), normalizedDirection1);
	
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Chasis (Carroceria - Nodo principal )

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

		/// helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getHelicopterPosX(), mainWindow.getHelicopterPosY(), mainWindow.getHelicopterPosZ())); 
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getHelicopterRotY()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Blackhawk_M.RenderModel();

		//Luz Mundo
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(-40.0f, 8.0f, 2.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Mundo_Luz.RenderModel(); 

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}