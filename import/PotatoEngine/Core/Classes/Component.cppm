module;

#include <functional>
#include <cstdint>

export module PotatoEngine.Core.Classes.Component;

namespace PotatoEngine::Core::Classes {

    export template <typename Object>
        class Component {
        public:
            std::uint_fast64_t uid;

            std::function<Object* (void)> Get_Object = []() { return nullptr; };

            Component() = default;
            virtual ~Component() = default;

            virtual void Start() {}
            virtual void Update(double) {}
            virtual void Fixed_Update(double) {}
    };
}