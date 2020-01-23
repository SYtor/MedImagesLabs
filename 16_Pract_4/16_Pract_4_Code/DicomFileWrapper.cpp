#include "DicomFileWrapper.h"

DicomFileWrapper::DicomFileWrapper(const std::string &imagePath) {
    dcmFileFormat = new DcmFileFormat();
    dcmFileFormat->loadFile(imagePath.c_str());
}

DicomFileWrapper::~DicomFileWrapper() {
    delete dcmFileFormat;
}

unsigned short DicomFileWrapper::getUShort(const DcmTagKey &dcmTagKey) {
    unsigned short value = 0;
    dcmFileFormat->getDataset()->findAndGetUint16(dcmTagKey, value);
    return value;
}

const unsigned char *DicomFileWrapper::getUCharArray(const DcmTagKey &dcmTagKey) {
    const unsigned char *value;
    dcmFileFormat->getDataset()->findAndGetUint8Array(dcmTagKey, value);
    return value;
}

const float *DicomFileWrapper::getFloatArray(const DcmTagKey &dcmTagKey) {
    const float *value;
    dcmFileFormat->getDataset()->findAndGetFloat32Array(dcmTagKey, value);
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

void DicomFileWrapper::saveTransformedImage(int width, int height, const float* pixels, double rescaleSlope, double rescaleIntercept) {
    DcmFileFormat newImage(*dcmFileFormat);
    newImage.getDataset()->putAndInsertString(DcmTag(DcmTagKey(0x0008, 0x0008)), "DERIVED\\SECONDARY\\MPR");
    newImage.getDataset()->putAndInsertString(DcmTag(DcmTagKey(0x0028, 0x1052)), std::to_string(rescaleIntercept).c_str());
    newImage.getDataset()->putAndInsertString(DcmTag(DcmTagKey(0x0028, 0x1053)), std::to_string(rescaleSlope).c_str());
    newImage.getDataset()->putAndInsertFloat32Array(DcmTagKey(0x7FE0,0x0008), pixels, width * height);
    newImage.getDataset()->putAndInsertUint16(DcmTag(DcmTagKey(0x0028, 0x0100)), 32);

    char uid[64];
    newImage.getDataset()->putAndInsertString(DcmTagKey(0x0020,0x000E), dcmGenerateUniqueIdentifier(uid)); //Series instance UID
    newImage.getDataset()->putAndInsertUint16(DcmTagKey(0x0200,0x0011), 1); //Series Number
    newImage.getDataset()->putAndInsertUint16(DcmTagKey(0x0200,0x0013), 1); //Instance Number
    newImage.getDataset()->putAndInsertString(DcmTagKey(0x0020,0x0016), dcmGenerateUniqueIdentifier(uid)); //SOP instance UID
    newImage.getDataset()->putAndInsertString(DcmTagKey(0x0002,0x0003), uid); //Media Storage SOP instance UID

    newImage.saveFile("derived.dcm");
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