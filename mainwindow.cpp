#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "setdesktop.h"

#include <QTimer>
#include <QThread>
#include <QMutexLocker>
#include <QSystemTrayIcon>
#include <QCoreApplication>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>

#define App (static_cast<QApplication*>(QCoreApplication::instance()))
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("X11OpacityTool"));
    ui->statusBar->showMessage("https://github.com/dependon/x11opacitytool");
//    initTray();
    initXcb();
    QTimer::singleShot(100, [ = ] {
        setAllWindows();
    });

    this->setMaximumSize(600, 150);
    this->move(App->desktop()->screen()->rect().center() - this->rect().center());
    m_eventThread = QThread::create([ = ]() {
        Display *display;
        Window rootwin;
        display = XOpenDisplay(NULL);
        rootwin = DefaultRootWindow(display);
        XSelectInput(display, rootwin, SubstructureNotifyMask); /*事件可以参考x.h*/
        XEvent event;
        while (m_bEvent) {
            XNextEvent(display, &event);
            if (event.type == CreateNotify) {

                XCreateWindowEvent *reparentevent = (XCreateWindowEvent *)&event;
                Window id = reparentevent->window;
                if (m_bFisrt) {
                    m_myId = id;
                    m_bFisrt = false;
                }
                QWindow *window = QWindow::fromWinId((unsigned long)id);
                uint32_t indexId = searchWindowType(id) ;
                qDebug() << indexId;
                if (window != nullptr && m_myId != id
                        && (indexId == 381 || indexId == 377 || indexId == 371
                            || indexId == 376 || indexId == 192266373 ||
                            indexId == 10809947 || indexId == 344672356)) {

                    int value = ui->opacitySlider->value();
                    double a = (double)value;
                    double o = a / 100.0;

                    window->setOpacity(o);

                    strucWindow st;
                    st.window = window;
                    st.wid = id;
                    st.name = "name";
                    st.opacity = o;
                    m_windowVec.insert(id, st);

                }
            } else if (event.type == ReparentNotify) {
                Window x11window;
                Display *x11display;
                char **srname = (char **)malloc(sizeof(char *));
                XReparentEvent *reparentevent = (XReparentEvent *)&event;

                printf("window: %ld \n", (unsigned long)(reparentevent->window));
                qDebug() << reparentevent->window;
                printf("parent: %ld \n", (unsigned long)(reparentevent->parent));
                qDebug() << reparentevent->parent;
                /*获取到新建窗口的window ID*/
                x11window = (unsigned long)(reparentevent->window);
                x11display = (reparentevent->display);
                XFetchName(x11display, x11window, srname);
                x11display = XOpenDisplay(NULL);  /*！！！注意这里以后如果你想对获取到的window ID进行操作必须重新链接*/

                if (*srname != NULL)
                    printf("%s\n", *srname);

                free(*srname);
            }
        }
    });
    m_eventThread->start();

    ui->setBtn->hide();
}

MainWindow::~MainWindow()
{
    m_bEvent = false;
    m_eventThread->wait();
    m_eventThread->quit();
    setAllWindowsOpacity(0.99);
    delete ui;
}

void MainWindow::initXcb()
{
    m_cookie  = xcb_ewmh_init_atoms(QX11Info::connection(), &m_ewmh_connection);
    xcb_ewmh_init_atoms_replies(&m_ewmh_connection, m_cookie, NULL);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void MainWindow::setAllWindows()
{
    qDebug() << "xxx1";
    char *str = NULL;
    QByteArray ba = "";
    str = (char *)malloc(ba.length() + 1);
    memset(str, 0, ba.length());
    memcpy(str, ba.data(), ba.length());
    qDebug() << "xxxx2";
    str[ba.length()] = '\0';
    //设置desktop透明
    int pid_t[128];
    find_pid_by_name(str, pid_t);
    int pid = pid_t[0];

    qDebug() << "xxxx3";
    Display *display = XOpenDisplay(0);
    WindowsMatchingPid match(display, XDefaultRootWindow(display), pid);
//    const list<Window> &result = match.result();
    const list<Window> &allresult = match.Allresult();
    qDebug() << "xxxx4";
    for (Window id : allresult) {
        QWindow *window = QWindow::fromWinId((unsigned long)id);
        uint32_t indexId = searchWindowType(id) ;
        qDebug() << indexId;
        if (window != nullptr && m_myId != id
                && (indexId == 381 || indexId == 377 || indexId == 371
                    || indexId == 376 || indexId == 192266373)) {

            int value = ui->opacitySlider->value();
            double a = (double)value;
            double o = a / 100.0;
            window->setOpacity(o);

            strucWindow st;
            st.window = window;
            st.wid = id;
            st.name = "name";
            st.opacity = o;
            m_windowVec.insert(id, st);

        }
    }
}

void MainWindow::setAllWindowsOpacity(double opacity)
{
    for (auto structwindow : m_windowVec) {
        if (structwindow.window) {
            structwindow.window->setOpacity(opacity);
            structwindow.opacity = opacity;
            m_windowVec.insert(structwindow.wid, structwindow);
        }
    }
}
void MainWindow::searchAllWindowType()
{
    QMutexLocker locker(&m_mutex);
    if (m_cookie) {
        for (auto st : m_windowVec) {
            xcb_get_property_cookie_t cooke = xcb_ewmh_get_wm_window_type(&m_ewmh_connection, st.wid);

            xcb_ewmh_get_atoms_reply_t name;
            xcb_ewmh_get_wm_window_type_reply(&m_ewmh_connection, cooke, &name, NULL);
            qDebug() << name.atoms[0];
        }
    }
}

uint32_t MainWindow::searchWindowType(int wid)
{
    uint32_t reId = 0;
    QMutexLocker locker(&m_mutex);
    if (m_cookie) {

        xcb_get_property_cookie_t cooke = xcb_ewmh_get_wm_window_type(&m_ewmh_connection, wid);

        xcb_ewmh_get_atoms_reply_t name;
        xcb_generic_error_t *error_t = new xcb_generic_error_t;
//        xcb_ewmh_get_wm_window_type_reply(&m_ewmh_connection, cooke, &name, NULL);
        xcb_ewmh_get_wm_window_type_reply(&m_ewmh_connection, cooke, &name, &error_t);
        qDebug() << "ssss";
        if (error_t) {
            qDebug() << error_t->response_type;
            qDebug() << error_t->error_code;
            qDebug() << error_t->sequence;
            qDebug() << error_t->resource_id;
            qDebug() << error_t->minor_code;
            qDebug() << error_t->major_code;
            delete error_t;
            error_t = NULL;
            return 381;
        } else {

        }
        qDebug() << "eeee";
        if (name.atoms && name.atoms_len == 1) {
            reId = name.atoms[0];
        }

    }
    return reId;
}

void MainWindow::quitApp()
{
    App->quit();
}
void MainWindow::initTray()
{
    m_traymenu = new QMenu();
    QAction *exitAction = new QAction(m_traymenu);
    exitAction->setText("退出");
    connect(exitAction, &QAction::triggered, this, &MainWindow::quitApp);
    QAction *setShowAction = new QAction(m_traymenu);
    setShowAction->setText("显示");
    connect(setShowAction, &QAction::triggered, this, [ = ] {
        this->show();
        this->activateWindow();
    });

    QAction *setHideAction = new QAction(m_traymenu);
    setHideAction->setText("隐藏");
    connect(setHideAction, &QAction::triggered, this, [ = ] {
        this->hide();
    });

    m_traymenu->addAction(setShowAction);
    m_traymenu->addAction(setHideAction);
    m_traymenu->addAction(exitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_traymenu);
    m_trayIcon->setToolTip("双击打开界面");
    m_trayIcon->show();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, [ = ](QSystemTrayIcon::ActivationReason reason) {
        if (QSystemTrayIcon::Trigger == reason) {
            this->show();
            this->activateWindow();
        }
    });
}

void MainWindow::on_setBtn_clicked()
{
    int value = ui->opacitySlider->value();
    double a = (double)value;
    double o = a / 100.0;
    setAllWindowsOpacity(o);
}

void MainWindow::on_opacitySlider_sliderReleased()
{
    int value = ui->opacitySlider->value();
    double a = (double)value;
    double o = a / 100.0;
    setAllWindowsOpacity(o);
    qDebug() << "xxx";
}
