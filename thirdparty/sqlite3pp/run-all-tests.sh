#!/bin/sh

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Contributor: huangminzhong2 <huangminzhong2@huawei.com>
# Maintainer: huangminzhong2 <huangminzhong2@huawei.com>

total_tests=10
passed_tests=0

# 检查是否提供了一个参数（armeabi-v7a或arm64-v8a）
if [ "$#" -ne 1 ]; then
    echo "error, please provide a parameter."
    exit 1 
fi
arch=$1

# 执行以下测试程序，需要在临时数据库foods.db中创建episodes和foods表
# 在临时数据库test.db创建contacts表，因此在执行以下测试程序前，先创建需要的表

# 连接到foods.db数据库并执行创建foods表，并插入一条数据的命令 
echo "CREATE TABLE foods (id INT PRIMARY KEY,name TEXT NOT NULL,type_id INTEGER NOT NULL);" | /data/tpc_c_cplusplus/lycium/usr/sqlite/${arch}/bin/sqlite3 foods.db
if [ $? -eq 0 ]; then  
    echo "foods create success"  
else  
    echo "foods create fail"  
fi

echo "INSERT INTO foods (id, name, type_id) VALUES (1, 'AAAA', 1234);" | /data/tpc_c_cplusplus/lycium/usr/sqlite/${arch}/bin/sqlite3 foods.db
if [ $? -eq 0 ]; then  
    echo "insert into foods success"  
else  
    echo "insert into foods fail"  
fi

# 连接到foods.db数据库并执行创建episodes表的命令  
echo "CREATE TABLE episodes (id INTEGER PRIMARY KEY,contact_id INTEGER,title TEXT,description TEXT,FOREIGN KEY (contact_id) REFERENCES contacts(id));" | /data/tpc_c_cplusplus/lycium/usr/sqlite/${arch}/bin/sqlite3 foods.db  
   
if [ $? -eq 0 ]; then
    echo "episodes create success"  
else
    echo "episodes create fail"
fi
  
# 连接到test.db数据库并执行创建contacts表的命令  
echo "CREATE TABLE contacts (id INTEGER PRIMARY KEY,name TEXT NOT NULL,phone TEXT NOT NULL,address TEXT,UNIQUE(name, phone));" | /data/tpc_c_cplusplus/lycium/usr/sqlite/${arch}/bin/sqlite3 test.db
 
if [ $? -eq 0 ]; then  
    echo "contacts create success"  
else  
    echo "contacts create fail"  
fi

./testaggregate
res=$?
if [ $res -ne 0 ]
then
    echo "1/10 testaggregate .......................... failed"
else
    echo "1/10 testaggregate .......................... passed"
  ((passed_tests++))
fi

./testattach
res=$?
if [ $res -ne 0 ]
then
    echo "2/10 testattach .......................... failed"
else
    echo "2/10 testattach .......................... passed"
    ((passed_tests++))
fi

./testbackup
res=$?
if [ $res -ne 0 ]
then
    echo "3/10 testbackup .......................... failed"
else
    echo "3/10 testbackup .......................... passed"
    ((passed_tests++))
fi

./testcallback
res=$?
if [ $res -ne 0 ]
then
    echo "4/10 testcallback .......................... failed"
else
    echo "4/10 testcallback .......................... passed"
    ((passed_tests++))
fi

./testdb
res=$?
if [ $res -ne 0 ]
then
    echo "5/10 testdb .......................... failed"
else
    echo "5/10 testdb .......................... passed"
    ((passed_tests++))
fi

./testdisconnect 
res=$?
if [ $res -ne 0 ]
then
    echo "6/10 testdisconnect .......................... failed"
else
    echo "6/10 testdisconnect .......................... passed"
    ((passed_tests++))
fi

./testfunction
res=$?
if [ $res -ne 0 ]
then
    echo "7/10 testfunction .......................... failed"
else
    echo "7/10 testfunction .......................... passed"
    ((passed_tests++))
fi

./testinsert
res=$?
if [ $res -ne 0 ]
then
    echo "8/10 testinsert .......................... failed"
else
    echo "8/10 testinsert .......................... passed"
    ((passed_tests++))
fi

./testinsertall 
res=$?
if [ $res -ne 0 ]
then
    echo "9/10 testinsertall .......................... failed"
else
    echo "9/10 testinsertall .......................... passed"
    ((passed_tests++))
fi

./testselect
res=$?
if [ $res -ne 0 ]
then
    echo "10/10 testselect .......................... failed"
else
    echo "10/10 testselect .......................... passed"
    ((passed_tests++))
fi

test_fail=$((total_tests - passed_tests)) 
if [ $passed_tests -ne 10 ]
then
    echo "$passed_tests tests passed, $test_fail tests failed"
else
    echo "100% tests passed, 0 tests failed out of 10"
fi
