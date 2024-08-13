## NotJSON库
### 介绍
NotJSON是一个以解析速度为最高导向的JSON解析库，并且使用tag + string类型擦除达到效率最大化
后续有新思路还会更新更多解析函数
### 用法
#### 添加
##### 添加普通类型
普通类型有：
	1.字符串
	2.双浮点数
	3.64位整型数
	4.空object
	5.空array
使用add_val_aschild(父节点key，节点key，节点value，节点所在层)来构造普通节点
使用add_val_atback(节点key，节点value)来默认添加到第1层
##### 添加列表类型
列表类型有：
	1.非空object
	2.非空array
使用add_dimension/pairlist_aschild(父节点key，节点key，节点value，节点所在层)来构造array/object节点
使用add_dimension/pairlist_atchild(节点key，节点value)来默认添加到第1层
#### 更改
通过set_xx(节点key，更改内容，节点层数)来更改类型