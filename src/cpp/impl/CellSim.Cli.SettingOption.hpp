#ifndef CELLSIM_CLI_SETTINGOPTION_HPP
#define CELLSIM_CLI_SETTINGOPTION_HPP

#include "base.hpp"
#include "CellSim.Cli.ValueOption.hpp"

namespace CellSim::Cli
{
    class SettingOption final : public ValueOption {
        private:

        static constexpr ::std::string_view s_fullName = "--setting";
        static constexpr ::std::string_view s_names = "-s, --setting";

        public:

        SettingOption();

        constexpr ::std::string_view FullName() const noexcept override;

        constexpr bool IsMatch(
            ::std::string_view optionName
        ) const noexcept override;

        constexpr ::std::string_view Names() const noexcept override;

        void OnActive(
            const CliOptions* sender,
            CliOptionActivationArgs args
        ) override;

        constexpr CliOptionType OptionType() const noexcept override;

        constexpr bool TakesControl() const noexcept override;
    };
}

namespace CellSim::Cli
{
    constexpr ::std::string_view SettingOption::FullName() const noexcept
    {
        return s_fullName;
    }

    constexpr bool SettingOption::IsMatch(
        ::std::string_view optionName
    ) const noexcept
    {
        using namespace std;
        return optionName == "-s"sv || optionName == "--setting"sv;
    }

    constexpr ::std::string_view SettingOption::Names() const noexcept
    {
        return s_names;
    }

    constexpr CliOptionType SettingOption::OptionType() const noexcept
    {
        return CliOptionType::Setting;
    }

    constexpr bool SettingOption::TakesControl() const noexcept
    {
        return false;
    }
}

#endif //!CELLSIM_CLI_SETTINGOPTION_HPP