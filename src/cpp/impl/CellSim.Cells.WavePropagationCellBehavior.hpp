#ifndef CELLSIM_CELLS_WAVEPROPAGATIONCELLBEHAVIOR_HPP
#define CELLSIM_CELLS_WAVEPROPAGATIONCELLBEHAVIOR_HPP

#include "base.hpp"
#include "CellSim.Cells.MoleculeAwareCellBehavior.hpp"
#include "CellSim.Molecular.MoleculeKind.hpp"

#include <map>

/*
812ページ目
| 記号                                            | C++変数名                                          | 説明                     |
| ---------------------------------------------- | ----------------------------------------------- | ---------------------- |
| KR                                             | CampReceptorDissociationConstantR               | cAMP-受容体複合体のR状態での解離定数  |
| KD                                             | CampReceptorDissociationConstantD               | cAMP-受容体複合体のD状態での解離定数  |
| k1                                             | RateConstantRtoD                                | R → D 変換の速度定数          |
| k-1                                            | RateConstantDtoR                                | D → R 変換の速度定数          |
| k2                                             | RateConstantRPtoDP                              | RP → DP 変換の速度定数        |
| k-2                                            | RateConstantDPtoRP                              | DP → RP 変換の速度定数        |
| cAMP receptor                                  | CampReceptorCount                               | cAMP受容体の数              |
| Adenylate cyclase                              | AdenylateCyclaseActivity                        | アデニル酸シクラーゼの活性          |
| Cell density                                   | CellDensity                                     | 細胞密度                   |
| Intracellular volume                           | IntracellularVolume                             | 細胞内体積                  |
| Activity of adenylate cyclase (Basal rate)     | AdenylateCyclaseBasalRate                       | アデニル酸シクラーゼの基礎活性        |
| Activity of adenylate cyclase (Max activation) | AdenylateCyclaseMaxRate                         | アデニル酸シクラーゼの最大活性        |
| Km,cyclase                                     | MichaelisConstantAdenylateCyclase               | アデニル酸シクラーゼのミカエリス定数     |
| ki                                             | IntracellularPhosphodiesteraseRateConstant      | 細胞内ホスホジエステラーゼの1次反応速度定数 |
| Vmax,PDE                                       | MaxActivityExtracellularPhosphodiesterase       | 細胞外ホスホジエステラーゼの最大活性     |
| Km,PDE                                         | MichaelisConstantExtracellularPhosphodiesterase | 細胞外ホスホジエステラーゼのミカエリス定数  |
| kt                                             | CampTransportRateConstant                       | 細胞外へのcAMP輸送の1次反応速度定数   |
| ATP                                            | AtpConcentration                                | ATP濃度                  |

813ページ目


変数	意味・役割
b	受容体 R（活性型）とリガンド（cAMP）P の結合率定数（結合反応 R + P → RP）
c	受容体 D（不活性型）の結合親和性に対する R の相対的親和性 (KR/KD)
e	活性化反応における協同係数。2分子の RP がアデニル酸シクラーゼ C に結合し、活性化複合体 E を形成（反応: 2RP + C → E）
f	酵素反応速度の係数で、E 状態（活性型）と C 状態（基礎型）による cAMP 合成効率に関連
g	cAMP 分解率（ホスホジエステラーゼによる分解を含む）
alpha	モデル中の ATP 濃度を Km（ミカエリス定数）で正規化したもの（a = [ATP]/Km）
beta	細胞内の cAMP 濃度を KR（R型受容体の解離定数）で正規化したもの
gamma	細胞外の cAMP 濃度を KR で正規化したもの

double m_campReceptorDissociationConstantR;
double m_campReceptorDissociationConstantD;
double m_rateConstantRtoD;
double m_rateConstantDtoR;
double m_rateConstantRPtoDP;
double m_rateConstantDPtoRP;
double m_campReceptorCount;
double m_adenylateCyclaseActivity;
double m_cellDensity;
double m_intracellularVolume;
double m_adenylateCyclaseBasalRate;
double m_adenylateCyclaseMaxRate;
double m_michaelisConstantAdenylateCyclase;
double m_intracellularPhosphodiesteraseRateConstant;
double m_maxActivityExtracellularPhosphodiesterase;
double m_michaelisConstantExtracellularPhosphodiesterase;
double m_campTransportRateConstant;
double m_atpConcentration;

*/

namespace CellSim::Cells
{
    class WavePropagationCellBehavior : public MoleculeAwareCellBehavior {
        private:

        double m_c;
        double m_k_1;
        double m_k_2;
        double m_l_1;
        double m_l_2;
        double m_q;
        double m_sigma;
        double m_k_i;
        double m_k_e;
        double m_k_t;
        double m_alpha;
        double m_h;
        double m_theta;
        double m_lambda;
        double m_epsilon;

        ::std::map<Molecular::MoleculeKind, double> m_rho_T;

        constexpr double ComputeDiffBeta(
            double beta,
            double gamma,
            double rho_T
        ) const noexcept;

        constexpr double ComputeDiffGamma(
            double beta,
            double gamma
        ) const noexcept;

        constexpr double ComputeDiffRhoT(
            double gamma,
            double rho_T
        ) const noexcept;

        constexpr double F1(
            double gamma
        ) const noexcept;

        constexpr double F2(
            double gamma
        ) const noexcept;

        constexpr double Phi(
            double gamma,
            double rho_T
        ) const noexcept;

        constexpr double Y(
            double gamma,
            double rho_T
        ) const noexcept;

        public:

        WavePropagationCellBehavior(
            double cellDivisionRadius,
            double growthRate,
            double synthesisRate,
            double degradationRate,
            double c,
            double k_1,
            double k_2,
            double l_1,
            double l_2,
            double q,
            double sigma,
            double k_i,
            double k_e,
            double k_t,
            double alpha,
            double h,
            double theta,
            double lambda,
            double epsilon
        );
        
        MolecularProcessResult ComputeMolecularProcess(
            const Cell* sender,
            MolecularProcessArgs args
        ) override;
        
        CellBehavior* CreateClone() const override;
        
        constexpr bool IsReusable() const noexcept override;
    };
}

namespace CellSim::Cells
{
    constexpr double WavePropagationCellBehavior::ComputeDiffBeta(
        double beta,
        double gamma,
        double rho_T
    ) const noexcept
    {
        return m_q * m_sigma * Phi(gamma, rho_T) - (m_k_i + m_k_t) * beta;
    }

    constexpr double WavePropagationCellBehavior::ComputeDiffGamma(
        double beta,
        double gamma
    ) const noexcept
    {
        return (m_k_t * beta) / m_h - m_k_e * gamma;
    }

    constexpr double WavePropagationCellBehavior::ComputeDiffRhoT(
        double gamma,
        double rho_T
    ) const noexcept
    {
        return -F1(gamma) * rho_T + F2(gamma) * (1 - rho_T);
    }

    constexpr double WavePropagationCellBehavior::F1(
        double gamma
    ) const noexcept
    {
        return (m_k_1 + m_k_2 * gamma) / (1 + gamma);
    }

    constexpr double WavePropagationCellBehavior::F2(
        double gamma
    ) const noexcept
    {
        return (m_k_1 * m_l_1 + m_k_2 * m_l_2 * m_c * gamma) / (1 + m_c * gamma);
    }

    constexpr double WavePropagationCellBehavior::Phi(
        double gamma,
        double rho_T
    ) const noexcept
    {
        double y = Y(rho_T, gamma);
        return
            (m_alpha * (m_lambda * m_theta + m_epsilon * y * y)) /
            (1 + m_alpha * m_theta + m_epsilon * y * y * (1 + m_alpha));
    }

    constexpr double WavePropagationCellBehavior::Y(
        double gamma,
        double rho_T
    ) const noexcept
    {
        return (rho_T * gamma) / (1 + gamma);
    }

    inline CellBehavior* WavePropagationCellBehavior::CreateClone() const
    {
        return new WavePropagationCellBehavior(*this);
    }

    constexpr bool WavePropagationCellBehavior::IsReusable() const noexcept
    {
        return false;
    }
}

#endif //!CELLSIM_CELLS_WAVEPROPAGATIONCELLBEHAVIOR_HPP