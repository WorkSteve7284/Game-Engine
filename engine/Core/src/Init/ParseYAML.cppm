export module PotatoEngine.Core.Init.ParseYAML;

import std;
using namespace std;

import PotatoEngine.Core.Init.File;
import PotatoEngine.Core.Init.YAML;

namespace PotatoEngine::Core::Init {
	export File ignore_comments(File& file) {
		vector<string> new_file;
		for (size_t i = 0; i < file.get_length(); ++i) {
			const string line = file.get_line(i);

			string final_string = "";

			bool in_string = false;
			bool escaped = false;
			for (char c : line) {
				switch (c) {
				case '\'':
					in_string = !in_string && !escaped;
					escaped = false;
					final_string += c;
					break;
				case '\"':
					in_string = !in_string && !escaped;
					escaped = false;
					final_string += c;
					break;
				case '\\':
					escaped = true;
					final_string += c;
					break;
				case '#':
					if (!in_string)
						goto end_line;
					final_string += c;
					break;
				default:
					escaped = false;
					final_string += c;
				}
			}
		end_line:
			new_file.push_back(final_string);
		}

		return new_file;
	}

	export File ignore_empty(File& file) {
		vector<string> new_file;

		hash<string> hasher;
		size_t empty = hasher("");

		for (size_t i = 0; i < file.get_length(); ++i) {
			if (file.get_line(i).find_first_of("abcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[{]}\\|;:\'\",<.>/?") == string::npos)
				continue;
			new_file.push_back(file.get_line(i));
		}

		return new_file;
	}

	export File ignore_header(File& file) {

		hash<string> hasher;

		size_t header = hasher("---");

		for (size_t i = 0; i < file.get_length(); ++i) {
			if (hasher(file.get_line(i)) == header) {
				return file.get_lines(i + 1, file.get_length());
			}
		}

		return file;
	}

	bool contains_only(string_view str, string_view filter) {
		return std::all_of(str.begin(), str.end(), [&](char c) { return filter.find(c) != std::string::npos; });
	}

	// Parse yaml object
	export YAMLObject parse_yaml_object(File yaml, size_t indents) {

		/*
		key: value
		*/

		/*
		map:
		  key: value
		  ...
		*/

		/*
		sequence:
		 - key: value
		*/
		static constexpr hash<string> hasher;
		static const size_t sequence = hasher(" - ");
		static const size_t space3 = hasher("   ");

		for (string& line : yaml.lines) {
			line.erase(0, indents);
		}

		YAMLObject object;

		size_t colon = yaml.get_line(0).find_first_of(':');

		object.key = yaml.get_line(0).substr(0, colon);

		// if this object is a key-value pair
		if (yaml.get_line(0).find_first_not_of(' ', colon + 1) != string::npos) {

			object.type = YAMLEntryType::KeyValue;
			string value = yaml.get_line(0).substr(colon + 1);
			value = value.substr(value.find_first_not_of(" "), value.find_last_not_of(" ") - value.find_first_not_of(" ") + 1);

			// determine type:

			static const array<size_t, 4> bools = { hasher("true"), hasher("false"), hasher("yes"), hasher("no") };

			const size_t hashed_value = hasher(value);

			if (contains_only(value, "-1234567890")) {
				object.value = stoi(value);
			}
			else if (contains_only(value, "-e1234567890.")) {
				object.value = stod(value);
			}
			else if (find(bools.begin(), bools.end(), hashed_value) != bools.end()) {
				if (hashed_value == bools[1] || hashed_value == bools[3]) {
					object.value = false;
				}
				else if (hashed_value == bools[0] || hashed_value == bools[2]) {
					object.value = true;
				}
			}
			else {
				object.value = std::string(value);
			}
			return object;
		}

		// if this object is a sequence
		else if (hasher(yaml.get_line(1).substr(0, 3)) == sequence) {
			object.type = YAMLEntryType::Sequence;
			vector<YAMLObject> value;
			// parse sequence objects
			size_t start_line = 1;
			for (size_t i = 2; i < yaml.get_length(); ++i) {
				const size_t hashed3 = hasher(yaml.get_line(i).substr(0, 3));
				if (hashed3 == sequence || hashed3 != space3) {
					value.push_back(parse_yaml_object(yaml.get_lines(start_line, i), 3));
					start_line = i;
				}
			}

			value.push_back(parse_yaml_object(yaml.get_lines(start_line, yaml.get_length()), 3));

			object.value = value;
			return object;
		}

		// if this object is a map
		else {
			object.type = YAMLEntryType::Map;
			vector<YAMLObject> value;

			// check if each line is indented more than the first
			const size_t base_indent_count = yaml.get_line(1).find_first_not_of(" ");

			string str;
			for (size_t i = 0; i <= base_indent_count; ++i) {
				str += " ";
			}
			const size_t hashed_indent = hasher(str);

			size_t start_line = 1;
			for (size_t i = 2; i < yaml.get_length(); ++i) {
				const size_t hashedn = hasher(yaml.get_line(i).substr(0, base_indent_count + 1));
				if (hashedn != hashed_indent) {
					value.push_back(parse_yaml_object(yaml.get_lines(start_line, i), base_indent_count));
					start_line = i;
				}
			}

			value.push_back(parse_yaml_object(yaml.get_lines(start_line, yaml.get_length()), base_indent_count));
			object.value = value;
			return object;
		}


	}

	// parse File
	export YAML parse_yaml_file(filesystem::path target) {
		File file;

		file.read_file(target);

		file = ignore_header(file);
		file = ignore_comments(file);
		file = ignore_empty(file);

		/*
		* go through lines
		* find line not starting with space
		* find next line not starting with space
		* parse that
		* repeat
		*/

		YAML yaml;

		hash<string> hasher;

		size_t space = hasher(" ");

		int64_t start_line = -1;

		for (size_t i = 0; i < file.get_length(); ++i) {
			string line = file.get_line(i);

			if (line[0] == ' ') // if line starts with space, go to next line
				continue;

			if (start_line < 0) {
				start_line = i;
			}
			else {
				YAMLObject new_obj = parse_yaml_object(file.get_lines(start_line, i), 0);
				yaml.objects.push_back(new_obj);
				start_line = i;
			}
		}

		if (start_line >= 0) {
			YAMLObject new_obj = parse_yaml_object(file.get_lines(start_line, file.get_length()), 0);
			yaml.objects.push_back(new_obj);
		}

		return yaml;
	}

	export YAML parse_yaml_string(File file) {

		file = ignore_header(file);
		file = ignore_comments(file);
		file = ignore_empty(file);

		YAML yaml;

		hash<string> hasher;

		size_t space = hasher(" ");

		int64_t start_line = -1;

		for (size_t i = 0; i < file.get_length(); ++i) {
			string line = file.get_line(i);

			if (line[0] == ' ') // if line starts with space, go to next line
				continue;

			if (start_line < 0) {
				start_line = i;
			}
			else {
				YAMLObject new_obj = parse_yaml_object(file.get_lines(start_line, i), 0);
				yaml.objects.push_back(new_obj);
				start_line = i;
			}
		}

		if (start_line >= 0) {
			YAMLObject new_obj = parse_yaml_object(file.get_lines(start_line, file.get_length()), 0);
			yaml.objects.push_back(new_obj);
		}

		return yaml;
	}
}