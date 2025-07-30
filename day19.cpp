#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Student {
private:
    int rollNo;
    string name;
    string department;
    float cgpa;

public:
    // Constructor
    Student(int r = 0, string n = "", string d = "", float c = 0.0) {
        rollNo = r;
        name = n;
        department = d;
        cgpa = c;
    }

    // Get student data from user
    void input() {
        cout << "Enter Roll No: ";
        cin >> rollNo;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Department: ";
        getline(cin, department);
        cout << "Enter CGPA: ";
        cin >> cgpa;
    }

    // Display student info
    void display() const {
        cout << "Roll No: " << rollNo
             << " | Name: " << name
             << " | Department: " << department
             << " | CGPA: " << cgpa << endl;
    }

    // Getters
    int getRollNo() const { return rollNo; }
    string getName() const { return name; }
    string getDepartment() const { return department; }
    float getCGPA() const { return cgpa; }

    // Serialize to file
    void writeToFile(ofstream &out) const {
        out << rollNo << "," << name << "," << department << "," << cgpa << "\n";
    }

    // Deserialize from file
    static Student fromString(const string &line) {
        Student s;
        size_t pos = 0, start = 0;
        int field = 0;
        string token;

        vector<string> tokens;
        while ((pos = line.find(',', start)) != string::npos) {
            tokens.push_back(line.substr(start, pos - start));
            start = pos + 1;
        }
        tokens.push_back(line.substr(start));

        if (tokens.size() == 4) {
            s.rollNo = stoi(tokens[0]);
            s.name = tokens[1];
            s.department = tokens[2];
            s.cgpa = stof(tokens[3]);
        }
        return s;
    }
};

// Function to load all students from file
vector<Student> loadStudents() {
    vector<Student> students;
    ifstream file("students.txt");
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            students.push_back(Student::fromString(line));
    }
    return students;
}

// Function to save all students to file
void saveStudents(const vector<Student> &students) {
    ofstream file("students.txt");
    for (const auto &s : students)
        s.writeToFile(file);
}

// CRUD Operations
void addStudent() {
    vector<Student> students = loadStudents();
    Student s;
    s.input();
    students.push_back(s);
    saveStudents(students);
    cout << "Student added successfully!\n";
}

void viewStudents() {
    vector<Student> students = loadStudents();
    if (students.empty()) {
        cout << "No records found.\n";
        return;
    }
    for (const auto &s : students)
        s.display();
}

void searchStudent() {
    vector<Student> students = loadStudents();
    int roll;
    cout << "Enter Roll No to search: ";
    cin >> roll;
    bool found = false;
    for (const auto &s : students) {
        if (s.getRollNo() == roll) {
            s.display();
            found = true;
            break;
        }
    }
    if (!found) cout << "Student not found!\n";
}

void updateStudent() {
    vector<Student> students = loadStudents();
    int roll;
    cout << "Enter Roll No to update: ";
    cin >> roll;
    bool found = false;
    for (auto &s : students) {
        if (s.getRollNo() == roll) {
            cout << "Enter new details:\n";
            s.input();
            found = true;
            break;
        }
    }
    if (found) {
        saveStudents(students);
        cout << "Record updated successfully!\n";
    } else {
        cout << "Student not found!\n";
    }
}

void deleteStudent() {
    vector<Student> students = loadStudents();
    int roll;
    cout << "Enter Roll No to delete: ";
    cin >> roll;
    bool found = false;

    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->getRollNo() == roll) {
            students.erase(it);
            found = true;
            break;
        }
    }
    if (found) {
        saveStudents(students);
        cout << "Record deleted successfully!\n";
    } else {
        cout << "Student not found!\n";
    }
}

int main() {
    int choice;
    do {
        cout << "\n--- Student Record CRUD System ---\n";
        cout << "1. Add Student\n2. View All Students\n3. Search Student\n";
        cout << "4. Update Student\n5. Delete Student\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
