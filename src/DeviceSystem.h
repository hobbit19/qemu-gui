#ifndef DEVICESYSTEM_H
#define DEVICESYSTEM_H

#include "Device.h"
#include "QemuGUICommon.h"


class DeviceMemory : public Device
{
public:
    static const char typeName[];

    DeviceMemory() {}
    DeviceMemory(const QString &memSize, Device *parent);

    virtual QString getDeviceTypeName() const { return typeName; }
    virtual QWidget *getEditorForm();

    void setSize(const QString &newSize) { size = newSize; }
    int getSize() const { return size.toInt(); }

    virtual BusType needsBus() const { return BusType::System; }

protected:
    virtual void saveParameters(QXmlStreamWriter &xml) const;
    virtual void readParameters(QXmlStreamReader &xml);
    virtual QString getCommandLineOption(CommandLineParameters &cmdParams);

private:
    QString size; /* in megabytes */
};


class DeviceSystem : public Device
{
public:
    DeviceSystem() {}
    DeviceSystem(const QString &n, Device *parent);

    virtual QString getDeviceTypeName() const = 0;

protected:
    virtual void saveParameters(QXmlStreamWriter &xml) const;
    virtual void readParameters(QXmlStreamReader &xml);
    void setName(const QString &newName) { name = newName; }
    QString getName() const { return name; }

private:
    QString name;
};

class DeviceMachine : public DeviceSystem
{
public:
    static const char typeName[];

    DeviceMachine() {}
    DeviceMachine(const QString &machineName, Device *parent);

    virtual QString getDeviceTypeName() const { return typeName; }

protected:
    virtual QString getCommandLineOption(CommandLineParameters &cmdParams);
};


class DeviceCpu : public DeviceSystem
{
public:
    static const char typeName[];

    DeviceCpu() {}
    DeviceCpu(const QString &cpuName, Device *parent);

    virtual QString getDeviceTypeName() const { return typeName; }

protected:
    virtual QString getCommandLineOption(CommandLineParameters &cmdParams);
};




#endif // DEVICESYSTEM_H


