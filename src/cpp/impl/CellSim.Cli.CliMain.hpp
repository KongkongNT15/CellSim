#ifndef CELLSIM_CLI_CLIMAIN_HPP
#define CELLSIM_CLI_CLIMAIN_HPP

#include "base.hpp"

namespace CellSim::Cli
{
    class CliMain final {
        public:

        CELLSIM_STATIC_CLASS(CliMain);

        [[nodiscard]]
        static int Run(
            int argc,
            char** argv
        ) noexcept;
    };
}

#endif //!CELLSIM_CLI_CLIMAIN_HPP