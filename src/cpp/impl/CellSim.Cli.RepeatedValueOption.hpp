#ifndef CELLSIM_CLI_REPEATEDVALUEOPTION_HPP
#define CELLSIM_CLI_REPEATEDVALUEOPTION_HPP

#include "base.hpp"
#include "CellSim.Cli.CliOption.hpp"

#include <vector>
#include <string>

namespace CellSim::Cli
{
    /// @brief 複数回指定できるオプション
    class RepeatedValueOption : public CliOption {
        protected:

        ::std::vector<::std::string> m_values;

        public:

        void AddValue(
            ::std::string_view value
        ) override;

        constexpr bool HasValue() const noexcept override;
        constexpr bool IsRepeatable() const noexcept override;

        ::std::string const& Value() const override;

        constexpr ::std::vector<::std::string> const&
        Values(
        ) const noexcept override;
    };
}

namespace CellSim::Cli
{
    inline void RepeatedValueOption::AddValue(
        ::std::string_view value
    )
    {
        m_values.emplace_back(value.data(), value.size());
    }

    constexpr bool RepeatedValueOption::HasValue() const noexcept
    {
        return true;
    }

    constexpr bool RepeatedValueOption::IsRepeatable() const noexcept
    {
        return true;
    }

    constexpr ::std::vector<::std::string> const&
    RepeatedValueOption::Values(
    ) const noexcept
    {
        return m_values;
    }
}

#endif //!CELLSIM_CLI_REPEATEDVALUEOPTION_HPP