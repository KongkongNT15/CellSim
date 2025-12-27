#ifndef CELLSIM_CELLALGORITHMS_DUALTREEBH_HPP
#define CELLSIM_CELLALGORITHMS_DUALTREEBH_HPP

#include "base.hpp"

#include "CellSim.CellAlgorithms.CellAlgorithm.hpp"
#include "CellSim.CellAlgorithms.Detail.Node.hpp"
#include <queue>

namespace CellSim::CellAlgorithms
{
    class DualTreeBH : public CellAlgorithm {
        private:
        Detail::Node root = Detail::Node();

        void insertCell(const Cells::Cell* c, Detail::Node* n);
        void makeTree(const Cells::CellCollection& cells, Detail::Node& root);
        Cells::CellInfo calcGravity(Detail::Node& root);
        void calcAllAffectableCells(Detail::Node& root);
        void collectLeafNodes(Detail::Node* node, std::vector<Detail::Node*>& leafNodes);
        void traverseTree(const Cells::Cell& c, ::std::queue<Detail::Node*>& waitingNodes, ::std::queue<Detail::Node*>& distantNodes, Detail::Node& currentNode);
        void traverseSub(const Cells::Cell& c, ::std::queue<Detail::Node*>& waitingNodes, ::std::queue<Detail::Node*>& distantNodes, Detail::Node& currentNode);
        void selectAffectableCellsUsingDTBH(const Cells::Cell& c, const Detail::Node& root, ::std::vector<Cells::CellInfo>& list) const;

        public:
        DualTreeBH();
        ~DualTreeBH();

        void BeforeAdvanceStep(const Simulation* sender, CellAlgorithmStepArgs args) override;

        ::std::vector<Cells::CellInfo> GetAffectableCellInfos(const Model::CellSimulationModel* sender, CellAlgorithmAffectableCellQueryArgs args) const override;

        bool HasMultithreadingSupport() const noexcept override;

        Threading::Generator<Cells::CellInfo> IterateAffectableCellInfos(const Model::CellSimulationModel* sender, CellAlgorithmAffectableCellQueryArgs args) const override;

        void OnAdvanceStep(const Simulation* sender, CellAlgorithmStepArgs args) override;
    };
}

#endif