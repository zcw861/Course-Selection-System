// Module
// File: entity.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-16 14:56:46
// Description:
//  为了解决函数的循环依赖的问题，因此采取此等方法。

export module cs:entity;

export import :entity.course;
export import :entity.person;
export import :entity.score;
export import :entity.secretary;
export import :entity.student;
export import :entity.teacher;


/*********************Course module 的实现****************************/

//选课操作
bool Course::acceptEnrollment(shared_ptr<Student> student)
{
    //不用判空，上层会做

    //已经在该课程内
    for (auto& s: _student){
        auto s_ptr = s.lock(); // 这里需要将weak_ptr转换为shared_ptr才能访问成员函数  -m-
        if (s_ptr && s_ptr->hasId(student->infoId()))
        {
            print("你已经在该课程中，无需再选！\n");
            return false;
        }
    }

    //看课程容量
    if (_student.size() >= m_capacity)
    {
        print("课程名额已满，选课失败！\n");
        return false;
    }

    //选课
    _student.push_back(student);
    student->enrollsIn(shared_from_this());
    print("{}\n 选课成功，目前选择该课程的人数: {}\n", student->infoName(), _student.size());
    return true;
}

//退课操作
bool Course::acceptWithdraw(shared_ptr<Student> student)
{
    //不用判空，上层会做

    //找到该学生
    auto s = std::find_if(_student.begin(), _student.end(),[&student](const weak_ptr<Student>& s)
    {
        auto s_ptr = s.lock();
        return s_ptr && s_ptr->hasId(student->infoId());
    });

    if (s != _student.end())
    {
        _student.erase(s);
        print("{}, 退课成功！\n", student->infoName());
        return true;
    }

    print("{}, 退课失败！你没有选这门 {} 课程\n", student->infoName(), m_courseName);
    return false;
}



/*********************score module 的实现****************************/

Score::Score(weak_ptr<Course> course, weak_ptr<Student> student, double grade)
    : _course(course), _student(student), m_grade(grade)
{}

string Score::infoCourse()
{
    //return std::format("{}", _course->infoId());
    //转化
    auto c_ptr = _course.lock();
       string courseId = c_ptr ? c_ptr->infoId() : "课程已销毁";

    return std::format("{}", courseId);
}

string Score::infoStudentId()
{
    //return std::format("{}", _student->infoId());
    //转化
    auto s_ptr = _student.lock();
       string studentId = s_ptr ? s_ptr->infoId() : "学生已销毁";

    return std::format("{}", studentId);
}

//给课程添加老师
bool Course::addTeacher(shared_ptr<Teacher> teacher)
{
    if (!teacher)
        return false;

    //已经在课程中
    for (auto& t: _teacher)
    {
        //转化
        auto t_ptr = t.lock();
        if (t_ptr && t_ptr->infoId() == teacher->infoId())
        {
            print("错误：{}老师已经有{}这个课程了，无需重复添加！\n", teacher->infoName(), this->infoName());
            return false;
        }
    }

    //添加到课程中
    _teacher.push_back(teacher);
    return true;
}


/*********************student module 的实现****************************/

void Student::enrollsIn(shared_ptr<Course> course)
{
    //不用判空，上层会做
    if (course->acceptEnrollment(shared_from_this()))
        _course.push_back(course);
}

//退课
bool Student::dropCourse(shared_ptr<Course> course)
{
    //不用判空，上层会做
        //找到该课程
    auto c = std::find_if(_course.begin(), _course.end(),[course](const weak_ptr<Course>& c) //
    {
        auto c_ptr = c.lock();
        return c_ptr && c_ptr->infoId() == course->infoId();
    }); //c 是weak_ptr<Course>，需lock()判空后调用成员函数


    if (course->acceptWithdraw(shared_from_this())) //转化
    {
        if (c != _course.end())
        {
            _course.erase(c);
            return true;
        }

        return false;
    }

    return false;
}

//输出打印成绩单
void Student::checkResult()
{
    print("\n+-----------成绩单-----------+\n");
    print("     姓名：{}  学号：{}\n", infoName(), infoId());

    for (auto sc: _transcript)
    {
        //course name and score
        //转化
        auto sc_ptr = sc.lock();
        if (sc_ptr)
            print("{}:  {}\n", sc_ptr->infoCourse(), sc_ptr->infoGrade());

        print("+---------------------------+\n");
    }
}

void Student::schedule()
{
    print("\n+-----------课程表-----------+\n");
    print("     姓名：{}  学号：{}\n", m_pname, m_pid);

    for (auto c: _course)
    {
        //转化
        auto c_ptr = c.lock();
        if (c_ptr)
            print("{}:  {}\n", c_ptr->infoName(),  c_ptr->infoCTL());
    }

    print("+---------------------------+\n");
}

/*********************teacher module 的实现****************************/

void Teacher::markGrade(shared_ptr<Student> student)
{
    //学生存在
    if (!student)
    {
        print("错误：没找到该学生！\n");
        return;
    }
    //教师没有课程
    if (_courses.empty())
    {
        print("错误：您没有负责的课程！\n");
        return;
    }


}

//查看学生的成绩单
void Teacher::checkStudentResult(shared_ptr<Student> student)
{
    //感觉上一层才需要关注是不是空指针,保险点，判断一下
    if (!student)
    {
        print("错误：未找到该学生！");
        return;
    }



}

void Teacher::printStudentRoster(shared_ptr<Course> course)
{
    if (_courses.size() == 0)
    {
        print("该教师没有教授课程！\n");
        return;
    }

    //转化
    for (auto c : _courses)
    {
        auto c_ptr = c.lock();
        if (c_ptr)
            c_ptr->roster();
    }
}

//输出选该课的学生表
void Course::roster()
{
    print("\n课程： {}     课程id： {}        选课人数： {}        最大容量： {}\n"
        , m_courseName, m_courseId, _student.size(), m_capacity);

    print("选课名单：");
    int flag = 0; //换行标志
    for (auto& s: _student)
    {
        //一排放八个
        if (flag % 8 == 0)
            print("\n");
        //
        auto s_ptr = s.lock();  //s是weak_ptr<Student>，需lock()判空后调用infoName()
        if (s_ptr)
            print("{:3}   ", s_ptr->infoName());
        flag++;
    }
}

//注册课程操作
void Teacher::assignCourse(shared_ptr<Course> course)
{
    //上一层会判空，最好是 OvO
    if (!course)
    {
        print("错误：未找到该课程！\n");
        return;
    }

    //成功
    if (course->addTeacher(shared_from_this()))
    {
        print("成功：{}负责教授{}课程。\n", infoName(), course->infoName());
        _courses.push_back(course);
        return;
    }

    print("错误：{}老师添加{}课程失败！\n", infoName(), course->infoName());

}


/*********************secretary module 的实现****************************/


//排课
bool Secretary::schedulingCourse(weak_ptr<Course> course)
{
    //没有
    //转化
    auto c_ptr = course.lock();

    if (!c_ptr)
    {
        print("错误：未找到该课程！\n");
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

    //就算不改，也算排课成功
    if (!isChange) return true;

    // 开始修改时间、地点
    string time = "";
    string location = "";

    print("请输入课程时间(例如：周一 3-4节)");
    cin >> time;
    print("请输入课程地点（如：致用楼105）");
    cin >> location;

    //调用相关函数(专门设置的bool类型)
    return c_ptr->modifyTL(time, location);

}







