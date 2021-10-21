#include "parser.h"

#include <iostream>
#include <exception>

Parser::Parser()
{

}

snapshot_t Parser::get_snapshot(const std::string &str)
{
    snapshot_t snapshot;

    try
    {
        nlohmann::json json_data = get_json(str);

        for (const auto &el : json_data["asks"])
            snapshot.asks[el[0]] = el[1];

        for (const auto &el : json_data["bids"])
            snapshot.bids[el[0]] = el[1];

        snapshot.timestamp = json_data["event_time"];

    }
    catch (...)
    {
        throw;
    }

    return snapshot;
}

update_t Parser::get_update(const std::string &str)
{
    update_t update;

    try
    {
        nlohmann::json json_data = get_json(str);

        for (auto &el : json_data["asks"])
            update.asks.emplace_back(el[0], el[1]);

        for (auto &el : json_data["bids"])
            update.bids.emplace_back(el[0], el[1]);

        update.timestamp = json_data["event_time"];

    }
    catch (...)
    {
        throw;
    }

    return update;
}

nlohmann::json Parser::get_json(const std::string &str)
{
    size_t parse_pos = str.find(parse_from);
    if (parse_pos == std::string::npos)
        throw std::runtime_error("ERROR: Couldn't parse snapshot data!");

    return nlohmann::json::parse(str.substr(parse_from.size() + parse_pos));
}
