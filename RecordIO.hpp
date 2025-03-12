#ifndef RECORDIO_HPP_
#define RECORDIO_HPP_

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class RecordIO {
public:
    // Default constructor
    RecordIO() = default;

    // CSV Export
    bool exportToCSV(const std::string& filename, const std::vector<std::vector<std::string>>& records);

    // CSV Import
    std::vector<std::vector<std::string>> importFromCSV(const std::string& filename);

    // JSON Export
    bool exportToJSON(const std::string& filename, const std::vector<std::vector<std::string>>& records);

    // JSON Import
    std::vector<std::vector<std::string>> importFromJSON(const std::string& filename);
};

#endif // RECORDIO_HPP_
