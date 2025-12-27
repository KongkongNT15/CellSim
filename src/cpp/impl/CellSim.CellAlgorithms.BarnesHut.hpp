#ifndef CELLSIM_CELLALGORITHMS_BARNESHUT_HPP
#define CELLSIM_CELLALGORITHMS_BARNESHUT_HPP

#include "CellSim.CellAlgorithms.CellAlgorithm.hpp"
#include "CellSim.CellAlgorithms.Detail.Node.hpp"
#include "base.hpp"

namespace CellSim::CellAlgorithms {
    /// @brief Baenes-Hut アルゴリズム
    class BarnesHut : public CellAlgorithm
    {
        private:
        Detail::Node root = Detail::Node();

        void insertCell(const Cells::Cell* c, Detail::Node* n);
        void makeTree(const Cells::CellCollection& cells, Detail::Node& root);
        Cells::CellInfo calcGravity(Detail::Node& root);
        void selectAffectableCellsUsingBH(const Cells::Cell& c, const Detail::Node& root, ::std::vector<Cells::CellInfo>& list) const;

        // TODO: ここにメンバーを追加します

        public:
        BarnesHut();
        ~BarnesHut();

        void BeforeAdvanceStep(const Simulation* sender, CellAlgorithmStepArgs args) override;

        ::std::vector<Cells::CellInfo> GetAffectableCellInfos(const Model::CellSimulationModel* sender, CellAlgorithmAffectableCellQueryArgs args) const override;

        bool HasMultithreadingSupport() const noexcept override;

        Threading::Generator<Cells::CellInfo> IterateAffectableCellInfos(const Model::CellSimulationModel* sender, CellAlgorithmAffectableCellQueryArgs args) const override;

        void OnAdvanceStep(const Simulation* sender, CellAlgorithmStepArgs args) override;
    };
}

#endif //! CELLSIM_CELLALGORITHMS_BARNESHUT_HPP