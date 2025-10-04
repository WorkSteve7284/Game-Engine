export module PotatoEngine.Core.Classes.Object;

import std;

import PotatoEngine.Core.Classes.Component;
import PotatoEngine.Core.Classes.SafePtr;
import PotatoEngine.Core.Classes.ObjectMap;
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
        
        std::function<std::uint_fast64_t(void)> new_uid = []() { return 0; };

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
        std::vector<SafePtr<Object>> find_objects_with_name(std::string);
        std::vector<SafePtr<Object>> find_objects_with_tag(std::string);
        std::vector<SafePtr<Object>> find_objects_with_tags(std::vector<std::string>);

        inline SafePtr<Object> find_object(std::string find_name) { return find_object_with_name(find_name); }
        inline SafePtr<Object> find_object(std::uint_fast64_t find_uid) { return find_object_with_uid(find_uid); }
        inline SafePtr<Object> find_object(std::vector<std::string> tags) { return find_object_with_tags(tags); }


        std::vector<std::string> get_tags();
        std::string add_tag(std::string);
        std::string remove_tag(std::string);

        void rehash_all();
        void rehash_name();

        void destroy();
        void destroy_object(SafePtr<Object>);
        void destroy_component(SafePtr<Component<Object>>);

        void clean_objects();
    public: // Member Variables
        Object* parent;


        PotatoEngine::Core::Event::EventManager events;

        std::uint_fast64_t uid = 0;

        std::string name;
        std::size_t name_hash;

        std::vector<std::size_t> tags_hash = {};

    protected: // Functions

        void rehash_tags();

    protected: // Member Variables
        std::vector<std::uint_fast64_t> to_remove;

        std::vector<std::string> tags = {};

        std::vector<std::unique_ptr<Object>> children;
    private:
        std::vector<std::unique_ptr<Component<Object>>> components;

    };

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

        components.push_back(std::move(component));

        component_ptr->uid = new_uid();

        object_map.add_object(component_ptr);

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
        for (SafePtr<Component<Object>> component : components) {
            if (component) component->start();
        }
        for (SafePtr<Object> object : children) {
            if (object) object->start();
        }
    }
    void Object::update(double delta_time) {
        for (SafePtr<Component<Object>> component : components) {
            if (component) component->update(delta_time);
        }
        for (SafePtr<Object> object : children) {
            if (object) object->update(delta_time);
        }
    }
    void Object::fixed_update(double delta_time) {
        for (SafePtr<Component<Object>> component : components) {
            if (component) component->fixed_update(delta_time);
        }
        for (SafePtr<Object> object : children) {
            if (object) object->fixed_update(delta_time);
        }
    }

    void Object::physics_update(double delta_time) {
        for (SafePtr<Component<Object>> component : components) {
            if (component) component->physics_update(delta_time);
        }
        for (SafePtr<Object> object : children) {
            if (object) object->physics_update(delta_time);
        }
    }

    SafePtr<Object> Object::add_object_ns() {
        std::unique_ptr<Object> object = std::make_unique<Object>();
        
        Object* object_ptr = object.get();

        children.emplace_back(std::move(object));

        object_ptr->parent = this;
        object_ptr->new_uid = [this]() { return this->new_uid(); };
        object_ptr->uid = new_uid();

        object_map.add_object(object_ptr);

        return object_ptr;
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

    //std::vector<SafePtr<Object>> Object::find_objects_with_name(std::string);

    //std::vector<SafePtr<Object>> Object::find_objects_with_tag(std::string);
    //std::vector<SafePtr<Object>> Object::find_objects_with_tags(std::vector<std::string>);

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

    void Object::destroy() {
        if (parent) {
            parent->destroy_object(this);
        }
    }

    void Object::destroy_object(SafePtr<Object> target) {
        const std::uint_fast64_t target_uid = target->uid;
        object_map.remove_object(target.get());
        to_remove.push_back(target_uid);
    }

    void Object::destroy_component(SafePtr<Component<Object>> target) {
        const std::uint_fast64_t target_uid = target->uid;
        object_map.remove_object(target.get());
        to_remove.push_back(target_uid);
    }

    void Object::clean_objects() {
        for (auto& target_uid : to_remove) {
            children.erase(std::remove_if(
                children.begin(),
                children.end(),
                [target_uid](const auto& c) {
                    return c->uid == target_uid;
                }
            ), children.end());
        }

        for (auto& target_uid : to_remove) {
            components.erase(std::remove_if(
                components.begin(),
                components.end(),
                [target_uid](const auto& c) {
                    return c->uid == target_uid;
                }
            ), components.end());
        }

        for (SafePtr<Object> object : children) {
            if (object) object->clean_objects();
        }
    }
}