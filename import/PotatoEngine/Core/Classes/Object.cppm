module;

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>

export module PotatoEngine.Core.Classes.Object;

import PotatoEngine.Core.Classes.Component;
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
            std::function<Object* (void)> Get_Parent = []() { return nullptr; };
            std::function<std::uint_fast64_t(void)> New_UID = []() { return 0; };

            void Start();
            void Update(double);
            void Fixed_Update(double);

            template <typename C, typename... Args> C& Add_Component_With_Type(Args&&...);
            template <typename C, typename... Args> C& Add_Component_With_Type_No_Start(Args&&...);

            Object& Add_Object_No_Start();
            Object& Add_Object();

            template <typename C, typename... Args> Object& Add_Object_With_Component_No_Start(Args&&...);
            template <typename C, typename... Args> Object& Add_Object_With_Component(Args&&...);

            template <typename C> C& Find_Component_With_Type();
            template <typename C> inline C& Find_Component() { return Find_Component_With_Type<C>(); }

            Component<Object>& Find_Component_With_UID(std::uint_fast64_t);
            inline Component<Object>& Find_Component(std::uint_fast64_t find_uid) { return Find_Component_With_UID(find_uid); }

            Object& Find_Object_With_Name(std::string);
            Object& Find_Object_With_UID(std::uint_fast64_t);
            Object& Find_Object_With_Tag(std::string);
            Object& Find_Object_With_Tags(std::vector<std::string>);

            inline Object& Find_Object(std::string find_name) { return Find_Object_With_Name(find_name); }
            inline Object& Find_Object(std::uint_fast64_t find_uid) { return Find_Object_With_UID(find_uid); }
            inline Object& Find_Object(std::vector<std::string> tags) { return Find_Object_With_Tags(tags); }


            std::vector<std::string> Get_Tags();
            std::string Add_Tag(std::string);
            std::string Remove_Tag(std::string);

            void Rehash_All();
            void Rehash_Name();

        public: // Member Variables

            PotatoEngine::Core::Event_Manager Events;

            std::uint_fast64_t uid;

            std::string name;
            std::size_t name_hash;
            
            std::vector<std::size_t> tags_hash = {};

        private: // Functions
            
            void Rehash_Tags();

        private: // Member Variables
            std::vector<std::string> tags = {};

            std::vector<std::unique_ptr<Object>> children;
            std::vector<std::unique_ptr<Component<Object>>> components;

    };

    // Templated Function Definitions
    template <typename C, typename... Args>
    Object& Object::Add_Object_With_Component_No_Start(Args&&... args) {
        Object& object = Add_Object_No_Start();

        object.Add_Component_With_Type_No_Start<C>(args...);

        return object;
    }

    template <typename C, typename... Args>
    Object& Object::Add_Object_With_Component(Args&&... args) {
        Object& object = Add_Object_With_Component_No_Start<C>(args...);

        object.Start();

        return object;
    }

    template <typename C, typename... Args>
    C& Object::Add_Component_With_Type_No_Start(Args&&... args) {
        components.emplace_back(std::make_unique<C>(args...));

        components.back()->Get_Object = [this]() { return this; };

        return static_cast<C&>(*(components.back()));
    }

    template <typename C, typename... Args>
    C& Object::Add_Component_With_Type(Args&&... args) {
        C& component = Add_Component_With_Type_No_Start<C>(args...);

        component.Start();

        return component;
    }

    template <typename C>
    C& Object::Find_Component_With_Type() {
        constexpr std::type_info c_type = typeid(C);

        for (auto& component : components) {
            if (typeid(component) == c_type) {
                return *component;
            }
        }

        throw PotatoEngine::Core::Exception::No_Component_Found("No Component with specified type found!");
    }

    // Other Function Definitions
    void Object::Start() {
        for (auto& component : components) {
            if (component) component->Start();
        }
        for (auto& object : children) {
            if (object) object->Start();
        }
    }
    void Object::Update(double delta_time) {
        for (auto& component : components) {
            if (component) component->Update(delta_time);
        }
        for (auto& object : children) {
            if (object) object->Update(delta_time);
        }
    }
    void Object::Fixed_Update(double delta_time) {
        for (auto& component : components) {
            if (component) component->Fixed_Update(delta_time);
        }
        for (auto& object : children) {
            if (object) object->Fixed_Update(delta_time);
        }
    }

    Object& Object::Add_Object_No_Start() {
        children.emplace_back(std::make_unique<Object>());

        children.back()->Get_Parent = [this]() { return this; };
        children.back()->New_UID = [this]() { return this->New_UID(); };
        children.back()->uid = New_UID();

        return *(children.back());
    }

    Object& Object::Add_Object() {
        Object& object = Add_Object_No_Start();
        object.Start();
        return object;
    }

    Component<Object>& Object::Find_Component_With_UID(std::uint_fast64_t find_uid) {
        for (auto& component : components) {
            if (component->uid == find_uid) {
                return *component;
            }
        }
        throw PotatoEngine::Core::Exception::No_Component_Found("No Component with specified UID found!");
    }

    Object& Object::Find_Object_With_Name(std::string find_name) {
        std::size_t find_name_hash = std::hash<std::string>()(find_name);

        for (auto& object : children) {
            if (object->name_hash == find_name_hash) {
                return *object;
            }
        }

        throw PotatoEngine::Core::Exception::No_Object_Found("No Object with specified name found!");
    }

    Object& Object::Find_Object_With_UID(std::uint_fast64_t find_uid) {
        for (auto& object : children) {
            if (object->uid == find_uid) {
                return *object;
            }
        }
        
        throw PotatoEngine::Core::Exception::No_Object_Found("No Object with specified UID found!");
    }

    Object& Object::Find_Object_With_Tag(std::string find_tag) {
        std::size_t tag_hash = std::hash<std::string>()(find_tag);

        for (auto& object : children) {
            for (std::size_t o_tag_hash : object->tags_hash) {
                if (o_tag_hash == tag_hash) {
                    return *object;
                }
            }
        }

        throw PotatoEngine::Core::Exception::No_Object_Found("No Object with specified tag found!");
    }

    Object& Object::Find_Object_With_Tags(std::vector<std::string> find_tags) {
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
                return *object;
        }

        throw PotatoEngine::Core::Exception::No_Object_Found("No Object with specified tags found!");
    }

    std::vector<std::string> Object::Get_Tags() {
        Rehash_All();
        return tags;
    }

    std::string Object::Add_Tag(std::string new_tag) {
        tags.push_back(new_tag);
        Rehash_All();
        return new_tag;
    }

    std::string Object::Remove_Tag(std::string old_tag) {
        std::size_t old_hash = std::hash<std::string>()(old_tag);
        tags.erase(tags.begin() + std::distance(std::begin(tags_hash), std::find(tags_hash.begin(), tags_hash.end(), old_hash)));
        Rehash_All();
        return old_tag;
    }

    void Object::Rehash_All() {
        Rehash_Name();
        Rehash_Tags();
    }

    void Object::Rehash_Name() {
        name_hash = std::hash<std::string>()(name);
    }

    void Object::Rehash_Tags() {
        std::hash<std::string> hasher;

        std::vector<std::size_t> new_hashes(tags.size());

        for (std::size_t i = 0; i < tags.size(); i++) {
            new_hashes[i] = hasher(tags[i]);
        }

        tags_hash = new_hashes;
    }
}

