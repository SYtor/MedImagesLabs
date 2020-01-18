#include "DicomFileWrapper.h"

#include "DicomFileWrapper.h"

DicomFileWrapper::DicomFileWrapper(const std::string &imagePath) {

    dcmFileFormat = new DcmFileFormat();
    dcmFileFormat->loadFile(imagePath.c_str());

    auto bitsAllocated = getUShort(DcmTagKey(0x0028, 0x0100));
    if (bitsAllocated != 8)
        throw std::runtime_error("Image not supported");
}

DicomFileWrapper::~DicomFileWrapper() {
    delete dcmFileFormat;
}

unsigned short DicomFileWrapper::getUShort(const DcmTagKey &dcmTagKey) {
    unsigned short value = 0;
    dcmFileFormat->getDataset()->findAndGetUint16(dcmTagKey, value);
    return value;
}

std::string DicomFileWrapper::getString(const DcmTagKey &dcmTagKey) {
    OFString value;
    dcmFileFormat->getDataset()->findAndGetOFString(dcmTagKey, value);
    return std::string(value.c_str());
}

const unsigned char *DicomFileWrapper::getUCharArray(const DcmTagKey &dcmTagKey) {
    const unsigned char *value;
    dcmFileFormat->getDataset()->findAndGetUint8Array(dcmTagKey, value);
    return value;
}

DcmCodeString DicomFileWrapper::getCodeString(const DcmTagKey &dcmTagKey) {
    OFString value;
    dcmFileFormat->getDataset()->findAndGetOFStringArray(dcmTagKey, value);
    DcmCodeString dcmCodeString(DcmTag(dcmTagKey), value.size());
    dcmCodeString.putString(value.c_str());
    return dcmCodeString;
}

double DicomFileWrapper::getDouble(const DcmTagKey &dcmTagKey) {
    Float64 value = 0;
    dcmFileFormat->getDataset()->findAndGetFloat64(dcmTagKey, value);
    return value;
}

std::pair<double, double> DicomFileWrapper::findMinMaxPixel(int width, int height, const unsigned char* pixels) {
    double min = 255;
    double max = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char pixel = *(pixels + width * y + x);
            if (pixel < min) min = pixel;
            if (pixel > max) max = pixel;
        }
    }
    return std::pair(min, max);
}

void DicomFileWrapper::saveTransformedImage(int width, int height, unsigned char* pixels) {

    DcmFileFormat newImage(*dcmFileFormat);
    newImage.getDataset()->putAndInsertString(DcmTag(DcmTagKey(0x0008, 0x0008)), "DERIVED\\SECONDARY\\MPR");
    newImage.saveFile("trans.dcm");

}