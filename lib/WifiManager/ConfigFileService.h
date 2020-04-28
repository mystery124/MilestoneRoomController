#ifndef ConfigFileService_h
#define ConfigFileService_h

#include <FS.h>
#include <ArduinoJson.h>

#include "Logger.h"
#include "ConfigFile.h"

namespace WiFiManagment {

    class ConfigFileService {
        public:
        ConfigFileService();
        ConfigFile& readConfigFile();
        bool deleteConfigFile();
        bool saveConfigFile(String&);
        bool saveConfigFile(ConfigFile &configF);
        bool hasConfigFile();

        private:
        bool cFilieInitialized = false;
        String cofingFilePath = "/c/config.json";
        ConfigFile cFile = {};
    };
};

#endif