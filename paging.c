#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct page {
 char *name; //alphanumeric name
 int referenced; //referenced bit
 int time_since_ref; //used to calculate referenced bit
 int load_time; //amount of time since page was loaded
 struct page *next; //pointer to the next struct page
} page;

page * createPage(int interval) 
{
    page * newpage = malloc(sizeof(page));
    
    char * pageName = malloc(15);
    int timeSinceRef;
    int loadTime;
    
    printf("NAME: ");
    scanf("%s", pageName);
    printf("TIME SINCE LAST REFERENCE: ");
    scanf("%d", &timeSinceRef);
    printf("LOAD TIME: ");
    scanf("%d", &loadTime);
    
    newpage->name = pageName;
    newpage->time_since_ref = timeSinceRef;
    newpage->load_time = loadTime;
    
    if (timeSinceRef <= interval)
    {
        newpage->referenced = 1;
    }
    else
    {
        newpage->referenced = 0;
    }
    
    return newpage;
    
}

void printQueue(page *first, int count)
{
    page *currentPage = first;
    
    printf("\nname: %15s|time: %5d|ref: %5d", currentPage->name, currentPage->load_time, currentPage->referenced);
    if (count > 1)
    {
        for (int k = 0; k < count-1; k++)
        {
            currentPage = currentPage->next;
            printf("\nname: %15s|time: %5d|ref: %5d", currentPage->name, currentPage->load_time, currentPage->referenced);
            if (k == count - 2)
            {
                printf("--BACK TO TOP->\n");
            }
        }
    }
}

page * addPage(page *newPage, page *firstPage, int count)
{
    
    if (count == 1)
    {
        newPage->next = newPage;
        return newPage;
    }
    
    page *currentPage = firstPage;
    page *nextPage = currentPage->next;
    page *lastPage = firstPage;
    
    //find last page
    for (int j = 0; j < count - 1; j++)
    {
        lastPage = lastPage->next;
    }
    
    for (int i = 0; i < count; i++)
    {
        currentPage = currentPage->next;
        nextPage = nextPage->next;
        
        //if newpage goes first
        if (newPage->load_time <= firstPage->load_time)
        {
            newPage->next = firstPage;
            lastPage->next = firstPage;
            return newPage;
        }
        
        //if new page goes second
        else if (newPage->load_time <= currentPage->load_time)
        {
            firstPage->next = newPage;
            newPage->next = currentPage;
            return firstPage;
        }
        
        //other
        else if (newPage->load_time <= nextPage->load_time)
        {
            newPage->next = newPage;
            currentPage->next = newPage;
            return firstPage;
        }
        
        //if new page goes last
        else if (newPage->load_time > lastPage->load_time)
        {
            lastPage->next = newPage;
            newPage->next = firstPage;
            return firstPage;
        }
        
    }
}

page * replace(page *newpage, page *first, int count)
{
    page *lastPage = first;
    //find last page
    for (int j = 0; j < count - 1; j++)
    {
        lastPage = lastPage->next;
    }
    
    page *previousPage = first;
    page *currentPage = first; //start with the oldest page, next up for eviction
    while (currentPage->referenced != 0) 
    { //skip through all the pages that have been recently referenced
    currentPage->referenced = 0;
    previousPage = currentPage;
    currentPage = currentPage->next; //move to the next page
    }
    newpage->next = currentPage->next;
    free(currentPage);
    previousPage->next = newpage;
    
}

int main() {
    
    int refInterval;
    int selection;
    int i = 1;
    
    page *newPage;
    page *firstPage;
    int count = 0;
    
    printf("ENTER THE REFERENCE INTERVAL FOR THE ALGORITHM: ");
    scanf("%d", &refInterval);
    
    while(i)
    {
        printf("\nCLOCK PAGE REPLACEMENT ALGORITHM:\n\t1) ADD PAGE\n\t2) DISPLAY QUEUE\n\t3) REPLACE PAGE\n\t4) QUIT\nSELECT AN OPTION: ");
        scanf("%d", &selection);
    
        switch(selection)
        {
            case 1:
                newPage = createPage(refInterval);
                count++;
                if (count == 1)
                {
                    firstPage = newPage;
                }
                firstPage = addPage(newPage, firstPage, count);
                break;
            case 2:
                printQueue(firstPage, count);
                break;
            case 3:
                newPage = createPage(refInterval);
                replace(newPage, firstPage, count);
                break;
            case 4:
                i = 0;
                break;
            default:
                printf("[INVALID SELECTION]\n");
        }
        
    }
   
    
    
    return 0;
}