module;

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <ctime>

export module PotatoEngine.Core.Debug;

namespace PotatoEngine::Core::Debug {
	
	export class Debug_Manager {
	public:
		Debug_Manager(std::filesystem::path);
		Debug_Manager() = default;
		~Debug_Manager();
	private:
		std::ofstream log_file;
		std::filesystem::path log_path;
	public:
		std::string Log(std::string);
		std::filesystem::path Get_Log_Path();
		std::filesystem::path Switch_Log_Path(std::filesystem::path);
	};

	Debug_Manager::Debug_Manager(std::filesystem::path new_path) : log_path(new_path) { log_file.open(log_path); }
	Debug_Manager::~Debug_Manager() { log_file.close(); }

	std::string Debug_Manager::Log(std::string message) {
		time_t timestamp = time(&timestamp);
		struct tm datetime = *localtime(&timestamp);

		std::string time_message = "[ " + std::to_string(datetime.tm_mday) + "-" + std::to_string(datetime.tm_mon) + "-" + std::to_string(datetime.tm_year + 1900) + ", " + std::to_string(datetime.tm_hour) + ":" + std::to_string(datetime.tm_min) + ":" + std::to_string(datetime.tm_sec) + " ] ";

		std::cout << time_message << message << "\n";

		log_file << time_message << message << "\n";

		return time_message + message;
	}

	std::filesystem::path Debug_Manager::Get_Log_Path() {
		return log_path;
	}

	std::filesystem::path Debug_Manager::Switch_Log_Path(std::filesystem::path new_path) {
		log_path = new_path;
		log_file.close();
		log_file.open(log_path);
		return log_path;
	}

	export Debug_Manager Debug("debug.log");
}