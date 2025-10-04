export module PotatoEngine.Core.Classes.Manager;

import std;

import PotatoEngine.Core.Classes.Object;
import PotatoEngine.Core.Classes.SafePtr;
import PotatoEngine.Core.Classes.ObjectMap;
import PotatoEngine.Core.Exception;
import PotatoEngine.Core.Time.TimeVariables;

namespace PotatoEngine::Core::Classes {

    export class Manager : public Object {
    public:
        Manager() { new_uid = [this]() { return ++uid; }; }

    public:
        //std::function<std::uint_fast64_t(void)> new_uid;

        bool is_running() { return running; }
        bool stop() { running = false; return running; }

        void start();
        void run();
        void update(double);
        void fixed_update(double);
        void physics_update(double);

    private:
        bool running = true;
    };

    void Manager::start() {
        for (auto& object : children) {
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
        for (SafePtr<Object> object : children) {
            if (object) object->update(delta_time);
        }
    }

    void Manager::fixed_update(double delta_time) {
        for (SafePtr<Object> object : children) {
            if (object) object->fixed_update(delta_time);
        }
    }

    void Manager::physics_update(double delta_time) {
        for (SafePtr<Object> object : children) {
            if (object) object->physics_update(delta_time);
        }
    }
}

namespace PotatoEngine::Core {
    export Classes::Manager* manager;
}