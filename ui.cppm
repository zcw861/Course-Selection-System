// Module
// File: ui.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-18 18:57:57
// Description:
// a UI class


export module cs:ui;

import std;
import :controller;
import :database;
import :entity;

using std::cin;
using std::print;
using std::string;
using std::shared_ptr;
using std::vector;

export class UI
{
public:
    explicit UI();
    //相当于main函数吧
    int exec();

private:
    //第一层：主面板
    void showMainMenu();
    //第二层：学生面板
    void showStudentMenu();
    //第二层：教师面板
    void showTeacherMenu();
    //地二层：秘书面板
    void secretaryMenu();

    //其实我感觉就目前而言根本不需要下面这三个函数

    //展示信息？？？？
    //void displayMessage(string msg);
    //输入账户ID？？？？
    //string inputAccountId();
    //输入课程ID？？？？
    //string inputCourseId();


    EnrollmentController enrollmentController;
    CourseController courseController;
    ScoreController scoreController;


};

static vector<shared_ptr<Student>>& students() {
    static vector<shared_ptr<Student>> v;
    return v;
}

static vector<shared_ptr<Teacher>>& teachers() {
    static vector<shared_ptr<Teacher>> v;
    return v;
}

static vector<shared_ptr<Course>>& courses() {
    static vector<shared_ptr<Course>> v;
    return v;
}

static vector<shared_ptr<Secretary>>& secretaries() {
    static vector<shared_ptr<Secretary>> v;
    return v;
}

static vector<shared_ptr<Score>>& scores() {
    static vector<shared_ptr<Score>> v;
    return v;
}



UI::UI()
    : enrollmentController(
        students(),
        teachers(),
        courses()
    ),
      courseController(
        courses(),
        teachers(),
        secretaries()
    ),
      scoreController(
        students(),
        teachers(),
        secretaries(),
        scores()
    )
{}


int UI::exec()
{
    while(1)
    {
        //进入主面板
        showMainMenu();

        char choice = ' ';
        //input
        cin >> choice;

        switch(choice){
            case '1':
                showStudentMenu();
                break;
            case '2':
                showTeacherMenu();
                break;
            case '3':
                secretaryMenu();
                break;
            case '4':
                return 0;
            default:
                print("错误：无效选项！\n");
                break;
        }
    }

    print("\n操作结束！谢谢使用！\n");

}

void UI::showMainMenu()
{
    //output
    print("欢迎使用！\n");
    print("=============== 课程管理系统 ===============\n");
    print("请选择模式类型：\n");
    print("1: 学生\n 2: 教师\n 3: 教学秘书:   \n4: 退出\n");
    print("选择:");
}

void UI::showStudentMenu()
{
    //先确定好学生对象
    print("请输入学号: ");
    string sid;
    cin >> sid;

    //创建对象
    shared_ptr<Student> student;
    //没有
    if (!StudentDatabase::singleton().findStudentById(sid, student))
    {
        print("错误：该学生不存在！\n");
        return; //这里是回到主菜单了
    }

    //确认成功
    char choice = ' ';
    print("欢迎！{}同学 \n", student->infoName());

    //output
    while (1)
    {
        print("\n===== 学生 =====\n");
        print("请选择服务：\n");
        print("1: 查询个人课表\n 2: 退课\n 3: 选课\n 4: 查看成绩单\n 0: 返回主菜单\n");
        print("选择：") ;

        cin >> choice;

        switch(choice){
        //查询课表
            case '1':
            {
                vector<shared_ptr<Course>> courses;
                if (!StudentDatabase::singleton().findSchedule(sid, courses))
                {
                    print("当前课表为空！\n");
                    break;
                }
                for (auto& c: courses)
                    print("{} {}\n", c->infoId(), c->infoName());
                break;
            }
        //退课
            case '2':
            {
                print("请输入想要退掉的课程的ID：");
                string cid;
                cin >> cid;

                if (enrollmentController.dropCourse(sid, cid))
                    print("退课成功！\n");
                else
                    print("退课失败！\n");
                break;
            }

        //选课
            case '3':
            {
                print("请输入想要选择的课程的ID：");
                string cid;
                cin >> cid;

                if (enrollmentController.enrollCourse(sid, cid))
                    print("选课成功！\n");
                else
                    print("选课失败! \n");
                break;
            }
        //查看成绩单
            case '4':
            {
                auto result = scoreController.getStudentTranscript(sid);

                if (result.empty())
                {
                    print("当前无成绩！\n");
                    break;
                }

                print("===== 成绩单 =====");
                for (auto& w: result)
                    //智能指针转化(lock)
                    if (auto s = w.lock())
                        print("{} 成绩：{}\n", s->infoCourse(),s->infoGrade());

                break;
            }
            case '0':
                return;
            default:
                print("错误：无效选项\n");
        }
    }

}


void UI::showTeacherMenu()
{
    //依旧确认身份环节
    print("请输入教师工号: ");
    string tid;
    cin >> tid;

    shared_ptr<Teacher> teacher;
    if (!TeacherDatabase::singleton().findTeacherById(tid, teacher))
    {
        print("错误：该教师不存在！\n");
        return;
    }

    //登陆成功
    print("欢迎！ {} 老师\n", teacher->infoName());


    //output
    print("===== 教师 =====\n");
    print("请选择服务：\n");
    print("1:   查询个人课表\n 2:   创建课程\n 3:   评成绩\n 0: 返回主菜单\n");
    print("选择：");

    char choice = ' ';
    cin >> choice;

    switch(choice){
    //查询课表
        case '1':
        {

            vector<shared_ptr<Course>> courses;
            if (!courseController.getCoursesByTeacher(tid, courses))
            {
                print("当前没有教授课程！\n");
                break;
            }


            if (courses.empty())
            {
                print("当前没有教授课程！\n");
                break;
            }

            for (auto& c: courses)
                print("{} {}\n", c->infoId(), c->infoName());
            break;
        }
    //创建课程
        case '2':
        {
            string name, cid;
            double credit, capacity;

            print("课程名：");
            cin >> name;
            print("课程ID: ");
            cin >> cid;
            print("学分: ");
            cin >> credit;
            print("容量: ");
            cin >> capacity;

            courseController.createCourse(name, cid, credit, capacity);

            //互相绑定
            courseController.attributeTeacher(tid,cid);
            print("课程创建成功！\n");
            break;
        }
    //评分
        case '3':
        {
            string sid, cid;
            double grade;

            print("学生学号: ");
            cin >> sid;
            print("课程ID: ");
            cin >> cid;
            print("成绩: ");
            cin >> grade;

            if (scoreController.gradeStudent(sid, cid, grade))
                print("成绩录入成功！\n");
            else
                print("成绩录入失败！\n");

            break;
        }
    //返回
        case '0':
            return;
        default:
            print("错误：无效选项！\n");
    }
}

void UI::secretaryMenu()
{
    char choice = ' ';
    while (1) {
        print("\n===== 教学秘书 =====\n");
        print("1: 初始化课程数据\n");
        print("0: 返回主菜单\n");
        print("选择：");

        cin >> choice;

        switch (choice) {
            case '1':
                courseController.initializeCourse();
                print("课程初始化完成！\n");
                break;
            case '0':
                return;
            default:
                print("错误：无效选项\n");
        }
    }
}















