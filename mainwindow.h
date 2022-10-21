#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QMap>
#include <QWindow>
#include <QMutex>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <QtX11Extras/QX11Info>
#include <QCloseEvent>


class QSystemTrayIcon;
class QThread;
struct strucWindow {
    int wid = 0;
    QString name = "";
    QWindow *window{nullptr};
    double opacity{1.0};
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setAllWindows();
    void setAllWindowsOpacity(double opacity = 0.99);
    void searchAllWindowType();
    uint32_t searchWindowType(int wid);

    QList<unsigned long> searchWindowid(const QString &name);
    void initTray();
    void initXcb();
    void initNoopacity();

    void closeEvent(QCloseEvent *event);
private slots:
    void quitApp();

    void on_opacitySlider_valueChanged(int);

private:
    Ui::MainWindow *ui;

    QMap <int, strucWindow> m_windowVec;
    xcb_ewmh_connection_t m_ewmh_connection;
    xcb_intern_atom_cookie_t *m_cookie{nullptr};
    QMutex m_mutex;
    bool m_bEvent = true;
    QThread *m_eventThread{nullptr};
    unsigned long m_myId = 0;
    bool m_bFisrt = true;
    QSystemTrayIcon *m_trayIcon{nullptr};
    QMenu *m_traymenu{nullptr};

    QList <unsigned long> m_noOpacityId;
};

#endif // MAINWINDOW_H
