#include "Device.h"
#include "DeviceFactory.h"

/*

Device properties:
 - parent bus type
 - child buses
   - type of children
   - number of children
*/

static const char xml_name[] = "Name";
static const char xml_removable[] = "removable";
static const char xml_cmdLine[] = "CmdLineOption";

Device::Device()
{
    init();
}

Device::Device(const QString &n, Device *parent)
    : QObject(NULL), name(n), additionalCmdOption("")
{
    init();
    if (parent)
        parent->addDevice(this);
}

void Device::init()
{
    static int lastId = 0;

    id = "device-" + QString::number(lastId++);
    isCanRemove = true;
}

void Device::addDevice(Device *dev)
{
    devices.append(dev);
    dev->setParent(this);
}

void Device::removeDevice(Device *dev)
{
    devices.removeOne(dev);
}

QString Device::getDescription() const
{
    return name;
}

QString Device::getCommonDeviceInfo()
{
    QString res = getDeviceInfo();
    foreach(Device *dev, devices)
    {
        res += dev->getCommonDeviceInfo();
    }
    return res;
}

void Device::save(QXmlStreamWriter &xml) const
{
    xml.writeStartElement(getDeviceTypeName());

    xml.writeStartElement(xml_name);
    if (isCanRemove)
    {
        xml.writeAttribute(xml_removable, "true");
    }
    xml.writeCharacters(name);
    xml.writeEndElement();

    if (isCanRemove)
    {
        xml.writeStartElement(xml_cmdLine);
        xml.writeCharacters(getAddtionalCommandLineOption());
        xml.writeEndElement();
    }

    saveParameters(xml);

    foreach(Device *dev, devices)
        dev->save(xml);
    
    xml.writeEndElement();
}

void Device::read(QXmlStreamReader &xml)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == getDeviceTypeName());

    xml.readNextStartElement();
    Q_ASSERT(xml.name() == xml_name);
    if (xml.attributes().empty())
        isCanRemove = false;
    name = xml.readElementText();

    if (isCanRemove)
    {
        xml.readNextStartElement();
        Q_ASSERT(xml.name() == xml_cmdLine);
        setAdditionalCommandLineOption(xml.readElementText());
    }

    readParameters(xml);

    // default children
    foreach(Device *dev, devices)
    {
        xml.readNextStartElement();
        dev->read(xml);
    }

    while (xml.readNextStartElement())
    {
        Device *dev = DeviceFactory::createDevice(QString().append(xml.name()));
        addDevice(dev);
        dev->read(xml);
    }
}

QString Device::getCommandLine(CommandLineParameters &cmdParams)
{
    QString addCmdOpt = additionalCmdOption.isEmpty() ? "" : "," + additionalCmdOption;
    QString res = getCommandLineOption(cmdParams) + addCmdOpt;
    foreach(Device *dev, devices)
        res += dev->getCommandLine(cmdParams);
    return res;
}
