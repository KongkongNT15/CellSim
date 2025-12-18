#include "CellSim.SimulationOption.hpp"
#include "CellSim.IO.DirectoryCreater.hpp"
#include <filesystem>

namespace CellSim
{
    SimulationOption::SimulationOption(
        bool isOutputBinary,
        bool isOutputCsv,
        bool isOutputImage,
        bool isOutputVideo,
        bool cleanOutput,
        ::std::string outputPath
    )
        : m_isOutputBinary(isOutputBinary)
        , m_isOutputCsv(isOutputCsv)
        , m_isOutputImage(isOutputImage)
        , m_isOutputVideo(isOutputVideo)
        , m_cleanOutput(cleanOutput)
        , m_outputPath(::std::move(outputPath))
        , m_outputBinaryPath()
        , m_outputBinaryCellPath()
        , m_outputBinaryMoleculePath()
        , m_outputCsvPath()
        , m_outputCsvCellPath()
        , m_outputCsvMoleculePath()
        , m_outputImagePath()
    {
        constexpr char pathSeparator = static_cast<char>(
            ::std::filesystem::path::preferred_separator
        );

        if (!m_outputPath.ends_with('\\') && !m_outputPath.ends_with('/')) {
            m_outputPath.push_back(pathSeparator);
        }

        m_outputBinaryPath = m_outputPath + s_binName;
        m_outputCsvPath = m_outputPath + s_csvName;
        m_outputImagePath = m_outputPath + s_imagesName;

        m_outputBinaryPath.push_back(pathSeparator);
        m_outputCsvPath.push_back(pathSeparator);
        m_outputImagePath.push_back(pathSeparator);

        m_outputBinaryCellPath = m_outputBinaryPath + s_cellsName;
        m_outputBinaryMoleculePath = m_outputBinaryPath + s_moleculesName;
        m_outputCsvCellPath = m_outputCsvPath + s_cellsName;
        m_outputCsvMoleculePath = m_outputCsvPath + s_moleculesName;
        m_outputImageMoleculePath = m_outputImagePath + s_moleculesName;

        m_outputBinaryCellPath.push_back(pathSeparator);
        m_outputBinaryMoleculePath.push_back(pathSeparator);
        m_outputCsvCellPath.push_back(pathSeparator);
        m_outputCsvMoleculePath.push_back(pathSeparator);
        m_outputImageMoleculePath.push_back(pathSeparator);
    }

    void SimulationOption::InitializeDirectories() const
    {
        auto f = [this] (::std::filesystem::path const& p, bool create) {
            if (m_cleanOutput && ::std::filesystem::exists(p)) {
                ::std::filesystem::remove_all(p);
            }
            if (create) {
                IO::DirectoryCreater::Create(p);
            }
        };

        f(m_outputPath, true);
        f(m_outputBinaryPath, m_isOutputBinary);
        f(m_outputBinaryCellPath, m_isOutputBinary);
        //f(m_outputBinaryMoleculePath, m_isOutputBinary);
        f(m_outputCsvPath, m_isOutputCsv);
        f(m_outputCsvCellPath, m_isOutputCsv);
        //f(m_outputCsvMoleculePath, m_isOutputCsv);
        f(m_outputImagePath, m_isOutputImage);
    }

}