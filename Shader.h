#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
class Shader
{
public:
	//program ID
	unsigned int ID;
	//constructor reads and builds Shader
	Shader(const char* vertexPath, const char* fragmentPath);
	//use/activate shader
	void use();
	//utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};
#endif
