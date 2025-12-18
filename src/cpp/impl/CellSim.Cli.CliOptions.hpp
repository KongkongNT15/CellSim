#ifndef CELLSIM_CLI_CLIOPTIONS_HPP
#define CELLSIM_CLI_CLIOPTIONS_HPP

#include "base.hpp"
#include "CellSim.Cli.CliOptionType.hpp"

#include <map>

namespace CellSim::Cli
{
    struct CliOptions {
        private:

        ::std::map<CliOptionType, CliOption*> m_options;
        CliOption* m_controllerOption;

        void m_activeOption();

        void m_enableOption(
            int argc,
            char** argv
        );

        public:

        CliOptions(
            int argc,
            char** argv
        );

        CliOptions(
            CliOptions const&
        ) = delete;

        ~CliOptions();

        CliOptions& operator=(
            CliOptions const&
        ) = delete;

        SimulationOption CreateSimulationOption() const;

        void Run();
    };
}

#endif //!CELLSIM_CLI_CLIOPTIONS_HPP