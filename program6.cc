//Program 6
//Mugdha Gupta
//mxg167030@utdallas.edu
//CS 3377.002
//program6.cc
//This program will read from bin file and display info


#include <iostream>
#include "cdk.h"
#include <fstream>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <sstream>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryFileHeader{
    public:
    uint32_t magicNumber;
    uint32_t versionNumber;
    uint64_t numRecords;
    };

const int maxRecordStringLength = 25;

class BinaryFileRecord{
    public:
    uint8_t strLength;
    char stringBuffer[maxRecordStringLength];
    };

int main()
{
  std::string magicNumberString, versionNumberString, numRecordsString, strlenString, actualString;
  std::stringstream ss;
  BinaryFileHeader *myHeader = new BinaryFileHeader();

  ifstream myBinFile ("cs3377.bin", ios::in|ios::binary);

  myBinFile.read((char *) myHeader, sizeof(BinaryFileHeader));
  //std::string magicNum = std::to_string(myHeader->magicNumber);

  ss << hex << uppercase << myHeader->magicNumber;
  magicNumberString = ss.str();
  ss.str(std::string());

  ss << myHeader->versionNumber;
  versionNumberString = ss.str();
  ss.str(std::string());

  ss << myHeader->numRecords;
  numRecordsString = ss.str();
  ss.str(std::string());

    cout << magicNumberString << " " << versionNumberString << " " << numRecordsString << endl;

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX *myMatrix;           // CDK Screen Matrix

  const char 		*rowTitles[MATRIX_WIDTH+1] = {"unused", "a", "b", "c"};
  const char 		*columnTitles[MATRIX_HEIGHT+1] = {"unused", "a", "b", "c", "d", "e"};
  int		boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) columnTitles, (char **) rowTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */
  setCDKMatrixCell(myMatrix, 1, 1, ("Magic: 0x" + magicNumberString).c_str());
  setCDKMatrixCell(myMatrix, 1, 2, ("Version: " + versionNumberString).c_str());
  setCDKMatrixCell(myMatrix, 1, 3, ("NumRecords: " +numRecordsString).c_str());
  
    ss << dec << "";

  for(uint8_t i =0; i < myHeader->numRecords; i++){
    BinaryFileRecord myRecord;
    myBinFile.read((char *) &myRecord, sizeof(myRecord));

    ss  <<"strlen: " << static_cast<int>(myRecord.strLength);
    

    setCDKMatrixCell(myMatrix, i+2, 1, ss.str().c_str());
    setCDKMatrixCell(myMatrix, i+2, 2, myRecord.stringBuffer);
    ss.str(std::string());
  }

  drawCDKMatrix(myMatrix, true);    /* required  */
  /* so we can see results */
  fgetc(stdin);


  // Cleanup screen
  endCDK();
}
