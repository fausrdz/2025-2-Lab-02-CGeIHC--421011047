//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

//clases de encabezado
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const float toRadians = 3.14159265f / 180.0;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

//Vertex Shaders
static const char* vShaderRojo = "shaders/shader_rojo.vert";
static const char* fShaderRojo = "shaders/shader_rojo.frag";
static const char* vShaderVerde = "shaders/shader_verde.vert";
static const char* fShaderVerde = "shaders/shader_verde.frag";
static const char* vShaderAzul = "shaders/shader_azul.vert";
static const char* fShaderAzul = "shaders/shader_azul.frag";
static const char* vShaderCafe = "shaders/shader_cafe.vert";
static const char* fShaderCafe = "shaders/shader_cafe.frag";
static const char* vShaderVerdeOscuro = "shaders/shader_verde_oscuro.vert";
static const char* fShaderVerdeOscuro = "shaders/shader_verde_oscuro.frag";

float angulo = 0.0f;

// Crear la pirámide
void CrearPiramide()
{
    GLfloat vertices[] = {
        // Base
        -1.1f, -1.1f, 1.1f,     // 0
         1.1f, -1.1f, 1.1f,     // 1
         1.1f, -1.1f, -1.1f,    // 2
        -1.1f, -1.1f, -1.1f,    // 3
        // Punta
         0.0f, 1.0f, 0.0f       // 4
    };

    unsigned int indices[] = {
        // Base
        0, 1, 2,
        0, 2, 3,
        // Caras
        0, 4, 1,    // Frente
        1, 4, 2,    // Derecha
        2, 4, 3,    // Atrás
        3, 4, 0     // Izquierda
    };

    Mesh* piramide = new Mesh();
    piramide->CreateMesh(vertices, indices, 15, 18);
    meshList.push_back(piramide);
}

// Crear un cubo
void CrearCubo()
{
    GLfloat vertices[] = {
        // Frente
        -1.0f, -1.0f,  1.0f,    // 0
         1.0f, -1.0f,  1.0f,    // 1
         1.0f,  1.0f,  1.0f,    // 2
        -1.0f,  1.0f,  1.0f,    // 3
        // Atrás
        -1.0f, -1.0f, -1.0f,    // 4
         1.0f, -1.0f, -1.0f,    // 5
         1.0f,  1.0f, -1.0f,    // 6
        -1.0f,  1.0f, -1.0f     // 7
    };

    unsigned int indices[] = {
        // Frente
        0, 1, 2,
        2, 3, 0,
        // Derecha
        1, 5, 6,
        6, 2, 1,
        // Atrás
        5, 4, 7,
        7, 6, 5,
        // Izquierda
        4, 0, 3,
        3, 7, 4,
        // Arriba
        3, 2, 6,
        6, 7, 3,
        // Abajo
        4, 5, 1,
        1, 0, 4
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(vertices, indices, 24, 36);
    meshList.push_back(cubo);
}

void CrearCasaFiguras()
{
    CrearPiramide();  // índice 0
    CrearCubo();      // índice 1
}

void CreateShaders()
{
    Shader* shaderRojo = new Shader();
    shaderRojo->CreateFromFiles(vShaderRojo, fShaderRojo);
    shaderList.push_back(*shaderRojo);

    Shader* shaderVerde = new Shader();
    shaderVerde->CreateFromFiles(vShaderVerde, fShaderVerde);
    shaderList.push_back(*shaderVerde);

    Shader* shaderAzul = new Shader();
    shaderAzul->CreateFromFiles(vShaderAzul, fShaderAzul);
    shaderList.push_back(*shaderAzul);

    Shader* shaderCafe = new Shader();
    shaderCafe->CreateFromFiles(vShaderCafe, fShaderCafe);
    shaderList.push_back(*shaderCafe);

    Shader* shaderVerdeOscuro = new Shader();
    shaderVerdeOscuro->CreateFromFiles(vShaderVerdeOscuro, fShaderVerdeOscuro);
    shaderList.push_back(*shaderVerdeOscuro);
}

int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();
    CrearCasaFiguras();
    CreateShaders();

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.getShouldClose())
    {
        glfwPollEvents();
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));

        // Troncos café (cubos)
        shaderList[3].useShader(); // Shader café
        uniformModel = shaderList[3].getModelLocation();
        uniformProjection = shaderList[3].getProjectLocation();

        // Tronco izquierdo
        glm::mat4 modelTroncoIzq = glm::mat4(1.0f);
        modelTroncoIzq = glm::translate(model, glm::vec3(-1.3f, -0.6f, 0.0f));
        modelTroncoIzq = glm::scale(modelTroncoIzq, glm::vec3(0.1f, 0.2f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTroncoIzq));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh();

        // Tronco derecho
        glm::mat4 modelTroncoDer = glm::mat4(1.0f);
        modelTroncoDer = glm::translate(model, glm::vec3(1.3f, -0.6f, 0.0f));
        modelTroncoDer = glm::scale(modelTroncoDer, glm::vec3(0.1f, 0.2f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTroncoDer));
        meshList[1]->RenderMesh();

        // Copas de árboles (pirámides verdes)
        shaderList[4].useShader(); // Shader verde oscuro
        uniformModel = shaderList[4].getModelLocation();
        uniformProjection = shaderList[4].getProjectLocation();

        // Copa izquierda
        glm::mat4 modelCopaIzq = glm::mat4(1.0f);
        modelCopaIzq = glm::translate(model, glm::vec3(-1.3f, -0.2f, 0.0f));
        modelCopaIzq = glm::scale(modelCopaIzq, glm::vec3(0.2f, 0.3f, 0.2f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCopaIzq));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh();

        // Copa derecha
        glm::mat4 modelCopaDer = glm::mat4(1.0f);
        modelCopaDer = glm::translate(model, glm::vec3(1.3f, -0.2f, 0.0f));
        modelCopaDer = glm::scale(modelCopaDer, glm::vec3(0.2f, 0.3f, 0.2f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCopaDer));
        meshList[0]->RenderMesh();

        // Casa (cubo rojo)
        shaderList[0].useShader(); // Shader rojo
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        glm::mat4 modelCasa = glm::mat4(1.0f);
        modelCasa = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
        modelCasa = glm::scale(modelCasa, glm::vec3(0.8f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasa));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh();

        // Techo (pirámide azul)
        shaderList[2].useShader(); // Shader azul
        uniformModel = shaderList[2].getModelLocation();
        uniformProjection = shaderList[2].getProjectLocation();
        glm::mat4 modelTecho = glm::mat4(1.0f);
        modelTecho = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.8f));
        modelTecho = glm::scale(modelTecho, glm::vec3(0.8f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTecho));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh();

        // Ventanas (cubos verdes)
        shaderList[1].useShader(); // Shader verde
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();

        // Ventana izquierda
        glm::mat4 modelVentanaIzq = glm::mat4(1.0f);
        modelVentanaIzq = glm::translate(model, glm::vec3(-0.35f, -0.3f, 0.2f));
        modelVentanaIzq = glm::scale(modelVentanaIzq, glm::vec3(0.2f, 0.2f, 0.01f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelVentanaIzq));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh();

        // Ventana derecha
        glm::mat4 modelVentanaDer = glm::mat4(1.0f);
        modelVentanaDer = glm::translate(model, glm::vec3(0.35f, -0.3f, 0.2f));
        modelVentanaDer = glm::scale(modelVentanaDer, glm::vec3(0.2f, 0.2f, 0.01f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelVentanaDer));
        meshList[1]->RenderMesh();

        // Puerta (cubo verde)
        glm::mat4 modelPuerta = glm::mat4(1.0f);
        modelPuerta = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.2f));
        modelPuerta = glm::scale(modelPuerta, glm::vec3(0.2f, 0.3f, 0.01f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPuerta));
        meshList[1]->RenderMesh();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    return 0;
}