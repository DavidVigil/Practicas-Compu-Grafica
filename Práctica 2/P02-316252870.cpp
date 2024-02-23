//Práctica 2 semestre 2024-2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* fShaderRojo = "shaders/shader.frag";

static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* fShaderVerde = "shaders/shader.frag";

static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* fShaderCafe = "shaders/shader.frag";

static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* fShaderAzul = "shaders/shader.frag";

static const char* vShaderVerdeOscuro = "shaders/shaderverdeoscuro.vert";
static const char* fShaderVerdeOscuro = "shaders/shader.frag";

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
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

void CrearLetrasyFiguras()
{
	GLfloat vertices_letraA[] = {
		//X			Y			Z			R		G		B
		-0.8f,	-0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.7f,	-0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.6f,	 0.4f,		0.0f,			0.98f, 0.62f, 0.235f,

		-0.7f,	-0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.6f,	 0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.5f,	 0.4f,		0.0f,			0.98f, 0.62f, 0.235f,

		-0.7f,	 0.0f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.7f,	-0.1f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.4f,	-0.1f,		0.0f,			0.98f, 0.62f, 0.235f,

		-0.7f,	 0.0f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.4f,	 0.0f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.4f,	-0.1f,		0.0f,			0.98f, 0.62f, 0.235f,

		-0.6f,	 0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.5f,	 0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.4f,	-0.4f,		0.0f,			0.98f, 0.62f, 0.235f,

		-0.5f,	 0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.4f,	-0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
		-0.3f,	-0.4f,		0.0f,			0.98f, 0.62f, 0.235f,
	};

	MeshColor* letraA = new MeshColor();
	letraA->CreateMeshColor(vertices_letraA, 108);
	meshColorList.push_back(letraA);

	GLfloat vertices_letraD[] = {
		//X			Y			Z			R		G		B
		-0.2f,	 0.4f,		0.0f,			0.98f, 0.204f, 0.953f,
		-0.2f,  -0.4f,		0.0f,			0.98f, 0.204f, 0.953f,
		-0.1f,  -0.4f,		0.0f,			0.98f, 0.204f, 0.953f,

		-0.2f,   0.4f,		0.0f,			0.98f, 0.204f, 0.953f,
		-0.1f,   0.4f,		0.0f,			0.98f, 0.204f, 0.953f,
		-0.1f,  -0.4f,		0.0f,			0.98f, 0.204f, 0.953f,

		-0.2f,   0.4f,		0.0f,			0.98f, 0.204f, 0.953f,
		 0.1f,   0.1f,		0.0f,			0.98f, 0.204f, 0.953f,
		 0.2f,   0.1f,		0.0f,			0.98f, 0.204f, 0.953f,

		-0.2f,	 0.4f,		0.0f,			0.98f, 0.204f, 0.953f,
		-0.1f,	 0.4f,		0.0f,			0.98f, 0.204f, 0.953f,
		0.2f,	 0.1f,		0.0f,			0.98f, 0.204f, 0.953f,

		0.1f,	 0.1f,		0.0f,			0.98f, 0.204f, 0.953f,
		0.2f,	 0.1f,		0.0f,			0.98f, 0.204f, 0.953f,
		0.2f,	-0.1f,		0.0f,			0.98f, 0.204f, 0.953f,

		0.1f,	 0.1f,		0.0f,			0.98f, 0.204f, 0.953f,
		0.1f,	-0.1f,		0.0f,			0.98f, 0.204f, 0.953f,
		0.2f,	-0.1f,		0.0f,			0.98f, 0.204f, 0.953f,

		0.1f,	-0.1f,		0.0f,			0.98f, 0.204f, 0.953f,
		0.2f,	-0.1f,		0.0f,			0.98f, 0.204f, 0.953f,
		-0.1f,	-0.4f,		0.0f,			0.98f, 0.204f, 0.953f,

		0.1f,	-0.1f,		0.0f,			0.98f, 0.204f, 0.953f,
		-0.2f,	-0.4f,		0.0f,			0.98f, 0.204f, 0.953f,
		-0.1f,	-0.4f,		0.0f,			0.98f, 0.204f, 0.953f,

	};

	MeshColor* letraD = new MeshColor();
	letraD->CreateMeshColor(vertices_letraD, 144);
	meshColorList.push_back(letraD);

	GLfloat vertices_letraV[] = {
		//X			Y			Z			R		G		B
		0.3f,	 0.4f,		0.0f,			0.165f, 0.969f, 0.663f,
		0.4f,	 0.4f,		0.0f,			0.165f, 0.969f, 0.663f,
		0.6f,	-0.4f,		0.0f,			0.165f, 0.969f, 0.663f,

		0.3f,	 0.4f,		0.0f,			0.165f, 0.969f, 0.663f,
		0.5f,	-0.4f,		0.0f,			0.165f, 0.969f, 0.663f,
		0.6f,	-0.4f,		0.0f,			0.165f, 0.969f, 0.663f,

		0.5f,	-0.4f,		0.0f,			0.165f, 0.969f, 0.663f,
		0.6f,	-0.4f,		0.0f,			0.165f, 0.969f, 0.663f,
		0.7f,	 0.4f,		0.0f,			0.165f, 0.969f, 0.663f,

		0.7f,	 0.4f,		0.0f,			0.165f, 0.969f, 0.663f,
		0.8f,	 0.4f,		0.0f,			0.165f, 0.969f, 0.663f,
		0.6f,	-0.4f,		0.0f,			0.165f, 0.969f, 0.663f,

	};

	MeshColor* letraV = new MeshColor();
	letraV->CreateMeshColor(vertices_letraV, 72);
	meshColorList.push_back(letraV);

}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShaderRojo, fShaderRojo);  //Shader de color rojo
	shaderList.push_back(*shaderRojo);

	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShaderVerde, fShaderVerde);  //Shader de color verde
	shaderList.push_back(*shaderVerde);

	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShaderCafe, fShaderCafe);  //Shader de color cafe
	shaderList.push_back(*shaderCafe);

	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShaderAzul, fShaderAzul);  //Shader de color azul
	shaderList.push_back(*shaderAzul);

	Shader* shaderVerdeOscuro = new Shader();
	shaderVerdeOscuro->CreateFromFiles(vShaderVerdeOscuro, fShaderVerdeOscuro);  //Shader de color verde oscuro
	shaderList.push_back(*shaderVerdeOscuro);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//Aquí se dibuja la letra A
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.1f, 0.5f, -4.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Aquí se dibuja la letra D
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -4.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		//Aquí se dibuja la letra V
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.1f, 0.5f, -4.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();
		
		//Para el color rojo se usa el primer set de shaders con índice 1 en ShaderList
		//Cubo rojo
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//angulo += 0.1f;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, -5.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para el color azul se usa el set de shaders con índice 4 en ShaderList
		//Piramide azul
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.45f, -5.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
	
		//Para el color verde se usa el set de shaders con índice 2 en ShaderList
		//Cubos verdes
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para el color verde oscuro se usa el set de shaders con índice 5 en ShaderList
		//Triangulos verde oscuro
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.7f, -0.1f, -2.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.7f, -0.1f, -2.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Para el color cafe se usa el set de shaders con índice 3 en ShaderList
		//Cubos cafes
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.7f, -0.46f, -5.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.7f, -0.46f, -5.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/