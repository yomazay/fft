#include "lob.h"

#include <iomanip>

LOB::LOB(const u_map_t &asks, const u_map_t &bids, long long &timestamp) :
    _asks(asks),
    _bids(bids),
    _timestamp(timestamp)

{
    auto comp = [](auto lhs, auto rhs) {return lhs.first < rhs.first;};

    _best_ask = std::min_element(_asks.begin(), _asks.end(), comp)->first;
    _best_bid = std::max_element(_bids.begin(), _bids.end(), comp)->first;

}



bool LOB::update(const vec_t &asks, const vec_t &bids, const long long &timestamp)
{
    if (_timestamp > timestamp)
        return false;

    _timestamp = timestamp;

    static auto comp = [](auto lhs, auto rhs) {return lhs.first < rhs.first;};

    bool find_best_ask = false;
    for (const auto &pos : asks)
    {
        if (pos.second > 0.0)
        {
            _asks[pos.first] = pos.second;
            if (!find_best_ask && _best_ask > pos.first)// || _asks.size() == 1)
                _best_ask = pos.first;
        }
        else
        {
            auto erase_pos = _asks.find(pos.first);
            if (erase_pos != _asks.end())
            {
                _asks.erase(erase_pos);
                if (_best_ask == pos.first)
                    find_best_ask = true;

            }
        }

    } // for
    if (find_best_ask)
        _best_ask = std::min_element(_asks.cbegin(), _asks.cend(), comp)->first;

    // TODO: duplicated code

    bool find_best_bid = false;
    for (const auto &pos : bids)
    {
        if (pos.second > 0.0)
        {
            _bids[pos.first] = pos.second;
            if (!find_best_bid && _best_bid < pos.first)// || _bids.size() == 1)
                _best_bid = pos.first;

        }
        else
        {
            auto erase_pos =_bids.find(pos.first);
            if (erase_pos != _bids.end())
            {
                _bids.erase(erase_pos);
                if (_best_bid == pos.first)
                    find_best_bid = true;

            }
        }
    } // for
    if (find_best_bid)
        _best_bid = std::max_element(_bids.begin(), _bids.end(), comp)->first;

    return true;

}



const lob_data_t LOB::get_data()
{
    return lob_data_t(_timestamp, _best_bid, _bids[_best_bid], _best_ask, _asks[_best_ask]);
}



std::ostream & operator<<(std::ostream &stream, const LOB &lob)
{

    stream << "=== LOB data ===" << std::endl;
    stream << "ASKS Price \tAmount" << std::endl;
    for (const auto &el : lob._asks)
    {
        stream << std::setprecision(10) << el.first << " \t" << el.second;
        if (el.first == lob._best_ask)
            stream << " \t<<< BEST" << std::endl;
        else
            stream << std::endl;
    }
    stream << std::endl;
    stream << "BIDS Price \tAmount" << std::endl;
    for (const auto &el : lob._bids)
    {
        stream << std::setprecision(10) << el.first << " \t" << el.second;
        if (el.first == lob._best_bid)
            stream << " \t<<< BEST" << std::endl;
        else
            stream << std::endl;
    }
    return stream;
}

