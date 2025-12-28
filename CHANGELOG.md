# 更新履歴

## [2.2.0 Alpha 2] 2025-12-21
### 不具合修正
- `--load-cell-csv`使用時に`Invalid`なCellが追加される不具合を修正

## [2.2.0 Alpha 1] 2025-12-21
### 新機能
- NamedPipeを実装
- イベントオブジェクトを追加
- コマンドラインオプション`--load-cell-csv`を追加
### 不具合修正
- ClusterFormationModelとNetworkFormationModelの計算式が間違っている問題を修正

## [2.1.2] 2025-12-15
### 不具合修正
- シミュレーションモデルのパラメータのうち、細胞の種類に依存しないパラメータが読み込まれない不具合を修正。

## [2.1.1] 2025-10-11
### 変更点
- Barnes-HutとDualTreeBHの更新
### 破壊的変更
- ParticleMeshを削除
- InitialMoleculeDistribution.Gaussianを削除

## [2.1.0(2)] 2025-09-20
### 不具合修正
- MSVCでビルドすると警告が表示される不具合を修正

## [2.1.0] 2025-09-17
### 新機能
- アルゴリズム側で相互作用を行う機能を追加
- コマンドラインオプション'--no-clean-output'を追加
- コマンドラインオプション'--param-sweep'を複数回指定可能にする機能を追加
- '-p --param'オプションに'optionArray=empty'で配列を空にする機能を追加
### 変更点
- モデルのパラメータを細胞の種類ごとに設定できるように変更
- ナイーブなアルゴリズムの高速化
- `cell.enableGrowth`がfalseの時に、細胞分裂も無効になるように変更
- CellBehaviorのパラメータを細胞の種類ごとに設定できるように変更
- 最大スレッド数を512に制限
- 設定ファイルのmolecularのMoleculeBehaviorのパラメータの記述方法を変更
- CellTypeに同じ名前を指定するとエラーになるように変更
- 実行後に生成される'config.txt'を'result.txt'に変更
- 'result.txt'に実行したCellSimのバージョンを記載
- 実行後に出力フォルダにパラメータを記載した'config.json'を生成するように変更
### 不具合修正
- macOSおよびLinuxでReleaseビルドにならない問題を修正
- '--param-sweep'を使ったときに2回目以降が実行されない可能性がある問題を修正
- CellBehaviorPtr::Null()を使うとSegmentation Faultが発生する可能性がある問題を修正

## [2.0.0] 2025-08-14
### 新機能
- ヘルプオプション`-h --help`を追加
- `-p --param`で配列を空にする機能を追加
### 変更点
- 分子データの描画方法を変更
- 分子データに同じ名前を指定するとエラーになるように変更
### 破壊的変更
- 2次元フラグの名前を"Enable2dMode"に統一
### 不具合修正
- `-p --param`, `--param-sweep`でcell.cellsとmolecularのパラメータを設定できない問題を修正

## [2.0.0 Beta 9] 2025-08-13
### 新機能
- 分子データを.bin, .csv, .pngで出力する機能を追加
### 変更点
- 分子データが存在しない場合はmoleculeフォルダを生成しないように更新
### 不具合修正
- 2次元シミュレーション時にNormalMoleculeBehaviorが2次元ではなく、3次元の拡散方程式を計算してしまう問題を修正
- 分子が拡散しない問題を修正

## [2.0.0 Beta 8] 2025-08-10
### 新機能
- Colorを追加
- CellCreateInfoを追加
- CellTypeにAddName()、Color, Id、Nameを追加
- MoleculeKindにAddName()、Id、Nameを追加
- config.jsonで複数種類のCellを指定できるように更新
- 画像・動画出力時のCellの色を指定できるように更新
- ターゲット環境がARMかx86かを判定するマクロを追加
- MoleculeFieldを動画出力する機能を追加
### 変更点
- Cellの等値判定を「Idの一致」から「アドレスの一致」に変更
### 破壊的変更
- CellTypeをenumからclassに変更
- MoleculeKindをenumからclassに変更
### 不具合修正
- .csvと.binにCellのIsAliveプロパティが出力されない問題を修正


## [2.0.0 Beta 7] 2025-07-23
### 新機能
- ダミーCellを作成するCell.CreateDummy()を追加
- Cellにthisがダミーであるかを判定するプロパティIsDummyを追加
- WavePropagationCellBehaviorを追加
### 変更点
- CellSimulationModel.ComputeForceOnCell(sender, args)のargs.CellAlgorithmがnullの時、ナイーブなアルゴリズムとして実行するように変更
- ナイーブなアルゴリズム使用時にパフォーマンスが向上
### 破壊的変更
- CellBehaviorのComputeMetabolicChange(), ComputeMoleculeEmitAmount()を削除し、代わりにComputeMolecularProcess()を追加
- CellBehaviorのHasState()を削除し、代わりにIsReusable()を追加
- CellのAppendMoleculeUnsafe()を削除
- CellのEmitMolecule(), Metabolize()を削除し、代わりにProcessMolecules()を追加
### 不具合修正
- UserSettingsが読み込まれない問題を修正
- CellAlgorithmをCellList以外にした状態でクラスタモデルを有効にするとアプリケーションがクラッシュする問題を修正
- CellAlgorithmをCellList以外にした状態でネットワーク形成モデルを動かそうとするとアプリケーションがクラッシュする問題を修正

## [2.0.0 Beta 6] 2025-07-09
### 新機能
- 出力画像・動画の縦横の長さを変更する設定を追加
- 並列処理を行うスレッド数の設定を追加
### 不具合修正
- `-p --param`で浮動小数点数を指定するとエラーが発生する問題を修正
- CellSim.Cli.RepeatedValueOption.Values()が基底クラスの仮想関数Values()をオーバーライドしない問題を修正


<!--
## [2.0.0 Beta 5] 2025-07-01
### 新機能
- パラメータオプション`-p --param`を追加
### 不具合修正
- クラスタ形成モデルの計算式の間違いを修正

-->