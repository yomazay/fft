#include <iomanip>
#include <chrono>
#include <fstream>

#include <lob.h>
#include <parser.h>

int main(int argc, char **argv)
{


    std::ifstream in_file;

    if (argc == 1)
        in_file.open("../test/huobi.log");
    else
        in_file.open(argv[1]);

    if (!in_file.is_open())
    {
        std::cout << "ERROR: Couldn't open input file!" << std::endl;
        return 1;
    }

    Parser parser;

    std::string first_line;
    std::getline(in_file, first_line);

    snapshot_t snapshot = parser.get_snapshot(first_line);

    LOB lob(snapshot.asks, snapshot.bids, snapshot.timestamp);

    std::vector<update_t> update_vec;

    for (std::string upd_line; std::getline(in_file, upd_line);)
    {
        update_t update = parser.get_update(upd_line);
        update_vec.emplace_back(
                    std::move(update.asks),
                    std::move(update.bids),
                    std::move(update.timestamp));
    }

    in_file.close();



    typedef std::chrono::high_resolution_clock clock_t;
    typedef std::chrono::duration<long long, std::ratio<1, 1000000000>> nsec;
    std::vector<long long> benchmark_upd;
    std::vector<long long> benchmark_get;

    std::vector<lob_data_t> data_vec;
    lob_data_t lob_data;

    for (const auto &upd : update_vec)
    {

        std::chrono::time_point<clock_t> upd_timer_start(clock_t::now());

        if (lob.update(upd.asks, upd.bids, upd.timestamp))
        {
            benchmark_upd.emplace_back(std::chrono::duration_cast<nsec>(clock_t::now() - upd_timer_start).count());

            std::chrono::time_point<clock_t> get_timer_start(clock_t::now());

            lob_data = lob.get_data();

            benchmark_get.emplace_back(std::chrono::duration_cast<nsec>(clock_t::now() - get_timer_start).count());

            data_vec.emplace_back(std::move(lob_data));

        }

    }

    std::ofstream out_file;
    if (argc < 3)
        out_file.open("../test/result.log");
    else
        out_file.open(argv[3]);

    if (!out_file.is_open())
    {
        std::cout << "ERROR: Couldn't open output file!" << std::endl;
        return 1;
    }

    for(const auto &el : data_vec)
        out_file << std::setprecision(10) << el << std::endl;

    out_file << std::flush;
    out_file.close();

    long long min_time = *std::min_element(benchmark_get.begin(), benchmark_get.end());
    long long max_time = *std::max_element(benchmark_get.begin(), benchmark_get.end());
    long long full_time = 0;

    for(const auto &el : benchmark_upd)
        full_time += el;

    std::cout << "=== UPDATE === " << std::endl;
    std::cout << "Full time (nsec): " << full_time << std::endl;
    std::cout << "Min time (nsec): " << min_time << std::endl;
    std::cout << "Avg time (nsec): " << full_time / (double)benchmark_upd.size() << std::endl;
    std::cout << "Max time (nsec): " << max_time << std::endl;

    min_time = *std::min_element(benchmark_get.begin(), benchmark_get.end());
    max_time = *std::max_element(benchmark_get.begin(), benchmark_get.end());
    full_time = 0;

    for(const auto &el : benchmark_get)
        full_time += el;

    std::cout << std::endl << "=== GET === " << std::endl;
    std::cout << "Full time (nsec): " << full_time << std::endl;
    std::cout << "Min time (nsec): " << min_time << std::endl;
    std::cout << "Avg time (nsec): " << full_time / (double)benchmark_get.size() << std::endl;
    std::cout << "Max time (nsec): " << max_time << std::endl;

    return 0;
}
