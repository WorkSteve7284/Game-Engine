module;

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>

export module PotatoEngine.Core.Classes.Object;

import PotatoEngine.Core.Classes.Component;
import PotatoEngine.Core.Classes.SafePtr;
import PotatoEngine.Core.Event;
import PotatoEngine.Core.Exception;

namespace PotatoEngine::Core::Classes {

    export class Object {

    public: // Constructors, Destructors, Etc.
        Object() = default;
        ~Object() = default;

        Object(const Object&) = delete;
        Object& operator=(const Object&) = delete;

    public: // Functions
        
        std::function<std::uint_fast64_t(void)> New_UID = []() { return 0; };

        void start();
        void update(double);
        void fixed_update(double);
        void physics_update(double);

        template <typename C, typename... Args> SafePtr<C> add_component(Args&&...);
        template <typename C, typename... Args> SafePtr<C> add_component_ns(Args&&...);

        SafePtr<Object> add_object();
        SafePtr<Object> add_object_ns();

        template <typename C, typename... Args> SafePtr<Object> add_object_with_component_ns(Args&&...);
        template <typename C, typename... Args> SafePtr<Object> add_object_with_component(Args&&...);

        template <typename C> bool has_component();
        template <typename C> SafePtr<C> find_component();

        SafePtr<Object> find_object_with_name(std::string);
        SafePtr<Object> find_object_with_uid(std::uint_fast64_t);
        SafePtr<Object> find_object_with_tag(std::string);
        SafePtr<Object> find_object_with_tags(std::vector<std::string>);

        inline SafePtr<Object> find_object(std::string find_name) { return find_object_with_name(find_name); }
        inline SafePtr<Object> find_object(std::uint_fast64_t find_uid) { return find_object_with_uid(find_uid); }
        inline SafePtr<Object> find_object(std::vector<std::string> tags) { return find_object_with_tags(tags); }


        std::vector<std::string> get_tags();
        std::string add_tag(std::string);
        std::string remove_tag(std::string);

        void rehash_all();
        void rehash_name();

    public: // Member Variables
        Object* parent;

        PotatoEngine::Core::Event::EventManager events;

        std::uint_fast64_t uid = 0;

        std::string name;
        std::size_t name_hash;

        std::vector<std::size_t> tags_hash = {};

    private: // Functions

        void rehash_tags();

    private: // Member Variables
        std::vector<std::string> tags = {};

        std::vector<std::unique_ptr<Object>> children;
        std::vector<std::unique_ptr<Component<Object>>> components;

    };
}

using namespace PotatoEngine::Core::Classes;

// Templated Function Definitions
template <typename C, typename... Args>
SafePtr<Object> Object::add_object_with_component_ns(Args&&... args) {
    SafePtr<Object> object = add_object_ns();

    object->add_component_ns<C>(args...);

    return object;
}

template <typename C, typename... Args>
SafePtr<Object> Object::add_object_with_component(Args&&... args) {
    SafePtr<Object> object = add_object_with_component_ns<C>(args...);

    object->start();

    return object;
}

template <typename C, typename... Args>
SafePtr<C> Object::add_component_ns(Args&&... args) {
    std::unique_ptr<C> component = std::make_unique<C>(args...);
    
    component->object = this;
    C* component_ptr = component.get();

    components.emplace_back(std::move(component));

    return component_ptr;
}

template <typename C, typename... Args>
SafePtr<C> Object::add_component(Args&&... args) {
    SafePtr<C> component = add_component_ns<C>(args...);

    component->start();

    return component;
}

template <typename C> bool Object::has_component() {
    for (auto& component : components) {
        if (dynamic_cast<C*>(component.get())) {
            return true;
        }
    }

    return false;
}

template <typename C>
SafePtr<C> Object::find_component() {
    for (auto& component : components) {
        if (auto c = dynamic_cast<C*>(component.get())) {
            return c;
        }
    }

    throw PotatoEngine::Core::Exception::NoComponentFound("No Component with specified type found!");
}

// Other Function Definitions
void Object::start() {
    for (auto& component : components) {
        if (component) component->start();
    }
    for (auto& object : children) {
        if (object) object->start();
    }
}
void Object::update(double delta_time) {
    for (auto& component : components) {
        if (component) component->update(delta_time);
    }
    for (auto& object : children) {
        if (object) object->update(delta_time);
    }
}
void Object::fixed_update(double delta_time) {
    for (auto& component : components) {
        if (component) component->fixed_update(delta_time);
    }
    for (auto& object : children) {
        if (object) object->fixed_update(delta_time);
    }
}

void Object::physics_update(double delta_time) {
    for (auto& component : components) {
        if (component) component->physics_update(delta_time);
    }
    for (auto& object : children) {
        if (object) object->physics_update(delta_time);
    }
}

SafePtr<Object> Object::add_object_ns() {
    children.emplace_back(std::make_unique<Object>());

    children.back()->parent = this;
    children.back()->New_UID = [this]() { return this->New_UID(); };
    children.back()->uid = New_UID();

    return children.back().get();
}

SafePtr<Object> Object::add_object() {
    SafePtr<Object> object = add_object_ns();
    object->start();
    return object;
}

SafePtr<Object> Object::find_object_with_name(std::string find_name) {
    std::size_t find_name_hash = std::hash<std::string>()(find_name);

    for (auto& object : children) {
        if (object->name_hash == find_name_hash) {
            return object.get();
        }
    }

    throw PotatoEngine::Core::Exception::NoObjectFound("No Object with specified name found!");
}

SafePtr<Object> Object::find_object_with_uid(std::uint_fast64_t find_uid) {
    for (auto& object : children) {
        if (object->uid == find_uid) {
            return object.get();
        }
    }

    throw PotatoEngine::Core::Exception::NoObjectFound("No Object with specified UID found!");
}

SafePtr<Object> Object::find_object_with_tag(std::string find_tag) {
    std::size_t tag_hash = std::hash<std::string>()(find_tag);

    for (auto& object : children) {
        for (std::size_t o_tag_hash : object->tags_hash) {
            if (o_tag_hash == tag_hash) {
                return object.get();
            }
        }
    }

    throw PotatoEngine::Core::Exception::NoObjectFound("No Object with specified tag found!");
}

SafePtr<Object> Object::find_object_with_tags(std::vector<std::string> find_tags) {
    std::vector<std::size_t> find_tags_hash(find_tags.size());

    std::hash<std::string> hasher;

    for (size_t i = 0; i < find_tags.size(); i++) {
        find_tags_hash[i] = hasher(find_tags[i]);
    }

    for (auto& object : children) {
        std::vector<bool> found_tags_bool(find_tags_hash.size());

        for (std::size_t o_tag_hash : object->tags_hash) {
            for (std::size_t i = 0; i < find_tags_hash.size(); i++) {
                if (o_tag_hash == find_tags_hash[i]) {
                    found_tags_bool[i] = true;
                }
            }
        }

        bool works = true;
        for (bool found_tag_bool : found_tags_bool) {
            if (!found_tag_bool) {
                works = false;
                break;
            }
        }

        if (works)
            return object.get();
    }

    throw PotatoEngine::Core::Exception::NoObjectFound("No Object with specified tags found!");
}

std::vector<std::string> Object::get_tags() {
    rehash_all();
    return tags;
}

std::string Object::add_tag(std::string new_tag) {
    tags.push_back(new_tag);
    rehash_all();
    return new_tag;
}

std::string Object::remove_tag(std::string old_tag) {
    std::size_t old_hash = std::hash<std::string>()(old_tag);
    tags.erase(tags.begin() + std::distance(std::begin(tags_hash), std::find(tags_hash.begin(), tags_hash.end(), old_hash)));
    rehash_all();
    return old_tag;
}

void Object::rehash_all() {
    rehash_name();
    rehash_tags();
}

void Object::rehash_name() {
    name_hash = std::hash<std::string>()(name);
}

void Object::rehash_tags() {
    std::hash<std::string> hasher;

    std::vector<std::size_t> new_hashes(tags.size());

    for (std::size_t i = 0; i < tags.size(); i++) {
        new_hashes[i] = hasher(tags[i]);
    }

    tags_hash = new_hashes;
}