#include "QemuLauncher.h"


QemuLauncher::QemuLauncher(const QString &qemu_install_dir_path, VMConfig *vm, const QString &port_qmp, const QString &port_monitor, QObject *parent)
    : QObject(parent)
{
    qemu_dir = qemu_install_dir_path
#ifdef Q_OS_WIN
        + "/qemu-system-i386w.exe";
#else
        + "/qemu-system-i386";
#endif
    virtual_machine = vm;

    this->port_monitor = port_monitor;
    this->port_qmp = port_qmp;
}

QemuLauncher::~QemuLauncher()
{
}

void QemuLauncher::kill_qemu_process()
{
    qemu->kill();
    emit qemu->finished(0, QProcess::ExitStatus::NormalExit);
}


void QemuLauncher::start_qemu()
{
    qemu = new QProcess();
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    connect(qemu, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finish_qemu(int, QProcess::ExitStatus)));
    QString mon = " -monitor \"tcp:127.0.0.1:" + port_monitor + ",server,wait\"";
    QString qmp = " -qmp \"tcp:127.0.0.1:" + port_qmp + ",server,wait\"";
    qemu->start(qemu_dir + " " + virtual_machine->get_image_path() + mon + qmp);
    qemu->waitForFinished(-1);
}

void QemuLauncher::finish_qemu(int exitCode, QProcess::ExitStatus ExitStatus)
{
    emit qemu_laucher_finished();
}
