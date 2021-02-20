#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

	Shader ourShader("shader.vert", "shader.frag");
	glEnable(GL_DEPTH_TEST);
	// enables depth test
	
// Generating and Loading Textures --------------------------------------------------------
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// sets the given texture as currently bound texture. Any subsequent texture operations will affect the currently bound texture object.

	//glTexParameteri sets texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat tex on s axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // repeat tex on t axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filter when downsclaling(minifying) image.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear filter when upscaling(magnifying) image.

	stbi_set_flip_vertically_on_load(true);
	// it is a flag so we have to set it once and it will be set for whole program and should be used before loading image!.
	// Image is flipped on load because opengl expects y axis 0 to be on bottom. but usually we take it on top so we have to flip image.


	// Opening stbi_image-----------------------------------------------------------
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
	// first argument takes in location of texture file.
	// second argument provides width
	// third argument provides height
	// The last argument of the function is useful if for example you want to load only the R, G, B channels from a four channel, PNG, image and ignore the transparency channel. If you want to load the image as is, pass 0 as the last parameter of the load function.

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	// This function generates the texture image for currently bound texture object
	// first argument - specifies the texture target; setting it to GL_TEXTURE_2D will generate a texture on currently bound texture object at the same target(so any textures bound to GL_TEXTURE_1D or any other target will not be affected).
	// second argument - specifies mipmap level for which we want to create a texture for if you want to set each mipmap level manually, but we'll leave it at base level 0.
	// third argument - third argument tells in what format we want to store the texture. Our image as rgb value so we'll store it as GL_RGB.
	// fourth argument and fifth argument - sets the width and height for resulting texture. We stored this earlier.
	// sixth argument - should always be zero(some legacy stuff).
	// seventh argument and eighth argument - specifies the format and datatype of the source image. We loaded the image with rgb format and stored it chars(bytes).
	// ninth argument - actual image data.
	glGenerateMipmap(GL_TEXTURE_2D);
	// creates all the required mipmaps. If we want to use those mipmaps we have to manually apply mipmaps by incrementing the second argument of glTexImage2D function.
	}

	else {
		std::cout << "Failed to load image data" << std::endl;
	}


	stbi_image_free(data);
	// as we have used image to generate textures and mipmaps. IT's a good practice to free image memory.

	// Creating second texture
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("Textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture image" << std::endl;
	}
	stbi_image_free(data);

	// ====================================================================================//
	// Vertices in 3d space
	float vertices[] = {
		// postions         //textures
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f
	};


	unsigned int indices[] = {  // note that we start from 0!
		// first face
		0, 1, 2,
		0, 2, 3,
		// second face
		4, 0, 3,
		7, 4, 3,
		// third face
		7, 4, 5,
		6, 7, 5,
		// fourth face
		6, 2, 1,
		1, 5, 6,
		// fifth face
		9, 3, 2,
		8, 9, 2,
		//sixth face
		11, 0, 1,
		10, 11, 1
	};

	glm::vec3 cubePos[]{
		glm::vec3(0.3f, 0.1f, -0.5f),
		glm::vec3(-0.5f, 0.3f, -1.0f),
		glm::vec3(-0.6f, 0.2f, -0.3f),
		glm::vec3(2.0f, 3.0f, 6.0f),
		glm::vec3(2.3f, 3.2f, 4.0f)
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	////This function specifies how OpenGL should interpret this data whenever a drawing call is made.
	////first Parameter: specifies which vertex attribute we want to configure. REMEMBER ? we specified the location of position vertex attribute in the vertex shader with "layout (location=0)". This sets the location of the vertex attribute to 0 and since we want to pass data to this location we set it as 0
	//// second Parameter: specifies the size of vertex attribute. Vertex attribute is a vec3 so we put 3.
	//// third Parameter: specifies the type of data which in this case is GL_FLOAT.
	//// fourth Parameter: specifies if we want the data to be normalised. In our case we entered normalised data so no need and set it to GL_FALSE
	//// fifth Parameter: it is known as stride and tells us the space between consecutive vertex attributes. As the next set of position data is 3*sizeof(float) away in the memory. So we write 3*sizeof(float). We could've set this to 0 and let the OpenGL determine the stride.(This only works when values are tightly packed or in a array.). We have to carefully determine the spacing between vertex attribute
	//// sixth parameter: this is the offset of where the position data begins in buffer. Since the position data is at the start of array this value is just 0. We will explore it later on.
	
	glEnableVertexAttribArray(0); //Enables to draw the image
	//glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//first argument: from which VAO index to start

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);// Bounds the created buffer object (that have specific id) with the target type object (GL_ARRAY_BUFFER (vertex array buffer))
	//// OpenGL allows us to bind several buffer types at once as long as they have a different buffer type.
	//// From this point any buffer call we make, it will be used to configure currently bound buffer which is VBO.

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	ourShader.use();// don't forget to activate / use the shader before setting uniforms.
	ourShader.setInt("ourTexture", 0);
	ourShader.setInt("ourTexture2", 1);

	
	while (!glfwWindowShouldClose(main_window)) {

		// Check for input--------------------------------------------------------------------------
		processInput(main_window);

		//rendering commands here-------------------------------------------------------------------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Clear the screen using this color.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //to clear the color buffer.

		// bind textures on corresponding texture units.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		glBindVertexArray(VAO);
		
		glm::mat4 transMat = glm::mat4(1.0f);
		transMat = glm::translate(transMat, glm::vec3(0.0f, 0.0f, -0.4f));
		transMat = glm::rotate(transMat, (float)glfwGetTime(), glm::vec3(1.0f, 0.5f,-0.2f));
//		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "transMat"), 1, GL_FALSE, glm::value_ptr(transMat));
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(55.0f), (float)800 / 600, 0.1f, 100.0f);
		ourShader.setMat4("model", transMat);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);
		//glDrawArrays(GL_TRIANGLES, 0, 3); // first parameter = OpenGL primitive type
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // draws object from indices provided


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
	

	glfwTerminate();
	return 0;
}
