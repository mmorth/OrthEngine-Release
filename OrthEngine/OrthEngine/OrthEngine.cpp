#include <cmath>
#include <iostream>
#include <string>

#include "OrthEngine.hpp"

int main()
{
	// Configure g3log
	auto worker = g3::LogWorker::createLogWorker();
	auto loggerHandle = worker->addDefaultLogger("global_logger", "./logs"); // Provide a unique logger ID and the log directory

	// Initialize g3log
	g3::initializeLogging(worker.get());

	// Create and start EngineApp
	EngineApp app;
	app.run();

	LOG(INFO) << "Application Start";

	return 0;
}
