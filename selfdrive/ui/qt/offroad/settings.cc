#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <QProcess>

#ifndef QCOM
#include "networking.hpp"
#endif
#include "settings.hpp"
#include "widgets/input.hpp"
#include "widgets/toggle.hpp"
#include "widgets/offroad_alerts.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/controls.hpp"
#include "widgets/ssh_keys.hpp"
#include "common/params.h"
#include "common/util.h"
#include "selfdrive/hardware/hw.h"
#include "ui.hpp"

TogglesPanel::TogglesPanel(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *toggles_list = new QVBoxLayout();

  QList<ParamControl*> toggles;

  toggles.append(new ParamControl("OpenpilotEnabledToggle",
                                  "啟用OPENPILOT",
                                  "어댑티브 크루즈 컨트롤 및 차선 유지 지원을 위해 오픈파일럿 시스템을 사용하십시오. 이 기능을 사용하려면 항상 주의를 기울여야 합니다. 이 설정을 변경하는 것은 자동차의 전원이 꺼졌을 때 적용됩니다.",
                                  "../assets/offroad/icon_openpilot.png",
                                  this));
  toggles.append(new ParamControl("IsLdwEnabled",
                                  "車道偏離警告",
                                  "50km/h이상의 속도로 주행하는 동안 방향 지시등이 활성화되지 않은 상태에서 차량이 감지된 차선 위를 넘어갈 경우 원래 차선으로 다시 방향을 전환하도록 경고를 보냅니다.",
                                  "../assets/offroad/icon_warning.png",
                                  this));
  toggles.append(new ParamControl("IsRHD",
                                  "啟用右駕",
                                  "오픈파일럿이 좌측 교통 규칙을 준수하도록 허용하고 우측 운전석에서 운전자 모니터링을 수행하십시오.",
                                  "../assets/offroad/icon_openpilot_mirrored.png",
                                  this));
  toggles.append(new ParamControl("IsMetric",
                                  "使用公制",
                                  "mi/h 대신 km/h 단위로 속도를 표시합니다.",
                                  "../assets/offroad/icon_metric.png",
                                  this));
  toggles.append(new ParamControl("CommunityFeaturesToggle",
                                  "啟用社區功能",
                                  "comma.ai에서 유지 또는 지원하지 않고 표준 안전 모델에 부합하는 것으로 확인되지 않은 오픈 소스 커뮤니티의 기능을 사용하십시오. 이러한 기능에는 커뮤니티 지원 자동차와 커뮤니티 지원 하드웨어가 포함됩니다. 이러한 기능을 사용할 때는 각별히 주의해야 합니다.",
                                  "../assets/offroad/icon_shell.png",
                                  this));

#ifdef QCOM2
  toggles.append(new ParamControl("IsUploadRawEnabled",
                                 "Upload Raw Logs",
                                 "Upload full logs and full resolution video by default while on WiFi. If not enabled, individual logs can be marked for upload at my.comma.ai/useradmin.",
                                 "../assets/offroad/icon_network.png",
                                 this));
#endif

  ParamControl *record_toggle = new ParamControl("RecordFront",
                                                 "上傳錄影",
                                                 "운전자 모니터링 카메라에서 데이터를 업로드하고 운전자 모니터링 알고리즘을 개선하십시오.",
                                                 "../assets/offroad/icon_network.png",
                                                 this);
  toggles.append(record_toggle);
  toggles.append(new ParamControl("EndToEndToggle",
                                   "不使用車道辨識模式 (alpha)",
                                   "이 모드에서 오픈파일럿은 차선을 따라 주행하지 않고 사람이 운전하는 것 처럼 주행합니다.",
                                   "../assets/offroad/icon_road.png",
                                   this));

#ifdef QCOM2
  toggles.append(new ParamControl("EnableWideCamera",
                                  "Enable use of Wide Angle Camera",
                                  "Use wide angle camera for driving and ui. Only takes effect after reboot.",
                                  "../assets/offroad/icon_openpilot.png",
                                  this));
  toggles.append(new ParamControl("EnableLteOnroad",
                                  "Enable LTE while onroad",
                                  "",
                                  "../assets/offroad/icon_network.png",
                                  this));

#endif
  toggles.append(new ParamControl("OpkrEnableDriverMonitoring",
                                  "啟用駕駛監控",
                                  "운전자 감시 모니터링을 사용합니다.",
                                  "../assets/offroad/icon_shell.png",
                                  this));
  toggles.append(new ParamControl("OpkrEnableLogger",
                                  "啟用記錄與上傳",
                                  "주행로그를 기록 후 콤마서버로 전송합니다.",
                                  "../assets/offroad/icon_shell.png",
                                  this));
  toggles.append(new ParamControl("OpenpilotLongitudinalControl",
                                  "啟用縱向控制",
                                  "스톡+비전 기반의 롱컨트롤을 활성화 합니다. 관련 작업이 완료된 경우에만 활성화 하십시오.",
                                  "../assets/offroad/icon_shell.png",
                                  this));
  toggles.append(new ParamControl("MadModeEnabled",
                                  "開啟暗黑模式",
                                  "크루즈 MainSW를 이용하여 오파를 활성화 합니다.",
                                  "../assets/offroad/icon_shell.png",
                                  this));
  toggles.append(new ParamControl("CommaStockUI",
                                  "啟用Comma Stock UI",
                                  "주행화면을 콤마의 순정 UI를 사용합니다.",
                                  "../assets/offroad/icon_shell.png",
                                  this));

  bool record_lock = Params().getBool("RecordFrontLock");
  record_toggle->setEnabled(!record_lock);

  for(ParamControl *toggle : toggles){
    if(toggles_list->count() != 0){
      toggles_list->addWidget(horizontal_line());
    }
    toggles_list->addWidget(toggle);
  }

  setLayout(toggles_list);
}

DevicePanel::DevicePanel(QWidget* parent) : QWidget(parent) {
  QVBoxLayout *device_layout = new QVBoxLayout;

  Params params = Params();

  QString dongle = QString::fromStdString(params.get("DongleId", false));
  device_layout->addWidget(new LabelControl("Dongle ID", dongle));
  device_layout->addWidget(horizontal_line());

  QString serial = QString::fromStdString(params.get("HardwareSerial", false));
  device_layout->addWidget(new LabelControl("Serial", serial));

  // offroad-only buttons
  QList<ButtonControl*> offroad_btns;

  offroad_btns.append(new ButtonControl("駕駛鏡頭", "預覽",
                                        "운전자 모니터링 카메라를 미리 보고 장치 장착 위치를 최적화하여 최상의 운전자 모니터링 환경을 제공하십시오. (차량이 꺼져 있어야 합니다.)",
                                        [=]() {
                                           Params().putBool("IsDriverViewEnabled", true);
                                           QUIState::ui_state.scene.driver_view = true;
                                        }, "", this));

  QString resetCalibDesc = "오픈파일럿을 사용하려면 장치를 왼쪽 또는 오른쪽으로 4°, 위 또는 아래로 5° 이내에 장착해야 합니다. 오픈파일럿이 지속적으로 보정되고 있으므로 재설정할 필요가 거의 없습니다.";
  ButtonControl *resetCalibBtn = new ButtonControl("C2重新校正", "校正", resetCalibDesc, [=]() {
    QString desc = "[基準: L/R 4°度 UP/DN 5°以內]";
    std::string calib_bytes = Params().get("CalibrationParams");
    if (!calib_bytes.empty()) {
      try {
        AlignedBuffer aligned_buf;
        capnp::FlatArrayMessageReader cmsg(aligned_buf.align(calib_bytes.data(), calib_bytes.size()));
        auto calib = cmsg.getRoot<cereal::Event>().getLiveCalibration();
        if (calib.getCalStatus() != 0) {
          double pitch = calib.getRpyCalib()[1] * (180 / M_PI);
          double yaw = calib.getRpyCalib()[2] * (180 / M_PI);
          desc += QString("\n장치가 %1° %2 그리고 %3° %4 위치해 있습니다.")
                                .arg(QString::number(std::abs(pitch), 'g', 1), pitch > 0 ? "위로" : "아래로",
                                     QString::number(std::abs(yaw), 'g', 1), yaw > 0 ? "오른쪽으로" : "왼쪽으로");
        }
      } catch (kj::Exception) {
        qInfo() << "캘리브레이션 파라미터 유효하지 않음";
      }
    }
    if (ConfirmationDialog::confirm(desc)) {
      //Params().remove("CalibrationParams");
    }
  }, "", this);
  connect(resetCalibBtn, &ButtonControl::showDescription, [=]() {
    QString desc = resetCalibDesc;
    std::string calib_bytes = Params().get("CalibrationParams");
    if (!calib_bytes.empty()) {
      try {
        AlignedBuffer aligned_buf;
        capnp::FlatArrayMessageReader cmsg(aligned_buf.align(calib_bytes.data(), calib_bytes.size()));
        auto calib = cmsg.getRoot<cereal::Event>().getLiveCalibration();
        if (calib.getCalStatus() != 0) {
          double pitch = calib.getRpyCalib()[1] * (180 / M_PI);
          double yaw = calib.getRpyCalib()[2] * (180 / M_PI);
          desc += QString("\n장치가 %1° %2 그리고 %3° %4 위치해 있습니다.")
                                .arg(QString::number(std::abs(pitch), 'g', 1), pitch > 0 ? "위로" : "아래로",
                                     QString::number(std::abs(yaw), 'g', 1), yaw > 0 ? "오른쪽으로" : "왼쪽으로");
        }
      } catch (kj::Exception) {
        qInfo() << "캘리브레이션 파라미터 유효하지 않음";
      }
    }
    resetCalibBtn->setDescription(desc);
  });
  offroad_btns.append(resetCalibBtn);

  offroad_btns.append(new ButtonControl("重看訓練指南", "查看",
                                        "오픈파일럿에 대한 규칙, 기능, 제한내용 등을 확인하세요.", [=]() {
    if (ConfirmationDialog::confirm("您想查看訓練指南嗎？")) {
      Params().remove("CompletedTrainingVersion");
      emit reviewTrainingGuide();
    }
  }, "", this));

  QString brand = params.getBool("Passive") ? "行車記錄器模式" : "OPENPILOT";
  offroad_btns.append(new ButtonControl(brand + " 제거", "제거", "", [=]() {
    if (ConfirmationDialog::confirm("제거하시겠습니까?")) {
      Params().putBool("DoUninstall", true);
    }
  }, "", this));

  for(auto &btn : offroad_btns){
    device_layout->addWidget(horizontal_line());
    QObject::connect(parent, SIGNAL(offroadTransition(bool)), btn, SLOT(setEnabled(bool)));
    device_layout->addWidget(btn);
  }

  device_layout->addWidget(horizontal_line());

  // cal reset and param init buttons
  QHBoxLayout *cal_param_init_layout = new QHBoxLayout();
  cal_param_init_layout->setSpacing(50);

  QPushButton *calinit_btn = new QPushButton("校準重置");
  cal_param_init_layout->addWidget(calinit_btn);
  QObject::connect(calinit_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("確定要重置嗎？將重新啟動", this)) {
      Params().remove("CalibrationParams");
      Params().remove("LiveParameters");
      QTimer::singleShot(1000, []() {
        Hardware::reboot();
      });
    }
  });

  QPushButton *paraminit_btn = new QPushButton("參數初始化");
  cal_param_init_layout->addWidget(paraminit_btn);
  QObject::connect(paraminit_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("確定將參數恢復為初始狀態嗎？", this)) {
      QProcess::execute("/data/openpilot/init_param.sh");
    }
  });

  // preset1 buttons
  QHBoxLayout *presetone_layout = new QHBoxLayout();
  presetone_layout->setSpacing(50);

  QPushButton *presetoneload_btn = new QPushButton("加載預設1");
  presetone_layout->addWidget(presetoneload_btn);
  QObject::connect(presetoneload_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("確定加載預設1嗎？", this)) {
      QProcess::execute("/data/openpilot/load_preset1.sh");
    }
  });

  QPushButton *presetonesave_btn = new QPushButton("保存預設1");
  presetone_layout->addWidget(presetonesave_btn);
  QObject::connect(presetonesave_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("確定要保存預設1嗎？", this)) {
      QProcess::execute("/data/openpilot/save_preset1.sh");
    }
  });

  // preset2 buttons
  QHBoxLayout *presettwo_layout = new QHBoxLayout();
  presettwo_layout->setSpacing(50);

  QPushButton *presettwoload_btn = new QPushButton("加載預設2");
  presettwo_layout->addWidget(presettwoload_btn);
  QObject::connect(presettwoload_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("確定要加載預設2嗎？", this)) {
      QProcess::execute("/data/openpilot/load_preset2.sh");
    }
  });

  QPushButton *presettwosave_btn = new QPushButton("保存預設2");
  presettwo_layout->addWidget(presettwosave_btn);
  QObject::connect(presettwosave_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("確定要保存預設2嗎", this)) {
      QProcess::execute("/data/openpilot/save_preset2.sh");
    }
  });

  // power buttons
  QHBoxLayout *power_layout = new QHBoxLayout();
  power_layout->setSpacing(50);

  QPushButton *reboot_btn = new QPushButton("重新開機");
  power_layout->addWidget(reboot_btn);
  QObject::connect(reboot_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("確定要重新開機嗎？", this)) {
      Hardware::reboot();
    }
  });

  QPushButton *poweroff_btn = new QPushButton("關機");
  poweroff_btn->setStyleSheet("background-color: #E22C2C;");
  power_layout->addWidget(poweroff_btn);
  QObject::connect(poweroff_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("確定要關機嗎？", this)) {
      Hardware::poweroff();
    }
  });

  device_layout->addLayout(cal_param_init_layout);

  device_layout->addWidget(horizontal_line());

  device_layout->addLayout(presetone_layout);
  device_layout->addLayout(presettwo_layout);

  device_layout->addWidget(horizontal_line());

  device_layout->addLayout(power_layout);

  setLayout(device_layout);
  setStyleSheet(R"(
    QPushButton {
      padding: 20;
      height: 120px;
      border-radius: 15px;
      background-color: #393939;
    }
  )");
}

DeveloperPanel::DeveloperPanel(QWidget* parent) : QFrame(parent) {
  QVBoxLayout *main_layout = new QVBoxLayout(this);
  setLayout(main_layout);
  setStyleSheet(R"(QLabel {font-size: 50px;})");
}

void DeveloperPanel::showEvent(QShowEvent *event) {
  Params params = Params();
  std::string brand = params.getBool("Passive") ? "行車記錄器" : "OPENPILOT";
  QList<QPair<QString, std::string>> dev_params = {
    {"Version", brand + " v" + params.get("Version", false).substr(0, 14)},
    {"Git Branch", params.get("GitBranch", false)},
    {"Git Commit", params.get("GitCommit", false).substr(0, 10)},
    {"Panda Firmware", params.get("PandaFirmwareHex", false)},
    {"OS Version", Hardware::get_os_version()},
  };

  for (int i = 0; i < dev_params.size(); i++) {
    const auto &[name, value] = dev_params[i];
    QString val = QString::fromStdString(value).trimmed();
    if (labels.size() > i) {
      labels[i]->setText(val);
    } else {
      labels.push_back(new LabelControl(name, val));
      layout()->addWidget(labels[i]);
      if (i < (dev_params.size() - 1)) {
        layout()->addWidget(horizontal_line());
      }
    }
  }
}

QWidget * network_panel(QWidget * parent) {
#ifdef QCOM
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setSpacing(30);

  layout->addWidget(new OpenpilotView());
  layout->addWidget(horizontal_line());
  // wifi + tethering buttons
  layout->addWidget(new ButtonControl("WiFi設定", "設定", "",
                                      [=]() { HardwareEon::launch_wifi(); }));
  layout->addWidget(horizontal_line());

  layout->addWidget(new ButtonControl("網路分享設定", "設定", "",
                                      [=]() { HardwareEon::launch_tethering(); }));
  layout->addWidget(horizontal_line());

  // SSH key management
  layout->addWidget(new SshToggle());
  layout->addWidget(horizontal_line());
  layout->addWidget(new SshControl());
  layout->addWidget(horizontal_line());
  layout->addWidget(new SshLegacyToggle());
  layout->addWidget(horizontal_line());

  layout->addWidget(new GitHash());
  const char* gitpull = "/data/openpilot/gitpull.sh ''";
  layout->addWidget(new ButtonControl("Git Pull", "執行", "會將遠端GIT複製到本機並覆蓋檔案，並自動重新開機",
                                      [=]() { 
                                        if (ConfirmationDialog::confirm("Git에서 변경사항 적용 후 자동 재부팅 됩니다. 없으면 재부팅하지 않습니다. 진행하시겠습니까?")){
                                          std::system(gitpull);
                                        }
                                      }));

  layout->addWidget(horizontal_line());

  const char* git_reset = "/data/openpilot/git_reset.sh ''";
  layout->addWidget(new ButtonControl("Git Reset", "執行", "重置GIT，強制退回遠端GIT上一個版本",
                                      [=]() { 
                                        if (ConfirmationDialog::confirm("로컬변경사항을 강제 초기화 후 리모트Git의 최신 커밋내역을 적용합니다. 진행하시겠습니까?")){
                                          std::system(git_reset);
                                        }
                                      }));


  layout->addWidget(horizontal_line());

  const char* gitpull_cancel = "/data/openpilot/gitpull_cancel.sh ''";
  layout->addWidget(new ButtonControl("取消Git Pull", "執行", "恢復到pull前的狀態，若有多次pull記錄，將回到上一個pull的狀態",
                                      [=]() { 
                                        if (ConfirmationDialog::confirm("GitPull 이전 상태로 되돌립니다. 진행하시겠습니까?")){
                                          std::system(gitpull_cancel);
                                        }
                                      }));

  layout->addWidget(horizontal_line());

  const char* panda_flashing = "/data/openpilot/panda_flashing.sh ''";
  layout->addWidget(new ButtonControl("重刷PANDA", "執行", "執行後C2後面綠燈會不斷閃爍，重刷完成後會自動重新開機，期間請勿關機或重新開機",
                                      [=]() {
                                        if (ConfirmationDialog::confirm("確定要重刷PANDA嗎？")) {
                                          std::system(panda_flashing);
                                        }
                                      }));

  layout->addStretch(1);

  QWidget *w = new QWidget;
  w->setLayout(layout);
#else
  Networking *w = new Networking(parent);
#endif
  return w;
}

QWidget * user_panel(QWidget * parent) {
  QVBoxLayout *layout = new QVBoxLayout;

  // OPKR
  layout->addWidget(new LabelControl("UI設定", ""));
  layout->addWidget(new AutoShutdown());
  //layout->addWidget(new AutoScreenDimmingToggle());
  layout->addWidget(new AutoScreenOff());
  layout->addWidget(new VolumeControl());
  layout->addWidget(new BrightnessControl());
  layout->addWidget(new GetoffAlertToggle());
  layout->addWidget(new BatteryChargingControlToggle());
  layout->addWidget(new ChargingMin());
  layout->addWidget(new ChargingMax());
  layout->addWidget(new FanSpeedGain());
  layout->addWidget(new DrivingRecordToggle());
  layout->addWidget(new HotspotOnBootToggle());
  layout->addWidget(new RecordCount());
  layout->addWidget(new RecordQuality());
  const char* record_del = "rm -f /storage/emulated/0/videos/*";
  layout->addWidget(new ButtonControl("刪除所有記錄檔案", "執行", "刪除所有記錄檔案",
                                      [=]() { 
                                        if (ConfirmationDialog::confirm("你確定要刪除所有記錄檔案嗎")){
                                          std::system(record_del);
                                        }
                                      }));
  layout->addWidget(new MonitoringMode());
  layout->addWidget(new MonitorEyesThreshold());
  layout->addWidget(new NormalEyesThreshold());
  layout->addWidget(new BlinkThreshold());

  layout->addWidget(horizontal_line());
  layout->addWidget(new LabelControl("行駛設定", ""));
  layout->addWidget(new AutoResumeToggle());
  layout->addWidget(new VariableCruiseToggle());
  layout->addWidget(new VariableCruiseProfile());
  layout->addWidget(new CruisemodeSelInit());
  layout->addWidget(new LaneChangeSpeed());
  layout->addWidget(new LaneChangeDelay());
  layout->addWidget(new LeftCurvOffset());
  layout->addWidget(new RightCurvOffset());
  layout->addWidget(new BlindSpotDetectToggle());
  layout->addWidget(new MaxAngleLimit());
  layout->addWidget(new SteerAngleCorrection());
  layout->addWidget(new TurnSteeringDisableToggle());
  layout->addWidget(new CruiseOverMaxSpeedToggle());
  layout->addWidget(new SpeedLimitOffset());
  layout->addWidget(new CruiseGapAdjustToggle());
  layout->addWidget(new AutoEnabledToggle());
  layout->addWidget(new CruiseAutoResToggle());
  layout->addWidget(new RESChoice());
  layout->addWidget(new SteerWindDownToggle());

  layout->addWidget(horizontal_line());
  layout->addWidget(new LabelControl("開發者設定", ""));
  layout->addWidget(new DebugUiOneToggle());
  layout->addWidget(new DebugUiTwoToggle());
  layout->addWidget(new LongLogToggle());
  layout->addWidget(new PrebuiltToggle());
  layout->addWidget(new FPTwoToggle());
  layout->addWidget(new LDWSToggle());
  layout->addWidget(new GearDToggle());
  layout->addWidget(new ComIssueToggle());
  const char* cal_ok = "cp -f /data/openpilot/selfdrive/assets/addon/param/CalibrationParams /data/params/d/";
  layout->addWidget(new ButtonControl("強制校正", "執行", "실주행으로 캘리브레이션을 설정하지 않고 이온을 초기화 한경우 인게이지 확인용도로 캘리브레이션을 강제 설정합니다.",
                                      [=]() { 
                                        if (ConfirmationDialog::confirm("캘리브레이션을 강제로 설정합니다. 인게이지 확인용이니 실 주행시에는 초기화 하시기 바랍니다.")){
                                          std::system(cal_ok);
                                        }
                                      }));
  layout->addWidget(horizontal_line());
  layout->addWidget(new CarRecognition());
  //layout->addWidget(new CarForceSet());
  //QString car_model = QString::fromStdString(Params().get("CarModel", false));
  //layout->addWidget(new LabelControl("현재차량모델", ""));
  //layout->addWidget(new LabelControl(car_model, ""));

  layout->addWidget(horizontal_line());
  layout->addWidget(new LabelControl("PANDA值", "警告"));
  layout->addWidget(new MaxSteer());
  layout->addWidget(new MaxRTDelta());
  layout->addWidget(new MaxRateUp());
  layout->addWidget(new MaxRateDown());
  const char* p_edit_go = "/data/openpilot/p_edit.sh ''";
  layout->addWidget(new ButtonControl("PANDA值優化", "執行", "將PANDA優化為適當值",
                                      [=]() { 
                                        if (ConfirmationDialog::confirm("판다 값을 최적화 합니다. 조지 싸장님 쌀랑해요~")){
                                          std::system(p_edit_go);
                                        }
                                      }));
  const char* m_edit_go = "/data/openpilot/m_edit.sh ''";
  layout->addWidget(new ButtonControl("監控優化", "執行", "修改參數並讓夜間行駛或隧道間能確實辨認",
                                      [=]() { 
                                        if (ConfirmationDialog::confirm("야간감시 및 터널 모니터링을 최적화 합니다. 싸장님 좋아요. 콤마 쌀랑해요~")){
                                          std::system(m_edit_go);
                                        }
                                      }));
  //layout->addWidget(horizontal_line());

  layout->addStretch(1);

  QWidget *w = new QWidget;
  w->setLayout(layout);

  return w;
}

QWidget * tuning_panel(QWidget * parent) {
  QVBoxLayout *layout = new QVBoxLayout;

  // OPKR
  layout->addWidget(new LabelControl("調整目錄", ""));
  layout->addWidget(new CameraOffset());
  layout->addWidget(new LiveSteerRatioToggle());
  layout->addWidget(new SRBaseControl());
  layout->addWidget(new SRMaxControl());
  layout->addWidget(new SteerActuatorDelay());
  layout->addWidget(new SteerRateCost());
  layout->addWidget(new SteerLimitTimer());
  layout->addWidget(new TireStiffnessFactor());
  layout->addWidget(new SteerMaxBase());
  layout->addWidget(new SteerMaxMax());
  layout->addWidget(new SteerMaxv());
  layout->addWidget(new VariableSteerMaxToggle());
  layout->addWidget(new SteerDeltaUpBase());
  layout->addWidget(new SteerDeltaUpMax());
  layout->addWidget(new SteerDeltaDownBase());
  layout->addWidget(new SteerDeltaDownMax());
  layout->addWidget(new VariableSteerDeltaToggle());
  layout->addWidget(new SteerThreshold());

  layout->addWidget(horizontal_line());

  layout->addWidget(new LabelControl("控制項目", ""));
  layout->addWidget(new LateralControl());
  layout->addWidget(new LiveTuneToggle());
  QString lat_control = QString::fromStdString(Params().get("LateralControlMethod", false));
  if (lat_control == "0") {
    layout->addWidget(new PidKp());
    layout->addWidget(new PidKi());
    layout->addWidget(new PidKd());
    layout->addWidget(new PidKf());
    layout->addWidget(new IgnoreZone());
    layout->addWidget(new ShaneFeedForward());
  } else if (lat_control == "1") {
    layout->addWidget(new InnerLoopGain());
    layout->addWidget(new OuterLoopGain());
    layout->addWidget(new TimeConstant());
    layout->addWidget(new ActuatorEffectiveness());
  } else if (lat_control == "2") {
    layout->addWidget(new Scale());
    layout->addWidget(new LqrKi());
    layout->addWidget(new DcGain());
  }

  layout->addStretch(1);

  QWidget *w = new QWidget;
  w->setLayout(layout);

  return w;
}

SettingsWindow::SettingsWindow(QWidget *parent) : QFrame(parent) {
  // setup two main layouts
  QVBoxLayout *sidebar_layout = new QVBoxLayout();
  sidebar_layout->setMargin(0);
  panel_widget = new QStackedWidget();
  panel_widget->setStyleSheet(R"(
    border-radius: 30px;
    background-color: #292929;
  )");

  // close button
  QPushButton *close_btn = new QPushButton("關閉");
  close_btn->setStyleSheet(R"(
    font-size: 60px;
    font-weight: bold;
    border 1px grey solid;
    border-radius: 100px;
    background-color: #292929;
  )");
  close_btn->setFixedSize(200, 200);
  sidebar_layout->addSpacing(45);
  sidebar_layout->addWidget(close_btn, 0, Qt::AlignCenter);
  QObject::connect(close_btn, &QPushButton::released, this, &SettingsWindow::closeSettings);

  // setup panels
  DevicePanel *device = new DevicePanel(this);
  QObject::connect(device, &DevicePanel::reviewTrainingGuide, this, &SettingsWindow::reviewTrainingGuide);

  QPair<QString, QWidget *> panels[] = {
    {"本機", device},
    {"網路", network_panel(this)},
    {"切換功能", new TogglesPanel(this)},
    {"版本", new DeveloperPanel()},
    {"使用者設定", user_panel(this)},
    {"調整", tuning_panel(this)},
  };

  sidebar_layout->addSpacing(45);
  nav_btns = new QButtonGroup();
  for (auto &[name, panel] : panels) {
    QPushButton *btn = new QPushButton(name);
    btn->setCheckable(true);
    btn->setStyleSheet(R"(
      QPushButton {
        color: grey;
        border: none;
        background: none;
        font-size: 65px;
        font-weight: 500;
        padding-top: 18px;
        padding-bottom: 18px;
      }
      QPushButton:checked {
        color: white;
      }
    )");

    nav_btns->addButton(btn);
    sidebar_layout->addWidget(btn, 0, Qt::AlignRight);

    panel->setContentsMargins(50, 25, 50, 25);

    ScrollView *panel_frame = new ScrollView(panel, this);
    panel_widget->addWidget(panel_frame);

    QObject::connect(btn, &QPushButton::released, [=, w = panel_frame]() {
      panel_widget->setCurrentWidget(w);
    });
  }
  sidebar_layout->setContentsMargins(50, 50, 100, 50);

  // main settings layout, sidebar + main panel
  QHBoxLayout *settings_layout = new QHBoxLayout();

  sidebar_widget = new QWidget;
  sidebar_widget->setLayout(sidebar_layout);
  sidebar_widget->setFixedWidth(500);
  settings_layout->addWidget(sidebar_widget);
  settings_layout->addWidget(panel_widget);

  setLayout(settings_layout);
  setStyleSheet(R"(
    * {
      color: white;
      font-size: 50px;
    }
    SettingsWindow {
      background-color: black;
    }
  )");
}

void SettingsWindow::hideEvent(QHideEvent *event){
#ifdef QCOM
  HardwareEon::close_activities();
#endif

  // TODO: this should be handled by the Dialog classes
  QList<QWidget*> children = findChildren<QWidget *>();
  for(auto &w : children){
    if(w->metaObject()->superClass()->className() == QString("QDialog")){
      w->close();
    }
  }
}

void SettingsWindow::showEvent(QShowEvent *event){
  panel_widget->setCurrentIndex(0);
  nav_btns->buttons()[0]->setChecked(true);
}

