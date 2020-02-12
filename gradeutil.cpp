/*gradeutil.cpp*/

//
// <<Gina Gerace>>
// U. of Illinois, Chicago
// CS 341, Spring 2019
// Project #02: GradeUtil API
// 

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "gradeutil.h"

using namespace std;

void Dept::addCourse(Course course)
{
	// Add course to Courses.
	(this->Courses).push_back(course);
}

void College::addDepartment(Dept department)
{
	// Add department to Depts.
	(this->Depts).push_back(department);
}


//
// API:
//

// 
// ParseCourse:
//
// Parses a CSV (comma-separated values) line into a Course
// object, which is then returned.  The given line must have
// the following format:
//
//   Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor
//
// Example:
//   BIOE,101,01,Intro to Bioengineering,22,8,2,1,0,1,0,0,0,5,Eddington
//
// Note the lack of spaces, except perhaps in the title.
// If the given line does not have this format, the behavior 
// of the function is undefined (it may crash, it may throw
// an exception, it may return).
//
Course ParseCourse(string csvline)
{
    stringstream  ss(csvline);
    //parseline:
    string Dept, Num, Section, Title, A, B, C, D, F, I, NR, S, U, W, Instructor;
    
    getline(ss,Dept,',');
    getline(ss,Num,',');
    getline(ss,Section,',');
    getline(ss,Title,',');
    getline(ss,A,','); getline(ss,B,',');  getline(ss,C,','); getline(ss,D,','); getline(ss,F,',');
    getline(ss,I,','); getline(ss,NR,','); getline(ss,S,','); getline(ss,U,','); getline(ss,W,',');
    getline(ss,Instructor,',');
      
    return Course(Dept, Title, stoi(Num), stoi(Section), Instructor, 
           /*A-F*/ stoi(A), stoi(B), stoi(C), stoi(D), stoi(F), 
           /*I,S,U,W,NR*/ stoi(I), stoi(S), stoi(U), stoi(W), stoi(NR));
}


//
// GetDFWRate:
//
// Returns the DFW rate as a percentage for a given course, 
// department, or college.  For a course whose grading type 
// is defined as Course::Letter, the DFW rate is defined as
// 
//   # of D grades + F grades + Withdrawals
//   -------------------------------------- * 100.0
//   # of A, B, C, D, F grades + Withdrawls
//
// The numerator is returned via the reference parameter DFW;
// the denominator is returned via the reference parameter N.
// If the course grading type is not Course::Letter, the DFW
// rate is 0.0, and parameters DFW and N are set to 0.  
//
// When computed for a dept or college, all courses of type 
// Course::Letter are considered in computing an overall DFW 
// rate for the dept or college.  The reference parameters 
// DFW and N are also computed across the dept or college.
//
double GetDFWRate(const Course& c, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  
  if((c.NumA + c.NumB + c.NumC + c.NumD + c.NumF) > 0) {
      DFW = c.NumD + c.NumF + c.NumW;
      N = DFW + c.NumA + c.NumB + c.NumC;
      double DFWrate = 100.00 * DFW / N;
      return DFWrate;  
  }
  
  return 0.0;
}

double GetDFWRate(const Dept& dept, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  
  if(!dept.Courses.empty()){
      for(Course c : dept.Courses){
          if((c.NumA + c.NumB + c.NumC + c.NumD + c.NumF) > 0) {
              DFW += (c.NumD + c.NumF + c.NumW);
              N += (c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW);
          }
      }
  }
    
  if(N == 0 || DFW == 0)
      return 0.0;
  else{
      double DFWrate = 100.00 * DFW / N;
      return DFWrate;
  }
}

double GetDFWRate(const College& college, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  if(!college.Depts.empty()){
      for(Dept d : college.Depts){
          if(!d.Courses.empty()){
              for(Course c : d.Courses){
                  if((c.NumA + c.NumB + c.NumC + c.NumD + c.NumF) > 0) {
                      DFW += (c.NumD + c.NumF + c.NumW);
                      N += (c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW);
                  }
              }
          }
      }
  }
  if(N == 0 || DFW == 0)
      return 0.0;
  else{
      double DFWrate = 100.00 * DFW / N;
      return DFWrate;
  }
}  


//
// GetGradeDistribution
//
// Returns an object containing the grade distribution for a given
// course, dept or college.  For a course whose grading type is
// defined as Course::Letter, the grade distribution is defined by
// the following values:
//
//   N: the # of A, B, C, D, F grades
//   NumA, NumB, NumC, NumD, NumF: # of A, B, C, D, F grades
//   PercentA, PercentB, PercentC, PercentD, PercentF: % of A, B, 
//     C, D, F grades.  Example: PercentA = NumA / N * 100.0
//
// If the course grading type is not Course::Letter, all values
// are 0.  When computed for a dept or college, all courses of
// type Course::Letter are considered in computing an overall 
// grade distribution for the dept or college.
//
GradeStats GetGradeDistribution(const Course& c)
{
  
  int N = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
  if (N <= 0)
      return GradeStats();
  else{
      double A = 100.00 * c.NumA / N;
      double B = 100.00 * c.NumB / N;
      double C = 100.00 * c.NumC / N;
      double D = 100.00 * c.NumD / N;
      double F = 100.00 * c.NumF / N;
      return GradeStats(N, c.NumA, c.NumB, c.NumC, c.NumD, c.NumF, A, B, C, D, F);  
  }
}

GradeStats GetGradeDistribution(const Dept& dept)
{
   int N = 0;
   int n = 0;
   int A = 0;
   int B = 0;
   int C = 0;
   int D = 0;
   int F = 0;
  if(!dept.Courses.empty()){
      for(Course c : dept.Courses){
          n = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
          if (n > 0){
              A += c.NumA;
              B += c.NumB;
              C += c.NumC;
              D += c.NumD;
              F += c.NumF;    
          }
      }
      N = A + B + C + D + F;
      if(N > 0){
          double pA = 100.00 * A / N;
          double pB = 100.00 * B / N;
          double pC = 100.00 * C / N;
          double pD = 100.00 * D / N;
          double pF = 100.00 * F / N;
          return GradeStats(N, A, B, C, D, F, pA, pB, pC, pD, pF); 
      }
  }
  
  return GradeStats();
}

GradeStats GetGradeDistribution(const College& college)
{
  int N = 0;
  int n = 0;
  int A = 0;
  int B = 0;
  int C = 0;
  int D = 0;
  int F = 0;
  if(!college.Depts.empty()) {    
     for(Dept dept : college.Depts){
          if(!dept.Courses.empty()){
              for(Course c : dept.Courses){
                  n = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
                  if (n > 0){
                      A += c.NumA;
                      B += c.NumB;
                      C += c.NumC;
                      D += c.NumD;
                      F += c.NumF;    
                  }
              }
          }
     }
      N = A + B + C + D + F;
      if(N > 0){
          double pA = 100.00 * A / N;
          double pB = 100.00 * B / N;
          double pC = 100.00 * C / N;
          double pD = 100.00 * D / N;
          double pF = 100.00 * F / N;
          return GradeStats(N, A, B, C, D, F, pA, pB, pC, pD, pF); 
      }
  }
  return GradeStats();
}


//
// FindCourses(dept, courseNumber)
//
// Searches the courses in the department for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, and returned in
// ascending order by section number.
//
vector<Course> FindCourses(const Dept& dept, int courseNumber)
{
  vector<Course>  courses;
  if(!dept.Courses.empty()){ 
     for(Course c : dept.Courses){
          if(c.Number == courseNumber)
              courses.push_back(c);
      }
  }
  if(!courses.empty()){
      std::sort(courses.begin(), courses.end(),
                [&](Course a,Course b){
                    if(a.Section < b.Section)
                        return true;
                    else
                        return false;
                });
  }
  return courses;
}


//
// FindCourses(dept, instructorPrefix)
//
// Searches the courses in the department for those whose 
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".
//
// If none are found, then the returned vector is empty.  If 
// one or more courses are found, copies of the course objects
// are returned in a vector, with the courses appearing in 
// ascending order by course number.  If two courses have the 
// same course number, they are given in ascending order by 
// section number.  Note that courses are NOT sorted by instructor
// name.
//
vector<Course> FindCourses(const Dept& dept, string instructorPrefix)
{
  bool same;
  int index;
  vector<Course>  courses;
  if(!dept.Courses.empty()){ 
     for(Course c : dept.Courses){
         same = true;
         index = 0;
         if(instructorPrefix.length() > c.Instructor.length())
             same = false;
         else {
             for(char& l : instructorPrefix){
                 if(index < c.Instructor.length() && c.Instructor.at(index) != l)
                     same = false;
                 index++;
             }
         }
         if(same == true)
             courses.push_back(c);
      }
  }
  if(!courses.empty()){
      //sort by course number
      std::sort(courses.begin(), courses.end(),
                [&](Course a,Course b){
                    if(a.Number < b.Number)
                        return true;
                    else
                        return false;
                });
      //sort by section number
      std::sort(courses.begin(), courses.end(),
                [&](Course a,Course b){
                    if(a.Number == b.Number){
                        if(a.Section < b.Section)
                            return true;
                        else
                            return false;
                    }
                    else
                        return false;
                });
  }
  return courses;
}


//
// FindCourses(college, courseNumber)
//
// Searches for all courses in the college for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department, then course number,
// and then section number.
//
vector<Course> FindCourses(const College& college, int courseNumber)
{
  vector<Course>  courses;
  if(!college.Depts.empty()){ 
         for(Dept d : college.Depts){
              if(!d.Courses.empty()){ 
                 for(Course c : d.Courses){
                      if(c.Number == courseNumber)
                          courses.push_back(c);
                  }
              }
         }
  }
  if(!courses.empty()){
      //sort by department
      std::sort(courses.begin(), courses.end(),
                [&](Course a,Course b){
                    if(a.Dept < b.Dept)
                        return true;
                    else
                        return false;
                });

      //sort by section number
      std::sort(courses.begin(), courses.end(),
                [&](Course a,Course b){
                    if(a.Dept == b.Dept){
                        if(a.Section < b.Section)
                            return true;
                        else
                            return false;
                    }
                    else
                        return false;
                });
  }
  return courses;
}


//
// FindCourses(college, instructorPrefix)
//
// Searches all the courses in the college for those whose
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department,
// then course number, and then section.
//
vector<Course> FindCourses(const College& college, string instructorPrefix)
{
  bool same;
  int index;
  vector<Course>  courses;

  if(!college.Depts.empty()){ 
     for(Dept dept : college.Depts){
          if(!dept.Courses.empty()){ 
             for(Course c : dept.Courses){
                 same = true;
                 index = 0;
                 if(instructorPrefix.length() > c.Instructor.length())
                     same = false;
                 else {
                     for(char& l : instructorPrefix){
                         if(index < c.Instructor.length() && c.Instructor.at(index) != l)
                             same = false;
                         index++;
                     }
                 }
                 if(same == true)
                     courses.push_back(c);
              }
          }
     }
  }
  if(!courses.empty()){
      //sort by department
      std::sort(courses.begin(), courses.end(),
                [&](Course a,Course b){
                    if(a.Dept < b.Dept)
                        return true;
                    else
                        return false;
                });
      //sort by course number
      std::sort(courses.begin(), courses.end(),
                [&](Course a,Course b){
                    if(a.Dept == b.Dept){
                        if(a.Number < b.Number)
                            return true;
                        else
                            return false;
                    }
                    else
                        return false;
                });
      //sort by section number
      std::sort(courses.begin(), courses.end(),
                [&](Course a,Course b){
                    if(a.Dept == b.Dept && a.Number == b.Number){
                        if(a.Section < b.Section)
                            return true;
                        else
                            return false;
                    }
                    else
                        return false;
                });
  }
  return courses;
}
