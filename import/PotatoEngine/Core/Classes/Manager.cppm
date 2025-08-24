module;

#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>

export module PotatoEngine.Core.Classes.Manager;

import PotatoEngine.Core.Classes.Object;
import PotatoEngine.Core.Exception;
import PotatoEngine.Core.Time;

namespace PotatoEngine::Core::Classes {

    export class Manager {
    public:
        Manager() = default;
        ~Manager() = default;

        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;

    public:
        std::uint_fast64_t New_UID() { return ++uid; }

        bool Is_Running() { return running; }
        bool Stop() { running = false; return running; }

        void Start();
        void Run();
        void Update(double);
        void Fixed_Update(double);

        Object& Add_Object();
        Object& Add_Object_No_Start();

        template <typename C, typename... Args> Object& Add_Object_With_Component_No_Start(Args&&...);
        template <typename C, typename... Args> Object& Add_Object_With_Component(Args&&...);

        Object& Find_Object_With_Name(std::string);
        Object& Find_Object_With_UID(std::uint_fast64_t);
        Object& Find_Object_With_Tag(std::string);
        Object& Find_Object_With_Tags(std::vector<std::string>);

        inline Object& Find_Object(std::string); // Name
        inline Object& Find_Object(std::uint_fast64_t); // UID
        inline Object& Find_Object(std::vector<std::string>); // Tags
    public:
    private:
    private:

        std::vector<std::unique_ptr<Object>> top_objects;

        std::uint_fast64_t uid;

        bool running = true;
    };

    void Manager::Start() {
        for (auto& object : top_objects) {
            if (object) object->Start();
        }
    }

    void Manager::Run() {
        auto start = std::chrono::steady_clock::now();
        auto last_update = std::chrono::steady_clock::now();
        auto last_fixed_update = std::chrono::steady_clock::now();

        while (Is_Running()) {

            auto now = std::chrono::steady_clock::now();
            double dt = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_update).count()) * 1e-9;
            last_update = std::chrono::steady_clock::now();
            Time::updates++;
            Update(dt);

            now = std::chrono::steady_clock::now();
            dt = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_fixed_update).count());
            if (dt >= 16'666'666) {
                last_fixed_update = std::chrono::steady_clock::now();
                Time::fixed_updates++;
                Fixed_Update(dt * 1e-9);
            }

            now = std::chrono::steady_clock::now();

            Time::runtime = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count()) * 1e-9;
        }
    }
    
    void Manager::Update(double delta_time) {
        for (auto& object : top_objects) {
            if (object) object->Update(delta_time);
        }
    }

    void Manager::Fixed_Update(double delta_time) {
        for (auto& object : top_objects) {
            if (object) object->Fixed_Update(delta_time);
        }
    }

    Object& Manager::Add_Object() {
        Object& object = Add_Object_No_Start();
        object.Start();
        return object;
    }

    Object& Manager::Add_Object_No_Start() {
        top_objects.emplace_back(std::make_unique<Object>());
        
        top_objects.back()->Get_Parent = [this]() { return nullptr; };
        top_objects.back()->New_UID = [this]() { return this->New_UID(); };
        top_objects.back()->uid = New_UID();

        return *(top_objects.back());
    
    }

    template <typename C, typename... Args> Object& Manager::Add_Object_With_Component_No_Start(Args&&... args) {
        Object& object = Add_Object_No_Start();
        
        object.Add_Component_With_Type_No_Start<C>(args...);

        return object;
    }

    template <typename C, typename... Args> Object& Manager::Add_Object_With_Component(Args&&... args) {
        Object& object = Add_Object_With_Component_No_Start<C>(args...);
        object.Start();
        return object;
    }

    Object& Manager::Find_Object_With_Name(std::string find_name) {
        std::size_t find_name_hash = std::hash<std::string>()(find_name);

        for (auto& object : top_objects) {
            if (object->name_hash == find_name_hash) {
                return *object;
            }
        }

        throw PotatoEngine::Core::Exception::No_Object_Found("No Object with specified name found!");
    }

    Object& Manager::Find_Object_With_UID(std::uint_fast64_t find_uid) {
        for (auto& object : top_objects) {
            if (object->uid == find_uid) {
                return *object;
            }
        }

        throw PotatoEngine::Core::Exception::No_Object_Found("No Object with specified name found!");
    }

    Object& Manager::Find_Object_With_Tag(std::string find_tag) {
        std::size_t find_tag_hash = std::hash<std::string>()(find_tag);

        for (auto& object : top_objects) {
            for (std::size_t o_tag_hash : object->tags_hash) {
                if (find_tag_hash == o_tag_hash) {
                    return *object;
                }
            }
        }

        throw PotatoEngine::Core::Exception::No_Object_Found("No Object with specified name found!");
    }
    Object& Manager::Find_Object_With_Tags(std::vector<std::string> find_tags) {
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
                return *object;
        }

        throw PotatoEngine::Core::Exception::No_Object_Found("No Object with specified tags found!");
    }
}