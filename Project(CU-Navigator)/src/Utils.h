#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    // Converts a string to lowercase
    std::string toLower(const std::string& str);
    
    // Checks if the 'full' string contains the 'sub' string (case-insensitive)
    bool fuzzyMatch(const std::string& full, const std::string& sub);
    
    // Converts distance in meters to estimated walking time
    std::string formatTime(double distanceMeters, double speedKmH = 5.0);
}

#endif // UTILS_H
