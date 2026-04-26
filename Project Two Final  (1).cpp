// Gunjan Patel
// CS-300 Project Two
// Advising Assistance Program

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Course {
public:
    string courseId;
    string courseTitle;
    vector<string> preRequisite;
};

string trim(const string& value) {
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

string toUpperCase(string text) {
    for (char& ch : text) {
        ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    }
    return text;
}

vector<string> splitLine(const string& line) {
    vector<string> parts;
    string part;
    stringstream ss(line);

    while (getline(ss, part, ',')) {
        parts.push_back(trim(part));
    }

    return parts;
}

int findCourseIndex(const vector<Course>& courses, const string& courseNumber) {
    string searchId = toUpperCase(courseNumber);

    for (size_t i = 0; i < courses.size(); ++i) {
        if (toUpperCase(courses[i].courseId) == searchId) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

bool loadVector(const string& fileName, vector<Course>& courses) {
    ifstream file(fileName);
    courses.clear();

    if (!file.is_open()) {
        cout << "Error: could not open file \"" << fileName << "\"." << endl;
        return false;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        ++lineNumber;
        line = trim(line);

        if (line.empty()) {
            continue;
        }

        vector<string> parts = splitLine(line);

        if (parts.size() < 2) {
            cout << "Warning: skipped invalid line " << lineNumber << "." << endl;
            continue;
        }

        Course tempCourse;
        tempCourse.courseId = toUpperCase(parts[0]);
        tempCourse.courseTitle = parts[1];

        bool validPrerequisites = true;
        for (size_t i = 2; i < parts.size(); ++i) {
            if (!parts[i].empty()) {
                tempCourse.preRequisite.push_back(toUpperCase(parts[i]));
            }
        }

        for (size_t i = 0; i < tempCourse.preRequisite.size(); ++i) {
            if (tempCourse.preRequisite[i] == tempCourse.courseId) {
                cout << "Warning: skipped line " << lineNumber
                     << " because a course cannot list itself as a prerequisite." << endl;
                validPrerequisites = false;
                break;
            }
        }

        if (validPrerequisites) {
            courses.push_back(tempCourse);
        }
    }

    file.close();

    for (size_t i = 0; i < courses.size(); ++i) {
        for (size_t j = 0; j < courses[i].preRequisite.size(); ++j) {
            if (findCourseIndex(courses, courses[i].preRequisite[j]) == -1) {
                cout << "Warning: prerequisite " << courses[i].preRequisite[j]
                     << " for course " << courses[i].courseId
                     << " was not found in the file." << endl;
            }
        }
    }

    cout << "Courses have been imported." << endl;
    return true;
}

void printCourseList(vector<Course> courses) {
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseId < b.courseId;
    });

    cout << endl;
    cout << "Here is a sample schedule:" << endl;

    for (const Course& course : courses) {
        cout << course.courseId << ", " << course.courseTitle << endl;
    }
}

void printCourseInformation(const vector<Course>& courses, const string& courseNumber) {
    int index = findCourseIndex(courses, courseNumber);

    if (index == -1) {
        cout << "Course not found." << endl;
        return;
    }

    const Course& course = courses[index];
    cout << endl;
    cout << course.courseId << ", " << course.courseTitle << endl;

    if (course.preRequisite.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.preRequisite.size(); ++i) {
            int preIndex = findCourseIndex(courses, course.preRequisite[i]);

            if (preIndex != -1) {
                cout << courses[preIndex].courseId << ", " << courses[preIndex].courseTitle;
            }
            else {
                cout << course.preRequisite[i];
            }

            if (i < course.preRequisite.size() - 1) {
                cout << "; ";
            }
        }
        cout << endl;
    }
}

int main() {
    vector<Course> courses;
    string fileName;
    string courseNumber;
    int choice = 0;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "That is not a valid option." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter the file name: ";
            cin >> ws;
            getline(cin, fileName);
            dataLoaded = loadVector(fileName, courses);
            break;

        case 2:
            if (!dataLoaded) {
                cout << "Please load the data file first." << endl;
            }
            else {
                printCourseList(courses);
            }
            break;

        case 3:
            if (!dataLoaded) {
                cout << "Please load the data file first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                printCourseInformation(courses, courseNumber);
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}
