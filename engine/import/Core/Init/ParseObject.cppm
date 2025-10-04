export module PotatoEngine.Core.Init.ParseObject;

import std;
using namespace std;

import PotatoEngine.Core.Init.Prefab;
import PotatoEngine.Core.Init.YAML;

namespace PotatoEngine::Core::Init {
    
    const size_t name = hash<string>()("name");
    const size_t components = hash<string>()("components");
    const size_t Components = hash<string>()("Components");
    const size_t objects = hash<string>()("objects");
    const size_t Objects = hash<string>()("Objects");

    export unordered_map<string, ObjectPrefab> prefabs;

    export ComponentPrefab parse_component(YAMLObject yaml) {

        static const array<size_t, 4> types = {
			hash<string>()("double"),
			hash<string>()("int"),
			hash<string>()("bool"),
			hash<string>()("class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >")
		};

        ComponentPrefab prefab;
        
        prefab.component = yaml.key;

        for (YAMLObject entry : any_cast<vector<YAMLObject>>(yaml.value)) {
            

            const size_t value_type = hash<string>()(entry.value.type().name());

			if (value_type == types[0]) {
                prefab.entries.push_back(ComponentPrefabEntry({entry.key, any_cast<double>(entry.value)}));
			}
			else if (value_type == types[1]) {
			    prefab.entries.push_back(ComponentPrefabEntry({entry.key, any_cast<int>(entry.value)}));
            }
			else if (value_type == types[2]) {
                prefab.entries.push_back(ComponentPrefabEntry({entry.key, any_cast<bool>(entry.value)}));
			}
			else if (value_type == types[3]) {
                prefab.entries.push_back(ComponentPrefabEntry({entry.key, any_cast<string>(entry.value)}));
			}
        }

        return prefab;
    }

    export vector<ComponentPrefab> parse_components(YAMLObject yaml) {

        vector<ComponentPrefab> prefabs;

        for (YAMLObject entry : any_cast<vector<YAMLObject>>(yaml.value)) {
            prefabs.push_back(parse_component(entry));
        }

        return prefabs;
    }

    export ObjectPrefab parse_object(YAMLObject);

    export vector<ObjectPrefab> parse_objects(YAMLObject yaml) {

        vector<ObjectPrefab> prefabs;

        for (YAMLObject entry : any_cast<vector<YAMLObject>>(yaml.value)) {
            prefabs.push_back(parse_object(entry));
        }

        return prefabs;

    }

    ObjectPrefab parse_object(YAMLObject yaml) {
        
        ObjectPrefab object;

        if (prefabs.count(yaml.key) > 0) {
            return prefabs[yaml.key];
        }

        for (YAMLObject entry : any_cast<vector<YAMLObject>>(yaml.value)) { // Loop through child YAMLObjects
            const size_t key = hash<string>()(entry.key);
            if (entry.type == YAMLEntryType::KeyValue && key == name) { // If this YAMLObject is a pair and the key is name, set name
                object.name = any_cast<string>(entry.value);

            } else {
                if (key == components || key == Components) {
                    object.components = parse_components(entry);
                } else if (key == Objects || key == objects) {
                    object.objects = parse_objects(entry);
                }
            }
        }

        prefabs[yaml.key] = object;

        return object;
    }

    export void parse_objects_yaml(YAML yaml) {
        for (YAMLObject entry : yaml.objects) {
            const size_t key = hash<string>()(entry.key);

            if (key == objects || key == Objects) {
                parse_objects(entry);
            }
        }
    }

}