#include <iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "camera.h"
//SOIL2
#include "SOIL2/SOIL2.h"
#include "SOIL2/stb_image.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <vector>
#include <algorithm>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 1000, HEIGHT = 1000;  // �½�����
										  // timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
//�������� ���������·�  �����Ϊ�˻�ȡ�û�������������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);
void transfer1();
void parameter();


int main()
{
	//int i = 1;
	float i = 0.0;
	float pos = 0.0;
	float g = 9.8f;
	bool touchDown = false;
	glfwInit();
	//OpenGL �汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// ��������
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // �õ����°�� OpenGL 3.3
																	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  // ��Ϊ GL_TRUE���ı䴰�ڣ��ݺ�Ȼ��
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "B17040425", nullptr, nullptr);  // �������ָĳ��Լ���ѧ��

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// next two lines are for mac retina display
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);  // ��ȡ���ڴ�С

	glfwMakeContextCurrent(window);  // �����½��ܶ� window
									 //�ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	//glViewport(0, 0, screenWidth, screenHeight);  // �ӣ�0,0����ʼ���㣬ֱ�� WIDTH �� HEIGHT
	glEnable(GL_DEPTH_TEST);//��һ�� �������� ��������Сʱ���������㲻�� ������Ȳ���
							//vs �Ƕ����ɫ����frag �Ǳ�Ե��ɫ��
	Shader ourShader = Shader("shader.vs", "shader.fs");
	Shader ourShaderNew = Shader("shader.vs", "shader.fs");
	//Shader cubeshader("cubemaps.vs", "cubemaps.fs");
	//��һ������ʼ��
	//GLfloat vertices[] = {
	//	//first triangle      //texture
	//	0.5f, 0.6f, 0.0f,      1.0f, 1.0f,  // ���Ͻ�
	//	0.5f, -0.6f, 0.0f,     1.0f, 0.0f,  // ���½�
	//	-0.5f, -0.6f, 0.0f,    0.0f, 0.0f,  // ���½�
	//	-0.5f, 0.6f, 0.0f,     0.0f, 1.0f   // ���Ͻ�
	//};
	//������ 3D
	//������
	GLfloat vertices[] = {
		//����                 //����
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//����׶
	//GLfloat vertices[] = {
	////����                 //����
	//0.5f,0.0f,0.0f,                      0.0f, 0.0f,  //1
	//0.0f,-0.5f,0.0f,	                  1.0f, 0.0f, //2
	//-0.5,0.0f,0.0f,	                      1.0f, 1.0f, //3

	//0.5f,0.0f,0.0f,                       1.0f, 1.0f, //1
	//0.0f,-0.5f,0.0f,                      0.0f, 1.0f, //2
	//0.0f,0.0f,0.5f,	                      0.0f, 0.0f, //4
	//	                      
	//-0.5,0.0f,0.0f,	                      0.0f, 0.0f, //3
	//0.5f,0.0f,0.0f, 	                  1.0f, 0.0f, //1
	//0.0f,0.0f,0.5f,	                      1.0f, 1.0f, //4

	//0.0f,-0.5f,0.0f,                      1.0f, 1.0f, //2
	//-0.5,0.0f,0.0f,	                      0.0f, 1.0f, //3
	//0.0f,0.0f,0.5f,	                      0.0f, 0.0f, //4
	//};
	//3D��ü����  Z�������������
	//����ÿ�ζ������Ȼ���  ��������whileѭ�������


	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	//10��������
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	//����������һ����
	//���֣�ÿһ��ͼƬ����Ҫ��ʼ��һ������ Ȼ���չ涨��ʽ���а󶨺ͳ�ʼ������


	//the date should be transfered to the memory on the Graphics Card, �����Դ�
	GLuint VAO, VBO;  //VAO:Vertex Array Object   VBO:Vertex Buffer Object ������
	glGenVertexArrays(1, &VAO);  // ���� VAO
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);  // �赱ǰֱ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //VAO �� VBO �ɶԳ���
										 // transfer the data��������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // ��̬���ʣ��������޸�
	transfer1();//�Ѿ�����װ

				//�ڶ���
	GLuint VAO1, VBO1;  //VAO:Vertex Array Object   VBO:Vertex Buffer Object ������
	glGenVertexArrays(1, &VAO1);  // ���� VAO
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);  // �赱ǰֱ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);  //VAO �� VBO �ɶԳ���
										  // transfer the data��������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // ��̬���ʣ��������޸�
	transfer1();//�Ѿ�����װ

				//transfer the index
				//GLuint EBO;
				//glGenBuffers(1, &EBO);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


				//glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glBindVertexArray(0);


				//��������ʼ������ɣ���
	GLuint texture0, texture1, texture2;
	int width, height;
	glGenTextures(1, &texture0);  // ������
	glBindTexture(GL_TEXTURE_2D, texture0); // �� 2D ���� ��ʵ����ʹ�õ�ʱ�����

											//�����Ʒ�ʽ
	parameter();
	//�����ͼ
	// load textures
	// -------------
	/*std::vector<std::string> faces
	{
	"C:/Users/���ȷ�/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/right.jpg",
	"C:/Users/���ȷ�/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/left.jpg",
	"C:/Users/���ȷ�/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/top.jpg",
	"C:/Users/���ȷ�/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/bottom.jpg",
	"C:/Users/���ȷ�/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/back.jpg",
	"C:/Users/���ȷ�/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/front.jpg",
	};*/
	std::vector<std::string> faces
	{
		//"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/right.jpg",
		//"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/left.jpg",
		//"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/top.jpg",
		//"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/bottom.jpg",
		//"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/back.jpg",
		//"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/front.jpg",
		"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/right.jpg",
		"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/left.jpg",
		"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/top.jpg",
		"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/bottom.jpg",
		"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/back.jpg",
		"C:/Users/���ȷ�/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/front.jpg",
	};
	//unsigned int cubemapTexture = loadCubemap(faces);




	// ��ȡ�ļ��� RGBA ��Ϣ
	unsigned char* image0 = SOIL_load_image("./images/lmy/top.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	unsigned char* image1 = SOIL_load_image("./images/lmy/front.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	unsigned char* image2 = SOIL_load_image("./images/lmy/right.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	// ������
	//ָ���������ļ��ռ�
	//��һ�� 0 ��ʾ�� 0 ��, ���� RGBA ��˳��, �ڶ��� 0 ���ù�, ��� unsigned_byte �ĸ�ʽ

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image0);
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap: ͼƬ������
	glBindTexture(GL_TEXTURE_2D, 0);


	//����һ����
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	parameter();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap: ͼƬ������
	glBindTexture(GL_TEXTURE_2D, 0);  //֮ǰ�󶨹�֮�� �ٴΰ󶨲���ı�

									  //����һ����
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	parameter();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap: ͼƬ������
	glBindTexture(GL_TEXTURE_2D, 0);  //֮ǰ�󶨹�֮�� �ٴΰ󶨲���ı�

									  //ʸ������ ---- Accelerator
									  //�������ͼ std::cout �� vans ��
									  //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //���һ����Ϊһ��������������� ��װ���� vertices �������
	glm::mat4 trans; //Ĭ����һ��4��4��λ����
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); //��λ�����һ��λ���������ݸ�glm::translate
																//vec = trans * vec;
																//std::cout << vec.x << vec.y << vec.z << std::endl;
																//���ú���
																//��ת��
																//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
																//���ţ�
																//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
																//ƽ�ƣ�
																//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	ourShader.Use();
	ourShaderNew.Use();
	//glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture0"), 0);
	//glUniform1i(glGetUniformLocation(ourShader.Program, "texture1"), 1);
	// ��ͼ
	//while (!glfwWindowShouldClose(window) && (i++))
	while (!glfwWindowShouldClose(window))
	{
		//glfwPollEvents();  // �������¼�ϵͳ��ȡ���������� / ���Ȳ��� Teacher told us to do so 
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//��ȡ����
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // ���ڱ�����ɫ��RGB�����һ����͸����
											   //glClear(GL_COLOR_BUFFER_BIT); //�����Ǿͻ����ҵ��¶����� �Ҿͱ�ע����...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//��� �����Ȼ���

														   //Bind the shader
														   //glUseProgram(shaderProgram);  // ʹ�õ�ɫ��, ��ע��
														   //ourShader.Use();
														   //cubeshader.Use();

														   //texture
														   //����任�Ͱ������������֧��任�������
														   //glActiveTexture(GL_TEXTURE0);  // �� 0 ��λ��, ��Ӧ�ľ��� frag ����� uniform, ���й����д�
														   //glBindTexture(GL_TEXTURE_2D, texture0);  // ��Ҫ
														   //glUniform1i(glGetUniformLocation(ourShader.Program, "texture0"), 0);
		ourShader.Use();
		ourShaderNew.Use();
		if (int(glfwGetTime()) % 2 < 1) {
			//std::cout << int(glfwGetTime()) << std::endl; //��ʱ��
			glActiveTexture(GL_TEXTURE0);  // �� 0 ��λ��, ��Ӧ�ľ��� frag ����� uniform, ���й����д�
			glBindTexture(GL_TEXTURE_2D, texture0);  // ��Ҫ
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);  // �� 0 ��λ��, ��Ӧ�ľ��� frag ����� uniform, ���й����д�
			glBindTexture(GL_TEXTURE_2D, texture1);  // ��Ҫ
		}
		glUniform1i(glGetUniformLocation(ourShader.Program, "texture0"), 0);



		//��������ʸ����
		//glm::mat4 trans;
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));//λ��
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //��ת
		//��ת��������ʲô��
		//����仯��һ��λ�ò����ı仯��û��ƽ�Ƶ���ת���ı���
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); //����

		//���� ����������ת���Σ��Ȱ�����ע�͵���
		//glm::mat4 trans; //nei ���û�еĻ� ����value_ptr����
		//������ ��һ���������
		//�ֱ��� x �ᡢy �ᡢz �������ת�������Ϊ 1.0f�������ƺ� [����] (1,1,1) ת
		//trans = glm::rotate(trans, glm::radians(20.0f) * static_cast<GLfloat>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));

		//3D�����԰ȣ�
		//ģ�;���
		//glm::mat4 model;
		glm::mat4 model = glm::mat4(1.0f);
		//ͨ������������������ģ�;������ǽ��ö�������任����������
		//��������x����ת��ʹ������������ڵ���һ����
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//�۲���� �����Լ�ת������ĸ�
		//glm::mat4 view;
		// �������ǽ�����������Ҫ�����ƶ������ķ������ƶ���
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//ͶӰ���� ͸��ͶӰ
		glm::mat4 projection = glm::mat4(1.0f);;
		//projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.3f, 1000.0f);
		//�ҿ��Ա��� ����ı䲻����Ŷ
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		//�����ģ�2D �� 3D!
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));


		//�ڸ����������¼��һ�� ��ʵ��Ҫ����ƭ�۾�
		//��Ҫ������z���Ǵ���Ļָ����ģ��������ϣ�����������ƶ������Ǿ�����z����������ƶ���
		//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);//����������� ��(0, 0, 0)
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);//ʹ�ü���ʵ�������任
		//���� �ʼ������
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		//����
		//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		//���������Ϊ������ƶ��ṩ����
		//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		//Look at ����һ������(Look at)����Ŀ��Ĺ۲����
		//������� �Զ� �����Լ�ת��
		//glm::mat4 view;
		//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//��ת ��Ծ �ұ�����
		//float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//glm::mat4 view;
		//�����Լ�д OpenGL�ṩ��lookat����������
		//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix(); //��ͷ�ļ����װ������


												 //Ϊ������ƶ��ṩ����
												 //�������Ƚ������λ������Ϊ֮ǰ�����cameraPos��
												 //�����ǵ�ǰ��λ�ü������Ǹոն���ķ���������
												 //�����ܱ�֤����������ô�ƶ������������ע����Ŀ�귽��
												 //view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

												 //����������ɫ���� �۾��ܿ����ˣ�
												 //unsigned int transformLoc = glGetUniformLocation(ourShader.Program, "transform");
												 //GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform"); //ͻȻ���� GLuint �� unsigned int���͹�
												 //Matrix4fv��4ά����fv����������
												 //transLoc������ uniform ��λ��
												 //1������ֻ����һ������
												 //GL_FALSE�����Ծ�������û�����������������к��С�GLM ��Ĭ�ϲ��־������������Բ�����Ҫ�û�����
												 //���ֱ�Ӹ��� transform �������飬��������Ҫ�Ѿ���ת��������ĸ�ʽ���ݡ�
												 //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


												 //ourShader.Use();
												 //glUniform1i(glGetUniformLocation(ourShader.Program, "texture0"), 0);
												 //glUniform1i(glGetUniformLocation(ourShader.Program, "texture1"), 1);
												 //cubeshader.Use();
												 //glUniform1i(glGetUniformLocation(cubeshader.Program, "cubemapTexture"), 0);  // ��Ӧ frag ����� ourTexture0, �� Program ���� ourTexture0
												 /*cubeshader.Use();
												 glUniform1i(glGetUniformLocation(ourShader.Program, "cubemapTexture"), 0);*/



												 //Draw the triangle
												 //��3D������ɫ�� ����
												 //�����������ǵøĸ� ����«��ư ʵ��model/view/projectionLoc
												 //int modelLoc = glGetUniformLocation(ourShader.Program, "model");
												 //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewlLoc = glGetUniformLocation(ourShader.Program, "view");
		//int viewlLoc = glGetUniformLocation(cubeshader.Program, "view");
		glUniformMatrix4fv(viewlLoc, 1, GL_FALSE, glm::value_ptr(view));

		//int projectionLoc = glGetUniformLocation(cubeshader.Program, "projection");
		int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//�����̼��ģ���  �����Ǵ󵨵���չ���ǵ�2Dƽ��Ϊһ��3D������


		//glBindVertexArray(VAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		glBindVertexArray(VAO);  // ʹ�� VAO��ֱ�Ӱ�


								 //��10���� 
		for (unsigned int j = 0; j < 3; j++)
		{
			//calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[j]);
			float angle = 20.0f * j;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			//{
			//model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.5f));
			//}
			int modelLoc = glGetUniformLocation(ourShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//�� һ����
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, cubePositions[9]);
		//model = glm::translate(model, glm::vec3(0.0f, -0.5*pos + 0.5f*g*pos*pos, 0.0f));
		//float angle = 20.0f * 0;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!touchDown) {
				i += deltaTime;
				std::cout << "UP: t is : " << i << std::endl;
				//std::cout << "DOWN: t is : " << i << std::endl;
				//model = glm::translate(model, glm::vec3(0.0f, 0.5*i-0.5f*g*i*i, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, -0.5*i + 0.5f*g*i*i, 0.0f));
				if (i >= 0.6f) {
					touchDown = true;
				}
			}
			if (touchDown) {
				i -= deltaTime;
				std::cout << "DOWN: t is : " << i << std::endl;
				//std::cout << "UP: t is : " << i << std::endl;
				//model = glm::translate(model, glm::vec3(0.0f, -0.5f*g*i*i, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, 0.5f*g*i*i, 0.0f));
				if (i < 0) {
					//�ճ�0�� ������̶�
					touchDown = false;
				}
			}


		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		{
			touchDown = false;
			pos = i;
			i = 0;
		}
		//int modelLoc = glGetUniformLocation(cubeshader.Program, "model");
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawArrays(GL_TRIANGLES, 0, 12);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//����1-st���������Ҿ��Ǽ� ..�Һ���ֻ�ܻ�EBO
		//3D ������36�����㰡��
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // ���
		glBindVertexArray(0);

		//�����
		ourShaderNew.Use();
		glUniform1i(glGetUniformLocation(ourShaderNew.Program, "texture0"), 0);
		glBindVertexArray(VAO1);  // ʹ�� VAO��ֱ�Ӱ�
		glActiveTexture(GL_TEXTURE0);  // �� 0 ��λ��, ��Ӧ�ľ��� frag ����� uniform, ���й����д�
		glBindTexture(GL_TEXTURE_2D, texture2);  // ��Ҫ ��ʼ��
		glm::mat4 modelnew = glm::mat4(1.0f);
		modelnew = glm::translate(modelnew, cubePositions[9]);
		modelnew = glm::translate(modelnew, glm::vec3(0.0f, -0.5*pos + 0.5f*g*pos*pos, 0.0f));
		float angle = 20.0f * 0;
		modelnew = glm::rotate(modelnew, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		int modelLoc = glGetUniformLocation(ourShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelnew));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);  // ����˫����л�����ʾһ������һ���ڻ������������
		glfwPollEvents();
	}
	//��������  ����  ȫ������
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	//glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	//glDeleteTextures(1, &cubemapTexture);

	glfwTerminate();
	return 0;
}


//�������庯���� step off��
//������װ
void transfer1() {
	//set the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	//0: ��Ӧ��ɫ���� location ��ֵ��3: ��Ӧ vec3 ��������GL_FLOAT: �����ͣ�GL_FALSE:��5*sizeof(GLfloat): ��Ӧ Buffer �ﴫ�����ݣ�(GLvoid*)0: �ӵ� 0 ��λ�ÿ�ʼ

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	//1: ��Ӧ��ɫ���� color ��ֵ��2: ��Ӧ���� vec2 ��������GL_FLOAT: �����ͣ�GL_FALSE:��5*sizeof(GLfloat): ÿ�ο�Խ 5 ����(GLvoid*) (3 * sizeof(GLfloat)): �ӵ� 0 ��λ�ÿ�ʼ
	glEnableVertexAttribArray(1);
}

//������װ
void parameter() {
	// ��ά: S���� T����   ��ά: S T R
	//i: ����    f: ������
	//��һ������ָ��������Ŀ�꣺����ʹ�õ��� 2D �����������Ŀ���� GL_TEXTURE_2D��
	//�ڶ���������Ҫ����ָ�����õ�ѡ����Ӧ�õ������ᡣ���Ǵ������õ���WRAPѡ�����ָ��S��T�ᡣ
	//���һ��������Ҫ���Ǵ���һ�����Ʒ�ʽ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // ��������
																   //�������
																   //GL_NEAREST:ѡȡ��������ĵ㣬��ע�������ȿ��ԣ�����Ե��ģ����
																   //GL_LINEAR:���忴Ч����΢ģ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//��ȡ�������� ����ţ�
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	/*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	camera.ProcessKeyboard(SPACE, deltaTime);*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//����ͬ��ͼƬ
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

//
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		//unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cube texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

	return textureID;
}