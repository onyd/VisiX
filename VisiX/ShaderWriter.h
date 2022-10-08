#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "ShaderClass.h"

#define INDENT = "    ";

static const enum GLSLVarMode {
	GLSL_LAYOUT_IN = 0,
	GLSL_IN,
	GLSL_OUT,
	GLSL_UNIFORM
} _GLSLVarMode;

static const char* glslVarModeIdent[] {
	"layout (location = {}) in",
	"in",
	"out",
	"uniform"
};

const enum GLSLType {
	GLSL_FLOAT = 0,
	GLSL_INT,
	GLSL_VEC2,
	GLSL_VEC3,
	GLSL_VEC4,
	GLSL_MAT2,
	GLSL_MAT3,
	GLSL_MAT4,
};

static const char* glslTypeIdent[] = {
	"float",
	"int",
	"vec2",
	"vec3",
	"vec4",
	"mat2",
	"mat3",
	"mat4"
};

class BasePositionEffect;
class BaseColorEffect;

class ShaderElementData;
class ShaderVariableData;
class ShaderRoutineData;

typedef std::unordered_map<const char*, std::shared_ptr<ShaderVariableData>> var_env_t;
typedef std::unordered_map<const char*, std::shared_ptr<ShaderRoutineData>> func_env_t;

class ShaderVariableData {
private:
	const GLSLType type;

public:
	ShaderVariableData(const GLSLType type);

	virtual void generateCode(const char* identifier, std::ostringstream& codeStream) const = 0;

	const char* getType() const;

	inline virtual bool isGlobalVariable() = 0;
};

class ShaderGlobalVariable : public ShaderVariableData {
protected:
	const int location;
	const GLSLVarMode mode;

public:
	ShaderGlobalVariable(const GLSLVarMode mode, const GLSLType type, const int location = -1);

	const char* getMode() const;

	void generateCode(const char* identifier, std::ostringstream& codeStream) const;

	inline virtual bool isGlobalVariable() {
		return true;
	}
};

class ShaderVariableInitialization {
private:
	const char* initExpr;

public:
	void generateCode(std::ostringstream& codeStream) const;
};

class ShaderMainVariable : public ShaderVariableData {
private:
	ShaderVariableInitialization initialization;

public:
	ShaderMainVariable(GLSLType type, ShaderVariableInitialization& init);

	void generateCode(const char* identifier, std::ostringstream& codeStream) const;

	inline virtual bool isGlobalVariable() {
		return false;
	}
};

class ShaderRoutineData {
private:
	std::vector<const char*> vars;
	const char* body;

public:
	ShaderRoutineData(const char* body)
		: body(body)
	{
	}

	ShaderRoutineData(std::initializer_list<const char*> vars, const char* body)
		: ShaderRoutineData(body)
	{
		for (auto& a : vars) {
			this->vars.push_back(a);
		}
	}

	void generateCode(const char* identifier, const var_env_t& var_env, std::ostringstream& codeStream) const;

};

class ShaderMain {
private:
	const char* body;

public:
	ShaderMain();

	ShaderMain(const char* code);

	void generateCode(std::ostringstream& codeStream) const;
};

class ShaderWriter {
public:
	ShaderWriter();

	void addVariable(const char* identifier, std::shared_ptr<ShaderVariableData> data);

	void addVariable(GLSLType type, const char* identifier, ShaderVariableInitialization& init);

	void addVariable(GLSLVarMode mode, GLSLType type, const char* identifier, const int location=-1);

	void addRootine(const char* identifier, std::initializer_list<const char*> vars, const char* body);

	void setMain(ShaderMain& mainCode);

	void updateShader();

	void saveShader(const char* filename);

	std::string getShaderSource();
	
private:
	std::ostringstream shaderCode;

	var_env_t var_env;
	func_env_t func_env;
	ShaderMain main;

	bool upToDate = true;
};
