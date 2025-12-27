// DualTreeBH.cpp
#include "CellSim.CellAlgorithms.DualTreeBH.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmStepArgs.hpp"
#include "CellSim.Cells.CellCollection.hpp"
#include "CellSim.Cells.CellInfo.hpp"
#include "CellSim.Settings.Config.CellAlgorithm.DualTreeBH.hpp"
#include "CellSim.Threading.Generator.hpp"

#include <iostream>

namespace CellSim::CellAlgorithms {

    DualTreeBH::DualTreeBH()
    {
    }
    DualTreeBH::~DualTreeBH()
    {
    }

    void DualTreeBH::insertCell(const Cells::Cell* c, Detail::Node* n)
    {
        int numCells = n->numCellsInSubTree;

        if (numCells > 1) {
            insertCell(c, n->findChildNode(*c));

        } else if (numCells == 1) {
            n->initChildren();
            insertCell(n->cellInNode, n->findChildNode(*n->cellInNode));
            n->cellInNode = nullptr;
            insertCell(c, n->findChildNode(*c));

        } else if (numCells == 0) {
            n->cellInNode = c;

        } else {
            std::cerr << "error: BarnesHut.cpp insertCell()" << std::endl;
            exit(1);
        }
        n->numCellsInSubTree++;
    }

    void DualTreeBH::makeTree(const Cells::CellCollection& cells, Detail::Node& root)
    {
        for (const Cells::Cell& pCell : cells) {
            insertCell(&pCell, &root);
        }
    }

    Cells::CellInfo DualTreeBH::calcGravity(Detail::Node& n)
    {
        int numCells = n.numCellsInSubTree;
        if (numCells > 1) {
            double totalWeight = 0.0;
            Numerics::Vector3 totalPos(0, 0, 0);
            for (auto& child : n.children) {
                Cells::CellInfo childGravity = calcGravity(*child);
                totalWeight += childGravity.Mass;
                totalPos += childGravity.Position * childGravity.Mass;
            }
            Numerics::Vector3 gravityPos = totalPos / totalWeight;
            n.gravityCell                = Cells::CellInfo(totalWeight, false, gravityPos, Numerics::Vector3::Zero(), 0.0, Cells::CellType::Invalid());

        } else if (numCells == 1) {
            n.gravityCell = Cells::CellInfo(*n.cellInNode);

        } else if (numCells == 0) {
            n.gravityCell = Cells::CellInfo(0.0, false, Numerics::Vector3::Zero(), Numerics::Vector3::Zero(), 0.0, Cells::CellType::Invalid());

        } else {
            std::cerr << "error: BarnesHut.cpp calcGravity()" << std::endl;
            exit(1);
        }
        return n.gravityCell;
    }

    void DualTreeBH::calcAllAffectableCells(Detail::Node& root)
    {
        std::vector<Detail::Node*> leafNodes;
        collectLeafNodes(&root, leafNodes);

        for (Detail::Node* leaf : leafNodes) {
            std::queue<Detail::Node*> waitingNodes;
            std::queue<Detail::Node*> distantNodes;

            waitingNodes.push(&root);
            traverseTree(*leaf->cellInNode, waitingNodes, distantNodes, *leaf);

            while (!distantNodes.empty()) {
                Detail::Node* dn = distantNodes.front();
                distantNodes.pop();
                leaf->affectableCells.push_back(dn->gravityCell);
            }
        }
    }

    void DualTreeBH::collectLeafNodes(Detail::Node* node, std::vector<Detail::Node*>& leafNodes)
    {
        if (!node)
            return;
        if (node->getIsLeaf() && node->cellInNode != nullptr) {
            leafNodes.push_back(node);
        } else {
            for (auto& child : node->children) {
                collectLeafNodes(child, leafNodes);
            }
        }
    }

    void DualTreeBH::traverseTree(const Cells::Cell& c, std::queue<Detail::Node*>& waitingNodes, std::queue<Detail::Node*>& distantNodes, Detail::Node& currentNode)
    {
        std::queue<Detail::Node*> nextWaitingNodes;

        while (!waitingNodes.empty()) {
            Detail::Node* node = waitingNodes.front();
            waitingNodes.pop();

            if (!node->getIsLeaf()) {
                Numerics::Vector3 diff = node->gravityCell.Position - currentNode.gravityCell.Position;
                double r               = diff.Length();
                double d               = node->getLength();
                if ((d / r) < Settings::Config::CellAlgorithm::DualTreeBH::Theta()) {
                    distantNodes.push(node);
                } else {
                    for (auto& child : node->children) {
                        if (child)
                            nextWaitingNodes.push(child);
                    }
                }
            } else if (node->getNodeID() != currentNode.getNodeID()) {
                distantNodes.push(node);
            }
        }

        if (!nextWaitingNodes.empty()) {
            traverseSub(c, nextWaitingNodes, distantNodes, currentNode);
        }
    }

    void DualTreeBH::traverseSub(const Cells::Cell& c, std::queue<Detail::Node*>& waitingNodes, std::queue<Detail::Node*>& distantNodes, Detail::Node& currentNode)
    {
        if (!currentNode.getIsLeaf()) {
            for (auto& child : currentNode.children) {
                if (child) {
                    traverseTree(c, waitingNodes, distantNodes, *child);
                }
            }
        } else {
            traverseTree(c, waitingNodes, distantNodes, currentNode);
        }
    }

    void DualTreeBH::BeforeAdvanceStep(const Simulation* sender, CellAlgorithmStepArgs args)
    {
        root.resetTree();
        makeTree(*args.Cells, root);
        calcGravity(root);
        calcAllAffectableCells(root); // ここを追加
    }

    std::vector<Cells::CellInfo> DualTreeBH::GetAffectableCellInfos([[maybe_unused]] const Model::CellSimulationModel* sender, CellAlgorithmAffectableCellQueryArgs args) const
    {
        std::vector<Cells::CellInfo> list;
        Detail::Node* node = root.findLeafNode(*args.Target);
        if (node) {
            list = node->affectableCells;
        }
        return list;
    }

    bool DualTreeBH::HasMultithreadingSupport() const noexcept
    {
        return true;
    }

    Threading::Generator<Cells::CellInfo> DualTreeBH::IterateAffectableCellInfos([[maybe_unused]] const Model::CellSimulationModel* sender,
                                                                                 [[maybe_unused]] CellAlgorithmAffectableCellQueryArgs args) const
    {
        co_return;
    }

    void DualTreeBH::OnAdvanceStep(const Simulation* sender, CellAlgorithmStepArgs args)
    {
    }

} // namespace