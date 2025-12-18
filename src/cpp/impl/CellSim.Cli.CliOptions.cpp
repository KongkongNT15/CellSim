#include "CellSim.Cli.CliOptions.hpp"
#include "CellSim.Cli.CliOptionArgs.hpp"
#include "CellSim.Cli.CliOptionActivationArgs.hpp"
#include "CellSim.Cli.BinaryOption.hpp"
#include "CellSim.Cli.CsvOption.hpp"
#include "CellSim.Cli.HelpOption.hpp"
#include "CellSim.Cli.ImageOption.hpp"
#include "CellSim.Cli.NoCleanOutputOption.hpp"
#include "CellSim.Cli.OutputOption.hpp"
#include "CellSim.Cli.ParamOption.hpp"
#include "CellSim.Cli.ParamSweepOption.hpp"
#include "CellSim.Cli.SettingOption.hpp"
#include "CellSim.Cli.VideoOption.hpp"
#include "CellSim.Messages.hpp"
#include "CellSim.Settings.Config.hpp"
#include "CellSim.Simulation.hpp"
#include "CellSim.SimulationOption.hpp"
#include "CellSim.Text.CString.hpp"

#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace CellSim::Cli
{
    void CliOptions::m_activeOption()
    {
        for (auto pair : m_options) {
            if (pair.second->IsEnabled()) {
                pair.second->OnActive(this, { &m_options });
            }
        }
    }

    void CliOptions::m_enableOption(
        int argc,
        char** argv
    )
    {
        for (int i = 1; i != argc; ++i) {
            ::std::string_view arg = argv[i];
            bool isMatch = false;
            for (auto pair : m_options) {
                if (pair.second->IsMatch(arg)) {
                    isMatch = true;
                    if (pair.second->TakesControl()) {
                        // 異なるコントローラーオプションを同時に指定するとエラー
                        if (m_controllerOption != nullptr && m_controllerOption != pair.second) [[unlikely]] {
                            throw ::std::runtime_error(
                                Text::CString::Format(
                                    Messages::Get(
                                        "Cli.CliOptions.m_enableOption.Error.TakesControlError"
                                    ).c_str(),
                                    m_controllerOption->Names().data(),
                                    pair.second->Names().data()
                                )
                            );
                        }
                        m_controllerOption = pair.second;
                    }
                    pair.second->Enable();
                    if (pair.second->HasValue()) {
                        ++i;

                        if (i == argc) [[unlikely]] {
                            ::std::string s;
                            s.push_back('\'');
                            s.append(pair.second->FullName());
                            s.push_back('\'');

                            s.append(
                                Messages::Get(
                                    "Cli.CliOptions.m_enableOption.Error.ValueError"
                                )
                            );

                            throw ::std::runtime_error(s);
                        }

                        pair.second->AddValue(argv[i]);
                    }

                    break;
                }
            }

            if (!isMatch) [[unlikely]] {
                throw ::std::runtime_error(
                    Text::CString::Format(
                        Messages::Get(
                            "Cli.CliOptions.m_enableOption.Error.MatchError"
                        ).c_str(),
                        argv[i]
                    )
                );
            }
        }

        m_activeOption();
    }

    CliOptions::CliOptions(int argc, char** argv)
        : m_options()
        , m_controllerOption(nullptr)
    {
        auto addOption = [this] (CliOption* option) {
            m_options.emplace(option->OptionType(), option);
        };

        // ここで、オプションを処理する順番を決めます

        addOption(new BinaryOption());
        addOption(new CsvOption());
        addOption(new HelpOption());
        addOption(new ImageOption());
        addOption(new NoCleanOutputOption());
        addOption(new OutputOption());
        addOption(new ParamOption());
        addOption(new ParamSweepOption());
        addOption(new SettingOption());
        addOption(new VideoOption());

        m_enableOption(argc, argv);
    }

    CliOptions::~CliOptions()
    {
        for (auto pair : m_options) {
            delete pair.second;
        }
    }

    SimulationOption CliOptions::CreateSimulationOption() const
    {
        return SimulationOption(
            m_options.at(CliOptionType::Binary)->IsEnabled(),
            m_options.at(CliOptionType::Csv)->IsEnabled(),
            m_options.at(CliOptionType::Image)->IsEnabled(),
            m_options.at(CliOptionType::Video)->IsEnabled(),
            !m_options.at(CliOptionType::NoCleanOutput)->IsEnabled(),
            m_options.at(CliOptionType::Output)->Value()
        );
    }

    void CliOptions::Run()
    {
        if (m_controllerOption != nullptr) {
            m_controllerOption->Run(this, { &m_options });
        }
        else {

            ::nlohmann::json config = Settings::Config::OpenJsonFile(
                m_options[CliOptionType::Setting]->Value()
            );

            auto pParamOption = static_cast<ParamOption*>(m_options[CliOptionType::Param]);

            if (pParamOption->IsEnabled()) {
                pParamOption->OverrideParameter(config);
            }

            Simulation sim(
                CreateSimulationOption(),
                ::std::move(config)
            );

            sim.Run();
        }
    }
}