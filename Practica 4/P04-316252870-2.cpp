//Pr�ctica 2 semestre 2024-2: �ndices, mesh, proyecciones, transformaciones geom�tricas
#include <stdio.h>
#include <string.h>
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
//clases para dar orden y limpieza al c�digo
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
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pir�mide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//funci�n para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los v�rtices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//funci�n para crear pir�mide cuadrangular unitaria
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
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(1090, 800);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindro(20, 1.0f);//�ndice 2 en MeshList
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
	CreateShaders();



	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelCabina(1.0);
	glm::mat4 modelTorso(1.0);

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		// Ejercicio 2: Gato robot

		// Reinicio de la matriz de modelo con valor de la matriz identidad
		model = glm::mat4(1.0);

		// Torso 
		model = glm::translate(model, glm::vec3(0.0f, 9.0f, -4.0f));
		modelTorso = model;
		modelaux = model;

		model = glm::scale(model, glm::vec3(15.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color torso 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Torso 


		model = modelaux; //Descartamos escala

		model = glm::translate(model, glm::vec3(4.0f, -0.2f, 0.0f));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 3.0f, 4.0f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Barriga 


		model = modelaux; // Descartamos transformaci�n

		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 2.0f, 4.0f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  //Barriga


		model = modelaux; // Descartamos transformaci�n

		model = glm::translate(model, glm::vec3(-4.8f, -2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 1.0f, 4.0f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Pecho


		// Pata delantera 1

		model = modelaux; //Descartamos transformaci�n

		model = glm::translate(model, glm::vec3(-4.0f, 1.0f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;  //Descartar escala del cilindro
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		// Parte superior
		model = modelaux;

		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 3.0f));
		modelaux = model;  //Descartar escala
		model = glm::scale(model, glm::vec3(2.5f, 2.0f, 4.0f));

		color = glm::vec3(0.9, 0.9f, 0.9f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Parte de la pata


		// Parte media
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion


		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 4.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Parte de la pata

		// Garra
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.5f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 1.5f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Garra


		modelaux = modelTorso;
		model = modelaux;   // Regreso al torso


		// Pata delantera 2

		model = glm::translate(model, glm::vec3(-4.0f, 1.0f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;  //Descartar escala del cilindro
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();  // Cilindro y articulacion

		// Parte superior 2
		model = modelaux;

		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 3.0f));
		modelaux = model;  //Descartar escala
		model = glm::scale(model, glm::vec3(2.5f, 2.0f, 4.0f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Parte de la pata

		// Parte media 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion


		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 4.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Parte de la pata

		// Garra
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion

		model = modelaux;  //Descarta escala de esfera
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.5f));
		modelaux = model;  //Descarta escala de la garra
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 1.5f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Garra


		modelaux = modelTorso;
		model = modelaux;   // Regreso al torso


		// Pata trazera 1

		model = glm::translate(model, glm::vec3(4.0f, 0.5f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;  //Descartar escala del cilindro
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();  // Cilindro y articulacion


		// Parte superior 3
		model = modelaux;

		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 2.75f));
		modelaux = model;  //Descartar escala
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 3.5f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Parte de la pata


		// Parte media 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.75f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion


		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 4.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Parte media de la pata


		// Garra 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.5f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.5f, 2.0f, 1.5f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Garra 3


		modelaux = modelTorso;
		model = modelaux;   // Regreso al torso


		// Pata trasera 2

		model = glm::translate(model, glm::vec3(4.0f, 0.5f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;  //Descartar escala del cilindro
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();  // Cilindro y articulacion

		// Parte superior 4
		model = modelaux;

		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 2.75f));
		modelaux = model;  //Descartar escala
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 3.5f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Parte de la pata


		// Parte media 4
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.75f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion


		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 4.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Parte de la pata

		// Garra 4
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion12()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.5f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.5f, 2.0f, 1.5f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();  // Garra 4


		modelaux = modelTorso;
		model = modelaux;   // Regreso al torso


		//Cola 

		//Parte 1
		model = glm::translate(model, glm::vec3(7.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion13()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion

		model = modelaux;  //Descartar escala de la esfera
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 3.0f, 1.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();  // Cilindro


		//Parte 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion14()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion

		model = modelaux;  //Descartar escala de la esfera
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.8f, 5.0f, 0.8f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();  // Cilindro


		//Parte 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion15()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion

		model = modelaux;  //Descartar escala de la esfera
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.7f, 2.0f, 0.7f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();  // Cilindro

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Esfera/articulacion


		modelaux = modelTorso;
		model = modelaux;   // Regreso al torso


		// Cabeza
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 2.0f, 5.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMeshGeometry();  // Rectangulo 

		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.5f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 2.0f, 1.0f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMeshGeometry();  // Rectangulo 

		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 4.0f));

		color = glm::vec3(0.9f, 0.9f, 0.9f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMeshGeometry();  // Rectangulo 


		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 2.0f, 4.0f));

		color = glm::vec3(0.1f, 0.1f, 0.1f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMeshGeometry();  // Rectangulo 

		//orejas
		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, 4.0f, -2.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMeshGeometry();  // Rectangulo 

		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMeshGeometry();  // Rectangulo 

		//Ojos
		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, 1.9f, 2.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.753f, 0.949f, 0.0f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, 1.9f, -2.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		color = glm::vec3(0.753f, 0.949f, 0.0f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		// Colmillos
		model = modelaux;
		model = glm::translate(model, glm::vec3(-11.8f, -0.1f, -1.2f));
		model = glm::scale(model, glm::vec3(0.1f, 0.7f, 0.1f));

		color = glm::vec3(0.929f, 0.906f, 0.863f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();  // Cono

		model = modelaux;
		model = glm::translate(model, glm::vec3(-11.8f, -0.1f, 1.2f));
		model = glm::scale(model, glm::vec3(0.1f, 0.7f, 0.1f));

		color = glm::vec3(0.929f, 0.906f, 0.863f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();  // Cono

		model = modelaux;
		model = glm::translate(model, glm::vec3(-11.8f, 0.15f, 2.2f));
		model = glm::rotate(model, glm::radians(190.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //T
		model = glm::scale(model, glm::vec3(0.1f, 0.7f, 0.1f));

		color = glm::vec3(0.929f, 0.906f, 0.863f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();  // Cono

		model = modelaux;
		model = glm::translate(model, glm::vec3(-11.8f, 0.15f, -2.2f));
		model = glm::rotate(model, glm::radians(190.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //T
		model = glm::scale(model, glm::vec3(0.1f, 0.7f, 0.1f));

		color = glm::vec3(0.929f, 0.906f, 0.863f);   // Color
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();  // Cono

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


