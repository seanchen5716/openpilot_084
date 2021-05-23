#include <QDateTime>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QProcess>

#include "common/util.h"
#include "common/params.h"
#include "common/timing.h"
#include "common/swaglog.h"

#include "home.hpp"
#include "widgets/drive_stats.hpp"
#include "widgets/setup.hpp"

// HomeWindow: the container for the offroad and onroad UIs

HomeWindow::HomeWindow(QWidget* parent) : QWidget(parent) {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);

  sidebar = new Sidebar(this);
  layout->addWidget(sidebar);
  QObject::connect(this, &HomeWindow::update, sidebar, &Sidebar::update);
  QObject::connect(sidebar, &Sidebar::openSettings, this, &HomeWindow::openSettings);

  slayout = new QStackedLayout();
  layout->addLayout(slayout);

  onroad = new OnroadWindow(this);
  slayout->addWidget(onroad);
  QObject::connect(this, &HomeWindow::update, onroad, &OnroadWindow::update);

  home = new OffroadHome();
  slayout->addWidget(home);
  QObject::connect(this, &HomeWindow::openSettings, home, &OffroadHome::refresh);

  setLayout(layout);
}

void HomeWindow::offroadTransition(bool offroad) {
  if (offroad) {
    slayout->setCurrentWidget(home);
  } else {
    slayout->setCurrentWidget(onroad);
  }
  sidebar->setVisible(offroad);
}

void HomeWindow::mousePressEvent(QMouseEvent* e) {
  // TODO: make a nice driver view widget
  if (QUIState::ui_state.scene.driver_view && !rec_btn.ptInRect(e->x(), e->y())) {
    Params().putBool("IsDriverViewEnabled", false);
    QUIState::ui_state.scene.driver_view = false;
    return;
  }
  // OPKR Settings button double click
  if (sidebar->isVisible() && settings_btn.ptInRect(e->x(), e->y())) {
    QUIState::ui_state.scene.setbtn_count = QUIState::ui_state.scene.setbtn_count + 1;
    if (QUIState::ui_state.scene.setbtn_count > 1) {
      emit openSettings();
    }
    return;
  }
  // OPKR home button double click
  if (sidebar->isVisible() && QUIState::ui_state.scene.car_state.getVEgo() < 0.5 && home_btn.ptInRect(e->x(), e->y())) {
    QUIState::ui_state.scene.homebtn_count = QUIState::ui_state.scene.homebtn_count + 1;
    if (QUIState::ui_state.scene.homebtn_count > 1) {
      QProcess::execute("/data/openpilot/run_mixplorer.sh");
    }
    return;
  }
  // OPKR add map
  if (QUIState::ui_state.scene.started && map_overlay_btn.ptInRect(e->x(), e->y())) {
    QUIState::ui_state.sound->play(AudibleAlert::CHIME_WARNING1);
    QProcess::execute("am start --activity-task-on-home com.opkr.maphack/com.opkr.maphack.MainActivity");
    QUIState::ui_state.scene.map_on_top = false;
    QUIState::ui_state.scene.map_on_overlay = !QUIState::ui_state.scene.map_on_overlay;
    return;
  }
  if (QUIState::ui_state.scene.started && !sidebar->isVisible() && !QUIState::ui_state.scene.map_on_top && map_btn.ptInRect(e->x(), e->y())) {
    QUIState::ui_state.sound->play(AudibleAlert::CHIME_WARNING1);
    QProcess::execute("am start --activity-task-on-home com.skt.tmap.ku/com.skt.tmap.activity.TmapNaviActivity");
    QUIState::ui_state.scene.map_on_top = true;
    return;
  }
  // OPKR REC
  if (QUIState::ui_state.scene.started && !sidebar->isVisible() && !QUIState::ui_state.scene.map_on_top && !QUIState::ui_state.scene.comma_stock_ui && rec_btn.ptInRect(e->x(), e->y())) {
    QUIState::ui_state.scene.recording = !QUIState::ui_state.scene.recording;
    QUIState::ui_state.scene.touched = true;
    return;
  }
  // Laneless mode
  if (QUIState::ui_state.scene.started && !sidebar->isVisible() && !QUIState::ui_state.scene.map_on_top && QUIState::ui_state.scene.end_to_end && !QUIState::ui_state.scene.comma_stock_ui && laneless_btn.ptInRect(e->x(), e->y())) {
    QUIState::ui_state.scene.laneless_mode = QUIState::ui_state.scene.laneless_mode + 1;
    if (QUIState::ui_state.scene.laneless_mode > 2) {
      QUIState::ui_state.scene.laneless_mode = 0;
    }
    if (QUIState::ui_state.scene.laneless_mode == 0) {
      Params().put("LanelessMode", "0", 1);
    } else if (QUIState::ui_state.scene.laneless_mode == 1) {
      Params().put("LanelessMode", "1", 1);
    } else if (QUIState::ui_state.scene.laneless_mode == 2) {
      Params().put("LanelessMode", "2", 1);
    }
    return;
  }
  // Monitoring mode
  if (QUIState::ui_state.scene.started && !sidebar->isVisible() && !QUIState::ui_state.scene.map_on_top && monitoring_btn.ptInRect(e->x(), e->y())) {
    QUIState::ui_state.scene.monitoring_mode = QUIState::ui_state.scene.monitoring_mode + 1;
    if (QUIState::ui_state.scene.monitoring_mode > 1) {
      QUIState::ui_state.scene.monitoring_mode = 0;
    }
    if (QUIState::ui_state.scene.monitoring_mode == 0) {
      Params().put("OpkrMonitoringMode", "0", 1);
    } else if (QUIState::ui_state.scene.monitoring_mode == 1) {
      Params().put("OpkrMonitoringMode", "1", 1);
    }
    return;
  }
  // // ML Button
  // if (QUIState::ui_state.scene.started && !sidebar->isVisible() && !QUIState::ui_state.scene.map_on_top && QUIState::ui_state.scene.model_long && !QUIState::ui_state.scene.comma_stock_ui && ml_btn.ptInRect(e->x(), e->y())) {
  //   QUIState::ui_state.scene.mlButtonEnabled = !QUIState::ui_state.scene.mlButtonEnabled;
  //   if (QUIState::ui_state.scene.mlButtonEnabled) {
  //     Params().put("ModelLongEnabled", "1", 1);
  //   } else {
  //     Params().put("ModelLongEnabled", "0", 1);
  //   }
  //   return;
  // }
  // Stock UI Toggle
  if (QUIState::ui_state.scene.started && !sidebar->isVisible() && !QUIState::ui_state.scene.map_on_top && stockui_btn.ptInRect(e->x(), e->y())) {
    QUIState::ui_state.scene.comma_stock_ui = !QUIState::ui_state.scene.comma_stock_ui;
    if (QUIState::ui_state.scene.comma_stock_ui) {
      Params().put("CommaStockUI", "1", 1);
    } else {
      Params().put("CommaStockUI", "0", 1);
    }
    return;
  }
  // Handle sidebar collapsing
  if (childAt(e->pos()) == onroad) {
    sidebar->setVisible(!sidebar->isVisible());
    QUIState::ui_state.sidebar_view = !QUIState::ui_state.sidebar_view;
  }

  QUIState::ui_state.scene.setbtn_count = 0;
  QUIState::ui_state.scene.homebtn_count = 0;
}


// OffroadHome: the offroad home page

OffroadHome::OffroadHome(QWidget* parent) : QFrame(parent) {
  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setMargin(50);

  // top header
  QHBoxLayout* header_layout = new QHBoxLayout();

  date = new QLabel();
  date->setStyleSheet(R"(font-size: 55px;)");
  header_layout->addWidget(date, 0, Qt::AlignHCenter | Qt::AlignLeft);

  alert_notification = new QPushButton();
  alert_notification->setVisible(false);
  QObject::connect(alert_notification, &QPushButton::released, this, &OffroadHome::openAlerts);
  header_layout->addWidget(alert_notification, 0, Qt::AlignHCenter | Qt::AlignRight);

  std::string brand = Params().getBool("Passive") ? "行車記錄器模式" : "OPENPILOT模式";
  QLabel* version = new QLabel(QString::fromStdString(brand + " v" + Params().get("Version")));
  version->setStyleSheet(R"(font-size: 45px;)");
  header_layout->addWidget(version, 0, Qt::AlignHCenter | Qt::AlignRight);

  main_layout->addLayout(header_layout);

  // main content
  main_layout->addSpacing(25);
  center_layout = new QStackedLayout();

  QHBoxLayout* statsAndSetup = new QHBoxLayout();
  statsAndSetup->setMargin(0);

  DriveStats* drive = new DriveStats;
  drive->setFixedSize(800, 800);
  statsAndSetup->addWidget(drive);

  SetupWidget* setup = new SetupWidget;
  //setup->setFixedSize(700, 700);
  statsAndSetup->addWidget(setup);

  QWidget* statsAndSetupWidget = new QWidget();
  statsAndSetupWidget->setLayout(statsAndSetup);

  center_layout->addWidget(statsAndSetupWidget);

  alerts_widget = new OffroadAlert();
  QObject::connect(alerts_widget, &OffroadAlert::closeAlerts, this, &OffroadHome::closeAlerts);
  center_layout->addWidget(alerts_widget);
  center_layout->setAlignment(alerts_widget, Qt::AlignCenter);

  main_layout->addLayout(center_layout, 1);

  // set up refresh timer
  timer = new QTimer(this);
  QObject::connect(timer, &QTimer::timeout, this, &OffroadHome::refresh);
  refresh();
  timer->start(10 * 1000);

  setLayout(main_layout);
  setStyleSheet(R"(
    OffroadHome {
      background-color: black;
    }
    * {
     color: white;
    }
  )");
}

void OffroadHome::openAlerts() {
  center_layout->setCurrentIndex(1);
}

void OffroadHome::closeAlerts() {
  center_layout->setCurrentIndex(0);
}

void OffroadHome::refresh() {
  bool first_refresh = !date->text().size();
  if (!isVisible() && !first_refresh) {
    return;
  }

  date->setText(QDateTime::currentDateTime().toString("yyyy年 M月 d日"));

  // update alerts

  alerts_widget->refresh();
  if (!alerts_widget->alertCount && !alerts_widget->updateAvailable) {
    emit closeAlerts();
    alert_notification->setVisible(false);
    return;
  }

  if (alerts_widget->updateAvailable) {
    alert_notification->setText("更新");
  } else {
    int alerts = alerts_widget->alertCount;
    alert_notification->setText(QString::number(alerts) + " 警告" + (alerts == 1 ? "" : "S"));
  }

  if (!alert_notification->isVisible() && !first_refresh) {
    emit openAlerts();
  }
  alert_notification->setVisible(true);

  // Red background for alerts, blue for update available
  QString style = QString(R"(
    padding: 15px;
    padding-left: 30px;
    padding-right: 30px;
    border: 1px solid;
    border-radius: 5px;
    font-size: 40px;
    font-weight: 500;
    background-color: #E22C2C;
  )");
  if (alerts_widget->updateAvailable) {
    style.replace("#E22C2C", "#364DEF");
  }
  alert_notification->setStyleSheet(style);
}
