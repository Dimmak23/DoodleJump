//* C++ std
#include <iostream>
#include <memory>

//* Vendor connector
#include "Launcher.hpp"

//* Custom libraries
#include "GameApplication.hpp"

int main(int argc, char** argv)
{
	//^ Create an application
	std::unique_ptr<GameApplication> application = std::make_unique<GameApplication>(argc, argv);

	//^ Check if everything could be initialized properly
	if (application->GetIsConstructWithError())
	{
		std::cout << "Press any key to close...\n";
		std::cin.get();
		return -1;
	}

	//^ Run it
	// int run_result = Launcher::Run(application.get());

	//^ Release applicaiton resources whould be don on destruction of 'application' variable
	// return run_result;
	return Launcher::Run(application.get());
}
