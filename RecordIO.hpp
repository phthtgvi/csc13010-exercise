#ifndef RECORDIO_HPP_
#define RECORDIO_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class RecordIO {
public:
    // Default constructor
    RecordIO() = default;

    // CSV Export
    bool exportToCSV(const std::string& filename, const std::vector<std::vector<std::string>>& records) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
            return false;
        }

        for (const auto& record : records) {
            for (size_t i = 0; i < record.size(); ++i) {
                file << record[i];
                if (i < record.size() - 1) {
                    file << ",";
                }
            }
            file << std::endl;
        }

        file.close();
        std::cout << "Successfully exported to CSV file: " << filename << std::endl;
        return true;
    }

    // CSV Import
    std::vector<std::vector<std::string>> importFromCSV(const std::string& filename) {
        std::vector<std::vector<std::string>> records;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
            return records; // Return an empty vector
        }

        while (std::getline(file, line)) {
            std::vector<std::string> record;
            std::stringstream lineStream(line);
            std::string cell;

            while (std::getline(lineStream, cell, ',')) {
                record.push_back(cell);
            }
            records.push_back(record);
        }

        file.close();
        std::cout << "Successfully imported from CSV file: " << filename << std::endl;
        return records;
    }

    // JSON Export
    bool exportToJSON(const std::string& filename, const std::vector<std::vector<std::string>>& records) {
        json j = json::array(); // Create a JSON array to hold the records

        for (const auto& record : records) {
            json record_json = json::array(); // Create a JSON array for each record
            for (const auto& cell : record) {
                record_json.push_back(cell);
            }
            j.push_back(record_json);
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
            return false;
        }

        file << std::setw(4) << j << std::endl; // Write JSON to file with indentation
        file.close();

        std::cout << "Successfully exported to JSON file: " << filename << std::endl;
        return true;
    }

    // JSON Import
    std::vector<std::vector<std::string>> importFromJSON(const std::string& filename) {
        std::vector<std::vector<std::string>> records;
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
            return records; // Return an empty vector
        }

        try {
            json j;
            file >> j; // Read the entire JSON from the file

            if (j.is_array()) {
                for (const auto& record_json : j) {
                    if (record_json.is_array()) {
                        std::vector<std::string> record;
                        for (const auto& cell_json : record_json) {
                            if (cell_json.is_string()) {
                                record.push_back(cell_json.get<std::string>());
                            }
                            else {
                                std::cerr << "Warning: Non-string value found in JSON record. Skipping.\n";
                            }
                        }
                        records.push_back(record);
                    }
                    else {
                        std::cerr << "Warning: Non-array element found in JSON data. Skipping.\n";
                    }
                }
            }
            else {
                std::cerr << "Error: JSON data is not an array.\n";
            }
        }
        catch (const json::parse_error& e) {
            std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
        }

        file.close();
        std::cout << "Successfully imported from JSON file: " << filename << std::endl;
        return records;
    }
};

#endif // RECORDIO_HPP_
