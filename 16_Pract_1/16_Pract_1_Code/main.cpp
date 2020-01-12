#include <iostream>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

using namespace std;

int main() {

    string in_file {"77654033/20010101/CR1/6154"};

    DcmFileFormat file_format;
    OFCondition status = file_format.loadFile(in_file.c_str());

    if (status.bad()) {
        cerr << "Problem opening file:" << in_file << endl;
        return 1;
    }

    DcmDataset* dataset = file_format.getDataset();

    OFString patient_name;
    OFCondition condition;
    condition = dataset->findAndGetOFStringArray(DCM_PatientName, patient_name);

    if (condition.good())  {
        cout << "Patient name is: " << patient_name << endl;
    } else {
        cerr << "Could not get patient name" << endl;
    }

    return 0;
}