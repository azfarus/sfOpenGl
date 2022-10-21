#include"StandardFunctions.h"
#include"vertexShader.h"



void pushVectors(std::vector<float>& v, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec2 d) {
	v.push_back(a.x);
	v.push_back(a.y);//position
	v.push_back(a.z);
	v.push_back(b.x);
	v.push_back(b.y);//color
	v.push_back(b.z);
	v.push_back(c.x);
	v.push_back(c.y);//normal
	v.push_back(c.z);
	v.push_back(d.x);
	v.push_back(d.y);

}

void pushvals(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness) {
	// z = x + y 
	v.clear();


	const float factor = (abs(fin_x - init_x) / sharpness);
	for (float x = init_x; x < fin_x; x += factor) {
		for (float y = init_y; y < fin_y; y += factor) {

			glm::vec3 vertex[4];
			float arrx[] = { factor , 0 , factor , 0 };
			float arry[] = { factor , factor , 0 , 0 };
			float m, n;
			for (int i = 0; i < 4; i++) {


				m = x + arrx[i];
				n = y + arry[i];
				vertex[i] = glm::vec3(m, n, asin(sin(m) + sin(n)));
			}
			glm::vec3 vex01, vex02, vex31, vex32, cp0, cp3, color;
			color = glm::vec3(1, 1, 1);
			vex01 = vertex[1] - vertex[0];
			vex02 = vertex[2] - vertex[0];
			vex31 = vertex[1] - vertex[3];
			vex32 = vertex[2] - vertex[3];

			cp0 = glm::cross(vex31, vex32);
			cp3 = glm::cross(vex02, vex01);
			cp0 = glm::normalize(cp0);
			cp3 = glm::normalize(cp3);
			glm::vec2 uv(0, 0);
			pushVectors(v, vertex[0], color, cp0 ,uv );
			pushVectors(v, vertex[1], color, cp0,uv);
			pushVectors(v, vertex[2], color, cp0, uv);

			pushVectors(v, vertex[3], color, cp3, uv);
			pushVectors(v, vertex[1], color, cp3, uv);
			pushVectors(v, vertex[2], color, cp3, uv);



		}
	}



}

GLuint shaderSet() {						//Compile and bind the shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);



	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	return shaderProgram;
}

sf::ContextSettings windowInit() {
	sf::ContextSettings set;
	set.majorVersion = 3;
	set.minorVersion = 2;
	set.antialiasingLevel = 8;
	set.attributeFlags = sf::ContextSettings::Core;
	set.depthBits = 24;
	set.stencilBits = 16;

	return set;
}

GLuint loadTexture(std::string filepath , int flip ) {
	stbi_set_flip_vertically_on_load(flip);
	int w, h, channels;
	unsigned char * data = stbi_load(filepath.c_str(), &w, &h, &channels, 0);
	GLuint texture = 0;
	if (data) {
		std::cout << "Loaded " << filepath << std::endl;
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else{
			std::cout << "Failed to load " << filepath << std::endl;
		
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	stbi_image_free(data);
	return texture;

	
	
}