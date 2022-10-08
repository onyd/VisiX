#include <stdexcept>
#include <format>
#include <fstream>

#include "ShaderWriter.h"

// ShaderVariableData
ShaderVariableData::ShaderVariableData(const GLSLType type) : type(type)
{
}

const char* ShaderVariableData::getType() const
{
	return glslTypeIdent[type];
}

// ShaderGlobalVariable
ShaderGlobalVariable::ShaderGlobalVariable(const GLSLVarMode mode, const GLSLType type, const int location)
	: mode(mode), ShaderVariableData(type), location(location) 
{
}

const char* ShaderGlobalVariable::getMode() const
{
	return glslVarModeIdent[mode];
}

void ShaderGlobalVariable::generateCode(const char* identifier, std::ostringstream& codeStream)  const 
{
	if (mode == GLSL_LAYOUT_IN) {
		if (location == -1) {
			throw std::runtime_error("ShaderVariableData with layout must have location >= 0");
		}
		codeStream << std::vformat(getMode(), std::make_format_args(location));
	}
	else {
		codeStream << getMode();
	}
	codeStream << " " << getType() << " " << identifier << ";";
} 

// ShaderVariableInitialization
void ShaderVariableInitialization::generateCode(std::ostringstream& codeStream) const
{
	codeStream << initExpr << ";";
}

// ShaderMainVariable
ShaderMainVariable::ShaderMainVariable(GLSLType type, ShaderVariableInitialization& init) : ShaderVariableData(type), initialization(init)
{
}

void ShaderMainVariable::generateCode(const char* identifier, std::ostringstream& codeStream) const
{
	codeStream << getType() << " " << identifier << " = ";
	initialization.generateCode(codeStream);
}

// ShaderRoutineData
void ShaderRoutineData::generateCode(const char* identifier, const var_env_t& var_env, std::ostringstream& codeStream)  const 
{
	codeStream << "void" << " " << identifier << "(";
	int i = 0;

	// Add first no global variable as function arguments
	std::shared_ptr<ShaderVariableData> v = var_env.at(vars[i]);
	while (i < vars.size() && v->isGlobalVariable()) {
		i++;
		v = var_env.at(vars[i]);
	}

	if (i < vars.size())
		codeStream << v->getType() << " " << vars[i];

	// Add others separated by comma
	while (i < vars.size()) {
		if (!v->isGlobalVariable()) {
			codeStream << ", " << v->getType() << " " << vars[i];
		}
		i++;
		v = var_env.at(vars[i]);
	}
	codeStream << ") {" << std::endl << body << std::endl << "};";
}

// ShaderMain
ShaderMain::ShaderMain() 
	: body("") 
{
}

ShaderMain::ShaderMain(const char* code)
	: body(code)
{
}

void ShaderMain::generateCode(std::ostringstream& codeStream) const
{
	codeStream << "int main() {" << std::endl << body << std::endl << "};";
}

// ShaderWriter
ShaderWriter::ShaderWriter() 
{
}

void ShaderWriter::addVariable(const char* identifier, std::shared_ptr<ShaderVariableData> data)
{
	var_env[identifier] = data;
	upToDate = false;
}


void ShaderWriter::addVariable(GLSLType type, const char* identifier, ShaderVariableInitialization& init) 
{
	if (var_env.contains(identifier)) {
		throw std::runtime_error("The variable: " + std::string(identifier) + " has already been declared.");
	}

	addVariable(identifier, std::make_shared<ShaderMainVariable>(ShaderMainVariable(type, init)));
}

void ShaderWriter::addVariable(GLSLVarMode mode, GLSLType type, const char* identifier, const int location) 
{
	if (var_env.contains(identifier)) {
		throw std::runtime_error("The variable: " + std::string(identifier) + " has already been declared.");
	}

	addVariable(identifier, std::make_shared<ShaderGlobalVariable>(ShaderGlobalVariable(mode, type, location)));
}

void ShaderWriter::addRootine(const char* identifier, std::initializer_list<const char*> args, const char* body)
{
	if (var_env.contains(identifier)) {
		throw std::runtime_error("The function: " + std::string(identifier) + " has already been declared.");
	}

	func_env[identifier] = std::make_shared<ShaderRoutineData>(ShaderRoutineData(args, body));
	upToDate = false;
}

void ShaderWriter::setMain(ShaderMain& mainCode) 
{
	main = mainCode;
	upToDate = false;
}

void ShaderWriter::updateShader() 
{
	shaderCode.clear();

	// Shader header
	shaderCode << "#version 330 core" << std::endl;

	// Add shader variables
	for (const auto& [identifier, data] : var_env) {
		data->generateCode(identifier, shaderCode);
		shaderCode << std::endl;
	}
	shaderCode << std::endl;

	// Add shader functions
	for (auto& [identifier, data] : func_env) {
		data->generateCode(identifier, var_env, shaderCode);
		shaderCode << std::endl << std::endl;
	}

	// Add shader main function
	main.generateCode(shaderCode);

	upToDate = true;
}

void ShaderWriter::saveShader(const char* filename) 
{
	// Save the vertex shader
	std::ofstream ofs(filename);
	ofs << getShaderSource();
	ofs.close();
}

std::string ShaderWriter::getShaderSource()
{
	if (!upToDate)
		updateShader();

	return shaderCode.str();
}

