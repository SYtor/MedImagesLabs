#include "DicomFileWrapper.h"

DicomFileWrapper::DicomFileWrapper(const std::string &imagePath) {

    dcmFileFormat = new DcmFileFormat();
    dcmFileFormat->loadFile(imagePath.c_str());

    dicomImage = new DicomImage(imagePath.c_str());

    int imageWidth = getImageWidth();
    int imageHeight = getImageHeight();
    if (!dicomImage->isMonochrome() || dicomImage->getOutputDataSize() / imageHeight / imageWidth != 1)
        throw std::runtime_error("Image not supported");
}

int DicomFileWrapper::getImageWidth() { return (int) dicomImage->getWidth(); }
int DicomFileWrapper::getImageHeight() { return (int) dicomImage->getHeight(); }

std::string DicomFileWrapper::getString(const DcmTagKey &dcmTagKey) {
    OFString value;
    dcmFileFormat->getDataset()->findAndGetOFString(dcmTagKey, value);
    return std::string(value.c_str());
}

double DicomFileWrapper::getDouble(const DcmTagKey &dcmTagKey) {
    Float64 value = 0;
    dcmFileFormat->getDataset()->findAndGetFloat64(dcmTagKey, value);
    return value;
}

const void *DicomFileWrapper::getImageOutputData(const int bits) {
    return dicomImage->getOutputData(bits);
}

std::pair<double, double> DicomFileWrapper::findMinMaxPixel() {
    int width = getImageWidth();
    int height = getImageHeight();
    auto pixelData = (unsigned char *) getImageOutputData(8);

    double min = 255;
    double max = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char pixel = *(pixelData + width * y + x);
            if (pixel < min) min = pixel;
            if (pixel > max) max = pixel;
        }
    }

    return std::pair(min, max);
}

void DicomFileWrapper::saveTransformedImage() {

    DcmFileFormat newImage(*dcmFileFormat);
    newImage.getDataset()->putAndInsertString(DcmTag(DcmTagKey(0x0008, 0x0008)), "DERIVED");
    newImage.saveFile("trans.dcm");

}