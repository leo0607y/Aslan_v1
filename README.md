# Aslan_v1

STM32F405RGマイコンを使用したロボット制御プロジェクト

## 概要

このプロジェクトは、STM32F405RGマイコンを使用した2輪ロボットの制御システムです。エンコーダによる位置検出、センサー読み取り、LED制御などの機能を実装しています。

## ハードウェア仕様

- **マイコン**: STM32F405RGT6
- **クロック**: HSE 24MHz → PLL 168MHz
- **エンコーダ**: 4096パルス/回転、減速比 18/64
- **ホイール半径**: 11.874mm
- **センサー**: ADC 12チャンネル
- **LED**: RGB LED × 1、単色LED × 6

## 主な機能

### エンコーダ (`Encoder.c/h`)
- 左右2つのエンコーダによる走行距離計測
- 1ms周期でのカウント更新
- 累積距離の計算

### センサー (`Sensor.c/h`)
- ADC DMAによる12チャンネルのセンサー値読み取り
- キャリブレーション機能
- リアルタイム値更新

### LED制御 (`LED.c/h`)
- RGB LED（赤、緑、青、白、黄、シアン、マゼンタ）
- 6個の単色LED個別制御

### スイッチ (`SW.c/h`)
- 左右2つのタクトスイッチ入力
- チャタリング防止処理

## ファイル構成

```
Aslan_v1/
├── Core/
│   ├── Inc/               # ヘッダーファイル
│   │   ├── main.h
│   │   ├── Encoder.h
│   │   ├── Sensor.h
│   │   ├── LED.h
│   │   ├── SW.h
│   │   └── ...
│   ├── Src/               # ソースファイル
│   │   ├── main.c
│   │   ├── Encoder.c
│   │   ├── Sensor.c
│   │   ├── LED.c
│   │   ├── SW.c
│   │   └── ...
│   └── Startup/
│       └── startup_stm32f405rgtx.s
├── Drivers/
│   ├── CMSIS/             # ARM CMSIS
│   └── STM32F4xx_HAL_Driver/  # STM32 HALライブラリ
├── Debug/                 # ビルド出力
├── Aslan_v1.ioc          # STM32CubeMX設定ファイル
└── STM32F405RGTX_FLASH.ld # リンカースクリプト
```

## 開発環境

- **IDE**: STM32CubeIDE 1.15.1
- **HAL Library**: STM32F4xx HAL Driver
- **設定ツール**: STM32CubeMX

## ビルド方法

1. STM32CubeIDEでプロジェクトを開く
2. プロジェクトを右クリック → `Build Project`
3. または `Ctrl + B` でビルド

## 書き込み方法

1. STM32F405RGボードをST-LINKで接続
2. `Run` → `Debug` または `F11`
3. デバッグ実行が開始されます

## タイマー設定

- **TIM6**: 1ms周期タイマー（エンコーダ更新）
- **TIM7**: 0.1ms周期タイマー（高速制御用）
- **TIM1**: モーター制御PWM
- **TIM3/TIM4**: エンコーダ入力
- **TIM8**: 予備

## 通信

- **USART1**: デバッグ用シリアル通信
  - printf関数によるログ出力に対応

## 主な定数

```c
#define WHEEL_RADIUS (11.874f / 1000.0f)    // ホイール半径 [m]
#define ENCODER_RESOLUTION 4096.0f           // エンコーダ分解能
#define REDUCTION_RATIO (18.0f / 64.0f)     // 減速比
#define ADC_NUM 12                           // ADCチャンネル数
```

## ライセンス

Copyright (c) 2025 STMicroelectronics.
All rights reserved.

このソフトウェアは、ソフトウェアコンポーネントのルートディレクトリにあるLICENSEファイルに記載された条件の下でライセンスされています。

## 作成者

- reoch

## 更新履歴

- 2025/11/23: プロジェクト作成
- 2025/11/25: エンコーダ機能実装
