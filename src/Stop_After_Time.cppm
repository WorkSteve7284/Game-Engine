export module Stop_After_Time;

import PotatoEngine.Core;

using namespace PotatoEngine::Core::Debug;

export class Stop_After_Time : public Component {
public:
	double target_time = 0;

	Stop_After_Time(double stop_time) : target_time(stop_time) {}
	//~Stop_After_Time() = default;

	void Start() override {
		Debug.Log("Starting");
	}

	void Update(double delta_time) override {
		if (Time::runtime >= target_time) {
			PotatoEngine::Get_Manager()->Stop();
		}
	}
};