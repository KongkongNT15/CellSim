#ifndef CELLSIM_CLI_VIDEOOPTION_HPP
#define CELLSIM_CLI_VIDEOOPTION_HPP

#include "base.hpp"
#include "CellSim.Cli.FlagOption.hpp"

namespace CellSim::Cli
{
    class VideoOption final : public FlagOption {
        private:

        static constexpr ::std::string_view s_fullName = "--video";
        static constexpr ::std::string_view s_names = "-v, --video";

        public:

        VideoOption() = default;

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
    constexpr ::std::string_view VideoOption::FullName() const noexcept
    {
        return s_fullName;
    }

    constexpr bool VideoOption::IsMatch(
        ::std::string_view optionName
    ) const noexcept
    {
        using namespace std;
        return optionName == "-v"sv || optionName == "--video"sv;
    }

    constexpr ::std::string_view VideoOption::Names() const noexcept
    {
        return s_names;
    }

    constexpr CliOptionType VideoOption::OptionType() const noexcept
    {
        return CliOptionType::Video;
    }

    constexpr bool VideoOption::TakesControl() const noexcept
    {
        return false;
    }
}

#endif //!CELLSIM_CLI_VIDEOOPTION_HPP