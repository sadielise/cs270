// File: struct.h

// Structure definition
typedef struct
{
    char firstName[80];
    char lastName[80];
    int homeworkScore;
    int labScore;
    int midtermScore;
    int finalScore;
    float totalPoints;
    char finalGrade;

} Student;

// Function Declarations 
void inputScores(Student *student);
void outputScores(Student student);
void calculateScores(Student *student);
