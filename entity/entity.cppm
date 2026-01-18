// Module
// File: entity.cppm    Version: 0.1.0  License: AGPLv3
// Created: JiangFan       2026-01-16 14:56:46
// Description:
//  为了避免循环依赖，这里没有引入实体之间的关联

export module cs:entity;

import std;

export import :entity.course;
export import :entity.person;
export import :entity.score;
export import :entity.secretary;
export import :entity.student;
export import :entity.teacher;



using std::print;
using std::string;
using std::weak_ptr;
using std::shared_ptr;
using std::vector;
using std::cin;



bool Course::acceptEnrollment(shared_ptr<Student> student) {
    for (auto& s : _student) {
        auto s_ptr = s.lock();
        if (s_ptr && s_ptr->hasId(student->infoId())) {
            print("你已经在这门课程中了，无需重复选课\n");
            return false;
        }
    }
    if (_student.size() >= static_cast<int>(m_capacity)) { // 确保转 int 为 size_t 以便比较时类型匹配
        print("课程容量已满，选课失败！\n");
        return false;
    }
    _student.push_back(student);
    // **移除一行，防止递归**
    // student->enrollsIn(shared_from_this());
    print("{} 选课成功！目前选该门课程的人数: {}\n", student->infoName(), _student.size());
    return true;
}


bool Course::acceptWithdraw(shared_ptr<Student> student) {
    auto s = std::find_if(_student.begin(), _student.end(), [&](const weak_ptr<Student>& s) {
        auto s_ptr = s.lock();
        return s_ptr && s_ptr->hasId(student->infoId());
    });
    if (s != _student.end()) {
        _student.erase(s);
        print("{}, 退课成功！\n", student->infoName());
        return true;
    }
    print("{}, 退课失败！没有选过 {} 课程\n", student->infoName(), m_courseName);
    return false;
}


void Course::roster() {
    print("\n课程： {}     课程id： {}        选课人数： {}        课程容量 {}\n",m_courseName, m_courseId, _student.size(), m_capacity);
    // print("选课人数");
    int flag = 0;
    for (auto& s : _student) {
        if (flag % 8 == 0) print("\n");
        auto s_ptr = s.lock();
        if (s_ptr) print("{:3}   ", s_ptr->infoName());
        flag++;
    }
}


bool Course::addTeacher(shared_ptr<Teacher> teacher) {
    if (!teacher) return false;
    for (auto& t : _teacher) {
        auto t_ptr = t.lock();
        if (t_ptr && t_ptr->infoId() == teacher->infoId()) {
            print("警告{}老师已经在{}这门课程里了，拒绝重复添加！\n", teacher->infoName(), this->infoName());
            return false;
        }
    }
    _teacher.push_back(teacher);
    return true;
}



/*********************score module 的实现****************************/

string Score::infoCourse()const {
    auto c_ptr = _course.lock();
    string courseId = c_ptr ? c_ptr->infoId() : "课程不存在";
    return std::format("{}", courseId);
}

string Score::infoStudentId() const{
    auto s_ptr = _student.lock();
    string studentId = s_ptr ? s_ptr->infoId() : "学生不存在";
    return std::format("{}", studentId);
}


/*********************student module 的实现****************************/

void Student::enrollsIn(shared_ptr<Course> course)
{
     for (auto& c : _course) {
         auto c_ptr = c.lock();
         if (c_ptr && c_ptr->infoId() == course->infoId()) {
             print("你已经选过这门课程了！\n");
            return; // 防止重复选课
         }
     }

    // 直接添加课程到学生的课程列表然后再调用 course->acceptEnrollment
    _course.push_back(course);
}

//退课
bool Student::dropCourse(shared_ptr<Course> course)
{
    // 1. 先在自己列表中查找是否有此课程
    auto c_it = std::find_if(_course.begin(), _course.end(), [&course](const weak_ptr<Course>& c) {
        auto c_ptr = c.lock();
        return c_ptr && c_ptr->infoId() == course->infoId();
    });

    if (c_it == _course.end()) {
        // 学生自己列表中没有这门课
        print("警告：{} 试图退课 {}，但这门课程不在其退课列表中！\n", infoName(), course->infoName());
        // 继续执行，因为可能课程那边已把学生移除，而学生这边没移除，数据不一致，
        course->acceptWithdraw(shared_from_this()); // 即使失败也无妨
        return false; // 自己操作失败，认为整体失败
    }

    // 2. 知道自己列表中有，在课程那边也尝试移除自己
    bool courseRemovedStudent = course->acceptWithdraw(shared_from_this());

    if (courseRemovedStudent) {
        // 3. 课程那边也成功了，才从自己的列表中移除
        _course.erase(c_it); // 使用之前找到的有效迭代器
        return true; // 双方都成功
    } else {
        // 4. 课程那边失败了，比如课程说人不够不能退等，导致自己列表移除了，但课程那边没移除，
        // 导致数据不一致！，下次选什么课可能导致自己列表中存在但课程列表中没有
        print("警告：{} 试图退课 {}，但课程端未找到对应学生进行移除！\n", infoName(), course->infoName());
        // 返回 false，因为退课操作并没有真正完成
        return false;
    }
    // (下面这行不会被执行，因为上面已经 return 了)
}

//打印成绩单
void Student::checkResult()
{
    print("\n+-----------成绩单-----------+\n");
    print("     姓名：{}  学号：{}\n", infoName(), infoId());

    for (auto sc: _transcript) // sc 是 weak_ptr<Score>
    {
        auto sc_ptr = sc.lock();
        if (sc_ptr) {
            // 只有 Score 对象有效时才打印信息
            print("{}:  {}\n", sc_ptr->infoCourse(), sc_ptr->infoGrade());
        }
        // 恢复或考虑获取逻辑，否则怎么展示
        // 如果每条记录后加分隔符，即使无效记录也有，
        // print("+---------------------------+\n");

        // 如果只在有效记录后加分隔符，
        // if (sc_ptr) {
        //     print("+---------------------------+\n");
        // }
    }
    // 如果中间记录统一，最后统一一个结尾分隔符
    print("+---------------------------+\n");
}

void Student::schedule()
{
    print("\n+-----------课程表-----------+\n");
    print("     姓名：{}  学号：{}\n", m_pname, m_pid);

    for (auto c: _course)
    {
        //转换
        auto c_ptr = c.lock();
        if (c_ptr)
            print("{}:  {}\n", c_ptr->infoName(),  c_ptr->infoCTL());
    }

    print("+---------------------------+\n");
}

/*********************teacher module 的实现****************************/

void Teacher::markGrade(shared_ptr<Student> student)
{
    //学生不存在
    if (!student)
    {
        print("找不到该学生\n");
        return;
    }
    //老师没有课程
    if (_courses.empty())
    {
        print("该老师没有开设任何课程\n");
        return;
    }


}

//查看学生的成绩单
void Teacher::checkStudentResult(shared_ptr<Student> student)
{
    //绝大多数情况是要确保不能为空,收到点，判断一下
    if (!student)
    {
        print("找不到该学生");
        return;
    }



}

void Teacher::printStudentRoster(shared_ptr<Course> course)
{
    if (_courses.size() == 0)
    {
        print("该老师没有开设课程\n");
        return;
    }

    //转换
    for (auto c : _courses)
    {
        auto c_ptr = c.lock();
        if (c_ptr)
            c_ptr->roster();
    }
}

//注册课程操作
void Teacher::assignCourse(shared_ptr<Course> course)
{
    //第一种为空，那就是 OvO
    if (!course)
    {
        print("找不到该课程\n");
        return;
    }

    //成功
    if (course->addTeacher(shared_from_this()))
    {
        print("成功将{}老师安排到{}课程。\n", infoName(), course->infoName());
        _courses.push_back(course);
        return;
    }

    print("让{}老师加入{}课程失败！\n", infoName(), course->infoName());

}


/*********************secretary module 的实现****************************/


//排课
bool Secretary::schedulingCourse(weak_ptr<Course> course)
{
    //没有
    //转换
    auto c_ptr = course.lock();

    if (!c_ptr)
    {
        print("找不到该课程\n");
        return false;
    }

    //开始排课

    //显示当前的课程信息
    print("课程: {}, ID: {}, {}\n", c_ptr->infoName(), c_ptr->infoId(), c_ptr->infoCTL());
    print("是否需要修改？(Y/N): ");

    bool isChange = false;
    char temp = ' ';

    while (1)
    {
        cin >> temp;
        if (temp == 'Y' || temp == 'y')
        {
            isChange = true;
            break;
        }
        else if (temp == 'N' || temp == 'n')
            break;
        else
            print("请输入正确的选项（Y/N）！\n");
    }

    //如果没改，也算排课成功
    if (!isChange) return true;

    // 开始修改时间、地点
    string time = "";
    string location = "";

    print("请输入课程时间(例如：周一 3-4节)");
    cin >> time;
    print("请输入课程地点（例如：教学楼105）");
    cin >> location;

    //调用函数(专门返回bool类型的)
    return c_ptr->modifyTL(time, location);

}


