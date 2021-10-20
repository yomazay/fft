#ifndef LOB_H
#define LOB_H

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

struct lob_data_t
{
    lob_data_t() {}
    lob_data_t(const long long &t, const double &b_p, const long double &b_a, const double &a_p, const long double &a_a) :
        timestamp(t),
        bid_price(b_p),
        bid_amount(b_a),
        ask_price(a_p),
        ask_amount(a_a)
    {}
    friend std::ostream& operator<<(std::ostream& stream, const lob_data_t &l_d)
    {
        stream << l_d.timestamp << ", " << l_d.bid_price << ", " << l_d.bid_amount << ", " << l_d.ask_price << ", " << l_d.ask_amount;
        return stream;
    }

    long long timestamp;
    double bid_price;
    long double bid_amount;
    double ask_price;
    long double ask_amount;

};

class LOB
{
public:
    typedef std::unordered_map<double, long double> u_map_t;
    typedef std::vector<std::pair<double, long double>> vec_t;

    LOB(const u_map_t &asks, const u_map_t &bids, long long &timestamp);
    bool update(const vec_t &asks, const vec_t &bids, const long long &timestamp);
    const lob_data_t get_data();
    friend std::ostream& operator<<(std::ostream& stream, const LOB &lob);

private:
    u_map_t _asks;
    u_map_t _bids;
    long long _timestamp;

    double _best_ask;
    double _best_bid;

};

#endif // LOB_H
