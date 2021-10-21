#include "lob.h"

#include <iomanip>

LOB::LOB(const u_map_t &asks, const u_map_t &bids, long long &timestamp) :
    _asks(asks),
    _bids(bids),
    _timestamp(timestamp)

{

}



bool LOB::update(const vec_t &asks, const vec_t &bids, const long long &timestamp)
{
    if (_timestamp > timestamp)
        return false;

    _timestamp = timestamp;


    for (const auto &pos : asks)
    {
        if (pos.second > 0.0)
            _asks[pos.first] = pos.second;
        else
        {
            auto erase_pos = _asks.find(pos.first);
            if (erase_pos != _asks.end())
                _asks.erase(erase_pos);
        }

    } // for

    // TODO: duplicated code

    for (const auto &pos : bids)
    {
        if (pos.second > 0.0)
            _bids[pos.first] = pos.second;
        else
        {
            auto erase_pos =_bids.find(pos.first);
            if (erase_pos != _bids.end())
                _bids.erase(erase_pos);
        }
    } // for

    return true;

}



const lob_data_t LOB::get_data()
{
    return lob_data_t(_timestamp, _bids.rbegin()->first, _bids.rbegin()->second, _asks.begin()->first, _asks.begin()->second);
}



std::ostream & operator<<(std::ostream &stream, const LOB &lob)
{

    stream << "=== LOB data ===" << std::endl;
    stream << "ASKS Price \tAmount" << std::endl;
    for (const auto &el : lob._asks)
        stream << std::setprecision(10) << el.first << " \t" << el.second << std::endl;

    stream << std::endl;
    stream << "BIDS Price \tAmount" << std::endl;
    for (const auto &el : lob._bids)
        stream << std::setprecision(10) << el.first << " \t" << el.second << std::endl;

    return stream;
}

