#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Creating Callback for windows resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// To check for inputs given by user
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {

	// Creating Vertex Shader
	const char* vertexShaderCode = "#version 420 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"	ourColor = aColor;\n"
		"}\0";

	// Creating Fragment Shader
	const char* fragmentShaderCode = "#version 420 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"void main(){\n"
		"	FragColor = vec4(ourColor, 1.0);\n"
		"}\0";


	// Initialising glfw and creating window context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* main_window = glfwCreateWindow(800, 600, "RockingEngine", NULL, NULL); // returns NULL if gone wrong.

	if (main_window == NULL) {
		std::cout << "Failed to Create window using glfw." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(main_window);

	// Initialising Glad here
	// GLAD manages function pointers for OpenGL so we want to initialise GLAD before we call any OpenGL function.
	// We pass the GLAD the function to load the address of the OpenGL function pointers which are OS Specific.
	// GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
	// just in case https://computergraphics.stackexchange.com/questions/8639/this-function-syntax-is-confusing-me
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD !" << std::endl;
		return -1;
	}

	// Setting up viewport
	glViewport(0, 0, 800, 600);

	// Checking for window resize
	glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback);

	//// =============================================================================================== //

	// Compiling vertex Shader code written above
	unsigned int vertexShader; // Shader Object referenced by an id thats' why unsigned int
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // we provide the type of shader as an argument to glCreateShader. It will create a shader of provided type and assign a specific id to object.

	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	// Attach shader source code to shader object
	// first argument = shader object to compile
	// second argument = how many strings we are passing as source code
	// third argument = actual shader source code of the vertex
	// fourth argument = array of string lengths, if null then strings have to be null terminated.

	glCompileShader(vertexShader); // Compiles the vertex Shader

	// Checking for compiler errors
	int success;
	char infoLog[512]; // storage container for error messages
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	// allows the developer to query a shader for information given as object parameter.
	// first argument = specifies the shader object to be queried.
	// second argument = specifies object parameter
	// third argument = returns the requested object parameter in the given GLint array.

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		// returns the information log for the requested object
		// first argument = shader object whose information log is queried.
		// second argument = size of character buffer for storing returned information log.
		// third argument = returns the length of string returned in info log.
		// fourth argument = array of characters that is used to return the information log.

		std::cout << "Error Shader Vertex Compilation Failed" << infoLog << std::endl;
	}
	// ============================================================================================//

	//Compiling Fragement Shader code written above
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	// Checking for compiling errors
	int fragSuccess;
	char fragInfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);

	if (!fragSuccess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragInfoLog);
		std::cout << "Error Shader Fragment Compilation Failed" << fragInfoLog << std::endl;
	}
	//====================================================================================================//

	//Creating Shader Program object to link all compiled shaders and render them in a scene.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // It creates program and returns id reference to the newly created program object. returns 0 if error occured.

	glAttachShader(shaderProgram, vertexShader); // Attaches the shader to program
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // Links all the attached shaders with program
	// During linking ouput of one should match with input of other shader. If anything goes wrong linking fails.

	// Checking for linking errors
	int linkingSuccess;
	char linkingInfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkingSuccess);
	if (!linkingSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, linkingInfoLog);
		std::cout << "Error Program Linking Failed" <<linkingInfoLog << std::endl;
	}

	// Using Shader object
	//glUseProgram(shaderProgram); // This function sets the given program object as the current shader program to use for subsequent drawing commands.

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Till now we have sent the vertex input data to the GPU and instructed GPU how it should process the vertex data within a vertex and fragment shader.
	// OpenGL yet doesn't know how it should interpret the vertex data in memory and how it should connect vertex data to vertex shader's attribute

	// ====================================================================================//
	// Vertices in 3d space
	float vertices[] = {
		// postions           //colors
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f , 1.0f
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	// ============================================================================================= //

	//unsigned int VBO; // Vertex Buffer Object can store large number of vertices in the GPU's memory. The advantage of using those buffer objects is that we can send large batches of data all at once to the graphics card instead of sending one by one. (Movement of data between cpu and gpu is very slow.)

	//glGenBuffers(1, &VBO); // Provides a specific id for created buffer.
	//// first parameter = number of buffers to create
	//// unique id assigned to VBO

	//glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bounds the created buffer object (that have specific id) with the target type object (GL_ARRAY_BUFFER (vertex array buffer))
	//// OpenGL allows us to bind several buffer types at once as long as they have a different buffer type.
	//// From this point any buffer call we make, it will be used to configure currently bound buffer which is VBO.

	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	//// Function used to copy user defined data into the currently bound buffer.
	//// first parameter = type of buffer of we want to copy data into
	//// second parameter = size of data(in bytes)
	//// third parameter = actual data we want to send
	//// fourth parameter = how graphics card want to manage the given data.
	//	// GL_STREAM_DRAW: data is set only once and used by GPU  atmost a few times.
	//	// GL_STATIC_DRAW: data is set only once and used by GPU many times.
	//	// GL_DYNAMIC_DRAW: data is changed a lot of times and is used by GPU many times.

	//// Finally, we stored the data within memory on the graphics card as managed by a vertex buffer object named VBO.

	//// =============================================================================================== //

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	////This function specifies how OpenGL should interpret this data whenever a drawing call is made.
	////first Parameter: specifies which vertex attribute we want to configure. REMEMBER ? we specified the location of position vertex attribute in the vertex shader with "layout (location=0)". This sets the location of the vertex attribute to 0 and since we want to pass data to this location we set it as 0
	//// second Parameter: specifies the size of vertex attribute. Vertex attribute is a vec3 so we put 3.
	//// third Parameter: specifies the type of data which in this case is GL_FLOAT.
	//// fourth Parameter: specifies if we want the data to be normalised. In our case we entered normalised data so no need and set it to GL_FALSE
	//// fifth Parameter: it is known as stride and tells us the space between consecutive vertex attributes. As the next set of position data is 3*sizeof(float) away in the memory. So we write 3*sizeof(float). We could've set this to 0 and let the OpenGL determine the stride.(This only works when values are tightly packed or in a array.). We have to carefully determine the spacing between vertex attribute
	//// sixth parameter: this is the offset of where the position data begins in buffer. Since the position data is at the start of array this value is just 0. We will explore it later on.

	//// =============================================================================================================//

	//// Vertex Attribute takes it data from the memory managed by VBO. But which VBO as we can have multiple VBO ?
	//// VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer .
	//// Since VBO is still bound before calling glVertexAttribPointer, vertex attribute 0 is now associated with its vertex data.

	//// =============================================================================================================//

	//// Now we specified how OpenGL should interpret the vertex data we should enable the vertex attribute with glEnableVertexAttribArray giving the vertex attribute {{location}} as its argument
	//glEnableVertexAttribArray(0);

	//// Creating Vertex Array Object 
	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO); // Generates vertex array object
	//glBindVertexArray(VAO);
	//glBindVertexArray(0);
	
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);// Generates vertex array object
	glGenBuffers(1, &VBO);// Provides a specific id for created buffer.
	//// first parameter = number of buffers to create
	//// unique id assigned to VBO

	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	glBindBuffer(GL_ARRAY_BUFFER, VBO);// Bounds the created buffer object (that have specific id) with the target type object (GL_ARRAY_BUFFER (vertex array buffer))
	//// OpenGL allows us to bind several buffer types at once as long as they have a different buffer type.
	//// From this point any buffer call we make, it will be used to configure currently bound buffer which is VBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//// Function used to copy user defined data into the currently bound buffer.
	//// first parameter = type of buffer of we want to copy data into
	//// second parameter = size of data(in bytes)
	//// third parameter = actual data we want to send
	//// fourth parameter = how graphics card want to manage the given data.
	//	// GL_STREAM_DRAW: data is set only once and used by GPU  atmost a few times.
	//	// GL_STATIC_DRAW: data is set only once and used by GPU many times.
	//	// GL_DYNAMIC_DRAW: data is changed a lot of times and is used by GPU many times.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	////This function specifies how OpenGL should interpret this data whenever a drawing call is made.
	////first Parameter: specifies which vertex attribute we want to configure. REMEMBER ? we specified the location of position vertex attribute in the vertex shader with "layout (location=0)". This sets the location of the vertex attribute to 0 and since we want to pass data to this location we set it as 0
	//// second Parameter: specifies the size of vertex attribute. Vertex attribute is a vec3 so we put 3.
	//// third Parameter: specifies the type of data which in this case is GL_FLOAT.
	//// fourth Parameter: specifies if we want the data to be normalised. In our case we entered normalised data so no need and set it to GL_FALSE
	//// fifth Parameter: it is known as stride and tells us the space between consecutive vertex attributes. As the next set of position data is 3*sizeof(float) away in the memory. So we write 3*sizeof(float). We could've set this to 0 and let the OpenGL determine the stride.(This only works when values are tightly packed or in a array.). We have to carefully determine the spacing between vertex attribute
	//// sixth parameter: this is the offset of where the position data begins in buffer. Since the position data is at the start of array this value is just 0. We will explore it later on.
	
	glEnableVertexAttribArray(0); //Enables to draw the image
	glEnableVertexAttribArray(1);
	//first argument: from which VAO index to start

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);// Bounds the created buffer object (that have specific id) with the target type object (GL_ARRAY_BUFFER (vertex array buffer))
	//// OpenGL allows us to bind several buffer types at once as long as they have a different buffer type.
	//// From this point any buffer call we make, it will be used to configure currently bound buffer which is VBO.

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	while (!glfwWindowShouldClose(main_window)) {

		// Check for input--------------------------------------------------------------------------
		processInput(main_window);

		//rendering commands here-------------------------------------------------------------------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Clear the screen using this color.
		glClear(GL_COLOR_BUFFER_BIT); //to clear the color buffer.


		glUseProgram(shaderProgram);
		float timeValue = glfwGetTime();
		float greenColorValue = sin(timeValue)/2.0f + 0.5f;
		int vertexUniformLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexUniformLocation, 0.0f, greenColorValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); // first parameter = OpenGL primitive type
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draws object from indices provided


		// second parameter = starting index of vertex array we'd like to draw
		// third parameter = number of vertices we want to draw

		// check and call events and swap buffers here ---------------------------------------------
		glfwSwapBuffers(main_window);
		// will swap the color buffer that is used to render to during this render iteration and show it as the output to the screen.
		// Search for Double Buffer for more information.

		glfwPollEvents(); // checking for key events or mouse movements.
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
