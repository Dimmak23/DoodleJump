//* SDL2: Connector
#include "SDL2_Connector/Launcher.hpp"
// #include "SDL2_Connector/Loger.hpp"
// #include "SDL2_Connector/IApplication.hpp"

//* Test application
#include "TestedGameApp.hpp"

int main(int argc, char** argv)
{
	std::unique_ptr<TestedGameApp> Application = std::make_unique<TestedGameApp>(argc, argv);

	return Launcher::Run(Application.get());
	// Launcher::Run(Application.get());

	// LogLine("Safe exit...");
	// return 0;
}
