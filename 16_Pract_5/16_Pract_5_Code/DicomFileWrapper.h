#ifndef LAB5_DICOMFILEWRAPPER_H
#define LAB5_DICOMFILEWRAPPER_H

#include <iostream>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

class DicomFileWrapper {
private:
    DcmFileFormat *dcmFileFormat;
    DicomImage *dicomImage;

public:
    DicomFileWrapper(const std::string &imagePath);

    int getImageWidth();
    int getImageHeight();

    Float64 getDouble(const DcmTagKey &dcmTagKey);
    OFVector<Float64> getDoubleArray(const DcmTagKey &dcmTagKey, unsigned long size);
    const void *getImageOutputData(int bits = 0);

};


#endif //LAB5_DICOMFILEWRAPPER_H
