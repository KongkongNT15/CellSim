#ifndef CELLSIM_SIMULATIONRESULTWRITER_HPP
#define CELLSIM_SIMULATIONRESULTWRITER_HPP

#include "base.hpp"
#include "CellSim.Molecular.MoleculeKind.hpp"
#include "CellSim.SimulationOption.hpp"

#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <opencv2/opencv.hpp>
#include <nlohmann/json_fwd.hpp>

namespace CellSim
{
    /// @brief シミュレーション結果の出力
    class SimulationResultWriter {
        private:
        static constexpr uint32_t s_log10(
            uint64_t step
        ) noexcept;

        /// @brief 
        /// @param step 
        /// @param digits 
        /// @param parentPath 
        /// @param extension 
        /// @return 
        [[nodiscard]]
        static ::std::string s_createFilePath(
            uint64_t step,
            uint32_t digits,
            ::std::string const& parentPath,
            ::std::string_view extension
        );

        SimulationOption m_option;

        /// @brief ステップ数の桁数
        uint32_t m_digits;

        int m_imageSize;
        double m_scale;

        ::cv::VideoWriter m_videoWriter;
        ::std::map<Molecular::MoleculeKind, ::cv::VideoWriter> m_moleculeVideos;

        void m_initialize();

        void m_saveBinaryCells(
            ::std::vector<Cells::Cell> const& cells,
            uint64_t step
        ) const;

        void m_saveBinaryMolecules(
            ::std::vector<Molecular::MoleculeField> const& cells,
            uint64_t step
        ) const;

        void m_saveCsvCells(
            ::std::vector<Cells::Cell> const& cells,
            uint64_t step
        ) const;

        void m_saveCsvMolecules(
            ::std::vector<Molecular::MoleculeField> const& cells,
            uint64_t step
        ) const;

        void m_saveImage(
            ::cv::Mat const& image,
            ::std::string const& parentPath,
            uint64_t step
        ) const;

        [[nodiscard]]
        ::cv::Mat m_createImage(
            bool isTransparent,
            bool is4Channel
        ) const;

        [[nodiscard]]
        ::cv::Mat m_drawCells(
            ::std::vector<Cells::Cell> const& cells
        ) const;

        void m_drawCells(
            ::std::vector<Cells::Cell> const& cells,
            ::cv::Mat& image
        ) const;

        ::cv::Mat m_drawMolecule(
            Molecular::MoleculeField const& field
        ) const;

        public:

        SimulationResultWriter(
            SimulationOption option
        );

        SimulationResultWriter(
            SimulationResultWriter const&
        ) = delete;

        ~SimulationResultWriter();

        SimulationResultWriter& operator=(
            SimulationResultWriter const&
        ) = delete;

        /// @brief Settings::Config::Load()を読んだ後にこれを呼ぶ
        void Initialize();

        void InitializeMoleculeData(
            ::std::vector<Molecular::MoleculeField> const& fields
        );

        void Save(
            Simulation const& simulation,
            uint64_t step
        );

        void SaveResult(
            uint64_t totalStep,
            size_t initialCellCount,
            int64_t totalMilliSeconds,
            Model::CellSimulationType simulationType,
            CellAlgorithms::CellAlgorithmType algorithmType
        ) const;

        void SaveConfig(
            ::nlohmann::json const& j
        ) const;
    };
}

namespace CellSim
{
    constexpr uint32_t SimulationResultWriter::s_log10(
        uint64_t step
    ) noexcept
    {
        if (step == 0) [[unlikely]] return 1;

        uint32_t result = 0;

        while (step != 0) {
            step /= 10;
            ++result;
        }

        return result;
    }
}

#endif //!CELLSIM_SIMULATIONRESULTWRITER_HPP