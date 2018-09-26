#include "DeviceStorage.h"
#include "DeviceBus.h"
#include "CommandLineParameters.h"

DeviceStorageController::DeviceStorageController(const QString &n, Device *parent)
    : Device(n, parent)
{

}

const char DeviceIdeController::typeName[] = "DeviceIdeController";

DeviceIdeController::DeviceIdeController(Device *parent)
    : DeviceStorageController("IDE", parent)
{
    initDefault();
}

void DeviceIdeController::initDefault()
{
    new DeviceBusIde(0, this);
    new DeviceBusIde(1, this);
}

DeviceStorage::DeviceStorage(const QString &n, Device *parent)
    : Device(n, parent)
{

}

/******************************************************************************
 * IDE HDD                                                                    *
 ******************************************************************************/

const char DeviceIdeHd::typeName[] = "DeviceIdeHd";
static const char xml_image[] = "Image";

DeviceIdeHd::DeviceIdeHd(const QString &img, Device *parent)
    : DeviceStorage("IDE-HD", parent), image(img)
{
}

QWidget *DeviceIdeHd::getEditorForm()
{
    return new DeviceIdeHdForm(this);
}

void DeviceIdeHd::saveParameters(QXmlStreamWriter &xml) const
{
    xml.writeStartElement(xml_image);
    xml.writeCharacters(image);
    xml.writeEndElement();
}

void DeviceIdeHd::readParameters(QXmlStreamReader &xml)
{
    xml.readNextStartElement();
    Q_ASSERT(xml.name() == xml_image);
    image = xml.readElementText();
}

QString DeviceIdeHd::getCommandLineOption(CommandLineParameters &cmdParams)
{
    DeviceBusIde *bus = dynamic_cast<DeviceBusIde *> (parent());
    Q_ASSERT(bus);

    if (cmdParams.getLaunchMode() == LaunchMode::NORMAL)
    {
        QString idFile = cmdParams.getNextID();
        QString cmdFile = " -drive file=" + image + ",if=none,id=" + idFile;
        return  cmdFile + " -device ide-hd,bus=" + bus->getDescription() +
            ",drive=" + idFile;
    }
    else
    {
        QString overlay = cmdParams.getOverlayForImage(image);
        QString idFile = cmdParams.getNextID();
        QString cmdFile = "-drive file=" + overlay + ",if=none,id=" + idFile;
        QString idDriver = cmdParams.getNextID();
            
        return cmdFile + " -drive driver=blkreplay,if=none,image=" +
            idFile + ",id=" + idDriver + " -device ide-hd,drive=" + idDriver;
    }
}

bool DeviceIdeHd::checkValidationDeviceInfo()
{
    if (!getImage().isEmpty())
        return true;
    return false;
}

/***************************************************************************
* DeviceIdeHdForm                                                          *
***************************************************************************/

DeviceIdeHdForm::DeviceIdeHdForm(DeviceIdeHd *dev) : device(dev)
{
    QGroupBox *ideFormGroup = this;
    QLineEdit *imageLine = new QLineEdit(ideFormGroup);
    QPushButton *selectImageBtn = new QPushButton("...", ideFormGroup);

    selectImageBtn->setFixedWidth(30);
    imageLine->setText(device->getImage());
    imageLine->setReadOnly(true);
    if (device->getImage().isEmpty())
    {
        imageLine->setStyleSheet("background: #EE756F");
    }

    QVBoxLayout *mainLay = new QVBoxLayout();
    QHBoxLayout *topLay = new QHBoxLayout();
    topLay->addWidget(imageLine);
    topLay->addWidget(selectImageBtn);

    mainLay->addLayout(topLay);
    mainLay->addStretch(500);

    ideFormGroup->setLayout(mainLay);
    connect(selectImageBtn, &QPushButton::clicked, this, &DeviceIdeHdForm::editImage);
    connect(this, SIGNAL(newImageSet(QString)), imageLine, SLOT(setText(QString)));
    connect(this, SIGNAL(newDiskCompleted(QString)), imageLine, SLOT(setStyleSheet(QString)));
}

void DeviceIdeHdForm::editImage()
{
    QString newImage = QFileDialog::getOpenFileName(nullptr, "Selecting image",
        "", "*.qcow *.qcow2 *.raw");
    if (!newImage.isEmpty())
    {
        emit newImageSet(newImage);
        emit newDiskCompleted("");
        device->setNewHDD(newImage);
    }
}

