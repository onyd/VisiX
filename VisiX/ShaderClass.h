#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glad/glad.h>

#include "Buffer.h"

std::string getFileContents(const char* filename);

class Shader
{
	public:
		GLuint m_id;

		/**
		 * @fn	Shader::Shader(const char* vertex_source, const char* fragment_source);
		 *
		 * @brief	Constructor
		 *
		 * @param 	vertex_source  	The vertex shader source code.
		 * @param 	fragment_source	The fragment shader source code.
		 */
		Shader(const char* vertex_source, const char* fragment_source);

		~Shader();

		/**
		 * @fn	static Shader Shader::from_file(const char* vertex_file, const char* fragment_file);
		 *
		 * @brief	Create shader program from file
		 *
		 * @param 	vertex_file  	The vertex shader file.
		 * @param 	fragment_file  	The fragment shader file.
		 *
		 * @returns	A Shader.
		 */
		static Shader from_file(const char* vertex_file, const char* fragment_file);


		/**
		 * @fn	void Shader::activate();
		 *
		 * @brief	Activates the shader program
		 */
		void activate() const;

		/**
		 * @fn	void Shader::destroy();
		 *
		 * @brief	Destroys the shader program
		 */
		void destroy();

	private:

		/**
		 * @fn	void Shader::compileErrors(unsigned int shader, const char* type);
		 *
		 * @brief	Compile errors
		 *
		 * @param 	shader	The shader.
		 * @param 	type  	The type.
		 */
		void compileErrors(unsigned int shader, const char* type);
};

class ComputeShader
{
public:
	GLuint m_id;

	/**
	* @fn	ComputeShader::ComputeShader(const char* source, const std::vector<Buffer>& buffers);
	*
	* @brief	Constructor
	*
	* @param 	source  	The compute shader source_code.
	*/
	ComputeShader(const char* source, const std::vector<Buffer>& buffers);

	~ComputeShader();

	/**
	 * @fn	static ComputeShader ComputeShader::from_file(const char* source_file, const std::vector<Buffer>& buffers);
	 *
	 * @brief	Create shader program from file
	 *
	 * @param 	source_file  	The vertex shader file.
	 *
	 * @returns	A Shader.
	 */
	static ComputeShader from_file(const char* source_file, const std::vector<Buffer>& buffers);


	/**
	 * @fn	void ComputeShader::activate();
	 *
	 * @brief	Activates the shader program
	 */
	void activate() const;

	void dispatch(GLuint group_size_x, GLuint group_size_y, GLuint group_size_z) const;

	/**
	 * @fn	void ComputeShader::destroy();
	 *
	 * @brief	Destroys the shader program
	 */
	void destroy();

private:
	std::vector<Buffer> m_buffers;
};