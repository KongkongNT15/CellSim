#include "CellSim.SimulationResultWriter.hpp"
#include "CellSim.Simulation.hpp"
#include "CellSim.CellAlgorithms.CellAlgorithmType.hpp"
#include "CellSim.Cells.Cell.hpp"
#include "CellSim.Cells.CellInfo.hpp"
#include "CellSim.Imaging.ImageHelper.hpp"
#include "CellSim.IO.DirectoryCreater.hpp"
#include "CellSim.Model.CellSimulationType.hpp"
#include "CellSim.Settings.Config.CellAlgorithm.hpp"
#include "CellSim.Settings.Config.Simulation.hpp"
#include "CellSim.Version.hpp"

#include <fstream>
#include <optional>
#include <stdio.h>
#include <stdexcept>

namespace CellSim
{
    ::std::string SimulationResultWriter::s_createFilePath(
        uint64_t step,
        uint32_t digits,
        ::std::string const& parentPath,
        ::std::string_view extension
    )
    {
        size_t filePathLength = parentPath.size() + digits + extension.size();
        size_t filePathCapacity = filePathLength + 1;

        // "%s%020llu%4s"
        char optionStr[13];
        
        ::snprintf(
            optionStr,
            13,
            "%%s%%0%lullu%%%zus",
            static_cast<unsigned long>(digits),
            extension.size()
        );
        
        ::std::string filePath(filePathLength, '\0');

        ::snprintf(
            filePath.data(),
            filePathCapacity,
            optionStr,
            parentPath.c_str(),
            step, extension.data()
        );

        return filePath;
    }

    void SimulationResultWriter::m_initialize()
    {
        m_option.InitializeDirectories();

        if (m_option.IsOutputVideo()) {
            int fourcc = ::cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

            bool v = m_videoWriter.open(
                m_option.OutputPath() + "out.avi",
                fourcc,
                20,
                cv::Size(
                    m_imageSize,
                    m_imageSize
                ),
                true
            );
        }
    }

    void SimulationResultWriter::m_saveBinaryCells(
        ::std::vector<Cells::Cell> const& cells,
        uint64_t step
    ) const
    {
        ::FILE* fp = ::fopen(
            s_createFilePath(
                step,
                m_digits,
                m_option.OutputBinaryCellPath(),
                ".bin"
            ).c_str(),
            "wb"
        );

        if (fp == nullptr) [[unlikely]] throw ::std::runtime_error("Failed to create .bin file");

        for (Cells::Cell const& cell : cells) {
            uint32_t id = cell.Id();
            ::fwrite(&id, sizeof(id), 1, fp);

            Numerics::Vector3 position = cell.Position();
            ::fwrite(&position, sizeof(position), 1, fp);

            Numerics::Vector3 velocity = cell.Velocity();
            ::fwrite(&velocity, sizeof(position), 1, fp);

            double radius = cell.Radius();
            ::fwrite(&radius, sizeof(radius), 1, fp);

            double mass = cell.Mass();
            ::fwrite(&mass, sizeof(mass), 1, fp);

            bool isAlive = cell.IsAlive();
            ::fwrite(&isAlive, sizeof(isAlive), 1, fp);

            uint64_t attachedCellCount = cell.AttachedCellCount();
            ::fwrite(&attachedCellCount, sizeof(attachedCellCount), 1, fp);

            for (const Cells::Cell* attachedCell : cell.AttachedCells()) {
                id = attachedCell->Id();
                ::fwrite(&id, sizeof(id), 1, fp);
            }
        }
        
        ::fclose(fp);
    }

    void SimulationResultWriter::m_saveBinaryMolecules(
        ::std::vector<Molecular::MoleculeField> const& fields,
        uint64_t step
    ) const
    {
        for (Molecular::MoleculeField const& field : fields) {
            ::FILE* fp = ::fopen(
                s_createFilePath(
                    step,
                    m_digits,
                    m_option.OutputBinaryMoleculePath() + field.Kind().Name() + '/',
                    ".bin"
                ).c_str(),
                "wb"
            );

            if (fp == nullptr) [[unlikely]] throw ::std::runtime_error("Failed to create .bin file");

            size_t gridCounts[]{ 
                field.GridCountX(),
                field.GridCountY(),
                field.GridCountZ()
            };

            ::fwrite(gridCounts, sizeof(gridCounts), 1, fp);

            auto data = field.Concentrations();

            ::fwrite(data.begin(), data.ElementSize(), data.Length(), fp);

            ::fclose(fp);
        }
    }

    void SimulationResultWriter::m_saveCsvMolecules(
        ::std::vector<Molecular::MoleculeField> const& fields,
        uint64_t step
    ) const
    {
        for (Molecular::MoleculeField const& field : fields) {
            ::std::ofstream ofs(
                s_createFilePath(
                    step,
                    m_digits,
                    m_option.OutputCsvMoleculePath() + field.Kind().Name() + '/',
                    ".csv"
                )
            );

            if (!ofs) [[unlikely]] throw ::std::runtime_error("Failed to create .csv file");

            ofs << "Nx," << field.GridCountX() << ::std::endl;
            ofs << "Ny," << field.GridCountY() << ::std::endl;
            ofs << "Nz," << field.GridCountZ() << ::std::endl;

            ofs << "x,y,z,amount" << ::std::endl;

            auto data = field.Concentrations();

            if (field.Enable2dMode()) {
                for (size_t x = 0; x < field.GridCountX(); x++) {
                    auto span2 = data[x];

                    for (size_t y = 0; y < field.GridCountY(); y++) {
                        ofs << x << ',' << y << ",0," << span2.At(y, 0) << ::std::endl;
                    }
                }
            }
            else {
                for (size_t x = 0; x < field.GridCountX(); x++) {
                    auto span2 = data[x];

                    for (size_t y = 0; y < field.GridCountY(); y++) {
                        auto span = span2[y];

                        for (size_t z = 0; z < field.GridCountZ(); z++) {
                            ofs << x << ',' << y << ',' << z << ',' << span[z] << ::std::endl; 
                        }
                    }
                }
            }

            
        }
    }

    void SimulationResultWriter::m_saveCsvCells(
        ::std::vector<Cells::Cell> const& cells,
        uint64_t step
    ) const
    {
        ::std::ofstream ofs(
            s_createFilePath(
                step,
                m_digits,
                m_option.OutputCsvCellPath(),
                ".csv"
            )
        );
        
        if (!ofs) [[unlikely]] throw ::std::runtime_error("Failed to create .csv file");

        ofs << "ID,Type,Position.X,Position.Y,Position.Z,Velocity.X,Velocity.Y,Velocity.Z,Radius,Mass,IsAlive,AttachedCellCount," << ::std::endl;

        for (Cells::Cell const& cell : cells) {
            Numerics::Vector3 velocity = cell.Velocity();
            ofs << cell.Id() << ',';
            ofs << cell.Type().Name() << ',';
            ofs << cell.PositionX() << ',';
            ofs << cell.PositionY() << ',';
            ofs << cell.PositionZ() << ',';
            ofs << velocity.X << ',';
            ofs << velocity.Y << ',';
            ofs << velocity.Z << ',';
            ofs << cell.Radius() << ',';
            ofs << cell.Mass() << ',';
            ofs << (cell.IsAlive() ? '1' : '0') << ',';
            ofs << cell.AttachedCellCount() << ',';

            for (const Cells::Cell* attachedCell : cell.AttachedCells()) {
                ofs << attachedCell->Id() << ',';
            }

            ofs << ::std::endl;
        }
        
    }

    ::cv::Mat SimulationResultWriter::m_createImage(
        bool isTransparent,
        bool is4Channel
    ) const
    {
        return ::cv::Mat{
            m_imageSize,
            m_imageSize,
            is4Channel ? CV_8UC4 : CV_8UC3,
            isTransparent ? ::cv::Scalar(0, 0, 0, 0) : ::cv::Scalar(0, 0, 0, 255)
        };
    }

    ::cv::Mat SimulationResultWriter::m_drawCells(
        ::std::vector<Cells::Cell> const& cells
    ) const
    {
        ::cv::Mat image = m_createImage(false, false);
        m_drawCells(cells, image);

        return image;
    }

    void SimulationResultWriter::m_drawCells(
        ::std::vector<Cells::Cell> const& cells,
        ::cv::Mat& image
    ) const
    {
        int radius = m_imageSize / 2;

        ::cv::circle(
            image,
            ::cv::Point{ radius, radius },
            radius,
            ::cv::Scalar(255, 255, 255, 255),
            1
        );

        ::cv::line(
            image,
            ::cv::Point{ radius, 0 },
            ::cv::Point{ radius, m_imageSize },
            ::cv::Scalar(255, 255, 255, 255),
            1
        );

        ::cv::line(
            image,
            ::cv::Point{ 0, radius },
            ::cv::Point{ m_imageSize, radius },
            ::cv::Scalar(255, 255, 255, 255),
            1
        );

        for (Cells::Cell const& cell : cells) {
            int pointX = static_cast<int>((cell.PositionX() + Settings::Config::Simulation::FieldRadiusX()) * m_scale);
            int pointY = static_cast<int>((cell.PositionY() + Settings::Config::Simulation::FieldRadiusY()) * m_scale);

            Graphics::Color color = cell.Type().Color();

            ::cv::circle(
                image,
                ::cv::Point{ pointX, pointY },
                static_cast<int>(cell.Radius() * m_scale),
                ::cv::Scalar(color.B, color.G, color.R, 255),
                1
            );

            uint32_t id = cell.Id();

            for (const Cells::Cell* pCell : cell.AttachedCells()) {
                if (pCell->Id() < id) continue;

                ::cv::line(
                    image,
                    ::cv::Point{ pointX, pointY },
                    ::cv::Point{
                        static_cast<int>((pCell->PositionX() + Settings::Config::Simulation::FieldRadiusX()) * m_scale),
                        static_cast<int>((pCell->PositionY() + Settings::Config::Simulation::FieldRadiusY()) * m_scale)
                    },
                    ::cv::Scalar(0, 255, 255, 255),
                    1
                );
            }
        }
    }

    ::cv::Mat SimulationResultWriter::m_drawMolecule(
        Molecular::MoleculeField const& field
    ) const
    {
        ::cv::Mat image{
            static_cast<int>(field.GridCountX() - 2),
            static_cast<int>(field.GridCountY() - 2),
            CV_8UC3,
            ::cv::Scalar(0, 0, 0)
        };
        ::cv::Scalar color(0, 0, 0);
        double threshold = field.Kind().Threshold();

        size_t z = field.Enable2dMode() ? 0 : field.GridCountZ() / 2;
        // 境界を除く
        size_t gridCount = field.GridCountX() - 2;
        int rectLength = static_cast<int>(m_imageSize / gridCount);

        auto concentrations = field.Concentrations();
        
        for (size_t x = 1; x < field.GridCountX(); ++x) {
            auto span2 = concentrations[x];
            for (size_t y = 1; y < field.GridCountY(); ++y) {
                double value = span2.At(y, z);

                // 透明度設定
                if (value < threshold) {
                    int v = static_cast<int>((value / threshold) * 1532);
                    
                    // 赤~白
                    if (v >= 1276) {
                        color[2] = 255;
                        color[1] = v - 1276;
                        color[0] = v - 1276;
                    }
                    // 黄色~赤
                    else if (v >= 1021) {
                        color[2] = 255;
                        color[1] = 1276 - v;
                        color[0] = 0;
                    }
                    // 緑~黄色
                    else if (v >= 766) {
                        color[2] = v - 766;
                        color[1] = 255;
                        color[0] = 0;
                    }
                    // 青みどり~緑
                    else if (v >= 511) {
                        color[2] = 0;
                        color[1] = 255;
                        color[0] = 766 - v;
                    }
                    // 青~青緑
                    else if (v >= 256) {
                        color[2] = 0;
                        color[1] = v - 255;
                        color[0] = 255;
                    }
                    // 黒~青
                    else {
                        color[2] = 0;
                        color[1] = 0;
                        color[0] = v;
                    }
                }
                else {
                    color[2] = 255;
                    color[1] = 255;
                    color[0] = 255;
                }

                ::cv::rectangle(
                    image,
                    ::cv::Rect{
                        static_cast<int>(x - 1),
                        static_cast<int>(y - 1),
                        1,
                        1
                    },
                    color, 
                    ::cv::LineTypes::FILLED // 塗りつぶし
                );
            }
        }

        ::cv::Mat resized;

        ::cv::resize(
            image,
            resized,
            ::cv::Size{ m_imageSize, m_imageSize },
            0,
            0,
            (field.GridCountX() - 2) > m_imageSize ? cv::InterpolationFlags::INTER_AREA : cv::InterpolationFlags::INTER_NEAREST
        );

        return resized;
    }

    void SimulationResultWriter::m_saveImage(
        ::cv::Mat const& image,
        ::std::string const& parentPath,
        uint64_t step
    ) const
    {
        ::cv::imwrite(
            s_createFilePath(
                step,
                m_digits,
                parentPath,
                ".png"
            ),
            image
        );
    }

    SimulationResultWriter::SimulationResultWriter(
        SimulationOption option
    )
        : m_option(::std::move(option))
        , m_digits(0)
        , m_imageSize(0)
        , m_scale(0)
        , m_videoWriter()
    {
    }

    SimulationResultWriter::~SimulationResultWriter()
    {
        auto f = [](::cv::VideoWriter& writer) {
            if (writer.isOpened()) writer.release();
        };
        
        f(m_videoWriter);

        for (auto& pair : m_moleculeVideos) {
            f(pair.second);
        }
    }

    void SimulationResultWriter::Initialize()
    {
        m_digits = s_log10(Settings::Config::Simulation::TotalSteps());
        m_imageSize = Settings::Config::Simulation::ImageSize();
        m_scale = Settings::Config::Simulation::ImageSize() / 2 / Settings::Config::Simulation::FieldRadius();

        m_initialize();
    }

    void SimulationResultWriter::InitializeMoleculeData(
        ::std::vector<Molecular::MoleculeField> const& fields
    )
    {
        if (fields.empty()) return;

        if (m_option.IsOutputBinary()) IO::DirectoryCreater::Create(m_option.OutputBinaryMoleculePath());
        if (m_option.IsOutputCsv()) IO::DirectoryCreater::Create(m_option.OutputCsvMoleculePath());
        if (m_option.IsOutputImage()) IO::DirectoryCreater::Create(m_option.OutputImageMoleculePath());

        for (const Molecular::MoleculeField& field : fields) {
            auto& videoWriter = m_moleculeVideos[field.Kind()];

            if (m_option.IsOutputBinary()) IO::DirectoryCreater::Create(m_option.OutputBinaryMoleculePath() + field.Kind().Name());
            if (m_option.IsOutputCsv()) IO::DirectoryCreater::Create(m_option.OutputCsvMoleculePath() + field.Kind().Name());
            if (m_option.IsOutputImage()) IO::DirectoryCreater::Create(m_option.OutputImageMoleculePath() + field.Kind().Name());
            
            if (m_option.IsOutputVideo()) {
                int fourcc = ::cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

                bool v = videoWriter.open(
                    m_option.OutputPath() + "out-" + field.Kind().Name() + ".avi",
                    fourcc,
                    20,
                    cv::Size(
                        m_imageSize,
                        m_imageSize
                    ),
                    true
                );
            }
        }
    }

    void SimulationResultWriter::Save(
        Simulation const& simulation,
        uint64_t step
    )
    {
        if (m_option.IsOutputBinary()) {
            m_saveBinaryCells(simulation.Cells(), step);
            m_saveBinaryMolecules(simulation.Molecules(), step);
        }

        if (m_option.IsOutputCsv()) {
            m_saveCsvCells(simulation.Cells(), step);
            m_saveCsvMolecules(simulation.Molecules(), step);
        }

        if (!m_option.IsOutputImage() && !m_option.IsOutputVideo()) return;

        if (simulation.Molecules().empty()) {
            ::cv::Mat image = m_drawCells(simulation.Cells());
            if (m_option.IsOutputImage()) m_saveImage(image, m_option.OutputImagePath(), step);
        
            if (m_option.IsOutputVideo()) {
                m_videoWriter.write(image);
            }
        }
        else {
            ::cv::Mat image = m_createImage(false, false);
            ::cv::Mat cellImage = m_createImage(true, true);
            m_drawCells(simulation.Cells(), cellImage);

            for (const Molecular::MoleculeField& field : simulation.Molecules()) {
                ::cv::Mat moleculeImage = m_drawMolecule(field);

                // 画像を合成
                ::cv::Mat combinedMolecular = Imaging::ImageHelper::CombineImages(moleculeImage, cellImage);

                if (m_option.IsOutputImage()) {
                    if (m_option.IsOutputImage()) {
                        m_saveImage(
                            combinedMolecular,
                            m_option.OutputImageMoleculePath() + field.Kind().Name() + '/',
                            step
                        );
                    }
                }

                if (m_option.IsOutputVideo()) {
                    m_moleculeVideos[field.Kind()].write(combinedMolecular);
                }
            }

            ::cv::Mat combined = Imaging::ImageHelper::CombineImages(image, cellImage);
            if (m_option.IsOutputImage()) {
                m_saveImage(
                    combined,
                    m_option.OutputImagePath(),
                    step
                );
            }

            if (m_option.IsOutputVideo()) {
                m_videoWriter.write(combined);
            }
        }

    }

    void SimulationResultWriter::SaveResult(
        uint64_t totalStep,
        size_t initialCellCount,
        int64_t totalMilliSeconds,
        Model::CellSimulationType simulationType,
        CellAlgorithms::CellAlgorithmType algorithmType
    ) const
    {
        ::std::ofstream ofs(m_option.OutputPath() + "result.txt");

        if (!ofs) [[unlikely]] throw ::std::runtime_error("Failed to create result.txt file");

        ofs << "Version                 : "
            << Version::Major()
            << '.'
            << Version::Minor()
            << '.'
            << Version::Patch()
            << '.'
            << Version::Build()
            << ::std::endl;
        
        ofs << "Initial cell count      : "
            << initialCellCount
            << ::std::endl;
        
        ofs << "Total processing time   : "
            << totalMilliSeconds
            << " milliseconds"
            << ::std::endl;

        ofs << "Average processing time : "
            << (static_cast<double>(totalMilliSeconds) / totalStep)
            << " milliseconds"
            << ::std::endl;

        ofs << "Simulation model        : ";

        switch (simulationType) {
            case Model::CellSimulationType::CellGrowth:       ofs << "CellGrowth";       break;
            case Model::CellSimulationType::ClusterFormation: ofs << "ClusterFormation"; break;
            case Model::CellSimulationType::ClusterRotation:  ofs << "ClusterRotation";  break;
            case Model::CellSimulationType::ClusterSprouting: ofs << "ClusterSprouting"; break;
            case Model::CellSimulationType::NetworkFormation: ofs << "NetworkFormation"; break;
            case Model::CellSimulationType::Null:             ofs << "Null";             break;
            case Model::CellSimulationType::User:             ofs << "User";             break;
        }

        ofs << ::std::endl;

        ofs << "Algorithm               : ";

        switch (algorithmType) {
            case CellAlgorithms::CellAlgorithmType::BarnesHut:    ofs << "BarnesHut";    break;
            case CellAlgorithms::CellAlgorithmType::CellList:     ofs << "CellList";     break;
            case CellAlgorithms::CellAlgorithmType::Naive:        ofs << "Naive";        break;
            case CellAlgorithms::CellAlgorithmType::Null:         ofs << "Null";         break;
            case CellAlgorithms::CellAlgorithmType::ParticleMesh: ofs << "ParticleMesh"; break;
            case CellAlgorithms::CellAlgorithmType::User:         ofs << "User";         break;
        }

        if (Settings::Config::CellAlgorithm::UseClusterModel()) ofs << "+Cluster";
        
        ofs << ::std::endl;
    }

    void SimulationResultWriter::SaveConfig(
        ::nlohmann::json const& j
    ) const
    {
        ::std::ofstream ofs(m_option.OutputPath() + "config.json");

        if (!ofs) [[unlikely]] throw ::std::runtime_error("Failed to create config.json file");

        ofs << j.dump(4);
    }
}