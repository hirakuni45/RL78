RL78
=========

RL78 sources 

## RL78 各ディレクトリー、及び概要など

> これは RL78 と、そのコンパイラである rl78-elf-gcc,g++ によるプログラムです。
>
> 現在は、主に「R5F100L」(G13 グループ) に対応した、ヘッダー、関連ライブラリーなどです。
>
> ※R5F100LGAFB、R5F100LEAFB
>
> プロジェクトは、Makefile、及び、関連ヘッダー、ソースコードからなり、専用のスタートアップルーチン、
> リンカースクリプトで構成されています。
> その為、専用のブートプログラムやローダーは必要なく、作成したバイナリーをそのまま実行できます。
>
> デバイスＩ／Ｏ操作では、C++ で構成されたクラスライブラリーを活用出来るように専用のヘッダーを用意
> してあり、各種デバイス用の小さなクラスライブラリーの充実も行っています。

## RL78 プロジェクト・リスト

 - /rl78prog        --->   RL78 フラッシュへのプログラム書き込みツール（Windows、OS-X、※Linux 対応）
 - /RL78            --->   デバイス、Ｉ／Ｏポート定義
 - /G13             --->   G13 グループ、リンカースクリプト
 - /common          --->   RL78 共有クラス、小規模なクラスライブラリーなど
 - /FIRST_test      --->   RL78/G13 デバイス向け初回テスト（LED の点滅）
 - /KiCAD_Lib       --->   KiCAD 用ライブラリー

## RL78 開発環境

 - RL78 用コンパイラ（rl78-elf）は gcc-4.9.3 を使っています。 
 - Windows では、事前に MSYS2 環境をインストールしておきます。

 - msys のアップグレード

```sh
   update-core
```

 - コンソールを開きなおす

```sh
   pacman -Su
```
 - アップデートは、通常複数回行われ、その際、コンソールの指示に従う事。
 - ※複数回、コンソールを開きなおす必要がある。

---

### RL78-elf-gcc コンパイラのビルド方法：（MSYS2）

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
## RL78 フラッシュプログラマーの構築

 - 以下は、まだ未完、実装中です・・・・・
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
