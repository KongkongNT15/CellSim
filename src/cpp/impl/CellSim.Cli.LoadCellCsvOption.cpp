#include "CellSim.Cli.LoadCellCsvOption.hpp"

namespace CellSim::Cli
{
    ::std::string_view LoadCellCsvOption::FullName() const noexcept
    {
        return s_fullName;
    }

    bool LoadCellCsvOption::IsMatch(
        ::std::string_view optionName
    ) const noexcept
    {
        return optionName == s_fullName;
    }

    ::std::string_view LoadCellCsvOption::Names() const noexcept
    {
        return s_fullName;
    }

    CliOptionType LoadCellCsvOption::OptionType() const noexcept
    {
        return CliOptionType::LoadCellCsv;
    }

    bool LoadCellCsvOption::TakesControl() const noexcept
    {
        return false;
    }
}