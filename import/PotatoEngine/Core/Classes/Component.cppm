module;

#include <functional>
#include <cstdint>

export module PotatoEngine.Core.Classes.Component;

namespace PotatoEngine::Core::Classes {

    export template <typename Object>
        class Component {
        public:
            std::uint_fast64_t uid; // Unique ID to each component and Object

            Object* object = nullptr; // Pointer to the Object which owns this component. Set upon component creation.

            Component() = default;
            virtual ~Component() = default;

            virtual void start() {}
            virtual void update(double) {}
            virtual void fixed_update(double) {}
            virtual void physics_update(double) {}
    };
}