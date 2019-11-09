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

const GLint WIDTH = 1000, HEIGHT = 1000;  // 新建窗口
										  // timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
//函数声明 定义在最下方  这就是为了获取用户输入与鼠标操作
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
	//OpenGL 版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 窗口设置
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 用的是新版的 OpenGL 3.3
																	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  // 改为 GL_TRUE，改变窗口，纵横比会变
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "B17040425", nullptr, nullptr);  // 窗口名字改成自己的学号

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// next two lines are for mac retina display
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);  // 获取窗口大小

	glfwMakeContextCurrent(window);  // 可以新建很多 window
									 //回调函数
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
	//glViewport(0, 0, screenWidth, screenHeight);  // 从（0,0）开始画点，直到 WIDTH 和 HEIGHT
	glEnable(GL_DEPTH_TEST);//这一句 可厉害了 我找了俩小时可找着是你不在 开启深度测试
							//vs 是顶点调色器，frag 是边缘调色器
	Shader ourShader = Shader("shader.vs", "shader.fs");
	Shader ourShaderNew = Shader("shader.vs", "shader.fs");
	//Shader cubeshader("cubemaps.vs", "cubemaps.fs");
	//第一个纹理开始辽
	//GLfloat vertices[] = {
	//	//first triangle      //texture
	//	0.5f, 0.6f, 0.0f,      1.0f, 1.0f,  // 右上角
	//	0.5f, -0.6f, 0.0f,     1.0f, 0.0f,  // 右下角
	//	-0.5f, -0.6f, 0.0f,    0.0f, 0.0f,  // 左下角
	//	-0.5f, 0.6f, 0.0f,     0.0f, 1.0f   // 左上角
	//};
	//玩儿大的 3D
	//立方体
	GLfloat vertices[] = {
		//坐标                 //纹理
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
	//四棱锥
	//GLfloat vertices[] = {
	////坐标                 //纹理
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
	//3D你得加这个  Z缓冲解决深度问题
	//还得每次都清空深度缓存  在下面吗while循环中添加


	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	//10个立方体
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


	//那咱们再来一个？
	//发现：每一个图片都需要初始化一个纹理 然后按照规定格式进行绑定和初始化！！


	//the date should be transfered to the memory on the Graphics Card, 传到显存
	GLuint VAO, VBO;  //VAO:Vertex Array Object   VBO:Vertex Buffer Object 传数据
	glGenVertexArrays(1, &VAO);  // 创建 VAO
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);  // 设当前直线
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //VAO 和 VBO 成对出现
										 // transfer the data：传数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 静态访问，几乎不修改
	transfer1();//已经被封装

				//第二个
	GLuint VAO1, VBO1;  //VAO:Vertex Array Object   VBO:Vertex Buffer Object 传数据
	glGenVertexArrays(1, &VAO1);  // 创建 VAO
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);  // 设当前直线
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);  //VAO 和 VBO 成对出现
										  // transfer the data：传数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 静态访问，几乎不修改
	transfer1();//已经被封装

				//transfer the index
				//GLuint EBO;
				//glGenBuffers(1, &EBO);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


				//glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glBindVertexArray(0);


				//我们来开始搞纹理吧！！
	GLuint texture0, texture1, texture2;
	int width, height;
	glGenTextures(1, &texture0);  // 绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture0); // 绑定 2D 纹理 其实可以使用的时候调用

											//纹理环绕方式
	parameter();
	//贴多个图
	// load textures
	// -------------
	/*std::vector<std::string> faces
	{
	"C:/Users/周奕飞/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/right.jpg",
	"C:/Users/周奕飞/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/left.jpg",
	"C:/Users/周奕飞/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/top.jpg",
	"C:/Users/周奕飞/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/bottom.jpg",
	"C:/Users/周奕飞/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/back.jpg",
	"C:/Users/周奕飞/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/front.jpg",
	};*/
	std::vector<std::string> faces
	{
		//"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/right.jpg",
		//"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/left.jpg",
		//"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/top.jpg",
		//"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/bottom.jpg",
		//"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/back.jpg",
		//"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/lmy/front.jpg",
		"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/right.jpg",
		"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/left.jpg",
		"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/top.jpg",
		"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/bottom.jpg",
		"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/back.jpg",
		"C:/Users/周奕飞/source/repos/GHomeworkFour/GHomeworkFour/images/skybox/front.jpg",
	};
	//unsigned int cubemapTexture = loadCubemap(faces);




	// 读取文件的 RGBA 信息
	unsigned char* image0 = SOIL_load_image("./images/lmy/top.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	unsigned char* image1 = SOIL_load_image("./images/lmy/front.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	unsigned char* image2 = SOIL_load_image("./images/lmy/right.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	// 贴纹理
	//指定传多大的文件空间
	//第一个 0 表示第 0 层, 按照 RGBA 的顺序, 第二个 0 不用管, 存成 unsigned_byte 的格式

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image0);
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap: 图片金字塔
	glBindTexture(GL_TEXTURE_2D, 0);


	//再来一个！
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	parameter();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap: 图片金字塔
	glBindTexture(GL_TEXTURE_2D, 0);  //之前绑定过之后 再次绑定不会改变

									  //再来一个！
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	parameter();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap: 图片金字塔
	glBindTexture(GL_TEXTURE_2D, 0);  //之前绑定过之后 再次绑定不会改变

									  //矢量操纵 ---- Accelerator
									  //不会玩儿图 std::cout 就 vans 了
									  //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //最后一个置为一方便进行向量操纵 假装我是 vertices 里的数据
	glm::mat4 trans; //默认是一个4×4单位矩阵。
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); //单位矩阵和一个位移向量传递给glm::translate
																//vec = trans * vec;
																//std::cout << vec.x << vec.y << vec.z << std::endl;
																//常用函数
																//旋转：
																//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
																//缩放：
																//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
																//平移：
																//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	ourShader.Use();
	ourShaderNew.Use();
	//glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture0"), 0);
	//glUniform1i(glGetUniformLocation(ourShader.Program, "texture1"), 1);
	// 画图
	//while (!glfwWindowShouldClose(window) && (i++))
	while (!glfwWindowShouldClose(window))
	{
		//glfwPollEvents();  // 把所有事件系统都取过来：键盘 / 鼠标等操作 Teacher told us to do so 
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//获取输入
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 窗口背景颜色，RGB，最后一个是透明度
											   //glClear(GL_COLOR_BUFFER_BIT); //下面那就话把我的事儿做了 我就被注释了...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//这儿 清空深度缓存

														   //Bind the shader
														   //glUseProgram(shaderProgram);  // 使用调色器, 不注释
														   //ourShader.Use();
														   //cubeshader.Use();

														   //texture
														   //不想变换就把下面的条件分支语句换成这个吧
														   //glActiveTexture(GL_TEXTURE0);  // 第 0 个位置, 对应的就是 frag 里面的 uniform, 运行过程中传
														   //glBindTexture(GL_TEXTURE_2D, texture0);  // 需要
														   //glUniform1i(glGetUniformLocation(ourShader.Program, "texture0"), 0);
		ourShader.Use();
		ourShaderNew.Use();
		if (int(glfwGetTime()) % 2 < 1) {
			//std::cout << int(glfwGetTime()) << std::endl; //计时器
			glActiveTexture(GL_TEXTURE0);  // 第 0 个位置, 对应的就是 frag 里面的 uniform, 运行过程中传
			glBindTexture(GL_TEXTURE_2D, texture0);  // 需要
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);  // 第 0 个位置, 对应的就是 frag 里面的 uniform, 运行过程中传
			glBindTexture(GL_TEXTURE_2D, texture1);  // 需要
		}
		glUniform1i(glGetUniformLocation(ourShader.Program, "texture0"), 0);



		//来？爱玩矢量？
		//glm::mat4 trans;
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));//位移
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //旋转
		//旋转的中心是什么？
		//两句变化换一下位置产生的变化：没有平移但旋转中心变了
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); //缩放

		//来吧 试试立体旋转矩形？先把上面注释掉吧
		//glm::mat4 trans; //nei 这句没有的话 下面value_ptr报错
		//来来来 看一看这个妖怪
		//分别绕 x 轴、y 轴、z 轴进行旋转，如果都为 1.0f，就是绕和 [向量] (1,1,1) 转
		//trans = glm::rotate(trans, glm::radians(20.0f) * static_cast<GLfloat>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));

		//3D？试试叭！
		//模型矩阵
		//glm::mat4 model;
		glm::mat4 model = glm::mat4(1.0f);
		//通过将顶点坐标乘以这个模型矩阵，我们将该顶点坐标变换到世界坐标
		//将其绕着x轴旋转，使它看起来像放在地上一样。
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//观察矩阵 让他自己转按下面的搞
		//glm::mat4 view;
		// ！！我们将矩阵向我们要进行移动场景的反方向移动。
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//投影矩阵 透视投影
		glm::mat4 projection = glm::mat4(1.0f);;
		//projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.3f, 1000.0f);
		//我可以变身 上面的变不了身哦
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		//玩儿大的！2D 变 3D!
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));


		//摆个摄像机来记录这一切 其实主要是欺骗眼睛
		//不要忘记正z轴是从屏幕指向你的，如果我们希望摄像机向后移动，我们就沿着z轴的正方向移动。
		//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);//摄像机看哪里 看(0, 0, 0)
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);//使用减法实现向量变换
		//右轴 笔记中详解
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		//上轴
		//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		//这两句可以为摄像机移动提供帮助
		//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		//Look at 创建一个看着(Look at)给定目标的观察矩阵。
		//不好玩儿 自动 让它自己转吧
		//glm::mat4 view;
		//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//旋转 跳跃 我闭着眼
		//float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//glm::mat4 view;
		//不用自己写 OpenGL提供了lookat函数给我们
		//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix(); //在头文件里封装了起来


												 //为摄像机移动提供帮助
												 //我们首先将摄像机位置设置为之前定义的cameraPos。
												 //方向是当前的位置加上我们刚刚定义的方向向量。
												 //这样能保证无论我们怎么移动，摄像机都会注视着目标方向。
												 //view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

												 //把它传到着色器吧 咱就能看到了！
												 //unsigned int transformLoc = glGetUniformLocation(ourShader.Program, "transform");
												 //GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform"); //突然发现 GLuint 是 unsigned int类型哈
												 //Matrix4fv：4维矩阵，fv：浮点类型
												 //transLoc：变量 uniform 的位置
												 //1：代表只传入一个矩阵
												 //GL_FALSE：不对矩阵进行置换，即不交换矩阵的行和列。GLM 的默认布局就是列主序，所以并不需要置换矩阵
												 //最后：直接给出 transform 矩阵数组，这里我们要把矩阵转换成数组的格式传递。
												 //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


												 //ourShader.Use();
												 //glUniform1i(glGetUniformLocation(ourShader.Program, "texture0"), 0);
												 //glUniform1i(glGetUniformLocation(ourShader.Program, "texture1"), 1);
												 //cubeshader.Use();
												 //glUniform1i(glGetUniformLocation(cubeshader.Program, "cubemapTexture"), 0);  // 对应 frag 里面的 ourTexture0, 在 Program 中找 ourTexture0
												 /*cubeshader.Use();
												 glUniform1i(glGetUniformLocation(ourShader.Program, "cubemapTexture"), 0);*/



												 //Draw the triangle
												 //把3D丢进着色器 丢！
												 //丢三个！！记得改改 依葫芦画瓢 实现model/view/projectionLoc
												 //int modelLoc = glGetUniformLocation(ourShader.Program, "model");
												 //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewlLoc = glGetUniformLocation(ourShader.Program, "view");
		//int viewlLoc = glGetUniformLocation(cubeshader.Program, "view");
		glUniformMatrix4fv(viewlLoc, 1, GL_FALSE, glm::value_ptr(view));

		//int projectionLoc = glGetUniformLocation(cubeshader.Program, "projection");
		int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//来个刺激的！！  让我们大胆地拓展我们的2D平面为一个3D立方体


		//glBindVertexArray(VAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		glBindVertexArray(VAO);  // 使用 VAO，直接绑定


								 //来10个！ 
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
		//怂 一个吧
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
					//空出0来 让物体固定
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
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//数字1-st：画几个我就是几 ..我好像只能画EBO
		//3D 这里有36个顶点啊！
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 解绑定
		glBindVertexArray(0);

		//另外的
		ourShaderNew.Use();
		glUniform1i(glGetUniformLocation(ourShaderNew.Program, "texture0"), 0);
		glBindVertexArray(VAO1);  // 使用 VAO，直接绑定
		glActiveTexture(GL_TEXTURE0);  // 第 0 个位置, 对应的就是 frag 里面的 uniform, 运行过程中传
		glBindTexture(GL_TEXTURE_2D, texture2);  // 需要 初始绑定
		glm::mat4 modelnew = glm::mat4(1.0f);
		modelnew = glm::translate(modelnew, cubePositions[9]);
		modelnew = glm::translate(modelnew, glm::vec3(0.0f, -0.5*pos + 0.5f*g*pos*pos, 0.0f));
		float angle = 20.0f * 0;
		modelnew = glm::rotate(modelnew, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		int modelLoc = glGetUniformLocation(ourShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelnew));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);  // 调用双面进行画，显示一个，另一个在画，画面更流畅
		glfwPollEvents();
	}
	//丢掉丢掉  丢掉  全都丢掉
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


//我来定义函数了 step off！
//函数封装
void transfer1() {
	//set the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	//0: 对应调色器里 location 的值；3: 对应 vec3 三个量；GL_FLOAT: 浮点型；GL_FALSE:；5*sizeof(GLfloat): 对应 Buffer 里传的数据；(GLvoid*)0: 从第 0 个位置开始

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	//1: 对应调色器里 color 的值；2: 对应纹理 vec2 两个量；GL_FLOAT: 浮点型；GL_FALSE:；5*sizeof(GLfloat): 每次跨越 5 个；(GLvoid*) (3 * sizeof(GLfloat)): 从第 0 个位置开始
	glEnableVertexAttribArray(1);
}

//函数封装
void parameter() {
	// 二维: S横向 T纵向   三维: S T R
	//i: 整型    f: 浮点型
	//第一个参数指定了纹理目标：我们使用的是 2D 纹理，因此纹理目标是 GL_TEXTURE_2D。
	//第二个参数需要我们指定设置的选项与应用的纹理轴。我们打算配置的是WRAP选项，并且指定S和T轴。
	//最后一个参数需要我们传递一个环绕方式。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // 横向坐标
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // 纵向坐标
																   //纹理过滤
																   //GL_NEAREST:选取靠的最近的点，关注点清晰度可以，但边缘会模糊。
																   //GL_LINEAR:整体看效果稍微模糊。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//获取键盘输入 你敢信？
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

//贴不同的图片
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