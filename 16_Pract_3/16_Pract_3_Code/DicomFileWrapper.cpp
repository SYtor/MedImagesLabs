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

const unsigned char* DicomFileWrapper::getUCharArray(const DcmTagKey &dcmTagKey) {
    const unsigned char *value;
    dcmFileFormat->getDataset()->findAndGetUint8Array(dcmTagKey, value);
    return value;
}
