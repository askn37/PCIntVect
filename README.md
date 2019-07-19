# PCIntVect

ピン変化割込関数

元は [Futilities](https://github.com/askn37/Futilities/) に含まれていたものだが、
Arduino IDE でのコンパイルで、環境によっては \_\_vect\_* 重複エラーが多発するのを避けるため
機能分離を行った。

## Arduino IDE への導入

1. .ZIPアーカイブをダウンロードする。[Click here](https://github.com/askn37/PCIntVect/archive/master.zip)

2. ライブラリマネージャで読み込む

    スケッチ -> ライブラリをインクルード -> .ZIP形式のライブラリをインストール...

## 使い方

```c
#include <pcintvect.h>

// 割込を開始
attachPCInterrupt(A0, togglePinVect);

volatile bool toggle = true;

void togglePinVect (void) {
  // bool edge = digitalRead(A0);
  toggle = ~toggle;
  digitalWrite(LED_BUILTIN, toggle);
}

// 割込を停止 - A0だけ停止、割込ベクタは解除しない
detachPCInterrupt(A0);

// 割込を停止 - A0を含むポートグループ全体を止め、割込ベクタも解除
detachPCInterrupt(A0, true);

```

## リファレンス

## pcintvect.h

ピン変化割込

依存性：

注意：
SoftwareSerial とは併用できない。

### void attachPCInterrupt (uint8\_t interruptPin, void (*userFunc)(void) = NULL)

指定のピンに対応する外部ピン変化割込を有効にし、
指定のユーザー定義ルーチンを割込ベクタに割り付ける。

通常の attachInterrupt() と違って、第1引数にはピン番号を直接記述する。
また割込種別は EDGE しかないため、第3引数はない。
第2引数の割込ベクタは同一ポートグループに属する各最大8本のピンで共有され、
個々のピンに個別に割り当てるものではないことに注意。

MCU休止状態の解除のように、割込発生の事実だけを利用して割込処理が不要であるなら、第2引数には NULLを指定する。

割込ベクタ（ポートグループ）は PCINT0からPCINT3の最大4つがあるが、
MCUの品種によってピン割付定義が異なる。
また Arduino の表向きのピン番号とも一致しない。

|MCU|PCINT0|PCINT1|PCINT2|PCINT3|品種|
|---|---|---|---|---|---|
|ATmega328P|8 9 10 11 12 13|A0 A1 A2 A3 A4 A5 (A6 A7)|0 1 2 3 4 5 6 7|---|UNO ProMini|
|ATmega32U4|8 9 10 11 (14 15 16 17)|---|---|---|Leonardo ProMicro|
|ATmega1284P|A0 A1 A2 A3 A4 A5 A6 A7|4 5 6 7 10 11 12 13|22 23 24 25 26 27 28 29|0 1 2 3 8 9 30 31|Boubino|

いくつかのピンは、attachInterrupt() での割込指定と独立して共存する。
あるいは HardwareSerial や SPI や I2C の割込とも独立して共存する。
つまりそれらの専用機能割込とは全く別に、干渉せずに使用できる。

例えば HardwareSerial は MCUが休止中は割込を発生しないが、
外部ピン変化割込は検出され、休止状態を解除させることができる。
両方を併用することで MCUを適切な省電力状態に置くことができる。
個々の割込でのピン状態（HIGH・LOW、あるいは立ち上がり・立ち下がり変化）はソフトウェアで判断しなければできないが、
休止状態解除目的に限ればそういった処理は省略可能である。

ポートグループ内のどのピンが割込発生要因になったかを判断するのは難しい。
だが異なるポートグループの単一のピンに限って使用するのであれば、
そのピンのHIGH・LOW変化だけを調べれば良いので単純であるし、
普通はそのようにして使うだろう。
SoftwareSerial はまさにそのようにして外部ピン変化割込を利用している。

### void detachPCInterrupt (uint8\_t interruptPin, bool removeAll = false)

第2引数を省略あるいは偽の場合、第1引数で指定したピンに対応する PCINT割込を解除する。割込ベクタは解除されない。

第2引数が真の場合、第1引数に対応するポートグループすべてのピンの PCINT割込を解除し、割込ベクタも解除する。

----

## 既知の不具合／制約／課題

- 主要な AVR 以外はテストされていない。
- 古い Arduino IDE には対応しない。1.8.5で動作確認。少なくとも C++11 は使えなければならない。
- 文中で繰り返し断っているが、pcintvect.h は SoftwareSerial.h と共存できない。
同様に PCInt Vecotr を排他的に使用するライブラリは使用できない。
- 英文マニュアルが未整備である。

## 改版履歴

- 0.1.1
  - [Futilities](https://github.com/askn37/Futilities/) から機能分離。

## 使用許諾

MIT

## 著作表示

朝日薫 / askn
(SenseWay Inc.)
Twitter: [@askn37](https://twitter.com/askn37)
GitHub: [https://github.com/askn37](https://github.com/askn37)
