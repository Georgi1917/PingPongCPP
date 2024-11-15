#include <iostream>
#include "Window.h"
using namespace std;

int main() 
{

	cout << "Creating Window..." << endl;

	Window* pWindow = new Window();

	bool running{ true };

	while (running)
	{
		if (!pWindow->ProccessMessages())
		{
			cout << "Closed Window." << endl;

			running = false;
		}

	}

	delete pWindow;
	pWindow = nullptr;

	return 0;

}
