#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

// Function to Get which Date is Older by Comparing Two Arrays with Year, Month and Day as Elements
bool getOlderDate(int date1[3], int date2[3])
{
  for (int n = 0; n < 3; n++)
    if (date1[n] != date2[n])
      return date1[n] <= date2[n];

  return false;
}

// Function to Get Movie Rent Date as an Array of Integers
string getCurrentDate(int date[])
{
  time_t t = time(nullptr); // Get Time since Epoch as a Textual Representation
  tm tm = *localtime(&t);   // Get tm Structure

  ostringstream stream; // Get Date as a String

  for (int i = 0; i < 3; i++)
  {
    if (i == 0)
      stream << put_time(&tm, "%Y"); // Save Year
    else if (i == 1)
      stream << put_time(&tm, "%m"); // Save Month
    else
      stream << put_time(&tm, "%d"); // Save Day

    date[i] = stoi(stream.str());
    stream.str(""); // Clear String
  }

  stream << put_time(&tm, "%Y-%m-%d");
  return stream.str(); // Return Stream as a String
}