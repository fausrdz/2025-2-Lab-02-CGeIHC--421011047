#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);
    int Initialise();
    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }
    bool getShouldClose() {
        return  glfwWindowShouldClose(mainWindow);
    }
    bool* getsKeys() { return keys; }
    GLfloat getXChange();
    GLfloat getYChange();
    void swapBuffers() { return glfwSwapBuffers(mainWindow); }
    GLfloat getrotay() { return rotay; }
    GLfloat getrotax() { return rotax; }
    GLfloat getrotaz() { return rotaz; }
    GLfloat getangulocola() { return angulocola; }
    GLfloat getarticulacion1() { return articulacion1; }
    GLfloat getarticulacion2() { return articulacion2; }
    GLfloat getarticulacion3() { return articulacion3; }
    GLfloat getarticulacion4() { return articulacion4; }
    GLfloat getarticulacion5() { return articulacion5; }
    // Variables para el delorean
    GLfloat getPosX() { return posX; }
    GLfloat getPosZ() { return posZ; }
    GLfloat getAutoRotY() { return autoRotY; }
    // Nuevas variables para el helicóptero
    GLfloat getHelicopterPosX() { return helicopterPosX; }
    GLfloat getHelicopterPosY() { return helicopterPosY; }
    GLfloat getHelicopterPosZ() { return helicopterPosZ; }
    GLfloat getHelicopterRotY() { return helicopterRotY; }
    // Variable para controlar la loz focos de movimiento
    bool getIsMovingForward() { return isMovingForward; }
    bool getIsMovingBackward() { return isMovingBackward; }
    // Variable para controlar la luz de la lámpara luz mundo
    bool getLampLightOn() { return lampLightOn; }
    // Variable para controlar la luz de la lámpara luz trofeo
    bool getTrofeoLightOn() { return trofeoLightOn; }
    ~Window();
private:
    GLFWwindow* mainWindow;
    GLint width, height;
    GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5;
    // Variables para la posición y rotación del auto
    GLfloat posX, posZ;    // Para la posición 
    GLfloat autoRotY;      // Para la rotación del auto y de ese efecto de girar 
    // Variables para la posición y rotación del helicóptero
    GLfloat helicopterPosX, helicopterPosY, helicopterPosZ;
    GLfloat helicopterRotY;
    // Variable para controlar el estado de la luz de la lámpara
    bool isMovingForward = false;  // Bandera para movimiento hacia adelante
    bool isMovingBackward = false; // Bandera para movimiento hacia atrás

    bool trofeoLightOn;  // Bandera para la luz del trofeo

    bool lampLightOn; // Bandera para la luz del mundo

    bool keys[1024];
    GLint bufferWidth, bufferHeight;
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    GLfloat angulocola;
    bool mouseFirstMoved;
    void createCallbacks();
    static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
    static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};