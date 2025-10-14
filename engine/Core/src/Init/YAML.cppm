export module PotatoEngine.Core.Init.YAML;

import std;
using namespace std;

namespace PotatoEngine::Core::Init {
	export enum class YAMLEntryType {
		KeyValue, // has a key and a value; in the file "key: value"
		Sequence, // ordered-ish list
		Map // unordered-ish list
	};

	export struct YAMLObject {
		string key;

		YAMLEntryType type;
		
		any value;
	};

	export struct YAML {
		vector<YAMLObject> objects;
	};

	export void read_yaml(YAMLObject, int);

	export void read_yaml(YAML yaml) {
		for (YAMLObject obj : yaml.objects) {
			read_yaml(obj, 1);
		}
	}

	export void read_yaml(YAMLObject yaml, int indent) {

		static const array<size_t, 4> types = {
			hash<string>()("double"),
			hash<string>()("int"),
			hash<string>()("bool"),
			hash<string>()("class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >")
		};

		for (int i = 0; i < indent; i++) {
			cout << " ";
		}

		if (yaml.type == YAMLEntryType::KeyValue) {

			const size_t value_type = hash<string>()(yaml.value.type().name());

			if (value_type == types[0]) {
				cout << yaml.key << ": " << any_cast<double>(yaml.value) << endl;
			}
			else if (value_type == types[1]) {
				cout << yaml.key << ": " << any_cast<int>(yaml.value) << endl;
			}
			else if (value_type == types[2]) {
				cout << yaml.key << ": " << any_cast<bool>(yaml.value) << endl;
			}
			else if (value_type == types[3]) {
					cout << yaml.key << ": " << any_cast<string>(yaml.value) << endl;
			}
		}
		else {
			cout << yaml.key << ":" << endl;
			for (YAMLObject entry : any_cast<vector<YAMLObject>>(yaml.value)) {
				read_yaml(entry, indent + 2);
			}
		}
	}
}