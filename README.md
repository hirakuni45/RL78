RL78
=========

## RL78 各ディレクトリー、及び概要など

これは RL78 と、そのコンパイラである rl78-elf-gcc,g++ によるプログラムです。
   
***ルネサスエレクトロニクスが提供する統合開発環境を使う事も出来ますが、永続的に使うには
製品版を購入するか、生成したバイナリーが６４Ｋバイト以下に限られる為、gcc を使います。   
また、Mac OS-X、Linux などのマルチプラットホームで開発出来る利便性も重視しています。***
   
現在は、主に「R5F100L」(G13 グループ) に対応した、ヘッダー、関連ライブラリーなどです。
   
・R5F100LCAFB (FLASH: 32K, RAM: 2K, DATA-FLASH:4K) @250   
・R5F100LEAFB (FLASH: 64K, RAM: 4K, DATA-FLASH:4K) @290   
・R5F100LGAFB (FLASH:128K, RAM:12K, DATA-FLASH:8K) @340 (変換基板実装済み @420)   
・R5F100LJAFB (FLASH:256K, RAM:20K, DATA-FLASH:8K) @400   
※ @xxx は、[秋月電子販売の参考価格](http://akizukidenshi.com/catalog/c/crl78g13/)
   
プロジェクトは、Makefile、及び、関連ヘッダー、ソースコードからなり、専用のスタートアップルーチン、
リンカースクリプトで構成されています。
その為、専用のブートプログラムやローダーは必要なく、作成したバイナリーをそのまま実行できます。   
これは、デバイスにハードウェアー・リセットが入力されて、自分のアプリケーションが動作するまでの
全てを詳細に公開してあり、判り易いものとなっています。   
   
デバイスＩ／Ｏ操作では、C++ で構成されたテンプレート・クラス・ライブラリーを使い、専用のヘッダー
と各種デバイス用のクラスライブラリーを用意してあり、最小限の実装で、高機能で柔軟な操作を実現するべく、
日々改良と研究を行っています。   
※各デバイス向けクラス・ライブラリーの提供を募集しています。（MIT ライセンスで提供出来る事）   
E-Mail: hira@rvf-rc45.net
   
## RL78 プロジェクト・リスト
   
 - /rl78prog        --->   RL78 フラッシュへのプログラム書き込みツール（Windows、OS-X、※Linux 対応）
 - /G13             --->   G13 グループ、リンカースクリプト、デバイス定義ファイル
 - /common          --->   RL78 共有クラス、小規模なクラスライブラリー、ユーティリティー
 - /FIRST_sample    --->   RL78/G13 デバイス向け超簡単なサンプル（LED の点滅）
 - /UART_sample     --->   RL78/G13 デバイス向け、シリアル・コミュニケーションのサンプル（RxD0）
 - /INTERVAL_TIMER_sample ---> RL78/G13 内臓１２ビット、インターバル・タイマーのサンプル
 - /SOFT_DELAY)sample ---> RL78/G13 ソフトウェアー・タイマーのサンプル
 - /DS3231_sample   --->   DS3231 Real Time Clock の時間表示、設定サンプル
 - /KiCAD_Lib       --->   KiCAD 用部品ライブラリー
   
## RL78 開発環境
   
 - RL78 用コンパイラ（rl78-elf）は gcc-4.9.3 を使っています。 
 - Windows では、事前に MSYS2 環境をインストールしておきます。
 - MSYS2 には、msys2、mingw32、mingw64 と３つの異なった環境がありますが、msys2 で行います。 
   
 - msys2 のアップグレード

```sh
   update-core
```

 - コンソールを開きなおす。（コンソールを開きなおすように、メッセージが表示されるはずです）

```sh
   pacman -Su
```
 - アップデートは、複数回行われ、その際、コンソールの指示に従う事。
 - ※複数回、コンソールを開きなおす必要がある。

---

### RL78-elf-gcc コンパイラのビルド方法：（msys2）

 - gcc、texinfo、gmp、mpfr、mpc、diffutils、automake、zlib tar、make、unzip コマンドなどをインストール
```sh
   pacman -S gcc
   pacman -S texinfo
   pacman -S mpc-devel
   pacman -S diffutils
   pacman -S automake
   pacman -S zlib
   pacman -S tar
   pacman -S make
   pacman -S unzip
```
  
 - git コマンドをインストール
```sh
   pacman -S git
```

#### binutils-2.25.1 をビルド
```sh
   cd
   tar xfvz binutils-2.25.1.tar.gz
   cd binutils-2.25.1
   mkdir rl78_build
   cd rl78_build
   ../configure --target=rl78-elf --prefix=/usr/local/rl78-elf --disable-nls
   make
   make install
```

 -  /usr/local/rl78-elf/bin へパスを通す（.bash_profile を編集して、パスを追加）

```
   PATH=$PATH:/usr/local/rl78-elf/bin
```

 -  コンソールを開きなおす。

```
   rl78-elf-as --version
```

 -  アセンブラコマンドを実行してみて、パスが有効か確かめる。
  
#### C コンパイラをビルド
 -  gcc-4.9.3では、MSYS2 環境を認識しない為、automake の設定をコピーする
``` sh
    cd
    tar xfvz gcc-4.9.3.tar.gz
    cd gcc-4.9.3
    cp /usr/share/automake-1.9/config.guess .
    mkdir rl78_build
	cd rl78_build
    ../configure --prefix=/usr/local/rl78-elf --target=rl78-elf --enable-languages=c --disable-libssp --with-newlib --disable-nls --disable-threads --disable-libgomp --disable-libmudflap --disable-libstdcxx-pch --disable-multilib --disable-bootstrap
    make
    make install
```
  
#### newlib をビルド
``` sh
    cd
    tar xfvz newlib-2.2.0.tar.gz
	cd newlib-2.2.0
    mkdir rl78_build
    cd rl78_build
    ../configure --target=rl78-elf --prefix=/usr/local/rl78-elf
	make
    make install
```
  
#### C++ コンパイラをビルド
``` sh
    cd
    cd gcc-4.9.3
    cd rl78_build
    ../configure --prefix=/usr/local/rl78-elf --target=rl78-elf --enable-languages=c,c++ --disable-libssp --with-newlib --disable-nls --disable-threads --disable-libgomp --disable-libmudflap --disable-libstdcxx-pch --disable-multilib --disable-bootstrap
    make
    make install
```
   
---
   
## RL78 プロジェクトのソースコードを取得

```
   git clone git@github.com:hirakuni45/RL78.git
```
   
--- 
   
## RL78/G13 デバイスへのプログラム書き込み方法

幾つかの方法がありますが、最も簡単で、コストがかからない方法は、シリアルインターフェースを使って
書き込む方法です。   
但し、Ｒ８Ｃのように直接接続する事は出来ません。
シリアルインターフェースからの３つ（５つ）の信号を適切な変換回路でマイコンと接続する必要があります。   
※USB シリアル変換モジュールなどを使うと、電源も取れて簡単です。
※（秋月電子、シリアル変換モジュール）http://akizukidenshi.com/catalog/g/gK-06894/
(1) RXD シリアル受信   
(2) TXD シリアル送信   
(3) RTS ハードウェアー制御信号   
(4) VCC 電源（５Ｖ又は３．３Ｖ）   
(5) GND 電源 ０Ｖ   
※ RTS 信号が取り出せる変換アダプターが必要です、DCD 信号では代用できません。   
※中国製の格安なモジュールは、RTS が無い物や、品質が安定していない為、お勧めしません、それらの事
項を理解していて対処出来る人だけ利用すると良いと思います。
   
 - 回路図は、rl78prog/KiCAD/ を参照して下さい、簡易書き込み回路があります。   
 - 正規の回路は、ルネサスエレクトロニクスのホームページからダウンロード出来ます。
 - もちろん、ルネサスエレクトロニクス販売のＥ１、又は、Ｅ２、エミュレーターでも書き込む事が出来ます。
 - RL78/G13（６４ピン製品） の「P40/TOOL0 (5)」、「/RESET (6)」に接続します。
 - シリアル通信は、開発過程では良く利用するので、スイッチで切り替えられるようにしておくと便利です。   
※切り替えの参考回路が、「rl78prog/KiCAD」にあります。   
   
## RL78 フラッシュプログラマーの構築

 - 以下は、まだ未完、実装中です・・・・・
 - ※ Renesas Flash Programmer の最新版を使って下さい。
 - rl78prog のビルドには「boost_1_60_0」が必要です。
 - boost はヘッダーのみ利用なので、ビルドの必要はありません、boost_1_60_0.zip を展開するだけです。
 - 又は、mingw64 環境などに pacman を使い boost をインストールして、そのパスを設定しても良いでしょう。

``` 
    cd /usr/local
    unzip boost_1_60_0.zip
```

 - rl78prog のビルド（MSYS2）
 - ビルドした実行ファイルは、~/bin に配置します。

``` sh
    cd rl78prog
    make
    mkdir ~/bin
    cp rl78_prog.exe ~/bin/.
    cp rl78_prog.conf ~/bin/.
　　※~/bin にパスを通しておく。
```
 - rl78_prog.conf を編集して、接続する COM ポート、ボーレートの設定をする。
 - /dev/ttyS10 -> COM11 に相当します。（数字に＋１する）
 - rl78prog/KiCAD/ に、RL78 プログラマー（書き込み機）の参考回路などが含まれます。
  
---
## 各プロジェクトの動作と秘訣

 - 各プロジェクトのリストを参考に、プロジェクトのディレクトリーに移動します。
 - 各プロジェクトは、「R5F100LGAFB」（ProgramFlash: 128KB、RAM: 12KB、DataFlash: 8KB）がデフォルトとなっています。
 - Makefile 内で、他のデバイスに変更する事が出来ます。※ワーク RAM サイズが違うので必ず合わせる必要があります。
 - 基本的には、内部発振を使い 32MHz で動作させています。
 - 動作周波数を変更したい場合は、同時に Makefile 内の F_CLK を指定する必要があります。
 - より正確なクロックで動かしたい場合、外部にクリスタルを接続し、外部発振を有効にすれば、使う事ができます。
 - 但し、最大２０ＭＨｚまでのクリスタルにしか対応しません。
 - 最大速度で動かしたい場合で正確性を要求する場合、発振モジュールを使い３２ＭＨｚを入力する必要があります。
 - 起動時の動作周波数は、common/option_bytes.c で行っています。
 - C++ のテンプレートを多用しているので、バイナリーは大きくなりがちです。
 - サイズを優先して最適化したい場合は「-Os」を指定する事で、バイナリーが小さくなります。
 - 「R5F100LGAFB」は、変換基板にハンダ付けされた物が格安で入手出来ます。（秋月電子）
 - 結線に関しては、RL78/G13 ハードウェアー・マニュアルを参照して下さい。
 - 基本的には電源を繋ぎ、適切なバイパスコンデンサを繋ぐだけで動作します。
 - 特別（複雑な場合）な接続を要求する場合は、KiCAD のプロジェクトファイルを用意しています。
   
---
 - ビルドします。（自動で、従属規則が生成されます）
``` sh
    make
```

 - プログラムの書き込み（rl78_prog が必要）
``` sh
    make run
```
---
## 新規プロジェクトの作成
 - 新規プロジェクト・ディレクトリーを作成します。
 - TMPtestなどから、雛形に使う「Makefile」、「main.cpp」をコピーします。
 - Makefile の必要な部分を書き換えて、自分のプロジェクト用にします。
 - Makefile 内のソースコードパスに、自分のソースコード名を追加する事が出来ます。
 - Makefile では「従属規則」が重要ですが、これは自動で生成します。

## 各ファイル概略
 - G13/R5F100LCAFB.ld　R5F100LCAFB ( 32K/ 2K/4K) 用リンカースクリプト   
 - G13/R5F100LEAFB.ld　R5F100LEAFB ( 64K/ 4K/4K) 用リンカースクリプト   
 - G13/R5F100LGAFB.ld　R5F100LGAFB (128K/12K/8K) 用リンカースクリプト   
 - G13/R5F100LJAFB.ld　R5F100LJAFB (256K/20K/8K) 用リンカースクリプト   
 - G13/readme.txt　リンカースクリプトの概略
 - G13/adc.hpp　Ａ／Ｄコンバーター定義
 - G13/dma.hpp　ＤＭＡコントローラー定義
 - G13/iica.hpp　ＩＩＣＡ（Ｉ２Ｃ）定義
 - G13/intr.hpp　割り込み制御関係定義
 - G13/port.hpp　ポート関係定義
 - G13/sau.hpp　シリアル・アレイ・ユニット定義
 - G13/system.hpp　システム関係定義
 - G13/timer.hpp　１２ビットインターバルタイマー定義
   
---
 - common/start.s　ハードウェアー・リセット初期化
 - common/init.c　main 初期化
 - common/option_bytes.hpp　オプション・バイト設定
 - common/vect.h　ハードウェアー・ベクター定義
 - common/vect.c　ハードウェアー・ベクター設定
 - common/time.h　時間関数定義（posix time.h にある機能の縮小版）
 - common/time.c　時間関数実装（posix time.c にある機能の縮小版）
 - common/command.hpp　行入力テンプレート
 - common/delay.hpp　ソフトウェアー・ディレイ（３２ＭＨｚ動作、マイクロ秒単位）
 - common/ds3231_io.hpp　ＤＳ３２３１、ＲＴＣ、入出力
 - common/eeprom_io.hpp　Ｉ２Ｃ－ＥＥＰＲＯＭ入出力
 - common/fifo.hpp　First-in first-out バッファ
 - common/format.hpp　文字列整形テンプレート
 - common/iica_io.hpp　ＩＩＣＡ入出力テンプレート
 - common/uart_io.hpp　シリアル・コミュニケーション入出力（ＵＡＲＴ）
   
---
License

MIT
