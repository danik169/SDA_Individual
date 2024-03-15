#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    double totalPrice;
    char category[MAX_SIZE];
} CategoryTotal;

typedef struct {
    int year;
    int month;
    int day;
    char saleDate[MAX_SIZE];
    int productID;
    char productName[MAX_SIZE];
    char productCategory[MAX_SIZE];
    char productSubcategory[MAX_SIZE];
    double unitPrice;
    unsigned int quantitySold;
    char saleCountry[MAX_SIZE];
    char saleCity[MAX_SIZE];
} SaleRecord;

int countLines(FILE* filePointer) {
    char character;
    int lines = 0;
    while ((character = fgetc(filePointer)) != EOF) {
        if (character == '\n') {
            lines++;
        }
    }
    rewind(filePointer);
    return lines;
}

void readFile(SaleRecord *salesArray, int numLines, FILE* filePointer) {
    for (int i = 0; i < numLines; i++) {
        fscanf(filePointer, "%19[^,],%d,%19[^,],%19[^,],%19[^,],%lf,%d,%29[^,],%29[^\n]",
               salesArray[i].saleDate, &salesArray[i].productID, salesArray[i].productName,
               salesArray[i].productCategory, salesArray[i].productSubcategory, &salesArray[i].unitPrice,
               &salesArray[i].quantitySold, salesArray[i].saleCountry, salesArray[i].saleCity);
        sscanf(salesArray[i].saleDate, "%d-%d-%d", &salesArray[i].year, &salesArray[i].month, &salesArray[i].day);
    }
}

void displaySales(SaleRecord *salesArray, int numElements) {
    for (int i = 0; i < numElements; i++) {
        printf("%d ELEMENT:\n", i + 1);
        printf("Date: %s\n", salesArray[i].saleDate);
        printf("ID: %d\n", salesArray[i].productID);
        printf("Name: %s\n", salesArray[i].productName);
        printf("Category: %s\n", salesArray[i].productCategory);
        printf("Subcategory: %s\n", salesArray[i].productSubcategory);
        printf("Price: %.2lf\n", salesArray[i].unitPrice);
        printf("Quantity: %d\n", salesArray[i].quantitySold);
        printf("Country: %s\n", salesArray[i].saleCountry);
        printf("City: %s\n", salesArray[i].saleCity);
    }
}

void yearlyRevenue(SaleRecord *salesArray, int year, int numLines) {
    double yearlyRevenue = 0, monthlyRevenue = 0;

    for(int month = 1; month <= 12; month++) {
        for (int i = 0; i < numLines; i++) {
            if (year == salesArray[i].year && month == salesArray[i].month ) {
                monthlyRevenue += salesArray[i].unitPrice * salesArray[i].quantitySold;
            }
        }
        if(monthlyRevenue != 0){
            printf("Revenue for month %d: %.2lf$\n", month, monthlyRevenue);
        } else {
            printf("No revenue for month %d\n", month);
        }
        yearlyRevenue += monthlyRevenue;
    }
    if (yearlyRevenue != 0){
        printf("Yearly revenue: %.2lf$\n", yearlyRevenue);
    } else {
        printf("No revenue for the year\n");
    }
}

void topCategories(SaleRecord *salesArray, int numLines) {
    CategoryTotal categories[MAX_SIZE] = {0};

    for (int i = 0; i < numLines; i++) {
        int found = 0;
        for (int j = 0; j < MAX_SIZE; j++) {
           if (strcmp(categories[j].category, salesArray[i].productCategory) == 0) {
                categories[j].totalPrice += salesArray[i].unitPrice * salesArray[i].quantitySold;
                found = 1;
                break;
            }
        }
        if (found == 0) {
            for (int j = 0; j < MAX_SIZE; j++) {
                if (categories[j].totalPrice == 0) {
                    strcpy(categories[j].category, salesArray[i].productCategory);
                    categories[j].totalPrice += salesArray[i].unitPrice * salesArray[i].quantitySold;
                    break;
                }
            }
        }
    }

    // Bubble sort pentru a gasi top 5 categorii
    for (int i = 0; i < MAX_SIZE - 1; i++) {
        for (int j = 0; j < MAX_SIZE - i - 1; j++) {
            if (categories[j].totalPrice < categories[j + 1].totalPrice) {
                CategoryTotal temp = categories[j];
                categories[j] = categories[j + 1];
                categories[j + 1] = temp;
            }
        }
    }

    // Afisarea la cele 5 top companii
    printf("Top five sold categories by total revenue:\n");
    for (int i = 0; i < 5; i++) {
        printf("Category: %s, Total Revenue: %.2lf$\n", categories[i].category, categories[i].totalPrice);
    }
}

void totalSalesByCategory(SaleRecord *salesArray, int numLines, char *category) {
    double totalSales = 0;

    for (int i = 0; i < numLines; i++) {
        if (strcmp(salesArray[i].productCategory, category) == 0) {
            totalSales += salesArray[i].unitPrice * salesArray[i].quantitySold;
        }
    }

    printf("Total sales for category '%s': %.2lf$\n", category, totalSales);
}

void cityWithMaxSalesInCountry(SaleRecord *salesArray, int numLines, char *country) {
    double maxSales = 0;
    char maxSalesCity[MAX_SIZE] = "";

    for (int i = 0; i < numLines; i++) {
        if (strcmp(salesArray[i].saleCountry, country) == 0) {
            double currentSales = salesArray[i].unitPrice * salesArray[i].quantitySold;
            if (currentSales > maxSales) {
                maxSales = currentSales;
                strcpy(maxSalesCity, salesArray[i].saleCity);
            }
        }
    }

    if (maxSales > 0) {
        printf("City with max sales in %s: %s (Total sales: %.2lf$)\n", country, maxSalesCity, maxSales);
    } else {
        printf("No sales found in %s\n", country);
    }
}


int main() {
    int numLines, choice, year, numElements;
    SaleRecord *salesArray;
    char category[MAX_SIZE], country[MAX_SIZE];

    FILE *filePointer = fopen("/Users/ceagleidanil/Univer/probSDA/problemasales/sales.txt", "rw+");
    if (filePointer == NULL) {
        printf("The file cannot be read.");
        return 0;
    }

    numLines = countLines(filePointer);
    salesArray = malloc(sizeof(SaleRecord) * numLines);

    readFile(salesArray, numLines, filePointer);

    fclose(filePointer);

   do {
       printf("1. Find yearly and monthly revenue\n");
       printf("2. Top 5 categories by revenue\n");
       printf("3. Sales per category\n");
       printf("4. City with maximum sales in a country\n");
       printf("5. Print some elements to verify file integrity\n");
       printf("0. Exit\n");
       printf("Choice:");
       scanf("%d", &choice);
       switch (choice) {
           case 1:
               year = 0;
               printf("Enter the year for revenue calculation:");
               scanf("%d", &year);
               yearlyRevenue(salesArray, year, numLines);
               break;
           case 2:
               topCategories(salesArray, numLines);
               break;
           case 3:
               printf("Enter category: ");
               scanf("%s", category);
               totalSalesByCategory(salesArray, numLines, category);
               break;
           case 4:
               printf("Enter country: ");
               scanf("%s", country);
               cityWithMaxSalesInCountry(salesArray, numLines, country);
               break;
           case 5:
               printf("How many elements do you want to print:");
               scanf("%d", &numElements);
               if (numLines < numElements || numElements < 0) {
                   printf("Please provide a valid number");
                   break;
               } else {
                   displaySales(salesArray, numElements);
               }
               break;
           case 0:
               break;
       }
   } while (choice != 0);
    free(salesArray);
    return 0;
}