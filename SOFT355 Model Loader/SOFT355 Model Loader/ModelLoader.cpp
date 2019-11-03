#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>


Model loadFromObj(std::string file) {
	Model model;

	std::cout << "Creating from: " + file;

	std::ifstream rfile;
	rfile.open(file);
	std::string line;


	if (rfile.is_open()) {
		while (std::getline(rfile, line)) {
			std::cout << line << std::endl;
		}
	}

	return model;
}


Model loadFromFile(std::string file) {
	
	std::string fileType = file.substr(file.length() - 4, file.length());

	Model newModel;

	if (fileType._Equal(".obj")) {
		newModel = loadFromObj(file);
	}
	else {
		std::cout << "Sorry this file type is not supported";
	}

	return newModel;

}


