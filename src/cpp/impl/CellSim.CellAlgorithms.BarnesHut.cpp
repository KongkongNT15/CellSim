#include "CellSim.CellAlgorithms.BarnesHut.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmStepArgs.hpp"
#include "CellSim.Cells.CellCollection.hpp"
#include "CellSim.Cells.CellInfo.hpp"
#include "CellSim.Settings.Config.CellAlgorithm.BarnesHut.hpp"
#include "CellSim.Threading.Generator.hpp"

#include <iostream>

namespace CellSim::CellAlgorithms {
    BarnesHut::BarnesHut()
    {
        // TODO: ここに処理を追加します
    }

    BarnesHut::~BarnesHut()
    {
        // TODO: ここに処理を追加します
    }

    /**
     * @brief 細胞cをノードnに挿入する
     * @brief 挿入された細胞はノードn下の適切な子ノードに割り振られる
     *
     * @param c
     * @param n
     */
    void BarnesHut::insertCell(const Cells::Cell* c, Detail::Node* n)
    {
        int numCells = n->numCellsInSubTree;

        if (numCells > 1) { // ノードnの子孫に複数の細胞が含まれる場合
            // 細胞cを子ノードへと渡す
            if (n->getIsLeaf()) {
                std::cerr << "here" << std::endl;
            }
            insertCell(c, n->findChildNode(*c));

        } else if (numCells == 1) { // ノードnの子孫に細胞が１つ含まれる場合
            // 子ノードを作成
            n->initChildren();
            // 持っている細胞を子ノードへ渡す
            insertCell(n->cellInNode, n->findChildNode(*n->cellInNode));
            n->cellInNode = nullptr;
            // 細胞cを子ノードへ渡す
            insertCell(c, n->findChildNode(*c));

        } else if (numCells == 0) { // ノードnの子孫に細胞が含まれない場合
            // ノードnに細胞cを登録する
            n->cellInNode = c;

        } else {
            std::cerr << "error: BarnesHut.cpp insertCell()" << std::endl;
            exit(1);
        }
        n->numCellsInSubTree++;
    }

    /**
     * @brief ツリーを構築する
     *
     * @param cells
     * @param root
     */
    void BarnesHut::makeTree(const Cells::CellCollection& cells, Detail::Node& root)
    {
        for (const Cells::Cell& pCell : cells) {
            insertCell(&pCell, &root);
        }
    }

    /**
     * @brief ノードnの重心を計算する
     *
     * @param root
     * @return CellInfo
     */
    Cells::CellInfo BarnesHut::calcGravity(Detail::Node& n)
    {
        int numCells = n.numCellsInSubTree;
        if (numCells > 1) { // ノードnの子孫に複数の細胞が含まれる場合
            double totalWeight = 0.0;
            Numerics::Vector3 totalPos(0, 0, 0);
            // 子ノードの重心を計算
            for (auto& child : n.children) {
                Cells::CellInfo childGravity = calcGravity(*child);
                totalWeight += childGravity.Mass;
                totalPos += childGravity.Position * childGravity.Mass;
            }
            // ノードnの重心を計算
            Numerics::Vector3 gravityPos = totalPos / totalWeight;
            n.gravityCell                = Cells::CellInfo(totalWeight, false, gravityPos, Numerics::Vector3::Zero(), 0.0, Cells::CellType::Invalid());

        } else if (numCells == 1) { // ノードnの子孫に細胞が１つ含まれる場合
            // ノードに登録されている細胞を重心細胞に
            n.gravityCell = Cells::CellInfo(*n.cellInNode);

        } else if (numCells == 0) { // ノードnの子孫に細胞が含まれない場合
            // 空の重心細胞を登録
            n.gravityCell = Cells::CellInfo(0.0, false, Numerics::Vector3::Zero(), Numerics::Vector3::Zero(), 0.0, Cells::CellType::Invalid());

        } else {
            std::cerr << "error: BarnesHut.cpp calcGravity()" << std::endl;
            exit(1);
        }
        return n.gravityCell;
    }

    /**
     * @brief BHアルゴリズムを使用して相互作用を及ぼす細胞を選出する
     *
     * @param c
     * @param root
     * @param list
     */
    void BarnesHut::selectAffectableCellsUsingBH(const Cells::Cell& c, const Detail::Node& n, ::std::vector<Cells::CellInfo>& list) const
    {
        int numCells = n.numCellsInSubTree;
        if (numCells > 1) { // ノードnの子孫に複数の細胞が含まれる場合
            Numerics::Vector3 diff = n.gravityCell.Position - c.Position();
            double r               = diff.Length(); // 細胞間距離
            double d               = n.getLength(); // グリッドサイズ

            if ((d / r) < Settings::Config::CellAlgorithm::BarnesHut::Theta()) {
                // 重心細胞をリストに登録
                list.emplace_back(n.gravityCell);
            } else {
                // 子ノードへ
                for (auto& child : n.children) {
                    if (child) {
                        selectAffectableCellsUsingBH(c, *child, list);
                    }
                }
            }

        } else if (numCells == 1) { // ノードnの子孫に細胞が１つ含まれる場合
            // 重心細胞をリストに登録
            list.emplace_back(n.gravityCell);

        } else if (numCells == 0) { // ノードnの子孫に細胞が含まれない場合
            // 何もしない

        } else {
            std::cerr << "error: BarnesHut.cpp selectAffectableCellsUsingBH()" << std::endl;
            exit(1);
        }
    }

    void BarnesHut::BeforeAdvanceStep(const Simulation* sender, CellAlgorithmStepArgs args)
    {
        // TODO: ここに処理を追加します
        // ツリーを初期化
        root.resetTree();
        // ツリーを構築
        makeTree(*args.Cells, root);
        // 重心を計算
        calcGravity(root);
        // root.printOctTree();
    }

    ::std::vector<Cells::CellInfo> BarnesHut::GetAffectableCellInfos([[maybe_unused]] const Model::CellSimulationModel* sender, [[maybe_unused]] CellAlgorithmAffectableCellQueryArgs args) const
    {
        //: ここに処理を追加します
        ::std::vector<Cells::CellInfo> list;
        selectAffectableCellsUsingBH(*args.Target, root, list);
        return list;
    }

    bool BarnesHut::HasMultithreadingSupport() const noexcept
    {
        // TODO: 複数スレッドによる処理ができない場合は'false'にします
        return true;
    }

    Threading::Generator<Cells::CellInfo> BarnesHut::IterateAffectableCellInfos([[maybe_unused]] const Model::CellSimulationModel* sender,
                                                                                [[maybe_unused]] CellAlgorithmAffectableCellQueryArgs args) const
    {
        //: ここに処理を追加します
        co_return;
    }

    void BarnesHut::OnAdvanceStep(const Simulation* sender, CellAlgorithmStepArgs args)
    {
    }
}