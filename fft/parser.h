#ifndef PARSER_H
#define PARSER_H

#include <string>

#include <nlohmann/json.hpp>
#include <types.h>

struct snapshot_t
{
    snapshot_t() : asks{}, bids{} {}
    snapshot_t(const u_map_t &a, const u_map_t &b, const long long &t) :
        asks(a),
        bids(b),
        timestamp(t)
    {}

    u_map_t asks;
    u_map_t bids;
    long long timestamp;
};

struct update_t
{
    update_t() : asks{}, bids{} {}
    update_t(const vec_t &a, const vec_t &b, const long long &t) :
        asks(a),
        bids(b),
        timestamp(t)
    {}

    vec_t asks;
    vec_t bids;
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
