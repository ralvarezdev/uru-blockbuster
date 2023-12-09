// Function to Get which Date is Older by Comparing Two Arrays with Year, Month and Day as Elements
bool getOlderDate(int date1[3], int date2[3])
{
  for (int n = 0; n < 3; n++)
    if (date1[n] != date2[n])
      return date1[n] < date2[n];

  return false;
}