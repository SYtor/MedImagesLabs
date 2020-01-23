#ifndef LAB5_DICOMFILEWRAPPER_H
#define LAB5_DICOMFILEWRAPPER_H

#include <iostream>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

class DicomFileWrapper {
private:
    DcmFileFormat *dcmFileFormat;
public:
    DicomFileWrapper(const std::string &imagePath);
    unsigned short getUShort(const DcmTagKey &dcmTagKey);
    double getDouble(const DcmTagKey& dcmTagKey);
    OFVector<double> getDoubleArray(const DcmTagKey &dcmTagKey, unsigned long size);
    const unsigned char* getUCharArray(const DcmTagKey &dcmTagKey);

};


#endif //LAB5_DICOMFILEWRAPPER_H
