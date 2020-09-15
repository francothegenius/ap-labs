//opening a file
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//closing a file
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFFSIZE 500
#define SIZEPACKAGES 5000
#define ATTSIZE 120


#define REPORT_FILE "packages_report.txt"

void analizeLog(char *logFile, char *report);
int getLine(FILE *file, char *buffer);
char* packageDate(char* line);
char* packageType(char* line);
char* packageName(char* line);


struct package
{
    //atributes
    char name[ATTSIZE];
    char dateInstalled[ATTSIZE];
    char dateUpgraded[ATTSIZE];
    char dateRemoved[ATTSIZE];
    int nOfUpgrades;
};

struct package packages[SIZEPACKAGES];

//gives the line in the buffer
//returns 0 if theres no line to read
//returns > 1 if theres line to read
//returns int besides the line itself(char) to save memory
int getLine(FILE *file, char *buffer){
    int numberOfChars = 0;
    char c;
    //we dont need to read the whole line since the data is at the beggining of line
    //so we optimize by limiting the numberOfChars read by the BUFFSIZE
    while ((c = getc(file)) != '\n' && numberOfChars < BUFFSIZE){
        if (c == EOF){
            break;
        }
        buffer[numberOfChars] = c;
        numberOfChars++;
        
    }
    if (numberOfChars == 0){
        return 0;
    }
    buffer[numberOfChars] ='\0';
    return numberOfChars;
    
}

char* packageType(char *line){
    //type of package is found after second ]
    int positionType = 0;
    int positionFound = 0;
    //variable used if already passed the second ]
    while(positionFound != 2 && line[positionType] != '\0' ){
        if(line[positionType] == ']'){
            positionFound++;
        }
        positionType++;
    }
    positionType++;
    char* keyWord = calloc(1, sizeof(char)*7);
    for(int i = 0; i < 7; i++, positionType++){
        keyWord[i] = line[positionType];
    }
    return keyWord;
}

char* packageDate(char *line){
    //variable used to know the size of the date
    int sizeOfDate = 0;
    /*We know that the date ends in the first ] found*/
    while(line[sizeOfDate] != ']'){
        sizeOfDate++;
    }
    //delete the [ of the size
    sizeOfDate = sizeOfDate-1;
    char* date = calloc(1, sizeof(char)*sizeOfDate);

    int j = 1;
    for(int i = 0; i < sizeOfDate;i++){
        date[i] = line[j];
        j++;
    }
    return date;
}

char* packageName(char *line){
    /*We know that the name is after the packagetype and before ' '*/
    /*First, we get after the package type*/
    int positionType = 0;
    int positionFound = 0;
    //variable used if already passed the second ]
    while(positionFound != 2 && line[positionType] != '\0'){
        if(line[positionType] == ']'){
            positionFound++;
        }
        positionType++;
    }
    positionType++;

    //pass the package type
    while(line[positionType] != ' ' && line[positionType] != '\0'){
        positionType++;
    }
    //add one to position to skip space
    //here we find the name of the package
    positionType++;

    //save the index of name position
    int nameStart = positionType;
    //save size of name
    int sizeOfName = 0;
    while(line[positionType] != ' ' && line[positionType] != '\0'){
        sizeOfName++;
        positionType++;
    }
    char* name = calloc(1, sizeof(char)*sizeOfName);
    for(int i = 0; i < sizeOfName; i++, nameStart++){
        name[i] = line[nameStart];
    }
    return name;
}


int main(int argc, char **argv) {
    if (argc != 5 || strcmp(argv[1], "-input") != 0 || strcmp(argv[3], "-report")!=0 ) {
        printf("ERROR on arguments. Try:\n");
        printf("./pacman-analizer -input pacman.txt -report packages_report.txt\n");
        return 1;
    } 
    
    analizeLog(argv[2], argv[4]);

    return 0;  
}

void analizeLog(char *logFile, char *report) {

    FILE* fd;
    fd = fopen(logFile, "r");
    if(fd == NULL){
        printf("Failed to open and read the file %s\n", logFile);
        return;
    }
    int packagesInstalled = 0;
    int packagesRemoved = 0;
    int packagesUpgraded = 0;
    int currentPackage = 0;
    printf("Generating Report from: [%s] log file\n", logFile);
    char lineBuff[BUFFSIZE];
    int line;
    //pointers
    char* type;
    char* name;
    char* date;
    //save data from pointers
    char typeS[ATTSIZE];
    char nameS[ATTSIZE];
    char dateS[ATTSIZE];
    while((line = getLine(fd, lineBuff)) > 0){
        //we get the type of the package
        type = packageType(lineBuff);
        strcpy(typeS, type);
        if(strcmp(typeS, "install") == 0){
        	name = packageName(lineBuff);
        	strcpy(nameS, name);
        	date = packageDate(lineBuff);
        	strcpy(dateS, date);
            //save in the struct
            strcpy(packages[currentPackage].name, nameS);
            strcpy(packages[currentPackage].dateInstalled, dateS);
            //since is starting, there are no upgrades or removal date
            packages[currentPackage].nOfUpgrades = 0;
            strcpy(packages[currentPackage].dateRemoved, "-");
            //move to the next space in the struct
            currentPackage++;
            //we add an installation package
            packagesInstalled++;
        }
        else if(strcmp(typeS, "upgrade") == 0){
        	name = packageName(lineBuff);
        	strcpy(nameS, name);
        	date = packageDate(lineBuff);
        	strcpy(dateS, date);
            // since it´s an upgrade, there already exists an installation
            // so we need to check in our package array
            for(int i = 0; i < SIZEPACKAGES; i++){
                //if we find in array
                if(strcmp(nameS, packages[i].name) == 0){
                    //we update the date
                    strcpy(packages[i].dateUpgraded, dateS);
                    packages[i].nOfUpgrades++;
                    break;
                }
            }
            packagesUpgraded++;
        }
        else if(strcmp(typeS, "removed") == 0){
        	name = packageName(lineBuff);
        	strcpy(nameS, name);
        	date = packageDate(lineBuff);
        	strcpy(dateS, date);       
            for(int i = 0; i < SIZEPACKAGES; i++){
                if(strcmp(nameS, packages[i].name) == 0){
                    strcpy(packages[i].dateRemoved, dateS);
                }
                break;
            }
            packagesRemoved++;
        }
    }
    //creating report file
    //open with system call
    int fileReport = open(report, O_CREAT | O_WRONLY, 0600);
    if(fileReport == -1){
        printf("Failed to create and open the %s file\n", report);
        exit(1);
    }
    //writing in file

    //summary
    char intToChar[12];
    //write with system call
    write(fileReport,"Pacman Packages Report\n", strlen("Pacman Packages Report\n"));
    write(fileReport, "----------------------\n",strlen("----------------------\n"));
    write(fileReport, "- Installed packages : ", strlen("- Installed packages :"));
    sprintf(intToChar," %d\n",packagesInstalled);
    write(fileReport, intToChar, strlen(intToChar));
    write(fileReport, "- Removed packages   : ", strlen("- Removed packages  : "));
    sprintf(intToChar," %d\n",packagesRemoved);
    write(fileReport, intToChar, strlen(intToChar));
    write(fileReport, "- Upgraded packages  :", strlen("- Upgraded packages  :"));
    sprintf(intToChar," %d\n",packagesUpgraded);
    write(fileReport, intToChar, strlen(intToChar));
    write(fileReport, "- Current installed  :", strlen("- Current installed  :"));
    sprintf(intToChar," %d\n",packagesInstalled - packagesRemoved);
    write(fileReport, intToChar, strlen(intToChar));

    //list of packages
    write(fileReport,"\nList of packages\n", strlen("\nList of packages\n"));
    write(fileReport,"----------------\n", strlen("----------------\n"));

    //struct package array
    for(int i = 0; i < SIZEPACKAGES; i++){
        //check if in the position is not empty
        if(strcmp(packages[i].name, "") != 0){
            write(fileReport,"- Package Name          : ", strlen("- Package Name          : "));
            write(fileReport,packages[i].name, strlen(packages[i].name));
            write(fileReport,"\n    - Install date      : ", strlen("\n    - Install date      : "));
            write(fileReport,packages[i].dateInstalled, strlen(packages[i].dateInstalled));
            write(fileReport,"\n    - Last update date  : ", strlen("\n    - Last update date  : "));
            write(fileReport,packages[i].dateUpgraded, strlen(packages[i].dateUpgraded));
            write(fileReport,"\n    - How many updates  : ", strlen("\n    - How many updates  : "));
            sprintf(intToChar, "%d", packages[i].nOfUpgrades);
            write(fileReport,intToChar, strlen(intToChar));
            write(fileReport,"\n    - Removal date      : ", strlen("\n    - Removal date      : "));
            write(fileReport,packages[i].dateRemoved, strlen(packages[i].dateRemoved));
            write(fileReport, "\n", strlen("\n"));
            
        }else{
            break;
        }
        
    }
    //close with system call
    if (close(fileReport) < 0)  { 
        perror("Error trying to close file"); 
        exit(1); 
    }

    fclose(fd);

    printf("Report is generated at: [%s]\n", report);
}
