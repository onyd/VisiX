#include "pch.h"
#include "CppUnitTest.h"

#include <glad/glad.h>

#include <iostream>
#include "Node.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(SceneCompilation)
	{
	public:
		TEST_METHOD(EmptyCompilation) {
			Node* root = new Node();

			root->compile();
			Assert::IsTrue(root->compiled);
			delete root;
		}

		TEST_METHOD(OnlyRootCompilation) {
			Node* root = new Node();

			Node* n1 = new Node();
			Node* n2 = new Node();
			Node* n3 = new Node();

			root->addNode(n1);
			root->addNode(n2);
			root->addNode(n3);

			root->compile();
			Assert::IsTrue(root->compiled);
			Assert::IsTrue(!n1->compiled);
			Assert::IsTrue(!n2->compiled);
			Assert::IsTrue(!n3->compiled);
			delete root;
		}
	};

}
