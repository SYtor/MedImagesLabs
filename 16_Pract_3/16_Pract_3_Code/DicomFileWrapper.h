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
    ~DicomFileWrapper();
    unsigned short getUShort(const DcmTagKey &dcmTagKey);
    std::string getString(const DcmTagKey &dcmTagKey);
    const unsigned char* getUCharArray(const DcmTagKey &dcmTagKey);
};


#endif //LAB5_DICOMFILEWRAPPER_H
