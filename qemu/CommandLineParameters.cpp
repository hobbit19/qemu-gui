#include "CommandLineParameters.h"

LaunchMode CommandLineParameters::getLaunchMode()
{
    return mode;
}

QString CommandLineParameters::getNextOverlayName()
{
    return overlayPath + "/overlay" + QString::number(diskID++) + ".ovl";
}

QString CommandLineParameters::getOverlayForImage(const QString &image)
{
    if (overlayEnabled)
    {
        images.append(image);
        overlays.append(getNextOverlayName());
        return overlays.back();
    }
    else
    {
        return image;
    }
    
}


