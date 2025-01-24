//* SDL2: Connector
#include "SDL2_connector/Launcher.hpp"
#include "SDL2_connector/Loger.hpp"

//* Game
#include "application/GameApplication.hpp"

//* C++ std
#include <memory>

int main(int argc, char** argv)
{
	//^ Create an application
	std::unique_ptr<GameApplication> application = std::make_unique<GameApplication>(argc, argv);

	//^ Check if everything could be initialized properly
	if (application->GetIsConstructWithError())
	{
		return -1;
	}

	int run_result{};
	{
		//^ Run it
		run_result = Launcher::Run(application.get());
		//^ Release applicaiton resources whould be don on destruction of 'application' variable
	}

	LogLine("Application exit with code: ", run_result);
	return run_result;

	// return Launcher::Run(application.get());
}
