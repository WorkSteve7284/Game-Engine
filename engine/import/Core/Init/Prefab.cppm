export module PotatoEngine.Core.Init.Prefab;

import std;
using namespace std;

namespace PotatoEngine::Core::Init {

    export struct ComponentPrefabEntry {

        ComponentPrefabEntry(pair<string, any> contents) : key(contents.first), value(contents.second) {}

        string key;
        any value;
    };
    
    export struct ComponentPrefab {
        string component;
        vector<ComponentPrefabEntry> entries;
    
        operator void() const {}
    };

    export struct ObjectPrefab {
        string name;
        vector<ComponentPrefab> components;
        vector<ObjectPrefab> objects;
    };
}