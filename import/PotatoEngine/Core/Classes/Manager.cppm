module;

#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>

export module PotatoEngine.Core.Classes.Manager;

import PotatoEngine.Core.Classes.Object;
import PotatoEngine.Core.Classes.SafePtr;
import PotatoEngine.Core.Exception;
import PotatoEngine.Core.Time.TimeVariables;

namespace PotatoEngine::Core::Classes {

    export class Manager {
    public:
        Manager() = default;

    public:
        std::uint_fast64_t new_uid() { return ++uid; }

        bool is_running() { return running; }
        bool stop() { running = false; return running; }

        void start();
        void run();
        void update(double);
        void fixed_update(double);
        void physics_update(double);

        SafePtr<Object> add_object();
        SafePtr<Object> add_object_ns();

        template <typename C, typename... Args> SafePtr<Object> add_object_with_component(Args&&...);
        template <typename C, typename... Args> SafePtr<Object> add_object_with_component_ns(Args&&...);

        SafePtr<Object> find_object_with_name(std::string);
        SafePtr<Object> find_object_with_uid(std::uint_fast64_t);
        SafePtr<Object> find_object_with_tag(std::string);
        SafePtr<Object> find_object_with_tags(std::vector<std::string>);

        inline SafePtr<Object> find_object(std::string find_name) { return find_object_with_name(find_name); }
        inline SafePtr<Object> find_object(std::uint_fast64_t find_uid) { return find_object_with_uid(find_uid); }
        inline SafePtr<Object> find_object(std::vector<std::string> find_tags) { return find_object_with_tags(find_tags); }
    public:
    private:
    private:

        std::vector<std::unique_ptr<Object>> top_objects;

        std::uint_fast64_t uid;

        bool running = true;
    };

    void Manager::start() {
        for (auto& object : top_objects) {
            if (object) object->start();
        }
    }

    void Manager::run() {
        auto start = std::chrono::steady_clock::now();
        auto last_update = std::chrono::steady_clock::now();
        auto last_fixed_update = std::chrono::steady_clock::now();
        auto last_physics_update = std::chrono::steady_clock::now();

        while (is_running()) {

            auto now = std::chrono::steady_clock::now();
            double dt = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_update).count()) * 1e-9;
            last_update = std::chrono::steady_clock::now();
            Time::updates++;
            update(dt);

            now = std::chrono::steady_clock::now();
            dt = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_fixed_update).count());
            if (dt >= 16'666'666) {
                last_fixed_update = std::chrono::steady_clock::now();
                Time::fixed_updates++;
                fixed_update(dt * 1e-9);
            }

            now = std::chrono::steady_clock::now();
            dt = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_update).count()) * 1e-9;
            last_physics_update = std::chrono::steady_clock::now();
            physics_update(dt);

            now = std::chrono::steady_clock::now();

            Time::runtime = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count()) * 1e-9;
        }
    }
    
    void Manager::update(double delta_time) {
        for (auto& object : top_objects) {
            if (object) object->update(delta_time);
        }
    }

    void Manager::fixed_update(double delta_time) {
        for (auto& object : top_objects) {
            if (object) object->fixed_update(delta_time);
        }
    }

    void Manager::physics_update(double delta_time) {
        for (auto& object : top_objects) {
            if (object) object->physics_update(delta_time);
        }
    }

    SafePtr<Object> Manager::add_object() {
        SafePtr<Object> object = add_object_ns();
        object->start();
        return object;
    }

    SafePtr<Object> Manager::add_object_ns() {
        std::unique_ptr<Object> object = std::make_unique<Object>();
        
        Object* object_ptr = object.get();

        top_objects.emplace_back(std::move(object));
        
        object_ptr->parent = nullptr;
        object_ptr->New_UID = [this]() { return this->new_uid(); };
        object_ptr->uid = new_uid();

        return object_ptr;
    
    }

    template <typename C, typename... Args> SafePtr<Object> Manager::add_object_with_component_ns(Args&&... args) {
        SafePtr<Object> object = add_object_ns();
        
        object->add_component_ns<C>(args...);

        return object;
    }

    template <typename C, typename... Args> SafePtr<Object> Manager::add_object_with_component(Args&&... args) {
        Object* object = add_object_with_component_ns<C>(args...);
        object->Start();
        return object;
    }

    SafePtr<Object> Manager::find_object_with_name(std::string find_name) {
        std::size_t find_name_hash = std::hash<std::string>()(find_name);

        for (auto& object : top_objects) {
            if (object->name_hash == find_name_hash) {
                return object.get();
            }
        }

        throw PotatoEngine::Core::Exception::NoObjectFound("No Object with specified name found!");
    }

    SafePtr<Object> Manager::find_object_with_uid(std::uint_fast64_t find_uid) {
        for (auto& object : top_objects) {
            if (object->uid == find_uid) {
                return object.get();
            }
        }

        throw PotatoEngine::Core::Exception::NoObjectFound("No Object with specified name found!");
    }

    SafePtr<Object> Manager::find_object_with_tag(std::string find_tag) {
        std::size_t find_tag_hash = std::hash<std::string>()(find_tag);

        for (auto& object : top_objects) {
            for (std::size_t o_tag_hash : object->tags_hash) {
                if (find_tag_hash == o_tag_hash) {
                    return object.get();
                }
            }
        }

        throw PotatoEngine::Core::Exception::NoObjectFound("No Object with specified name found!");
    }
    SafePtr<Object> Manager::find_object_with_tags(std::vector<std::string> find_tags) {
        std::vector<std::size_t> find_tags_hash(find_tags.size());

        std::hash<std::string> hasher;

        for (size_t i = 0; i < find_tags.size(); i++) {
            find_tags_hash[i] = hasher(find_tags[i]);
        }

        for (auto& object : top_objects) {
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
}

namespace PotatoEngine::Core {
    export Classes::Manager* manager;
}