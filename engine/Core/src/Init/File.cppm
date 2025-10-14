export module PotatoEngine.Core.Init.File;

import std;
using namespace std;

namespace PotatoEngine::Core::Init {

	export struct File {
		vector<string> lines;

		File() { lines = {}; }

		File(string file) {
			size_t pos = 0;

			while (pos < file.length()) {
				size_t next_pos = file.find('\n', pos);

				if (next_pos == string::npos) {
					next_pos = file.length();
				}

				lines.push_back(file.substr(pos, pos - (next_pos + 1)));
				pos = next_pos + 1;
			}
		}

		File(vector<string> file) {
			lines = file;
		}

		void read_file(filesystem::path target) {
			lines.clear();

			ifstream file(target);

			if (!file.is_open()) {
				file.close();
				cerr << "Error: File " << target.string() << " failed to open" << std::endl;
			}

			string line;
			while (getline(file, line)) {
				lines.push_back(line);
			}

			file.close();
		}

		inline string get_line(size_t line) const {
			return lines.at(line);
		}

		inline vector<string> get_lines(size_t start, size_t end) const {
			return vector<string>(lines.begin() + start, lines.begin() + end);
		}

		inline size_t get_length() const {
			return lines.size();
		}

		inline void log() const {
			for (int i = 0; i < get_length(); ++i) {
				cout << i << ": " << get_line(i) << endl;
			}
		}

	};
}