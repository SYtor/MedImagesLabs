#ifndef LAB4_DICOMFILEWRAPPER_H
#define LAB4_DICOMFILEWRAPPER_H

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
    DcmCodeString getCodeString(const DcmTagKey& dcmTagKey);
    double getDouble(const DcmTagKey& dcmTagKey);
    std::string getString(const DcmTagKey &dcmTagKey);
    const unsigned char* getUCharArray(const DcmTagKey &dcmTagKey);
    void saveTransformedImage(int width, int height, const unsigned char* pixels, double rescaleSlope, double rescaleIntercept);

    static std::pair<double, double> findMinMaxPixel(int width, int height, const unsigned char* pixels);

};


#endif //LAB4_DICOMFILEWRAPPER_H
