所有的manager类都应当是单例的。<br>
自己编写的所有文件与类命名都应当带上“DB”前缀。<br>
需要向控制台输出信息时，请都使用DBLog中的方法。目前有log、print和printLine，接收参数均为std::string。这样做的目的是方便日后的调试和修改。不再输出调试信息时，只需修改log函数的内容。需要修改输出方式，如cout转为printf，也只需修改print和printLine两个函数。<br>
<br>
<br>
DATA/DATA_BASE_NAME/TABLE_NAME/TABLE <br>
每个文件夹下有一个description文件夹，DATA_BASE_NAME层用于描述数据库的名字，TABLE_NAME层用于描述表的名字，TABLE层用于描述表的具体细节。 <br>
文件夹包含的内容 <br>
data <br>
description(该文件大小为一个page) <br>
usage(每两个byte描述data的每个页面剩余字节数) <br>
indexed{INDEX_NAME}(TODO用于描述索引) <br>
data的详细表示方法：<br>
[2 bytes INDEX_LENGTH][INDEX](这么做的目的在于支持varchar)<br>
description的详细表示方法：<br>
[2 bytes total length], [2 bytes number of data]<br>
{[1 bytes data type], [2 bytes data length], [2 bytes offset in data page], [2 bytes length of name of data], [name of data]} * NUMBER_OF_DATA<br>
第0个index是rid，留下的接口是DBFileManager::insertData, DBFileManager::deleteData, DBFilemanager::getDataByKey这三个函数用于插入删除查找数据。<br>
TODO:<br>
重构代码，分出IndexManager模块。<br>
处理内存溢出
