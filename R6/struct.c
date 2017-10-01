// File: struct.c

// Include files
#include <stdio.h>
#include "struct.h"

// Input scores
void inputScores(Student *student)
{
    printf("First name: ");
    scanf("%s", student->firstName);
    printf("Last name: ");
    scanf("%s", student->lastName);
    printf("average homework score: ");
    scanf("%d", &student->homeworkScore);
    printf("average lab score: ");
    scanf("%d", &student->labScore);
    printf("midterm score: ");
    scanf("%d", &student->midtermScore);
    printf("final score: ");
    scanf("%d", &student->finalScore);


}

// Output scores
void outputScores(Student student)
{
    printf("First name: %s\n", student.firstName);
    printf("Last name: %s\n", student.lastName);
    printf("Homework: %d\n", student.homeworkScore);
    printf("Lab: %d\n", student.labScore);
    printf("Midterm: %d\n", student.midtermScore);
    printf("Final: %d\n", student.finalScore);
    printf("Total Points: %f\n", student.totalPoints);
    printf("Letter Grade: %c\n", student.finalGrade);
}

//calculateScores
void calculateScores(Student *student)
{
    student->totalPoints = (student->homeworkScore*0.3) + (student->labScore*0.2) + (student->midtermScore*0.2) + (student->finalScore*0.3);

    if(totalPoints > 90.0) student->finalGrade = 'A';
    else if(totalPoints > 80.0) student->finalGrade = 'B';
    else if(totalPoints > 70.0) student->finalGrade = 'C';
    else if(totalPoints > 60.0) student->finalGrade = 'D';
    else student->finalGrade = 'F';
}
