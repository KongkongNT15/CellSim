#include "CellSim.CellAlgorithms.ClusterModel.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmAffectableCellQueryArgs.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithm.hpp"
#include "CellSim.CellAlgorithms.CellList.hpp"
#include "CellSim.Cells.Cell.hpp"
#include "CellSim.Cells.CellCollection.hpp"
#include "CellSim.Cells.CellInfo.hpp"

namespace CellSim::CellAlgorithms
{
    void ClusterModel::Combine(
        Cells::CellCollection& cells,
        ::std::vector<Molecular::MoleculeField> const& fields,
        const CellList* pCellList
    )
    {
        auto f = [] (Cells::Cell& cell, Cells::Cell& cell2) {
            if (cell2.Type() != cell.Type()) return;

            double radius = cell.Radius() + cell2.Radius();

            //距離 < 細胞半径 + 細胞半径 でくっつく
            //高速化のため、２乗で計算
            if ((cell.Position() - cell2.Position()).SquareLength() < radius * radius) {
                cell.Combine(cell2);
            }
        };

        if (pCellList == nullptr) {
            for (auto itr1 = cells.begin(), end = cells.end(); itr1 != end; ++itr1) {

                if (itr1->Type() == Cells::CellType::Invalid()) continue;

                for (auto itr2 = itr1 + 1; itr2 != end; ++itr2) {
                    f(*itr1, *itr2);
                }
            }
        }
        else {
            for (auto itr1 = cells.begin(), end = cells.end(); itr1 != end; ++itr1) {
                Cells::Cell& cell = *itr1;
                
                if (cell.Type() == Cells::CellType::Invalid()) continue;

                CELLSIM_CELLALGORITHMS_CELLALGORITHM_ITERATE_EX(
                    cellInfo,
                    pCellList,
                    nullptr,
                    &cell,
                    &cells,
                    &fields,
                    {
                        auto& cellRef = *const_cast<Cells::Cell*>(cellInfo.CellPtr);

                        if (&cellRef == &cell) {
                            continue;
                        }
                        f(cell, cellRef);
                    }
                )
            }
        }

        
    }
}