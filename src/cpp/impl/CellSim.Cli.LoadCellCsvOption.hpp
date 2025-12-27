#ifndef CELLSIM_CLI_LOADCELLCSVOPTION_HPP
#define CELLSIM_CLI_LOADCELLCSVOPTION_HPP

#include "base.hpp"
#include "CellSim.Cli.ValueOption.hpp"

#include <string_view>

namespace CellSim::Cli
{
    class LoadCellCsvOption final : public ValueOption {
        private:
        static constexpr ::std::string_view s_fullName = "--load-cell-csv";

        public:

        ::std::string_view FullName() const noexcept override;

        bool IsMatch(::std::string_view optionName) const noexcept override;

        ::std::string_view Names() const noexcept override;

        CliOptionType OptionType() const noexcept override;

        bool TakesControl() const noexcept override;
    };
}

#endif //!CELLSIM_CLI_LOADCELLCSVOPTION_HPP