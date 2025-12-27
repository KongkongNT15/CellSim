#ifndef CELLSIM_SIMULATIONOPTION_HPP
#define CELLSIM_SIMULATIONOPTION_HPP

#include "base.hpp"

#include <map>
#include <string>

namespace CellSim
{
    /// @brief コマンドラインで指定したシミュレーションで使用するオプション
    class SimulationOption {
        private:

        static constexpr const char* s_binName = "bin";
        static constexpr const char* s_cellsName = "cells";
        static constexpr const char* s_csvName = "csv";
        static constexpr const char* s_imagesName = "images";
        static constexpr const char* s_moleculesName = "molecules";

        /// @brief データをバイナリファイルとして出力するかどうか
        bool m_isOutputBinary;

        /// @brief データをバイナリファイルとして出力するかどうか
        bool m_isOutputCsv;

        /// @brief データを画像として出力するかどうか
        bool m_isOutputImage;

        /// @brief データを動画として出力するかどうか
        bool m_isOutputVideo;

        /// @brief 
        bool m_cleanOutput;

        ::std::string m_inputCsvCellPath;

        ::std::string m_outputPath;
        ::std::string m_outputBinaryPath;
        ::std::string m_outputBinaryCellPath;
        ::std::string m_outputBinaryMoleculePath;
        ::std::string m_outputCsvPath;
        ::std::string m_outputCsvCellPath;
        ::std::string m_outputCsvMoleculePath;
        ::std::string m_outputImagePath;
        ::std::string m_outputImageMoleculePath;
        
        public:

        [[nodiscard]]
        static constexpr const char* BinName() noexcept;

        [[nodiscard]]
        static constexpr const char* CellsName() noexcept;

        [[nodiscard]]
        static constexpr const char* CsvName() noexcept;

        [[nodiscard]]
        static constexpr const char* ImagesName() noexcept;

        [[nodiscard]]
        static constexpr const char* MoleculesName() noexcept;

        SimulationOption(
            bool isOutputBinary,
            bool isOutputCsv,
            bool isOutputImage,
            bool isOutputVideo,
            bool cleanOutput,
            ::std::string inputCsvCellPath,
            ::std::string outputPath
        );

        SimulationOption(
            const ::std::map<Cli::CliOptionType, Cli::CliOption*>& options
        );

        SimulationOption(
            const ::std::map<Cli::CliOptionType, Cli::CliOption*>& options,
            ::std::string outputPath
        );

        [[nodiscard]]
        constexpr bool IsInputCsvCell() const noexcept;

        /// @brief データをバイナリファイルとして出力するかどうか
        [[nodiscard]] constexpr bool IsOutputBinary() const noexcept;

        /// @brief データをcsvファイルとして出力するかどうか
        [[nodiscard]] constexpr bool IsOutputCsv() const noexcept;

        /// @brief データを画像として出力するかどうか
        [[nodiscard]] constexpr bool IsOutputImage() const noexcept;

        /// @brief データを動画として出力するかどうか
        [[nodiscard]] constexpr bool IsOutputVideo() const noexcept;

        [[nodiscard]] constexpr ::std::string const& InputCsvCellPath() const noexcept;

        [[nodiscard]] constexpr ::std::string const& OutputPath() const noexcept;
        [[nodiscard]] constexpr ::std::string const& OutputBinaryPath() const noexcept;
        [[nodiscard]] constexpr ::std::string const& OutputBinaryCellPath() const noexcept;
        [[nodiscard]] constexpr ::std::string const& OutputBinaryMoleculePath() const noexcept;
        [[nodiscard]] constexpr ::std::string const& OutputCsvPath() const noexcept;
        [[nodiscard]] constexpr ::std::string const& OutputCsvCellPath() const noexcept;
        [[nodiscard]] constexpr ::std::string const& OutputCsvMoleculePath() const noexcept;
        [[nodiscard]] constexpr ::std::string const& OutputImagePath() const noexcept;
        [[nodiscard]] constexpr ::std::string const& OutputImageMoleculePath() const noexcept;
        

        void InitializeDirectories() const;
    };
}

namespace CellSim
{
    constexpr const char* SimulationOption::BinName() noexcept
    {
        return s_binName;
    }

    constexpr const char* SimulationOption::CellsName() noexcept
    {
        return s_cellsName;
    }

    constexpr const char* SimulationOption::CsvName() noexcept
    {
        return s_csvName;
    }

    constexpr const char* SimulationOption::ImagesName() noexcept
    {
        return s_imagesName;
    }

    constexpr const char* SimulationOption::MoleculesName() noexcept
    {
        return s_moleculesName;
    }

    constexpr bool SimulationOption::IsInputCsvCell() const noexcept
    {
        return !m_inputCsvCellPath.empty();
    }

    constexpr bool SimulationOption::IsOutputBinary() const noexcept
    {
        return m_isOutputBinary;
    }

    constexpr bool SimulationOption::IsOutputCsv() const noexcept
    {
        return m_isOutputCsv;
    }

    constexpr bool SimulationOption::IsOutputImage() const noexcept
    {
        return m_isOutputImage;
    }

    constexpr bool SimulationOption::IsOutputVideo() const noexcept
    {
        return m_isOutputVideo;
    }

    constexpr ::std::string const&
    SimulationOption::InputCsvCellPath() const noexcept
    {
        return m_inputCsvCellPath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputPath() const noexcept
    {
        return m_outputPath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputBinaryPath() const noexcept
    {
        return m_outputBinaryPath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputBinaryCellPath() const noexcept
    {
        return m_outputBinaryCellPath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputBinaryMoleculePath() const noexcept
    {
        return m_outputBinaryMoleculePath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputCsvPath() const noexcept
    {
        return m_outputCsvPath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputCsvCellPath() const noexcept
    {
        return m_outputCsvCellPath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputCsvMoleculePath() const noexcept
    {
        return m_outputCsvMoleculePath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputImagePath() const noexcept
    {
        return m_outputImagePath;
    }

    constexpr ::std::string const&
    SimulationOption::OutputImageMoleculePath() const noexcept
    {
        return m_outputImageMoleculePath;
    }
}

#endif //!CELLSIM_SIMULATIONOPTION_HPP