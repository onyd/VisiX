#include "ShaderClass.h"

std::string getFileContents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw errno;
}

Shader::Shader(const char* vertex_source, const char* fragment_source) {
	// Compile vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
	glCompileShader(vertex_shader);
	compileErrors(vertex_shader, "VERTEX");

	// Compile fragment shader
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
	glCompileShader(fragment_shader);
	compileErrors(fragment_shader, "FRAGMENT");

	// Create usable shader program from vertex and fragment shaders
	m_id = glCreateProgram();
	glAttachShader(m_id, vertex_shader);
	glAttachShader(m_id, fragment_shader);
	glLinkProgram(m_id);

	// Vertex and fragment shader are no longer needed as there are in the shader program
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
	destroy();
}

Shader Shader::from_file(const char* vertex_file, const char* fragment_file)
{
	std::string vertex_code = getFileContents(vertex_file);
	std::string fragment_code = getFileContents(fragment_file);

	const char* vertex_source = vertex_code.c_str();
	const char* fragment_source = fragment_code.c_str();

	return Shader(vertex_source, fragment_source);
}

void Shader::activate() const {
	glUseProgram(m_id);
}

void Shader::destroy() {
	glDeleteProgram(m_id);
}

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint has_compiled;
	// Character array to store error message in
	char info_log[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &has_compiled);
		if (has_compiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << info_log << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &has_compiled);
		if (has_compiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << info_log << std::endl;
		}
	}
}

ComputeShader::ComputeShader(const char* source, const std::vector<Buffer>& buffers) : m_buffers(buffers)
{
	// Compile compute shader
	GLuint compute_shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute_shader, 1, &source, nullptr);
	glCompileShader(compute_shader);

	// Error check
	GLint has_compiled;
	glGetProgramiv(compute_shader, GL_COMPILE_STATUS, &has_compiled);
	if (has_compiled == GL_FALSE)
	{
		GLint max_length = 0;
		glGetShaderiv(compute_shader, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> error_log(max_length);
		glGetShaderInfoLog(compute_shader, max_length, nullptr, &error_log[0]);
		if (error_log.size() > 0) {
			std::cout << "SHADER_LINKING_ERROR for COMPUTE SHADER\n" << error_log.data() << std::endl;

			glDeleteShader(compute_shader);
			return;
		}
	}

	// Create usable shader program from compute shader
	m_id = glCreateProgram();
	glAttachShader(m_id, compute_shader);
	glLinkProgram(m_id);

	// Compute shader is no longer needed as it is in the shader program
	glDeleteShader(compute_shader);
}

ComputeShader::~ComputeShader()
{
	destroy();
}

ComputeShader ComputeShader::from_file(const char* source_file, const std::vector<Buffer>& buffers)
{
	std::string compute_code = getFileContents(source_file);

	const char* compute_source = compute_code.c_str();

	return ComputeShader(compute_source, buffers);
}

void ComputeShader::activate() const
{
	glUseProgram(m_id);
	for (int i = 0; i < m_buffers.size(); i++)
		m_buffers[i].bindBase(i);
}

void ComputeShader::dispatch(GLuint group_size_x, GLuint group_size_y, GLuint group_size_z) const
{
	glDispatchCompute(group_size_x, group_size_y, group_size_z);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	for (int i = 0; i < m_buffers.size(); i++)
		m_buffers[i].unbindBase(i);
	glUseProgram(0);
}

void ComputeShader::destroy()
{
	glDeleteProgram(m_id);
}
