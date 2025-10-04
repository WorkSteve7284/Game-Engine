export module PotatoEngine.Core.Init.CreateObject;

import std;
using namespace std;

import PotatoEngine.Core.Classes;
using namespace PotatoEngine::Core::Classes;
import PotatoEngine.Core.Init.File;
import PotatoEngine.Core.Init.ParseYAML;
import PotatoEngine.Core.Init.YAML;
import PotatoEngine.Core.Init.Prefab;
import PotatoEngine.Components;

namespace PotatoEngine::Core::Init {

    export void initialize_object(SafePtr<Object> owner, ObjectPrefab& prefab) {
        // go through each one
        // Initialize objects

        SafePtr<Object> object = owner->add_object_ns();
        object->name = prefab.name;

        for (ComponentPrefab& component : prefab.components) {
            component_constructors[component.component](object, component);
        }

        for (ObjectPrefab& new_object : prefab.objects) {
            initialize_object(object, new_object);
        }
    }

}