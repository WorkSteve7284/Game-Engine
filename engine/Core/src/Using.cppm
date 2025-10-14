export module PotatoEngine.Core.Using;

import PotatoEngine.Core.Classes;

export using Manager = PotatoEngine::Core::Classes::Manager;
export using Object = PotatoEngine::Core::Classes::Object;
export using Component = PotatoEngine::Core::Classes::Component<Object>;
export template <typename T> using SafePtr = PotatoEngine::Core::Classes::SafePtr<T>;