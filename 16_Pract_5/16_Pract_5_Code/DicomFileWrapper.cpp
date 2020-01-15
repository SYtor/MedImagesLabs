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

Float64 DicomFileWrapper::getDouble(const DcmTagKey &dcmTagKey) {
    Float64 value = 0;
    dcmFileFormat->getDataset()->findAndGetFloat64(dcmTagKey, value);
    return value;
}

OFVector<Float64> DicomFileWrapper::getDoubleArray(const DcmTagKey& dcmTagKey, unsigned long size) {
    const char * stringValue = nullptr;
    auto condition = dcmFileFormat->getDataset()->findAndGetString(dcmTagKey, stringValue);
    OFVector<Float64> arrayValue;
    OFString ofstr(stringValue);
    DcmDecimalString str(DcmTag(dcmTagKey), ofstr.size());
    str.putString(stringValue);
    auto condition2 = str.getFloat64Vector(arrayValue);
    return arrayValue;
}

const void *DicomFileWrapper::getImageOutputData(const int bits) {
    return dicomImage->getOutputData(bits);
}
