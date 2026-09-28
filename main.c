/*Name: Hala Khalil
  ID: 1231019
  Sec: 1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Course
{
    char courseId[20];
    char courseTitle[100];// Stores the full course name
    int creditHours;
    char semester[20];
    struct Course *next;// Links multiple courses for one student
} Course;

typedef struct Student
{
    char name[100];
    int id;
    char major[100];
    Course *courses;// Head of the linked list of courses
} Student;

typedef struct AVLNode
{
    Student student;  // Each node represents one student
    struct AVLNode *left; // Points to students with smaller IDs
    struct AVLNode *right; // Points to students with larger IDs
    int height;
} AVLNode;


int getHeight(AVLNode *node)
{
    if (node == NULL)
        return -1;// If node is empty, height is -1
    return node->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b; // Return the larger value
}

// Right rotation used to restore AVL balance
AVLNode* rotateRight(AVLNode *node)
{
    AVLNode *leftChild = node->left;
    AVLNode *temp = leftChild->right;

    leftChild->right = node;
    node->left = temp;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    leftChild->height = max(getHeight(leftChild->left), getHeight(leftChild->right)) + 1;

    return leftChild;
}
// Left rotation used to restore AVL balance
AVLNode* rotateLeft(AVLNode *node)
{
    AVLNode *rightChild = node->right;
    AVLNode *temp = rightChild->left;

    rightChild->left = node;
    node->right = temp;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    rightChild->height = max(getHeight(rightChild->left), getHeight(rightChild->right)) + 1;

    return rightChild;
}

int getBalance(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return  getHeight(node->left) - getHeight(node->right);// Balance factor
}


Course* addCourse(Course *head,char *code,char *title,int credits,char *semester)
{

    for (Course *i = head; i!=NULL; i = i->next)// Checks all courses already registered by the student
        if (!strcasecmp(i->courseId, code) && !strcasecmp(i->semester, semester)) // Skip duplicate course in the same semester
        {
            return NULL;
        }
    Course *newCourse = malloc(sizeof(Course));
    if (!newCourse)
        return head;

// Store course information in the new node
    strcpy(newCourse->courseId, code);
    strcpy(newCourse->courseTitle, title);
    strcpy(newCourse->semester, semester);
    newCourse->creditHours = credits;

    newCourse->next = head;//O(1)
    return newCourse;
}
AVLNode* insertStudent(AVLNode *root,char *name, int id, char *major,char *courseId, char *courseTitle,int credits, char *semester)
{

    if (root == NULL)
    {
        AVLNode *newNode = malloc(sizeof(AVLNode));
        if (newNode == NULL)
            return NULL;

        strcpy(newNode->student.name, name);
        strcpy(newNode->student.major, major);
        newNode->student.id = id;
// Passing NULL because the student is new and has no courses yet
        newNode->student.courses = addCourse(NULL, courseId, courseTitle, credits, semester);

        newNode->left = newNode->right = NULL;
        newNode->height = 0;//leaf
        return newNode;
    }

    if (id < root->student.id)// Go left if ID is smaller
        root->left = insertStudent(root->left, name, id, major, courseId, courseTitle, credits, semester);
    else if (id > root->student.id)// Go right if ID is larger
        root->right = insertStudent(root->right, name, id, major, courseId, courseTitle, credits, semester);
    else // Same ID: add course to existing student
    {
        Course *newList = addCourse(root->student.courses,
                                    courseId, courseTitle, credits, semester);

        if (newList == NULL)
            return root;

        root->student.courses = newList;
        return root;
    }
// Update the height of the current node after insertion
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

    int balance = getBalance(root);// Calculate balance

    if (balance > 1 && id < root->left->student.id)// Left Left Case
        return rotateRight(root);

    if (balance < -1 && id > root->right->student.id)// Right Right Case
        return rotateLeft(root);

    if (balance > 1 && id > root->left->student.id)//Left Right Case
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && id < root->right->student.id)// Right Left Case
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

AVLNode* loadFromFile(AVLNode *root)
{
    FILE *fp = fopen("reg.txt", "r");
    char line[300];          // store one line from the file
    int lineNo = 0;          // line counter
//validation
    if (fp == NULL)
    {
        printf("Error opening reg.txt\n");
        return root;
    }

    while (fgets(line, sizeof(line), fp))
    {
        lineNo++;

        char *name= strtok(line, "#");
        char *idStr = strtok(NULL, "#");
        char *major = strtok(NULL, "#");
        char *courseId = strtok(NULL, "#");
        char *courseTitle = strtok(NULL, "#");
        char *creditStr = strtok(NULL, "#");
        char *semester = strtok(NULL, "\n");

        if (!name || !idStr || !major || !courseId || !courseTitle || !creditStr || !semester)
        {
            printf("ERROR: Invalid file format at line %d.\n", lineNo);
            fclose(fp);
            return root;
        }

        int id = 0;
        for (int i = 0; idStr[i] != '\0'; i++)
        {
            if (idStr[i] < '0' || idStr[i] > '9')
            {
                printf("ERROR: Invalid student ID at line %d.\n", lineNo);
                fclose(fp);
                return root;
            }
            id = id * 10 + (idStr[i] - '0');//convert to int
        }

        int credits = 0;
        for (int i = 0; creditStr[i] != '\0'; i++)
        {
            if (creditStr[i] < '0' || creditStr[i] > '9')
            {
                printf("ERROR: Invalid credit hours at line %d.\n", lineNo);
                fclose(fp);
                return root;
            }
            credits = credits * 10 + (creditStr[i] - '0');
        }

        root = insertStudent(root, name, id, major,courseId, courseTitle, credits, semester);
    }

    fclose(fp);
    printf("Data loaded successfully.\n");
    return root;
}
// Check if character is a digit
int isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

// Check if character is a letter only (A-Z or a-z)
int isAlpha(char c)
{
    if (c >= 'A' && c <= 'Z') return 1;
    if (c >= 'a' && c <= 'z') return 1;
    return 0;
}

// Check if character is alphanumeric (letter or digit)
int isAlNum(char c)
{
    if (c >= '0' && c <= '9') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    if (c >= 'a' && c <= 'z') return 1;
    return 0;
}

// Validate student name or major (letters only, no spaces)
int checkName(char s[])
{
    if (strlen(s) < 2)
        return 0;

    for (int i = 0; s[i] != '\0'; i++)
        if (!isAlpha(s[i])&& s[i] != ' ')
            return 0;

    return 1;
}

// Validate student ID (must be positive)
int checkStudentID(int id)
{
    return (id > 0);
}

// Validate course ID (alphanumeric, length between 3 and 10)
int checkCourseID(char id[])
{
    if (strlen(id) < 3 || strlen(id) > 10)
        return 0;

    for (int i = 0; id[i] != '\0'; i++)
        if (!isAlNum(id[i]))
            return 0;

    return 1;
}

// Validate credit hours (basic range check)
int checkCredits(int c)
{
    return (c > 0 && c <= 6);
}
AVLNode* searchByID(AVLNode *root, int id)
{
    if (!root) return NULL;

    if (id < root->student.id)
        return searchByID(root->left, id);
    else if (id > root->student.id)
        return searchByID(root->right, id);
    else
        return root;
}
AVLNode* insertFromUser(AVLNode *root)
{
    Student temp;
    Course tempC;

    printf("Enter Student ID: ");
    scanf("%d", &temp.id);

    if (!checkStudentID(temp.id))
    {
        printf("Invalid student ID\n");
        return root;
    }
    // Search if student already exists in AVL tree
    AVLNode *found = searchByID(root, temp.id);

    getchar(); // remove '\n' after scanf

    if (found == NULL) //Student does NOT exist (new student)
    {
        printf("Enter Student Name: ");
        gets(temp.name);

        if (!checkName(temp.name))
        {
            printf("Invalid student name\n");
            return root;
        }

        printf("Enter Major: ");
        gets(temp.major);

        if (!checkName(temp.major))
        {
            printf("Invalid major\n");
            return root;
        }
    }
// Case 2: Student already exists

    else
    {
         char choice;

    printf("Student already exists. Add a new course for this student? (y/n): ");
    scanf("%c", &choice);
    getchar();

    if (choice != 'y' && choice != 'Y')
    {
        printf("Operation cancelled\n");
        return root;
    }

// Copy existing student data
    strcpy(temp.name, found->student.name);
    strcpy(temp.major, found->student.major);
}
    printf("Enter Course Code: ");
    scanf("%s", tempC.courseId);

    if (!checkCourseID(tempC.courseId))
    {
        printf("Invalid course code\n");
        return root;
    }

    getchar(); // remove '\n'

    printf("Enter Course Title: ");
    gets(tempC.courseTitle);

    if (strlen(tempC.courseTitle) < 2)
    {
        printf("Invalid course title\n");
        return root;
    }

    printf("Enter Credit Hours: ");
    scanf("%d", &tempC.creditHours);

    if (!checkCredits(tempC.creditHours))
    {
        printf("Invalid credit hours\n");
        return root;
    }

    getchar(); // remove '\n'

    printf("Enter Semester: ");
    gets(tempC.semester);

    if (strlen(tempC.semester) < 3)
    {
        printf("Invalid semester\n");
        return root;
    }
    // Save pointer to old course list (used to detect duplicates)
    AVLNode *before = searchByID(root, temp.id);
    // Insert student or add course using AVL insertion
    Course *oldCourses = before ? before->student.courses : NULL;

    root = insertStudent(root,temp.name,temp.id,temp.major,tempC.courseId,tempC.courseTitle,tempC.creditHours,tempC.semester);
    // Search again after insertion
    AVLNode *after = searchByID(root, temp.id);

    // If course list did not change, course was duplicate
    if (before && after && oldCourses == after->student.courses)
    {
        printf("Course already registered for this semester\n");
    }
    else
    {
        printf("Registration added successfully.\n");
    }

    return root;

}
void addCourseFromUpdate(Student *s)
{
    char code[20], title[100], semester[20];
    int credits;

    printf("Enter course code: ");
    scanf("%s", code);
    getchar();

    if (!checkCourseID(code))
    {
        printf("Invalid course code\n");
        return;
    }

    printf("Enter course title: ");
    gets(title);

    if (strlen(title) < 2)
    {
        printf("Invalid course title\n");
        return;
    }

    printf("Enter credit hours: ");
    scanf("%d", &credits);
    getchar();

    if (!checkCredits(credits))
    {
        printf("Invalid credit hours\n");
        return;
    }

    printf("Enter semester: ");
    gets(semester);

    if (strlen(semester) < 3)
    {
        printf("Invalid semester\n");
        return;
    }
    // Try to add course to student's course list
    Course *newList = addCourse(s->courses, code, title, credits, semester);
    // If addCourse returns NULL, course already exists in same semester
    if (!newList)
        printf("Course already registered for this semester\n");
    else
    {
        s->courses = newList;
        printf("Course added successfully\n");
    }
}

void deleteCourse(Student *s)
{
    char code[20];
    printf("Enter course code to delete: ");
    gets(code);
    // Search for the course in the linked list
    Course *curr = s->courses, *prev = NULL;

    while (curr && strcasecmp(curr->courseId, code))
    {
        prev = curr;
        curr = curr->next;
    }

    if (!curr)
    {
        printf("Course not found\n");
        return;
    }

    if (!prev)
        s->courses = curr->next;
    else
        prev->next = curr->next;

// Free memory of deleted course
    free(curr);
    printf("Course deleted successfully\n");
}


void listStudentsByName(AVLNode *root, char *name, int *count)
{
    if (!root) return;

    listStudentsByName(root->left, name, count);
    // Check current node
    if (!strcasecmp(root->student.name, name))
    {
        printf("%s %d %s\n",root->student.name, root->student.id,root->student.major);
        (*count)++;
    }

    listStudentsByName(root->right, name, count);
}

void updateCourse(Student *s)
{
    char code[20];
    printf("Enter course code to update: ");
    gets(code);

    Course *c = s->courses;
    while (c && strcasecmp(c->courseId, code))
        c = c->next;

    if (!c)
    {
        printf("Course not found\n");
        return;
    }

    int choice;
    printf("1. Update course title\n");
    printf("2. Update credit hours\n");
    printf("3. Update semester\n");
    printf("Choice: ");
    scanf("%d", &choice);
    getchar();

        // Update course title
    if (choice == 1)
    {
        printf("Enter new course title: ");
        gets(c->courseTitle);

        if (strlen(c->courseTitle) < 2)
        {
            printf("Invalid course title\n");
            return;
        }

        printf("Course title updated successfully\n");
    }
        // Update credit hours
    else if (choice == 2)
    {
        int cr;
        printf("Enter new credit hours: ");
        scanf("%d", &cr);
        getchar();

        if (!checkCredits(cr))
        {
            printf("Invalid credit hours\n");
            return;
        }

        c->creditHours = cr;
        printf("Credit hours updated successfully\n");
    }
        // Update semester
    else if (choice == 3)
    {
        printf("Enter new semester: ");
        gets(c->semester);

        if (strlen(c->semester) < 3)
        {
            printf("Invalid semester\n");
            return;
        }

        printf("Semester updated successfully\n");
    }
    else
        printf("Invalid choice\n");
}

void updateStudentByID(AVLNode *root, int id)
{
        // Search student by ID in AVL tree
    AVLNode *node = searchByID(root, id);

    if (!node)
    {
        printf("Student not found\n");
        return;
    }

    int choice;
    do
    {
        printf("\n1. Update name\n");
        printf("2. Update major\n");
        printf("3. Add course\n");
        printf("4. Delete course\n");
        printf("5. Update course\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

                // Update student name
        if (choice == 1)
        {
            char newName[100];
            printf("Enter new name: ");
            gets(newName);

            if (!checkName(newName))
                printf("Invalid name\n");
            else
            {
                strcpy(node->student.name, newName);
                printf("Name updated successfully\n");
            }
        }
                // Update student major
        else if (choice == 2)
        {
            char newMajor[100];
            printf("Enter new major: ");
            gets(newMajor);

            if (!checkName(newMajor))
                printf("Invalid major\n");
            else
            {
                strcpy(node->student.major, newMajor);
                printf("Major updated successfully\n");
            }
        }
        else if (choice == 3) // Add new course

        {
            addCourseFromUpdate(&node->student);
        }
        else if (choice == 4)// Delete course
        {
            deleteCourse(&node->student);
        }
        else if (choice == 5) // Update course
        {
            updateCourse(&node->student);
        }
        else if (choice != 0)
        {
            printf("Invalid choice\n");
        }

    } while (choice != 0);
}

void listByCourse(AVLNode *root, char *code, int *found)
{
    if (!root) return;

    listByCourse(root->left, code, found);

    // Check all courses for current student
    for (Course *c = root->student.courses; c; c = c->next)
        if (!strcasecmp(c->courseId, code))
        {
            printf("%s %d\n", root->student.name, root->student.id);
            *found = 1;
        }

    listByCourse(root->right, code, found);
}

AVLNode* minValueNode(AVLNode* node)
{
    AVLNode* current = node;

    // Go to leftmost node
    while (current && current->left != NULL)
        current = current->left;

    return current;
}
void freeCourses(Course *c)
{
    while (c)
    {
        Course *temp = c;
        c = c->next;
        free(temp);
    }
}

AVLNode* deleteAVLNode(AVLNode* root, int id)
{
    if (root == NULL)
        return root;

    if (id < root->student.id)
        root->left = deleteAVLNode(root->left, id);

    else if (id > root->student.id)
        root->right = deleteAVLNode(root->right, id);

    else
    {
        // node with one child or no child
        if (root->left == NULL || root->right == NULL)
        {
            AVLNode *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            {
                *root = *temp;
            }
            freeCourses(temp->student.courses);
            free(temp);
        }
        else
        {
            // node with two children
            AVLNode* temp = minValueNode(root->right);

            root->student = temp->student;
            root->right = deleteAVLNode(root->right, temp->student.id);
        }
    }

    if (root == NULL)
        return root;

    // update height
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

    int balance = getBalance(root);

    // Left Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    // Left Right
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Right Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    // Right Left
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}
void deleteStudentRegistration(AVLNode **root)
{
    int id;

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    if (!checkStudentID(id))
    {
        printf("Invalid student ID\n");
        return;
    }

    if (searchByID(*root, id) == NULL)
    {
        printf("Student not found\n");
        return;
    }

    *root = deleteAVLNode(*root, id);
    printf("Student registration deleted.\n");
}


void saveInorder(AVLNode *root, FILE *fp)
{
    if (!root) return;

    saveInorder(root->left, fp);

    for (Course *c = root->student.courses; c; c = c->next)
        fprintf(fp, "%s#%d#%s#%s#%s#%d#%s\n",root->student.name,root->student.id,root->student.major,c->courseId,c->courseTitle,c->creditHours,c->semester);

    saveInorder(root->right, fp);
}

void saveAVLtoHashFile(AVLNode *root)
{
    FILE *fp = fopen("students_hash.data", "w");
    if (!fp)
    {
        printf("file not found");
        return;
    }
    saveInorder(root, fp);
    printf("Save successfully");
    fclose(fp);
}
void avlMenu()
{
    AVLNode *root = NULL;
    int choice;
    char name[100], courseCode[20];

    do
    {
        printf("\n--- AVL Menu ---\n");
        printf("1. Load data from reg.txt\n");
        printf("2. Insert new registration\n");
        printf("3. Find student by name and update\n");
        printf("4. List students in same course\n");
        printf("5. Delete student registration\n");
        printf("6. Save data to students_hash.data\n");
        printf("7. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            root = loadFromFile(root);
            break;
        case 2:
            root = insertFromUser(root);
            break;
        case 3:
        {
            int found = 0;

            printf("Student name: ");
            getchar();
            gets(name);
            int count = 0;

            listStudentsByName(root, name, &count);

            if (count == 0)
            {
                printf("Student not found\n");
            }
            else
            {
                int id;
                printf("Enter Student ID: ");
                scanf("%d", &id);
                getchar();
                updateStudentByID(root, id);
            }

            break;
        }
        case 4:
        {
            int found = 0;

            printf("Course code: ");
            scanf("%s", courseCode);

            listByCourse(root, courseCode, &found);

            if (!found)
                printf("Course not found\n");

            break;
        }


        case 5:
            deleteStudentRegistration(&root);
            break;
        case 6:
            saveAVLtoHashFile(root);
            break;
        }
    }
    while (choice != 7);
}
//*******************************************
// to start in hash

#define TABLE_SIZE 101// 101 is a prime number to help reduce collisions

typedef enum
{
    EMPTY,// the slot has never been used
    OCCUPIED,//the slot currently stores a record
    DELETED//the slot previously stored a record but was deleted
} SlotStatus;

// Represents a single record in the hash table
typedef struct
{
    char name[100];//Key
    int id;
    char major[100];
    char courseId[20];
    char courseTitle[100];
    int creditHours;
    char semester[20];
    SlotStatus status;  // EMPTY, OCCUPIED, or DELETED
} HashEntry;

HashEntry hashTable[TABLE_SIZE];// Index range is from 0 to TABLE_SIZE - 1

// Initializes the hash table
void initHashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
        hashTable[i].status = EMPTY;
}

int hashName(const char *s)
{
    // Computes the hash value for a string and reduces collisions
    int h = 0;
    for (int i = 0; s[i] != '\0'; i++)
        h = (h * 32) + s[i];

    if (h < 0)
        h = -h;// if it a negative value

    return h % TABLE_SIZE;// valid index within the table
}


void printHashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        printf("[%d]: ", i);

        if (hashTable[i].status == EMPTY)
            printf("EMPTY\n");
        else if (hashTable[i].status == DELETED)
            printf("DELETED\n");
        else
            printf("%s %d %s %s %s %d %s\n",hashTable[i].name, hashTable[i].id,hashTable[i].major,hashTable[i].courseId,hashTable[i].courseTitle,hashTable[i].creditHours,hashTable[i].semester);

    }
}


void printHashSize()
{
    printf("Table size: %d\n", TABLE_SIZE);
}

void printHashFunction()
{
    printf("h(key) = (h * 32 + key[i]) %% %d\n", TABLE_SIZE);
}

int insertHash(HashEntry e)
{
    int index = hashName(e.name);

    // Linear probing
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        int pos = (index + i) % TABLE_SIZE;

        // Insert in EMPTY or DELETED slot
        if (hashTable[pos].status == EMPTY || hashTable[pos].status == DELETED)
        {
            hashTable[pos] = e;
            hashTable[pos].status = OCCUPIED;
            return 1;
        }
    }
    return 0; // Table full
}

int searchHash(char *name)
{
    int index = hashName(name);

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        int pos = (index + i) % TABLE_SIZE;

         // Stop search if empty slot found
        if (hashTable[pos].status == EMPTY)
            return -1;
        // Found matching record
        if (hashTable[pos].status == OCCUPIED &&!strcasecmp(hashTable[pos].name, name))
            return pos;
    }
    return -1;
}

void deleteFromHash(char *name)
{
    int pos = searchHash(name);

    if (pos == -1)
    {
        printf("Student not found\n");
        return;
    }

    hashTable[pos].status = DELETED;
    printf("Record deleted successfully\n");
}

void loadHashFromFile()
{
    FILE *fp = fopen("students_hash.data", "r");
    if (!fp)
    {
        printf("File not found\n");
        return;
    }

    char line[300];
    while (fgets(line, sizeof(line), fp))
    {
        HashEntry e;
        char *token;

        token = strtok(line, "#");
        strcpy(e.name, token);
        token = strtok(NULL, "#");
        e.id = atoi(token);
        token = strtok(NULL, "#");
        strcpy(e.major, token);
        token = strtok(NULL, "#");
        strcpy(e.courseId, token);
        token = strtok(NULL, "#");
        strcpy(e.courseTitle, token);
        token = strtok(NULL, "#");
        e.creditHours = atoi(token);
        token = strtok(NULL, "\n");
        strcpy(e.semester, token);

        e.status = OCCUPIED;
        insertHash(e);
    }

    fclose(fp);
}

void saveHashToFile()
{
    FILE *fp = fopen("reg.txt", "w");
    if (!fp)
    {
        printf("File not found\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (hashTable[i].status == OCCUPIED)
        {
            fprintf(fp, "%s#%d#%s#%s#%s#%d#%s\n",
                    hashTable[i].name,
                    hashTable[i].id,
                    hashTable[i].major,
                    hashTable[i].courseId,
                    hashTable[i].courseTitle,
                    hashTable[i].creditHours,
                    hashTable[i].semester);
        }
    }

    fclose(fp);
    printf("Hash table saved to reg.txt\n");
}
void insertHashFromUser()
{
    HashEntry e;

    printf("Student name: ");
    gets(e.name);

    printf("Student ID: ");
    scanf("%d", &e.id);
    getchar();

    printf("Major: ");
    gets(e.major);

    printf("Course ID: ");
    scanf("%s", e.courseId);
    getchar();

    printf("Course Title: ");
    gets(e.courseTitle);

    printf("Credit Hours: ");
    scanf("%d", &e.creditHours);
    getchar();

    printf("Semester: ");
    gets(e.semester);

    e.status = OCCUPIED;

        // Check for duplicate record
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (hashTable[i].status == OCCUPIED &&hashTable[i].id == e.id &&!strcasecmp(hashTable[i].courseId, e.courseId) &&!strcasecmp(hashTable[i].semester, e.semester))
        {
            printf("Record already exists for this student and semester\n");
            return;
        }
    }

    if (insertHash(e))
        printf("Record inserted successfully\n");
    else
        printf("Hash table is full\n");
}

void hashMenu()
{
    int choice;
    char name[100];
    int initialized = 0;

    do
    {
        if (!initialized)
        {
            initHashTable();
            loadHashFromFile();
            initialized = 1;
        }

        printf("\n--- Hash Table Menu ---\n");
        printf("1. Print hash table\n");
        printf("2. Print table size\n");
        printf("3. Print hash function\n");
        printf("4. Insert new record\n");
        printf("5. Search for student\n");
        printf("6. Delete record\n");
        printf("7. Save hash table to reg.txt\n");
        printf("8. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            printHashTable();
            break;

        case 2:
            printHashSize();
            break;

        case 3:
            printHashFunction();
            break;

        case 4:
            insertHashFromUser();
            break;

        case 5:

{
    char name[100];
    int option;

    printf("Student name: ");
    gets(name);

    int pos = searchHash(name);

    printf("1. Check if student exists\n");
    printf("2. Show student information\n");
    printf("Choice: ");
    scanf("%d", &option);
    getchar();

    if (option == 1)
    {
        if (pos == -1)
            printf("Student not found\n");
        else
            printf("Student found\n");
    }
    else if (option == 2)
    {
        if (pos == -1)
        {
            printf("Student not found\n");
        }
        else
        {
            printf("Student found at index %d\n", pos);
            printf("%s %d %s %s %s %d %s\n",hashTable[pos].name,hashTable[pos].id,hashTable[pos].major,hashTable[pos].courseId,hashTable[pos].courseTitle,hashTable[pos].creditHours, hashTable[pos].semester);
        }
    }
    break;
}


        case 6:
            printf("Student name: ");
            gets(name);
            deleteFromHash(name);
            break;

        case 7:
            saveHashToFile();
            break;
        }
    }
    while (choice != 8);
}

int main()
{
    int choice;

    do
    {
        printf("\nStudent Course Registration System\n");
        printf("1. AVL Tree Part\n");
        printf("2. Hash Table Part\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            avlMenu();
        else if (choice == 2)
            hashMenu();

    }
    while (choice != 3);

    return 0;
}

