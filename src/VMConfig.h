#ifndef VMCONFIG_H
#define VMCONFIG_H

#include <QWidget>
#include <QtWidgets>
#include "Device.h"
#include "DeviceUsb.h"

class CommandLineParameters;

class VMConfig : public QObject
{
    Q_OBJECT

public:
    VMConfig(QObject *parent, const QString &path);
    ~VMConfig();

public:
    void readVMConfig();
    bool save_vm_config(const QString &path) const;
    void save_vm_config() const;
    void createVMFolder(const QString &path) const;
    void set_name(const QString &name_vm_);
    void setCmdLine(const QString &cmdLine);
    void addDefaultBus(const QString &image);
    void addDeviceMemory(const QString &size);
    void addDeviceMachine(const QString &name);
    void addDeviceCpu(const QString &name);
    void addUsbDevice();
    QString get_vm_info();

    void setPlatform(const QString &platformVM);
    QString getPlatform();

    QString get_name();
    QString getCmdLine();
    QString get_dir_path();
    QString getCommandLine(CommandLineParameters &cmdParams);
    Device *getSystemDevice() { return &system; }
    QString getPathRRDir();
    QStringList getReplayList() { return replayList; }
    void fillReplayList();

    void remove_directory_vm();
    static void remove_directory_vm(const QString &dir);

private:
    QFile *list_of_vm_file;

    QString path;
    QString name_vm;
    QString dir_path;
    QString platform;
    QString addCmdLine;

    Device system;
    QStringList replayList;

};

#endif //VMCONFIG_H

