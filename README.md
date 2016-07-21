RL78
=========

## RL78 各ディレクトリー、及び概要など

これは RL78 と、そのコンパイラである rl78-elf-gcc,g++ によるプログラムです。
   
***ルネサスエレクトロニクスが提供する統合開発環境を使う事も出来ますが、永続的に使うには
製品版を購入するか、生成したバイナリーが６４Ｋバイト以下に限られる為、gcc を使います。***
   
現在は、主に「R5F100L」(G13 グループ) に対応した、ヘッダー、関連ライブラリーなどです。
   
・R5F100LCAFB (FLASH: 32K, RAM: 2K, DATA-FLASH:4K) @250   
・R5F100LEAFB (FLASH: 64K, RAM: 4K, DATA-FLASH:4K) @290   
・R5F100LGAFB (FLASH:128K, RAM:12K, DATA-FLASH:8K) @340 (変換基板実装済み @420)   
・R5F100LJAFB (FLASH:256K, RAM:20K, DATA-FLASH:8K) @400   
※ @xxx は、[秋月電子販売の参考価格](http://akizukidenshi.com/catalog/c/crl78g13/)
   
プロジェクトは、Makefile、及び、関連ヘッダー、ソースコードからなり、専用のスタートアップルーチン、
リンカースクリプトで構成されています。
その為、専用のブートプログラムやローダーは必要なく、作成したバイナリーをそのまま実行できます。
   
デバイスＩ／Ｏ操作では、C++ で構成されたテンプレート・クラス・ライブラリーを使い、専用のヘッダー
と各種デバイス用のクラスライブラリーを用意してあり、最小限の実装で、高機能な操作を実現するべく、
日々充実を行っています。
※各デバイス向けクラス・ライブラリーの提供を募集しています。（MIT ライセンスで提供出来る事）   
E-Mail: hira@rvf-rc45.net
   
## RL78 プロジェクト・リスト
   
 - /rl78prog        --->   RL78 フラッシュへのプログラム書き込みツール（Windows、OS-X、※Linux 対応）
 - /G13             --->   G13 グループ、リンカースクリプト
 - /common          --->   RL78 共有クラス、小規模なクラスライブラリーなど
 - /FIRST_test      --->   RL78/G13 デバイス向け初回テスト（LED の点滅）
 - /UART_test       --->   RL78/G13 デバイス向け
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
シリアルインターフェースからの３つ（５つ）の信号があれば書きこめます。
USB シリアル変換モジュールなどを使うと、電源も取れて簡単です。
(1) RXD シリアル受信
(2) TXD シリアル送信
(3) RTS ハードウェアー制御信号
(4) VCC 電源（５Ｖ又は３．３Ｖ）
(5) GND 電源 ０Ｖ
※ RTS 信号が取り出せる変換アダプターが必要です、DCD 信号では代用できません。
   
回路図は、rl78prog/KiCAD/ を参照して下さい、簡易回路があります。
   
## RL78 フラッシュプログラマーの構築

 - 以下は、まだ未完、実装中です・・・・・
 - ※ Renesas Flash Programmer V3.01 を使って下さい。
 - rl78prog のビルドには「boost_1_60_0」が必要です。
 - boost はヘッダーのみ利用なので、ビルドの必要はありません、boost_1_60_0.zip を展開するだけです。

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
## 各プロジェクトの動作

 - 各プロジェクトのリストを参考に、プロジェクトのディレクトリーに移動します。

 - ビルドします。（自動で、従属規則が生成されます）
``` sh
    make
```

 - プログラムの書き込み（rl78_prog が必要）
``` sh
    make run
```
---
License

MIT
