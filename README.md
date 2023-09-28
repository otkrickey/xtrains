# xtrains
このシステムは、架空の電車や路線について、ユーザーが目的の経路を検索するためのアプリケーションです。
乗り換え時間、階段の数、乗り換え回数などの様々な要因を考慮した上で最適な経路を提案することを目指しています。

## 主要機能
- 駅から駅への経路検索
- 乗り換え時の利便性を考慮した経路検索
- CLIによるユーザーインターフェース

## 技術仕様
- C++17 (GCC 11.4.0)
- Cmake 3.22.1

## 開発環境のセットアップ
### 1. C++の開発環境をセットアップする
#### WSL2 (Ubuntu 22.04 LTS)
```bash
sudo apt update
sudo apt install build-essential cmake
```

### 2. VSCodeの拡張機能をインストールする
- [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)


### 3. リポジトリをクローンする
```bash
git clone https://github.com/otkrickey/xtrains.git
cd xtrains
```

### 4. ビルド
`Ctrl + Shift + B` でビルドを実行する。

### 5. デバッグ
`F5` でデバッグを実行する。

### 6. 実行
`Shift + F5` で実行する。