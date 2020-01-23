#include "DicomFileWrapper.h"

DicomFileWrapper::DicomFileWrapper(const std::string &imagePath) {
    dcmFileFormat = new DcmFileFormat();
    dcmFileFormat->loadFile(imagePath.c_str());
}

unsigned short DicomFileWrapper::getUShort(const DcmTagKey &dcmTagKey) {
    unsigned short value = 0;
    dcmFileFormat->getDataset()->findAndGetUint16(dcmTagKey, value);
    return value;
}

double DicomFileWrapper::getDouble(const DcmTagKey &dcmTagKey) {
    Float64 value = 0;
    dcmFileFormat->getDataset()->findAndGetFloat64(dcmTagKey, value);
    return value;
}

OFVector<double> DicomFileWrapper::getDoubleArray(const DcmTagKey& dcmTagKey, unsigned long size) {
    const char * stringValue = nullptr;
    auto condition = dcmFileFormat->getDataset()->findAndGetString(dcmTagKey, stringValue);
    OFVector<Float64> arrayValue;
    OFString ofstr(stringValue);
    DcmDecimalString str(DcmTag(dcmTagKey), ofstr.size());
    str.putString(stringValue);
    auto condition2 = str.getFloat64Vector(arrayValue);
    return arrayValue;
}

const unsigned char* DicomFileWrapper::getUCharArray(const DcmTagKey &dcmTagKey) {
    const unsigned char *value;
    dcmFileFormat->getDataset()->findAndGetUint8Array(dcmTagKey, value);
    return value;
}
