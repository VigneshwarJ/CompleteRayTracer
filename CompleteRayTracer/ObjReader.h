#pragma once
#include <vector>
#include "Shapes.h"
#include "Scene.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void readObjFiles(std::string fileName, Material* mtl,Scene& scene)
{
	std::vector<glm::vec3> vertices;
	std::vector<std::unique_ptr<Triangle>> obj;

	std::ifstream inFile(fileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
					  std::istream_iterator<std::string>{} };
		if (!tokens.size() < 1)
		{
			if (tokens[0] == "v")
			{
				vertices.push_back(glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
			}
			else if (tokens[0] == "vn" || tokens[0] == "vt")
			{
				//vertices.push_back(glm::vec3(0, 0, 0));
			}
			else if (tokens[0] == "f")
			{
				glm::vec3 c = glm::vec3(0.7f, 0.7f, 0.7f);
				glm::vec3 cSpec = glm::vec3(1, 1, 1);
				Material* mt = mtl;

				std::string delimiter = "/";
				int p0 = stoi(tokens[1].substr(0, tokens[1].find(delimiter)));
				int p1 = stoi(tokens[2].substr(0, tokens[2].find(delimiter)));
				int p2 = stoi(tokens[3].substr(0, tokens[3].find(delimiter)));

				p0 = (p0 < 0) ? -1 * p0 : p0;
				p1 = (p1 < 0) ? -1 * p1 : p1;
				p2 = (p2 < 0) ? -1 * p2 : p2;

				scene.Shapes.push_back(std::make_unique<Triangle>(vertices[p0-1], vertices[p1 - 1], vertices[p2 - 1], mt));
			}
		}
	}
	std::cout << "Loading obj done vertices number : " << vertices.size()<< std::endl;
	return;
}
