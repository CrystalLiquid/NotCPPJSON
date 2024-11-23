## NotJSON库
### 介绍
NotCPPJSON是一个header only的以解析速度为最高导向的JSON解析库， \
并且使用tag + string类型擦除+BIT树达到内存和解析效率最大化 \
注意，本库仅支持解析compactJSON，请您不要使用自己的读取逻辑，HP_IO内json_file已经 \
写有高性能读取+转换的函数，并且还有独立的formatJSON 到 compactJSON的转换函数 \
后续有新思路还会更新更多解析函数，例如手动构建栈迭代解析
### api规范
本库所有初级api均只会通过函数调用（通过下标参数来搜索 删除，分情况的函数获取下标），而非运算符重载
只有高级api（比如仅使用层和key来获取 搜索 删除 添加 更改）才会用运算符重载
#### 解析
通过调用json_map.parse(json)传入储存标准JSON内容的字符串完成解析
#### 序列化
通过调用json_map.serialize()来获得一个字符串返回值
#### 获得值
json.get_val()通过tag得知当前的类型，并且合理地返回一个合法的类型 \
如果您需要直接打印，可以通过json.value获得字符串，因为本库正是通过字符串存储的
#### 添加
##### 添加新的树
您需要手动构造一个root空节点来保证您的JSON是标准JSON文件
##### 添加普通类型
普通类型有：\
	1.字符串 \
	2.双浮点数 \
	3.64位整型数 \
	4.空object \
	5.空array \
使用`[节点key][节点value]`来默认添加到第1层 \
使用`[父节点index](节点key,节点value)`来添加到某个列表下 
##### 添加列表类型
列表类型有：\
	1.非空object \
	2.非空array	\
使用`[节点key](json{}(节点key,节点value))`来默认添加到第1层 \
使用`[父节点index](json{}(节点key,节点value))`多个子节点到父列表
#### Find API
本JSON库通过FindAPI来搜索节点，以此避免需要输入层数 \
通过find函数得到列表 \
例子map.find("list")[0].print_info(); \
find列表有以下api： \
##### 快速查看
print_info() \
print_typeof() \
##### 输出类型的enum数字
typeof_which() \
##### 删除
delete_which() \
##### 访问子列表
list_of()[n] \
##### 更改
暂定为set_val(...)
正在施工中。。。。。