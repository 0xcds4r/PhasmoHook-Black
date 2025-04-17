#pragma once

class Config {
private:
    nlohmann::json jsonObj;
    std::filesystem::path configPath;

    bool loadFromFile() {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            return false;
        }
        try {
            file >> jsonObj;
            return true;
        }
        catch (const std::exception& e) {
            /*Log(std::string("Failed to load config: ") + e.what());*/
            return false;
        }
    }

    bool saveToFile() {
        std::ofstream file(configPath, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            //Log("Failed to open config file for writing");
            return false;
        }
        try {
            file << jsonObj.dump(4); 
            return true;
        }
        catch (const std::exception& e) {
            //Log(std::string("Failed to save config: ") + e.what());
            return false;
        }
    }

public:
    Config(std::filesystem::path path)
    {
        configPath = path;
        if (!loadFromFile()) {
            initConfig();
        }
    }

    void initConfig() {
        jsonObj = {
            {"version", "1.3"},
            {"version_code", 130},
            {"version_tag", "BLACK"},
            {"locale", "eng"}
        };
        saveToFile();
    }

    std::string getVersion() const {
        return jsonObj.value("version", "Dev"); 
    }

    int getVersionCode() const {
        return jsonObj.value("version_code", 777);
    }

    std::string getVersionTag() const {
        return jsonObj.value("version_tag", "black"); 
    }

    std::string getLocale() const {
        return jsonObj.value("locale", "eng");
    }

    void setVersion(const std::string& version) {
        jsonObj["version"] = version;
        saveToFile();
    }

    void setVersionCode(int code) {
        jsonObj["version_code"] = code;
        saveToFile();
    }

    void setVersionTag(const std::string& tag) {
        jsonObj["version_tag"] = tag;
        saveToFile();
    }

    void setLocale(const std::string& locale) {
        jsonObj["locale"] = locale;
        saveToFile();
    }
};