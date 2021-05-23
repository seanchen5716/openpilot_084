#pragma once

#include <QTimer>
#include <QPushButton>
#include <QNetworkAccessManager>

#include "widgets/controls.hpp"
#include "selfdrive/hardware/hw.h"

// SSH enable toggle
class SshToggle : public ToggleControl {
  Q_OBJECT

public:
  SshToggle() : ToggleControl("使用SSH連線登入", "", "", Hardware::get_ssh_enabled()) {
    QObject::connect(this, &SshToggle::toggleFlipped, [=](bool state) {
      Hardware::set_ssh_enabled(state);
    });
  }
};

class SshLegacyToggle : public ToggleControl {
  Q_OBJECT

public:
  SshLegacyToggle() : ToggleControl("使用原有的公鑰", "使用原有的公鑰連線登入ssh(0.8.2以下適用)", "", Params().getBool("OpkrSSHLegacy")) {
    QObject::connect(this, &SshLegacyToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrSSHLegacy", &value, 1);
    });
  }
};

class GetoffAlertToggle : public ToggleControl {
  Q_OBJECT

public:
  GetoffAlertToggle() : ToggleControl("熄火後通知取下EON", "(C2無法使用)熄火後通知取下EON", "../assets/offroad/icon_shell.png", Params().getBool("OpkrEnableGetoffAlert")) {
    QObject::connect(this, &GetoffAlertToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrEnableGetoffAlert", &value, 1);
    });
  }
};

class AutoResumeToggle : public ToggleControl {
  Q_OBJECT

public:
  AutoResumeToggle() : ToggleControl("Stop & Go", "開啟SCC的狀態，停止時會跟隨前車起步", "../assets/offroad/icon_shell.png", Params().getBool("OpkrAutoResume")) {
    QObject::connect(this, &AutoResumeToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrAutoResume", &value, 1);
    });
  }
};

class VariableCruiseToggle : public ToggleControl {
  Q_OBJECT

public:
  VariableCruiseToggle() : ToggleControl("可變速巡航", "開啟SCC的狀態，使用按鈕來輔助進行加減速", "../assets/offroad/icon_shell.png", Params().getBool("OpkrVariableCruise")) {
    QObject::connect(this, &VariableCruiseToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrVariableCruise", &value, 1);
    });
  }
};

class CruiseGapAdjustToggle : public ToggleControl {
  Q_OBJECT

public:
  CruiseGapAdjustToggle() : ToggleControl("停止時自動改變車距", "停止時將前車車距改為1輛，以便快速跟車起步。若滿足條件後，將回到設定的車距", "../assets/offroad/icon_shell.png", Params().getBool("CruiseGapAdjust")) {
    QObject::connect(this, &CruiseGapAdjustToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("CruiseGapAdjust", &value, 1);
    });
  }
};

class AutoEnabledToggle : public ToggleControl {
  Q_OBJECT

public:
  AutoEnabledToggle() : ToggleControl("自動接手", "在C2與汽車分離的狀態下，開啟CRUISE（儀表板顯示CRUISE，未指定速度），並接上C2後，C2會自動接手", "../assets/offroad/icon_shell.png", Params().getBool("AutoEnable")) {
    QObject::connect(this, &AutoEnabledToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("AutoEnable", &value, 1);
    });
  }
};

class CruiseAutoResToggle : public ToggleControl {
  Q_OBJECT

public:
  CruiseAutoResToggle() : ToggleControl("自動恢復速度RES", "在巡航待機模式下(儀表板顯示CRUISE，且不得按下取消) 鬆開剎車踏板後，將會自動恢復指定速度。當前與設定速度超過30km/h，並偵測到前車時啟動此功能", "../assets/offroad/icon_shell.png", Params().getBool("CruiseAutoRes")) {
    QObject::connect(this, &CruiseAutoResToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("CruiseAutoRes", &value, 1);
    });
  }
};

class BatteryChargingControlToggle : public ToggleControl {
  Q_OBJECT

public:
  BatteryChargingControlToggle() : ToggleControl("啟用電池控制", "啟用電池控制", "../assets/offroad/icon_shell.png", Params().getBool("OpkrBatteryChargingControl")) {
    QObject::connect(this, &BatteryChargingControlToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrBatteryChargingControl", &value, 1);
    });
  }
};

class BlindSpotDetectToggle : public ToggleControl {
  Q_OBJECT

public:
  BlindSpotDetectToggle() : ToggleControl("顯示盲點偵測圖標", "若偵測到後方來車，將顯示圖標", "../assets/offroad/icon_shell.png", Params().getBool("OpkrBlindSpotDetect")) {
    QObject::connect(this, &BlindSpotDetectToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrBlindSpotDetect", &value, 1);
    });
  }
};

class SteerWindDownToggle : public ToggleControl {
  Q_OBJECT

public:
  SteerWindDownToggle() : ToggleControl("Steer Wind Down", "顯示Steer Warning警告時將會逐漸減少轉向角度。某些車種會出現方向盤轉向角度限制，請關閉此功能以使用最大轉向角度", "../assets/offroad/icon_shell.png", Params().getBool("SteerWindDown")) {
    QObject::connect(this, &SteerWindDownToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("SteerWindDown", &value, 1);
    });
  }
};

class AutoScreenDimmingToggle : public ToggleControl {
  Q_OBJECT

public:
  AutoScreenDimmingToggle() : ToggleControl("駕駛時螢幕亮度控制", "在駕駛時保持最低亮度以保障駕駛安全及池消耗及溫度，在突發事件時增加亮度", "../assets/offroad/icon_shell.png", Params().getBool("OpkrAutoScreenDimming")) {
    QObject::connect(this, &AutoScreenDimmingToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrAutoScreenDimming", &value, 1);
    });
  }
};

class LiveSteerRatioToggle : public ToggleControl {
  Q_OBJECT

public:
  LiveSteerRatioToggle() : ToggleControl("使用Live SteerRatio", "使用ive SteerRatio以取代可變/固定SR", "../assets/offroad/icon_shell.png", Params().getBool("OpkrLiveSteerRatio")) {
    QObject::connect(this, &LiveSteerRatioToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrLiveSteerRatio", &value, 1);
    });
  }
};

class VariableSteerMaxToggle : public ToggleControl {
  Q_OBJECT

public:
  VariableSteerMaxToggle() : ToggleControl("使用可變SteerMax", "根據彎道曲度使用可變SteerMax", "../assets/offroad/icon_shell.png", Params().getBool("OpkrVariableSteerMax")) {
    QObject::connect(this, &VariableSteerMaxToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrVariableSteerMax", &value, 1);
    });
  }
};

class VariableSteerDeltaToggle : public ToggleControl {
  Q_OBJECT

public:
  VariableSteerDeltaToggle() : ToggleControl("使用可變SteerDelta", "根據彎道曲度使用可變SteerDelta( 更改DeltaUp ~ 5, 更改DeltaDown ~ 10", "../assets/offroad/icon_shell.png", Params().getBool("OpkrVariableSteerDelta")) {
    QObject::connect(this, &VariableSteerDeltaToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrVariableSteerDelta", &value, 1);
    });
  }
};

class LiveTuneToggle : public ToggleControl {
  Q_OBJECT

public:
  LiveTuneToggle() : ToggleControl("即時更新", "開啟後修改值後將會即時更新", "../assets/offroad/icon_shell.png", Params().getBool("OpkrLiveTune")) {
    QObject::connect(this, &LiveTuneToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrLiveTune", &value, 1);
    });
  }
};

class ShaneFeedForward : public ToggleControl {
  Q_OBJECT

public:
  ShaneFeedForward() : ToggleControl("使用Shane FeedForward", "開啟Shane FeedForward後，直線時扭距值會降低，轉彎時扭距會動態調整", "../assets/offroad/icon_shell.png", Params().getBool("ShaneFeedForward")) {
    QObject::connect(this, &ShaneFeedForward::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("ShaneFeedForward", &value, 1);
    });
  }
};

class DrivingRecordToggle : public ToggleControl {
  Q_OBJECT

public:
  DrivingRecordToggle() : ToggleControl("自動錄影功能", "在執行期間會自動開啟/關閉錄影，車輛起步時自動錄影，停止時關閉錄影", "../assets/offroad/icon_shell.png", Params().getBool("OpkrDrivingRecord")) {
    QObject::connect(this, &DrivingRecordToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrDrivingRecord", &value, 1);
    });
  }
};

class TurnSteeringDisableToggle : public ToggleControl {
  Q_OBJECT

public:
  TurnSteeringDisableToggle() : ToggleControl("打方向燈時解除自動變換車道", "低速時將解除自動變換車道功能", "../assets/offroad/icon_shell.png", Params().getBool("OpkrTurnSteeringDisable")) {
    QObject::connect(this, &TurnSteeringDisableToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrTurnSteeringDisable", &value, 1);
    });
  }
};

class HotspotOnBootToggle : public ToggleControl {
  Q_OBJECT

public:
  HotspotOnBootToggle() : ToggleControl("自動啟動網路熱點", "發動時自動啟動網路熱點", "../assets/offroad/icon_shell.png", Params().getBool("OpkrHotspotOnBoot")) {
    QObject::connect(this, &HotspotOnBootToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("OpkrHotspotOnBoot", &value, 1);
    });
  }
};

class CruiseOverMaxSpeedToggle : public ToggleControl {
  Q_OBJECT

public:
  CruiseOverMaxSpeedToggle() : ToggleControl("設定速度同步超過的速度", "若速度大於設定的速度，會同步實際速度", "../assets/offroad/icon_shell.png", Params().getBool("CruiseOverMaxSpeed")) {
    QObject::connect(this, &CruiseOverMaxSpeedToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("CruiseOverMaxSpeed", &value, 1);
    });
  }
};

class DebugUiOneToggle : public ToggleControl {
  Q_OBJECT

public:
  DebugUiOneToggle() : ToggleControl("DEBUG UI 1", "", "../assets/offroad/icon_shell.png", Params().getBool("DebugUi1")) {
    QObject::connect(this, &DebugUiOneToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("DebugUi1", &value, 1);
    });
  }
};

class DebugUiTwoToggle : public ToggleControl {
  Q_OBJECT

public:
  DebugUiTwoToggle() : ToggleControl("DEBUG UI 2", "", "../assets/offroad/icon_shell.png", Params().getBool("DebugUi2")) {
    QObject::connect(this, &DebugUiTwoToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("DebugUi2", &value, 1);
    });
  }
};

class LongLogToggle : public ToggleControl {
  Q_OBJECT

public:
  LongLogToggle() : ToggleControl("LONG LOG顯示", "將長時間的測試數據顯示在螢幕上，而非log檔內", "../assets/offroad/icon_shell.png", Params().getBool("LongLogDisplay")) {
    QObject::connect(this, &LongLogToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("LongLogDisplay", &value, 1);
    });
  }
};

class PrebuiltToggle : public ToggleControl {
  Q_OBJECT

public:
  PrebuiltToggle() : ToggleControl("關閉Prebuilt", "若您修改了UI，請關閉自動重建UI檔案", "../assets/offroad/icon_shell.png", Params().getBool("PutPrebuiltOn")) {
    QObject::connect(this, &PrebuiltToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("PutPrebuiltOn", &value, 1);
    });
  }
};

class LDWSToggle : public ToggleControl {
  Q_OBJECT

public:
  LDWSToggle() : ToggleControl("LDWS車輛設定", "", "../assets/offroad/icon_shell.png", Params().getBool("LdwsCarFix")) {
    QObject::connect(this, &LDWSToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("LdwsCarFix", &value, 1);
    });
  }
};

class FPTwoToggle : public ToggleControl {
  Q_OBJECT

public:
  FPTwoToggle() : ToggleControl("使用FingerPrint 2.0", "透過fp2辨識ECU", "../assets/offroad/icon_shell.png", Params().getBool("FingerprintTwoSet")) {
    QObject::connect(this, &FPTwoToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("FingerprintTwoSet", &value, 1);
    });
  }
};

class GearDToggle : public ToggleControl {
  Q_OBJECT

public:
  GearDToggle() : ToggleControl("驅動方式辨識", "由於驅動方式導致車輛無法辨識的情況下使用，從根本上來說必須要解析CAN才能完全解決，但透過此方法能暫時解決", "../assets/offroad/icon_shell.png", Params().getBool("JustDoGearD")) {
    QObject::connect(this, &GearDToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("JustDoGearD", &value, 1);
    });
  }
};

class ComIssueToggle : public ToggleControl {
  Q_OBJECT

public:
  ComIssueToggle() : ToggleControl("關閉通訊錯誤警報", "關閉PANDA通訊錯誤時的警報", "../assets/offroad/icon_shell.png", Params().getBool("ComIssueGone")) {
    QObject::connect(this, &ComIssueToggle::toggleFlipped, [=](int state) {
      char value = state ? '1' : '0';
      Params().put("ComIssueGone", &value, 1);
    });
  }
};

// SSH key management widget
class SshControl : public AbstractControl {
  Q_OBJECT

public:
  SshControl();

private:
  QPushButton btn;
  QString username;
  QLabel username_label;

  // networking
  QTimer* networkTimer;
  QNetworkReply* reply;
  QNetworkAccessManager* manager;

  void refresh();
  void getUserKeys(QString username);

signals:
  void failedResponse(QString errorString);

private slots:
  void timeout();
  void parseResponse();
};

// 오픈파일럿 미리보기
class OpenpilotView : public AbstractControl {
  Q_OBJECT

public:
  OpenpilotView();

private:
  QPushButton btn;

  void refresh();
};

// 차량 강제등록
class CarRecognition : public AbstractControl {
  Q_OBJECT

public:
  CarRecognition();

private:
  QPushButton btn;
  QString carname;
  QLabel carname_label;

  void refresh(QString carname);
};

class CarForceSet : public AbstractControl {
  Q_OBJECT

public:
  CarForceSet();

private:
  QPushButton btnc;
  QString carname;
  //QLabel carname_label;

  void refreshc();
};


// UI 설정
class AutoShutdown : public AbstractControl {
  Q_OBJECT

public:
  AutoShutdown();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class AutoScreenOff : public AbstractControl {
  Q_OBJECT

public:
  AutoScreenOff();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class VolumeControl : public AbstractControl {
  Q_OBJECT

public:
  VolumeControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class BrightnessControl : public AbstractControl {
  Q_OBJECT

public:
  BrightnessControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class ChargingMin : public AbstractControl {
  Q_OBJECT

public:
  ChargingMin();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};
class ChargingMax : public AbstractControl {
  Q_OBJECT

public:
  ChargingMax();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};


// 주행 설정
class CruisemodeSelInit : public AbstractControl {
  Q_OBJECT

public:
  CruisemodeSelInit();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class VariableCruiseProfile : public AbstractControl {
  Q_OBJECT

public:
  VariableCruiseProfile();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class LaneChangeSpeed : public AbstractControl {
  Q_OBJECT

public:
  LaneChangeSpeed();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class LaneChangeDelay : public AbstractControl {
  Q_OBJECT

public:
  LaneChangeDelay();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class LeftCurvOffset : public AbstractControl {
  Q_OBJECT

public:
  LeftCurvOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};
class RightCurvOffset : public AbstractControl {
  Q_OBJECT

public:
  RightCurvOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class MaxAngleLimit : public AbstractControl {
  Q_OBJECT

public:
  MaxAngleLimit();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SpeedLimitOffset : public AbstractControl {
  Q_OBJECT

public:
  SpeedLimitOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

// 튜닝 설정
class CameraOffset : public AbstractControl {
  Q_OBJECT

public:
  CameraOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SRBaseControl : public AbstractControl {
  Q_OBJECT

public:
  SRBaseControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};
class SRMaxControl : public AbstractControl {
  Q_OBJECT

public:
  SRMaxControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerActuatorDelay : public AbstractControl {
  Q_OBJECT

public:
  SteerActuatorDelay();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerRateCost : public AbstractControl {
  Q_OBJECT

public:
  SteerRateCost();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerLimitTimer : public AbstractControl {
  Q_OBJECT

public:
  SteerLimitTimer();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class TireStiffnessFactor : public AbstractControl {
  Q_OBJECT

public:
  TireStiffnessFactor();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerMaxBase : public AbstractControl {
  Q_OBJECT

public:
  SteerMaxBase();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerMaxMax : public AbstractControl {
  Q_OBJECT

public:
  SteerMaxMax();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerMaxv : public AbstractControl {
  Q_OBJECT

public:
  SteerMaxv();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerDeltaUpBase : public AbstractControl {
  Q_OBJECT

public:
  SteerDeltaUpBase();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerDeltaUpMax : public AbstractControl {
  Q_OBJECT

public:
  SteerDeltaUpMax();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerDeltaDownBase : public AbstractControl {
  Q_OBJECT

public:
  SteerDeltaDownBase();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerDeltaDownMax : public AbstractControl {
  Q_OBJECT

public:
  SteerDeltaDownMax();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};


// 제어 설정
class LateralControl : public AbstractControl {
  Q_OBJECT

public:
  LateralControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class PidKp : public AbstractControl {
  Q_OBJECT

public:
  PidKp();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class PidKi : public AbstractControl {
  Q_OBJECT

public:
  PidKi();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class PidKd : public AbstractControl {
  Q_OBJECT

public:
  PidKd();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class PidKf : public AbstractControl {
  Q_OBJECT

public:
  PidKf();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class IgnoreZone : public AbstractControl {
  Q_OBJECT

public:
  IgnoreZone();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class OuterLoopGain : public AbstractControl {
  Q_OBJECT

public:
  OuterLoopGain();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class InnerLoopGain : public AbstractControl {
  Q_OBJECT

public:
  InnerLoopGain();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class TimeConstant : public AbstractControl {
  Q_OBJECT

public:
  TimeConstant();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class ActuatorEffectiveness : public AbstractControl {
  Q_OBJECT

public:
  ActuatorEffectiveness();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class Scale : public AbstractControl {
  Q_OBJECT

public:
  Scale();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class LqrKi : public AbstractControl {
  Q_OBJECT

public:
  LqrKi();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class DcGain : public AbstractControl {
  Q_OBJECT

public:
  DcGain();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerAngleCorrection : public AbstractControl {
  Q_OBJECT

public:
  SteerAngleCorrection();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class MaxSteer : public AbstractControl {
  Q_OBJECT

public:
  MaxSteer();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class MaxRTDelta : public AbstractControl {
  Q_OBJECT

public:
  MaxRTDelta();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class MaxRateUp : public AbstractControl {
  Q_OBJECT

public:
  MaxRateUp();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class MaxRateDown : public AbstractControl {
  Q_OBJECT

public:
  MaxRateDown();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class SteerThreshold : public AbstractControl {
  Q_OBJECT

public:
  SteerThreshold();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class RecordCount : public AbstractControl {
  Q_OBJECT

public:
  RecordCount();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class RecordQuality : public AbstractControl {
  Q_OBJECT

public:
  RecordQuality();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class GitHash : public AbstractControl {
  Q_OBJECT

public:
  GitHash();

private:
  QLabel local_hash;
  QLabel remote_hash;
};

class RESChoice : public AbstractControl {
  Q_OBJECT

public:
  RESChoice();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class MonitoringMode : public AbstractControl {
  Q_OBJECT

public:
  MonitoringMode();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class MonitorEyesThreshold : public AbstractControl {
  Q_OBJECT

public:
  MonitorEyesThreshold();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class NormalEyesThreshold : public AbstractControl {
  Q_OBJECT

public:
  NormalEyesThreshold();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class BlinkThreshold : public AbstractControl {
  Q_OBJECT

public:
  BlinkThreshold();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};

class FanSpeedGain : public AbstractControl {
  Q_OBJECT

public:
  FanSpeedGain();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};