export module PotatoEngine.Core.Classes.Component;

import std;

import PotatoEngine.Core.Classes.SafePtr;

namespace PotatoEngine::Core::Classes {

    export template <typename Object> class Component {
    public:
        std::uint_fast64_t uid;

        SafePtr<Object> object = nullptr; // Pointer to the Object which owns this component. Set upon component creation.

        Component() = default;
        virtual ~Component() = default;

        virtual void start() {}
        virtual void update(double) {}
        virtual void fixed_update(double) {}
        virtual void physics_update(double) {}

        void destroy();
    };

    template <typename O> void Component<O>::destroy() {
        object->destroy_component(this);
    }
}