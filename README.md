# Overview
分子通信シミュレーションのための汎用シミュレータであり、ユーザがシミュレーションのパラメータや力学作用を定義することでさまざまなモデルに対応したシミュレーションを実行することができます。

# マニュアル
詳しい使い方はこちらを参照してください。
[マニュアル](./docs/ja/README.md)

# 既知の不具合
- 環境によってOpenCVのビルドに失敗することがある

# ビルド・動作要件
- C++20をサポートするコンパイラ
- CMake (version 3.11~3.31)
- Windows or macOS or Linux
# ビルド方法
`Windows`の場合
```sh
cd build-scripts
.\build-windows.bat
```

`macOS`の場合
```sh
cd build-scripts
chmod +x build-macos.sh
./build-macos.sh
```

`Linux`の場合
```sh
cd build-scripts
chmod +x build-linux.sh
./build-linux.sh
```

## 実行ファイルの場所
`./build-scripts/build/bin/`に作られます。  
MSVCでビルドされた場合は`./build-scripts/build/bin/Release/`に作られます。

# コマンドラインオプション
### `-b` `--binary`
実行結果を`.bin`ファイルとして出力します。
```sh
./cellsim -b
```
### `-c` `--csv`
実行結果を`.csv`ファイルとして出力します。
```sh
./cellsim -c
```

### `-h` `--help`
ヘルプを表示します。  
**注意:** このオプションはほかのオプションと併用できません。
```sh
./cellsim -h
```

### `-i` `--image`
実行結果を画像ファイルとして出力します。
```sh
./cellsim -i
```
### `-o` `--output`
実行結果の出力ディレクトリを指定します。  
**注意:** 指定しない場合は`./result/`になります。
```sh
./cellsim -b -i -o "./my-output-path"
```
```sh
./cellsim -b -i -o "./my-output-path/"
```
### `-p` `--param`
パラメータをオーバーライドします。
ここでは、細胞の初期数をオーバーライドしています。
```sh
./cellsim -b -p cell.cells.0.cellCount=1000
```
複数のパラメータをオーバーライドすることもできます。
```sh
./cellsim -b -p cell.cells.0.cellCount=3000 -p cellAlgorithm.useClusterModel=true
```
### `-s` `--setting`
使用する設定ファイルのパスを指定します。  
**注意:** 指定しない場合は`./config.json`が使われます。
```sh
./cellsim -b -s "./my-config.json"
```
### `-v` `--video`
実行結果を動画ファイルとして出力します。
```sh
./cellsim -v
```

<!--
### `--create-image`
出力したcsvファイルまたはbinファイルから画像を作成します。
このオプションは`-o` `--output`のみと併用可能です。  
**注意:** 指定しない場合は`./result/images/`になります。
```sh
./cellsim --create-image "./my-csvpath" -o "./my-image-path"
```
```sh
./cellsim --create-image "./my-csvpath/" -o "./my-image-path"
```
### `--create-video`
出力したcsvファイル、binファイル、画像ファイルから動画を作成します。
このオプションは`-o` `--output`のみと併用可能です。  
**注意:** 指定しない場合は`./result/out.mp4`になります。
```sh
./cellsim --create-image "./my-imagepath" -o "./my-video-path/my-video-name.mp4"
```
```sh
./cellsim --create-image "./my-imagepath/" -o "./my-video-path/my-video-name.mp4"
```
### `--named-pipe-in` 
GUIツールと通信するための、読み取り用の名前付きパイプの名前を指定します。
シミュレータは指定したパイプから情報を読み取ります。  
**注意:** このオプションは`--named-pipe-out`と併用する必要があります。
```sh
./cellsim --named-pipe-in "my-pipe-in" --named-pipe-out "my-pipe-out"
```
### `--named-pipe-out`
GUIツールと通信するための、書き込み用の名前付きパイプの名前を指定します。
シミュレータは指定したパイプに情報を書き込みます。  
**注意:** このオプションは`--named-pipe-in`と併用する必要があります。
```sh
./cellsim --named-pipe-in "my-pipe-in" --named-pipe-out "my-pipe-out"
```

-->

### `--load-cell-csv`
cellの初期配置をCSVファイルから読み込みます
**注意:** 設定ファイル(.json)は指定したCSVファイルを出力したときに使ったものを使用してください。
```sh
./cellsim -c ---load-cell-csv "./output.csv"
```

### `--no-clean-output`
シミュレーション実行前に出力先のフォルダの中身を削除しないように指定します
```sh
./cellsim -c --no-clean-output -o outDir
```

### `--param-sweep`
パラメータ範囲を順番に実行します。
以下の例では細胞数を1000個から2000個まで100刻みで実行します。
```sh
./cellsim -b --param-sweep cell.cells.0.cellCount=1000:2000:100
```
複数のパラメータを範囲実行することもできます。
```sh
./cellsim -c --param-sweep cell.cells.0.cellCount=1000:2000:100 --param-sweep cell.cells.0.modelParameters.clusterFormation.lambda=10:20:1
```