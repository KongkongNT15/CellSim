#include "CellSim.Cli.ParamSweepOption.hpp"
#include "CellSim.Cli.ParamOption.hpp"
#include "CellSim.Cli.CliOptionArgs.hpp"
#include "CellSim.IO.DirectoryCreater.hpp"
#include "CellSim.Messages.hpp"
#include "CellSim.Simulation.hpp"
#include "CellSim.SimulationOption.hpp"
#include "CellSim.Settings.Config.hpp"
#include "CellSim.Text.JsonHelper.hpp"
#include <nlohmann/json.hpp>

#include <sstream>
#include <stdexcept>

namespace CellSim::Cli
{

    template <Numerics::NumberType TNum>
    void ParamSweepOption::s_sweep(
        ::std::string_view first,
        ::std::string_view second,
        ::std::string_view third,
        ::std::string const& paramName,
        ::std::vector<::std::string>::const_iterator current,
        ::std::vector<::std::string>::const_iterator end,
        nlohmann::json& config,
        ::std::string const& path,
        CliOptionArgs args
    )
    {
        TNum begin;
        TNum last;
        TNum delta;

        ::std::string buffer;

        buffer.append(first);
        buffer.push_back(' ');
        buffer.append(second);
        buffer.push_back(' ');
        buffer.append(third);
        buffer.push_back(' ');

        ::std::istringstream sin(::std::move(buffer));

        sin >> begin >> last >> delta;

        if (!sin) [[unlikely]] throw ::std::runtime_error("Cli.ParamSweepOption.Run.Error");

        ::nlohmann::json* j = Text::JsonHelper::GetParam(config, paramName);

        if (j == nullptr) [[unlikely]] {
            throw ::std::runtime_error(
                Messages::Get(
                    "Cli.ParamSweepOption.CheckValue.Error.NotFound"
                )
            );
        }
        if (!j->is_number()) [[unlikely]] {
            throw ::std::runtime_error(
                Messages::Get(
                    "Cli.ParamSweepOption.CheckValue.Error.NotNumber"
                )
            );
        }

        auto next = current + 1;

        // 設定を適用して出力ディレクトリのパスを取得
        auto f = [&] (TNum current) {
            *j = current;

            ::std::ostringstream sout;
            sout << path << paramName << '=' << current << '/';

            ::std::string path = sout.str();

            IO::DirectoryCreater::Create(path);

            return path;
        };

        if (next == end) {
            for (TNum current = begin; current <= last; current += delta) {

                ::std::string path = f(current);

                SimulationOption option{
                    *args.Options,
                    ::std::move(path)
                };

                Simulation simulation{ ::std::move(option), config };

                simulation.Run();
            }
        }
        else {
            for (TNum current = begin; current <= last; current += delta) {
                ::std::string path = f(current);
                s_run(
                    next,
                    end,
                    path,
                    args
                );
            }
        }
        
    }

    void ParamSweepOption::s_run(
        ::std::vector<::std::string>::const_iterator current,
        ::std::vector<::std::string>::const_iterator end,
        ::std::string const& path,
        CliOptionArgs args
    )
    {
        if (current == end) return;

        ::std::string_view paramView = *current;

        size_t index = paramView.find('=');

        if (index == ::std::string_view::npos) [[unlikely]] {
            throw ::std::runtime_error("Cli.ParamSweepOption.Run.Error");
        }

        paramView = paramView.substr(index + 1);
        ::std::string paramName = current->substr(0, index);

        index = paramView.find(':');

        if (index == ::std::string_view::npos) [[unlikely]] {
            throw ::std::runtime_error("Cli.ParamSweepOption.Run.Error");
        }

        ::std::string_view first = paramView.substr(0, index);

        paramView = paramView.substr(index + 1);

        index = paramView.find(':');

        if (index == ::std::string_view::npos) [[unlikely]] {
            throw ::std::runtime_error("Cli.ParamSweepOption.Run.Error");
        }

        ::std::string_view second = paramView.substr(0, index);

        ::std::string_view third = paramView.substr(index + 1);

        bool isFloatingPoint = false;

        if (first.find('.') != ::std::string_view::npos) isFloatingPoint = true;
        if (second.find('.') != ::std::string_view::npos) isFloatingPoint = true;
        if (third.find('.') != ::std::string_view::npos) isFloatingPoint = true;

        ::nlohmann::json config = Settings::Config::OpenJsonFile(
            args.Options->at(CliOptionType::Setting)->Value()
        );

        auto pParamOption = static_cast<ParamOption*>(args.Options->at(CliOptionType::Param));

        if (pParamOption->IsEnabled()) {
            pParamOption->OverrideParameter(config);
        }

        if (isFloatingPoint) {
            s_sweep<double>(
                first,
                second,
                third,
                paramName,
                current,
                end,
                config,
                path,
                args
            );
        }
        else {
            s_sweep<int64_t>(
                first,
                second,
                third,
                paramName,
                current,
                end,
                config,
                path,
                args
            );
        }
    }

    void ParamSweepOption::Run(
        const CliOptions*,
        CliOptionArgs args
    )
    {
        s_run(
            m_values.cbegin(),
            m_values.cend(),
            args.Options->at(CliOptionType::Output)->Value(),
            args
        );
    }
}