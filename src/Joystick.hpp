#include <fstream>
#include <gecom/Window.hpp>

namespace pxljm {
    class JoystickAxisProfile {
        int m_axisIndex;
        float m_deadMin, m_deadMax;
        bool m_invert;
        float m_scale;

    public:
        JoystickAxisProfile(int index, float min, float max, bool invert, float scale) :
            m_axisIndex(index),
            m_deadMin(min),
            m_deadMax(max),
            m_invert(invert),
            m_scale(scale) { }

        int index() { return m_axisIndex; }
        float deadMin() { return m_deadMin; }
        float deadMax() { return m_deadMax; }
        float invert() { return m_invert ? -1 : 1; }
        float scale() { return m_scale; }
    };

    class ButtonProfile {
        int m_btn;
        float m_amt;
        bool m_invert;

    public:
        ButtonProfile(int btn, float amt, bool invert) :
            m_btn(btn),
            m_amt(amt),
            m_invert(invert) { }

        int btn() { return m_btn; }
        float invert() { return m_invert ? -1 : 1; }
        float amt() { return m_amt; }
    };

    class JoystickProfile {
        std::map<std::string, std::shared_ptr<JoystickAxisProfile>> m_axes;
        std::map<std::string, std::shared_ptr<ButtonProfile>> m_buttons;
                bool m_keyboard;
    public:
        void setKeyboard(bool k) {
            m_keyboard = k;
        }

        bool keyboard() { return m_keyboard; }

        void addAxis(std::string role, std::shared_ptr<JoystickAxisProfile> p) {
            m_axes[role] = p;
        }

        void addButton(std::string role, std::shared_ptr<ButtonProfile> b) {
            m_buttons[role] = b;
        }

        bool hasAxisWithRole(std::string role) {
            return m_axes.count(role) == 1;
        }

        bool hasButtonWithRole(std::string role) {
            return m_buttons.count(role) == 1;
        }

        std::shared_ptr<JoystickAxisProfile> getAxisWithRole(std::string role) {
            return m_axes[role];
        }

        std::shared_ptr<ButtonProfile> getButtonWithRole(std::string role) {
            return m_buttons[role];
        }
    };

    class Joystick {
        int m_connectedIndex;
        std::shared_ptr<JoystickProfile> m_profile;
    public:
        Joystick(int connectedIndex, std::shared_ptr<JoystickProfile> p) : m_connectedIndex(connectedIndex), m_profile(p) { }

        float getAxisValue(std::string role) {
            if(m_connectedIndex < 0) return 0;
            int nAxes;
            const float* axes = glfwGetJoystickAxes(m_connectedIndex, &nAxes);

            if(!m_profile->hasAxisWithRole(role))
                return 0;

            auto roleProfile = m_profile->getAxisWithRole(role);
            float axisValue = axes[roleProfile->index()];

            if(axisValue >= roleProfile->deadMin() && axisValue <= roleProfile->deadMax())
                axisValue = 0;

            axisValue *= roleProfile->invert();
            axisValue *= roleProfile->scale();

            return axisValue;
        }

        float getButtonValue(std::string role, gecom::WindowEventProxy &wep) {
            if(!m_profile->hasButtonWithRole(role))
                return 0;

            auto button = m_profile->getButtonWithRole(role);

            if(m_profile->keyboard()) {
                if(wep.getKey(button->btn()))
                    return button->amt() * button->invert();
            } else {
            }

            return 0;
        }
    };

    class JoystickManager {
        std::map<std::string, std::shared_ptr<JoystickProfile> > m_profiles;
    public:
        JoystickManager(std::string fname) {
            std::ifstream configFile(fname);

            if (!configFile)
                throw std::runtime_error("can't load input configurations");

            json config;
            config << configFile;

            for(auto profile_json : config["joystick_profiles"]) {
                auto profile = std::make_shared<JoystickProfile>();
                profile->setKeyboard(profile_json["name"] == "Keyboard" ? true : false);

                for(auto stick_json : profile_json["sticks"]) {
                    gecom::Log::info() << "stick_json: " << stick_json;
                    auto stick = std::make_shared<JoystickAxisProfile>(stick_json["axis"], stick_json["dead"][0], stick_json["dead"][1], stick_json["invert"], stick_json["scale"]);
                    profile->addAxis(stick_json["role"], stick);
                }

                for(auto button_json : profile_json["buttons"]) {
                    gecom::Log::info() << "button_json: " << button_json;
                    auto button = std::make_shared<ButtonProfile>(button_json["btn"], button_json["amt"], button_json["invert"]);
                    profile->addButton(button_json["role"], button);
                }

                m_profiles[profile_json["name"]] = profile;
            }
        }

        std::shared_ptr<Joystick> findJoystick() {
            for(int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; i++) {
                if(!glfwJoystickPresent(i)) continue;

                const char* name = glfwGetJoystickName(i);
				gecom::Log::info() << "Testing against: " << name;
                if(m_profiles.count(name) == 1) {
                    gecom::Log::info() << "Found joystick: " << name;
                    return std::make_shared<Joystick>(i, m_profiles[name]);
                }
            }

            gecom::Log::info() << "Using keyboard for input";
            return std::make_shared<Joystick>(-1, m_profiles["Keyboard"]);
        }
    };
}