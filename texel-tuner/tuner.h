#pragma once
#include "cstdint"
#include "string"
#include "vector"

namespace Tuner {
    struct DataSource {
        std::string path;
        bool side_to_move_wdl;
        int64_t position_limit;
    };

    void run(const std::vector<DataSource>& sources);
}
