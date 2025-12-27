# エラーの原因
cmake_minimum_required(VERSION 3.11)

# プロジェクト名
project(MulticellularMolecularCommunicationSystemSimulator)

# C++20を指定
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ここにソースファイルを追加
set(SOURCES
    src/cpp/main.cpp
    src/cpp/CellSim.Users.UserCellAlgorithm.cpp
    src/cpp/CellSim.Users.UserCellBehavior.cpp
    src/cpp/CellSim.Users.UserMoleculeBehavior.cpp
    src/cpp/CellSim.Settings.Config.CellAlgorithm.User.cpp
    src/cpp/CellSim.Settings.Config.CellBehavior.User.cpp
    src/cpp/CellSim.Settings.Config.SimulationModel.User.cpp
    src/cpp/CellSim.Settings.Config.UserSettings.cpp
    src/cpp/CellSim.Users.UserSimulationModel.cpp
    src/cpp/impl/CellSim.Messages.cpp
    src/cpp/impl/CellSim.Simulation.cpp
    src/cpp/impl/CellSim.SimulationOption.cpp
    src/cpp/impl/CellSim.SimulationResultWriter.cpp
    src/cpp/impl/CellSim.CellAlgorithms.BarnesHut.cpp
    src/cpp/impl/CellSim.CellAlgorithms.CellAlgorithm.cpp
    src/cpp/impl/CellSim.CellAlgorithms.CellList.cpp
    src/cpp/impl/CellSim.CellAlgorithms.ClusterModel.cpp
    src/cpp/impl/CellSim.CellAlgorithms.Detail.Node.cpp
    src/cpp/impl/CellSim.CellAlgorithms.NaiveAlgorithm.cpp
    src/cpp/impl/CellSim.CellAlgorithms.ParticleMesh.cpp
    src/cpp/impl/CellSim.Cells.Cell.cpp
    src/cpp/impl/CellSim.Cells.CellBehaviorPtr.cpp
    src/cpp/impl/CellSim.Cells.CellCollection.cpp
    src/cpp/impl/CellSim.Cells.CellType.cpp
    src/cpp/impl/CellSim.Cells.MoleculeAwareCellBehavior.cpp
    src/cpp/impl/CellSim.Cells.NormalCellBehavior.cpp
    src/cpp/impl/CellSim.Cells.WavePropagationCellBehavior.cpp
    src/cpp/impl/CellSim.Cli.BinaryOption.cpp
    src/cpp/impl/CellSim.Cli.CliMain.cpp
    src/cpp/impl/CellSim.Cli.CliOption.cpp
    src/cpp/impl/CellSim.Cli.CliOptions.cpp
    src/cpp/impl/CellSim.Cli.CsvOption.cpp
    src/cpp/impl/CellSim.Cli.FlagOption.cpp
    src/cpp/impl/CellSim.Cli.HelpOption.cpp
    src/cpp/impl/CellSim.Cli.ImageOption.cpp
    src/cpp/impl/CellSim.Cli.LoadCellCsvOption.cpp
    src/cpp/impl/CellSim.Cli.OutputOption.cpp
    src/cpp/impl/CellSim.Cli.ParamOption.cpp
    src/cpp/impl/CellSim.Cli.ParamSweepOption.cpp
    src/cpp/impl/CellSim.Cli.RepeatedValueOption.cpp
    src/cpp/impl/CellSim.Cli.SettingOption.cpp
    src/cpp/impl/CellSim.Cli.ValueOption.cpp
    src/cpp/impl/CellSim.Cli.VideoOption.cpp
    src/cpp/impl/CellSim.Events.Event.cpp
    src/cpp/impl/CellSim.Events.RemoveCellEvent.cpp
    src/cpp/impl/CellSim.Graphics.Color.cpp
    src/cpp/impl/CellSim.Imaging.ImageHelper.cpp
    src/cpp/impl/CellSim.IO.DirectoryCreater.cpp
    src/cpp/impl/CellSim.IO.NamedPipe.cpp
    src/cpp/impl/CellSim.Model.CellGrowthModel.cpp
    src/cpp/impl/CellSim.Model.CellSimulationModel.cpp
    src/cpp/impl/CellSim.Model.ClusterFormationModel.cpp
    src/cpp/impl/CellSim.Model.ClusterRotationModel.cpp
    src/cpp/impl/CellSim.Model.ClusterSproutingModel.cpp
    src/cpp/impl/CellSim.Model.NetworkFormationModel.cpp
    src/cpp/impl/CellSim.Molecular.Molecule.cpp
    src/cpp/impl/CellSim.Molecular.MoleculeBehavior.cpp
    src/cpp/impl/CellSim.Molecular.MoleculeField.cpp
    src/cpp/impl/CellSim.Molecular.MoleculeKind.cpp
    src/cpp/impl/CellSim.Molecular.NormalMoleculeBehavior.cpp
    src/cpp/impl/CellSim.Numerics.Numbers.cpp
    src/cpp/impl/CellSim.Settings.Config.cpp
    src/cpp/impl/CellSim.Settings.Config.Cell.cpp
    src/cpp/impl/CellSim.Settings.Config.CellAlgorithm.cpp
    src/cpp/impl/CellSim.Settings.Config.CellAlgorithm.BarnesHut.cpp
    src/cpp/impl/CellSim.Settings.Config.CellAlgorithm.CellList.cpp
    src/cpp/impl/CellSim.Settings.Config.CellAlgorithm.ParticleMesh.cpp
    src/cpp/impl/CellSim.Settings.Config.CellBehavior.cpp
    src/cpp/impl/CellSim.Settings.Config.CellBehavior.MoleculeAware.cpp
    src/cpp/impl/CellSim.Settings.Config.CellBehavior.Normal.cpp
    src/cpp/impl/CellSim.Settings.Config.CellBehavior.WavePropagation.cpp
    src/cpp/impl/CellSim.Settings.Config.Molecular.cpp
    src/cpp/impl/CellSim.Settings.Config.Molecular.Behavior.cpp
    src/cpp/impl/CellSim.Settings.Config.Molecular.Behavior.Normal.cpp
    src/cpp/impl/CellSim.Settings.Config.Optimization.cpp
    src/cpp/impl/CellSim.Settings.Config.Simulation.cpp
    src/cpp/impl/CellSim.Settings.Config.SimulationModel.cpp
    src/cpp/impl/CellSim.Settings.Config.SimulationModel.CellGrowth.cpp
    src/cpp/impl/CellSim.Settings.Config.SimulationModel.ClusterFormation.cpp
    src/cpp/impl/CellSim.Settings.Config.SimulationModel.ClusterRotation.cpp
    src/cpp/impl/CellSim.Settings.Config.SimulationModel.ClusterSprouting.cpp
    src/cpp/impl/CellSim.Settings.Config.SimulationModel.NetworkFormation.cpp
    src/cpp/impl/CellSim.Text.JsonHelper.cpp
    src/cpp/impl/CellSim.Threading.StlThreadPool.cpp
    src/cpp/impl/CellSim.Threading.ThreadPool.cpp
)

# ターゲット名は'cellsim'
add_executable(cellsim ${SOURCES})

# 追加のオプション
target_compile_options(cellsim PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8> # MSVCはデフォルトでANSIになるのでUTF-8に変更
    #$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2>
    #$<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O2>
)

# ビルド後に必要なファイルをコピー
add_custom_command(TARGET cellsim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/resources/cellsim/config.json
            $<TARGET_FILE_DIR:cellsim>/config.json
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/resources/cellsim/schema.json
            $<TARGET_FILE_DIR:cellsim>/schema.json
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/resources/cellsim/languages
            $<TARGET_FILE_DIR:cellsim>/languages
)

# OpenCVの取得に使う
include(FetchContent)

# OpenCVのURLからデータを取得
FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv.git
    GIT_TAG        "4.11.0"
)

# 明示的にビルドオプションを設定（不要ならコメントアウトしてもOK）
# Chat-GPTのコピペだよ
set(BUILD_LIST core,imgproc,highgui,imgcodecs,videoio CACHE STRING "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_PERF_TESTS OFF CACHE BOOL "" FORCE)
set(WITH_FFMPEG ON CACHE BOOL "" FORCE)
set(WITH_PNG ON)
set(WITH_JPEG ON)
set(WITH_TIFF ON)

# よくわからんやつ
FetchContent_MakeAvailable(opencv)

# OpenCVライブラリとリンク
# Chat-GPTのコピペだよ
target_link_libraries(cellsim PRIVATE opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_videoio)

# 追加のincludeパス
target_include_directories(cellsim PRIVATE
    ${CMAKE_SOURCE_DIR}/src/cpp/thirdparty
    ${CMAKE_SOURCE_DIR}/build-scripts/build/_deps/opencv-src/include
    ${CMAKE_SOURCE_DIR}/build-scripts/build/_deps/opencv-src/modules/core/include
    ${CMAKE_SOURCE_DIR}/build-scripts/build/_deps/opencv-src/modules/highgui/include
    ${CMAKE_SOURCE_DIR}/build-scripts/build/_deps/opencv-src/modules/imgcodecs/include
    ${CMAKE_SOURCE_DIR}/build-scripts/build/_deps/opencv-src/modules/imgproc/include
    ${CMAKE_SOURCE_DIR}/build-scripts/build/_deps/opencv-src/modules/videoio/include
    ${CMAKE_SOURCE_DIR}/build-scripts/build
)