#include "CellSim.Cells.CellCollection.hpp"

namespace CellSim::Cells
{
    bool CellCollection::Detach(
        Cell& cell1,
        Cell& cell2
    ) noexcept
    {
        auto itr = cell1.m_attachedCells.begin();
        auto end = cell1.m_attachedCells.end();

        while (itr != end) {

            if (*itr == &cell2) {

                cell1.m_attachedCells.erase(itr);

                auto itr2 = cell2.m_attachedCells.begin();
                auto end2 = cell2.m_attachedCells.end();

                while (itr2 != end2) {
                    if (*itr2 == &cell1) {
                        cell2.m_attachedCells.erase(itr2);
                        break;
                    }

                    ++itr2;
                }

                return true;
            }

            ++itr;
        }

        return false;
    }

    void CellCollection::RemoveWithDetachUnsafe(
        ::std::vector<Cell>::iterator itr
    ) noexcept
    {
        ::puts("void CellCollection::RemoveWithDetachUnsafe()は実装されていません");

        ::exit(1);
    }
}