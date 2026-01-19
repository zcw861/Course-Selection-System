# Course-Selection-System

version 0.1.0 JiangFan
    基本实现了实体类的功能实现，但教师类和秘书类的部分实现还没有完成（要考虑的还有很多）。当然，最重要的是，上一层的模块文件调用时，需要修改这些实体类的实现的位置，以及import的模块，不然会有循环依赖的问题。建议先解决循环依赖问题，不然代码其他逻辑报错根本看不到（已经利用AI找到一些bug了）。
    最后注意：由于这些类有些是shared_ptr有些是weak_ptr,请合理使用lock函数，实体类中使用了该函数的地方基本标注了 //转化

version 0.1.1 JiangFan
    修复了实体类的循环依赖问题，并解决了已出现的报错。
    修改了对模块的命名和模块文件的分类
    将readme.txt内容放到了readme.md（之前没看到）
    将实体类都放在了单独的文件中。

version 0.1.2 JiangFan
    实体类功能全部完成，增加了一些函数接口，Course的修改学分时间地点的函数(modifyCTL)、老师注册课程

version 0.1.3 JiangFan
    修改了部分实体类的接口极其功能实现，主要是老师教授课程、教学秘书排课。且把Course的modifyCTL、函数修改为modifyTL,只修改时间地点。修改了类图(实体类部分)。
