#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

struct snapshot_t
{
    snapshot_t() : asks{}, bids{} {}
    snapshot_t(const std::unordered_map<double, long double> &a, const std::unordered_map<double, long double> &b, const long long &t) :
        asks(a),
        bids(b),
        timestamp(t)
    {}

    std::unordered_map<double, long double> asks;
    std::unordered_map<double, long double> bids;
    long long timestamp;
};

struct update_t
{
    update_t() : asks{}, bids{} {}
    update_t(const std::vector<std::pair<double, long double>> &a, const std::vector<std::pair<double, long double>> &b, const long long &t) :
        asks(a),
        bids(b),
        timestamp(t)
    {}

    std::vector<std::pair<double, long double>> asks;
    std::vector<std::pair<double, long double>> bids;
    long long timestamp;
};

class Parser
{
    std::string parse_from = "Get Object: "; // TODO: magic number

    nlohmann::json get_json(const std::string &str);

public:
    Parser();
    snapshot_t get_snapshot(const std::string &str);
    update_t get_update(const std::string &str);

};

#endif // PARSER_H
