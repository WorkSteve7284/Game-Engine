module;

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <ctime>
#include <format>

export module PotatoEngine.Core.Debug;

namespace PotatoEngine::Core::Debug {
	
	export class DebugManager {
	public:
		DebugManager(std::filesystem::path);
		DebugManager() = default;
		~DebugManager();
	private:
		std::ofstream log_file;
		std::filesystem::path log_path;
	public:
		std::string log(std::string);
		std::string error(std::string);
		std::filesystem::path get_log_path();
		std::filesystem::path switch_log_path(std::filesystem::path);
	};

	DebugManager::DebugManager(std::filesystem::path new_path) : log_path(new_path) { log_file.open(log_path); }
	DebugManager::~DebugManager() { log_file.close(); }

	std::string DebugManager::log(std::string message) {
		time_t timestamp = time(&timestamp);
		struct tm datetime = *localtime(&timestamp);

		std::string time_message = std::format("[ {}-{}-{}, {}:{}:{} ] ", datetime.tm_mday, datetime.tm_mon, datetime.tm_year + 1900, datetime.tm_hour, datetime.tm_min, datetime.tm_sec);

		std::cout << time_message << message << "\n";

		log_file << time_message << message << "\n";

		return time_message + message;
	}

	std::string DebugManager::error(std::string message) {
		time_t timestamp = time(&timestamp);
		struct tm datetime = *localtime(&timestamp);

		std::string time_message = std::format("[ {}-{}-{}, {}:{}:{} ] ERROR: ", datetime.tm_mday, datetime.tm_mon, datetime.tm_year + 1900, datetime.tm_hour, datetime.tm_min, datetime.tm_sec);

		std::cout << time_message << message << "\n";

		log_file << time_message << message << "\n";

		return time_message + message;
	}

	std::filesystem::path DebugManager::get_log_path() {
		return log_path;
	}

	std::filesystem::path DebugManager::switch_log_path(std::filesystem::path new_path) {
		log_path = new_path;
		log_file.close();
		log_file.open(log_path);
		return log_path;
	}

	export DebugManager Debug("debug.log");
}