#ifndef LAB4_DICOMFILEWRAPPER_H
#define LAB4_DICOMFILEWRAPPER_H

#include <iostream>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

class DicomFileWrapper {
private:
    DcmFileFormat* dcmFileFormat;
    DicomImage* dicomImage;

public:
    DicomFileWrapper(const std::string& imagePath);
    int getImageWidth();
    int getImageHeight();
    DcmCodeString getCodeString(const DcmTagKey& dcmTagKey);
    double getDouble(const DcmTagKey& dcmTagKey);
    const void *getImageOutputData(int bits = 0);
    std::pair<double, double> findMinMaxPixel();
    void saveTransformedImage();
};


#endif //LAB4_DICOMFILEWRAPPER_H
