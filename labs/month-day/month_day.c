#include <stdio.h>
#include <stdlib.h>

/*Francisco Mariano Amézquita Ramos*/
int validDate = 1;
//days of the months of a year and a leap year
static char daysOfMonths[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};
//nameOfMonthss of the month
static char *nameOfMonths[] = {
   "INVALID DATE",
   "January", "February", "March",
   "April", "May", "June",
   "July", "August", "September",
   "October", "November", "December"
};

void month_day(int year, int yearday, int *pmonth, int *pday){

    //if leap is 1-> true
    int leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    if(yearday > 365 + leap){
        printf("Yearday exceeds the year days\n");
        validDate = 0;
        return;
    }
    while(yearday > daysOfMonths[leap][*pmonth]){
        yearday -= daysOfMonths[leap][*pmonth];
        (*pmonth)++;

    }
    *pday = yearday;
}

int main(int argc, char **argv) {
    int year;
    int yearday;
    int month = 0;
    int day = 0;

    //validate arguments
    if(argc!=3){
        printf("Please insert data as follows: <year> <yearday>\n");
        return -1;
    }
    year = atoi(argv[1]);
    yearday = atoi(argv[2]);
    if(year <= 0 || yearday <= 0){
        printf("Year and Yearday must be higher than 0\n");
        return -1;
    }
    month_day(year, yearday, &month, &day);
    if(validDate){
        printf("%s %02d, %d\n", nameOfMonths[month], day, year);
    }
    
    return 0;
}