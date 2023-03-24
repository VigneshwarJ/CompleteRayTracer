#include "MainApp.h"


// Main code
int main(int, char**) {

	MainApp* app = new  MainApp();
	app->Render();
	delete app;
	return 0;
}