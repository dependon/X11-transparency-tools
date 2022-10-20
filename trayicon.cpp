#include "trayicon.h"
#include "mainwindow.h"
#include <QMenu>
#include <QCoreApplication>
#include <QApplication>
#define App (static_cast<QApplication*>(QCoreApplication::instance()))
TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    m_window = new MainWindow();

    m_traymenu = new QMenu();
    QAction *exitAction = new QAction(m_traymenu);
    exitAction->setText("退出");
    connect(exitAction, &QAction::triggered, this, &TrayIcon::quitApp);
    QAction *setShowAction = new QAction(m_traymenu);
    setShowAction->setText("显示");
    connect(setShowAction, &QAction::triggered, this, [ = ] {
        m_window->show();
        m_window->activateWindow();
    });

    QAction *setHideAction = new QAction(m_traymenu);
    setHideAction->setText("隐藏");
    connect(setHideAction, &QAction::triggered, this, [ = ] {
        m_window->hide();
    });

    m_traymenu->addAction(setShowAction);
    m_traymenu->addAction(setHideAction);
    m_traymenu->addAction(exitAction);

    this->setContextMenu(m_traymenu);
    connect(this, &QSystemTrayIcon::activated, this, [ = ](QSystemTrayIcon::ActivationReason reason) {
        if (QSystemTrayIcon::Trigger == reason) {
            m_window->show();
            m_window->activateWindow();
        }
    });

    m_window->show();

    setIcon(QIcon(":/install/x11-transparency-tools.svg"));
}
TrayIcon::~TrayIcon()
{
    delete  m_window;
    m_window = nullptr;
}
void TrayIcon::quitApp()
{
    App->exit();
}
