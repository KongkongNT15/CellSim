#include "CellSim.Cli.CliMain.hpp"
#include "CellSim.Cli.CliOptions.hpp"
#include "CellSim.Cells.CellType.hpp"
#include "CellSim.Messages.hpp"
#include "CellSim.Version.hpp"
#include <iostream>
#include <stdexcept>

namespace CellSim::Cli
{
    int CliMain::Run(
        int argc,
        char** argv
    ) noexcept
    {
        ::printf(
            "CellSim %d.%d.%d %s\n",
            Version::Major(),
            Version::Minor(),
            Version::Patch(),
            Version::Extension()
        );
        putchar('\n');
        
        try {
            Messages::Initialize();
            Cells::CellType::Initialize();

            CliOptions options(argc, argv);

            options.Run();
        }
        catch (::std::exception& e) {
            ::fprintf(stderr, "%s\n", e.what());
            return 1;
        }

        return 0;
    }
}