#include "Utils.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>

namespace Utils {
    std::string toLower(const std::string& str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
            [](unsigned char c){ return std::tolower(c); });
        return lowerStr;
    }

    bool fuzzyMatch(const std::string& full, const std::string& sub) {
        if (sub.empty()) return true;
        std::string lowerFull = toLower(full);
        std::string lowerSub = toLower(sub);
        return lowerFull.find(lowerSub) != std::string::npos;
    }

    std::string formatTime(double distanceMeters, double speedKmH) {
        // speed is km/h, distance is meters.
        // speed in m/s = (speedKmH * 1000) / 3600
        double speedMs = (speedKmH * 1000.0) / 3600.0;
        double timeSeconds = distanceMeters / speedMs;
        
        int minutes = static_cast<int>(timeSeconds / 60);
        int seconds = static_cast<int>(timeSeconds) % 60;
        
        std::stringstream ss;
        if (minutes > 0) {
            ss << minutes << " min ";
        }
        ss << seconds << " sec";
        return ss.str();
    }
}
