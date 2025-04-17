#pragma once

class AntiStamina : public Cheat {
public:
    AntiStamina(const std::string& name) : Cheat(name) {}
    void Update() override;
    void Draw() override;
    void OnEnable() override;
    void OnDisable() override;
};