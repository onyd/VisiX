#include "pch.h"
#include "CppUnitTest.h"

#include "ShaderWriter.h"
#include "Property.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(ShaderWriterTest)
	{
	public:
		
		TEST_METHOD(MinimalShaderTest)
		{
			const char* expected =
				"#version 330 core\n"
				"\n"
				"int main() {\n"
				"\n"
				"};";

			ShaderWriter writer;
			writer.updateShader();
			std::string code = writer.getShaderProgram();
			Assert::AreEqual(expected, code.c_str());
		}

		TEST_METHOD(VariableShaderTest)
		{
			const char* expected =
				"#version 330 core\n"
				"out vec2 v3;\n"
				"layout (location = 0) in vec3 v1;\n"
				"uniform mat4 v4;\n"
				"in float v2;\n"
				"\n"
				"int main() {\n"
				"\n"
				"};";

			ShaderWriter writer;

			writer.addVariable(GLSL_LAYOUT_IN, GLSL_VEC3, "v1", 0);
			writer.addVariable(GLSL_IN, GLSL_FLOAT, "v2");
			writer.addVariable(GLSL_OUT, GLSL_VEC2, "v3");
			writer.addVariable(GLSL_UNIFORM, GLSL_MAT4, "v4");

			writer.updateShader();
			std::string code = writer.getShaderProgram();
			Assert::AreEqual(expected, code.c_str());
		}

		TEST_METHOD(FunctionShaderTest)
		{
			const char* expected =
				"#version 330 core\n"
				"\n"
				"float f3() {\n"
				"\n"
				"};\n"
				"\n"
				"float f1() {\n"
				"\n"
				"};\n"
				"\n"
				"float f2() {\n"
				"\n"
				"};\n"
				"\n"
				"int main() {\n"
				"\n"
				"};";

			ShaderWriter writer;

			writer.addFunction(GLSL_FLOAT, "f1", {}, "");
			writer.addFunction(GLSL_FLOAT, "f2", {}, "");
			writer.addFunction(GLSL_FLOAT, "f3", {}, "");

			writer.updateShader();
			std::string code = writer.getShaderProgram();
			Assert::AreEqual(expected, code.c_str());
		}

		TEST_METHOD(FullShaderTest)
		{
			const char* expected =
				"#version 330 core\n"
				"layout (location = 0) in vec3 pos;\n"
				"uniform mat4 mat;\n"
				"\n"
				"float transform() {\n"
				"	gl_Position = mat * vec4(pos, 1.0);\n"
				"};\n"
				"\n"
				"int main() {\n"
				"	transform();\n"
				"};";

			ShaderWriter writer;

			// Add some variables
			writer.addVariable(GLSL_LAYOUT_IN, GLSL_VEC3, "pos", 0);
			writer.addVariable(GLSL_UNIFORM, GLSL_MAT4, "mat");

			// Add some functions
			const char* body =
				"	gl_Position = mat * vec4(pos, 1.0);";

			writer.addFunction(GLSL_FLOAT, "transform", {}, body);

			// Set the main code
			const char* mainCode = "	transform();";
			ShaderMain shaderMain(mainCode);

			writer.setMain(shaderMain);

			// Generate the shader code
			writer.updateShader();
			std::string code = writer.getShaderProgram();
			Assert::AreEqual(expected, code.c_str());
		}
	};

	TEST_CLASS(PropertyTest), public PropertyHandler {
	public:
		TEST_METHOD_CLEANUP(PropertyReset) {
			v.value = 2;
		}
		TEST_METHOD(PropertyDeclaration) {
			Assert::AreEqual(2, v.value);
		}
		
		TEST_METHOD(PropertyModification) {
			v.value = 5;
			Assert::AreEqual(5, v.value);
		}


		TEST_METHOD(PropertyBinding) {
			int check;
			callback_t<int> callback = [&check](const int& x) { check = x; };
			v.bind(callback);
			v.value = 5;
			Assert::AreEqual(5, check);
		}

		TEST_METHOD(PropertyHandlerBinding) {
			int check;

			// Callback that propagate change to a variable
			callback_t<int> callback = [&check](const int& x) { check = x; };
			
			// Bind the callback to v
			bind("v", callback);
			
			// Change the property value
			v.value += 3;
			
			// Verify that check has also taken the property value
			Assert::AreEqual(5, check);
		}

		TEST_METHOD(PropertyHandlerMultiBinding) {
			int check = 0; // Use as checking value

			// Create callbacks
			callback_t<int> callback1 = [&check](const int& x) { check++; };
			callback_t<int> callback2 = [&check](const int& x) { check++; };

			// Bind them to v
			bind("v", callback1);
			bind("v", callback2);

			// CHange the value
			v.value = 2;

			// Verify that check has correctly been incremented by the two callbacks
			Assert::AreEqual(2, check);
		}

		TEST_METHOD(PropertyHandlerUnbinding) {
			int check = 0; // Use as checking value

			// Create callbacks
			callback_t<int> callback1 = [&check](const int& x) { check++; };
			callback_t<int> callback2 = [&check](const int& x) { check++; };

			// Bind them to v
			bind("v", callback1);
			bind("v", callback2);

			// But unbind them 
			unbindAll("v");

			// CHange the value
			v.value = 2;

			// Verify that check has not changed as callbacks has been all unbinded
			Assert::AreEqual(0, check);
		}

		CLASS_PROPERTY(int, v, 2)
	};
}
