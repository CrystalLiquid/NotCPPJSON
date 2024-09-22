## NotJSON库
### 介绍
NotCPPJSON是一个header only的以解析速度为最高导向的JSON解析库， \
并且使用tag + string类型擦除+BIT树达到内存和解析效率最大化 \
后续有新思路还会更新更多解析函数，例如手动构建栈迭代解析
### 用法
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
使用add_val_aschild(父节点key，节点key，节点value，节点所在层)来构造普通节点 \
使用add_val_atback(节点key，节点value)来默认添加到第1层
##### 添加列表类型
列表类型有：\
	1.非空object \
	2.非空array	\
使用add_dimension/pairlist_atchild(节点key，节点value)来默认添加到第1层 \
使用set_dimension/pairlist_aschild(父节点key，节点key，节点value，节点所在层)来添加一个array/object父节点和多个子节点
#### 更改
通过set_xx(节点key，更改内容，节点层数)来更改类型 \
使用set_dimension/pairlist_aschild(父节点key，节点key，节点value，节点所在层)来改变一个节点为array/object节点