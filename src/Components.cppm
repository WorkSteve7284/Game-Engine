export module PotatoEngine.Components;

import std;
using namespace std;

import PotatoEngine.Core.Classes;
import PotatoEngine.Core.Init.Prefab;
using namespace PotatoEngine::Core::Classes;

import test;

template <typename T>
auto component_constructor() {
    return [](SafePtr<Object> obj, PotatoEngine::Core::Init::ComponentPrefab prefab) {obj->add_component_ns<T>(prefab); };
}

export unordered_map<string, function<void(SafePtr<Object>, PotatoEngine::Core::Init::ComponentPrefab)>> component_constructors = {
    { "Test", component_constructor<Test>() }
};