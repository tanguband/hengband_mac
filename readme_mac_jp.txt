変愚蛮怒 2.2.1 macOS

ローグライクゲーム、変愚蛮怒の Mac フロントエンドです。
Mac OS X 10.9以降をターゲットとしています。
ビルド・動作の確認は macOS High Sierra 10.13, Xcode 9.3 で行なっています。


1. コンパイル済アプリケーションについて

~/Documents/Hengband 以下に保存ディレクトリを勝手に作ります。
（現時点では、変更不可）
~/Library/Preferences/jp.sourceforge.hengband.Hengband.plist
にGUI関連の初期設定が保存されます。
変愚蛮怒の lib/ は Hengband.app/Contents/Resources 以下にあります。
Finder で右クリック→パッケージの内容を表示して下さい。


2. ビルド方法について

build/ 以下のファイルを変愚蛮怒の最新版ソースツリーに設置します。
src/cocoa/ （ディレクトリ全体）
src/main-cocoa.m
src/makefile.osx
lib/pref/pref-mac.prf （上書き）

ターミナルで src/ に cd し、
ソースコードをEUCに変換
nkf -e -—overwrite *.c
nkf -e -—overwrite *.h
メイク
make -f makefile.osx
するとツリートップにビルドされます。（要 Xcode）


3. 未サポートの機能

・IMEによる日本語入力（ペーストは可能）
・テキストのコピー

・グラフィックタイル
・サウンド

・保存ディレクトリの変更


4. 既知の問題

・小さい文字でアンチエイリアスが見づらい
・文字の間隔が不適切（特にヒラギノやDFPフォント）
・文字の描画が消えてしまう
・64bit モードでの動作が不安定
　（ビルド済および makefile は 32bit に制限しています）


5. 履歴

変愚蛮怒 2.0.0 の Mac OS X フロントエンドは vanilla Angband 3.3.0 から移植されました。
https://github.com/oddstr/hengband-cocoa

変愚蛮怒 2.1.4以降対応
https://github.com/shimitei/hengband-cocoa
