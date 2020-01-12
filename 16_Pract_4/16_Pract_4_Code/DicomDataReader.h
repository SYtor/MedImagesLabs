//
// Created by syt0r on 1/12/20.
//

#ifndef LAB4_DICOMDATAREADER_H
#define LAB4_DICOMDATAREADER_H

#include <iostream>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

class DicomDataReader {
private:
    DcmFileFormat* dcmFileFormat;
    DicomImage* dicomImage;

public:
    DicomDataReader(const std::string& imagePath);
    int getImageWidth();
    int getImageHeight();
    std::string getString(const DcmTagKey& dcmTagKey);
    double getDouble(const DcmTagKey& dcmTagKey);
    const void *getImageOutputData(int bits = 0);
    std::pair<double, double> findMinMaxPixel();
};


#endif //LAB4_DICOMDATAREADER_H
