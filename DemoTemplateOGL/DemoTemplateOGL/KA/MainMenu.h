#ifndef MAINMENU_H
#define MAINMENU_H

#ifdef _WIN32 
#include <windows.h>
#include <windowsx.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Menu.h"

class MainMenu {
private:
	std::vector<Billboard2D*> billBoard2D;
	Billboard2D* camara;
public:
	MainMenu(Camera* cam);
	MainMenu(Model* camIni);
	void InitGraph(Camera* main);

	int update(int& menuOption);

	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
	MainMenu* Render();

	std::vector<Billboard2D*>* getLoadedBillboards2D();
	Model* getMainModel();
	void setMainModel(Billboard2D* mainModel);
	~MainMenu();
};

#endif