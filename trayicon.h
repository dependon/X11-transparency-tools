#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
class MainWindow;
class QMenu;
class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    TrayIcon(QObject *parent = nullptr);
    ~TrayIcon();

    void quitApp();
private:
    MainWindow *m_window{nullptr};
    QMenu *m_traymenu{nullptr};
};

#endif // TRAYICON_H
